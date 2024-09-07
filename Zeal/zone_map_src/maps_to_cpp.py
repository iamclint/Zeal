"""Script for converting a directory of EQ Map files to a .h and .cpp file.

Expected input: Directory with a map_files subdirectory where each file has a zone
short name as the basename with a suffix of .txt, _1.txt, _2.txt, or _3.txt.
The input directory should also include a zone short name to zone ID translation
csv file 'zone_id_lut.csv'.

Output: Generates two files (EXPORT_CPP_HEADER_FILENAME and
EXPORT_CPP_SOURCE_FILENAME) that can be compiled into Zeal.
"""

# Future upgrade: Consider zip compression of the zone data.

import argparse
import csv
import glob
import math
import os


# C++ header file is included here and exported as part of the process.
EXPORT_CPP_HEADER_FILENAME = 'zone_map_data.h'
EXPORT_CPP_HEADER_CONTENT = r'''
// Auto-generated file using maps_to_cpp.py
#pragma once
#include <stdint.h>

struct ZoneMapLine {
   int16_t x0;  // Starting point of line segment.
   int16_t y0;
   int16_t z0;
   int16_t x1;  // Ending point of line segment.
   int16_t y1;
   int16_t z1;
   uint8_t red;  // Line RGB color.
   uint8_t green;
   uint8_t blue;
};

struct ZoneMapLabel {
    int16_t x;  // Label location.
    int16_t y;
    int16_t z;
    uint8_t red;  // Label color.
    uint8_t green;
    uint8_t blue;
    const char* label;  // Label text.
};

struct ZoneMapData {
   const char* name;
   const int max_x;
   const int min_x;
   const int max_y;
   const int min_y;
   const int num_lines;
   const int num_labels;
   const ZoneMapLine* lines;
   const ZoneMapLabel* labels;
};

const ZoneMapData* get_zone_map_data(int zone_id);

'''


EXPORT_CPP_SOURCE_FILENAME = 'zone_map_data.cpp'
EXPORT_CPP_SOURCE_HEADER = r'''
// Auto-generated file using maps_to_cpp.py

#include "zone_map_data.h"

'''



def parse_file(file: str) -> dict:
    """Parses a map txt file to pull out lines and labels."""
    with open(file, 'r') as fp:
        input_lines = fp.readlines()

    result = {'lines': [], 'labels': []}
    for input_line in input_lines:
        splits = input_line[1:].split(',')
        if input_line[0] == 'L':
            result['lines'].append((float(splits[0]), float(splits[1]), float(splits[2]), 
                    float(splits[3]), float(splits[4]), float(splits[5]),
                    int(splits[6]), int(splits[7]), int(splits[8]),))
        elif input_line[0] == 'P':
            result['labels'].append((float(splits[0]), float(splits[1]), float(splits[2]), 
                       int(splits[3]), int(splits[4]), int(splits[5]), splits[7].strip(),))
        elif input_line.strip():
            print(f'Invalid line: {input_line}')
          
    return result

def append_zone_data(existing_zone_data: dict | None, zone_data: dict) -> dict:
    """Appends new zone data to any existing data."""
    if existing_zone_data is None:
        return zone_data

    existing_zone_data['lines'].extend(zone_data['lines'])
    existing_zone_data['labels'].extend(zone_data['labels'])
    return existing_zone_data

def add_zone_max_mins(all_zone_data: dict) -> dict:
    """Adds the max x and y coordinates for the zone for proper scaling."""
    for zone_data in all_zone_data.values():
        min_x = zone_data['lines'][0][0]  # Assume there's at least one line'
        max_x = min_x
        min_y = zone_data['lines'][0][1]
        max_y = min_y
        for line in zone_data['lines']:
            min_x = min(min_x, min(line[0], line[3]))
            max_x = max(max_x, max(line[0], line[3]))
            min_y = min(min_y, min(line[1], line[4]))
            max_y = max(max_y, max(line[1], line[4]))
        # Ignore the labels for max and mins but handle empty label case.
        if not zone_data['labels']:
            zone_data['labels'].append((0.0, 0.0, 0.0, 0, 0, 0, 'origin'),)
        if max_x == min_x:
            max_x = min_x + 1
        if max_y == min_y:
            max_y = min_y + 1
        zone_data['max_x'] = max_x
        zone_data['min_x'] = min_x
        zone_data['max_y'] = max_y
        zone_data['min_y'] = min_y

    return all_zone_data

def add_zone_ids(input_directory:str, all_zone_data: dict) -> dict:
    """Adds the zone id lookup numbers for the zone names."""

    with open(os.path.join(input_directory, 'zone_id_lut.csv'), 'r') as fp:
        csv_reader = csv.reader(fp, delimiter=',')
        zone_ids = {row[1]:row[0] for row in csv_reader}

    for zone_name, zone_data in all_zone_data.items():
        all_zone_data[zone_name]['id'] = zone_ids[zone_name]
        all_zone_data[zone_name]['id_instanced'] = zone_ids.get(f'{zone_name}_instanced', '')
        print(f'{zone_name}: id={zone_data["id"]} ({zone_data["id_instanced"]}), '
                f'n_lines={len(zone_data["lines"])}, '
                f'n_labels={len(zone_data["labels"])}')

    return all_zone_data


def format_line(line) -> str:
    result = '{'
    for x in line:
        result += f'{int(round(x))},'
    result += '}'
    return result

def format_label(label) -> str:
    result = '{'
    for x in label[0:6]:
        result += f'{int(round(x))},'
    description = label[6].replace('"', '')
    result += f'"{description}"'
    result += '}'
    return result

def export_single_zone_data(zone_name: str, zone_data: dict, fp):
    """Write the zone data to the existing open fp."""

    fp.write(f'\nstatic const ZoneMapLine {zone_name}_lines[] = {{')
    for line in zone_data['lines']:
        fp.write(format_line(line) + ',')
    fp.write('};\n')
    fp.write(f'static const ZoneMapLabel {zone_name}_labels[] = {{')
    for label in zone_data['labels']:
        fp.write(format_label(label) + ',')
    fp.write('};\n')
    fp.write(f'static const ZoneMapData {zone_name}_data = {{\n')
    fp.write(f'    "{zone_name}", ')
    fp.write(f'{math.ceil(zone_data["max_x"])}, {math.floor(zone_data["min_x"])}, ')
    fp.write(f'{math.ceil(zone_data["max_y"])}, {math.floor(zone_data["min_y"])}, ')
    fp.write(f'{len(zone_data["lines"])}, {len(zone_data["labels"])},\n')
    fp.write(f'    {zone_name}_lines, {zone_name}_labels\n')
    fp.write('};\n')

def export_lookup_function(all_zone_data: dict, fp):
    """Generates the simple lookup from Zone ID to a Zone's ZoneMapData."""
    fp.write('\nconst ZoneMapData* get_zone_map_data(int zone_id) {\n')
    fp.write('    switch(zone_id) {\n')
    for zone_name, zone_data in all_zone_data.items():
        fp.write(f'      case {zone_data["id"]}:\n')
        if zone_data['id_instanced']:
            fp.write(f'      case {zone_data["id_instanced"]}:\n')
        fp.write(f'        return &{zone_name}_data;\n')
    fp.write('      default:\n')
    fp.write('        return nullptr;\n')
    fp.write('    }\n')
    fp.write('}\n')

def export_files(all_zone_data: dict, output_directory: str):
    """Exports all of the zone data to the c++ files."""

    # First write out the header file, which is a static file.
    with open(os.path.join(output_directory, EXPORT_CPP_HEADER_FILENAME),'w') as fp:
        fp.write(EXPORT_CPP_HEADER_CONTENT)

    # Then generate the source file with the zone data.
    with open(os.path.join(output_directory, EXPORT_CPP_SOURCE_FILENAME),'w') as fp:
        fp.write(EXPORT_CPP_SOURCE_HEADER)  # Start with the static header.

        # Then sequentially write out the data for each zone.
        zone_names = sorted(list(all_zone_data.keys()))
        for zone_name in zone_names:
            export_single_zone_data(zone_name, all_zone_data[zone_name], fp)

        # Generate the lookup function at the end.
        export_lookup_function(all_zone_data, fp)


def process_directory(input_directory: str, output_directory: str):
    """Parses every map file in input_directory to generate output_files."""

    # Just sweep in all text files in the input_directory and assemble all of the data.
    glob_search = os.path.join(input_directory, 'map_files', '*.txt')
    all_zone_data = {}
    for file in glob.glob(glob_search):
        zone_data = parse_file(file)
        zone_name = os.path.basename(file).lower()
        for suffix in ('_1.txt', '_2.txt', '_3.txt', '_4.txt', '.txt'):
            zone_name = zone_name.removesuffix(suffix)
        all_zone_data[zone_name] = append_zone_data(all_zone_data.get(zone_name, None), zone_data)

    all_zone_data = add_zone_max_mins(all_zone_data)  # Add max and min boundaries for each zone.

    # Add the zone IDs and then export the data (generate the c++ files).
    all_zone_data = add_zone_ids(input_directory, all_zone_data)
    export_files(all_zone_data, output_directory)

def main() -> str:
    """Parse command-line arguments and execute the script."""
    parser = argparse.ArgumentParser(description='Convert directory of map files to a .h and .cpp file.')
    parser.add_argument(
        '--input_directory',
        default='./',
        type=str,
        help='Input directory with map txt files')
    args = parser.parse_args()
    parser.add_argument(
        '--output_directory',
        default='../',
        type=str,
        help='Output directory to write .h and .cpp files')
    args = parser.parse_args()

    process_directory(input_directory = args.input_directory, output_directory = args.output_directory)

if __name__ == '__main__':
    main()
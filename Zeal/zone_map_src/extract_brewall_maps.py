"""Script for extracting only Quarm (PoP) maps from Brewall."""

import argparse
import csv
import os
import shutil

def _get_list_of_zones() -> list[str]:
    """Returns a list of short zone names from the lut csv file."""
    with open('zone_id_lut.csv', 'r') as fp:
        csv_reader = csv.reader(fp, delimiter=',')
        zone_names = [row[1] for row in csv_reader]
        return zone_names


def extract_map_files(input_directory: str):
    zone_list = _get_list_of_zones()
    
    output_map_dir = 'map_files'
    if os.path.exists(output_map_dir) and os.listdir(output_map_dir):
        print(f'Error: Output folder "{output_map_dir}" is not empty. Aborting. ')
        return
    os.makedirs(output_map_dir, exist_ok = True)

    missing_zones = []
    for zone_name in zone_list:
        suffix_list = ['.txt', '_1.txt']
        for suffix in suffix_list:
            base_filename = f'{zone_name}{suffix}'
            input_file = os.path.join(input_directory, base_filename)
            output_file = os.path.join(output_map_dir, base_filename)

            # If available, use the original versions of some completely revamped zones.
            # There are two possible permutations of the original tag.
            base_original = f'{zone_name}_original{suffix}'
            input_original = os.path.join(input_directory, base_original)
            if not os.path.exists(input_original):  # Try the second type.
                base_original = base_original.replace('_original_1.txt', '_1_original.txt')
                input_original = os.path.join(input_directory, base_original)

            if os.path.exists(input_original):
                shutil.copy(input_original, output_file)  # Renamed in the copy.
            elif os.path.exists(input_file):
                shutil.copy(input_file, output_file)  # Otherwise a straight copy.
            else:
                missing_zones.append(zone_name)

    print(f'Processed {len(zone_list)} zone names')
    missing_zones = list(set(missing_zones))  # Remove duplicates.
    missing_other = [zone for zone in missing_zones if not 'instanced' in zone]
    print(f'Skipped {len(missing_zones) - len(missing_other)} instanced zones')
    print(f'Skipped {len(missing_other)} other zones: ')
    print(missing_other)

def main() -> str:
    """Parse command-line arguments and execute the script."""
    parser = argparse.ArgumentParser(description='Extract Quarm relevant map files from Brewall')
    parser.add_argument(
        '--input_directory',
        default='./../../../brewall-20240109',
        type=str,
        help='Input directory with extracted Brewall map data')
    args = parser.parse_args()

    extract_map_files(input_directory = args.input_directory)

if __name__ == '__main__':
    main()
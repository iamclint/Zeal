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
        file_list = [f'{zone_name}.txt', f'{zone_name}_1.txt']
        for filename in file_list:
            if os.path.exists(os.path.join(input_directory, filename)):
                shutil.copy(os.path.join(input_directory, filename), output_map_dir)
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
"""Script for extracting short_name and zoneidnumber from a full zone table."""

import csv

def parse_file(in_filename: str, out_filename: str):

    short_name_field = 0
    zoneidnumber_field = 12
    with open(in_filename, 'r') as fp:
        csv_reader = csv.reader(fp)
        zone_map = {int(row[zoneidnumber_field]):row[short_name_field] for row in csv_reader}

    zone_ids = sorted(list(zone_map.keys()))
    with open(out_filename, 'w', newline='') as fp:
        csv_writer = csv.writer(fp)
        for id in zone_ids:
            csv_writer.writerow([id, zone_map[id]])

if __name__ == '__main__':
    parse_file('zone.csv', 'zone_id_lut.csv')
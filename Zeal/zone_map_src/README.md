# Zone Map generation utilities

Support scripts and data files for producing the auto-generated zone_map_data.h and zone_map_data.cpp files.

## Map data source:
* The map data in the `map_files` directory was sourced from:
  https://www.eqmaps.info/eq-map-files/ (Brewall's Everquest Maps 20240109)
* The Brewall maps have greater detail and consistent layer color coding (useful for z-filtering).
* The original unzipped download was 142 MB but included maps from eras past PoP.
* The `extract_brewall_maps.py` used the zone names in the `zone_id_lut.csv` file described below
  to copy out only Quarm PoP related maps, which reduced the `map_files` down to 36 MB.
* Note: The `bazaar.txt` and `bazaar_1.txt` were out of era maps without any `_original` versions
  - Deleted them manually and copied over a correct `bazaar.txt` into the `map_files` folder

## Integer zone_id to short zone name lookup (zone_id_lut.csv)
* Zeal has access to the current zone with an integer zone_id
* That zone_id maps to the `zoneidnumber` (field [12]) of the `zone` data table
* The map data files are prefixed with the `short_name` (field [0]) of the `zone` data table
* A simple script was used to generate the `zone_id_lut.csv` file
  `python extract_zone_id_lut.csv`

## Generation of the zone_map_data.h and zone_map_data.cpp files
* Note: "Succor" points of interest are filtered out and handled automatically in the code
* Execute the script below from the zone_map_src directory and it by default uses the checked in
  input files and writes the output to the Zeal source directory (at ../)
  `python maps_to_cpp.py`

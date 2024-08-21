# Zone Map generation utilities

Support scripts and data files for producing the auto-generated zone_map_data.h and zone_map_data.cpp files.

## Map data source:
* The map_files/ directory was downloaded from below without modification:
  https://github.com/EJWellman/QuarmTool/tree/a728dc9fd00fd8599dfb15b973749f43a5043435/src/map_files
* The map data files above are 18 MB and add ~4 MB to Zeal after conversion to zone_map_data.cpp
* The alternative, richer Brewtal maps were 142 MB

## Integer zone_id to short zone name lookup (zone_id_lut.csv)
* Zeal has access to the current zone with an integer zone_id
* That zone_id maps to the `zoneidnumber` (field [12]) of the `zone` data table
* The map data files are prefixed with the `short_name` (field [0]) of the `zone` data table
* A simple script was used to generate the `zone_id_lut.csv` file
  `python extract_zone_id_lut.csv`

## Generation of the zone_map_data.h and zone_map_data.cpp files
* Execute the script below from the zone_map_src directory and it by default uses the checked in
  input files and writes the output to the Zeal source directory (at ../)
  `python maps_to_cpp.py`

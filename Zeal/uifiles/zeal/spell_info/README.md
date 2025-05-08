# Enhanced spell info

This folder contains the `spell_info.txt` file which is optionally used by Zeal
to show extra spell information in the spell and spell scroll displays.

The python script scrapes Icestorm's yaqds.cc website to pull down the spell
descriptions and then format the `spell_info.txt`. The file format is a simple
one line per spell id indexed from 0 to 3999 (lines 1 to 4000) using the
`^` character as a line delimiter.

If enabled, Zeal just extracts the line from the file based on spell ID and
replaces the window text with it.

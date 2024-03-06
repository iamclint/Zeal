### Zeal Features
- Huge camera motion improvement in the third person and sensitivity for first person
- Additional binds
- Additional commands
- Verious bug fixes
  
### Commands
___
- `/hidecorpse`
  - **Arguments:** `looted`, `none`
  - **Aliases:** `/hideco`, `/hidec`, `/hc`
  - **Example:** `/hidecorpse looted`
  - **Description:** `looted` Hides a corpse after you have looted it., `none` reveals all hidden corpses

- `/showhelm`
  - **Aliases:** `/helm`
  - **Arguments:** `on, off`
  - **Example:** `/showhelm on`
  - **Description:** Toggles your helmet.

- `/showlootlockouts`
  - **Aliases:** `/showlootlockout`, `/showlockout`, `/sll`
  - **Description:** Shows you your current loot lockouts on Quarm.

- `/zealcam`
  - **Aliases:** `/smoothing`
  - **Arguments:** `x y 3rdperson_x 3rdperson_y`
  - **Example:** `/zealcam 0.7 0.2 0.7 0.2` if 3rd person arguments are not supplied, the first x and y are applied to both
  - **Description:** Toggles Zeal's mouse look smoothing methods, the first 2 arguments are first person sensitivity, and the last 2 are for 3rd person

- `/autoinventory`
  - **Aliases:** `/autoinv`, `/ai`
  - **Description:** Drops whatever is on your cursor into your inventory.

- `/camp`
  - **Description:** Auto sits before camping.

- `/zeal`
  - **Arguments:** `version`
  - **Description:** Shows the version of zeal.

- `/help zeal`
  - **Description:** Shows the custom Zeal commands.

- `/timestamp`
  - **Aliases:** `/tms`
  - **Description:** Shows message timestamps.

- `/outputfile`
  - **Aliases:** `/output`, `/out`
  - **Arguments:** `inventory | spellbook` `[optional_name]`
  - **Example:** `/outputfile inventory my_inventory`
  - **Description:**
    - `inventory` outputs information about your equipment, inventory bag slots, held item, and bank slots to a file.
    - `spellbook` outputs a list of all spell ids current scribed in your spellbook.

- `/buffs`
  - **Description:** Outputs the players buff timers to the chat only if they are using OldUI.

- `/bluecon`
  - **Description:** Changes the blue con color to usercolor #70 which is otherwise unused, you can edit in the options window.

- `/alarm`
  - **Arguments:** `oldui`
  - **Description:** Re-opens the alarm window, if oldui is specified it allows for an alarm on it.
___
### Binds
- Cycle through nearest NPCs
- Cycle through nearest PCs
- Strafe Right
- Strafe Left
- Auto Inventory
- Toggle last 2 targets
- Reply target
___
### UI
- **Gauge EqType's**
  - `23` EXP Per Hour

- **Label EqType's**
  - `80` Mana/Max Mana
  - `81` Exp Per Hour Percentage
  - `124` Current Mana
  - `125` Max Mana
___
### Building and Installation
<br>
32bit x86
<br>
file extension .asi
<br>
move zeal.asi into the root of your game folder

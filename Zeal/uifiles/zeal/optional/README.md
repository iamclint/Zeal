# Optional Zeal UI Files

Extra xml files that can be used to update the default or custom UI skins.

## EQUI_CharacterSelect.xml

- Adds the "Zone Select" button for setting the background and explore mode
  in a XML controlled location (if the default simple button location is undesirable)
  - UI label: "Zeal_ZoneSelect"
- The code adds a default simple button if the character select xml does have
  this child button specifying the location.

### Installation if using this file
- Can copy to the uifiles/zeal directory or to the custom or default skin
  - If not copied to ufiles/default directory, the xml does not take affect until 
    returning (camping) to character select
  - Note this is the same behavior for all custom character select skins.
- Based on the default xml file
- If using a custom skin, it requires that the active EQUI_Animations.xml include
  the Ui2DAnimations from following section in the default EQUI_Animations.xml:
  - -- Title graphics for Character Create --


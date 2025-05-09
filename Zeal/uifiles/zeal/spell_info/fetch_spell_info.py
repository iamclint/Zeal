"""Simple hack script that scrapes the spell info from Icestorm's yaqds.cc site.

  It is slow but works. Likely to be upgraded 'soon'.

  The output is the spell_info.txt which creates a single line per spell ID index
  using a '^' as the delimiter between display lines.
"""

#%%
import http.client  # Used since no additional packages needed.
import json

def get_spell_info(text):
    """Extracts the json-compatible _description blob."""
    try:
        if "Spell does not exist" in text:
            return ""
        text = text.replace(": L{{pet:level}} {{pet:name}}", " pet") # Hack workarounds.
        text = text.replace(" {{spell:targetTypeName}}", " target type")
        info = text[text.find('{'):text.rfind('}')+1]
        if info:
            description = json.loads(info).get('_description', '')
            if description:
                count = len(description)
                while count:
                    count -= 1
                    if not description[count]:
                        return description[:count]
    except Exception as exc:
        print(str(exc))
        print(info)

    return ""

spell_lines = []
for id in range(4000): # Fetch all 4000 spell_ids supported in TAKP client.
    if id % 25 == 0:
        print(f'Id: {id}')
    conn = http.client.HTTPSConnection("yaqds.cc")
    conn.request("GET", f"/r/spell/{id}")
    res = conn.getresponse()
    spellinfo = ""
    if res.status == 200:
        spellinfo = get_spell_info(res.read().decode("utf-8"))
        if spellinfo:
            spellinfo = '^'.join(spellinfo)
    spell_lines.append(spellinfo + '\n')    

with open('spell_info.txt', 'wt') as file:
    file.writelines(spell_lines)

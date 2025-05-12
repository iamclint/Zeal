"""Simple script to grab the data from Icestorm's yaqds.cc site.

  The output is the spell_info.txt which creates a single line per spell ID index
  (from [0] to [3999]) using a '^' as the delimiter between output display lines
"""

#%%
import http.client  # Used since no additional packages needed.

def fetch_data(filename: str) -> bool:
    """Fetches the spell information from yaqds.cc."""
    # Executes a GET for a zeal formatted response. The json header request
    # plus the ?zeal=true triggers the server to send the raw zeal data."""
    conn = http.client.HTTPSConnection("yaqds.cc")
    headers = {'Accept': 'application/json'} 
    conn.request("GET", "/r/data/spell?zeal=true", headers=headers) 
    res = conn.getresponse()
    if res.status != 200:
        print("Error: Did not get a successful response")
        return False

    spell_info = res.read().decode("utf-8")
    spell_lines = spell_info.splitlines()
    if len(spell_lines) != 4000:
        print(f"Error: incorrect # of spells returned: {len(spell_lines)}")
        return False
    # Strip the unneeded casting message formatters.
    for index, line in enumerate(spell_lines):
        if line.count("^^") == 2:
            spell_lines[index] = line[:line.rfind("^^")]
        spell_lines[index] += '\n'

    with open(filename, 'wt') as file:
        file.writelines(spell_lines)
    print(f"Wrote data to {filename}")
    return True

if fetch_data("spell_info.txt"):
    print("Success")
else:
    print("Failed")


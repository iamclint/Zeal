#pragma once
#include <windows.h>
#include <string>
//original credits for this go to mmofry

static inline DWORD GetSpellCategory(DWORD spellID)
{
	switch (spellID)
	{
	case 3: //Summon Corpse
		return 125;
	case 4: //Summon Waterstone
		return 18;
	case 6: //Ignite Blood
		return 20;
	case 7: //Hymn of Restoration
		return 79;
	case 9: //Superior Healing
		return 42;
	case 10: //Augmentation
		return 125;
	case 11: //Holy Armor
		return 95;
	case 12: //Healing
		return 42;
	case 13: //Complete Healing
		return 42;
	case 14: //Strike
		return 25;
	case 15: //Greater Healing
		return 42;
	case 16: //Smite
		return 25;
	case 17: //Light Healing
		return 42;
	case 18: //Guard
		return 95;
	case 19: //Armor of Faith
		return 95;
	case 20: //Shield of Words
		return 95;
	case 21: //Berserker Strength
		return 95;
	case 22: //Force Snap
		return 25;
	case 23: //Force Strike
		return 25;
	case 24: //Strip Enchantment
		return 126;
	case 25: //Pillage Enchantment
		return 126;
	case 26: //Skin like Wood
		return 45;
	case 27: //Pogonip
		return 25;
	case 28: //Avalanche
		return 25;
	case 29: //Ice
		return 25;
	case 31: //Scourge
		return 20;
	case 32: //Plague
		return 20;
	case 33: //Brilliance
		return 95;
	case 34: //Superior Camouflage
		return 125;
	case 35: //Bind Affinity
		return 125;
	case 36: //Gate
		return 123;
	case 37: //Hammer of Striking
		return 18;
	case 38: //Lightning Bolt
		return 25;
	case 39: //Quickness
		return 125;
	case 40: //Strengthen
		return 95;
	case 41: //Weaken
		return 126;
	case 42: //Invisibility
		return 125;
	case 43: //Yaulp II
		return 95;
	case 44: //Yaulp III
		return 95;
	case 45: //Pacify
		return 126;
	case 46: //Ultravision
		return 125;
	case 47: //Calm
		return 126;
	case 48: //Cancel Magic
		return 126;
	case 49: //Nullify Magic
		return 126;
	case 50: //Summon Food
		return 18;
	case 51: //Glimpse
		return 125;
	case 52: //Abundant Drink
		return 18;
	case 53: //Abundant Food
		return 18;
	case 54: //Frost Bolt
		return 25;
	case 55: //Cornucopia
		return 18;
	case 56: //Everfount
		return 18;
	case 57: //Firestrike
		return 25;
	case 58: //Elementalkin: Earth
		return 69;
	case 59: //Panic the Dead
		return 126;
	case 60: //Resist Fire
		return 95;
	case 61: //Resist Cold
		return 95;
	case 62: //Resist Poison
		return 95;
	case 63: //Resist Disease
		return 95;
	case 64: //Resist Magic
		return 95;
	case 65: //Major Shielding
		return 45;
	case 66: //Greater Shielding
		return 45;
	case 67: //Arch Shielding
		return 45;
	case 68: //Bolt of Flame
		return 25;
	case 69: //Cinder Bolt
		return 25;
	case 70: //Lava Bolt
		return 25;
	case 71: //Anarchy
		return 25;
	case 72: //Group Resist Magic
		return 95;
	case 73: //Gravity Flux
		return 25;
	case 74: //Mana Sieve
		return 126;
	case 75: //Sicken
		return 20;
	case 76: //Ensnaring Roots
		return 126;
	case 77: //Engulfing Roots
		return 126;
	case 78: //Immolate
		return 20;
	case 79: //Spirit Sight
		return 125;
	case 80: //See Invisible
		return 125;
	case 81: //Phantom Chain
		return 79;
	case 82: //Phantom Plate
		return 79;
	case 83: //Rain of Fire
		return 25;
	case 84: //Shifting Sight
		return 125;
	case 85: //Firestorm
		return 25;
	case 86: //Enduring Breath
		return 125;
	case 88: //Harm Touch
		return 25;
	case 89: //Daring
		return 45;
	case 90: //Shadow Sight
		return 125;
	case 91: //Ignite
		return 25;
	case 92: //Burst of Fire
		return 25;
	case 93: //Burst of Flame
		return 25;
	case 94: //Burn
		return 25;
	case 95: //Counteract Poison
		return 42;
	case 96: //Counteract Disease
		return 42;
	case 97: //Abolish Poison
		return 42;
	case 98: //Abolish Disease
		return 42;
	case 99: //Creeping Crud
		return 20;
	case 100: //Summon Throwing Dagger
		return 18;
	case 101: //Summon Arrows
		return 18;
	case 102: //Spear of Warding
		return 18;
	case 103: //Summon Coldstone
		return 18;
	case 104: //Dagger of Symbols
		return 18;
	case 105: //Summon Ring of Flight
		return 18;
	case 106: //Burnout II
		return 69;
	case 107: //Burnout III
		return 69;
	case 108: //Elemental Shield
		return 95;
	case 109: //Elemental Armor
		return 95;
	case 110: //Malaise
		return 126;
	case 111: //Malaisement
		return 126;
	case 112: //Malosi
		return 126;
	case 113: //Shock of Spikes
		return 25;
	case 114: //Shock of Swords
		return 25;
	case 115: //Dismiss Summoned
		return 25;
	case 116: //Banish Summoned
		return 25;
	case 117: //Dismiss Undead
		return 25;
	case 118: //Banish Undead
		return 25;
	case 120: //Blaze
		return 25;
	case 121: //Rain of Lava
		return 25;
	case 122: //Flame Arc
		return 25;
	case 123: //Holy Might
		return 25;
	case 124: //Force
		return 25;
	case 125: //Sound of Force
		return 25;
	case 126: //Inspire Fear
		return 126;
	case 127: //Invoke Fear
		return 126;
	case 128: //Wave of Fear
		return 126;
	case 129: //Shield of Brambles
		return 125;
	case 130: //Divine Barrier
		return 125;
	case 131: //Instill
		return 126;
	case 132: //Immobilize
		return 126;
	case 133: //Paralyzing Earth
		return 126;
	case 134: //Blinding Luminance
		return 126;
	case 135: //Word of Health
		return 42;
	case 136: //Word of Healing
		return 42;
	case 137: //Pack Regeneration
		return 79;
	case 138: //Pack Chloroplast
		return 79;
	case 139: //Feral Spirit
		return 69;
	case 140: //Savage Spirit
		return 69;
	case 141: //Beguile Animals
		return 126;
	case 142: //Allure of the Wild
		return 126;
	case 143: //Sunbeam
		return 126;
	case 144: //Regeneration
		return 79;
	case 145: //Chloroplast
		return 79;
	case 146: //Spirit of Monkey
		return 95;
	case 147: //Spirit Strength
		return 95;
	case 148: //Spirit of Cat
		return 95;
	case 149: //Spirit of Ox
		return 95;
	case 150: //Alluring Aura
		return 95;
	case 151: //Raging Strength
		return 95;
	case 152: //Deftness
		return 95;
	case 153: //Furious Strength
		return 95;
	case 154: //Agility
		return 95;
	case 155: //Glamour
		return 95;
	case 156: //Charisma
		return 95;
	case 157: //Dexterity
		return 95;
	case 158: //Stamina
		return 95;
	case 159: //Strength
		return 95;
	case 160: //Nimble
		return 95;
	case 161: //Health
		return 95;
	case 162: //Listless Power
		return 126;
	case 163: //Incapacitate
		return 126;
	case 164: //Companion Spirit
		return 69;
	case 165: //Guardian Spirit
		return 69;
	case 166: //Frenzied Spirit
		return 69;
	case 167: //Talisman of Tnarg
		return 45;
	case 168: //Talisman of Altuna
		return 45;
	case 169: //Pack Spirit
		return 125;
	case 170: //Alacrity
		return 125;
	case 171: //Celerity
		return 125;
	case 172: //Swift like the Wind
		return 125;
	case 173: //Benevolence
		return 125;
	case 174: //Clarity
		return 79;
	case 175: //Insight
		return 95;
	case 176: //Berserker Spirit
		return 125;
	case 177: //Color Shift
		return 25;
	case 178: //Color Skew
		return 25;
	case 179: //Feckless Might
		return 126;
	case 180: //Insipid Weakness
		return 126;
	case 181: //Weakness
		return 126;
	case 182: //Beguile
		return 126;
	case 183: //Cajoling Whispers
		return 126;
	case 184: //Allure
		return 126;
	case 185: //Tepid Deeds
		return 126;
	case 186: //Shiftless Deeds
		return 126;
	case 187: //Enthrall
		return 126;
	case 188: //Entrance
		return 126;
	case 189: //Flame Flux
		return 25;
	case 190: //Dazzle
		return 126;
	case 191: //Feedback
		return 125;
	case 192: //Mind Wipe
		return 126;
	case 193: //Blanket of Forgetfulness
		return 126;
	case 194: //Reoccurring Amnesia
		return 126;
	case 195: //Gasping Embrace
		return 20;
	case 196: //Dominate Undead
		return 126;
	case 197: //Beguile Undead
		return 126;
	case 198: //Cajole Undead
		return 126;
	case 199: //Harmshield
		return 125;
	case 200: //Minor Healing
		return 42;
	case 201: //Flash of Light
		return 126;
	case 202: //Courage
		return 45;
	case 203: //Cure Poison
		return 42;
	case 204: //Shock of Poison
		return 25;
	case 205: //True North
		return 125;
	case 207: //Divine Aura
		return 125;
	case 208: //Lull
		return 126;
	case 209: //Spook the Dead
		return 126;
	case 210: //Yaulp
		return 95;
	case 211: //Summon Drink
		return 18;
	case 212: //Cure Blindness
		return 42;
	case 213: //Cure Disease
		return 42;
	case 215: //Reckless Strength
		return 95;
	case 216: //Stun
		return 25;
	case 217: //Combust
		return 25;
	case 218: //Ward Undead
		return 25;
	case 219: //Center
		return 45;
	case 220: //Spirit of Cheetah
		return 125;
	case 221: //Sense the Dead
		return 125;
	case 222: //Invigor
		return 42;
	case 223: //Hammer of Wrath
		return 18;
	case 224: //Endure Fire
		return 95;
	case 225: //Endure Cold
		return 95;
	case 226: //Endure Disease
		return 95;
	case 227: //Endure Poison
		return 95;
	case 228: //Endure Magic
		return 95;
	case 229: //Fear
		return 126;
	case 230: //Root
		return 126;
	case 231: //Word of Pain
		return 25;
	case 232: //Sense Summoned
		return 125;
	case 233: //Expulse Undead
		return 25;
	case 234: //Halo of Light
		return 18;
	case 235: //Invisibility versus Undead
		return 125;
	case 236: //Shieldskin
		return 125;
	case 237: //Dance of the Fireflies
		return 18;
	case 238: //Sense Animals
		return 125;
	case 239: //Flame Lick
		return 20;
	case 240: //Lull Animal
		return 126;
	case 241: //Panic Animal
		return 126;
	case 242: //Snare
		return 126;
	case 243: //Illusion: Iksar
		return 125;
	case 244: //Bravery
		return 45;
	case 245: //Befriend Animal
		return 126;
	case 246: //Lesser Shielding
		return 45;
	case 247: //Camouflage
		return 125;
	case 248: //Ward Summoned
		return 25;
	case 249: //Grasping Roots
		return 126;
	case 250: //Harmony
		return 126;
	case 252: //Invoke Lightning
		return 25;
	case 253: //Whirling Wind
		return 25;
	case 254: //Firefist
		return 95;
	case 255: //Invisibility versus Animals
		return 125;
	case 256: //Shield of Thistles
		return 125;
	case 257: //Starshine
		return 18;
	case 258: //Treeform
		return 125;
	case 259: //Drones of Doom
		return 20;
	case 260: //Charm Animals
		return 126;
	case 261: //Levitate
		return 125;
	case 262: //Cascade of Hail
		return 25;
	case 263: //Skin like Rock
		return 45;
	case 264: //Stinging Swarm
		return 20;
	case 265: //Cannibalize
		return 125;
	case 266: //Dexterous Aura
		return 95;
	case 267: //Inner Fire
		return 45;
	case 268: //Strength of Earth
		return 95;
	case 269: //Feet like Cat
		return 95;
	case 270: //Drowsy
		return 126;
	case 271: //Fleeting Fury
		return 95;
	case 272: //Spirit Pouch
		return 18;
	case 273: //Shield of Barbs
		return 125;
	case 274: //Scale Skin
		return 95;
	case 275: //Frost Rift
		return 25;
	case 276: //Serpent Sight
		return 125;
	case 277: //Tainted Breath
		return 20;
	case 278: //Spirit of Wolf
		return 125;
	case 279: //Spirit of Bear
		return 95;
	case 280: //Burst of Strength
		return 95;
	case 281: //Disempower
		return 126;
	case 282: //Spirit Strike
		return 25;
	case 283: //Turtle Skin
		return 95;
	case 284: //Spirit of Snake
		return 95;
	case 285: //Pendril's Animation
		return 69;
	case 286: //Shallow Breath
		return 20;
	case 287: //Minor Illusion
		return 125;
	case 288: //Minor Shielding
		return 45;
	case 289: //Taper Enchantment
		return 126;
	case 290: //Color Flux
		return 25;
	case 291: //Enfeeblement
		return 126;
	case 292: //Mesmerize
		return 126;
	case 293: //Haze
		return 95;
	case 294: //Suffocating Sphere
		return 20;
	case 295: //Mircyl's Animation
		return 69;
	case 296: //Chaotic Feedback
		return 25;
	case 297: //Eye of Confusion
		return 126;
	case 298: //Alliance
		return 125;
	case 299: //Sentinel
		return 125;
	case 300: //Charm
		return 126;
	case 301: //Memory Blur
		return 126;
	case 302: //Languid Pace
		return 126;
	case 303: //Whirl till you hurl
		return 25;
	case 304: //Chase the Moon
		return 126;
	case 305: //Identify
		return 125;
	case 306: //Sanity Warp
		return 25;
	case 307: //Mesmerization
		return 126;
	case 308: //Frenzy
		return 95;
	case 309: //Shielding
		return 45;
	case 310: //Flare
		return 125;
	case 311: //Summon Dagger
		return 18;
	case 312: //Valor
		return 45;
	case 313: //Fire Flux
		return 25;
	case 314: //Resolution
		return 45;
	case 315: //Elementalkin: Water
		return 69;
	case 316: //Elementalkin: Fire
		return 69;
	case 317: //Elementalkin: Air
		return 69;
	case 318: //Summon Bandages
		return 18;
	case 319: //Summon Fang
		return 18;
	case 320: //Summon Heatstone
		return 18;
	case 321: //Summon Wisp
		return 18;
	case 322: //Flame Bolt
		return 25;
	case 323: //Eye of Zomm
		return 125;
	case 324: //Shock of Blades
		return 25;
	case 325: //Dimensional Pocket
		return 18;
	case 326: //Fury
		return 95;
	case 327: //Burnout
		return 69;
	case 328: //Column of Fire
		return 25;
	case 329: //Wrath
		return 25;
	case 330: //Rain of Blades
		return 25;
	case 331: //Reclaim Energy
		return 69;
	case 332: //Shield of Fire
		return 125;
	case 333: //Phantom Leather
		return 79;
	case 334: //Shock of Flame
		return 25;
	case 335: //Minor Summoning: Earth
		return 69;
	case 336: //Minor Summoning: Water
		return 69;
	case 337: //Rage
		return 95;
	case 338: //Cavorting Bones
		return 69;
	case 339: //Coldlight
		return 18;
	case 340: //Disease Cloud
		return 20;
	case 341: //Lifetap
		return 114;
	case 342: //Locate Corpse
		return 125;
	case 343: //Siphon Strength
		return 114;
	case 344: //Clinging Darkness
		return 20;
	case 345: //Shrink
		return 125;
	case 346: //Grim Aura
		return 95;
	case 347: //Numb the Dead
		return 126;
	case 348: //Poison Bolt
		return 20;
	case 349: //Rising Dexterity
		return 95;
	case 350: //Chaos Flux
		return 25;
	case 351: //Bone Walk
		return 69;
	case 352: //Deadeye
		return 125;
	case 353: //Mend Bones
		return 69;
	case 354: //Shadow Step
		return 125;
	case 355: //Engulfing Darkness
		return 20;
	case 356: //Shield of Thorns
		return 125;
	case 357: //Dark Empathy
		return 42;
	case 358: //Impart Strength
		return 95;
	case 359: //Vampiric Embrace
		return 125;
	case 360: //Heat Blood
		return 20;
	case 361: //Sight Graft
		return 125;
	case 362: //Convoke Shadow
		return 69;
	case 363: //Wave of Enfeeblement
		return 126;
	case 364: //Banshee Aura
		return 125;
	case 365: //Infectious Cloud
		return 20;
	case 366: //Feign Death
		return 125;
	case 367: //Heart Flutter
		return 20;
	case 368: //Spirit Armor
		return 95;
	case 369: //Hungry Earth
		return 126;
	case 370: //Shadow Vortex
		return 126;
	case 371: //Voice Graft
		return 125;
	case 372: //Blast of Cold
		return 25;
	case 373: //Sphere of Light
		return 18;
	case 374: //Numbing Cold
		return 25;
	case 375: //Fade
		return 125;
	case 376: //Shock of Fire
		return 25;
	case 377: //Icestrike
		return 25;
	case 378: //O'Keils Radiation
		return 125;
	case 379: //Fingers of Fire
		return 25;
	case 380: //Column of Frost
		return 25;
	case 381: //Resistant Skin
		return 95;
	case 382: //Frost Spiral of Al'Kabor
		return 25;
	case 383: //Shock of Lightning
		return 25;
	case 384: //Assiduous Vision
		return 125;
	case 385: //Project Lightning
		return 25;
	case 386: //Pillar of Fire
		return 25;
	case 387: //Leatherskin
		return 125;
	case 388: //Resuscitate
		return 42;
	case 389: //Guardian
		return 95;
	case 390: //Thicken Mana
		return 18;
	case 391: //Revive
		return 42;
	case 392: //Resurrection
		return 42;
	case 393: //Steelskin
		return 125;
	case 394: //Diamondskin
		return 125;
	case 395: //Minor Summoning: Fire
		return 69;
	case 396: //Minor Summoning: Air
		return 69;
	case 397: //Elementaling: Earth
		return 69;
	case 398: //Elementaling: Water
		return 69;
	case 399: //Elementaling: Fire
		return 69;
	case 400: //Elementaling: Air
		return 69;
	case 401: //Elemental: Earth
		return 69;
	case 402: //Elemental: Water
		return 69;
	case 403: //Elemental: Fire
		return 69;
	case 404: //Elemental: Air
		return 69;
	case 405: //Tremor
		return 25;
	case 406: //Earthquake
		return 25;
	case 407: //Cast Sight
		return 125;
	case 408: //Curse of the Simple Mind
		return 126;
	case 409: //Rain of Spikes
		return 25;
	case 410: //Rain of Swords
		return 25;
	case 411: //Shield of Flame
		return 125;
	case 412: //Shield of Lava
		return 125;
	case 413: //Word of Shadow
		return 25;
	case 414: //Word of Spirit
		return 25;
	case 415: //Word of Souls
		return 25;
	case 416: //Word Divine
		return 25;
	case 417: //Extinguish Fatigue
		return 42;
	case 418: //Lightning Strike
		return 25;
	case 419: //Careless Lightning
		return 25;
	case 420: //Lightning Blast
		return 25;
	case 421: //Skin like Steel
		return 45;
	case 422: //Skin like Diamond
		return 45;
	case 423: //Skin like Nature
		return 45;
	case 424: //Scale of Wolf
		return 125;
	case 425: //Wolf Form
		return 125;
	case 426: //Greater Wolf Form
		return 125;
	case 427: //Form of the Great Wolf
		return 125;
	case 428: //Share Wolf Form
		return 125;
	case 429: //Strength of Stone
		return 95;
	case 430: //Storm Strength
		return 95;
	case 431: //Shifting Shield
		return 95;
	case 432: //Shield of Spikes
		return 125;
	case 433: //Fire
		return 25;
	case 434: //Envenomed Breath
		return 20;
	case 435: //Venom of the Snake
		return 20;
	case 436: //Envenomed Bolt
		return 20;
	case 437: //Poison Storm
		return 25;
	case 438: //Gale of Poison
		return 25;
	case 439: //Crystallize Mana
		return 18;
	case 440: //Animate Dead
		return 69;
	case 441: //Summon Dead
		return 69;
	case 442: //Malignant Dead
		return 69;
	case 443: //Invoke Death
		return 69;
	case 444: //Renew Bones
		return 69;
	case 445: //Lifedraw
		return 114;
	case 446: //Siphon Life
		return 114;
	case 447: //Drain Soul
		return 114;
	case 448: //Rest the Dead
		return 126;
	case 449: //Intensify Death
		return 69;
	case 450: //Suffocate
		return 20;
	case 451: //Boil Blood
		return 20;
	case 452: //Dooming Darkness
		return 20;
	case 453: //Cascading Darkness
		return 20;
	case 454: //Vampiric Curse
		return 114;
	case 455: //Surge of Enfeeblement
		return 126;
	case 456: //Bond of Death
		return 114;
	case 457: //Dead Man Floating
		return 125;
	case 458: //Fire Spiral of Al'Kabor
		return 25;
	case 459: //Shock Spiral of Al'Kabor
		return 25;
	case 460: //Force Spiral of Al'Kabor
		return 25;
	case 461: //Cast Force
		return 25;
	case 462: //Column of Lightning
		return 25;
	case 463: //Circle of Force
		return 25;
	case 464: //Frost Shock
		return 25;
	case 465: //Inferno Shock
		return 25;
	case 466: //Lightning Shock
		return 25;
	case 467: //Lightning Storm
		return 25;
	case 468: //Energy Storm
		return 25;
	case 469: //Lava Storm
		return 25;
	case 470: //Thunder Strike
		return 25;
	case 471: //Thunderclap
		return 25;
	case 472: //Inspire Fear2
		return 126;
	case 473: //Invoke Fear II
		return 126;
	case 474: //Radius of Fear2
		return 126;
	case 475: //Fear2
		return 126;
	case 477: //Fire Bolt
		return 25;
	case 478: //Breath of the Dead
		return 125;
	case 479: //Inferno Shield
		return 125;
	case 480: //Atone
		return 126;
	case 481: //Rune I
		return 125;
	case 482: //Rune II
		return 125;
	case 483: //Rune III
		return 125;
	case 484: //Rune IV
		return 125;
	case 485: //Symbol of Transal
		return 45;
	case 486: //Symbol of Ryltan
		return 45;
	case 487: //Symbol of Pinzarn
		return 45;
	case 488: //Symbol of Naltron
		return 45;
	case 489: //Sympathetic Aura
		return 95;
	case 490: //Enveloping Roots
		return 126;
	case 491: //Leering Corpse
		return 69;
	case 492: //Restless Bones
		return 69;
	case 493: //Haunting Corpse
		return 69;
	case 494: //Invoke Shadow
		return 69;
	case 495: //Cackling Bones
		return 69;
	case 496: //Lesser Summoning: Earth
		return 69;
	case 497: //Lesser Summoning: Water
		return 69;
	case 498: //Lesser Summoning: Fire
		return 69;
	case 499: //Lesser Summoning: Air
		return 69;
	case 500: //Bind Sight
		return 125;
	case 501: //Soothe
		return 126;
	case 502: //Lifespike
		return 114;
	case 503: //Tishan's Clash
		return 25;
	case 504: //Frenzied Strength
		return 95;
	case 505: //Walking Sleep
		return 126;
	case 506: //Tagar's Insects
		return 126;
	case 507: //Togor's Insects
		return 126;
	case 508: //Frost Strike
		return 25;
	case 509: //Winter's Roar
		return 25;
	case 510: //Blizzard Blast
		return 25;
	case 511: //Affliction
		return 20;
	case 512: //Ensnare
		return 126;
	case 513: //Calm Animal
		return 126;
	case 514: //Terrorize Animal
		return 126;
	case 515: //Thistlecoat
		return 125;
	case 516: //Barbcoat
		return 125;
	case 517: //Bramblecoat
		return 125;
	case 518: //Spikecoat
		return 125;
	case 519: //Thorncoat
		return 125;
	case 520: //Dizzying Wind
		return 25;
	case 521: //Choke
		return 20;
	case 522: //Gather Shadows
		return 125;
	case 524: //Spirit Tap
		return 114;
	case 525: //Drain Spirit
		return 114;
	case 526: //Insidious Fever
		return 126;
	case 527: //Insidious Malady
		return 126;
	case 528: //Yonder
		return 125;
	case 529: //Gaze
		return 125;
	case 530: //Ring of Karana
		return 123;
	case 531: //Ring of Commons
		return 123;
	case 532: //Ring of Butcher
		return 123;
	case 533: //Ring of Toxxulia
		return 123;
	case 534: //Ring of Lavastorm
		return 123;
	case 535: //Ring of Ro
		return 123;
	case 536: //Ring of Feerrott
		return 123;
	case 537: //Ring of Steamfont
		return 123;
	case 538: //Ring of Misty
		return 123;
	case 539: //Chill Sight
		return 125;
	case 540: //Clarify Mana
		return 18;
	case 541: //Tox Gate
		return 123;
	case 542: //North Gate
		return 123;
	case 543: //Fay Gate
		return 123;
	case 544: //Common Gate
		return 123;
	case 545: //Nek Gate
		return 123;
	case 546: //Cazic Gate
		return 123;
	case 547: //Ro Gate
		return 123;
	case 548: //West Gate
		return 123;
	case 549: //Screaming Terror
		return 126;
	case 550: //Circle of Karana
		return 123;
	case 551: //Circle of Commons
		return 123;
	case 552: //Circle of Toxxulia
		return 123;
	case 553: //Circle of Butcher
		return 123;
	case 554: //Circle of Lavastorm
		return 123;
	case 555: //Circle of Ro
		return 123;
	case 556: //Circle of Feerrott
		return 123;
	case 557: //Circle of Steamfont
		return 123;
	case 558: //Circle of Misty
		return 123;
	case 559: //Ignite Bones
		return 25;
	case 560: //Furor
		return 25;
	case 561: //Tox Portal
		return 123;
	case 562: //North Portal
		return 123;
	case 563: //Fay Portal
		return 123;
	case 564: //Nek Portal
		return 123;
	case 565: //Cazic Portal
		return 123;
	case 566: //Common Portal
		return 123;
	case 567: //Ro Portal
		return 123;
	case 568: //West Portal
		return 123;
	case 569: //Summoning: Earth
		return 69;
	case 570: //Summoning: Water
		return 69;
	case 571: //Summoning: Fire
		return 69;
	case 572: //Summoning: Air
		return 69;
	case 573: //Greater Summoning: Earth
		return 69;
	case 574: //Greater Summoning: Water
		return 69;
	case 575: //Greater Summoning: Fire
		return 69;
	case 576: //Greater Summoning: Air
		return 69;
	case 577: //Vigilant Spirit
		return 69;
	case 578: //Sight
		return 125;
	case 579: //Magnify
		return 125;
	case 580: //Vision
		return 125;
	case 581: //Illusion: Skeleton
		return 125;
	case 582: //Illusion: Human
		return 125;
	case 583: //Illusion: Half-Elf
		return 125;
	case 584: //Illusion: Earth Elemental
		return 125;
	case 585: //Illusion: Werewolf
		return 125;
	case 586: //Illusion: Barbarian
		return 125;
	case 587: //Illusion: Erudite
		return 125;
	case 588: //Illusion: Wood Elf
		return 125;
	case 589: //Illusion: High Elf
		return 125;
	case 590: //Illusion: Dark Elf
		return 125;
	case 591: //Illusion: Dwarf
		return 125;
	case 592: //Illusion: Troll
		return 125;
	case 593: //Illusion: Ogre
		return 125;
	case 594: //Illusion: Halfling
		return 125;
	case 595: //Illusion: Gnome
		return 125;
	case 596: //Illusion: Dry Bone
		return 125;
	case 597: //Illusion: Air Elemental
		return 125;
	case 598: //Illusion: Fire Elemental
		return 125;
	case 599: //Illusion: Water Elemental
		return 125;
	case 600: //Illusion: Spirit Wolf
		return 125;
	case 601: //Illusion: Tree
		return 125;
	case 602: //Evacuate: North
		return 123;
	case 603: //Evacuate: Fay
		return 123;
	case 604: //Evacuate: Ro
		return 123;
	case 605: //Evacuate: Nek
		return 123;
	case 606: //Evacuate: West
		return 123;
	case 607: //Succor: East
		return 123;
	case 608: //Succor: Butcher
		return 123;
	case 609: //Succor: Ro
		return 123;
	case 610: //Succor: Lavastorm
		return 123;
	case 611: //Succor: North
		return 123;
	case 612: //Markar's Clash
		return 25;
	case 613: //Staff of Tracing
		return 18;
	case 614: //Staff of Warding
		return 18;
	case 615: //Staff of Runes
		return 18;
	case 616: //Staff of Symbols
		return 18;
	case 617: //Sword of Runes
		return 18;
	case 618: //Dimensional Hole
		return 18;
	case 619: //Dyn`s Dizzying Draught
		return 25;
	case 620: //Minor Conjuration: Earth
		return 69;
	case 621: //Minor Conjuration: Water
		return 69;
	case 622: //Minor Conjuration: Fire
		return 69;
	case 623: //Minor Conjuration: Air
		return 69;
	case 624: //Lesser Conjuration: Earth
		return 69;
	case 625: //Lesser Conjuration: Water
		return 69;
	case 626: //Lesser Conjuration: Fire
		return 69;
	case 627: //Lesser Conjuration: Air
		return 69;
	case 628: //Conjuration: Earth
		return 69;
	case 629: //Conjuration: Water
		return 69;
	case 630: //Conjuration: Fire
		return 69;
	case 631: //Conjuration: Air
		return 69;
	case 632: //Greater Conjuration: Earth
		return 69;
	case 633: //Greater Conjuration: Water
		return 69;
	case 634: //Greater Conjuration: Fire
		return 69;
	case 635: //Greater Conjuration: Air
		return 69;
	case 636: //Bonds of Force
		return 126;
	case 640: //Creeping Vision
		return 125;
	case 641: //Dark Pact
		return 125;
	case 642: //Allure of Death
		return 125;
	case 643: //Call of Bones
		return 125;
	case 644: //Lich
		return 125;
	case 645: //Ebbing Strength
		return 126;
	case 646: //Radiant Visage
		return 95;
	case 647: //Adorning Grace
		return 95;
	case 648: //Rampage
		return 125;
	case 649: //Protect
		return 95;
	case 650: //Mist
		return 95;
	case 651: //Cloud
		return 95;
	case 652: //Obscure
		return 95;
	case 653: //Shade
		return 95;
	case 654: //Shadow
		return 95;
	case 655: //Eyes of the Cat
		return 125;
	case 656: //Shock of Ice
		return 25;
	case 657: //Flame Shock
		return 25;
	case 658: //Ice Shock
		return 25;
	case 659: //Conflagration
		return 25;
	case 660: //Frost Storm
		return 25;
	case 661: //Augment Death
		return 69;
	case 662: //Expel Undead
		return 25;
	case 663: //Expulse Summoned
		return 25;
	case 664: //Expel Summoned
		return 25;
	case 665: //Drifting Death
		return 20;
	case 666: //Alter Plane: Hate
		return 123;
	case 667: //Enchant Silver
		return 18;
	case 668: //Enchant Electrum
		return 18;
	case 669: //Enchant Gold
		return 18;
	case 670: //Enchant Platinum
		return 18;
	case 671: //Starfire
		return 25;
	case 672: //Retribution
		return 25;
	case 673: //Discordant Mind
		return 25;
	case 674: //Alter Plane: Sky
		return 123;
	case 675: //Hammer of Requital
		return 18;
	case 676: //Tashan
		return 126;
	case 677: //Tashani
		return 126;
	case 678: //Tashania
		return 126;
	case 679: //Heat Sight
		return 125;
	case 680: //Barrier of Combustion
		return 125;
	case 681: //Juli`s Animation
		return 69;
	case 682: //Kilan`s Animation
		return 69;
	case 683: //Shalee`s Animation
		return 69;
	case 684: //Sisna`s Animation
		return 69;
	case 685: //Sagar`s Animation
		return 69;
	case 686: //Uleen`s Animation
		return 69;
	case 687: //Boltran`s Animation
		return 69;
	case 688: //Aanya's Animation
		return 69;
	case 689: //Yegoreff`s Animation
		return 69;
	case 690: //Kintaz`s Animation
		return 69;
	case 691: //Call of Flame
		return 25;
	case 692: //Life Leech
		return 114;
	case 693: //Divine Might
		return 125;
	case 694: //Pact of Shadow
		return 42;
	case 695: //Distill Mana
		return 18;
	case 696: //Purify Mana
		return 18;
	case 697: //Breeze
		return 79;
	case 698: //Track Corpse
		return 125;
	case 699: //Defoliate
		return 25;
	case 700: //Chant of Battle
		return 125;
	case 701: //Anthem de Arms
		return 125;
	case 702: //McVaxius` Berserker Crescendo
		return 125;
	case 703: //Chords of Dissonance
		return 20;
	case 704: //Brusco`s Boastful Bellow
		return 25;
	case 705: //Largo's Melodic Binding
		return 126;
	case 706: //Angstlich`s Appalling Screech
		return 126;
	case 707: //Fufil`s Curtailing Chant
		return 20;
	case 708: //Cinda`s Charismatic Carillon
		return 125;
	case 709: //Guardian Rhythms
		return 95;
	case 710: //Elemental Rhythms
		return 95;
	case 711: //Purifying Rhythms
		return 95;
	case 712: //Psalm of Warmth
		return 125;
	case 713: //Psalm of Cooling
		return 125;
	case 714: //Psalm of Mystic Shielding
		return 95;
	case 715: //Psalm of Vitality
		return 125;
	case 716: //Psalm of Purity
		return 125;
	case 717: //Selo`s Accelerando
		return 125;
	case 718: //Agilmente`s Aria of Eagles
		return 125;
	case 719: //Shauri`s Sonorous Clouding
		return 125;
	case 720: //Lyssa`s Locating Lyric
		return 125;
	case 721: //Lyssa`s Solidarity of Vision
		return 125;
	case 722: //Jaxan`s Jig o` Vigor
		return 42;
	case 723: //Cassindra's Chorus of Clarity
		return 79;
	case 724: //Kelin`s Lucid Lullaby
		return 126;
	case 725: //Solon's Song of the Sirens
		return 126;
	case 726: //Syvelian`s Anti-Magic Aria
		return 126;
	case 727: //Alenia`s Disenchanting Melody
		return 126;
	case 728: //Kelin`s Lugubrious Lament
		return 126;
	case 729: //Tarew`s Aquatic Ayre
		return 125;
	case 730: //Denon`s Disruptive Discord
		return 20;
	case 731: //Wrath of Al'Kabor
		return 25;
	case 732: //Ice Comet
		return 25;
	case 733: //Supernova
		return 25;
	case 734: //Jonthan's Whistling Warsong
		return 125;
	case 735: //Lyssa`s Veracious Concord
		return 125;
	case 736: //Denon`s Dissension
		return 126;
	case 737: //Lyssa`s Cataloging Libretto
		return 125;
	case 738: //Selo`s Consonant Chain
		return 126;
	case 739: //Melanie`s Mellifluous Motion
		return 125;
	case 740: //Vilia`s Verses of Celerity
		return 125;
	case 741: //Crission`s Pixie Strike
		return 126;
	case 742: //Denon`s Desperate Dirge
		return 25;
	case 743: //Tuyen`s Chant of Flame
		return 20;
	case 744: //Tuyen`s Chant of Frost
		return 20;
	case 745: //Cassindra`s Elegy
		return 95;
	case 746: //Selo`s Chords of Cessation
		return 126;
	case 747: //Verses of Victory
		return 125;
	case 748: //Niv`s Melody of Preservation
		return 125;
	case 749: //Jonthan's Provocation
		return 125;
	case 750: //Solon's Bewitching Bravura
		return 126;
	case 752: //Concussion
		return 126;
	case 753: //Beguile Plants
		return 126;
	case 754: //Cannibalize II
		return 125;
	case 755: //Rend
		return 25;
	case 761: //Contact Poison I
		return 25;
	case 763: //System Shock I
		return 25;
	case 767: //Liquid Silver I
		return 25;
	case 786: //Wurm Blaze
		return 25;
	case 792: //Fist of Fire
		return 25;
	case 793: //Fist of Air
		return 25;
	case 794: //Fist of Earth
		return 25;
	case 804: //Magi Bolt
		return 25;
	case 805: //Magi Strike
		return 25;
	case 807: //Magi Circle
		return 25;
	case 808: //Avatar Power
		return 25;
	case 812: //SumMonsterAttack
		return 25;
	case 817: //Guide Bolt
		return 25;
	case 823: //Divine Might Effect
		return 25;
	case 829: //FireHornet
		return 25;
	case 831: //Sathir's Gaze
		return 25;
	case 832: //WurmBreath
		return 25;
	case 834: //Sathir's Mesmerization
		return 25;
	case 835: //Chaos Breath
		return 25;
	case 837: //Stun Breath
		return 25;
	case 839: //Lightning Breath
		return 25;
	case 848: //Elemental Mastery Strike
		return 25;
	case 849: //ElementalMasteryBlast
		return 25;
	case 851: //Shardwurm Breath
		return 25;
	case 859: //Lava Breath - Test
		return 25;
	case 860: //DrakeBreath
		return 25;
	case 861: //Lava Breath
		return 25;
	case 862: //Frost Breath
		return 25;
	case 863: //Telekinesis
		return 25;
	case 868: //Sionachie`s Dreams
		return 126;
	case 893: //FireElementalAttack2
		return 25;
	case 904: //Knockback
		return 25;
	case 907: //DryBoneFireBurst
		return 25;
	case 908: //IceBoneFrostBurst
		return 25;
	case 910: //SnakeEleFireBurst
		return 25;
	case 917: //Smolder
		return 25;
	case 922: //Sonic
		return 25;
	case 929: //Harm Touch NPC
		return 25;
	case 931: //Life Drain
		return 25;
	case 945: //Ykesha
		return 25;
	case 951: //Fiery Death
		return 25;
	case 952: //Frosty Death
		return 25;
	case 966: //FireElementalAttack
		return 25;
	case 968: //WaterElementalAttack
		return 25;
	case 978: //FrostAOE
		return 25;
	case 982: //Cazic Touch
		return 25;
	case 985: //Efreeti Fire
		return 25;
	case 987: //Spiroc Thunder
		return 25;
	case 988: //Greater Spiroc Thunder
		return 25;
	case 989: //Entomb in Ice
		return 25;
	case 995: //Soul Devour
		return 25;
	case 1009: //FireBeetleSpit
		return 25;
	case 1017: //Fishnova
		return 25;
	case 1020: //Air Elemental Strike
		return 25;
	case 1021: //Water Elemental Strike
		return 25;
	case 1024: //Thunderclap
		return 25;
	case 1026: //Thunder Call
		return 25;
	case 1027: //Thunder Storm
		return 25;
	case 1028: //Static Storm
		return 25;
	case 1030: //Sand Storm
		return 25;
	case 1031: //Stone Gale
		return 25;
	case 1032: //Hail Storm
		return 25;
	case 1036: //Storm Flame
		return 25;
	case 1043: //Manastorm
		return 25;
	case 1045: //Chain Lightning
		return 25;
	case 1047: //Deluge
		return 25;
	case 1048: //Monsoons
		return 25;
	case 1049: //Tempest Wind
		return 25;
	case 1050: //Raging Blizzard
		return 25;
	case 1071: //Punishing Blow
		return 25;
	case 1074: //Steam Blast
		return 25;
	case 1075: //Electrical Short
		return 25;
	case 1077: //Mana Beam
		return 25;
	case 1078: //Gyrosonic Disruption
		return 25;
	case 1084: //Barrage of Debris
		return 25;
	case 1100: //Dreams of Ayonae
		return 126;
	case 1106: //Sear
		return 20;
	case 1107: //Tremor of Judgment
		return 25;
	case 1142: //Pain Harvest
		return 25;
	case 1144: //Jagged Rain
		return 25;
	case 1145: //Touch of Pain
		return 25;
	case 1151: //Raven Screech
		return 25;
	case 1155: //Black Symbol of Agony
		return 25;
	case 1167: //Draconic Rage Strike
		return 25;
	case 1168: //Draconic Rage Strike
		return 25;
	case 1172: //Sting of the Shissar
		return 25;
	case 1173: //Bite of the Shissar
		return 25;
	case 1180: //Zombie Bane
		return 25;
	case 1181: //Mayong's Bane
		return 25;
	case 1188: //Bixie Sting
		return 25;
	case 1189: //Scoriae Bite
		return 25;
	case 1194: //Illusion: Fier`dal
		return 125;
	case 1196: //Ancient: Lcea's Lament
		return 79;
	case 1197: //Ancient: Lullaby of Shadow
		return 126;
	case 1216: //Guide Bolt
		return 25;
	case 1221: //Terror of Darkness
		return 126;
	case 1222: //Terror of Shadows
		return 126;
	case 1223: //Terror of Death
		return 126;
	case 1224: //Terror of Terris
		return 126;
	case 1225: //Voice of Darkness
		return 125;
	case 1226: //Voice of Shadows
		return 125;
	case 1227: //Voice of Death
		return 125;
	case 1228: //Voice of Terris
		return 125;
	case 1244: //Magi Bolt
		return 25;
	case 1245: //Magi Strike
		return 25;
	case 1247: //Magi Circle
		return 25;
	case 1269: //Fangol's Breath
		return 25;
	case 1279: //Velium Chill of Al`Kabor
		return 25;
	case 1283: //Celestial Cleansing
		return 42;
	case 1284: //Valiant Companion
		return 69;
	case 1285: //Summon Companion
		return 69;
	case 1286: //Expedience
		return 69;
	case 1287: //Cassindra`s Chant of Clarity
		return 79;
	case 1288: //Divine Glory
		return 45;
	case 1289: //Strengthen Death
		return 69;
	case 1290: //Chloroblast
		return 42;
	case 1291: //Nature's Touch
		return 42;
	case 1296: //Cinder Jolt
		return 126;
	case 1310: //Porlos' Fury
		return 25;
	case 1311: //Hsagra's Wrath
		return 25;
	case 1314: //SpectraStun
		return 25;
	case 1317: //Repulse
		return 25;
	case 1325: //Combine Gate
		return 123;
	case 1326: //Ring of the Combines
		return 123;
	case 1332: //Cannibalize IV
		return 125;
	case 1334: //Translocate: Group
		return 123;
	case 1336: //Translocate: Fay
		return 123;
	case 1337: //Translocate: Tox
		return 123;
	case 1338: //Translocate: North
		return 123;
	case 1339: //Translocate: Combine
		return 123;
	case 1356: //Frosty Death2
		return 25;
	case 1359: //Enchant Clay
		return 18;
	case 1366: //Rage of the Sky
		return 25;
	case 1369: //Poisonous Chill
		return 25;
	case 1371: //Translocate: Nek
		return 123;
	case 1372: //Translocate: Common
		return 123;
	case 1373: //Translocate: Ro
		return 123;
	case 1374: //Translocate: West
		return 123;
	case 1375: //Translocate: Cazic
		return 123;
	case 1376: //Shroud of Undeath
		return 125;
	case 1377: //Primal Avatar
		return 95;
	case 1382: //Summon Holy Ale of Brell
		return 18;
	case 1391: //Dead Men Floating
		return 125;
	case 1392: //Fireburst
		return 25;
	case 1393: //Gangrenous Touch of Zum`uul
		return 114;
	case 1394: //Maelstrom of Electricity
		return 25;
	case 1397: //Strength of Nature
		return 45;
	case 1398: //Circle of Wakening Lands
		return 123;
	case 1399: //Wakening Lands Portal
		return 123;
	case 1400: //Monster Summoning I
		return 69;
	case 1401: //Summon Shard of the Core
		return 18;
	case 1402: //Monster Summoning II
		return 69;
	case 1403: //Elemental Maelstrom
		return 20;
	case 1404: //Monster Summoning III
		return 69;
	case 1405: //Wrath of the Elements
		return 20;
	case 1406: //Improved Invisibility
		return 125;
	case 1407: //Wandering Mind
		return 126;
	case 1408: //Gift of Magic
		return 95;
	case 1409: //Gift of Insight
		return 95;
	case 1410: //Gift of Brilliance
		return 95;
	case 1411: //Improved Invis to Undead
		return 125;
	case 1412: //Chilling Embrace
		return 20;
	case 1413: //Corporeal Empathy
		return 42;
	case 1414: //Augmentation of Death
		return 69;
	case 1415: //Torbas' Acid Blast
		return 25;
	case 1416: //Arch Lich
		return 125;
	case 1417: //Iceclad Gate
		return 123;
	case 1418: //Iceclad Portal
		return 123;
	case 1419: //O'Keils Flickering Flame
		return 125;
	case 1420: //Invisibility to Undead
		return 125;
	case 1421: //Enticement of Flame
		return 25;
	case 1422: //Translocate
		return 123;
	case 1423: //Great Divide Portal
		return 123;
	case 1425: //Cobalt Scar Portal
		return 123;
	case 1426: //Ice Spear of Solist
		return 25;
	case 1427: //Shock of the Tainted
		return 25;
	case 1428: //Tumultuous Strength
		return 95;
	case 1429: //Blast of Poison
		return 25;
	case 1430: //Spirit Quickening
		return 69;
	case 1431: //Form of the Great Bear
		return 125;
	case 1432: //Focus of Spirit
		return 45;
	case 1433: //Ring of Iceclad
		return 123;
	case 1434: //Circle of Iceclad
		return 123;
	case 1435: //Improved Superior Camouflage
		return 125;
	case 1436: //Fixation of Ro
		return 126;
	case 1437: //Ro's Fiery Sundering
		return 126;
	case 1438: //Circle of Great Divide
		return 123;
	case 1439: //Fury of Air
		return 25;
	case 1440: //Circle of Cobalt Scar
		return 123;
	case 1442: //Protection of the Glades
		return 45;
	case 1443: //Turning of the Unnatural
		return 20;
	case 1444: //Celestial Healing
		return 42;
	case 1445: //Armor of Protection
		return 45;
	case 1446: //Stun Command
		return 25;
	case 1447: //Aegolism
		return 45;
	case 1448: //Cantata of Soothing
		return 79;
	case 1449: //Melody of Ervaj
		return 125;
	case 1450: //Shield of Songs
		return 125;
	case 1451: //Occlusion of Sound
		return 126;
	case 1452: //Composition of Ervaj
		return 125;
	case 1453: //Divine Purpose
		return 125;
	case 1454: //Flame of Light
		return 25;
	case 1455: //Wave of Healing
		return 42;
	case 1456: //Divine Strength
		return 45;
	case 1457: //Shroud of Hate
		return 114;
	case 1458: //Shroud of Pain
		return 114;
	case 1459: //Shroud of Death
		return 125;
	case 1460: //Death Peace
		return 125;
	case 1461: //Call of Sky
		return 125;
	case 1462: //Call of Earth
		return 125;
	case 1463: //Call of Fire
		return 125;
	case 1464: //Call of the Predator
		return 95;
	case 1465: //Call of Sky Strike
		return 25;
	case 1467: //Call of Fire Strike
		return 25;
	case 1472: //Burnout IV
		return 69;
	case 1474: //Boon of the Garou
		return 125;
	case 1475: //Nature Walkers Behest
		return 69;
	case 1479: //Wave of Flame
		return 25;
	case 1480: //Silver Breath
		return 25;
	case 1481: //Scream of Chaos
		return 25;
	case 1482: //Electric Blast
		return 25;
	case 1484: //Tsunami
		return 25;
	case 1487: //Rain of Cold
		return 25;
	case 1488: //Rain of Molten Lava
		return 25;
	case 1489: //Wave of Cold
		return 25;
	case 1490: //Wave of Heat
		return 25;
	case 1494: //Flame Jet
		return 25;
	case 1498: //Doljons Rage
		return 25;
	case 1503: //Modulating Rod
		return 18;
	case 1504: //Renew Elements
		return 69;
	case 1505: //Renew Summoning
		return 69;
	case 1508: //Asystole
		return 20;
	case 1509: //Leach
		return 114;
	case 1510: //Shadow Compact
		return 42;
	case 1511: //Scent of Dusk
		return 126;
	case 1512: //Scent of Shadow
		return 126;
	case 1513: //Scent of Darkness
		return 126;
	case 1514: //Rapacious Subvention
		return 42;
	case 1515: //Covetous Subversion
		return 42;
	case 1516: //Combine Portal
		return 123;
	case 1517: //Circle of the Combines
		return 123;
	case 1518: //Remedy
		return 42;
	case 1519: //Divine Light
		return 42;
	case 1520: //Word of Vigor
		return 42;
	case 1521: //Word of Restoration
		return 42;
	case 1522: //Celestial Elixir
		return 42;
	case 1523: //Word of Redemption
		return 42;
	case 1524: //Reviviscence
		return 42;
	case 1525: //Antidote
		return 42;
	case 1526: //Annul Magic
		return 126;
	case 1527: //Trepidation
		return 126;
	case 1528: //Exile Undead
		return 25;
	case 1529: //Exile Summoned
		return 25;
	case 1530: //Banishment of Shadows
		return 25;
	case 1531: //Banishment
		return 25;
	case 1532: //Dread of Night
		return 126;
	case 1533: //Heroism
		return 45;
	case 1534: //Yaulp IV
		return 95;
	case 1535: //Symbol of Marzin
		return 45;
	case 1536: //Heroic Bond
		return 45;
	case 1537: //Bulwark of Faith
		return 95;
	case 1538: //Heroic Bond
		return 45;
	case 1539: //Fortitude
		return 45;
	case 1540: //Aegis
		return 95;
	case 1541: //Wake of Tranquility
		return 126;
	case 1542: //Upheaval
		return 25;
	case 1543: //Reckoning
		return 25;
	case 1544: //Enforced Reverence
		return 25;
	case 1545: //The Unspoken Word
		return 25;
	case 1546: //Divine Intervention
		return 125;
	case 1547: //Death Pact
		return 125;
	case 1548: //Mark of Karn
		return 125;
	case 1550: //Repulse Animal
		return 126;
	case 1551: //Circle of Winter
		return 95;
	case 1552: //Circle of Summer
		return 95;
	case 1553: //Call of Karana
		return 126;
	case 1554: //Spirit of Scale
		return 125;
	case 1555: //Glamour of Tunare
		return 126;
	case 1556: //Tunare's Request
		return 126;
	case 1557: //Girdle of Karana
		return 95;
	case 1558: //Bladecoat
		return 125;
	case 1559: //Natureskin
		return 45;
	case 1560: //Shield of Blades
		return 125;
	case 1561: //Legacy of Thorn
		return 125;
	case 1562: //Form of the Howler
		return 125;
	case 1563: //Form of the Hunter
		return 125;
	case 1564: //Spirit of Oak
		return 125;
	case 1565: //Mask of the Hunter
		return 79;
	case 1566: //Egress
		return 123;
	case 1567: //Succor
		return 123;
	case 1568: //Regrowth
		return 79;
	case 1569: //Regrowth of the Grove
		return 79;
	case 1570: //Talisman of Jasinth
		return 95;
	case 1571: //Talisman of Shadoo
		return 95;
	case 1572: //Cannibalize III
		return 125;
	case 1573: //Insidious Decay
		return 126;
	case 1574: //Spirit of the Howler
		return 69;
	case 1575: //Acumen
		return 125;
	case 1576: //Torpor
		return 42;
	case 1577: //Malosini
		return 126;
	case 1578: //Malo
		return 126;
	case 1579: //Talisman of the Cat
		return 95;
	case 1580: //Talisman of the Brute
		return 95;
	case 1581: //Talisman of the Rhino
		return 95;
	case 1582: //Talisman of the Serpent
		return 95;
	case 1583: //Talisman of the Raptor
		return 95;
	case 1584: //Shroud of the Spirits
		return 95;
	case 1585: //Talisman of Kragg
		return 45;
	case 1586: //Ice Strike
		return 25;
	case 1587: //Torrent of Poison
		return 25;
	case 1588: //Turgur's Insects
		return 126;
	case 1589: //Tigir's Insects
		return 126;
	case 1590: //Bane of Nife
		return 20;
	case 1591: //Pox of Bertoxxulous
		return 20;
	case 1592: //Cripple
		return 126;
	case 1593: //Maniacal Strength
		return 95;
	case 1594: //Deliriously Nimble
		return 95;
	case 1595: //Riotous Health
		return 95;
	case 1596: //Mortal Deftness
		return 95;
	case 1597: //Unfailing Reverence
		return 95;
	case 1598: //Avatar
		return 95;
	case 1599: //Voice of the Berserker
		return 95;
	case 1600: //Breath of Ro
		return 20;
	case 1601: //Winged Death
		return 20;
	case 1602: //Blizzard
		return 25;
	case 1603: //Scoriae
		return 25;
	case 1604: //Breath of Karana
		return 25;
	case 1605: //Frost
		return 25;
	case 1606: //Fist of Karana
		return 25;
	case 1607: //Wildfire
		return 25;
	case 1608: //Entrapping Roots
		return 126;
	case 1609: //Manaskin
		return 125;
	case 1610: //Shield of the Magi
		return 45;
	case 1611: //Demi Lich
		return 125;
	case 1612: //Quivering Veil of Xarn
		return 125;
	case 1613: //Deflux
		return 114;
	case 1614: //Chill Bones
		return 25;
	case 1615: //Cessation of Cor
		return 20;
	case 1616: //Vexing Mordinia
		return 114;
	case 1617: //Pyrocruor
		return 20;
	case 1618: //Touch of Night
		return 114;
	case 1619: //Devouring Darkness
		return 20;
	case 1620: //Splurt
		return 20;
	case 1621: //Minion of Shadows
		return 69;
	case 1622: //Servant of Bones
		return 69;
	case 1623: //Emissary of Thule
		return 69;
	case 1624: //Thrall of Bones
		return 126;
	case 1625: //Skin of the Shadow
		return 125;
	case 1626: //Levant
		return 123;
	case 1627: //Abscond
		return 123;
	case 1628: //Evacuate
		return 123;
	case 1629: //Enslave Death
		return 126;
	case 1630: //Defoliation
		return 25;
	case 1631: //Atol's Spectral Shackles
		return 126;
	case 1632: //Plainsight
		return 125;
	case 1633: //Fetter
		return 126;
	case 1634: //Tishan's Discord
		return 25;
	case 1635: //Markar's Discord
		return 25;
	case 1636: //Invert Gravity
		return 25;
	case 1637: //Draught of Fire
		return 25;
	case 1638: //Lure of Flame
		return 25;
	case 1639: //Voltaic Draught
		return 25;
	case 1640: //Lure of Lightning
		return 25;
	case 1641: //Draught of Ice
		return 25;
	case 1642: //Lure of Frost
		return 25;
	case 1643: //Draught of Jiva
		return 25;
	case 1644: //Pillar of Flame
		return 25;
	case 1645: //Pillar of Lightning
		return 25;
	case 1646: //Pillar of Frost
		return 25;
	case 1647: //Tears of Prexus
		return 25;
	case 1648: //Tears of Solusek
		return 25;
	case 1649: //Tears of Druzzil
		return 25;
	case 1650: //Inferno of Al'Kabor
		return 25;
	case 1651: //Retribution of Al'Kabor
		return 25;
	case 1652: //Vengeance of Al'Kabor
		return 25;
	case 1653: //Jyll's Static Pulse
		return 25;
	case 1654: //Jyll's Zephyr of Ice
		return 25;
	case 1655: //Jyll's Wave of Heat
		return 25;
	case 1656: //Thunderbold
		return 25;
	case 1657: //Winds of Gelid
		return 25;
	case 1658: //Sunstrike
		return 25;
	case 1659: //Scintillation
		return 25;
	case 1660: //Char
		return 25;
	case 1661: //Scars of Sigil
		return 25;
	case 1662: //Sirocco
		return 25;
	case 1663: //Shock of Steel
		return 25;
	case 1664: //Seeking Flame of Seukor
		return 25;
	case 1665: //Manastorm
		return 25;
	case 1666: //Phantom Armor
		return 79;
	case 1667: //Cadeau of Flame
		return 125;
	case 1668: //Boon of Immolation
		return 125;
	case 1669: //Aegis of Ro
		return 125;
	case 1670: //Velocity
		return 69;
	case 1671: //Vocarate: Earth
		return 69;
	case 1672: //Vocarate: Water
		return 69;
	case 1673: //Vocarate: Fire
		return 69;
	case 1674: //Vocarate: Air
		return 69;
	case 1675: //Greater Vocaration: Earth
		return 69;
	case 1676: //Greater Vocaration: Water
		return 69;
	case 1677: //Greater Vocaration: Fire
		return 69;
	case 1678: //Greater Vocaration: Air
		return 69;
	case 1679: //Dyzil's Deafening Decoy
		return 69;
	case 1680: //Gift of Xev
		return 18;
	case 1681: //Bristlebane's Bundle
		return 18;
	case 1682: //Quiver of Marr
		return 18;
	case 1683: //Bandoleer of Luclin
		return 18;
	case 1684: //Pouch of Quellious
		return 18;
	case 1685: //Muzzle of Mardu
		return 18;
	case 1686: //Theft of Thought
		return 126;
	case 1687: //Collaboration
		return 125;
	case 1688: //Enlightenment
		return 95;
	case 1689: //Rune V
		return 125;
	case 1690: //Fascination
		return 126;
	case 1691: //Glamour of Kintaz
		return 126;
	case 1692: //Rapture
		return 126;
	case 1693: //Clarity II
		return 79;
	case 1694: //Boon of the Clear Mind
		return 79;
	case 1695: //Gift of Pure Thought
		return 79;
	case 1696: //Color Slant
		return 25;
	case 1697: //Recant Magic
		return 126;
	case 1698: //Dementia
		return 25;
	case 1699: //Wind of Tashani
		return 126;
	case 1700: //Torment of Argli
		return 20;
	case 1701: //Overwhelming Splendor
		return 95;
	case 1702: //Tashanian
		return 126;
	case 1703: //Asphyxiate
		return 20;
	case 1704: //Wind of Tashanian
		return 126;
	case 1705: //Boltran`s Agacerie
		return 126;
	case 1707: //Dictate
		return 126;
	case 1708: //Aanya's Quickening
		return 125;
	case 1709: //Wonderous Rapidity
		return 125;
	case 1710: //Visions of Grandeur
		return 125;
	case 1711: //Umbra
		return 95;
	case 1712: //Forlorn Deeds
		return 126;
	case 1713: //Bedlam
		return 125;
	case 1714: //Memory Flux
		return 126;
	case 1715: //Largarn's Lamentation
		return 126;
	case 1716: //Scent of Terris
		return 126;
	case 1717: //Shadowbond
		return 42;
	case 1718: //Sedulous Subversion
		return 42;
	case 1719: //Engorging Roots
		return 126;
	case 1720: //Eye of Tallon
		return 125;
	case 1721: //Unswerving Hammer of Faith
		return 69;
	case 1722: //Flaming Sword of Xuzl
		return 69;
	case 1723: //Zumaik`s Animation
		return 69;
	case 1724: //Disintegrate
		return 25;
	case 1725: //Wake of Karana
		return 125;
	case 1726: //Sunskin
		return 125;
	case 1727: //Legacy of Spike
		return 125;
	case 1728: //Manasink
		return 125;
	case 1729: //Augment
		return 125;
	case 1733: //Convergence
		return 42;
	case 1734: //Infusion
		return 42;
	case 1735: //Trucidation
		return 114;
	case 1736: //Wind of the North
		return 123;
	case 1737: //Wind of the South
		return 123;
	case 1738: //Tishan's Relocation
		return 123;
	case 1739: //Markar's Relocation
		return 123;
	case 1740: //Dustdevil
		return 25;
	case 1741: //Jolt
		return 126;
	case 1742: //Bobbing Corpse
		return 125;
	case 1743: //Divine Favor
		return 45;
	case 1744: //Harvest
		return 42;
	case 1747: //Brusco`s Bombastic Bellow
		return 25;
	case 1748: //Angstlich's Assonance
		return 20;
	case 1749: //Kazumi's Note of Preservation
		return 125;
	case 1750: //Selo`s Song of Travel
		return 125;
	case 1751: //Largo`s Absonant Binding
		return 126;
	case 1752: //Nillipus` March of the Wee
		return 125;
	case 1753: //Song of Twilight
		return 126;
	case 1754: //Song of Dawn
		return 126;
	case 1755: //Song of Highsun
		return 125;
	case 1756: //Song of Midnight
		return 126;
	case 1757: //Vilia`s Chorus of Celerity
		return 125;
	case 1758: //Selo`s Assonant Strane
		return 126;
	case 1759: //Cantata of Replenishment
		return 79;
	case 1760: //McVaxius` Rousing Rondo
		return 125;
	case 1761: //Cassindra's Insipid Ditty
		return 126;
	case 1762: //Jonthan's Inspiration
		return 125;
	case 1763: //Niv`s Harmonic
		return 95;
	case 1764: //Denon`s Bereavement
		return 20;
	case 1765: //Solon's Charismatic Concord
		return 95;
	case 1767: //Bonds of Tunare
		return 126;
	case 1768: //Sacrifice
		return 18;
	case 1769: //Lure of Ice
		return 25;
	case 1770: //Rage of Zomm
		return 69;
	case 1771: //Call of the Hero
		return 125;
	case 1772: //Mala
		return 126;
	case 1773: //Conjure Corpse
		return 125;
	case 1774: //Naltron's Mark
		return 45;
	case 1776: //Spirit of Wolf
		return 125;
	case 1784: //Velium Shards
		return 25;
	case 1785: //Flamesong
		return 25;
	case 1793: //Judgment of Ice
		return 25;
	case 1794: //Shards of Sorrow
		return 25;
	case 1797: //Enchant Velium
		return 18;
	case 1798: //Imbue Opal
		return 18;
	case 1799: //Imbue Topaz
		return 18;
	case 1800: //Imbue Plains Pebble
		return 18;
	case 1802: //Storm Strike
		return 25;
	case 1803: //Shrieking Howl
		return 25;
	case 1807: //Stunning Blow
		return 25;
	case 1812: //Nature's Wrath
		return 25;
	case 1815: //Flames of Ro
		return 25;
	case 1819: //Primal Essence
		return 95;
	case 1820: //Divine Wrath
		return 25;
	case 1827: //Frost Shards
		return 25;
	case 1831: //Diminution
		return 125;
	case 1834: //Poison Animal I
		return 25;
	case 1835: //Poison Summoned I
		return 25;
	case 1843: //Poison Animal II
		return 25;
	case 1844: //Poison Animal III
		return 25;
	case 1845: //Poison Summoned II
		return 25;
	case 1846: //Poison Summoned III
		return 25;
	case 1853: //Contact Poison II
		return 25;
	case 1854: //Contact Poison III
		return 25;
	case 1855: //Contact Poison IV
		return 25;
	case 1860: //System Shock II
		return 25;
	case 1861: //System Shock III
		return 25;
	case 1862: //System Shock IV
		return 25;
	case 1870: //Liquid Silver II
		return 25;
	case 1871: //Liquid Silver III
		return 25;
	case 1874: //Ant Legs
		return 125;
	case 1881: //System Shock V
		return 25;
	case 1884: //Imbue Ivory
		return 18;
	case 1885: //Imbue Amber
		return 18;
	case 1886: //Imbue Sapphire
		return 18;
	case 1887: //Imbue Ruby
		return 18;
	case 1888: //Imbue Emerald
		return 18;
	case 1889: //Enchant Mithril
		return 18;
	case 1890: //Enchant Adamantite
		return 18;
	case 1891: //Imbue Jade
		return 18;
	case 1892: //Enchant Steel
		return 18;
	case 1893: //Enchant Brellium
		return 18;
	case 1894: //Imbue Black Pearl
		return 18;
	case 1895: //Imbue Diamond
		return 18;
	case 1896: //Imbue Rose Quartz
		return 18;
	case 1897: //Imbue Black Sapphire
		return 18;
	case 1898: //Imbue Peridot
		return 18;
	case 1899: //Imbue Fire Opal
		return 18;
	case 1941: //Lava Breath
		return 25;
	case 1942: //Frost Breath
		return 25;
	case 1943: //Molten Breath
		return 25;
	case 1944: //Summon Orb
		return 18;
	case 1947: //Ice Rend
		return 25;
	case 1948: //Destroy
		return 25;
	case 1953: //Mastodon Stomp
		return 25;
	case 1954: //Devour Soul
		return 25;
	case 1955: //DrakeBreathBig
		return 25;
	case 1957: //Holy Shock
		return 25;
	case 1968: //Stunning Strike
		return 25;
	case 1969: //Flame of the Efreeti
		return 25;
	case 1970: //Verlekarnorm's Disaster
		return 25;
	case 1971: //Rocksmash
		return 25;
	case 2005: //Nature's Holy Wrath
		return 25;
	case 2006: //Static
		return 25;
	case 2014: //Incinerate Bones
		return 25;
	case 2015: //Conglaciation of Bone
		return 25;
	case 2016: //Dementing Visions
		return 25;
	case 2019: //Thunder Strike
		return 25;
	case 2020: //Circle of Surefall Glade
		return 123;
	case 2021: //Ring of Surefall Glade
		return 123;
	case 2022: //Translocate: Iceclad
		return 123;
	case 2023: //Translocate: Great Divide
		return 123;
	case 2024: //Translocate: Wakening Lands
		return 123;
	case 2025: //Translocate: Cobalt Scar
		return 123;
	case 2026: //Great Divide Gate
		return 123;
	case 2027: //Wakening Lands Gate
		return 123;
	case 2028: //Cobalt Scar Gate
		return 123;
	case 2029: //Ring of Great Divide
		return 123;
	case 2030: //Ring of Wakening Lands
		return 123;
	case 2031: //Ring of Cobalt Scar
		return 123;
	case 2035: //Tentacle Sting
		return 25;
	case 2036: //Rain of the Arch Mage
		return 25;
	case 2040: //Winds of the Archmage
		return 25;
	case 2043: //Kambooz's Touch
		return 25;
	case 2047: //Death Shackles
		return 25;
	case 2048: //Ssraeshza's Command
		return 25;
	case 2054: //Icy Claws
		return 25;
	case 2068: //Blast of Frost
		return 25;
	case 2070: //Marauder's Wrath
		return 25;
	case 2075: //Umbral Rot
		return 25;
	case 2076: //Presence of Ssraeshza
		return 25;
	case 2085: //Lesser Infusion 
		return 25;
	case 2086: //Infusion
		return 25;
	case 2087: //Greater Infusion
		return 25;
	case 2091: //Lesser Rejuvenation
		return 25;
	case 2092: //Rejuvination 
		return 25;
	case 2093: //Greater Rejuvenation
		return 25;
	case 2094: //Zruk Breath
		return 25;
	case 2101: //Pain and Suffering
		return 25;
	case 2102: //Drake Breath
		return 25;
	case 2103: //Drake Breath
		return 25;
	case 2104: //Drake Breath
		return 25;
	case 2105: //Drake Breath
		return 25;
	case 2106: //Gift of A'err
		return 25;
	case 2109: //Ancient: High Priest's Bulwark
		return 45;
	case 2110: //Skin like Wood
		return 45;
	case 2111: //Burst of Flame
		return 25;
	case 2112: //Ancient: Feral Avatar
		return 95;
	case 2113: //Ancient: Scourge of Nife
		return 20;
	case 2114: //Ancient: Master of Death
		return 125;
	case 2115: //Ancient: Lifebane
		return 25;
	case 2116: //Ancient: Destruction of Ice
		return 25;
	case 2117: //Ancient: Greater Concussion
		return 126;
	case 2118: //Ancient: Shock of Sun
		return 25;
	case 2119: //Ancient: Burnout Blaze
		return 69;
	case 2120: //Ancient: Eternal Rapture
		return 126;
	case 2121: //Ancient: Chaotic Visions
		return 25;
	case 2122: //Ancient: Gift of Aegolism
		return 45;
	case 2125: //Ancient: Legacy of Blades
		return 125;
	case 2126: //Ancient: Starfire of Ro
		return 25;
	case 2127: //Tragedy at Cazic Thule
		return 25;
	case 2130: //Horrific Force
		return 25;
	case 2131: //Vortex of Horror
		return 25;
	case 2137: //Rain of Terror
		return 25;
	case 2139: //Corpse Breath
		return 125;
	case 2156: //Deadly Curse of Noqufiel
		return 25;
	case 2157: //Word of Command
		return 25;
	case 2161: //Shock of Shadows
		return 25;
	case 2162: //Black Winds
		return 25;
	case 2163: //Lure of Shadows
		return 25;
	case 2167: //Fling
		return 25;
	case 2168: //Reanimation
		return 42;
	case 2169: //Reconstitution
		return 42;
	case 2170: //Reparation
		return 42;
	case 2171: //Renewal
		return 42;
	case 2172: //Restoration
		return 42;
	case 2173: //Hand of the Gods
		return 25;
	case 2175: //Celestial Health
		return 42;
	case 2176: //Spiritual Light
		return 79;
	case 2177: //Spiritual Radiance
		return 79;
	case 2178: //Spiritual Brawn
		return 45;
	case 2179: //Tunare's Renewal
		return 42;
	case 2180: //Ethereal Elixir
		return 42;
	case 2181: //Hammer of Judgment
		return 18;
	case 2182: //Ethereal Light
		return 42;
	case 2183: //Lesser Succor
		return 123;
	case 2184: //Lesser Evacuate
		return 123;
	case 2188: //Protection of the Cabbage
		return 45;
	case 2190: //Divine Stun
		return 25;
	case 2202: //Mana Shield
		return 125;
	case 2203: //Donlo's Dementia
		return 125;
	case 2206: //Tortured Memory
		return 25;
	case 2213: //Lesser Summon Corpse
		return 125;
	case 2230: //Summon Brass Choker
		return 18;
	case 2231: //Summon Silver Choker
		return 18;
	case 2232: //Summon Golden Choker
		return 18;
	case 2233: //Summon Linen Mantle
		return 18;
	case 2234: //Summon Leather Mantle
		return 18;
	case 2235: //Summon Silken Mantle
		return 18;
	case 2236: //Summon Jade Bracelet
		return 18;
	case 2237: //Summon Opal Bracelet
		return 18;
	case 2238: //Summon Ruby Bracelet
		return 18;
	case 2239: //Summon Tiny Ring
		return 18;
	case 2240: //Summon Twisted Ring
		return 18;
	case 2241: //Summon Studded Ring
		return 18;
	case 2242: //Summon Tarnished Bauble
		return 18;
	case 2243: //Summon Shiny Bauble
		return 18;
	case 2244: //Summon Brilliant Bauble
		return 18;
	case 2248: //Acumen
		return 125;
	case 2249: //River's Rancor
		return 25;
	case 2250: //Fiery Retribution
		return 25;
	case 2251: //Furor of the Wild
		return 25;
	case 2255: //Wrath of the Wild
		return 25;
	case 2258: //Frigid Dominion
		return 25;
	case 2261: //Frozen Torrent
		return 25;
	case 2264: //Hail of Ice
		return 25;
	case 2268: //Touch of the Void
		return 25;
	case 2312: //Life Bind
		return 79;
	case 2321: //Energy Burst
		return 25;
	case 2326: //Yaulp V
		return 95;
	case 2375: //Spectral Essence
		return 25;
	case 2377: //Screeching Ricochet
		return 25;
	case 2378: //Drakeen Breath
		return 25;
	case 2379: //Drakeen Monsoon
		return 25;
	case 2380: //Drakeen Vortex
		return 25;
	case 2381: //Wing Draft
		return 25;
	case 2382: //Wing Gust
		return 25;
	case 2383: //Wing Squall
		return 25;
	case 2384: //Wing Tempest
		return 25;
	case 2385: //Frost Pummel
		return 25;
	case 2386: //Ice Pummel
		return 25;
	case 2387: //Frigid Shard Pummel
		return 25;
	case 2392: //Sweltering Carcass
		return 25;
	case 2417: //Ring of Grimling
		return 123;
	case 2418: //Grimling Gate
		return 123;
	case 2419: //Circle of Grimling
		return 123;
	case 2420: //Grimling Portal
		return 123;
	case 2421: //Translocate: Grimling
		return 123;
	case 2422: //Ring of Twilight
		return 123;
	case 2423: //Twilight Gate
		return 123;
	case 2424: //Circle of Twilight
		return 123;
	case 2425: //Twilight Portal
		return 123;
	case 2426: //Translocate: Twilight
		return 123;
	case 2427: //Ring of Dawnshroud
		return 123;
	case 2428: //Dawnshroud Gate
		return 123;
	case 2429: //Circle of Dawnshroud
		return 123;
	case 2430: //Dawnshroud Portal
		return 123;
	case 2431: //Translocate: Dawnshroud
		return 123;
	case 2432: //Circle of the Nexus
		return 123;
	case 2433: //Ring of the Nexus
		return 123;
	case 2434: //Avatar
		return 95;
	case 2435: //Kragg's Mending
		return 42;
	case 2436: //War Arrows
		return 25;
	case 2437: //Hendin Arrow
		return 25;
	case 2443: //Blade of Vallon
		return 25;
	case 2450: //Barb of Tallon
		return 25;
	case 2452: //Barb of Tallon
		return 25;
	case 2453: //Thorns of Drunder
		return 25;
	case 2462: //Ethereal Remedy
		return 42;
	case 2501: //Sanctuary
		return 125;
	case 2502: //Celestial Remedy
		return 42;
	case 2503: //Sermon of the Righteous
		return 20;
	case 2504: //Sacred Word
		return 25;
	case 2505: //Armor of the Faithful
		return 45;
	case 2506: //Epitaph of Life
		return 20;
	case 2507: //Mark of Retribution
		return 125;
	case 2508: //Judgment
		return 25;
	case 2509: //Blessed Armor of the Risen
		return 45;
	case 2510: //Blessing of Aegolism
		return 45;
	case 2511: //Protection of Wood
		return 45;
	case 2512: //Protection of Rock
		return 45;
	case 2513: //Protection of Steel
		return 45;
	case 2514: //Protection of Diamond
		return 45;
	case 2515: //Protection of Nature
		return 45;
	case 2516: //Foliage Shield
		return 125;
	case 2517: //Spirit of Eagle
		return 125;
	case 2518: //Ro's Smoldering Disjunction
		return 126;
	case 2519: //Circle of Seasons
		return 95;
	case 2520: //Nature's Recovery
		return 42;
	case 2521: //Talisman of the Beast
		return 95;
	case 2522: //Grow
		return 125;
	case 2523: //Form of the Bear
		return 125;
	case 2524: //Spirit of Bih`Li
		return 125;
	case 2525: //Harnessing of Spirit
		return 45;
	case 2526: //Disinfecting Aura
		return 42;
	case 2527: //Plague of Insects
		return 126;
	case 2528: //Regrowth of Dar Khura
		return 79;
	case 2529: //Talisman of Epuration
		return 95;
	case 2530: //Khura's Focusing
		return 45;
	case 2531: //Summon Elemental Defender
		return 18;
	case 2532: //Summon Phantom Leather
		return 18;
	case 2533: //Summon Phantom Chain
		return 18;
	case 2534: //Summon Phantom Plate
		return 18;
	case 2535: //Summon Elemental Blanket
		return 18;
	case 2536: //Transon's Elemental Infusion
		return 69;
	case 2537: //Veil of Elements
		return 125;
	case 2538: //Mass Mystical Transvergance
		return 18;
	case 2539: //Transon's Phantasmal Protection
		return 45;
	case 2540: //Shock of Fiery Blades
		return 25;
	case 2541: //Focus Death
		return 69;
	case 2542: //Shackle of Bone
		return 126;
	case 2543: //Eternities Torment
		return 20;
	case 2544: //Shackle of Spirit
		return 126;
	case 2545: //Insidious Retrogression
		return 126;
	case 2546: //Degeneration
		return 114;
	case 2547: //Succussion of Shadows
		return 114;
	case 2548: //Crippling Claudication
		return 114;
	case 2549: //Mind Wrack
		return 126;
	case 2550: //Zevfeer's Theft of Vitae
		return 114;
	case 2551: //O'Keils Embers
		return 125;
	case 2552: //Garrisons Mighty Mana Shock
		return 25;
	case 2553: //Minor Familiar
		return 69;
	case 2554: //Elnerick's Entombment of Ice
		return 126;
	case 2555: //Lesser Familiar
		return 69;
	case 2556: //Firetree's Familiar Augment
		return 69;
	case 2557: //Familiar
		return 69;
	case 2558: //Decession
		return 123;
	case 2559: //Spellshield
		return 125;
	case 2560: //Greater Familiar
		return 69;
	case 2561: //Intellectual Advancement
		return 95;
	case 2562: //Intellectual Superiority
		return 95;
	case 2563: //Haunting Visage
		return 125;
	case 2564: //Calming Visage
		return 125;
	case 2565: //Illusion: Imp
		return 125;
	case 2566: //Trickster's Augmentation
		return 125;
	case 2567: //Beguiling Visage
		return 125;
	case 2568: //Horrifying Visage
		return 125;
	case 2569: //Glamorous Visage
		return 125;
	case 2570: //Koadic's Endless Intellect
		return 79;
	case 2571: //Despair
		return 114;
	case 2572: //Scream of Hate
		return 114;
	case 2573: //Scream of Pain
		return 114;
	case 2574: //Scream of Death
		return 125;
	case 2575: //Abduction of Strength
		return 114;
	case 2576: //Mental Corruption
		return 125;
	case 2577: //Torrent of Hate
		return 114;
	case 2578: //Torrent of Pain
		return 114;
	case 2579: //Torrent of Fatigue
		return 114;
	case 2580: //Cloak of the Akheva
		return 45;
	case 2581: //Cease
		return 25;
	case 2582: //Desist
		return 25;
	case 2583: //Instrument of Nife
		return 125;
	case 2584: //Divine Vigor
		return 45;
	case 2585: //Valor of Marr
		return 125;
	case 2586: //Thunder of Karana
		return 126;
	case 2587: //Quellious' Word of Tranquility
		return 25;
	case 2588: //Breath of Tunare
		return 125;
	case 2589: //Healing Wave of Prexus
		return 42;
	case 2590: //Brell's Mountainous Barrier
		return 45;
	case 2591: //Tangling Weeds
		return 126;
	case 2592: //Hawk Eye
		return 125;
	case 2593: //Riftwind's Protection
		return 125;
	case 2594: //Nature's Precision
		return 95;
	case 2595: //Force of Nature
		return 125;
	case 2596: //Falcon Eye
		return 125;
	case 2597: //Jolting Blades
		return 126;
	case 2598: //Mark of the Predator
		return 95;
	case 2599: //Eagle Eye
		return 125;
	case 2600: //Warder's Protection
		return 45;
	case 2601: //Magical Monologue
		return 125;
	case 2602: //Song of Sustenance
		return 125;
	case 2603: //Amplification
		return 125;
	case 2604: //Katta's Song of Sword Dancing
		return 125;
	case 2605: //Selo`s Accelerating Chorus
		return 125;
	case 2606: //Battlecry of the Vah Shir
		return 125;
	case 2607: //Elemental Chorus
		return 95;
	case 2608: //Purifying Chorus
		return 95;
	case 2609: //Chorus of Replenishment
		return 79;
	case 2610: //Warsong of the Vah Shir
		return 125;
	case 2611: //Sharik's Replenishing
		return 69;
	case 2612: //Spirit of Sharik
		return 69;
	case 2613: //Keshuval's Rejuvenation
		return 69;
	case 2614: //Spirit of Keshuval
		return 69;
	case 2615: //Herikol's Soothing
		return 69;
	case 2616: //Spirit of Herikol
		return 69;
	case 2617: //Yekan's Recovery
		return 69;
	case 2618: //Spirit of Yekan
		return 69;
	case 2619: //Yekan's Quickening
		return 69;
	case 2620: //Vigor of Zehkes
		return 69;
	case 2621: //Spirit of Kashek
		return 69;
	case 2622: //Aid of Khurenz
		return 69;
	case 2623: //Spirit of Omakin
		return 69;
	case 2624: //Sha's Restoration
		return 69;
	case 2625: //Omakin's Alacrity
		return 69;
	case 2626: //Spirit of Zehkes
		return 69;
	case 2627: //Spirit of Khurenz
		return 69;
	case 2628: //Sha's Ferocity
		return 69;
	case 2629: //Spiritual Purity
		return 79;
	case 2630: //Spiritual Strength
		return 45;
	case 2631: //Spirit of Khati Sha
		return 69;
	case 2632: //Summon Warder
		return 69;
	case 2633: //Spirit of Khaliz
		return 69;
	case 2634: //Sha's Lethargy
		return 126;
	case 2635: //Spirit of Lightning
		return 69;
	case 2636: //Spirit of the Blizzard
		return 69;
	case 2637: //Spirit of Inferno
		return 69;
	case 2638: //Spirit of the Scorpion
		return 69;
	case 2639: //Spirit of Vermin
		return 69;
	case 2640: //Spirit of Wind
		return 69;
	case 2641: //Spirit of the Storm
		return 69;
	case 2642: //Claw of Khati Sha
		return 25;
	case 2650: //Blazing Heat
		return 25;
	case 2651: //Vibrant Might
		return 25;
	case 2652: //Descending Might
		return 25;
	case 2654: //Fireblast
		return 25;
	case 2656: //Wrathful Strike
		return 25;
	case 2657: //Terrifying Darkness
		return 25;
	case 2658: //Lightning Surge
		return 25;
	case 2662: //Storm of Lightning
		return 25;
	case 2663: //Clash of Will
		return 25;
	case 2664: //Frostcall
		return 25;
	case 2665: //Wintercall
		return 25;
	case 2669: //Storm of Ice
		return 25;
	case 2670: //Rebuke the Dead
		return 25;
	case 2678: //Fungal Vengeance
		return 25;
	case 2710: //Trickster's Torment
		return 25;
	case 2711: //Trickster's TormentSK
		return 25;
	case 2717: //Mental Corruption Strike
		return 25;
	case 2720: //Spirit of Lightning Strike
		return 25;
	case 2721: //Spirit of Blizzard Strike
		return 25;
	case 2722: //Spirit of Inferno Strike
		return 25;
	case 2723: //Spirit of Scorpion Strike
		return 25;
	case 2724: //Spirit of Vermin Strike
		return 25;
	case 2725: //Spirit of Wind Strike
		return 25;
	case 2726: //Spirit of Storm Strike
		return 25;
	case 2729: //Condemnation of Nife
		return 25;
	case 2732: //Molten Fist
		return 25;
	case 2734: //The Nexus
		return 123;
	case 2736: //SpellTheft1
		return 126;
	case 2742: //Purify Soul
		return 42;
	case 2750: //Rabid Bear
		return 125;
	case 2752: //Restore Companion
		return 69;
	case 2754: //Frenzied Burnout
		return 69;
	case 2757: //Wave of Revulsion
		return 126;
	case 2758: //Improved Familiar
		return 69;
	case 2759: //Undead Pact
		return 126;
	case 2761: //Dominating Gaze
		return 126;
	case 2762: //Disease Touch
		return 25;
	case 2763: //Poison Touch
		return 25;
	case 2764: //Call to Corpse
		return 125;
	case 2766: //Life Curse
		return 114;
	case 2767: //Dragon Force
		return 25;
	case 2768: //Grimling LT 30
		return 25;
	case 2770: //Rain of Spores
		return 25;
	case 2802: //Flurry of Pebbles
		return 25;
	case 2809: //Wave of Death
		return 25;
	case 2816: //Storm Tremor
		return 25;
	case 2822: //Upheaval
		return 25;
	case 2826: //Illusion: Vah Shir
		return 125;
	case 2833: //AdvisorNova
		return 25;
	case 2836: //Grimling Comet
		return 25;
	case 2842: //Shrieker Stun
		return 25;
	case 2858: //AcryliaKB
		return 25;
	case 2859: //Touch of Vinitras
		return 25;
	case 2877: //Moonfire
		return 25;
	case 2878: //Fireclaw
		return 25;
	case 2879: //Phantasmal Armor
		return 79;
	case 2880: //Remove Greater Curse
		return 42;
	case 2881: //Everlasting Breath
		return 125;
	case 2882: //Firetree's Familiar Enhancement
		return 69;
	case 2883: //Elnerick's Electrical Rending
		return 25;
	case 2884: //Garrison's Superior Sundering
		return 25;
	case 2885: //Funeral Pyre of Kelador
		return 20;
	case 2886: //Acumen of Dar Khura
		return 125;
	case 2887: //Mask of the Stalker
		return 79;
	case 2888: //Spirit of Flame
		return 69;
	case 2889: //Spirit of Flame Strike
		return 25;
	case 2890: //Spirit of Snow
		return 69;
	case 2891: //Spirit of Snow Strike
		return 25;
	case 2892: //Deathly Temptation
		return 125;
	case 2893: //Marzin's Mark
		return 45;
	case 2894: //Levitation
		return 125;
	case 2895: //Speed of the Brood
		return 125;
	case 2896: //Transon's Elemental Renewal
		return 69;
	case 2901: //Illumination
		return 25;
	case 2902: //Shissar Broodling Poison
		return 25;
	case 2908: //Banshee Wail
		return 25;
	case 2927: //Storm Tremor
		return 25;
	case 2936: //Ervaj's Lost Composition
		return 126;
	case 2941: //Savagery
		return 95;
	case 2942: //Sha's Advantage
		return 126;
	case 2943: //Translocate: Nexus
		return 123;
	case 2944: //Nexus Portal
		return 123;
	case 2945: //Nexus Gate
		return 123;
	case 2946: //Remove Curse
		return 42;
	case 2950: //Grol Baku Strike
		return 25;
	case 2951: //Grol Baku Strike
		return 25;
	case 2952: //Strike of the Grol Baku
		return 25;
	case 2956: //Fire Blast
		return 25;
	case 2957: //Water Blast
		return 25;
	case 2969: //Shadow Creep
		return 114;
	case 2973: //Ember Strike
		return 25;
	case 2984: //Lotus Spines
		return 25;
	case 2988: //Wave of Toxicity
		return 25;
	case 2991: //Deathly Ice
		return 25;
	case 2992: //Deathly Fire
		return 25;
	case 2993: //Deathly Spores
		return 25;
	case 2994: //Deathly Fever
		return 25;
	case 2995: //Deep Spores
		return 25;
	case 2996: //Claw of the Hunter
		return 25;
	case 2997: //Claw of the Beast
		return 25;
	case 2999: //Claw of Bestial Fury
		return 25;
	case 3000: //Devouring Nightmare
		return 25;
	case 3004: //Fist of Lava
		return 25;
	case 3005: //Ball of Lava
		return 25;
	case 3013: //Fiery Strike
		return 25;
	case 3017: //Mighty Bellow of Fire
		return 25;
	case 3018: //Nova Inferno
		return 25;
	case 3020: //Rain of Burning Fire
		return 25;
	case 3030: //Dreams of Thule
		return 126;
	case 3031: //Xegony's Phantasmal Guard
		return 79;
	case 3032: //Touch of Mujaki
		return 114;
	case 3034: //Aeldorb's Animation
		return 69;
	case 3035: //Neurotoxin
		return 25;
	case 3036: //Wrath of Ice
		return 25;
	case 3037: //Wrath of Fire
		return 25;
	case 3038: //Wrath of Wind
		return 25;
	case 3039: //Protection of the Wild
		return 125;
	case 3040: //Belt of Magi'Kot
		return 18;
	case 3041: //Blade of Walnan
		return 18;
	case 3042: //Fist of Ixiblat
		return 18;
	case 3043: //Blade of The Kedge
		return 18;
	case 3044: //Girdle of Magi'Kot
		return 18;
	case 3045: //Talisman of Return
		return 18;
	case 3047: //Kazad`s Mark
		return 45;
	case 3051: //Fiery Assault
		return 25;
	case 3057: //Tidal Freeze
		return 25;
	case 3063: //Illusion: Froglok
		return 125;
	case 3066: //Requiem of Time
		return 126;
	case 3069: //Seething Hatred
		return 25;
	case 3071: //Insipid Dreams
		return 25;
	case 3100: //Mark of Retaliation
		return 125;
	case 3107: //Cry of Fire
		return 125;
	case 3129: //Call of Sky Strike
		return 25;
	case 3130: //Call of Sky Strike
		return 25;
	case 3131: //Call of Fire Strike
		return 25;
	case 3132: //Call of Fire Strike
		return 25;
	case 3133: //Cry of Fire Strike
		return 25;
	case 3134: //Portal of Knowledge
		return 123;
	case 3135: //Hammer of Divinity
		return 18;
	case 3136: //Hammer of Souls
		return 18;
	case 3156: //Torrent of Brambles
		return 25;
	case 3162: //Wind Strike
		return 25;
	case 3163: //Storm Avalanche
		return 25;
	case 3164: //Froglok Misery
		return 25;
	case 3167: //Strike of Marr
		return 25;
	case 3172: //Denial
		return 25;
	case 3176: //Butchery
		return 25;
	case 3177: //Prayer of Pain
		return 25;
	case 3178: //Vallon's Quickening
		return 125;
	case 3179: //Spirit of Rellic Strike
		return 25;
	case 3180: //Knowledge Portal
		return 123;
	case 3181: //Translocate: Knowledge
		return 123;
	case 3182: //Ring of Knowledge
		return 123;
	case 3183: //Knowledge Gate
		return 123;
	case 3184: //Circle of Knowledge
		return 123;
	case 3185: //Flight of Eagles
		return 125;
	case 3186: //Yaulp VI
		return 95;
	case 3187: //Sermon of Penitence
		return 20;
	case 3188: //Rod of Mystical Transvergance
		return 18;
	case 3189: //Tears of Arlyxir
		return 25;
	case 3190: //Crusader`s Touch
		return 42;
	case 3191: //Shock of Magic
		return 25;
	case 3192: //Earthen Roots
		return 126;
	case 3194: //Greater Fetter
		return 126;
	case 3195: //Greater Immobilize
		return 126;
	case 3196: //Petrifying Earth
		return 126;
	case 3197: //Pacification
		return 126;
	case 3198: //Flameshield of Ro
		return 125;
	case 3199: //Arcane Rune
		return 125;
	case 3205: //Summon Platinum Choker
		return 18;
	case 3206: //Summon Runed Mantle
		return 18;
	case 3207: //Summon Sapphire Bracelet
		return 18;
	case 3208: //Summon Spiked Ring
		return 18;
	case 3209: //Summon Glowing Bauble
		return 18;
	case 3210: //Summon Jewelry Bag
		return 18;
	case 3221: //Shattering Glass
		return 25;
	case 3222: //Web of Glass
		return 25;
	case 3223: //Shards of Glass
		return 25;
	case 3227: //Shroud of Chaos
		return 125;
	case 3229: //Boggle
		return 126;
	case 3232: //Karana's Renewal
		return 42;
	case 3233: //Tnarg`s Mending
		return 42;
	case 3234: //Protection of the Nine
		return 45;
	case 3235: //Focus of Soul
		return 45;
	case 3236: //no spell
		return 25;
	case 3237: //Burnout V
		return 69;
	case 3238: //Destroy Summoned
		return 25;
	case 3239: //Planar Renewal
		return 69;
	case 3240: //Speed of Vallon
		return 125;
	case 3241: //Night`s Dark Terror
		return 125;
	case 3242: //Guard of Druzzil
		return 95;
	case 3243: //Teleport
		return 123;
	case 3244: //Greater Decession
		return 123;
	case 3245: //Force of Akilae
		return 25;
	case 3246: //Shackles of Tunare
		return 126;
	case 3247: //Aura of the Crusader
		return 45;
	case 3255: //Wrath of the Wild
		return 125;
	case 3256: //Wrath of the Wild
		return 125;
	case 3257: //Wrath of the Wild
		return 125;
	case 3258: //Eldritch Rune
		return 125;
	case 3259: //Eldritch Rune
		return 125;
	case 3260: //Eldritch Rune
		return 125;
	case 3264: //Allegiant Familiar
		return 69;
	case 3265: //Servant of Ro
		return 69;
	case 3266: //Servant of Ro
		return 69;
	case 3267: //Servant of Ro
		return 69;
	case 3271: //Guardian of the Forest
		return 125;
	case 3272: //Guardian of the Forest
		return 125;
	case 3273: //Guardian of the Forest
		return 125;
	case 3274: //Virulent Paralysis
		return 126;
	case 3275: //Virulent Paralysis
		return 126;
	case 3276: //Virulent Paralysis
		return 126;
	case 3281: //Servant's Bolt
		return 25;
	case 3282: //Boastful Bellow
		return 25;
	case 3289: //Frenzy of Spirit
		return 125;
	case 3290: //Hobble of Spirits
		return 69;
	case 3291: //Paragon of Spirit
		return 79;
	case 3295: //Legacy of Bracken
		return 125;
	case 3296: //Faith
		return 45;
	case 3297: //Radiant Cure1
		return 42;
	case 3298: //Radiant Cure2
		return 42;
	case 3299: //Radiant Cure3
		return 42;
	case 3300: //Shield of the Arcane
		return 45;
	case 3301: //Force Shield
		return 125;
	case 3302: //Shield of Maelin
		return 45;
	case 3303: //Blood of Thule
		return 20;
	case 3304: //Legacy of Zek
		return 69;
	case 3305: //Rune of Death
		return 69;
	case 3306: //Saryrn's Kiss
		return 114;
	case 3308: //Death's Silence
		return 126;
	case 3309: //Embracing Darkness
		return 20;
	case 3310: //Saryrn's Companion
		return 69;
	case 3311: //Seduction of Saryrn
		return 125;
	case 3312: //Touch of Death
		return 69;
	case 3314: //Child of Bertoxxulous
		return 69;
	case 3315: //Dark Plague
		return 20;
	case 3316: //Word of Terris
		return 126;
	case 3317: //Ward of Xegony
		return 69;
	case 3318: //Firebolt of Tallon
		return 25;
	case 3319: //Sun Storm
		return 25;
	case 3320: //Servant of Marr
		return 69;
	case 3321: //Black Steel
		return 25;
	case 3322: //Child of Ro
		return 69;
	case 3323: //Maelstrom of Thunder
		return 25;
	case 3324: //Rathe's Son
		return 69;
	case 3325: //Sun Vortex
		return 25;
	case 3326: //Resistant Armor
		return 95;
	case 3327: //Tears of Ro
		return 25;
	case 3328: //Lure of Thunder
		return 25;
	case 3329: //Elemental Barrier
		return 95;
	case 3330: //Draught of Ro
		return 25;
	case 3331: //Lure of Ro
		return 25;
	case 3332: //Tears of Marr
		return 25;
	case 3333: //Telekin
		return 25;
	case 3334: //Draught of Thunder
		return 25;
	case 3335: //Agnarr's Thunder
		return 25;
	case 3336: //Draught of E`ci
		return 25;
	case 3337: //Iceflame of E`ci
		return 125;
	case 3338: //Harvest of Druzzil
		return 42;
	case 3339: //Strike of Solusek
		return 25;
	case 3341: //Apathy
		return 126;
	case 3342: //Howl of Tashan
		return 126;
	case 3343: //Rune of Zebuxoruk
		return 125;
	case 3344: //Imbue Nightmare
		return 18;
	case 3345: //Strangle
		return 20;
	case 3346: //Imbue Storm
		return 18;
	case 3347: //Beckon
		return 126;
	case 3348: //Torment of Scio
		return 20;
	case 3349: //Insanity
		return 25;
	case 3350: //Tranquility
		return 79;
	case 3351: //Uproar
		return 125;
	case 3352: //Imbue Earth
		return 18;
	case 3353: //Imbue Air
		return 18;
	case 3354: //Sleep
		return 126;
	case 3355: //Command of Druzzil
		return 126;
	case 3356: //Imbue Fire
		return 18;
	case 3357: //Imbue Water
		return 18;
	case 3358: //Bliss
		return 126;
	case 3359: //Word of Morell
		return 126;
	case 3360: //Voice of Quellious
		return 79;
	case 3361: //Silent Song of Quellious
		return 126;
	case 3362: //Rizlona's Call of Flame
		return 125;
	case 3363: //Tuyen`s Chant of the Plague
		return 20;
	case 3364: //Druzzil's Disillusionment
		return 126;
	case 3365: //Melody of Mischief
		return 126;
	case 3366: //Saryrn's Scream of Pain
		return 25;
	case 3367: //Tuyen`s Chant of Fire
		return 20;
	case 3368: //Psalm of Veeshan
		return 125;
	case 3369: //Dreams of Terris
		return 126;
	case 3370: //Tuyen`s Chant of Venom
		return 20;
	case 3371: //Call of the Banshee
		return 126;
	case 3372: //Chorus of Marr
		return 79;
	case 3373: //Tuyen`s Chant of Ice
		return 20;
	case 3374: //Warsong of Zek
		return 125;
	case 3375: //Harmony of Sound
		return 126;
	case 3376: //Lullaby of Morell
		return 126;
	case 3377: //True Spirit
		return 69;
	case 3378: //Agility of the Wrulan
		return 95;
	case 3379: //Spear of Torment
		return 25;
	case 3380: //Cloud of Grummus
		return 126;
	case 3381: //Ancestral Guard
		return 95;
	case 3382: //Endurance of the Boar
		return 95;
	case 3383: //Talisman of the Wrulan
		return 95;
	case 3384: //Talisman of the Tribunal
		return 95;
	case 3385: //Tears of Saryrn
		return 25;
	case 3386: //Malicious Decay
		return 126;
	case 3387: //Malosinia
		return 126;
	case 3388: //Strength of the Diaku
		return 95;
	case 3389: //Talisman of the Boar
		return 95;
	case 3390: //Velium Strike
		return 25;
	case 3391: //Talisman of Alacrity
		return 125;
	case 3392: //Talisman of the Diaku
		return 95;
	case 3393: //Tiny Terror
		return 125;
	case 3394: //Breath of Ultor
		return 20;
	case 3395: //Malos
		return 126;
	case 3396: //Blood of Saryrn
		return 20;
	case 3397: //Focus of the Seventh
		return 45;
	case 3398: //Quiescence
		return 42;
	case 3399: //Ferine Avatar
		return 95;
	case 3400: //Festering Darkness
		return 20;
	case 3401: //Touch of Volatis
		return 114;
	case 3403: //Aura of Pain
		return 114;
	case 3405: //Terror of Thule
		return 126;
	case 3406: //Aura of Darkness
		return 114;
	case 3408: //Zevfeer's Bite
		return 114;
	case 3410: //Voice of Thule
		return 125;
	case 3411: //Aura of Hate
		return 114;
	case 3413: //Touch of Innoruuk
		return 114;
	case 3415: //Nature's Rebuke
		return 125;
	case 3416: //Nature's Rebuke Strike
		return 25;
	case 3417: //Spirit of the Predator
		return 95;
	case 3418: //Frozen Wind
		return 25;
	case 3419: //Call of the Rathe
		return 125;
	case 3420: //Cry of Thunder
		return 125;
	case 3421: //Cry of Thunder Strike
		return 25;
	case 3422: //Ward of Nife
		return 125;
	case 3423: //Ward of Nife Strike
		return 25;
	case 3424: //Pious Might
		return 125;
	case 3425: //Pious Might Strike
		return 25;
	case 3426: //Quellious' Word of Serenity
		return 25;
	case 3427: //Wave of Marr
		return 42;
	case 3428: //Deny Undead
		return 25;
	case 3429: //Touch of Nife
		return 42;
	case 3430: //Light of Nife
		return 42;
	case 3431: //Brushfire
		return 25;
	case 3432: //Brell's Stalwart Shield
		return 45;
	case 3433: //Replenishment
		return 79;
	case 3434: //Storm's Fury
		return 25;
	case 3435: //Hand of Ro
		return 126;
	case 3436: //Winter's Storm
		return 25;
	case 3437: //Immolation of Ro
		return 20;
	case 3438: //Karana's Rage
		return 25;
	case 3439: //Nature's Might
		return 95;
	case 3440: //Ro's Illumination
		return 126;
	case 3441: //Blessing of Replenishment
		return 79;
	case 3442: //E'ci's Frosty Breath
		return 126;
	case 3443: //Nature's Infusion
		return 42;
	case 3444: //Protection of Seasons
		return 95;
	case 3445: //Command of Tunare
		return 126;
	case 3446: //Swarming Death
		return 20;
	case 3447: //Savage Roots
		return 126;
	case 3448: //Shield of Bracken
		return 125;
	case 3449: //Summer's Flame
		return 25;
	case 3450: //Brackencoat
		return 125;
	case 3451: //Blessing of the Nine
		return 45;
	case 3452: //Winter's Frost
		return 25;
	case 3453: //Mask of the Forest
		return 79;
	case 3454: //Infusion of Spirit
		return 45;
	case 3455: //Healing of Sorsha
		return 69;
	case 3456: //Spiritual Vigor
		return 45;
	case 3457: //Spirit of Arag
		return 69;
	case 3458: //Arag`s Celerity
		return 69;
	case 3459: //Spirit of Rellic
		return 69;
	case 3460: //Spiritual Dominion
		return 79;
	case 3461: //Spirit of Sorsha
		return 69;
	case 3462: //Sha's Revenge
		return 126;
	case 3463: //Ferocity
		return 95;
	case 3464: //The Silent Command
		return 25;
	case 3465: //Supernal Remedy
		return 42;
	case 3466: //Symbol of Kazad
		return 45;
	case 3467: //Virtue
		return 45;
	case 3468: //Destroy Undead
		return 25;
	case 3469: //Mark of Kings
		return 125;
	case 3470: //Ward of Gallantry
		return 95;
	case 3471: //Word of Replenishment
		return 42;
	case 3472: //Blessing of Reverence
		return 125;
	case 3473: //Catastrophe
		return 25;
	case 3474: //Armor of the Zealot
		return 45;
	case 3475: //Supernal Elixir
		return 42;
	case 3476: //Condemnation
		return 25;
	case 3477: //Mark of the Righteous
		return 125;
	case 3478: //Hammer of Damnation
		return 18;
	case 3479: //Hand of Virtue
		return 45;
	case 3480: //Supernal Light
		return 42;
	case 3481: //Tarnation
		return 25;
	case 3482: //Sound of Might
		return 25;
	case 3483: //Elemental Silence
		return 126;
	case 3484: //Call of the Arch Mage
		return 126;
	case 3485: //Supernal Cleansing
		return 42;
	case 3486: //Maelstrom of Ro
		return 125;
	case 3487: //Strength of Tunare
		return 45;
	case 3488: //Pact of Hate
		return 125;
	case 3489: //Blood of Hate
		return 20;
	case 3490: //Cloak of Luclin
		return 45;
	case 3491: //Spear of Decay
		return 25;
	case 3492: //Scorpion Venom
		return 20;
	case 3493: //Frost Spear
		return 25;
	case 3494: //Luggald Blood
		return 18;
	case 3498: //Gallenite's Lifetap Test
		return 25;
	case 3560: //Spear of Pain
		return 25;
	case 3561: //Spear of Disease
		return 25;
	case 3562: //Spear of Plague
		return 25;
	case 3564: //Burning Arrow
		return 25;
	case 3565: //Flaming Arrow
		return 25;
	case 3566: //Tuyen`s Chant of Poison
		return 20;
	case 3567: //Tuyen`s Chant of Disease
		return 20;
	case 3568: //Ice Spear
		return 25;
	case 3569: //Frost Shard
		return 25;
	case 3570: //Ice Shard
		return 25;
	case 3571: //Torbas' Poison Blast
		return 25;
	case 3572: //Torbas' Venom Blast
		return 25;
	case 3573: //Shock of Venom
		return 25;
	case 3574: //Blast of Venom
		return 25;
	case 3575: //Blessing of Piety
		return 125;
	case 3576: //Blessing of Faith
		return 125;
	case 3577: //Wave of Life
		return 42;
	case 3578: //Brell's Steadfast Aegis
		return 45;
	case 3579: //Share Form of the Great Wolf
		return 125;
	case 3580: //Spirit of Ash
		return 125;
	case 3581: //O`Keils Levity
		return 125;
	case 3582: //Elemental Cloak
		return 95;
	case 3583: //Tiny Companion
		return 69;
	case 3584: //Refresh Summoning
		return 69;
	case 3585: //Entrancing Lights
		return 126;
	case 3586: //Illusion: Scaled Wolf
		return 125;
	case 3587: //Planar Strike
		return 25;
	case 3589: //Ethereal Strike
		return 25;
	case 3591: //Imbue Disease
		return 18;
	case 3592: //Imbue Valor
		return 18;
	case 3593: //Imbue War
		return 18;
	case 3594: //Imbue Torment
		return 18;
	case 3595: //Imbue Justice
		return 18;
	case 3601: //Harmony of Nature
		return 126;
	case 3618: //Eclipse Aura Strike
		return 25;
	case 3619: //Eclipse Aura Strike
		return 25;
	case 3621: //Frost Claw
		return 25;
	case 3623: //Burning Barb
		return 25;
	case 3624: //Anger
		return 25;
	case 3626: //Tendrils of Fire
		return 25;
	case 3630: //Time Lapse
		return 25;
	case 3645: //Sting of Ayonae
		return 25;
	case 3646: //Bite of Bertoxxulous
		return 25;
	case 3648: //Time Snap
		return 25;
	case 3650: //Dark Empathy Recourse
		return 25;
	case 3651: //Wind of Marr
		return 79;
	case 3665: //Curtain Call
		return 25;
	case 3668: //Pawn's Plight
		return 20;
	case 3670: //Queen's Checkmate
		return 25;
	case 3681: //Aria of Innocence
		return 42;
	case 3682: //Aria of Asceticism
		return 42;
	case 3683: //Ethereal Cleansing
		return 42;
	case 3684: //Light of Life
		return 42;
	case 3685: //Comatose
		return 125;
	case 3686: //Blood of Pain
		return 20;
	case 3687: //Swarm of Pain
		return 20;
	case 3688: //Icewind
		return 25;
	case 3689: //Malaria
		return 20;
	case 3690: //Bond of the Wild
		return 69;
	case 3691: //Bond of the Wild R.
		return 45;
	case 3692: //Temperance
		return 45;
	case 3693: //Pure Blood
		return 42;
	case 3694: //Stoicism
		return 42;
	case 3695: //Frost Zephyr
		return 126;
	case 3696: //Leviathan Eyes
		return 125;
	case 3697: //Scryer's Trespass
		return 126;
	case 3699: //Primal Remedy
		return 69;
	case 3700: //Elemental Empathy
		return 69;
	case 3702: //Auspice
		return 114;
	case 3704: //Soul Empathy
		return 69;
	case 3706: //Frozen Harpoon
		return 25;
	case 3748: //Insipid Decay
		return 25;
	case 3753: //Torrent of Agony
		return 114;
	case 3764: //Rain of Bile
		return 25;
	case 3785: //Hate's Fury
		return 25;
	case 3792: //Circle of Stonebrunt
		return 123;
	case 3793: //Stonebrunt Portal
		return 123;
	case 3794: //Ring of Stonebrunt
		return 123;
	case 3795: //Stonebrunt Gate
		return 123;
	case 3796: //Mind Tap
		return 126;
	case 3799: //Dismal Wind
		return 25;
	case 3803: //Pique
		return 25;
	case 3806: //Distraction
		return 126;
	case 3809: //Reclamation
		return 79;
	case 3810: //Eruption
		return 25;
	case 3811: //Vision Shift
		return 125;
	case 3833: //Translocate: Stonebrunt
		return 123;
	case 3834: //Healing Water
		return 42;
	case 3842: //Blood of Nadox
		return 42;
	case 3847: //Cloak of Khala Dun
		return 125;
	case 3848: //Tortured Memory II
		return 25;
	case 3849: //Alter Plane: Hate II
		return 123;
	case 3854: //Form of Protection
		return 125;
	case 3855: //Form of Defense
		return 125;
	case 3856: //Form of Endurance
		return 125;
	case 3857: //Form of Rejuvenation
		return 125;
	case 3861: //Pestilence Shock
		return 125;
	case 3864: //Soul Claw
		return 125;
	case 3876: //Frozen Shards
		return 25;
	case 3877: //Nightmares
		return 20;
	case 3878: //Time Rend
		return 25;
	case 3881: //Hand of Retribution
		return 25;
	case 3909: //Clinging Clay
		return 126;
	case 3910: //Flames of Condemnation
		return 25;
	case 3921: //Guide Evacuation
		return 123;
	case 3975: //Force of Akera
		return 25;
	case 3976: //Draught of Lightning
		return 25;
	case 3981: //Mass Clarify Mana
		return 18;
	case 3982: //Mass Crystallize Mana
		return 18;
	case 3983: //Mass Distill Mana
		return 18;
	case 3984: //Mass Enchant Adamantite
		return 18;
	case 3985: //Mass Enchant Brellium
		return 18;
	case 3986: //Mass Enchant Clay
		return 18;
	case 3987: //Mass Enchant Electrum
		return 18;
	case 3988: //Mass Enchant Gold
		return 18;
	case 3989: //Mass Enchant Mithril
		return 18;
	case 3990: //Mass Enchant Platinum
		return 18;
	case 3991: //Mass Enchant Silver
		return 18;
	case 3992: //Mass Enchant Steel
		return 18;
	case 3993: //Mass Enchant Velium
		return 18;
	case 3994: //Mass Imbue Amber
		return 18;
	case 3995: //Mass Imbue Black Pearl
		return 18;
	case 3996: //Mass Imbue Black Sapphire
		return 18;
	case 3997: //Mass Imbue Diamond
		return 18;
	case 3998: //Mass Imbue Emerald
		return 18;
	case 3999: //Mass Imbue Fire Opal
		return 18;
	case 4000: //Mass Imbue Ivory
		return 18;
	case 4001: //Mass Imbue Jade
		return 18;
	case 4002: //Mass Imbue Opal
		return 18;
	case 4003: //Mass Imbue Peridot
		return 18;
	case 4004: //Mass Imbue Plains Pebble
		return 18;
	case 4005: //Mass Imbue Rose Quartz
		return 18;
	case 4006: //Mass Imbue Ruby
		return 18;
	case 4007: //Mass Imbue Sapphire
		return 18;
	case 4008: //Mass Imbue Topaz
		return 18;
	case 4009: //Mass Purify Mana
		return 18;
	case 4010: //Mass Thicken Mana
		return 18;
	case 4011: //Kindle
		return 69;
	case 4017: //Illusion: Guktan
		return 125;
	case 4018: //RytanGuard1
		return 69;
	case 4019: //RytanGuard2
		return 69;
	case 4020: //RytanGuard3
		return 69;
	case 4021: //RytanGuard4
		return 69;
	case 4022: //RytanBoltTest
		return 25;
	case 4027: //Summon Wooden Bracelet
		return 18;
	case 4028: //Summon Stone Bracelet
		return 18;
	case 4029: //Summon Iron Bracelet
		return 18;
	case 4030: //Summon Steel Bracelet
		return 18;
	case 4049: //Circle of Cooling
		return 95;
	case 4050: //Circle of Warmth
		return 95;
	case 4051: //Talisman of Purity
		return 95;
	case 4052: //Talisman of Vitality
		return 95;
	case 4053: //Blessing of Temperance
		return 45;
	case 4054: //Spirit of the Shrew
		return 125;
	case 4055: //Pack Shrew
		return 125;
	case 4056: //Remove Minor Curse
		return 42;
	case 4057: //Remove Lesser Curse
		return 42;
	case 4058: //Feral Pack
		return 125;
	case 4059: //Call of Ice
		return 125;
	case 4062: //Dark Temptation
		return 125;
	case 4063: //Call of Darkness
		return 125;
	case 4064: //Austerity
		return 45;
	case 4065: //Blessing of Austerity
		return 45;
	case 4066: //Ice Meteor
		return 25;
	case 4067: //Ward of Calrena
		return 125;
	case 4068: //Guard of Calrena
		return 125;
	case 4069: //Protection of Calrena
		return 125;
	case 4070: //Magi Ward
		return 125;
	case 4071: //Mana Ward
		return 125;
	case 4072: //Deception
		return 125;
	case 4073: //Ward of Alendar
		return 125;
	case 4074: //Guard of Alendar
		return 125;
	case 4075: //Protection of Alendar
		return 125;
	case 4076: //Bulwark of Alendar
		return 125;
	case 4077: //Ordinance
		return 126;
	case 4078: //Wind of the Desert
		return 25;
	case 4079: //Ward of Calliav
		return 69;
	case 4080: //Guard of Calliav
		return 69;
	case 4081: //Protection of Calliav
		return 69;
	case 4082: //Summon: Orb of Exploration
		return 18;
	case 4083: //Rizlona's Embers
		return 125;
	case 4084: //Rizlona's Fire
		return 125;
	case 4085: //Forpar's Aria of Affliction
		return 125;
	case 4086: //Forpar's Psalm of Pain
		return 125;
	case 4087: //Forpar's Verse of Venom
		return 125;
	case 4088: //Ward of Vie
		return 125;
	case 4089: //Guard of Vie
		return 125;
	case 4090: //Protection of Vie
		return 125;
	case 4091: //Bulwark of Vie
		return 125;
	case 4092: //Curse
		return 20;
	case 4093: //Odium
		return 20;
	case 4094: //Anathema
		return 20;
	case 4095: //Bane
		return 20;
	case 4096: //Dark Soul
		return 20;
	case 4097: //Imprecation
		return 20;
	case 4098: //Horror
		return 20;
	case 4099: //Bounce
		return 125;
	case 4100: //Reflect
		return 125;
	case 4101: //Scythe of Innoruuk
		return 25;
	case 4102: //Scythe of Darkness
		return 25;
	case 4103: //Scythe of Death
		return 25;
	case 4104: //Vengeance of the Wild
		return 20;
	case 4105: //Vengeance of Nature
		return 20;
	case 4106: //Vengeance of Tunare
		return 20;
	case 4107: //Feral Form
		return 125;
	case 4108: //Aura of Reverence
		return 125;
	case 4109: //Guidance
		return 45;
	case 4110: //Burning Sand
		return 25;
	case 4111: //Fire Swarm
		return 20;
	case 4112: //Call of the Muse
		return 125;
	case 4210: //Fufil`s Diminishing Dirge
		return 20;
	case 4239: //Breathless Mist
		return 125;
	case 4240: //Essence of Concealment
		return 125;
	case 4241: //Weightless Mist
		return 125;
	case 4242: //Mist of the Wolf
		return 125;
	case 4252: //Xalirilan's Lesser Appraisal
		return 66;
	case 4253: //Xalirilan's Appraisal
		return 66;
	case 4254: //Xalirilan's Greater Appraisal
		return 66;
	case 4255: //Wuggan's Lesser Appraisal
		return 66;
	case 4256: //Wuggan's Appraisal
		return 66;
	case 4257: //Wuggan's Greater Appraisal
		return 66;
	case 4258: //Iony's Lesser Augury
		return 66;
	case 4259: //Iony's Augury
		return 66;
	case 4260: //Iony's Greater Augury
		return 66;
	case 4261: //Reebo's Lesser Augury
		return 66;
	case 4262: //Reebo's Augury
		return 66;
	case 4263: //Reebo's Greater Augury
		return 66;
	case 4264: //Xalirilan's Lesser Discombobulation
		return 66;
	case 4265: //Xalirilan's Discombobulation
		return 66;
	case 4266: //Xalirilan's Greater Discombobulation
		return 66;
	case 4267: //Wuggan's Lesser Discombobulation
		return 66;
	case 4268: //Wuggan's Discombobulation
		return 66;
	case 4269: //Wuggan's Greater Discombobulation
		return 66;
	case 4270: //Iony's Lesser Exorcism
		return 66;
	case 4271: //Iony's Exorcism
		return 66;
	case 4272: //Iony's Greater Exorcism
		return 66;
	case 4273: //Reebo's Lesser Exorcism
		return 66;
	case 4274: //Reebo's Exorcism
		return 66;
	case 4275: //Reebo's Greater Exorcism
		return 66;
	case 4276: //Xalirilan's Lesser Extrication
		return 66;
	case 4277: //Xalirilan's Extrication
		return 66;
	case 4278: //Xalirilan's Greater Extrication
		return 66;
	case 4279: //Wuggan's Lesser Extrication
		return 66;
	case 4280: //Wuggan's Extrication
		return 66;
	case 4281: //Wuggan's Greater Extrication
		return 66;
	case 4282: //Iony's Lesser Cleansing
		return 66;
	case 4283: //Iony's Cleansing
		return 66;
	case 4284: //Iony's Greater Cleansing
		return 66;
	case 4285: //Reebo's Lesser Cleansing
		return 66;
	case 4286: //Reebo's Cleansing
		return 66;
	case 4287: //Reebo's Greater Cleansing
		return 66;
	case 4291: //Aura of Quellious
		return 79;
	case 4350: //Transmute Hunter's Dagger
		return 18;
	case 4351: //Transmute Hunter's Barbs
		return 18;
	case 4352: //Transmute Wayfarer's Bread
		return 18;
	case 4353: //Transmute Wayfarer's Tonic
		return 18;
	case 4354: //Transmute Traveler's Bandage
		return 18;
	case 4355: //Transmute Wayfarer's Wine
		return 18;
	case 4356: //Bite of Ykesha
		return 25;
	case 4357: //Strike of Ykesha
		return 25;
	case 4358: //Force of Ykesha
		return 25;
	case 4359: //Wrath of Ykesha
		return 25;
	case 4360: //Rujarkian Breath
		return 25;
	case 4361: //Rujarkian Mist
		return 25;
	case 4362: //Rujarkian Poison
		return 25;
	case 4363: //Rujarkian Bile
		return 25;
	case 4364: //Rujarkian Venom
		return 25;
	case 4365: //Heated Blade
		return 25;
	case 4366: //Burning Blade
		return 25;
	case 4367: //Blazing Blade
		return 25;
	case 4368: //Flaming Blade
		return 25;
	case 4369: //Inferno Blade
		return 25;
	case 4370: //Vampire Touch
		return 25;
	case 4371: //Vampire Claw
		return 25;
	case 4372: //Vampire Talon
		return 25;
	case 4373: //Vampire Fangs
		return 25;
	case 4374: //Vampire Kiss
		return 25;
	case 4375: //Chill
		return 25;
	case 4376: //Icicle Strike
		return 25;
	case 4377: //Icicle Claw
		return 25;
	case 4378: //Vox' Bite
		return 25;
	case 4379: //Permafrost
		return 25;
	case 4380: //Mirror I
		return 125;
	case 4381: //Mirror II
		return 125;
	case 4382: //Mirror III
		return 125;
	case 4383: //Mirror IV
		return 125;
	case 4384: //Guard I
		return 125;
	case 4385: //Guard II
		return 125;
	case 4386: //Guard III
		return 125;
	case 4387: //Guard IV
		return 125;
	case 4388: //Spell Guard I
		return 125;
	case 4389: //Spell Guard II
		return 125;
	case 4390: //Spell Guard III
		return 125;
	case 4391: //Spell Guard IV
		return 125;
	case 4395: //Selo's Rhythm of Speed
		return 125;
	case 4408: //Color Cloud
		return 25;
	case 4413: //Golem Pulverize
		return 25;
	case 4414: //Rimebone Frost Burst
		return 25;
	case 4418: //Illusion: Frost Bone
		return 125;
	case 4489: //Taelosian Vengeance
		return 126;
	case 4492: //Geostrike
		return 25;
	case 4493: //Earth Wave
		return 25;
	case 4496: //Rock Storm
		return 25;
	case 4497: //Earth Shards
		return 25;
	case 4498: //Aggressive Discipline
		return 27;
	case 4499: //Defensive Discipline
		return 27;
	case 4500: //Holyforge Discipline
		return 27;
	case 4501: //Precision Discipline
		return 27;
	case 4502: //Voiddance Discipline
		return 27;
	case 4503: //Evasive Discipline
		return 27;
	case 4504: //Leechcurse Discipline
		return 27;
	case 4505: //Deadeye Discipline
		return 27;
	case 4506: //Trueshot Discipline
		return 27;
	case 4507: //Silentfist Discipline
		return 27;
	case 4508: //Ashenhand Discipline
		return 27;
	case 4509: //Whirlwind Discipline
		return 27;
	case 4510: //Stonestance Discipline
		return 27;
	case 4511: //Thunderkick Discipline
		return 27;
	case 4512: //Innerflame Discipline
		return 27;
	case 4513: //Hundred Fists Discipline
		return 27;
	case 4514: //Mighty Strike Discipline
		return 27;
	case 4515: //Nimble Discipline
		return 27;
	case 4516: //Deftdance Discipline
		return 27;
	case 4517: //Kinesthetics Discipline
		return 27;
	case 4518: //Sanctification Discipline
		return 27;
	case 4519: //Weapon Shield Discipline
		return 27;
	case 4520: //Unholy Aura Discipline
		return 27;
	case 4521: //Bestial Alignment I
		return 95;
	case 4522: //Bestial Alignment II
		return 95;
	case 4523: //Bestial Alignment III
		return 95;
	case 4524: //Bestial Alignment I
		return 95;
	case 4525: //Bestial Alignment II
		return 95;
	case 4526: //Bestial Alignment III
		return 95;
	case 4527: //Bestial Alignment I
		return 95;
	case 4528: //Bestial Alignment II
		return 95;
	case 4529: //Bestial Alignment III
		return 95;
	case 4530: //Bestial Alignment I
		return 95;
	case 4531: //Bestial Alignment II
		return 95;
	case 4532: //Bestial Alignment III
		return 95;
	case 4533: //Bestial Alignment I
		return 95;
	case 4534: //Bestial Alignment II
		return 95;
	case 4535: //Bestial Alignment III
		return 95;
	case 4536: //Frenzied Aura
		return 125;
	case 4537: //Icy Grasp
		return 69;
	case 4538: //Icy Grasp Strike
		return 25;
	case 4549: //Divine Avatar I
		return 95;
	case 4550: //Divine Avatar II
		return 95;
	case 4551: //Divine Avatar III
		return 95;
	case 4555: //Elemental Domination
		return 126;
	case 4567: //Aneuk Grasp
		return 20;
	case 4574: //Hynid Snap
		return 20;
	case 4578: //Turepta Crush
		return 25;
	case 4579: //Ukun Chains
		return 20;
	case 4585: //Resistant Discipline
		return 27;
	case 4586: //Puretone Discipline
		return 27;
	case 4587: //Fearless Discipline
		return 27;
	case 4588: //Infuriate
		return 115;
	case 4589: //Barrier
		return 115;
	case 4590: //Cover
		return 115;
	case 4591: //Guard
		return 115;
	case 4592: //Infallible
		return 115;
	case 4593: //Crippling Strike
		return 115;
	case 4595: //Muscle Shock
		return 115;
	case 4596: //Armor Slice
		return 115;
	case 4597: //Gauntlet Strike
		return 115;
	case 4598: //Head Bash
		return 115;
	case 4599: //Rally Cry
		return 115;
	case 4600: //Shin Kick
		return 115;
	case 4601: //Rage
		return 115;
	case 4602: //Power Slam
		return 115;
	case 4603: //Stomp
		return 115;
	case 4604: //Back Swing
		return 115;
	case 4605: //Slice
		return 115;
	case 4607: //Flurry
		return 115;
	case 4608: //Provoke
		return 15;
	case 4612: //Enrage
		return 113;
	case 4614: //Phantom Zephyr
		return 15;
	case 4615: //Fortitude
		return 115;
	case 4616: //Pain Tolerance
		return 115;
	case 4618: //Fortune
		return 115;
	case 4619: //Quick Feet
		return 115;
	case 4620: //Ton Po's Defense
		return 115;
	case 4621: //Focused Aura
		return 115;
	case 4622: //Overwhelm
		return 115;
	case 4623: //Tranquil Force
		return 115;
	case 4624: //Grapple
		return 115;
	case 4625: //Armor Crush
		return 115;
	case 4626: //Leg Sweep
		return 115;
	case 4627: //Nerve Strike
		return 115;
	case 4628: //Nerve Spasm
		return 115;
	case 4629: //Thunderkick
		return 115;
	case 4630: //Master's Fury
		return 115;
	case 4631: //Ashenhand
		return 115;
	case 4632: //Aura of Speed
		return 115;
	case 4633: //Whirlwind Kick
		return 115;
	case 4634: //Dragon Strike
		return 115;
	case 4635: //Tranquil Focus
		return 115;
	case 4636: //Rapid Jab
		return 115;
	case 4637: //Wind of Force
		return 115;
	case 4638: //Pain Strike
		return 115;
	case 4639: //Indirection
		return 115;
	case 4640: //Focus
		return 115;
	case 4641: //Reflexes
		return 115;
	case 4642: //Mental Block
		return 115;
	case 4643: //Proficiency
		return 115;
	case 4644: //Bind
		return 115;
	case 4645: //Armor Pierce
		return 115;
	case 4646: //Eye Gouge
		return 115;
	case 4647: //Tendon Slice
		return 115;
	case 4648: //Wrist Slice
		return 115;
	case 4649: //Assassin's Focus
		return 115;
	case 4650: //Lunge
		return 115;
	case 4651: //Direct Assault
		return 115;
	case 4652: //Vital Cut
		return 115;
	case 4653: //Blood Feast
		return 115;
	case 4654: //Blood Slice
		return 115;
	case 4655: //Energy Sap
		return 115;
	case 4656: //Mind Snap
		return 115;
	case 4657: //Burning Spasm
		return 115;
	case 4658: //Double Stab
		return 115;
	case 4659: //Sneak Attack
		return 15;
	case 4670: //Fortitude Discipline
		return 27;
	case 4671: //Protective Spirit Discipline
		return 27;
	case 4672: //Charge Discipline
		return 27;
	case 4673: //Counterattack Discipline
		return 27;
	case 4674: //Furious Discipline
		return 27;
	case 4675: //Fellstrike Discipline
		return 27;
	case 4676: //Duelist Discipline
		return 27;
	case 4677: //Blinding Speed Discipline
		return 27;
	case 4678: //Bestial Fury Discipline
		return 27;
	case 4679: //Energy Sap Recourse
		return 115;
	case 4680: //Cover Recourse
		return 115;
	case 4681: //Bellow
		return 15;
	case 4682: //Berate
		return 15;
	case 4683: //Phantom Wind
		return 15;
	case 4684: //Phantom Echo
		return 15;
	case 4685: //Thief's Vengeance
		return 15;
	case 4686: //Assassin's Strike
		return 15;
	case 4687: //Healing Will Discipline
		return 27;
	case 4688: //Stonewall Discipline
		return 27;
	case 4689: //Spirit of Rage Discipline
		return 27;
	case 4690: //Earthwalk Discipline
		return 27;
	case 4691: //Speed Focus Discipline
		return 27;
	case 4692: //Planeswalk Discipline
		return 27;
	case 4693: //Concentration Discipline
		return 27;
	case 4694: //Deadly Precision Discipline
		return 27;
	case 4695: //Twisted Chance Discipline
		return 27;
	case 4696: //Weapon Affinity Discipline
		return 27;
	case 4697: //Incite
		return 15;
	case 4698: //Phantom Call
		return 15;
	case 4699: //Bite of the Hounds
		return 25;
	case 4700: //Claw of the Beast
		return 25;
	case 4701: //Warhound's Affliction
		return 25;
	case 4704: //Blood Scream
		return 42;
	case 4706: //Mindless Rage
		return 115;
	case 4716: //Call of Rav
		return 126;
	case 4717: //Diseased Maw
		return 126;
	case 4721: //Focused Will Discipline
		return 27;
	case 4724: //Abysmal Replenishment
		return 79;
	case 4726: //Bite of Keras
		return 25;
	case 4741: //Reflection of Discord I
		return 125;
	case 4742: //Reflection of Discord II
		return 125;
	case 4743: //Reflection of Discord III
		return 125;
	case 4786: //Icy Grasp
		return 69;
	case 4788: //Feral Swipe
		return 115;
	case 4789: //Touch of the Divine
		return 125;
	case 4795: //Aura of Restoration
		return 79;
	case 4801: //Doppelganger Recourse
		return 125;
	case 4802: //Flames of Kesh`yk I
		return 69;
	case 4803: //Flames of Kesh`yk II
		return 69;
	case 4804: //Flames of Kesh`yk III
		return 69;
	case 4805: //Frost of Kesh`yk I
		return 69;
	case 4806: //Frost of Kesh`yk II
		return 69;
	case 4807: //Frost of Kesh`yk III
		return 69;
	case 4808: //Lightning of Kesh`yk I
		return 69;
	case 4809: //Lightning of Kesh`yk II
		return 69;
	case 4810: //Lightning of Kesh`yk III
		return 69;
	case 4811: //Flames of Kesh`yk Effect I
		return 69;
	case 4812: //Flames of Kesh`yk Effect II
		return 69;
	case 4813: //Flames of Kesh`yk Effect III
		return 69;
	case 4814: //Frost of Kesh`yk Effect I
		return 69;
	case 4815: //Frost of Kesh`yk Effect II
		return 69;
	case 4816: //Frost of Kesh`yk Effect III
		return 69;
	case 4817: //Lightning of Kesh`yk Effect I
		return 69;
	case 4818: //Lightning of Kesh`yk Effect II
		return 69;
	case 4819: //Lightning of Kesh`yk Effect III
		return 69;
	case 4820: //Rabid Companion I
		return 69;
	case 4821: //Rabid Companion II
		return 69;
	case 4822: //Rabid Companion III
		return 69;
	case 4841: //Aura of Fire
		return 79;
	case 4842: //Exultant Bellow I
		return 25;
	case 4843: //Exultant Bellow II
		return 25;
	case 4844: //Exultant Bellow III
		return 25;
	case 4845: //Exultant Bellow IV
		return 25;
	case 4846: //Exultant Bellow V
		return 25;
	case 4849: //Heartstopper
		return 126;
	case 4853: //Listless Strength
		return 126;
	case 4871: //War March of the Mastruq
		return 125;
	case 4872: //Echo of the Trusik
		return 125;
	case 4873: //Dark Echo
		return 125;
	case 4874: //Turepta Blood
		return 20;
	case 4875: //Trushar's Mending
		return 42;
	case 4876: //Trushar's Frost
		return 25;
	case 4877: //Apathy of the Nihil
		return 126;
	case 4878: //Bliss of the Nihil
		return 126;
	case 4879: //Madness of Ikkibi
		return 25;
	case 4880: //Holy Light
		return 42;
	case 4881: //Order
		return 25;
	case 4882: //Holy Elixir
		return 42;
	case 4883: //Sylvan Infusion
		return 42;
	case 4884: //Sylvan Fire
		return 25;
	case 4885: //Sylvan Embers
		return 20;
	case 4886: //Elemental Siphon
		return 69;
	case 4887: //Rock of Taelosia
		return 25;
	case 4888: //Monster Summoning IV
		return 69;
	case 4889: //Night Stalker
		return 114;
	case 4890: //Night Fire
		return 20;
	case 4891: //Night's Beckon
		return 114;
	case 4893: //Wave of Trushar
		return 42;
	case 4894: //Light of Order
		return 42;
	case 4895: //Holy Order
		return 125;
	case 4896: //Sylvan Light
		return 42;
	case 4897: //Sylvan Burn
		return 25;
	case 4898: //Sylvan Call
		return 125;
	case 4899: //Breath of Trushar
		return 42;
	case 4900: //Balance of the Nihil
		return 126;
	case 4901: //Daluda's Mending
		return 42;
	case 4902: //Mental Horror
		return 125;
	case 4903: //Black Shroud
		return 125;
	case 4904: //Miasmic spear
		return 25;
	case 4905: //Black Ice
		return 25;
	case 4906: //White Fire
		return 25;
	case 4907: //Telaka
		return 25;
	case 4908: //Mental Horror Strike
		return 25;
	case 4912: //Sylvan Call Strike
		return 25;
	case 4913: //Holy Order Strike
		return 25;
	case 4928: //Leg Strike
		return 126;
	case 4929: //Leg Cut
		return 126;
	case 4930: //Leg Slice
		return 126;
	case 4931: //Head Strike
		return 25;
	case 4932: //Head Pummel
		return 25;
	case 4933: //Head Crush
		return 25;
	case 4934: //Diversive Strike
		return 126;
	case 4935: //Distracting Strike
		return 126;
	case 4936: //Confusing Strike
		return 126;
	case 4937: //Corroded Axe
		return 18;
	case 4938: //Blunt Axe
		return 18;
	case 4939: //Steel Axe
		return 18;
	case 4940: //Bearded Axe
		return 18;
	case 4941: //Mithril Axe
		return 18;
	case 4942: //Balanced War Axe
		return 18;
	case 4943: //Bonesplicer Axe
		return 18;
	case 4944: //Fleshtear Axe
		return 18;
	case 4945: //Cold Steel Cleaving Axe
		return 18;
	case 4946: //Mithril Bloodaxe
		return 18;
	case 4947: //Rage Axe
		return 18;
	case 4948: //Bloodseekers Axe
		return 18;
	case 4949: //Battlerage Axe
		return 18;
	case 4950: //Deathfury Axe
		return 18;
	case 4957: //Shock of Discord
		return 125;
	case 4960: //Simmering Rage
		return 15;
	case 4961: //Bubbling Rage
		return 15;
	case 4962: //Boiling Rage
		return 15;
	case 4963: //Natimbi Gate
		return 123;
	case 4964: //Translocate: Natimbi
		return 123;
	case 4965: //Natimbi Portal
		return 123;
	case 4966: //Circle of Natimbi
		return 123;
	case 4967: //Ring of Natimbi
		return 123;
	case 4968: //Dark Arrow
		return 25;
	case 4970: //Prism Strike
		return 25;
	case 4971: //Ancient: Chaos Chant
		return 20;
	case 4972: //Ancient: Frozen Chaos
		return 25;
	case 4973: //Ancient: Chaos Censure
		return 25;
	case 4974: //Ancient: Chaos Frost
		return 25;
	case 4975: //Ancient: Chaos Madness
		return 25;
	case 4976: //Ancient: Chaos Vortex
		return 25;
	case 4977: //Ancient: Force of Chaos
		return 25;
	case 4978: //Ancient: Seduction of Chaos
		return 125;
	case 4979: //Ancient: Chaotic Pain
		return 125;
	case 4980: //Ancient: Burning Chaos
		return 25;
	case 4981: //Ancient: Strike of Chaos
		return 25;
	case 4982: //Ancient: Bite of Chaos
		return 114;
	case 4991: //Coordinated Strike
		return 25;
	case 4992: //Malevolent Assault
		return 20;
	case 4993: //Malevolent Vex
		return 25;
	case 4994: //Searing Blood Arrow I
		return 126;
	case 4995: //Searing Blood Arrow II
		return 126;
	case 4996: //Searing Blood Arrow III
		return 126;
	case 4997: //Arrow of Renewal
		return 42;
	case 5000: //Righteous Assault
		return 25;
	case 5001: //Bury
		return 25;
	case 5002: //Mana Blast
		return 25;
	case 5003: //Impoverished Lifeblood
		return 25;
	case 5004: //Tamuik's Suggestion
		return 25;
	case 5005: //Tamuik's Ghastly Presence
		return 25;
	case 5006: //Tamuik's Spectral Step
		return 25;
	case 5007: //Curse of Tunik Tamuik
		return 25;
	case 5008: //Bane of Tunik Tamuik
		return 25;
	case 5009: //Unholy Barrage
		return 25;
	case 5010: //Strike of Glory
		return 25;
	case 5011: //Salve
		return 42;
	case 5012: //Spike of Disease
		return 25;
	case 5013: //SpellTheft2
		return 126;
	case 5014: //SpellTheft3
		return 126;
	case 5015: //Bellow of the Mastruq
		return 15;
	case 5016: //Ancient: Chaos Cry
		return 15;
	case 5017: //Kyv Strike
		return 15;
	case 5018: //Ancient: Chaos Strike
		return 15;
	case 5019: //Phantom Shadow
		return 15;
	case 5020: //Ancient: Phantom Chaos
		return 15;
	case 5022: //Dark Balance
		return 25;
	case 5023: //Chaos Epidemic
		return 25;
	case 5024: //Chaos Epidemic
		return 25;
	case 5027: //Battle Cry
		return 95;
	case 5028: //War Cry
		return 95;
	case 5029: //Battle Cry of Dravel
		return 95;
	case 5030: //War Cry of Dravel
		return 95;
	case 5031: //Battle Cry of the Mastruq
		return 95;
	case 5032: //Ancient: Cry of Chaos
		return 95;
	case 5033: //Berserker Rage
		return 95;
	case 5034: //Burning Rage Discipline
		return 27;
	case 5035: //Focused Fury Discipline
		return 27;
	case 5036: //Battle Sense Discipline
		return 27;
	case 5037: //Cleaving Rage Discipline
		return 27;
	case 5038: //Battle Focus Discipline
		return 27;
	case 5039: //Inspired Anger Discipline
		return 27;
	case 5040: //Reckless Discipline
		return 27;
	case 5041: //Blind Rage Discipline
		return 27;
	case 5042: //Indomitable Discipline
		return 27;
	case 5043: //Cleaving Anger Discipline
		return 27;
	case 5044: //Sprint Discipline
		return 27;
	case 5045: //Test1
		return 27;
	case 5046: //Test2
		return 15;
	case 5047: //Test3
		return 15;
	case 5048: //Test4
		return 15;
	case 5049: //Test5
		return 15;
	case 5050: //Test6
		return 27;
	case 5051: //Aura of Destruction
		return 25;
	case 5052: //Spirit's Touch
		return 25;
	case 5053: //Destructive Crush
		return 25;
	case 5054: //Wave of Destruction
		return 25;
	case 5055: //Creeping Fury
		return 25;
	case 5056: //Rampaging Force
		return 25;
	case 5057: //Barxt's Destructive Touch
		return 25;
	case 5058: //Barxt's Mental Corruption
		return 25;
	case 5059: //Wave of Absolute Power
		return 25;
	case 5060: //Discordant Light
		return 42;
	case 5063: //Mug
		return 115;
	case 5064: //Hastened Thoughts
		return 125;
	case 5070: //Armor Shatter
		return 25;
	case 5071: //Energy Siphon
		return 25;
	case 5073: //Soul Vortex
		return 25;
	case 5074: //Black Pox
		return 25;
	case 5094: //test ac
		return 27;
	case 5095: //Item Pet I
		return 69;
	case 5096: //Item Pet II
		return 69;
	case 5101: //Fire Shard
		return 18;
	case 5102: //Frost Hammer
		return 18;
	case 5103: //Flame Strike
		return 25;
	case 5104: //Frost Strike
		return 25;
	case 5105: //Geomantra
		return 125;
	case 5107: //Tainted Axe of Hatred
		return 18;
	case 5118: //Intoxicating Fury
		return 25;
	case 5119: //Force of Trusik's Rage
		return 25;
	case 5120: //Withering Destruction
		return 25;
	case 5125: //Venom Claw
		return 25;
	case 5127: //Prism Skin
		return 25;
	case 5133: //Elemental Draw
		return 69;
	case 5135: //SpellTheft4
		return 69;
	case 5148: //Massive Explosion
		return 25;
	case 5150: //Gloomingdeep Guard
		return 45;
	case 5190: //PvPSilTest1
		return 25;
	case 5191: //PvPSilTest2
		return 25;
	case 5192: //PvPSilTest3
		return 25;
	case 5193: //PvPSilTest4
		return 25;
	case 5194: //PvPSilTest5
		return 25;
	case 5195: //PvPSil2Test1
		return 25;
	case 5196: //PvPSil2Test2
		return 25;
	case 5197: //PvPSil2Test3
		return 25;
	case 5198: //PvPSil2Test4
		return 25;
	case 5199: //PvPSil2Test5
		return 25;
	case 5200: //PvPStunTest1
		return 69;
	case 5201: //PvPStunTest2
		return 69;
	case 5202: //PvPStunTest3
		return 69;
	case 5203: //PvPStunTest4
		return 69;
	case 5204: //PvPStunTest5
		return 69;
	case 5205: //5200 Strike
		return 69;
	case 5206: //5201 Strike
		return 69;
	case 5207: //5202 Strike
		return 69;
	case 5208: //5203 Strike
		return 69;
	case 5209: //5204 Strike
		return 69;
	case 5210: //PvPSnareTest1
		return 69;
	case 5211: //PvPSnareTest2
		return 69;
	case 5212: //PvPSnareTest3
		return 69;
	case 5213: //PvPSnareTest4
		return 69;
	case 5214: //PvPSnareTest5
		return 69;
	case 5215: //5210 Strike
		return 69;
	case 5216: //5211 Strike
		return 69;
	case 5217: //5212 Strike
		return 69;
	case 5218: //5213 Strike
		return 69;
	case 5219: //5214 Strike
		return 69;
	case 5220: //Jarsath Frenzy
		return 95;
	case 5221: //Rage of Xyzith
		return 25;
	case 5222: //Morternum
		return 114;
	case 5226: //Arias' Guard
		return 79;
	case 5250: //Confidence
		return 45;
	case 5251: //Pious Remedy
		return 42;
	case 5252: //Symbol of Balikor
		return 45;
	case 5253: //Ward of Valiance
		return 95;
	case 5254: //Shock of Wonder
		return 25;
	case 5255: //Sermon of Reproach
		return 20;
	case 5256: //Unswerving Hammer of Retribution
		return 69;
	case 5257: //Conviction
		return 45;
	case 5258: //Blessing of Devotion
		return 125;
	case 5259: //Pious Elixir
		return 42;
	case 5260: //Reproach
		return 25;
	case 5261: //Panoply of Vie
		return 125;
	case 5262: //Omen-Cleric-PH
		return 125;
	case 5263: //Omen-Cleric-PH
		return 125;
	case 5264: //Hammer of Reproach
		return 18;
	case 5265: //Pious Light
		return 42;
	case 5266: //Sound of Divinity
		return 25;
	case 5267: //Omen-Cleric-PH
		return 126;
	case 5268: //Desolate Undead
		return 25;
	case 5269: //Mark of the Blameless
		return 125;
	case 5270: //Word of Vivification
		return 42;
	case 5271: //Calamity
		return 25;
	case 5272: //Aura of Devotion
		return 125;
	case 5273: //Yaulp VII
		return 95;
	case 5274: //Placate
		return 126;
	case 5275: //Silent Dictation
		return 25;
	case 5276: //Armor of the Pious
		return 45;
	case 5277: //Balikor's Mark
		return 45;
	case 5278: //Hand of Conviction
		return 45;
	case 5279: //Ancient: Pious Conscience
		return 25;
	case 5280: //Direction
		return 45;
	case 5281: //Omen-Paladin-PH
		return 126;
	case 5282: //Touch of Piety
		return 42;
	case 5283: //Crusader's Purity
		return 42;
	case 5284: //Force of Piety
		return 25;
	case 5285: //Silvered Fury
		return 125;
	case 5286: //Spurn Undead
		return 25;
	case 5287: //Symbol of Jeron
		return 45;
	case 5288: //Pious Fury
		return 125;
	case 5289: //Light of Piety
		return 42;
	case 5290: //Hand of Direction
		return 45;
	case 5291: //Armor of the Champion
		return 45;
	case 5292: //Serene Command
		return 25;
	case 5293: //Pious Cleansing
		return 42;
	case 5294: //Bulwark of Piety
		return 95;
	case 5295: //Jeron's Mark
		return 45;
	case 5296: //Wave of Piety
		return 42;
	case 5297: //Brell's Brawny Bulwark
		return 45;
	case 5298: //Affirmation
		return 45;
	case 5299: //Ancient: Force of Jeron
		return 25;
	case 5300: //Nature Veil
		return 126;
	case 5301: //Displace Summoned
		return 25;
	case 5302: //Shield of Briar
		return 125;
	case 5303: //Locust Swarm
		return 20;
	case 5304: //Sylvan Water
		return 42;
	case 5305: //Guard of the Earth
		return 125;
	case 5306: //Strength of the Hunter
		return 45;
	case 5307: //Briarcoat
		return 125;
	case 5308: //Nature's Veil Parry
		return 126;
	case 5309: //Frost Wind
		return 25;
	case 5310: //Hunter's Vigor
		return 79;
	case 5311: //Nature's Denial
		return 125;
	case 5312: //Howl of the Predator
		return 95;
	case 5313: //Hearth Embers
		return 25;
	case 5314: //Nature's Balance
		return 126;
	case 5315: //Onyx Skin
		return 45;
	case 5316: //Tranquility of the Glade
		return 126;
	case 5317: //Ward of the Hunter
		return 125;
	case 5318: //Call of Lightning
		return 125;
	case 5319: //Ancient: North Wind
		return 25;
	case 5320: //Blood of Discord
		return 20;
	case 5321: //Dark Tendrils
		return 20;
	case 5322: //Dark Constriction
		return 20;
	case 5323: //Bond of Inruku
		return 114;
	case 5324: //Touch of Inruku
		return 114;
	case 5325: //Inruku's Bite
		return 114;
	case 5326: //Omen-SK-PH
		return 25;
	case 5327: //Shroud of Discord
		return 125;
	case 5328: //Theft of Pain
		return 114;
	case 5329: //Terror of Discord
		return 126;
	case 5330: //Blood of Inruku
		return 20;
	case 5331: //Son of Decay
		return 69;
	case 5332: //Rune of Decay
		return 69;
	case 5333: //Pact of Decay
		return 125;
	case 5334: //Spear of Muram
		return 25;
	case 5335: //Scythe of Inruku
		return 25;
	case 5336: //Dread Gaze
		return 126;
	case 5337: //Theft of Hate
		return 114;
	case 5338: //Touch of the Devourer
		return 114;
	case 5339: //Cloak of Discord
		return 45;
	case 5340: //Ancient: Bite of Muram
		return 114;
	case 5341: //Omen-Druid-PH
		return 126;
	case 5342: //Oaken Vigor
		return 79;
	case 5343: //Stormwatch
		return 25;
	case 5344: //Hand of the Sun
		return 126;
	case 5345: //Tempest Wind
		return 25;
	case 5346: //Earth Shiver
		return 25;
	case 5347: //Nature's Serenity
		return 126;
	case 5348: //Immolation of the Sun
		return 20;
	case 5349: //Hungry Vines
		return 126;
	case 5350: //Lion's Strength
		return 95;
	case 5351: //Sun's Corona
		return 126;
	case 5352: //Steeloak Skin
		return 45;
	case 5353: //Blessing of Oak
		return 79;
	case 5354: //Glacier Breath
		return 126;
	case 5355: //Chlorotrope
		return 42;
	case 5356: //Oaken Guard
		return 125;
	case 5357: //Wasp Swarm
		return 20;
	case 5358: //Nettle Shield
		return 125;
	case 5359: //Nature's Beckon
		return 126;
	case 5360: //Omen-Druid-PH
		return 126;
	case 5361: //Solstice Strike
		return 25;
	case 5362: //Nettlecoat
		return 125;
	case 5363: //Vengeance of the Sun
		return 20;
	case 5364: //Desolate Summoned
		return 25;
	case 5365: //Circle of Nettles
		return 125;
	case 5366: //Blessing of Steeloak
		return 45;
	case 5367: //Glitterfrost
		return 25;
	case 5368: //Mask of the Wild
		return 79;
	case 5369: //Ancient: Glacier Frost
		return 25;
	case 5370: //Luvwen's Aria of Serenity
		return 126;
	case 5371: //Vulka's Chant of Disease
		return 20;
	case 5372: //Bellow of Chaos
		return 25;
	case 5373: //Luvwen's Lullaby
		return 126;
	case 5374: //Verse of Vesagran
		return 95;
	case 5375: //Zuriki's Song of Shenanigans
		return 126;
	case 5376: //War March of Muram
		return 125;
	case 5377: //Cantata of Life
		return 79;
	case 5378: //Vulka's Chant of Poison
		return 20;
	case 5379: //Vulka's Chant of Frost
		return 20;
	case 5380: //Yelhun's Mystic Call
		return 125;
	case 5381: //Dirge of Metala
		return 126;
	case 5382: //Eriki's Psalm of Power
		return 125;
	case 5383: //Voice of the Vampire
		return 126;
	case 5384: //Chorus of Life
		return 79;
	case 5385: //Vulka's Chant of Flame
		return 20;
	case 5386: //Omen-Bard-PH
		return 126;
	case 5387: //Vulka's Lullaby
		return 126;
	case 5388: //Ancient: Call of Power
		return 125;
	case 5389: //Farrel's Companion
		return 69;
	case 5390: //Spirit of Sense
		return 95;
	case 5391: //Yoppa's Spear of Venom
		return 25;
	case 5392: //Putrid Decay
		return 126;
	case 5393: //Spirit of Perseverance
		return 79;
	case 5394: //Crippling Spasm
		return 126;
	case 5395: //Yoppa's Mending
		return 42;
	case 5396: //Wunshi's Focusing
		return 45;
	case 5397: //Ancestral Bulwark
		return 95;
	case 5398: //Spirit of Fortitude
		return 95;
	case 5399: //Talisman of Sense
		return 95;
	case 5400: //Vindictive Spirit
		return 126;
	case 5401: //Yoppa's Rain of Venom
		return 25;
	case 5402: //Spirit Veil
		return 125;
	case 5403: //Pained Memory
		return 125;
	case 5404: //Spirit of Might
		return 95;
	case 5405: //Talisman of Fortitude
		return 95;
	case 5406: //Talisman of Perseverance
		return 79;
	case 5407: //Shroud of Erana
		return 126;
	case 5408: //Ice Age
		return 25;
	case 5409: //Talisman of Might
		return 95;
	case 5410: //Pure Spirit
		return 42;
	case 5411: //Breath of Wunshi
		return 20;
	case 5412: //Curse of Sisslak
		return 20;
	case 5413: //Shroud of Erana Parry
		return 126;
	case 5414: //Blood of Yoppa
		return 20;
	case 5415: //Talisman of Wunshi
		return 45;
	case 5416: //Spiritual Serenity
		return 42;
	case 5417: //Champion
		return 95;
	case 5418: //Ancient: Ancestral Calling
		return 125;
	case 5419: //Soulspike
		return 114;
	case 5420: //Acikin
		return 25;
	case 5421: //Shadow Guard
		return 45;
	case 5422: //Omen-Nec-PH
		return 69;
	case 5423: //Chaos Plague
		return 20;
	case 5424: //Grip of Mori
		return 20;
	case 5425: //Glyph of Darkness
		return 69;
	case 5426: //Fang of Death
		return 114;
	case 5427: //Scent of Midnight
		return 126;
	case 5428: //Dull Pain
		return 125;
	case 5429: //Dark Hold
		return 126;
	case 5430: //Desecrating Darkness
		return 20;
	case 5431: //Lost Soul
		return 69;
	case 5432: //Dark Nightmare
		return 20;
	case 5433: //Chaos Venom
		return 20;
	case 5434: //Dark Possession
		return 125;
	case 5435: //Dark Salve
		return 69;
	case 5436: //Bulwark of Calliav
		return 69;
	case 5437: //Pyre of Mori
		return 20;
	case 5438: //Dark Assassin
		return 69;
	case 5439: //Word of Chaos
		return 126;
	case 5440: //Desolate Undead
		return 25;
	case 5441: //Ancient: Curse of Mori
		return 20;
	case 5442: //Icebane
		return 25;
	case 5443: //Ether Shield
		return 45;
	case 5444: //Tears of the Sun
		return 25;
	case 5445: //Lightningbane
		return 25;
	case 5446: //Spark of Fire
		return 25;
	case 5447: //Firebane
		return 25;
	case 5448: //Ether Skin
		return 125;
	case 5449: //Spark of Thunder
		return 25;
	case 5450: //Thundaka
		return 25;
	case 5451: //Circle of Thunder
		return 25;
	case 5452: //Spark of Lightning
		return 25;
	case 5453: //Ether Ward
		return 125;
	case 5454: //Meteor Storm
		return 25;
	case 5455: //Circle of Fire
		return 25;
	case 5456: //Telekara
		return 25;
	case 5457: //Spark of Ice
		return 25;
	case 5458: //Gelidin Comet
		return 25;
	case 5459: //Bulwark of Calrena
		return 125;
	case 5460: //Solist's Frozen Sword
		return 69;
	case 5461: //Gelid Rains
		return 25;
	case 5462: //Corona Flare
		return 25;
	case 5463: //Ancient: Core Fire
		return 25;
	case 5464: //Summon Calliav's Runed Mantle
		return 18;
	case 5465: //Summon Staff of the North Wind
		return 18;
	case 5466: //Fireskin
		return 125;
	case 5467: //Summon Fireblade
		return 18;
	case 5468: //Summon Calliav's Jeweled Bracelet
		return 18;
	case 5469: //Summon Calliav's Spiked Ring
		return 18;
	case 5470: //Summon Calliav's Glowing Bauble
		return 18;
	case 5471: //Summon Calliav's Steel Bracelet
		return 18;
	case 5472: //Elemental Aura
		return 45;
	case 5473: //Child of Wind
		return 69;
	case 5474: //Bolt of Jerikor
		return 25;
	case 5475: //Summon Calliav's Platinum Choker
		return 18;
	case 5476: //Phantom Shield
		return 79;
	case 5477: //Summon Dagger of the Deep
		return 18;
	case 5478: //Elemental Fury
		return 69;
	case 5479: //Rain of Jerikor
		return 25;
	case 5480: //Child of Water
		return 69;
	case 5481: //Burning Earth
		return 25;
	case 5482: //Omen-Mage-PH
		return 18;
	case 5483: //Summon Pouch of Jerikor
		return 18;
	case 5484: //Blade Strike
		return 25;
	case 5485: //Child of Fire
		return 69;
	case 5486: //Summon Sphere of Air
		return 18;
	case 5487: //Omen-Mage-PH
		return 126;
	case 5488: //Circle of Fireskin
		return 125;
	case 5489: //Summon Crystal Belt
		return 18;
	case 5490: //Desolate Summoned
		return 25;
	case 5491: //Renewal of Jerikor
		return 69;
	case 5492: //Pyrilen Skin
		return 125;
	case 5493: //Star Scream
		return 25;
	case 5494: //Bulwark of Calliav
		return 69;
	case 5495: //Child of Earth
		return 69;
	case 5496: //Star Strike
		return 25;
	case 5497: //Elemental Simulcram
		return 69;
	case 5498: //Ancient: Nova Strike
		return 25;
	case 5499: //Synapsis Spasm
		return 126;
	case 5500: //Ethereal Rune
		return 125;
	case 5501: //Omen-Enc-PH
		return 126;
	case 5502: //Mystic Shield
		return 45;
	case 5503: //Felicity
		return 126;
	case 5504: //Rune of Salik
		return 125;
	case 5505: //Salik's Animation
		return 69;
	case 5506: //Placate
		return 126;
	case 5507: //Speed of Salik
		return 125;
	case 5508: //Omen-Enc-PH
		return 95;
	case 5509: //Arcane Noose
		return 20;
	case 5510: //Compel
		return 126;
	case 5511: //Wake of Felicity
		return 126;
	case 5512: //Omen-Enc-PH
		return 20;
	case 5513: //Clairvoyance
		return 79;
	case 5514: //Mayhem
		return 125;
	case 5515: //Wall of Alendar
		return 125;
	case 5516: //Color Snap
		return 25;
	case 5517: //Circle of Alendar
		return 125;
	case 5518: //Psychosis
		return 25;
	case 5519: //True Name
		return 126;
	case 5520: //Euphoria
		return 126;
	case 5521: //Hastening of Salik
		return 125;
	case 5522: //Voice of Clairvoyance
		return 79;
	case 5523: //Ancient: Neurosis
		return 25;
	case 5524: //Omen-Bst-PH
		return 126;
	case 5525: //Omen-Bst-PH
		return 45;
	case 5526: //Healing of Mikkily
		return 69;
	case 5527: //Chimera Blood
		return 20;
	case 5528: //Muada's Mending
		return 42;
	case 5529: //Focus of Alladnu
		return 45;
	case 5530: //Spiritual Vitality
		return 45;
	case 5531: //Spirit of Alladnu
		return 69;
	case 5532: //Omen-Bst-PH
		return 125;
	case 5533: //Growl of the Beast
		return 69;
	case 5534: //Spirit of Irionu
		return 69;
	case 5535: //Glacier Spear
		return 25;
	case 5536: //Feral Vigor
		return 79;
	case 5537: //Spiritual Ascendance
		return 79;
	case 5538: //Spirit of Rashara
		return 69;
	case 5539: //Feral Guard
		return 69;
	case 5540: //Festering Malady
		return 20;
	case 5541: //Omen-Bst-PH
		return 126;
	case 5542: //Ferocity of Irionu
		return 95;
	case 5543: //Ancient: Savage Ice
		return 25;
	case 5554: //Cloud of Discord
		return 25;
	case 5555: //Bellow of Tunat'Muram
		return 25;
	case 5556: //Whirling Smash
		return 25;
	case 5560: //Blistering Rage
		return 15;
	case 5570: //Pillage Magic
		return 126;
	case 5571: //Tangle
		return 126;
	case 5572: //Entangle
		return 126;
	case 5573: //Corporeal Empathy Recourse
		return 42;
	case 5605: //test mana
		return 79;
	case 5731: //Circle of Barindu
		return 123;
	case 5732: //Barindu Portal
		return 123;
	case 5733: //Ring of Barindu
		return 123;
	case 5734: //Barindu Gate
		return 123;
	case 5735: //Translocate: Barindu
		return 123;
	case 5741: //Pyrilen Bolt
		return 25;
	case 5744: //Kiss of the Pyrilen
		return 25;
	case 5745: //Pyrilonis' Vengeance
		return 25;
	case 5760: //Gelaqua's Embrace
		return 25;
	case 5761: //Heart of Frost
		return 25;
	case 5874: //Advanced Dire Charm
		return 126;
	case 5875: //Advanced Dire Charm Animal
		return 126;
	case 5876: //Advanced Dire Charm Undead
		return 126;
	case 5919: //Death Peace
		return 125;
	case 5924: //Celestial Stun
		return 25;
	case 5931: //Embrace of Shadows
		return 125;
	case 5976: //Plagued Filth
		return 25;
	case 5979: //Infection of Pain
		return 25;
	case 5980: //Orb's Curse
		return 25;
	case 5989: //Gloom Toxin
		return 25;
	case 5990: //Shade Mantle
		return 25;
	case 5992: //Numbing Touch
		return 25;
	case 5993: //Girplan Chatter
		return 25;
	case 5994: //Body Slam
		return 25;
	case 5996: //Bazu Grip
		return 25;
	case 5997: //Pyrilen Ember
		return 25;
	case 5998: //Pyrilen Charm
		return 25;
	case 5999: //Pyronic Lash
		return 25;
	case 6000: //Pyrilen Fury
		return 25;
	case 6001: //Pyronic Assault
		return 25;
	case 6003: //Chimeran Laceration
		return 25;
	case 6004: //Chimeran Breath
		return 25;
	case 6005: //Infected Bite
		return 25;
	case 6006: //Gelidran Sleet
		return 25;
	case 6007: //Frostcicle
		return 25;
	case 6008: //Gelidran Hail
		return 25;
	case 6009: //Gelidran Stalagmite
		return 25;
	case 6010: //Freezing Touch
		return 25;
	case 6012: //Crushing Ice
		return 25;
	case 6013: //Ice Shards
		return 25;
	case 6014: //Feranic Grasp
		return 25;
	case 6016: //Feran Tentacle
		return 25;
	case 6017: //Darkbreath
		return 25;
	case 6019: //Clinging Apathy
		return 25;
	case 6020: //Wing Strike
		return 25;
	case 6021: //Deep Gouge
		return 25;
	case 6023: //Dragornian Malady
		return 25;
	case 6024: //Dragornian Venom
		return 25;
	case 6025: //Discordling Leap
		return 25;
	case 6026: //Discordling Ruin
		return 25;
	case 6027: //Chaotica
		return 25;
	case 6028: //Seething Bite
		return 25;
	case 6029: //Sinking Fangs
		return 25;
	case 6049: //Lightning Strike
		return 25;
	case 6050: //Static Pulse
		return 25;
	case 6051: //Fire Strike
		return 25;
	case 6052: //Ice Strike
		return 25;
	case 6053: //Bane of Dranik
		return 25;
	case 6054: //Bane of Dranik
		return 25;
	case 6055: //Bane of Dranik
		return 25;
	case 6065: //Murk Acid
		return 25;
	case 6066: //Murk Acid
		return 25;
	case 6103: //Nature's Denial Strike
		return 25;
	case 6105: //Silvered Fury Strike
		return 25;
	case 6120: //Phase Walk
		return 125;
	case 6121: //Shroud of Air
		return 125;
	case 6122: //Cloud of Indifference
		return 125;
	case 6123: //Cloak of Nature
		return 125;
	case 6124: //Shadow of Death
		return 125;
	case 6125: //Sun Cloak
		return 125;
	case 6126: //Hand of Order
		return 25;
	case 6128: //Gelidran Aura
		return 125;
	case 6140: //Ancient: Hallowed Light
		return 42;
	case 6141: //Ancient: Chlorobon
		return 42;
	case 6142: //Ancient: Wilslik's Mending
		return 42;
	case 6143: //Ancient: Touch of Orshilak
		return 114;
	case 6144: //Ancient: Voice of Muram
		return 126;
	case 6145: //Ancient: Veil of Pyrilonus
		return 125;
	case 6146: //Ancient: Spear of Gelaqua
		return 25;
	case 6152: //Vindictive Spirit Recourse
		return 125;
	case 6153: //Elemental Simulcram Recourse
		return 45;
	case 6154: //Hungry Vines Recourse
		return 125;
	case 6156: //Oaken Guard Parry
		return 126;
	case 6169: //Crippling Strike
		return 126;
	case 6170: //Mind Strike
		return 25;
	case 6171: //Baffling Strike
		return 126;
	case 6172: //Axe of the Destroyer
		return 18;
	case 6173: //Bazu Bellow
		return 15;
	case 6174: //Daggerfall
		return 15;
	case 6175: //Phantom Cry
		return 15;
	case 6176: //Slaughter Gate
		return 123;
	case 6177: //Translocate: Slaughter
		return 123;
	case 6178: //Slaughter Portal
		return 123;
	case 6179: //Circle of Slaughter
		return 123;
	case 6180: //Ring of Slaughter
		return 123;
	case 6181: //Bloodfields Gate
		return 123;
	case 6182: //Translocate: Bloodfields
		return 123;
	case 6183: //Bloodfields Portal
		return 123;
	case 6184: //Circle of Bloodfields
		return 123;
	case 6185: //Ring of Bloodfields
		return 123;
	case 6187: //Limit Test Hit
		return 114;
	case 6190: //Shocking Defense Discipline
		return 27;
	case 6191: //Aura of Runes Discipline
		return 27;
	case 6192: //Savage Onslaught Discipline
		return 27;
	case 6193: //Dreamwalk Discipline
		return 27;
	case 6194: //Rapid Kick Discipline
		return 27;
	case 6195: //Counterforce Discipline
		return 27;
	case 6196: //Deadly Aim Discipline
		return 27;
	case 6197: //Frenzied Stabbing Discipline
		return 27;
	case 6198: //Imperceptible Discipline
		return 27;
	case 6199: //Vengeful Flurry Discipline
		return 27;
	case 6200: //Unpredictable Rage Discipline
		return 27;
	case 6201: //Unflinching Will Discipline
		return 27;
	case 6202: //Stun Effect
		return 27;
	case 6203: //Rune Effect
		return 27;
	case 6204: //Slow Effect
		return 27;
	case 6205: //Poison DD Effect
		return 27;
	case 6206: //Lower Hate Effect
		return 27;
	case 6207: //Increase Damage Effect
		return 27;
	case 6209: //Test Doom Rune
		return 125;
	case 6233: //Harmonic Balance
		return 95;
	case 6265: //Divine Balance
		return 95;
	case 6330: //Chaotic Strike I
		return 25;
	case 6331: //Chaotic Strike II
		return 25;
	case 6332: //Chaotic Strike III
		return 25;
	case 6333: //Chaotic Strike IV
		return 25;
	case 6334: //Chaotic Strike V
		return 25;
	case 6335: //Chaotic Strike VI
		return 25;
	case 6336: //Chaotic Strike VII
		return 25;
	case 6337: //Life Sap I
		return 25;
	case 6338: //Life Sap II
		return 25;
	case 6339: //Life Sap III
		return 25;
	case 6340: //Life Sap IV
		return 25;
	case 6341: //Freezing Strike I
		return 25;
	case 6342: //Freezing Strike II
		return 25;
	case 6343: //Freezing Strike III
		return 25;
	case 6344: //Freezing Strike IV
		return 25;
	case 6345: //Freezing Strike V
		return 25;
	case 6346: //Freezing Strike VI
		return 25;
	case 6347: //Freezing Strike VII
		return 25;
	case 6348: //Freezing Strike VIII
		return 25;
	case 6349: //Freezing Strike IX
		return 25;
	case 6350: //Fiery Strike I
		return 25;
	case 6351: //Fiery Strike II
		return 25;
	case 6352: //Fiery Strike III
		return 25;
	case 6353: //Fiery Strike IV
		return 25;
	case 6354: //Fiery Strike V
		return 25;
	case 6355: //Fiery Strike VI
		return 25;
	case 6356: //Fiery Strike VII
		return 25;
	case 6357: //Fiery Strike VIII
		return 25;
	case 6358: //Fiery Strike IX
		return 25;
	case 6359: //Form of Defense I
		return 125;
	case 6360: //Form of Defense III
		return 125;
	case 6361: //Form of Protection I
		return 125;
	case 6362: //Form of Protection III
		return 125;
	case 6363: //Form of Endurance I
		return 125;
	case 6364: //Form of Endurance III
		return 125;
	case 6365: //Form of Rejuvenation I
		return 125;
	case 6366: //Form of Rejuvenation III
		return 125;
	case 6499: //Gelid Claw
		return 25;
	case 6500: //Stunning Strike
		return 25;
	case 6502: //Unpack Brewer's Still
		return 18;
	case 6505: //Blood Dream
		return 25;
	case 6512: //Lupine Rage
		return 25;
	case 6513: //Devour Enchantment
		return 126;
	case 6514: //Blessing of Rallos Zek
		return 18;
	case 6515: //Blessing of The Tribunal
		return 18;
	case 6516: //Blessing of Cazic Thule
		return 18;
	case 6517: //Blessing of Brell Serilis
		return 18;
	case 6518: //Blessing of Tunare
		return 18;
	case 6519: //Blessing of Innoruuk
		return 18;
	case 6520: //Blessing of Prexus
		return 18;
	case 6521: //Blessing of Mithaniel Marr
		return 18;
	case 6522: //Blessing of Erollisi Marr
		return 18;
	case 6523: //Blessing of Bertoxxulous
		return 18;
	case 6524: //Blessing of Solusek Ro
		return 18;
	case 6525: //Blessing of Karana
		return 18;
	case 6526: //Blessing of Bristlebane
		return 18;
	case 6527: //Blessing of Quellious
		return 18;
	case 6528: //Blessing of Rodcet Nife
		return 18;
	case 6529: //Blessing of Veeshan
		return 18;
	case 6530: //Words of the Sceptic
		return 18;
	case 6532: //Makural's Curse
		return 125;
	case 6533: //RB_reflect_test
		return 125;
	case 6534: //Makural's Torment
		return 25;
	case 6535: //Makural's TormentSK
		return 25;
	case 6563: //Mass Illusion: Human
		return 125;
	case 6564: //Mass Illusion: Barbarian
		return 125;
	case 6565: //Mass Illusion: Erudite
		return 125;
	case 6566: //Mass Illusion: Wood Elf
		return 125;
	case 6567: //Mass Illusion: Fier`dal
		return 125;
	case 6568: //Mass Illusion: High Elf
		return 125;
	case 6569: //Mass Illusion: Dark Elf
		return 125;
	case 6570: //Mass Illusion: Half-Elf
		return 125;
	case 6571: //Mass Illusion: Dwarf
		return 125;
	case 6572: //Mass Illusion: Troll
		return 125;
	case 6573: //Mass Illusion: Ogre
		return 125;
	case 6574: //Mass Illusion: Halfling
		return 125;
	case 6575: //Mass Illusion: Gnome
		return 125;
	case 6576: //Mass Illusion: Werewolf
		return 125;
	case 6577: //Mass Illusion: Froglok
		return 125;
	case 6578: //Mass Illusion: Imp
		return 125;
	case 6579: //Mass Illusion: Earth Elemental
		return 125;
	case 6580: //Mass Illusion: Air Elemental
		return 125;
	case 6581: //Mass Illusion: Fire Elemental
		return 125;
	case 6582: //Mass Illusion: Water Elemental
		return 125;
	case 6583: //Mass Illusion: Scarecrow
		return 125;
	case 6584: //Mass Illusion: Spirit Wolf
		return 125;
	case 6585: //Mass Illusion: Iksar
		return 125;
	case 6586: //Mass Illusion: Vah Shir
		return 125;
	case 6587: //Mass Illusion: Guktan
		return 125;
	case 6588: //Mass Illusion: Scaled Wolf
		return 125;
	case 6589: //Mass Illusion: Skeleton
		return 125;
	case 6590: //Mass Illusion: Dry Bone
		return 125;
	case 6591: //Mass Illusion: Frost Bone
		return 125;
	case 6592: //GM Bind Sight
		return 125;
	case 6656: //Spray of Venom
		return 25;
	case 6662: //Ward of Retribution
		return 125;
	case 6663: //Guard of Righteousness
		return 27;
	case 6664: //Earthen Shackles
		return 126;
	case 6665: //Serpent Vines
		return 126;
	case 6666: //Storm Blade
		return 125;
	case 6667: //Spirit of the Panther
		return 125;
	case 6668: //Shadow Orb
		return 114;
	case 6669: //Claw of Vox
		return 25;
	case 6670: //Summon: Molten Orb
		return 18;
	case 6671: //Rune of Rikkukin
		return 125;
	case 6672: //Growl of the Panther
		return 69;
	case 6673: //Soul Shield
		return 27;
	case 6674: //Storm Blade Strike
		return 25;
	case 6675: //Storm Blade Strike SK
		return 25;
	case 6676: //Magma Jet
		return 25;
	case 6677: //Shadow Orb Recourse
		return 18;
	case 6717: //Growl of the Panther
		return 95;
	case 6719: //Ward of Retribution Parry
		return 126;
	case 6724: //Panther Maw
		return 25;
	case 6725: //Cyclone Blade
		return 126;
	case 6726: //Assassin's Feint
		return 15;
	case 6727: //Dragon Fang
		return 15;
	case 6728: //Dragon Fang Strike
		return 15;
	case 6729: //Destroyer's Volley
		return 15;
	case 6730: //Ward of Vengeance
		return 125;
	case 6731: //Guard of Humility
		return 27;
	case 6732: //Earthen Embrace
		return 126;
	case 6733: //Mire Thorns
		return 126;
	case 6734: //Song of the Storm
		return 125;
	case 6735: //Spirit of the Leopard
		return 95;
	case 6736: //Soul Orb
		return 114;
	case 6737: //Claw of Frost
		return 25;
	case 6738: //Summon: Lava Orb
		return 18;
	case 6739: //Rune of the Scale
		return 125;
	case 6740: //Growl of the Leopard
		return 69;
	case 6741: //Soul Guard
		return 27;
	case 6742: //Song of the Storm Strike
		return 25;
	case 6743: //Song of the Storm Strike SK
		return 25;
	case 6744: //Lava Jet
		return 25;
	case 6745: //Soul Orb Recourse
		return 18;
	case 6747: //Growl of the Leopard
		return 95;
	case 6748: //Ward of Vengeance Parry
		return 126;
	case 6749: //Leopard Maw
		return 25;
	case 6750: //Whirlwind Blade
		return 126;
	case 6751: //Rogue's Ploy
		return 15;
	case 6752: //Leopard Claw
		return 15;
	case 6753: //Leopard Claw Strike
		return 15;
	case 6754: //Rage Volley
		return 15;
	case 6771: //Geomantra II
		return 125;
	case 6777: //Leopard Maw
		return 25;
	case 6778: //Leopard Maw SK
		return 25;
	case 6779: //Panther Maw
		return 25;
	case 6780: //Panther Maw SK
		return 25;
	case 6782: //Magma Blast
		return 25;
	case 6826: //Desolate Deeds
		return 126;
	case 6827: //Balance of Discord
		return 126;
	case 6828: //Sha's Legacy
		return 126;
	case 6839: //Static Strike
		return 25;
	case 6840: //Firestrike
		return 25;
	case 6841: //Bolt of Flame
		return 25;
	case 6842: //Cinder Bolt
		return 25;
	case 6843: //Anarchy
		return 25;
	case 6844: //Shock of Spikes
		return 25;
	case 6845: //Dismiss Summoned
		return 25;
	case 6846: //Dismiss Undead
		return 25;
	case 6847: //Blaze
		return 25;
	case 6848: //Shock of Poison
		return 25;
	case 6849: //Shock of Flame
		return 25;
	case 6850: //Chaos Flux
		return 25;
	case 6851: //Icestrike
		return 25;
	case 6852: //Icicle Shock
		return 25;
	case 6853: //Lifedraw
		return 114;
	case 6854: //Drain Soul
		return 114;
	case 6855: //Frost Shock
		return 25;
	case 6856: //Inferno Shock
		return 25;
	case 6857: //Lightning Shock
		return 25;
	case 6858: //Winter's Roar
		return 25;
	case 6859: //Spirit Tap
		return 114;
	case 6860: //Drain Spirit
		return 114;
	case 6861: //Shock of Ice
		return 25;
	case 6862: //Flame Shock
		return 25;
	case 6863: //Ice Shock
		return 25;
	case 6864: //Conflagration
		return 25;
	case 6865: //Expel Undead
		return 25;
	case 6866: //Rend
		return 25;
	case 6867: //Torbas' Acid Blast
		return 25;
	case 6868: //Frost
		return 25;
	case 6869: //Sunstrike
		return 25;
	case 6870: //Blast of Frost
		return 25;
	case 6871: //Shock of Fiery Blades
		return 25;
	case 6872: //Burning Arrow
		return 25;
	case 6873: //Nature's Renewal
		return 42;
	case 6874: //Spirit Salve
		return 42;
	case 6875: //Healing Light
		return 42;
	case 6876: //Forest's Renewal
		return 42;
	case 6877: //Kragg's Salve
		return 42;
	case 6878: //Greater Healing Light
		return 42;
	case 6899: //Flash Powder Explosion
		return 25;
	case 6902: //Ward of the Divine
		return 125;
	case 6903: //Ward of Rebuke
		return 125;
	case 6904: //Ward of the Divine Parry
		return 126;
	case 6905: //Ward of Rebuke Parry
		return 126;
	case 6906: //Spirit of the Puma
		return 125;
	case 6907: //Spirit of the Jaguar
		return 125;
	case 6908: //Puma Maw
		return 25;
	case 6909: //Puma Maw SK
		return 25;
	case 6910: //Elixir of Healing I
		return 42;
	case 6911: //Elixir of Healing II
		return 42;
	case 6912: //Elixir of Healing III
		return 42;
	case 6913: //Elixir of Healing IV
		return 42;
	case 6914: //Elixir of Healing V
		return 42;
	case 6915: //Elixir of Healing VI
		return 42;
	case 6916: //Elixir of Healing VII
		return 42;
	case 6917: //Elixir of Healing VIII
		return 42;
	case 6918: //Elixir of Healing IX
		return 42;
	case 6919: //Elixir of Healing X
		return 42;
	case 6920: //Healing Potion I
		return 42;
	case 6921: //Healing Potion II
		return 42;
	case 6922: //Healing Potion III
		return 42;
	case 6923: //Healing Potion IV
		return 42;
	case 6924: //Healing Potion V
		return 42;
	case 6925: //Healing Potion VI
		return 42;
	case 6926: //Healing Potion VII
		return 42;
	case 6927: //Healing Potion VIII
		return 42;
	case 6928: //Healing Potion IX
		return 42;
	case 6929: //Healing Potion X
		return 42;
	case 6930: //Elixir of Health I
		return 45;
	case 6931: //Elixir of Health II
		return 45;
	case 6932: //Elixir of Health III
		return 45;
	case 6933: //Elixir of Health IV
		return 45;
	case 6934: //Elixir of Health V
		return 45;
	case 6935: //Elixir of Health VI
		return 45;
	case 6936: //Elixir of Health VII
		return 45;
	case 6937: //Elixir of Health VIII
		return 45;
	case 6938: //Elixir of Health IX
		return 45;
	case 6939: //Elixir of Health X
		return 45;
	case 6940: //Elixir of Speed I
		return 125;
	case 6941: //Elixir of Speed II
		return 125;
	case 6942: //Elixir of Speed III
		return 125;
	case 6943: //Elixir of Speed IV
		return 125;
	case 6944: //Elixir of Speed V
		return 125;
	case 6945: //Elixir of Speed VI
		return 125;
	case 6946: //Elixir of Speed VII
		return 125;
	case 6947: //Elixir of Speed VIII
		return 125;
	case 6948: //Elixir of Speed IX
		return 125;
	case 6949: //Elixir of Speed X
		return 125;
	case 6950: //Elixir of Clarity I
		return 79;
	case 6951: //Elixir of Clarity II
		return 79;
	case 6952: //Elixir of Clarity III
		return 79;
	case 6953: //Elixir of Clarity IV
		return 79;
	case 6954: //Elixir of Clarity V
		return 79;
	case 6955: //Elixir of Clarity VI
		return 79;
	case 6956: //Elixir of Clarity VII
		return 79;
	case 6957: //Elixir of Clarity VIII
		return 79;
	case 6958: //Elixir of Clarity IX
		return 79;
	case 6959: //Elixir of Clarity X
		return 79;
	case 6960: //Grip of Zanivar
		return 20;
	case 6961: //Zanivar's Lifedraw
		return 114;
	case 6962: //Zanivar's Poison Bolt
		return 20;
	case 6963: //Minion of Zanivar
		return 69;
	case 6965: //Rampage of Rathkan
		return 25;
	case 6966: //Hurl of Rathkan
		return 25;
	case 6967: //Shock of Rathkan
		return 25;
	case 6968: //Lantern Bomb
		return 25;
	case 6969: //Flashpowder Bomb
		return 25;
	case 6973: //Intangibility
		return 125;
	case 6976: //Retch Weed
		return 20;
	case 6977: //Deistic Voice
		return 126;
	case 6978: //Deistic Bellow
		return 126;
	case 6979: //Deistic Howl
		return 126;
	case 6980: //Unholy Voice
		return 126;
	case 6981: //Unholy Bellow
		return 126;
	case 6982: //Unholy Howl
		return 126;
	case 6983: //Phobia
		return 126;
	case 6984: //Jitterskin
		return 126;
	case 6985: //Anxiety Attack
		return 126;
	case 6986: //Shadow Voice
		return 126;
	case 6987: //Shadow Bellow
		return 126;
	case 6988: //Shadow Howl
		return 126;
	case 6989: //Cower the Dead
		return 126;
	case 6990: //Death's Despair
		return 126;
	case 6991: //Revulsion of Death
		return 126;
	case 6992: //Eidolon Voice
		return 126;
	case 6993: //Eidolon Bellow
		return 126;
	case 6994: //Eidolon Howl
		return 126;
	case 6995: //Soulless Fear
		return 126;
	case 6996: //Soulless Panic
		return 126;
	case 6997: //Soulless Terror
		return 126;
	case 6998: //Instinctual Fear
		return 126;
	case 6999: //Instinctual Panic
		return 126;
	case 7000: //Instinctual Terror
		return 126;
	case 7001: //Angstlich's Echo of Terror
		return 126;
	case 7002: //Angstlich's Wail of Panic
		return 126;
	case 7003: //Circle of Dreams
		return 126;
	case 7004: //Guard of Piety
		return 27;
	case 7005: //Ichor Guard
		return 27;
	case 7168: //Obscuring Sporecloud
		return 125;
	case 7169: //Root of Weakness
		return 126;
	case 7170: //Rage of the Root
		return 125;
	case 7171: //Fungal Refreshment
		return 42;
	case 7172: //Spore Snooze
		return 126;
	case 7173: //Fungal Sheen
		return 95;
	case 7177: //Blind Fury I
		return 125;
	case 7178: //Blind Fury II
		return 125;
	case 7179: //Blind Fury III
		return 125;
	case 7180: //Orc Smash I
		return 25;
	case 7181: //Orc Smash II
		return 25;
	case 7182: //Orc Smash III
		return 25;
	case 7183: //Blood Rage I
		return 125;
	case 7184: //Blood Rage II
		return 125;
	case 7185: //Blood Rage III
		return 125;
	case 7186: //Dark Bellow I
		return 126;
	case 7187: //Dark Bellow II
		return 126;
	case 7188: //Dark Bellow III
		return 126;
	case 7189: //Wave of Fire
		return 25;
	case 7190: //Tide of Sloth I
		return 126;
	case 7191: //Tide of Sloth II
		return 126;
	case 7192: //Tide of Sloth III
		return 126;
	case 7193: //Fiery Surge I
		return 25;
	case 7194: //Fiery Surge II
		return 25;
	case 7195: //Fiery Surge III
		return 25;
	case 7199: //Soothing Remedy
		return 42;
	case 7200: //Orcish Regeneration I
		return 125;
	case 7201: //Orcish Regeneration II
		return 125;
	case 7202: //Orcish Regeneration III
		return 125;
	case 7203: //Weak Knees
		return 126;
	case 7204: //Complete Refreshment
		return 42;
	case 7205: //Hand of Darkness
		return 125;
	case 7206: //Shadowmend
		return 42;
	case 7207: //Soulmend
		return 82;
	case 7208: //Arachnae Scream
		return 126;
	case 7209: //Voice of Vule
		return 95;
	case 7210: //Speed of the Spider
		return 125;
	case 7211: //Skinwalker's Mindwave
		return 126;
	case 7212: //Dire Musings
		return 126;
	case 7213: //Thoughtraze
		return 25;
	case 7214: //Dark Messenger
		return 69;
	case 7215: //Bite of Night
		return 20;
	case 7216: //Chanted Doom
		return 20;
	case 7217: //Vile Spirit
		return 114;
	case 7218: //Spiteful Hex
		return 20;
	case 7219: //Eboncall
		return 25;
	case 7220: //Stormreaver
		return 25;
	case 7221: //Ethereal Carapace
		return 125;
	case 7222: //Master's Shadow
		return 125;
	case 7223: //Ice Spray
		return 25;
	case 7224: //Needling Annoyance
		return 125;
	case 7232: //Jaguar Maw
		return 25;
	case 7233: //Jaguar Maw SK
		return 25;
	case 7400: //Heal Wounds I
		return 42;
	case 7401: //Heal Wounds II
		return 42;
	case 7402: //Heal Wounds III
		return 42;
	case 7403: //Heal Wounds IV
		return 42;
	case 7404: //Heal Wounds V
		return 42;
	case 7405: //Heal Wounds VI
		return 42;
	case 7406: //Heal Wounds VII
		return 42;
	case 7407: //Heal Wounds VIII
		return 42;
	case 7408: //Heal Wounds IX
		return 42;
	case 7409: //Heal Wounds X
		return 42;
	case 7410: //Heal Wounds XI
		return 42;
	case 7411: //Heal Wounds XII
		return 42;
	case 7412: //Heal Wounds XIII
		return 42;
	case 7413: //Heal Wounds XIV
		return 42;
	case 7414: //Fire I
		return 25;
	case 7415: //Fire II
		return 25;
	case 7416: //Fire III
		return 25;
	case 7417: //Fire IV
		return 25;
	case 7418: //Fire V
		return 25;
	case 7419: //Fire VI
		return 25;
	case 7420: //Fire VII
		return 25;
	case 7421: //Fire VIII
		return 25;
	case 7422: //Fire IX
		return 25;
	case 7423: //Fire X
		return 25;
	case 7424: //Fire XI
		return 25;
	case 7425: //Fire XII
		return 25;
	case 7426: //Fire XIII
		return 25;
	case 7427: //Fire XIV
		return 25;
	case 7428: //Frost I
		return 25;
	case 7429: //Frost II
		return 25;
	case 7430: //Frost III
		return 25;
	case 7431: //Frost IV
		return 25;
	case 7432: //Frost V
		return 25;
	case 7433: //Frost VI
		return 25;
	case 7434: //Frost VII
		return 25;
	case 7435: //Frost VIII
		return 25;
	case 7436: //Frost IX
		return 25;
	case 7437: //Frost X
		return 25;
	case 7438: //Frost XI
		return 25;
	case 7439: //Frost XII
		return 25;
	case 7440: //Frost XIII
		return 25;
	case 7441: //Frost XIV
		return 25;
	case 7442: //Thunder I
		return 25;
	case 7443: //Thunder II
		return 25;
	case 7444: //Thunder III
		return 25;
	case 7445: //Thunder IV
		return 25;
	case 7446: //Thunder V
		return 25;
	case 7447: //Thunder VI
		return 25;
	case 7448: //Thunder VII
		return 25;
	case 7449: //Thunder VIII
		return 25;
	case 7450: //Thunder IX
		return 25;
	case 7451: //Thunder X
		return 25;
	case 7452: //Thunder XI
		return 25;
	case 7453: //Thunder XII
		return 25;
	case 7454: //Thunder XIII
		return 25;
	case 7455: //Thunder XIV
		return 25;
	case 7465: //Smoke Bomb I
		return 15;
	case 7466: //Smoke Bomb II
		return 15;
	case 7467: //Smoke Bomb III
		return 15;
	case 7468: //Smoke Bomb IV
		return 15;
	case 7469: //Smoke Bomb V
		return 15;
	case 7470: //Smoke Bomb VI
		return 15;
	case 7471: //Smoke Bomb VII
		return 15;
	case 7472: //Smoke Bomb VIII
		return 15;
	case 7473: //Smoke Bomb IX
		return 15;
	case 7474: //Smoke Bomb X
		return 15;
	case 7475: //Smoke Screen
		return 15;
	case 7476: //Pain Tolerance
		return 27;
	case 7477: //Cazic Touch II
		return 25;
	case 7478: //Destroy II
		return 25;
	case 7481: //Hamstring I
		return 126;
	case 7482: //Hamstring II
		return 126;
	case 7483: //Lesion I
		return 20;
	case 7484: //Lesion II
		return 20;
	case 7485: //Lesion III
		return 20;
	case 7486: //Lesion IV
		return 20;
	case 7487: //Lesion V
		return 20;
	case 7488: //Lesion VI
		return 20;
	case 7489: //Lesion VII
		return 20;
	case 7490: //Lesion VIII
		return 20;
	case 7491: //Lesion IX
		return 20;
	case 7492: //Lesion X
		return 20;
	case 7496: //Frost of the Ancients I
		return 20;
	case 7497: //Frost of the Ancients II
		return 20;
	case 7498: //Frost of the Ancients III
		return 20;
	case 7499: //Frost of the Ancients IV
		return 20;
	case 7500: //Frost of the Ancients V
		return 20;
	case 7501: //Frost of the Ancients VI
		return 20;
	case 7502: //Frost of the Ancients VII
		return 20;
	case 7503: //Frost of the Ancients VIII
		return 20;
	case 7504: //Frost of the Ancients IX
		return 20;
	case 7505: //Frost of the Ancients X
		return 20;
	case 7506: //Cure Poison I
		return 42;
	case 7507: //Cure Poison II
		return 42;
	case 7508: //Cure Poison III
		return 42;
	case 7509: //Cure Poison IV
		return 42;
	case 7510: //Cure Disease I
		return 42;
	case 7511: //Cure Disease II
		return 42;
	case 7512: //Cure Disease III
		return 42;
	case 7513: //Cure Disease IV
		return 42;
	case 7514: //Remove Curse I
		return 42;
	case 7515: //Remove Curse II
		return 42;
	case 7516: //Remove Curse III
		return 42;
	case 7517: //Remove Curse IV
		return 42;
	case 7518: //Play Dead I
		return 125;
	case 7519: //Play Dead II
		return 125;
	case 7520: //Play Dead III
		return 125;
	case 7521: //Play Dead IV
		return 125;
	case 7522: //Gore I
		return 25;
	case 7523: //Gore II
		return 25;
	case 7524: //Gore III
		return 25;
	case 7525: //Gore IV
		return 25;
	case 7526: //Gore V
		return 25;
	case 7527: //Gore VI
		return 25;
	case 7528: //War Bellow
		return 126;
	case 7529: //War Bellow Recourse
		return 95;
	case 7531: //Sleep I
		return 126;
	case 7532: //Sleep II
		return 126;
	case 7533: //Sleep III
		return 126;
	case 7534: //Sleep IV
		return 126;
	case 7535: //Sleep V
		return 126;
	case 7536: //Lethargy I
		return 126;
	case 7537: //Lethargy II
		return 126;
	case 7538: //Lethargy III
		return 126;
	case 7539: //Lethargy IV
		return 126;
	case 7540: //Lethargy V
		return 126;
	case 7541: //Plane Shift: Ethereal
		return 27;
	case 7542: //Plane Shift: Material
		return 27;
	case 7543: //Blink
		return 27;
	case 7545: //Swarm of Pain I
		return 20;
	case 7546: //Swarm of Pain II
		return 20;
	case 7547: //Swarm of Pain III
		return 20;
	case 7548: //Swarm of Pain IV
		return 20;
	case 7549: //Swarm of Pain V
		return 20;
	case 7550: //Swarm of Pain VI
		return 20;
	case 7551: //Swarm of Pain VII
		return 20;
	case 7552: //Swarm of Pain VIII
		return 20;
	case 7553: //Swarm of Pain IX
		return 20;
	case 7554: //Swarm of Pain X
		return 20;
	case 7555: //Fungal Malady I
		return 20;
	case 7556: //Fungal Malady II
		return 20;
	case 7557: //Fungal Malady III
		return 20;
	case 7558: //Fungal Malady IV
		return 20;
	case 7559: //Fungal Malady V
		return 20;
	case 7560: //Fungal Malady VI
		return 20;
	case 7561: //Fungal Malady VII
		return 20;
	case 7562: //Fungal Malady VIII
		return 20;
	case 7563: //Fungal Malady IX
		return 20;
	case 7564: //Fungal Malady X
		return 20;
	case 7565: //Ward of the Bear I
		return 45;
	case 7566: //Ward of the Bear II
		return 45;
	case 7567: //Ward of the Bear III
		return 45;
	case 7568: //Ward of the Wolf I
		return 125;
	case 7569: //Ward of the Wolf II
		return 95;
	case 7570: //Ward of the Wolf III
		return 95;
	case 7571: //Ward of the Tiger I
		return 39008;
	case 7572: //Ward of the Tiger II
		return 39008;
	case 7573: //Ward of the Tiger III
		return 95;
	case 7574: //Ward of the Crocodile I
		return 125;
	case 7575: //Ward of the Crocodile II
		return 125;
	case 7576: //Ward of the Crocodile III
		return 125;
	case 7577: //Ward of the Scaled Wolf I
		return 95;
	case 7578: //Ward of the Scaled Wolf II
		return 95;
	case 7579: //Ward of the Scaled Wolf III
		return 95;
	case 7580: //Ward of the Raptor I
		return 125;
	case 7581: //Ward of the Raptor II
		return 95;
	case 7582: //Ward of the Raptor III
		return 95;
	case 7583: //Ward of the Garou I
		return 95;
	case 7584: //Ward of the Garou II
		return 95;
	case 7585: //Ward of the Garou III
		return 95;
	case 7589: //Fire Skin I
		return 125;
	case 7590: //Fire Skin II
		return 125;
	case 7591: //Fire Skin III
		return 125;
	case 7592: //Fire Skin IV
		return 125;
	case 7593: //Fire Skin V
		return 125;
	case 7594: //Fire Skin VI
		return 125;
	case 7595: //Fire Skin VII
		return 125;
	case 7596: //Fire Skin VIII
		return 125;
	case 7597: //Fire Skin IX
		return 125;
	case 7598: //Fire Skin X
		return 125;
	case 7599: //Gargoyle Glance
		return 25;
	case 7701: //Weakening Roots
		return 126;
	case 7729: //Stealth
		return 125;
	case 7745: //Stunning Roar
		return 25;
	case 7746: //Whirlwind
		return 27;
	case 7762: //Maul I
		return 15;
	case 7763: //Maul II
		return 15;
	case 7764: //Maul III
		return 15;
	case 7765: //Maul IV
		return 15;
	case 7766: //Maul V
		return 15;
	case 7767: //Maul VI
		return 15;
	case 7768: //Maul VII
		return 15;
	case 7769: //Maul VIII
		return 15;
	case 7770: //Maul IX
		return 15;
	case 7771: //Maul X
		return 15;
	case 7772: //Maul XI
		return 15;
	case 7773: //Maul XII
		return 15;
	case 7774: //Maul XIII
		return 15;
	case 7775: //Maul XIV
		return 15;
	case 7776: //Mana Bolt I
		return 25;
	case 7777: //Mana Bolt II
		return 25;
	case 7778: //Mana Bolt III
		return 25;
	case 7779: //Mana Bolt IV
		return 25;
	case 7780: //Mana Bolt V
		return 25;
	case 7781: //Mana Bolt VI
		return 25;
	case 7782: //Mana Bolt VII
		return 25;
	case 7783: //Mana Bolt VIII
		return 25;
	case 7784: //Mana Bolt IX
		return 25;
	case 7785: //Mana Bolt X
		return 25;
	case 7786: //Mana Bolt XI
		return 25;
	case 7787: //Mana Bolt XII
		return 25;
	case 7788: //Mana Bolt XIII
		return 25;
	case 7789: //Mana Bolt XIV
		return 25;
	case 7790: //Spirit Sending
		return 123;
	case 7800: //Draygun's Touch
		return 20;
	case 7801: //Draygun's Touch
		return 20;
	case 7802: //Draygun's Touch
		return 20;
	case 7803: //Draygun's Touch
		return 20;
	case 7804: //Draygun's Touch
		return 20;
	case 7805: //Curse of the Nine
		return 20;
	case 7806: //Curse of the Nine
		return 20;
	case 7807: //Curse of the Nine
		return 20;
	case 7808: //Curse of the Nine
		return 20;
	case 7809: //Curse of the Nine
		return 20;
	case 7810: //Blood of the Shadowmane
		return 20;
	case 7811: //Blood of the Shadowmane
		return 20;
	case 7812: //Blood of the Shadowmane
		return 20;
	case 7813: //Blood of the Shadowmane
		return 20;
	case 7814: //Blood of the Shadowmane
		return 20;
	case 7815: //Theft of Rage
		return 20;
	case 7816: //Theft of Rage
		return 20;
	case 7817: //Theft of Rage
		return 20;
	case 7818: //Theft of Rage
		return 20;
	case 7819: //Theft of Rage
		return 20;
	case 7820: //Curse of the Hivequeen
		return 20;
	case 7821: //Curse of the Hivequeen
		return 20;
	case 7822: //Curse of the Hivequeen
		return 20;
	case 7823: //Curse of the Hivequeen
		return 20;
	case 7824: //Curse of the Hivequeen
		return 20;
	case 7838: //Form of Defense IV
		return 125;
	case 7839: //Form of Protection IV
		return 125;
	case 7840: //Form of Endurance IV
		return 125;
	case 7841: //Form of Rejuvenation IV
		return 125;
	case 7994: //Dread Pyre
		return 20;
	case 7995: //Call for Blood
		return 25;
	case 7996: //Call for Blood Recourse
		return 25;
	case 7999: //Corath Venom
		return 20;
	case 8000: //Commanding Voice
		return 95;
	case 8001: //Thief's eyes
		return 95;
	case 8002: //Fists of Wu
		return 95;
	case 8003: //Cry Havoc
		return 95;
	case 8004: //Death's Regret
		return 25;
	case 8005: //Bind Death
		return 25;
	case 8006: //Chromastrike
		return 25;
	case 8007: //Desperate Renewal
		return 42;
	case 8008: //Skin of the Reptile
		return 125;
	case 8009: //Skin of the Rep. Trigger
		return 42;
	case 8010: //Spore Spiral
		return 126;
	case 8011: //Dawnstrike
		return 25;
	case 8012: //Blessing of the Dawn
		return 20;
	case 8015: //Lingering Sloth
		return 125;
	case 8016: //Lingering Sloth Trigger
		return 125;
	case 8017: //Hungry Plague
		return 126;
	case 8018: //Breath of Antraygus
		return 25;
	case 8019: //Warder's Wrath
		return 27;
	case 8020: //Hail of Arrows
		return 25;
	case 8021: //Bestial Empathy
		return 69;
	case 8022: //Fickle Shadows
		return 25;
	case 8023: //Fickle Shadows Recourse
		return 20;
	case 8025: //Touch of Draygun
		return 114;
	case 8026: //Gift of Draygun
		return 45;
	case 8027: //Last Rites
		return 25;
	case 8028: //Last Rites Trigger
		return 25;
	case 8029: //Silent Piety
		return 95;
	case 8030: //Thousand Blades
		return 27;
	case 8031: //Creeping Dreams
		return 126;
	case 8032: //Mana Flare
		return 125;
	case 8033: //Mana Flare Strike
		return 69;
	case 8034: //Colored Chaos
		return 25;
	case 8035: //Echoing Madness
		return 126;
	case 8036: //Illusion: Orc
		return 125;
	case 8037: //Raging Servant
		return 69;
	case 8038: //Burning Aura
		return 125;
	case 8039: //Burning Vengeance
		return 125;
	case 8040: //Fickle Fire
		return 25;
	case 8041: //Clinging Frost
		return 25;
	case 8042: //Clinging Frost Trigger
		return 25;
	case 8043: //Ether Flame
		return 25;
	case 8044: //Mana Weave
		return 25;
	case 8045: //Mana Weave Recourse
		return 25;
	case 8075: //Fickle Fire Recourse
		return 25;
	case 8090: //Armor Cleave I
		return 126;
	case 8091: //Armor Cleave II
		return 126;
	case 8092: //Armor Cleave III
		return 126;
	case 8093: //Armor Cleave IV
		return 126;
	case 8094: //Armor Cleave V
		return 126;
	case 8095: //Armor Cleave VI
		return 126;
	case 8096: //Armor Cleave VII
		return 126;
	case 8097: //Armor Cleave VIII
		return 126;
	case 8098: //Armor Cleave IX
		return 126;
	case 8099: //Armor Cleave X
		return 126;
	case 8106: //Perfected Heal
		return 42;
	case 8114: //Shrieker Sonic Wave
		return 25;
	case 8115: //Shrieker Sonic Wave
		return 25;
	case 8116: //Shrieker Sonic Wave
		return 25;
	case 8117: //Nimbus Shrieker Wave
		return 25;
	case 8118: //Nimbus Shrieker Wave
		return 25;
	case 8119: //Nimbus Shrieker Wave
		return 25;
	case 8120: //Retch Spore
		return 25;
	case 8121: //Retch Spore
		return 25;
	case 8122: //Retch Spore
		return 25;
	case 8123: //Hammer Time
		return 25;
	case 8144: //Net
		return 126;
	case 8145: //Clinging Net
		return 126;
	case 8149: //Stealthy Getaway
		return 123;
	case 8153: //Eternal Thought
		return 79;
	case 8171: //Pure Thought I
		return 79;
	case 8172: //Pure Thought II
		return 79;
	case 8173: //Pure Thought III
		return 79;
	case 8174: //Pure Thought IV
		return 79;
	case 8175: //Pure Thought V
		return 79;
	case 8176: //Pure Thought VI
		return 79;
	case 8177: //Pure Thought VII
		return 79;
	case 8178: //Pure Thought VIII
		return 79;
	case 8179: //Pure Thought IX
		return 79;
	case 8180: //Pure Thought X
		return 79;
	case 8200: //Gift of Illsalin
		return 79;
	case 8201: //Guardian of Ro
		return 69;
	case 8202: //Guardian of Ro
		return 69;
	case 8203: //Guardian of Ro
		return 69;
	case 8204: //Guardian's Bolt I
		return 25;
	case 8210: //Feral Roar I
		return 15;
	case 8211: //Feral Roar II
		return 15;
	case 8212: //Feral Roar III
		return 15;
	case 8213: //Feral Roar IV
		return 15;
	case 8214: //Greater Rabid Bear
		return 125;
	case 8215: //Greater Rabid Bear
		return 125;
	case 8216: //Greater Rabid Bear
		return 125;
	case 8218: //Ancestral Guard
		return 125;
	case 8219: //Ancestral Guard
		return 125;
	case 8220: //Ancestral Guard
		return 125;
	case 8233: //Empathic Fury
		return 27;
	case 8234: //Empathic Fury
		return 27;
	case 8235: //Circle of Undershore
		return 123;
	case 8236: //Undershore Portal
		return 123;
	case 8237: //Ring of Undershore
		return 123;
	case 8238: //Undershore Gate
		return 123;
	case 8239: //Translocate: Undershore
		return 123;
	case 8267: //Feral Roar V
		return 15;
	case 8268: //Feral Roar VI
		return 15;
	case 8275: //Infection Test 1
		return 126;
	case 8276: //Infection Test 2
		return 126;
	case 8277: //Fling
		return 25;
	case 8278: //Fetter of Spirits
		return 69;
	case 8280: //Boon of Vitality I
		return 45;
	case 8281: //Boon of Vitality II
		return 45;
	case 8282: //Boon of Vitality III
		return 45;
	case 8283: //Boon of Vitality IV
		return 45;
	case 8284: //Boon of Vitality V
		return 45;
	case 8285: //Boon of Vitality VI
		return 45;
	case 8286: //Boon of Vitality VII
		return 45;
	case 8287: //Boon of Vitality VIII
		return 45;
	case 8288: //Boon of Vitality IX
		return 45;
	case 8289: //Boon of Vitality X
		return 45;
	case 8290: //Gift of Speed I
		return 125;
	case 8291: //Gift of Speed II
		return 125;
	case 8292: //Gift of Speed III
		return 125;
	case 8293: //Gift of Speed IV
		return 125;
	case 8294: //Gift of Speed V
		return 125;
	case 8295: //Gift of Speed VI
		return 125;
	case 8296: //Gift of Speed VII
		return 125;
	case 8297: //Gift of Speed VIII
		return 125;
	case 8298: //Gift of Speed IX
		return 125;
	case 8299: //Gift of Speed X
		return 125;
	case 8300: //Malaise I
		return 126;
	case 8301: //Malaise II
		return 126;
	case 8302: //Malaise III
		return 126;
	case 8303: //Malaise IV
		return 126;
	case 8304: //Malaise V
		return 126;
	case 8305: //Stun I
		return 25;
	case 8306: //Stun II
		return 25;
	case 8307: //Stun III
		return 25;
	case 8308: //Stun IV
		return 25;
	case 8309: //Stun V
		return 25;
	case 8310: //Gaze of the Beholder I
		return 126;
	case 8311: //Gaze of the Beholder II
		return 126;
	case 8312: //Gaze of the Beholder III
		return 126;
	case 8313: //Gaze of the Beholder IV
		return 126;
	case 8314: //Gaze of the Beholder V
		return 126;
	case 8315: //Gaze of the Beholder VI
		return 126;
	case 8316: //Gaze of the Beholder VII
		return 126;
	case 8317: //Gaze of the Beholder VIII
		return 126;
	case 8318: //Gaze of the Beholder IX
		return 126;
	case 8319: //Gaze of the Beholder X
		return 126;
	case 8320: //Gaze of the Beholder XI
		return 126;
	case 8321: //Gaze of the Beholder XII
		return 126;
	case 8322: //Gaze of the Beholder XIII
		return 126;
	case 8323: //Gaze of the Beholder XIV
		return 126;
	case 8324: //Pure Water I
		return 42;
	case 8325: //Pure Water II
		return 42;
	case 8326: //Pure Water III
		return 42;
	case 8327: //Pure Water IV
		return 42;
	case 8328: //Gale Force
		return 25;
	case 8329: //Fungal Regrowth I
		return 42;
	case 8330: //Fungal Regrowth II
		return 42;
	case 8331: //Fungal Regrowth III
		return 42;
	case 8332: //Fungal Regrowth IV
		return 42;
	case 8333: //Fungal Regrowth V
		return 42;
	case 8334: //Creeping Plague
		return 125;
	case 8335: //Creeping Plague Trigger
		return 125;
	case 8336: //Stunning Blow I
		return 25;
	case 8337: //Stunning Blow II
		return 25;
	case 8338: //Stunning Blow III
		return 25;
	case 8339: //Stunning Blow IV
		return 25;
	case 8340: //Stunning Blow V
		return 25;
	case 8341: //Dark Gift I
		return 125;
	case 8342: //Dark Gift II
		return 125;
	case 8343: //Dark Gift III
		return 125;
	case 8344: //Dark Gift IV
		return 125;
	case 8345: //Dark Gift V
		return 125;
	case 8346: //Dark Gift VI
		return 125;
	case 8347: //Dark Gift VII
		return 125;
	case 8348: //Dark Siphon I
		return 114;
	case 8349: //Dark Siphon II
		return 114;
	case 8350: //Dark Siphon III
		return 114;
	case 8351: //Dark Siphon IV
		return 114;
	case 8352: //Dark Siphon V
		return 114;
	case 8353: //Dark Siphon VI
		return 114;
	case 8354: //Dark Siphon VII
		return 114;
	case 8372: //Stone Skin I
		return 125;
	case 8373: //Stone Skin II
		return 125;
	case 8374: //Stone Skin III
		return 125;
	case 8375: //Stone Skin IV
		return 125;
	case 8376: //Stone Skin V
		return 125;
	case 8377: //Stone Skin VI
		return 125;
	case 8378: //Shadowed Dark Hold
		return 126;
	case 8379: //Shadowed Word of Chaos
		return 126;
	case 8380: //Shadowed Curse of Mori
		return 20;
	case 8381: //Shadowed Meteor Storm
		return 25;
	case 8382: //Shadowed Corona Flare
		return 25;
	case 8383: //Shadowed Core Fire
		return 25;
	case 8400: //Guardian's Bolt II
		return 25;
	case 8401: //Guardian's Bolt III
		return 25;
	case 8410: //Hand of Holy Vengeance I
		return 25;
	case 8411: //Hand of Holy Vengeance II
		return 25;
	case 8412: //Hand of Holy Vengeance III
		return 25;
	case 8413: //Hand of Holy Vengeance IV
		return 25;
	case 8414: //Hand of Holy Vengeance V
		return 25;
	case 8421: //Jailor's Fury
		return 25;
	case 8444: //Blinding Dust
		return 15;
	default:
		return 0;
	}
}
static inline DWORD GetSpellSubCategory(DWORD spellID)
{
	switch (spellID)
	{
	case 3: //Summon Corpse
		return 64;
	case 4: //Summon Waterstone
		return 109;
	case 6: //Ignite Blood
		return 38;
	case 7: //Hymn of Restoration
		return 43;
	case 9: //Superior Healing
		return 42;
	case 10: //Augmentation
		return 41;
	case 11: //Holy Armor
		return 6;
	case 12: //Healing
		return 42;
	case 13: //Complete Healing
		return 42;
	case 14: //Strike
		return 58;
	case 15: //Greater Healing
		return 42;
	case 16: //Smite
		return 58;
	case 17: //Light Healing
		return 42;
	case 18: //Guard
		return 6;
	case 19: //Armor of Faith
		return 6;
	case 20: //Shield of Words
		return 6;
	case 21: //Berserker Strength
		return 96;
	case 22: //Force Snap
		return 58;
	case 23: //Force Strike
		return 58;
	case 24: //Strip Enchantment
		return 31;
	case 25: //Pillage Enchantment
		return 31;
	case 26: //Skin like Wood
		return 46;
	case 27: //Pogonip
		return 14;
	case 28: //Avalanche
		return 14;
	case 29: //Ice
		return 14;
	case 31: //Scourge
		return 29;
	case 32: //Plague
		return 29;
	case 33: //Brilliance
		return 130;
	case 34: //Superior Camouflage
		return 51;
	case 35: //Bind Affinity
		return 64;
	case 36: //Gate
		return 64;
	case 37: //Hammer of Striking
		return 110;
	case 38: //Lightning Bolt
		return 58;
	case 39: //Quickness
		return 41;
	case 40: //Strengthen
		return 96;
	case 41: //Weaken
		return 30;
	case 42: //Invisibility
		return 51;
	case 43: //Yaulp II
		return 7;
	case 44: //Yaulp III
		return 7;
	case 45: //Pacify
		return 11;
	case 46: //Ultravision
		return 129;
	case 47: //Calm
		return 11;
	case 48: //Cancel Magic
		return 31;
	case 49: //Nullify Magic
		return 31;
	case 50: //Summon Food
		return 108;
	case 51: //Glimpse
		return 129;
	case 52: //Abundant Drink
		return 108;
	case 53: //Abundant Food
		return 108;
	case 54: //Frost Bolt
		return 14;
	case 55: //Cornucopia
		return 108;
	case 56: //Everfount
		return 108;
	case 57: //Firestrike
		return 38;
	case 58: //Elementalkin: Earth
		return 100;
	case 59: //Panic the Dead
		return 37;
	case 60: //Resist Fire
		return 80;
	case 61: //Resist Cold
		return 80;
	case 62: //Resist Poison
		return 80;
	case 63: //Resist Disease
		return 80;
	case 64: //Resist Magic
		return 80;
	case 65: //Major Shielding
		return 87;
	case 66: //Greater Shielding
		return 87;
	case 67: //Arch Shielding
		return 87;
	case 68: //Bolt of Flame
		return 38;
	case 69: //Cinder Bolt
		return 38;
	case 70: //Lava Bolt
		return 38;
	case 71: //Anarchy
		return 58;
	case 72: //Group Resist Magic
		return 80;
	case 73: //Gravity Flux
		return 58;
	case 74: //Mana Sieve
		return 60;
	case 75: //Sicken
		return 29;
	case 76: //Ensnaring Roots
		return 83;
	case 77: //Engulfing Roots
		return 83;
	case 78: //Immolate
		return 38;
	case 79: //Spirit Sight
		return 129;
	case 80: //See Invisible
		return 129;
	case 81: //Phantom Chain
		return 43;
	case 82: //Phantom Plate
		return 43;
	case 83: //Rain of Fire
		return 38;
	case 84: //Shifting Sight
		return 129;
	case 85: //Firestorm
		return 38;
	case 86: //Enduring Breath
		return 64;
	case 88: //Harm Touch
		return 58;
	case 89: //Daring
		return 46;
	case 90: //Shadow Sight
		return 129;
	case 91: //Ignite
		return 38;
	case 92: //Burst of Fire
		return 38;
	case 93: //Burst of Flame
		return 38;
	case 94: //Burn
		return 38;
	case 95: //Counteract Poison
		return 19;
	case 96: //Counteract Disease
		return 19;
	case 97: //Abolish Poison
		return 19;
	case 98: //Abolish Disease
		return 19;
	case 99: //Creeping Crud
		return 58;
	case 100: //Summon Throwing Dagger
		return 110;
	case 101: //Summon Arrows
		return 110;
	case 102: //Spear of Warding
		return 110;
	case 103: //Summon Coldstone
		return 109;
	case 104: //Dagger of Symbols
		return 110;
	case 105: //Summon Ring of Flight
		return 109;
	case 106: //Burnout II
		return 70;
	case 107: //Burnout III
		return 70;
	case 108: //Elemental Shield
		return 80;
	case 109: //Elemental Armor
		return 80;
	case 110: //Malaise
		return 81;
	case 111: //Malaisement
		return 81;
	case 112: //Malosi
		return 81;
	case 113: //Shock of Spikes
		return 58;
	case 114: //Shock of Swords
		return 58;
	case 115: //Dismiss Summoned
		return 111;
	case 116: //Banish Summoned
		return 111;
	case 117: //Dismiss Undead
		return 124;
	case 118: //Banish Undead
		return 124;
	case 120: //Blaze
		return 38;
	case 121: //Rain of Lava
		return 38;
	case 122: //Flame Arc
		return 38;
	case 123: //Holy Might
		return 97;
	case 124: //Force
		return 97;
	case 125: //Sound of Force
		return 97;
	case 126: //Inspire Fear
		return 37;
	case 127: //Invoke Fear
		return 37;
	case 128: //Wave of Fear
		return 37;
	case 129: //Shield of Brambles
		return 21;
	case 130: //Divine Barrier
		return 52;
	case 131: //Instill
		return 83;
	case 132: //Immobilize
		return 83;
	case 133: //Paralyzing Earth
		return 83;
	case 134: //Blinding Luminance
		return 9;
	case 135: //Word of Health
		return 42;
	case 136: //Word of Healing
		return 42;
	case 137: //Pack Regeneration
		return 43;
	case 138: //Pack Chloroplast
		return 43;
	case 139: //Feral Spirit
		return 70;
	case 140: //Savage Spirit
		return 70;
	case 141: //Beguile Animals
		return 13;
	case 142: //Allure of the Wild
		return 13;
	case 143: //Sunbeam
		return 9;
	case 144: //Regeneration
		return 43;
	case 145: //Chloroplast
		return 43;
	case 146: //Spirit of Monkey
		return 24;
	case 147: //Spirit Strength
		return 96;
	case 148: //Spirit of Cat
		return 2;
	case 149: //Spirit of Ox
		return 94;
	case 150: //Alluring Aura
		return 12;
	case 151: //Raging Strength
		return 96;
	case 152: //Deftness
		return 24;
	case 153: //Furious Strength
		return 96;
	case 154: //Agility
		return 2;
	case 155: //Glamour
		return 12;
	case 156: //Charisma
		return 12;
	case 157: //Dexterity
		return 24;
	case 158: //Stamina
		return 94;
	case 159: //Strength
		return 96;
	case 160: //Nimble
		return 2;
	case 161: //Health
		return 94;
	case 162: //Listless Power
		return 30;
	case 163: //Incapacitate
		return 30;
	case 164: //Companion Spirit
		return 104;
	case 165: //Guardian Spirit
		return 104;
	case 166: //Frenzied Spirit
		return 104;
	case 167: //Talisman of Tnarg
		return 87;
	case 168: //Talisman of Altuna
		return 87;
	case 169: //Pack Spirit
		return 65;
	case 170: //Alacrity
		return 41;
	case 171: //Celerity
		return 41;
	case 172: //Swift like the Wind
		return 41;
	case 173: //Benevolence
		return 3;
	case 174: //Clarity
		return 59;
	case 175: //Insight
		return 130;
	case 176: //Berserker Spirit
		return 84;
	case 177: //Color Shift
		return 97;
	case 178: //Color Skew
		return 97;
	case 179: //Feckless Might
		return 30;
	case 180: //Insipid Weakness
		return 30;
	case 181: //Weakness
		return 30;
	case 182: //Beguile
		return 13;
	case 183: //Cajoling Whispers
		return 13;
	case 184: //Allure
		return 13;
	case 185: //Tepid Deeds
		return 88;
	case 186: //Shiftless Deeds
		return 88;
	case 187: //Enthrall
		return 35;
	case 188: //Entrance
		return 35;
	case 189: //Flame Flux
		return 38;
	case 190: //Dazzle
		return 35;
	case 191: //Feedback
		return 21;
	case 192: //Mind Wipe
		return 63;
	case 193: //Blanket of Forgetfulness
		return 63;
	case 194: //Reoccurring Amnesia
		return 63;
	case 195: //Gasping Embrace
		return 58;
	case 196: //Dominate Undead
		return 13;
	case 197: //Beguile Undead
		return 13;
	case 198: //Cajole Undead
		return 13;
	case 199: //Harmshield
		return 52;
	case 200: //Minor Healing
		return 42;
	case 201: //Flash of Light
		return 9;
	case 202: //Courage
		return 46;
	case 203: //Cure Poison
		return 19;
	case 204: //Shock of Poison
		return 75;
	case 205: //True North
		return 64;
	case 207: //Divine Aura
		return 52;
	case 208: //Lull
		return 11;
	case 209: //Spook the Dead
		return 37;
	case 210: //Yaulp
		return 7;
	case 211: //Summon Drink
		return 108;
	case 212: //Cure Blindness
		return 19;
	case 213: //Cure Disease
		return 19;
	case 215: //Reckless Strength
		return 96;
	case 216: //Stun
		return 97;
	case 217: //Combust
		return 38;
	case 218: //Ward Undead
		return 124;
	case 219: //Center
		return 46;
	case 220: //Spirit of Cheetah
		return 65;
	case 221: //Sense the Dead
		return 64;
	case 222: //Invigor
		return 94;
	case 223: //Hammer of Wrath
		return 110;
	case 224: //Endure Fire
		return 80;
	case 225: //Endure Cold
		return 80;
	case 226: //Endure Disease
		return 80;
	case 227: //Endure Poison
		return 80;
	case 228: //Endure Magic
		return 80;
	case 229: //Fear
		return 37;
	case 230: //Root
		return 83;
	case 231: //Word of Pain
		return 58;
	case 232: //Sense Summoned
		return 111;
	case 233: //Expulse Undead
		return 124;
	case 234: //Halo of Light
		return 109;
	case 235: //Invisibility versus Undead
		return 51;
	case 236: //Shieldskin
		return 84;
	case 237: //Dance of the Fireflies
		return 109;
	case 238: //Sense Animals
		return 4;
	case 239: //Flame Lick
		return 38;
	case 240: //Lull Animal
		return 11;
	case 241: //Panic Animal
		return 37;
	case 242: //Snare
		return 89;
	case 243: //Illusion: Iksar
		return 49;
	case 244: //Bravery
		return 46;
	case 245: //Befriend Animal
		return 13;
	case 246: //Lesser Shielding
		return 87;
	case 247: //Camouflage
		return 51;
	case 248: //Ward Summoned
		return 111;
	case 249: //Grasping Roots
		return 83;
	case 250: //Harmony
		return 11;
	case 252: //Invoke Lightning
		return 58;
	case 253: //Whirling Wind
		return 58;
	case 254: //Firefist
		return 7;
	case 255: //Invisibility versus Animals
		return 51;
	case 256: //Shield of Thistles
		return 21;
	case 257: //Starshine
		return 109;
	case 258: //Treeform
		return 48;
	case 259: //Drones of Doom
		return 58;
	case 260: //Charm Animals
		return 13;
	case 261: //Levitate
		return 55;
	case 262: //Cascade of Hail
		return 14;
	case 263: //Skin like Rock
		return 46;
	case 264: //Stinging Swarm
		return 58;
	case 265: //Cannibalize
		return 17;
	case 266: //Dexterous Aura
		return 24;
	case 267: //Inner Fire
		return 46;
	case 268: //Strength of Earth
		return 96;
	case 269: //Feet like Cat
		return 2;
	case 270: //Drowsy
		return 88;
	case 271: //Fleeting Fury
		return 96;
	case 272: //Spirit Pouch
		return 109;
	case 273: //Shield of Barbs
		return 21;
	case 274: //Scale Skin
		return 6;
	case 275: //Frost Rift
		return 14;
	case 276: //Serpent Sight
		return 129;
	case 277: //Tainted Breath
		return 75;
	case 278: //Spirit of Wolf
		return 65;
	case 279: //Spirit of Bear
		return 94;
	case 280: //Burst of Strength
		return 96;
	case 281: //Disempower
		return 30;
	case 282: //Spirit Strike
		return 14;
	case 283: //Turtle Skin
		return 6;
	case 284: //Spirit of Snake
		return 12;
	case 285: //Pendril's Animation
		return 99;
	case 286: //Shallow Breath
		return 58;
	case 287: //Minor Illusion
		return 48;
	case 288: //Minor Shielding
		return 87;
	case 289: //Taper Enchantment
		return 31;
	case 290: //Color Flux
		return 97;
	case 291: //Enfeeblement
		return 30;
	case 292: //Mesmerize
		return 35;
	case 293: //Haze
		return 6;
	case 294: //Suffocating Sphere
		return 58;
	case 295: //Mircyl's Animation
		return 99;
	case 296: //Chaotic Feedback
		return 58;
	case 297: //Eye of Confusion
		return 9;
	case 298: //Alliance
		return 3;
	case 299: //Sentinel
		return 64;
	case 300: //Charm
		return 13;
	case 301: //Memory Blur
		return 63;
	case 302: //Languid Pace
		return 88;
	case 303: //Whirl till you hurl
		return 97;
	case 304: //Chase the Moon
		return 37;
	case 305: //Identify
		return 64;
	case 306: //Sanity Warp
		return 58;
	case 307: //Mesmerization
		return 35;
	case 308: //Frenzy
		return 96;
	case 309: //Shielding
		return 87;
	case 310: //Flare
		return 64;
	case 311: //Summon Dagger
		return 110;
	case 312: //Valor
		return 46;
	case 313: //Fire Flux
		return 38;
	case 314: //Resolution
		return 46;
	case 315: //Elementalkin: Water
		return 105;
	case 316: //Elementalkin: Fire
		return 102;
	case 317: //Elementalkin: Air
		return 98;
	case 318: //Summon Bandages
		return 109;
	case 319: //Summon Fang
		return 110;
	case 320: //Summon Heatstone
		return 109;
	case 321: //Summon Wisp
		return 109;
	case 322: //Flame Bolt
		return 38;
	case 323: //Eye of Zomm
		return 129;
	case 324: //Shock of Blades
		return 58;
	case 325: //Dimensional Pocket
		return 109;
	case 326: //Fury
		return 96;
	case 327: //Burnout
		return 70;
	case 328: //Column of Fire
		return 38;
	case 329: //Wrath
		return 58;
	case 330: //Rain of Blades
		return 58;
	case 331: //Reclaim Energy
		return 64;
	case 332: //Shield of Fire
		return 21;
	case 333: //Phantom Leather
		return 43;
	case 334: //Shock of Flame
		return 38;
	case 335: //Minor Summoning: Earth
		return 100;
	case 336: //Minor Summoning: Water
		return 105;
	case 337: //Rage
		return 96;
	case 338: //Cavorting Bones
		return 103;
	case 339: //Coldlight
		return 109;
	case 340: //Disease Cloud
		return 29;
	case 341: //Lifetap
		return 43;
	case 342: //Locate Corpse
		return 64;
	case 343: //Siphon Strength
		return 76;
	case 344: //Clinging Darkness
		return 58;
	case 345: //Shrink
		return 64;
	case 346: //Grim Aura
		return 7;
	case 347: //Numb the Dead
		return 11;
	case 348: //Poison Bolt
		return 75;
	case 349: //Rising Dexterity
		return 24;
	case 350: //Chaos Flux
		return 58;
	case 351: //Bone Walk
		return 103;
	case 352: //Deadeye
		return 129;
	case 353: //Mend Bones
		return 42;
	case 354: //Shadow Step
		return 86;
	case 355: //Engulfing Darkness
		return 58;
	case 356: //Shield of Thorns
		return 21;
	case 357: //Dark Empathy
		return 56;
	case 358: //Impart Strength
		return 96;
	case 359: //Vampiric Embrace
		return 16;
	case 360: //Heat Blood
		return 38;
	case 361: //Sight Graft
		return 129;
	case 362: //Convoke Shadow
		return 103;
	case 363: //Wave of Enfeeblement
		return 30;
	case 364: //Banshee Aura
		return 21;
	case 365: //Infectious Cloud
		return 29;
	case 366: //Feign Death
		return 64;
	case 367: //Heart Flutter
		return 29;
	case 368: //Spirit Armor
		return 6;
	case 369: //Hungry Earth
		return 83;
	case 370: //Shadow Vortex
		return 30;
	case 371: //Voice Graft
		return 64;
	case 372: //Blast of Cold
		return 14;
	case 373: //Sphere of Light
		return 109;
	case 374: //Numbing Cold
		return 14;
	case 375: //Fade
		return 86;
	case 376: //Shock of Fire
		return 38;
	case 377: //Icestrike
		return 14;
	case 378: //O'Keils Radiation
		return 21;
	case 379: //Fingers of Fire
		return 38;
	case 380: //Column of Frost
		return 14;
	case 381: //Resistant Skin
		return 80;
	case 382: //Frost Spiral of Al'Kabor
		return 14;
	case 383: //Shock of Lightning
		return 58;
	case 384: //Assiduous Vision
		return 129;
	case 385: //Project Lightning
		return 58;
	case 386: //Pillar of Fire
		return 38;
	case 387: //Leatherskin
		return 84;
	case 388: //Resuscitate
		return 82;
	case 389: //Guardian
		return 6;
	case 390: //Thicken Mana
		return 64;
	case 391: //Revive
		return 82;
	case 392: //Resurrection
		return 82;
	case 393: //Steelskin
		return 84;
	case 394: //Diamondskin
		return 84;
	case 395: //Minor Summoning: Fire
		return 102;
	case 396: //Minor Summoning: Air
		return 98;
	case 397: //Elementaling: Earth
		return 100;
	case 398: //Elementaling: Water
		return 105;
	case 399: //Elementaling: Fire
		return 102;
	case 400: //Elementaling: Air
		return 98;
	case 401: //Elemental: Earth
		return 100;
	case 402: //Elemental: Water
		return 105;
	case 403: //Elemental: Fire
		return 102;
	case 404: //Elemental: Air
		return 98;
	case 405: //Tremor
		return 58;
	case 406: //Earthquake
		return 58;
	case 407: //Cast Sight
		return 129;
	case 408: //Curse of the Simple Mind
		return 30;
	case 409: //Rain of Spikes
		return 58;
	case 410: //Rain of Swords
		return 58;
	case 411: //Shield of Flame
		return 21;
	case 412: //Shield of Lava
		return 21;
	case 413: //Word of Shadow
		return 58;
	case 414: //Word of Spirit
		return 58;
	case 415: //Word of Souls
		return 58;
	case 416: //Word Divine
		return 58;
	case 417: //Extinguish Fatigue
		return 94;
	case 418: //Lightning Strike
		return 58;
	case 419: //Careless Lightning
		return 58;
	case 420: //Lightning Blast
		return 58;
	case 421: //Skin like Steel
		return 46;
	case 422: //Skin like Diamond
		return 46;
	case 423: //Skin like Nature
		return 46;
	case 424: //Scale of Wolf
		return 65;
	case 425: //Wolf Form
		return 65;
	case 426: //Greater Wolf Form
		return 65;
	case 427: //Form of the Great Wolf
		return 65;
	case 428: //Share Wolf Form
		return 65;
	case 429: //Strength of Stone
		return 96;
	case 430: //Storm Strength
		return 96;
	case 431: //Shifting Shield
		return 6;
	case 432: //Shield of Spikes
		return 21;
	case 433: //Fire
		return 38;
	case 434: //Envenomed Breath
		return 75;
	case 435: //Venom of the Snake
		return 75;
	case 436: //Envenomed Bolt
		return 75;
	case 437: //Poison Storm
		return 75;
	case 438: //Gale of Poison
		return 75;
	case 439: //Crystallize Mana
		return 64;
	case 440: //Animate Dead
		return 103;
	case 441: //Summon Dead
		return 103;
	case 442: //Malignant Dead
		return 103;
	case 443: //Invoke Death
		return 103;
	case 444: //Renew Bones
		return 42;
	case 445: //Lifedraw
		return 43;
	case 446: //Siphon Life
		return 43;
	case 447: //Drain Soul
		return 43;
	case 448: //Rest the Dead
		return 11;
	case 449: //Intensify Death
		return 70;
	case 450: //Suffocate
		return 58;
	case 451: //Boil Blood
		return 38;
	case 452: //Dooming Darkness
		return 58;
	case 453: //Cascading Darkness
		return 58;
	case 454: //Vampiric Curse
		return 33;
	case 455: //Surge of Enfeeblement
		return 30;
	case 456: //Bond of Death
		return 33;
	case 457: //Dead Man Floating
		return 55;
	case 458: //Fire Spiral of Al'Kabor
		return 38;
	case 459: //Shock Spiral of Al'Kabor
		return 58;
	case 460: //Force Spiral of Al'Kabor
		return 58;
	case 461: //Cast Force
		return 58;
	case 462: //Column of Lightning
		return 38;
	case 463: //Circle of Force
		return 38;
	case 464: //Frost Shock
		return 14;
	case 465: //Inferno Shock
		return 38;
	case 466: //Lightning Shock
		return 58;
	case 467: //Lightning Storm
		return 58;
	case 468: //Energy Storm
		return 58;
	case 469: //Lava Storm
		return 38;
	case 470: //Thunder Strike
		return 58;
	case 471: //Thunderclap
		return 58;
	case 472: //Inspire Fear2
		return 37;
	case 473: //Invoke Fear II
		return 37;
	case 474: //Radius of Fear2
		return 37;
	case 475: //Fear2
		return 37;
	case 477: //Fire Bolt
		return 38;
	case 478: //Breath of the Dead
		return 64;
	case 479: //Inferno Shield
		return 21;
	case 480: //Atone
		return 63;
	case 481: //Rune I
		return 84;
	case 482: //Rune II
		return 84;
	case 483: //Rune III
		return 84;
	case 484: //Rune IV
		return 84;
	case 485: //Symbol of Transal
		return 112;
	case 486: //Symbol of Ryltan
		return 112;
	case 487: //Symbol of Pinzarn
		return 112;
	case 488: //Symbol of Naltron
		return 112;
	case 489: //Sympathetic Aura
		return 12;
	case 490: //Enveloping Roots
		return 83;
	case 491: //Leering Corpse
		return 103;
	case 492: //Restless Bones
		return 103;
	case 493: //Haunting Corpse
		return 103;
	case 494: //Invoke Shadow
		return 103;
	case 495: //Cackling Bones
		return 103;
	case 496: //Lesser Summoning: Earth
		return 100;
	case 497: //Lesser Summoning: Water
		return 105;
	case 498: //Lesser Summoning: Fire
		return 102;
	case 499: //Lesser Summoning: Air
		return 98;
	case 500: //Bind Sight
		return 129;
	case 501: //Soothe
		return 11;
	case 502: //Lifespike
		return 43;
	case 503: //Tishan's Clash
		return 97;
	case 504: //Frenzied Strength
		return 96;
	case 505: //Walking Sleep
		return 88;
	case 506: //Tagar's Insects
		return 88;
	case 507: //Togor's Insects
		return 88;
	case 508: //Frost Strike
		return 14;
	case 509: //Winter's Roar
		return 14;
	case 510: //Blizzard Blast
		return 14;
	case 511: //Affliction
		return 29;
	case 512: //Ensnare
		return 89;
	case 513: //Calm Animal
		return 11;
	case 514: //Terrorize Animal
		return 37;
	case 515: //Thistlecoat
		return 21;
	case 516: //Barbcoat
		return 21;
	case 517: //Bramblecoat
		return 21;
	case 518: //Spikecoat
		return 21;
	case 519: //Thorncoat
		return 21;
	case 520: //Dizzying Wind
		return 58;
	case 521: //Choke
		return 58;
	case 522: //Gather Shadows
		return 51;
	case 524: //Spirit Tap
		return 43;
	case 525: //Drain Spirit
		return 43;
	case 526: //Insidious Fever
		return 81;
	case 527: //Insidious Malady
		return 81;
	case 528: //Yonder
		return 86;
	case 529: //Gaze
		return 129;
	case 530: //Ring of Karana
		return 5;
	case 531: //Ring of Commons
		return 5;
	case 532: //Ring of Butcher
		return 36;
	case 533: //Ring of Toxxulia
		return 67;
	case 534: //Ring of Lavastorm
		return 5;
	case 535: //Ring of Ro
		return 5;
	case 536: //Ring of Feerrott
		return 5;
	case 537: //Ring of Steamfont
		return 36;
	case 538: //Ring of Misty
		return 5;
	case 539: //Chill Sight
		return 129;
	case 540: //Clarify Mana
		return 64;
	case 541: //Tox Gate
		return 67;
	case 542: //North Gate
		return 5;
	case 543: //Fay Gate
		return 36;
	case 544: //Common Gate
		return 5;
	case 545: //Nek Gate
		return 5;
	case 546: //Cazic Gate
		return 5;
	case 547: //Ro Gate
		return 5;
	case 548: //West Gate
		return 5;
	case 549: //Screaming Terror
		return 35;
	case 550: //Circle of Karana
		return 5;
	case 551: //Circle of Commons
		return 5;
	case 552: //Circle of Toxxulia
		return 67;
	case 553: //Circle of Butcher
		return 36;
	case 554: //Circle of Lavastorm
		return 5;
	case 555: //Circle of Ro
		return 5;
	case 556: //Circle of Feerrott
		return 5;
	case 557: //Circle of Steamfont
		return 36;
	case 558: //Circle of Misty
		return 5;
	case 559: //Ignite Bones
		return 38;
	case 560: //Furor
		return 58;
	case 561: //Tox Portal
		return 67;
	case 562: //North Portal
		return 5;
	case 563: //Fay Portal
		return 36;
	case 564: //Nek Portal
		return 5;
	case 565: //Cazic Portal
		return 5;
	case 566: //Common Portal
		return 5;
	case 567: //Ro Portal
		return 5;
	case 568: //West Portal
		return 5;
	case 569: //Summoning: Earth
		return 100;
	case 570: //Summoning: Water
		return 105;
	case 571: //Summoning: Fire
		return 102;
	case 572: //Summoning: Air
		return 98;
	case 573: //Greater Summoning: Earth
		return 100;
	case 574: //Greater Summoning: Water
		return 105;
	case 575: //Greater Summoning: Fire
		return 102;
	case 576: //Greater Summoning: Air
		return 98;
	case 577: //Vigilant Spirit
		return 104;
	case 578: //Sight
		return 129;
	case 579: //Magnify
		return 129;
	case 580: //Vision
		return 129;
	case 581: //Illusion: Skeleton
		return 48;
	case 582: //Illusion: Human
		return 49;
	case 583: //Illusion: Half-Elf
		return 49;
	case 584: //Illusion: Earth Elemental
		return 48;
	case 585: //Illusion: Werewolf
		return 48;
	case 586: //Illusion: Barbarian
		return 49;
	case 587: //Illusion: Erudite
		return 49;
	case 588: //Illusion: Wood Elf
		return 49;
	case 589: //Illusion: High Elf
		return 49;
	case 590: //Illusion: Dark Elf
		return 49;
	case 591: //Illusion: Dwarf
		return 49;
	case 592: //Illusion: Troll
		return 49;
	case 593: //Illusion: Ogre
		return 49;
	case 594: //Illusion: Halfling
		return 49;
	case 595: //Illusion: Gnome
		return 49;
	case 596: //Illusion: Dry Bone
		return 48;
	case 597: //Illusion: Air Elemental
		return 48;
	case 598: //Illusion: Fire Elemental
		return 48;
	case 599: //Illusion: Water Elemental
		return 48;
	case 600: //Illusion: Spirit Wolf
		return 48;
	case 601: //Illusion: Tree
		return 48;
	case 602: //Evacuate: North
		return 5;
	case 603: //Evacuate: Fay
		return 36;
	case 604: //Evacuate: Ro
		return 5;
	case 605: //Evacuate: Nek
		return 5;
	case 606: //Evacuate: West
		return 5;
	case 607: //Succor: East
		return 5;
	case 608: //Succor: Butcher
		return 36;
	case 609: //Succor: Ro
		return 5;
	case 610: //Succor: Lavastorm
		return 5;
	case 611: //Succor: North
		return 5;
	case 612: //Markar's Clash
		return 97;
	case 613: //Staff of Tracing
		return 110;
	case 614: //Staff of Warding
		return 110;
	case 615: //Staff of Runes
		return 110;
	case 616: //Staff of Symbols
		return 110;
	case 617: //Sword of Runes
		return 110;
	case 618: //Dimensional Hole
		return 109;
	case 619: //Dyn`s Dizzying Draught
		return 97;
	case 620: //Minor Conjuration: Earth
		return 100;
	case 621: //Minor Conjuration: Water
		return 105;
	case 622: //Minor Conjuration: Fire
		return 102;
	case 623: //Minor Conjuration: Air
		return 98;
	case 624: //Lesser Conjuration: Earth
		return 100;
	case 625: //Lesser Conjuration: Water
		return 105;
	case 626: //Lesser Conjuration: Fire
		return 102;
	case 627: //Lesser Conjuration: Air
		return 98;
	case 628: //Conjuration: Earth
		return 100;
	case 629: //Conjuration: Water
		return 105;
	case 630: //Conjuration: Fire
		return 102;
	case 631: //Conjuration: Air
		return 98;
	case 632: //Greater Conjuration: Earth
		return 100;
	case 633: //Greater Conjuration: Water
		return 105;
	case 634: //Greater Conjuration: Fire
		return 102;
	case 635: //Greater Conjuration: Air
		return 98;
	case 636: //Bonds of Force
		return 89;
	case 640: //Creeping Vision
		return 129;
	case 641: //Dark Pact
		return 17;
	case 642: //Allure of Death
		return 17;
	case 643: //Call of Bones
		return 17;
	case 644: //Lich
		return 17;
	case 645: //Ebbing Strength
		return 30;
	case 646: //Radiant Visage
		return 12;
	case 647: //Adorning Grace
		return 12;
	case 648: //Rampage
		return 84;
	case 649: //Protect
		return 6;
	case 650: //Mist
		return 6;
	case 651: //Cloud
		return 6;
	case 652: //Obscure
		return 6;
	case 653: //Shade
		return 6;
	case 654: //Shadow
		return 6;
	case 655: //Eyes of the Cat
		return 129;
	case 656: //Shock of Ice
		return 14;
	case 657: //Flame Shock
		return 38;
	case 658: //Ice Shock
		return 14;
	case 659: //Conflagration
		return 38;
	case 660: //Frost Storm
		return 14;
	case 661: //Augment Death
		return 70;
	case 662: //Expel Undead
		return 124;
	case 663: //Expulse Summoned
		return 111;
	case 664: //Expel Summoned
		return 111;
	case 665: //Drifting Death
		return 58;
	case 666: //Alter Plane: Hate
		return 116;
	case 667: //Enchant Silver
		return 34;
	case 668: //Enchant Electrum
		return 34;
	case 669: //Enchant Gold
		return 34;
	case 670: //Enchant Platinum
		return 34;
	case 671: //Starfire
		return 38;
	case 672: //Retribution
		return 58;
	case 673: //Discordant Mind
		return 58;
	case 674: //Alter Plane: Sky
		return 116;
	case 675: //Hammer of Requital
		return 110;
	case 676: //Tashan
		return 81;
	case 677: //Tashani
		return 81;
	case 678: //Tashania
		return 81;
	case 679: //Heat Sight
		return 129;
	case 680: //Barrier of Combustion
		return 21;
	case 681: //Juli`s Animation
		return 99;
	case 682: //Kilan`s Animation
		return 99;
	case 683: //Shalee`s Animation
		return 99;
	case 684: //Sisna`s Animation
		return 99;
	case 685: //Sagar`s Animation
		return 99;
	case 686: //Uleen`s Animation
		return 99;
	case 687: //Boltran`s Animation
		return 99;
	case 688: //Aanya's Animation
		return 99;
	case 689: //Yegoreff`s Animation
		return 99;
	case 690: //Kintaz`s Animation
		return 99;
	case 691: //Call of Flame
		return 38;
	case 692: //Life Leech
		return 43;
	case 693: //Divine Might
		return 16;
	case 694: //Pact of Shadow
		return 56;
	case 695: //Distill Mana
		return 64;
	case 696: //Purify Mana
		return 64;
	case 697: //Breeze
		return 59;
	case 698: //Track Corpse
		return 64;
	case 699: //Defoliate
		return 74;
	case 700: //Chant of Battle
		return 41;
	case 701: //Anthem de Arms
		return 41;
	case 702: //McVaxius` Berserker Crescendo
		return 41;
	case 703: //Chords of Dissonance
		return 58;
	case 704: //Brusco`s Boastful Bellow
		return 58;
	case 705: //Largo's Melodic Binding
		return 88;
	case 706: //Angstlich`s Appalling Screech
		return 37;
	case 707: //Fufil`s Curtailing Chant
		return 58;
	case 708: //Cinda`s Charismatic Carillon
		return 3;
	case 709: //Guardian Rhythms
		return 80;
	case 710: //Elemental Rhythms
		return 80;
	case 711: //Purifying Rhythms
		return 80;
	case 712: //Psalm of Warmth
		return 21;
	case 713: //Psalm of Cooling
		return 21;
	case 714: //Psalm of Mystic Shielding
		return 80;
	case 715: //Psalm of Vitality
		return 21;
	case 716: //Psalm of Purity
		return 21;
	case 717: //Selo`s Accelerando
		return 65;
	case 718: //Agilmente`s Aria of Eagles
		return 55;
	case 719: //Shauri`s Sonorous Clouding
		return 51;
	case 720: //Lyssa`s Locating Lyric
		return 64;
	case 721: //Lyssa`s Solidarity of Vision
		return 129;
	case 722: //Jaxan`s Jig o` Vigor
		return 94;
	case 723: //Cassindra's Chorus of Clarity
		return 59;
	case 724: //Kelin`s Lucid Lullaby
		return 35;
	case 725: //Solon's Song of the Sirens
		return 13;
	case 726: //Syvelian`s Anti-Magic Aria
		return 31;
	case 727: //Alenia`s Disenchanting Melody
		return 31;
	case 728: //Kelin`s Lugubrious Lament
		return 11;
	case 729: //Tarew`s Aquatic Ayre
		return 64;
	case 730: //Denon`s Disruptive Discord
		return 58;
	case 731: //Wrath of Al'Kabor
		return 14;
	case 732: //Ice Comet
		return 14;
	case 733: //Supernova
		return 38;
	case 734: //Jonthan's Whistling Warsong
		return 41;
	case 735: //Lyssa`s Veracious Concord
		return 129;
	case 736: //Denon`s Dissension
		return 60;
	case 737: //Lyssa`s Cataloging Libretto
		return 64;
	case 738: //Selo`s Consonant Chain
		return 88;
	case 739: //Melanie`s Mellifluous Motion
		return 86;
	case 740: //Vilia`s Verses of Celerity
		return 41;
	case 741: //Crission`s Pixie Strike
		return 35;
	case 742: //Denon`s Desperate Dirge
		return 58;
	case 743: //Tuyen`s Chant of Flame
		return 38;
	case 744: //Tuyen`s Chant of Frost
		return 14;
	case 745: //Cassindra`s Elegy
		return 130;
	case 746: //Selo`s Chords of Cessation
		return 88;
	case 747: //Verses of Victory
		return 41;
	case 748: //Niv`s Melody of Preservation
		return 64;
	case 749: //Jonthan's Provocation
		return 41;
	case 750: //Solon's Bewitching Bravura
		return 13;
	case 752: //Concussion
		return 53;
	case 753: //Beguile Plants
		return 13;
	case 754: //Cannibalize II
		return 17;
	case 755: //Rend
		return 58;
	case 761: //Contact Poison I
		return 75;
	case 763: //System Shock I
		return 75;
	case 767: //Liquid Silver I
		return 124;
	case 786: //Wurm Blaze
		return 38;
	case 792: //Fist of Fire
		return 38;
	case 793: //Fist of Air
		return 58;
	case 794: //Fist of Earth
		return 58;
	case 804: //Magi Bolt
		return 0;
	case 805: //Magi Strike
		return 38;
	case 807: //Magi Circle
		return 58;
	case 808: //Avatar Power
		return 0;
	case 812: //SumMonsterAttack
		return 58;
	case 817: //Guide Bolt
		return 0;
	case 823: //Divine Might Effect
		return 58;
	case 829: //FireHornet
		return 38;
	case 831: //Sathir's Gaze
		return 0;
	case 832: //WurmBreath
		return 38;
	case 834: //Sathir's Mesmerization
		return 0;
	case 835: //Chaos Breath
		return 58;
	case 837: //Stun Breath
		return 58;
	case 839: //Lightning Breath
		return 58;
	case 848: //Elemental Mastery Strike
		return 58;
	case 849: //ElementalMasteryBlast
		return 14;
	case 851: //Shardwurm Breath
		return 14;
	case 859: //Lava Breath - Test
		return 38;
	case 860: //DrakeBreath
		return 38;
	case 861: //Lava Breath
		return 38;
	case 862: //Frost Breath
		return 14;
	case 863: //Telekinesis
		return 58;
	case 868: //Sionachie`s Dreams
		return 35;
	case 893: //FireElementalAttack2
		return 58;
	case 904: //Knockback
		return 58;
	case 907: //DryBoneFireBurst
		return 38;
	case 908: //IceBoneFrostBurst
		return 14;
	case 910: //SnakeEleFireBurst
		return 38;
	case 917: //Smolder
		return 38;
	case 922: //Sonic
		return 58;
	case 929: //Harm Touch NPC
		return 58;
	case 931: //Life Drain
		return 58;
	case 945: //Ykesha
		return 58;
	case 951: //Fiery Death
		return 38;
	case 952: //Frosty Death
		return 14;
	case 966: //FireElementalAttack
		return 38;
	case 968: //WaterElementalAttack
		return 14;
	case 978: //FrostAOE
		return 14;
	case 982: //Cazic Touch
		return 0;
	case 985: //Efreeti Fire
		return 0;
	case 987: //Spiroc Thunder
		return 58;
	case 988: //Greater Spiroc Thunder
		return 0;
	case 989: //Entomb in Ice
		return 0;
	case 995: //Soul Devour
		return 0;
	case 1009: //FireBeetleSpit
		return 38;
	case 1017: //Fishnova
		return 38;
	case 1020: //Air Elemental Strike
		return 58;
	case 1021: //Water Elemental Strike
		return 14;
	case 1024: //Thunderclap
		return 58;
	case 1026: //Thunder Call
		return 58;
	case 1027: //Thunder Storm
		return 58;
	case 1028: //Static Storm
		return 58;
	case 1030: //Sand Storm
		return 0;
	case 1031: //Stone Gale
		return 0;
	case 1032: //Hail Storm
		return 58;
	case 1036: //Storm Flame
		return 0;
	case 1043: //Manastorm
		return 58;
	case 1045: //Chain Lightning
		return 58;
	case 1047: //Deluge
		return 14;
	case 1048: //Monsoons
		return 14;
	case 1049: //Tempest Wind
		return 58;
	case 1050: //Raging Blizzard
		return 14;
	case 1071: //Punishing Blow
		return 58;
	case 1074: //Steam Blast
		return 38;
	case 1075: //Electrical Short
		return 58;
	case 1077: //Mana Beam
		return 0;
	case 1078: //Gyrosonic Disruption
		return 58;
	case 1084: //Barrage of Debris
		return 0;
	case 1100: //Dreams of Ayonae
		return 35;
	case 1106: //Sear
		return 38;
	case 1107: //Tremor of Judgment
		return 58;
	case 1142: //Pain Harvest
		return 58;
	case 1144: //Jagged Rain
		return 58;
	case 1145: //Touch of Pain
		return 14;
	case 1151: //Raven Screech
		return 0;
	case 1155: //Black Symbol of Agony
		return 58;
	case 1167: //Draconic Rage Strike
		return 38;
	case 1168: //Draconic Rage Strike
		return 38;
	case 1172: //Sting of the Shissar
		return 75;
	case 1173: //Bite of the Shissar
		return 75;
	case 1180: //Zombie Bane
		return 124;
	case 1181: //Mayong's Bane
		return 124;
	case 1188: //Bixie Sting
		return 75;
	case 1189: //Scoriae Bite
		return 75;
	case 1194: //Illusion: Fier`dal
		return 49;
	case 1196: //Ancient: Lcea's Lament
		return 44;
	case 1197: //Ancient: Lullaby of Shadow
		return 35;
	case 1216: //Guide Bolt
		return 0;
	case 1221: //Terror of Darkness
		return 53;
	case 1222: //Terror of Shadows
		return 53;
	case 1223: //Terror of Death
		return 53;
	case 1224: //Terror of Terris
		return 53;
	case 1225: //Voice of Darkness
		return 128;
	case 1226: //Voice of Shadows
		return 128;
	case 1227: //Voice of Death
		return 128;
	case 1228: //Voice of Terris
		return 128;
	case 1244: //Magi Bolt
		return 0;
	case 1245: //Magi Strike
		return 38;
	case 1247: //Magi Circle
		return 58;
	case 1269: //Fangol's Breath
		return 75;
	case 1279: //Velium Chill of Al`Kabor
		return 14;
	case 1283: //Celestial Cleansing
		return 32;
	case 1284: //Valiant Companion
		return 71;
	case 1285: //Summon Companion
		return 64;
	case 1286: //Expedience
		return 71;
	case 1287: //Cassindra`s Chant of Clarity
		return 59;
	case 1288: //Divine Glory
		return 47;
	case 1289: //Strengthen Death
		return 70;
	case 1290: //Chloroblast
		return 42;
	case 1291: //Nature's Touch
		return 42;
	case 1296: //Cinder Jolt
		return 53;
	case 1310: //Porlos' Fury
		return 8;
	case 1311: //Hsagra's Wrath
		return 8;
	case 1314: //SpectraStun
		return 58;
	case 1317: //Repulse
		return 58;
	case 1325: //Combine Gate
		return 54;
	case 1326: //Ring of the Combines
		return 54;
	case 1332: //Cannibalize IV
		return 17;
	case 1334: //Translocate: Group
		return 64;
	case 1336: //Translocate: Fay
		return 36;
	case 1337: //Translocate: Tox
		return 67;
	case 1338: //Translocate: North
		return 5;
	case 1339: //Translocate: Combine
		return 54;
	case 1356: //Frosty Death2
		return 14;
	case 1359: //Enchant Clay
		return 34;
	case 1366: //Rage of the Sky
		return 0;
	case 1369: //Poisonous Chill
		return 75;
	case 1371: //Translocate: Nek
		return 5;
	case 1372: //Translocate: Common
		return 5;
	case 1373: //Translocate: Ro
		return 5;
	case 1374: //Translocate: West
		return 5;
	case 1375: //Translocate: Cazic
		return 5;
	case 1376: //Shroud of Undeath
		return 64;
	case 1377: //Primal Avatar
		return 7;
	case 1382: //Summon Holy Ale of Brell
		return 109;
	case 1391: //Dead Men Floating
		return 55;
	case 1392: //Fireburst
		return 58;
	case 1393: //Gangrenous Touch of Zum`uul
		return 43;
	case 1394: //Maelstrom of Electricity
		return 58;
	case 1397: //Strength of Nature
		return 47;
	case 1398: //Circle of Wakening Lands
		return 127;
	case 1399: //Wakening Lands Portal
		return 127;
	case 1400: //Monster Summoning I
		return 64;
	case 1401: //Summon Shard of the Core
		return 109;
	case 1402: //Monster Summoning II
		return 64;
	case 1403: //Elemental Maelstrom
		return 58;
	case 1404: //Monster Summoning III
		return 64;
	case 1405: //Wrath of the Elements
		return 58;
	case 1406: //Improved Invisibility
		return 51;
	case 1407: //Wandering Mind
		return 60;
	case 1408: //Gift of Magic
		return 59;
	case 1409: //Gift of Insight
		return 59;
	case 1410: //Gift of Brilliance
		return 59;
	case 1411: //Improved Invis to Undead
		return 51;
	case 1412: //Chilling Embrace
		return 75;
	case 1413: //Corporeal Empathy
		return 56;
	case 1414: //Augmentation of Death
		return 70;
	case 1415: //Torbas' Acid Blast
		return 75;
	case 1416: //Arch Lich
		return 17;
	case 1417: //Iceclad Gate
		return 127;
	case 1418: //Iceclad Portal
		return 127;
	case 1419: //O'Keils Flickering Flame
		return 21;
	case 1420: //Invisibility to Undead
		return 51;
	case 1421: //Enticement of Flame
		return 38;
	case 1422: //Translocate
		return 64;
	case 1423: //Great Divide Portal
		return 127;
	case 1425: //Cobalt Scar Portal
		return 127;
	case 1426: //Ice Spear of Solist
		return 14;
	case 1427: //Shock of the Tainted
		return 75;
	case 1428: //Tumultuous Strength
		return 96;
	case 1429: //Blast of Poison
		return 75;
	case 1430: //Spirit Quickening
		return 70;
	case 1431: //Form of the Great Bear
		return 48;
	case 1432: //Focus of Spirit
		return 87;
	case 1433: //Ring of Iceclad
		return 127;
	case 1434: //Circle of Iceclad
		return 127;
	case 1435: //Improved Superior Camouflage
		return 51;
	case 1436: //Fixation of Ro
		return 81;
	case 1437: //Ro's Fiery Sundering
		return 81;
	case 1438: //Circle of Great Divide
		return 127;
	case 1439: //Fury of Air
		return 58;
	case 1440: //Circle of Cobalt Scar
		return 127;
	case 1442: //Protection of the Glades
		return 46;
	case 1443: //Turning of the Unnatural
		return 124;
	case 1444: //Celestial Healing
		return 32;
	case 1445: //Armor of Protection
		return 87;
	case 1446: //Stun Command
		return 97;
	case 1447: //Aegolism
		return 1;
	case 1448: //Cantata of Soothing
		return 44;
	case 1449: //Melody of Ervaj
		return 41;
	case 1450: //Shield of Songs
		return 84;
	case 1451: //Occlusion of Sound
		return 81;
	case 1452: //Composition of Ervaj
		return 41;
	case 1453: //Divine Purpose
		return 17;
	case 1454: //Flame of Light
		return 58;
	case 1455: //Wave of Healing
		return 42;
	case 1456: //Divine Strength
		return 47;
	case 1457: //Shroud of Hate
		return 76;
	case 1458: //Shroud of Pain
		return 76;
	case 1459: //Shroud of Death
		return 16;
	case 1460: //Death Peace
		return 64;
	case 1461: //Call of Sky
		return 16;
	case 1462: //Call of Earth
		return 21;
	case 1463: //Call of Fire
		return 16;
	case 1464: //Call of the Predator
		return 7;
	case 1465: //Call of Sky Strike
		return 58;
	case 1467: //Call of Fire Strike
		return 38;
	case 1472: //Burnout IV
		return 70;
	case 1474: //Boon of the Garou
		return 16;
	case 1475: //Nature Walkers Behest
		return 104;
	case 1479: //Wave of Flame
		return 14;
	case 1480: //Silver Breath
		return 0;
	case 1481: //Scream of Chaos
		return 58;
	case 1482: //Electric Blast
		return 58;
	case 1484: //Tsunami
		return 14;
	case 1487: //Rain of Cold
		return 14;
	case 1488: //Rain of Molten Lava
		return 38;
	case 1489: //Wave of Cold
		return 14;
	case 1490: //Wave of Heat
		return 38;
	case 1494: //Flame Jet
		return 38;
	case 1498: //Doljons Rage
		return 14;
	case 1503: //Modulating Rod
		return 109;
	case 1504: //Renew Elements
		return 42;
	case 1505: //Renew Summoning
		return 42;
	case 1508: //Asystole
		return 29;
	case 1509: //Leach
		return 33;
	case 1510: //Shadow Compact
		return 56;
	case 1511: //Scent of Dusk
		return 81;
	case 1512: //Scent of Shadow
		return 81;
	case 1513: //Scent of Darkness
		return 81;
	case 1514: //Rapacious Subvention
		return 61;
	case 1515: //Covetous Subversion
		return 61;
	case 1516: //Combine Portal
		return 54;
	case 1517: //Circle of the Combines
		return 54;
	case 1518: //Remedy
		return 42;
	case 1519: //Divine Light
		return 42;
	case 1520: //Word of Vigor
		return 42;
	case 1521: //Word of Restoration
		return 42;
	case 1522: //Celestial Elixir
		return 32;
	case 1523: //Word of Redemption
		return 42;
	case 1524: //Reviviscence
		return 82;
	case 1525: //Antidote
		return 19;
	case 1526: //Annul Magic
		return 31;
	case 1527: //Trepidation
		return 37;
	case 1528: //Exile Undead
		return 124;
	case 1529: //Exile Summoned
		return 111;
	case 1530: //Banishment of Shadows
		return 23;
	case 1531: //Banishment
		return 23;
	case 1532: //Dread of Night
		return 37;
	case 1533: //Heroism
		return 46;
	case 1534: //Yaulp IV
		return 7;
	case 1535: //Symbol of Marzin
		return 112;
	case 1536: //Heroic Bond
		return 46;
	case 1537: //Bulwark of Faith
		return 6;
	case 1538: //Heroic Bond
		return 46;
	case 1539: //Fortitude
		return 46;
	case 1540: //Aegis
		return 6;
	case 1541: //Wake of Tranquility
		return 11;
	case 1542: //Upheaval
		return 58;
	case 1543: //Reckoning
		return 58;
	case 1544: //Enforced Reverence
		return 97;
	case 1545: //The Unspoken Word
		return 97;
	case 1546: //Divine Intervention
		return 64;
	case 1547: //Death Pact
		return 64;
	case 1548: //Mark of Karn
		return 64;
	case 1550: //Repulse Animal
		return 37;
	case 1551: //Circle of Winter
		return 80;
	case 1552: //Circle of Summer
		return 80;
	case 1553: //Call of Karana
		return 13;
	case 1554: //Spirit of Scale
		return 65;
	case 1555: //Glamour of Tunare
		return 81;
	case 1556: //Tunare's Request
		return 13;
	case 1557: //Girdle of Karana
		return 96;
	case 1558: //Bladecoat
		return 21;
	case 1559: //Natureskin
		return 46;
	case 1560: //Shield of Blades
		return 21;
	case 1561: //Legacy of Thorn
		return 21;
	case 1562: //Form of the Howler
		return 65;
	case 1563: //Form of the Hunter
		return 65;
	case 1564: //Spirit of Oak
		return 48;
	case 1565: //Mask of the Hunter
		return 59;
	case 1566: //Egress
		return 64;
	case 1567: //Succor
		return 64;
	case 1568: //Regrowth
		return 43;
	case 1569: //Regrowth of the Grove
		return 43;
	case 1570: //Talisman of Jasinth
		return 80;
	case 1571: //Talisman of Shadoo
		return 80;
	case 1572: //Cannibalize III
		return 17;
	case 1573: //Insidious Decay
		return 81;
	case 1574: //Spirit of the Howler
		return 104;
	case 1575: //Acumen
		return 129;
	case 1576: //Torpor
		return 32;
	case 1577: //Malosini
		return 81;
	case 1578: //Malo
		return 81;
	case 1579: //Talisman of the Cat
		return 2;
	case 1580: //Talisman of the Brute
		return 94;
	case 1581: //Talisman of the Rhino
		return 96;
	case 1582: //Talisman of the Serpent
		return 12;
	case 1583: //Talisman of the Raptor
		return 24;
	case 1584: //Shroud of the Spirits
		return 6;
	case 1585: //Talisman of Kragg
		return 87;
	case 1586: //Ice Strike
		return 14;
	case 1587: //Torrent of Poison
		return 75;
	case 1588: //Turgur's Insects
		return 88;
	case 1589: //Tigir's Insects
		return 88;
	case 1590: //Bane of Nife
		return 75;
	case 1591: //Pox of Bertoxxulous
		return 29;
	case 1592: //Cripple
		return 30;
	case 1593: //Maniacal Strength
		return 96;
	case 1594: //Deliriously Nimble
		return 2;
	case 1595: //Riotous Health
		return 94;
	case 1596: //Mortal Deftness
		return 24;
	case 1597: //Unfailing Reverence
		return 12;
	case 1598: //Avatar
		return 7;
	case 1599: //Voice of the Berserker
		return 96;
	case 1600: //Breath of Ro
		return 38;
	case 1601: //Winged Death
		return 58;
	case 1602: //Blizzard
		return 14;
	case 1603: //Scoriae
		return 38;
	case 1604: //Breath of Karana
		return 58;
	case 1605: //Frost
		return 14;
	case 1606: //Fist of Karana
		return 58;
	case 1607: //Wildfire
		return 38;
	case 1608: //Entrapping Roots
		return 83;
	case 1609: //Manaskin
		return 84;
	case 1610: //Shield of the Magi
		return 87;
	case 1611: //Demi Lich
		return 17;
	case 1612: //Quivering Veil of Xarn
		return 52;
	case 1613: //Deflux
		return 43;
	case 1614: //Chill Bones
		return 14;
	case 1615: //Cessation of Cor
		return 29;
	case 1616: //Vexing Mordinia
		return 33;
	case 1617: //Pyrocruor
		return 38;
	case 1618: //Touch of Night
		return 43;
	case 1619: //Devouring Darkness
		return 58;
	case 1620: //Splurt
		return 58;
	case 1621: //Minion of Shadows
		return 103;
	case 1622: //Servant of Bones
		return 103;
	case 1623: //Emissary of Thule
		return 103;
	case 1624: //Thrall of Bones
		return 13;
	case 1625: //Skin of the Shadow
		return 51;
	case 1626: //Levant
		return 64;
	case 1627: //Abscond
		return 64;
	case 1628: //Evacuate
		return 64;
	case 1629: //Enslave Death
		return 13;
	case 1630: //Defoliation
		return 74;
	case 1631: //Atol's Spectral Shackles
		return 89;
	case 1632: //Plainsight
		return 129;
	case 1633: //Fetter
		return 83;
	case 1634: //Tishan's Discord
		return 97;
	case 1635: //Markar's Discord
		return 97;
	case 1636: //Invert Gravity
		return 58;
	case 1637: //Draught of Fire
		return 38;
	case 1638: //Lure of Flame
		return 38;
	case 1639: //Voltaic Draught
		return 58;
	case 1640: //Lure of Lightning
		return 58;
	case 1641: //Draught of Ice
		return 14;
	case 1642: //Lure of Frost
		return 14;
	case 1643: //Draught of Jiva
		return 58;
	case 1644: //Pillar of Flame
		return 38;
	case 1645: //Pillar of Lightning
		return 58;
	case 1646: //Pillar of Frost
		return 14;
	case 1647: //Tears of Prexus
		return 14;
	case 1648: //Tears of Solusek
		return 38;
	case 1649: //Tears of Druzzil
		return 58;
	case 1650: //Inferno of Al'Kabor
		return 38;
	case 1651: //Retribution of Al'Kabor
		return 14;
	case 1652: //Vengeance of Al'Kabor
		return 58;
	case 1653: //Jyll's Static Pulse
		return 58;
	case 1654: //Jyll's Zephyr of Ice
		return 14;
	case 1655: //Jyll's Wave of Heat
		return 38;
	case 1656: //Thunderbold
		return 58;
	case 1657: //Winds of Gelid
		return 14;
	case 1658: //Sunstrike
		return 38;
	case 1659: //Scintillation
		return 38;
	case 1660: //Char
		return 38;
	case 1661: //Scars of Sigil
		return 38;
	case 1662: //Sirocco
		return 38;
	case 1663: //Shock of Steel
		return 58;
	case 1664: //Seeking Flame of Seukor
		return 38;
	case 1665: //Manastorm
		return 58;
	case 1666: //Phantom Armor
		return 43;
	case 1667: //Cadeau of Flame
		return 21;
	case 1668: //Boon of Immolation
		return 21;
	case 1669: //Aegis of Ro
		return 21;
	case 1670: //Velocity
		return 71;
	case 1671: //Vocarate: Earth
		return 100;
	case 1672: //Vocarate: Water
		return 105;
	case 1673: //Vocarate: Fire
		return 102;
	case 1674: //Vocarate: Air
		return 98;
	case 1675: //Greater Vocaration: Earth
		return 100;
	case 1676: //Greater Vocaration: Water
		return 105;
	case 1677: //Greater Vocaration: Fire
		return 102;
	case 1678: //Greater Vocaration: Air
		return 98;
	case 1679: //Dyzil's Deafening Decoy
		return 64;
	case 1680: //Gift of Xev
		return 108;
	case 1681: //Bristlebane's Bundle
		return 109;
	case 1682: //Quiver of Marr
		return 110;
	case 1683: //Bandoleer of Luclin
		return 110;
	case 1684: //Pouch of Quellious
		return 110;
	case 1685: //Muzzle of Mardu
		return 64;
	case 1686: //Theft of Thought
		return 60;
	case 1687: //Collaboration
		return 3;
	case 1688: //Enlightenment
		return 130;
	case 1689: //Rune V
		return 84;
	case 1690: //Fascination
		return 35;
	case 1691: //Glamour of Kintaz
		return 35;
	case 1692: //Rapture
		return 35;
	case 1693: //Clarity II
		return 59;
	case 1694: //Boon of the Clear Mind
		return 59;
	case 1695: //Gift of Pure Thought
		return 59;
	case 1696: //Color Slant
		return 97;
	case 1697: //Recant Magic
		return 31;
	case 1698: //Dementia
		return 58;
	case 1699: //Wind of Tashani
		return 81;
	case 1700: //Torment of Argli
		return 58;
	case 1701: //Overwhelming Splendor
		return 12;
	case 1702: //Tashanian
		return 81;
	case 1703: //Asphyxiate
		return 58;
	case 1704: //Wind of Tashanian
		return 81;
	case 1705: //Boltran`s Agacerie
		return 13;
	case 1707: //Dictate
		return 13;
	case 1708: //Aanya's Quickening
		return 41;
	case 1709: //Wonderous Rapidity
		return 41;
	case 1710: //Visions of Grandeur
		return 41;
	case 1711: //Umbra
		return 6;
	case 1712: //Forlorn Deeds
		return 88;
	case 1713: //Bedlam
		return 84;
	case 1714: //Memory Flux
		return 63;
	case 1715: //Largarn's Lamentation
		return 60;
	case 1716: //Scent of Terris
		return 81;
	case 1717: //Shadowbond
		return 56;
	case 1718: //Sedulous Subversion
		return 61;
	case 1719: //Engorging Roots
		return 83;
	case 1720: //Eye of Tallon
		return 129;
	case 1721: //Unswerving Hammer of Faith
		return 99;
	case 1722: //Flaming Sword of Xuzl
		return 99;
	case 1723: //Zumaik`s Animation
		return 99;
	case 1724: //Disintegrate
		return 23;
	case 1725: //Wake of Karana
		return 64;
	case 1726: //Sunskin
		return 51;
	case 1727: //Legacy of Spike
		return 21;
	case 1728: //Manasink
		return 93;
	case 1729: //Augment
		return 41;
	case 1733: //Convergence
		return 82;
	case 1734: //Infusion
		return 61;
	case 1735: //Trucidation
		return 43;
	case 1736: //Wind of the North
		return 54;
	case 1737: //Wind of the South
		return 54;
	case 1738: //Tishan's Relocation
		return 54;
	case 1739: //Markar's Relocation
		return 54;
	case 1740: //Dustdevil
		return 58;
	case 1741: //Jolt
		return 53;
	case 1742: //Bobbing Corpse
		return 55;
	case 1743: //Divine Favor
		return 47;
	case 1744: //Harvest
		return 61;
	case 1747: //Brusco`s Bombastic Bellow
		return 58;
	case 1748: //Angstlich's Assonance
		return 58;
	case 1749: //Kazumi's Note of Preservation
		return 52;
	case 1750: //Selo`s Song of Travel
		return 65;
	case 1751: //Largo`s Absonant Binding
		return 88;
	case 1752: //Nillipus` March of the Wee
		return 84;
	case 1753: //Song of Twilight
		return 35;
	case 1754: //Song of Dawn
		return 35;
	case 1755: //Song of Highsun
		return 86;
	case 1756: //Song of Midnight
		return 37;
	case 1757: //Vilia`s Chorus of Celerity
		return 41;
	case 1758: //Selo`s Assonant Strane
		return 88;
	case 1759: //Cantata of Replenishment
		return 44;
	case 1760: //McVaxius` Rousing Rondo
		return 21;
	case 1761: //Cassindra's Insipid Ditty
		return 60;
	case 1762: //Jonthan's Inspiration
		return 41;
	case 1763: //Niv`s Harmonic
		return 6;
	case 1764: //Denon`s Bereavement
		return 75;
	case 1765: //Solon's Charismatic Concord
		return 12;
	case 1767: //Bonds of Tunare
		return 89;
	case 1768: //Sacrifice
		return 50;
	case 1769: //Lure of Ice
		return 14;
	case 1770: //Rage of Zomm
		return 64;
	case 1771: //Call of the Hero
		return 64;
	case 1772: //Mala
		return 81;
	case 1773: //Conjure Corpse
		return 64;
	case 1774: //Naltron's Mark
		return 112;
	case 1776: //Spirit of Wolf
		return 65;
	case 1784: //Velium Shards
		return 14;
	case 1785: //Flamesong
		return 38;
	case 1793: //Judgment of Ice
		return 14;
	case 1794: //Shards of Sorrow
		return 14;
	case 1797: //Enchant Velium
		return 34;
	case 1798: //Imbue Opal
		return 50;
	case 1799: //Imbue Topaz
		return 50;
	case 1800: //Imbue Plains Pebble
		return 50;
	case 1802: //Storm Strike
		return 0;
	case 1803: //Shrieking Howl
		return 58;
	case 1807: //Stunning Blow
		return 58;
	case 1812: //Nature's Wrath
		return 0;
	case 1815: //Flames of Ro
		return 38;
	case 1819: //Primal Essence
		return 96;
	case 1820: //Divine Wrath
		return 58;
	case 1827: //Frost Shards
		return 14;
	case 1831: //Diminution
		return 64;
	case 1834: //Poison Animal I
		return 75;
	case 1835: //Poison Summoned I
		return 75;
	case 1843: //Poison Animal II
		return 75;
	case 1844: //Poison Animal III
		return 75;
	case 1845: //Poison Summoned II
		return 75;
	case 1846: //Poison Summoned III
		return 75;
	case 1853: //Contact Poison II
		return 75;
	case 1854: //Contact Poison III
		return 75;
	case 1855: //Contact Poison IV
		return 75;
	case 1860: //System Shock II
		return 75;
	case 1861: //System Shock III
		return 75;
	case 1862: //System Shock IV
		return 75;
	case 1870: //Liquid Silver II
		return 124;
	case 1871: //Liquid Silver III
		return 124;
	case 1874: //Ant Legs
		return 64;
	case 1881: //System Shock V
		return 75;
	case 1884: //Imbue Ivory
		return 50;
	case 1885: //Imbue Amber
		return 50;
	case 1886: //Imbue Sapphire
		return 50;
	case 1887: //Imbue Ruby
		return 50;
	case 1888: //Imbue Emerald
		return 50;
	case 1889: //Enchant Mithril
		return 34;
	case 1890: //Enchant Adamantite
		return 34;
	case 1891: //Imbue Jade
		return 50;
	case 1892: //Enchant Steel
		return 34;
	case 1893: //Enchant Brellium
		return 34;
	case 1894: //Imbue Black Pearl
		return 50;
	case 1895: //Imbue Diamond
		return 50;
	case 1896: //Imbue Rose Quartz
		return 50;
	case 1897: //Imbue Black Sapphire
		return 50;
	case 1898: //Imbue Peridot
		return 50;
	case 1899: //Imbue Fire Opal
		return 50;
	case 1941: //Lava Breath
		return 38;
	case 1942: //Frost Breath
		return 14;
	case 1943: //Molten Breath
		return 38;
	case 1944: //Summon Orb
		return 110;
	case 1947: //Ice Rend
		return 14;
	case 1948: //Destroy
		return 0;
	case 1953: //Mastodon Stomp
		return 58;
	case 1954: //Devour Soul
		return 0;
	case 1955: //DrakeBreathBig
		return 38;
	case 1957: //Holy Shock
		return 58;
	case 1968: //Stunning Strike
		return 58;
	case 1969: //Flame of the Efreeti
		return 38;
	case 1970: //Verlekarnorm's Disaster
		return 58;
	case 1971: //Rocksmash
		return 58;
	case 2005: //Nature's Holy Wrath
		return 58;
	case 2006: //Static
		return 58;
	case 2014: //Incinerate Bones
		return 38;
	case 2015: //Conglaciation of Bone
		return 14;
	case 2016: //Dementing Visions
		return 58;
	case 2019: //Thunder Strike
		return 58;
	case 2020: //Circle of Surefall Glade
		return 5;
	case 2021: //Ring of Surefall Glade
		return 5;
	case 2022: //Translocate: Iceclad
		return 127;
	case 2023: //Translocate: Great Divide
		return 127;
	case 2024: //Translocate: Wakening Lands
		return 127;
	case 2025: //Translocate: Cobalt Scar
		return 127;
	case 2026: //Great Divide Gate
		return 127;
	case 2027: //Wakening Lands Gate
		return 127;
	case 2028: //Cobalt Scar Gate
		return 127;
	case 2029: //Ring of Great Divide
		return 127;
	case 2030: //Ring of Wakening Lands
		return 127;
	case 2031: //Ring of Cobalt Scar
		return 127;
	case 2035: //Tentacle Sting
		return 75;
	case 2036: //Rain of the Arch Mage
		return 0;
	case 2040: //Winds of the Archmage
		return 58;
	case 2043: //Kambooz's Touch
		return 75;
	case 2047: //Death Shackles
		return 0;
	case 2048: //Ssraeshza's Command
		return 58;
	case 2054: //Icy Claws
		return 14;
	case 2068: //Blast of Frost
		return 14;
	case 2070: //Marauder's Wrath
		return 38;
	case 2075: //Umbral Rot
		return 29;
	case 2076: //Presence of Ssraeshza
		return 0;
	case 2085: //Lesser Infusion 
		return 0;
	case 2086: //Infusion
		return 0;
	case 2087: //Greater Infusion
		return 0;
	case 2091: //Lesser Rejuvenation
		return 0;
	case 2092: //Rejuvination 
		return 0;
	case 2093: //Greater Rejuvenation
		return 0;
	case 2094: //Zruk Breath
		return 0;
	case 2101: //Pain and Suffering
		return 0;
	case 2102: //Drake Breath
		return 75;
	case 2103: //Drake Breath
		return 38;
	case 2104: //Drake Breath
		return 14;
	case 2105: //Drake Breath
		return 29;
	case 2106: //Gift of A'err
		return 0;
	case 2109: //Ancient: High Priest's Bulwark
		return 87;
	case 2110: //Skin like Wood
		return 46;
	case 2111: //Burst of Flame
		return 38;
	case 2112: //Ancient: Feral Avatar
		return 7;
	case 2113: //Ancient: Scourge of Nife
		return 75;
	case 2114: //Ancient: Master of Death
		return 17;
	case 2115: //Ancient: Lifebane
		return 75;
	case 2116: //Ancient: Destruction of Ice
		return 14;
	case 2117: //Ancient: Greater Concussion
		return 53;
	case 2118: //Ancient: Shock of Sun
		return 38;
	case 2119: //Ancient: Burnout Blaze
		return 70;
	case 2120: //Ancient: Eternal Rapture
		return 35;
	case 2121: //Ancient: Chaotic Visions
		return 58;
	case 2122: //Ancient: Gift of Aegolism
		return 1;
	case 2125: //Ancient: Legacy of Blades
		return 21;
	case 2126: //Ancient: Starfire of Ro
		return 38;
	case 2127: //Tragedy at Cazic Thule
		return 0;
	case 2130: //Horrific Force
		return 58;
	case 2131: //Vortex of Horror
		return 58;
	case 2137: //Rain of Terror
		return 75;
	case 2139: //Corpse Breath
		return 64;
	case 2156: //Deadly Curse of Noqufiel
		return 0;
	case 2157: //Word of Command
		return 58;
	case 2161: //Shock of Shadows
		return 58;
	case 2162: //Black Winds
		return 58;
	case 2163: //Lure of Shadows
		return 58;
	case 2167: //Fling
		return 0;
	case 2168: //Reanimation
		return 82;
	case 2169: //Reconstitution
		return 82;
	case 2170: //Reparation
		return 82;
	case 2171: //Renewal
		return 82;
	case 2172: //Restoration
		return 82;
	case 2173: //Hand of the Gods
		return 0;
	case 2175: //Celestial Health
		return 32;
	case 2176: //Spiritual Light
		return 44;
	case 2177: //Spiritual Radiance
		return 44;
	case 2178: //Spiritual Brawn
		return 47;
	case 2179: //Tunare's Renewal
		return 42;
	case 2180: //Ethereal Elixir
		return 32;
	case 2181: //Hammer of Judgment
		return 110;
	case 2182: //Ethereal Light
		return 42;
	case 2183: //Lesser Succor
		return 64;
	case 2184: //Lesser Evacuate
		return 64;
	case 2188: //Protection of the Cabbage
		return 46;
	case 2190: //Divine Stun
		return 97;
	case 2202: //Mana Shield
		return 93;
	case 2203: //Donlo's Dementia
		return 64;
	case 2206: //Tortured Memory
		return 38;
	case 2213: //Lesser Summon Corpse
		return 64;
	case 2230: //Summon Brass Choker
		return 107;
	case 2231: //Summon Silver Choker
		return 107;
	case 2232: //Summon Golden Choker
		return 107;
	case 2233: //Summon Linen Mantle
		return 107;
	case 2234: //Summon Leather Mantle
		return 107;
	case 2235: //Summon Silken Mantle
		return 107;
	case 2236: //Summon Jade Bracelet
		return 107;
	case 2237: //Summon Opal Bracelet
		return 107;
	case 2238: //Summon Ruby Bracelet
		return 107;
	case 2239: //Summon Tiny Ring
		return 107;
	case 2240: //Summon Twisted Ring
		return 107;
	case 2241: //Summon Studded Ring
		return 107;
	case 2242: //Summon Tarnished Bauble
		return 107;
	case 2243: //Summon Shiny Bauble
		return 107;
	case 2244: //Summon Brilliant Bauble
		return 107;
	case 2248: //Acumen
		return 129;
	case 2249: //River's Rancor
		return 58;
	case 2250: //Fiery Retribution
		return 38;
	case 2251: //Furor of the Wild
		return 14;
	case 2255: //Wrath of the Wild
		return 14;
	case 2258: //Frigid Dominion
		return 14;
	case 2261: //Frozen Torrent
		return 14;
	case 2264: //Hail of Ice
		return 14;
	case 2268: //Touch of the Void
		return 0;
	case 2312: //Life Bind
		return 43;
	case 2321: //Energy Burst
		return 0;
	case 2326: //Yaulp V
		return 7;
	case 2375: //Spectral Essence
		return 0;
	case 2377: //Screeching Ricochet
		return 58;
	case 2378: //Drakeen Breath
		return 14;
	case 2379: //Drakeen Monsoon
		return 14;
	case 2380: //Drakeen Vortex
		return 14;
	case 2381: //Wing Draft
		return 58;
	case 2382: //Wing Gust
		return 58;
	case 2383: //Wing Squall
		return 58;
	case 2384: //Wing Tempest
		return 58;
	case 2385: //Frost Pummel
		return 14;
	case 2386: //Ice Pummel
		return 14;
	case 2387: //Frigid Shard Pummel
		return 14;
	case 2392: //Sweltering Carcass
		return 0;
	case 2417: //Ring of Grimling
		return 57;
	case 2418: //Grimling Gate
		return 57;
	case 2419: //Circle of Grimling
		return 57;
	case 2420: //Grimling Portal
		return 57;
	case 2421: //Translocate: Grimling
		return 57;
	case 2422: //Ring of Twilight
		return 57;
	case 2423: //Twilight Gate
		return 57;
	case 2424: //Circle of Twilight
		return 57;
	case 2425: //Twilight Portal
		return 57;
	case 2426: //Translocate: Twilight
		return 57;
	case 2427: //Ring of Dawnshroud
		return 57;
	case 2428: //Dawnshroud Gate
		return 57;
	case 2429: //Circle of Dawnshroud
		return 57;
	case 2430: //Dawnshroud Portal
		return 57;
	case 2431: //Translocate: Dawnshroud
		return 57;
	case 2432: //Circle of the Nexus
		return 57;
	case 2433: //Ring of the Nexus
		return 57;
	case 2434: //Avatar
		return 7;
	case 2435: //Kragg's Mending
		return 42;
	case 2436: //War Arrows
		return 58;
	case 2437: //Hendin Arrow
		return 0;
	case 2443: //Blade of Vallon
		return 58;
	case 2450: //Barb of Tallon
		return 38;
	case 2452: //Barb of Tallon
		return 75;
	case 2453: //Thorns of Drunder
		return 0;
	case 2462: //Ethereal Remedy
		return 77;
	case 2501: //Sanctuary
		return 64;
	case 2502: //Celestial Remedy
		return 32;
	case 2503: //Sermon of the Righteous
		return 124;
	case 2504: //Sacred Word
		return 58;
	case 2505: //Armor of the Faithful
		return 87;
	case 2506: //Epitaph of Life
		return 124;
	case 2507: //Mark of Retribution
		return 21;
	case 2508: //Judgment
		return 58;
	case 2509: //Blessed Armor of the Risen
		return 87;
	case 2510: //Blessing of Aegolism
		return 1;
	case 2511: //Protection of Wood
		return 46;
	case 2512: //Protection of Rock
		return 46;
	case 2513: //Protection of Steel
		return 46;
	case 2514: //Protection of Diamond
		return 46;
	case 2515: //Protection of Nature
		return 46;
	case 2516: //Foliage Shield
		return 51;
	case 2517: //Spirit of Eagle
		return 65;
	case 2518: //Ro's Smoldering Disjunction
		return 81;
	case 2519: //Circle of Seasons
		return 80;
	case 2520: //Nature's Recovery
		return 32;
	case 2521: //Talisman of the Beast
		return 96;
	case 2522: //Grow
		return 64;
	case 2523: //Form of the Bear
		return 48;
	case 2524: //Spirit of Bih`Li
		return 65;
	case 2525: //Harnessing of Spirit
		return 87;
	case 2526: //Disinfecting Aura
		return 19;
	case 2527: //Plague of Insects
		return 88;
	case 2528: //Regrowth of Dar Khura
		return 43;
	case 2529: //Talisman of Epuration
		return 80;
	case 2530: //Khura's Focusing
		return 87;
	case 2531: //Summon Elemental Defender
		return 106;
	case 2532: //Summon Phantom Leather
		return 106;
	case 2533: //Summon Phantom Chain
		return 106;
	case 2534: //Summon Phantom Plate
		return 106;
	case 2535: //Summon Elemental Blanket
		return 106;
	case 2536: //Transon's Elemental Infusion
		return 42;
	case 2537: //Veil of Elements
		return 51;
	case 2538: //Mass Mystical Transvergance
		return 109;
	case 2539: //Transon's Phantasmal Protection
		return 44;
	case 2540: //Shock of Fiery Blades
		return 38;
	case 2541: //Focus Death
		return 70;
	case 2542: //Shackle of Bone
		return 124;
	case 2543: //Eternities Torment
		return 124;
	case 2544: //Shackle of Spirit
		return 124;
	case 2545: //Insidious Retrogression
		return 89;
	case 2546: //Degeneration
		return 76;
	case 2547: //Succussion of Shadows
		return 76;
	case 2548: //Crippling Claudication
		return 76;
	case 2549: //Mind Wrack
		return 60;
	case 2550: //Zevfeer's Theft of Vitae
		return 33;
	case 2551: //O'Keils Embers
		return 21;
	case 2552: //Garrisons Mighty Mana Shock
		return 58;
	case 2553: //Minor Familiar
		return 101;
	case 2554: //Elnerick's Entombment of Ice
		return 83;
	case 2555: //Lesser Familiar
		return 101;
	case 2556: //Firetree's Familiar Augment
		return 71;
	case 2557: //Familiar
		return 101;
	case 2558: //Decession
		return 64;
	case 2559: //Spellshield
		return 93;
	case 2560: //Greater Familiar
		return 101;
	case 2561: //Intellectual Advancement
		return 39;
	case 2562: //Intellectual Superiority
		return 39;
	case 2563: //Haunting Visage
		return 128;
	case 2564: //Calming Visage
		return 128;
	case 2565: //Illusion: Imp
		return 48;
	case 2566: //Trickster's Augmentation
		return 16;
	case 2567: //Beguiling Visage
		return 128;
	case 2568: //Horrifying Visage
		return 128;
	case 2569: //Glamorous Visage
		return 128;
	case 2570: //Koadic's Endless Intellect
		return 59;
	case 2571: //Despair
		return 76;
	case 2572: //Scream of Hate
		return 76;
	case 2573: //Scream of Pain
		return 76;
	case 2574: //Scream of Death
		return 16;
	case 2575: //Abduction of Strength
		return 76;
	case 2576: //Mental Corruption
		return 16;
	case 2577: //Torrent of Hate
		return 76;
	case 2578: //Torrent of Pain
		return 76;
	case 2579: //Torrent of Fatigue
		return 76;
	case 2580: //Cloak of the Akheva
		return 87;
	case 2581: //Cease
		return 97;
	case 2582: //Desist
		return 97;
	case 2583: //Instrument of Nife
		return 16;
	case 2584: //Divine Vigor
		return 47;
	case 2585: //Valor of Marr
		return 41;
	case 2586: //Thunder of Karana
		return 60;
	case 2587: //Quellious' Word of Tranquility
		return 97;
	case 2588: //Breath of Tunare
		return 17;
	case 2589: //Healing Wave of Prexus
		return 42;
	case 2590: //Brell's Mountainous Barrier
		return 47;
	case 2591: //Tangling Weeds
		return 89;
	case 2592: //Hawk Eye
		return 129;
	case 2593: //Riftwind's Protection
		return 21;
	case 2594: //Nature's Precision
		return 7;
	case 2595: //Force of Nature
		return 21;
	case 2596: //Falcon Eye
		return 129;
	case 2597: //Jolting Blades
		return 53;
	case 2598: //Mark of the Predator
		return 7;
	case 2599: //Eagle Eye
		return 129;
	case 2600: //Warder's Protection
		return 47;
	case 2601: //Magical Monologue
		return 64;
	case 2602: //Song of Sustenance
		return 64;
	case 2603: //Amplification
		return 64;
	case 2604: //Katta's Song of Sword Dancing
		return 16;
	case 2605: //Selo`s Accelerating Chorus
		return 65;
	case 2606: //Battlecry of the Vah Shir
		return 41;
	case 2607: //Elemental Chorus
		return 80;
	case 2608: //Purifying Chorus
		return 80;
	case 2609: //Chorus of Replenishment
		return 44;
	case 2610: //Warsong of the Vah Shir
		return 41;
	case 2611: //Sharik's Replenishing
		return 42;
	case 2612: //Spirit of Sharik
		return 104;
	case 2613: //Keshuval's Rejuvenation
		return 42;
	case 2614: //Spirit of Keshuval
		return 104;
	case 2615: //Herikol's Soothing
		return 42;
	case 2616: //Spirit of Herikol
		return 104;
	case 2617: //Yekan's Recovery
		return 42;
	case 2618: //Spirit of Yekan
		return 104;
	case 2619: //Yekan's Quickening
		return 70;
	case 2620: //Vigor of Zehkes
		return 42;
	case 2621: //Spirit of Kashek
		return 104;
	case 2622: //Aid of Khurenz
		return 42;
	case 2623: //Spirit of Omakin
		return 104;
	case 2624: //Sha's Restoration
		return 42;
	case 2625: //Omakin's Alacrity
		return 70;
	case 2626: //Spirit of Zehkes
		return 104;
	case 2627: //Spirit of Khurenz
		return 104;
	case 2628: //Sha's Ferocity
		return 70;
	case 2629: //Spiritual Purity
		return 44;
	case 2630: //Spiritual Strength
		return 47;
	case 2631: //Spirit of Khati Sha
		return 104;
	case 2632: //Summon Warder
		return 104;
	case 2633: //Spirit of Khaliz
		return 104;
	case 2634: //Sha's Lethargy
		return 88;
	case 2635: //Spirit of Lightning
		return 71;
	case 2636: //Spirit of the Blizzard
		return 71;
	case 2637: //Spirit of Inferno
		return 71;
	case 2638: //Spirit of the Scorpion
		return 71;
	case 2639: //Spirit of Vermin
		return 71;
	case 2640: //Spirit of Wind
		return 71;
	case 2641: //Spirit of the Storm
		return 71;
	case 2642: //Claw of Khati Sha
		return 38;
	case 2650: //Blazing Heat
		return 38;
	case 2651: //Vibrant Might
		return 58;
	case 2652: //Descending Might
		return 58;
	case 2654: //Fireblast
		return 38;
	case 2656: //Wrathful Strike
		return 58;
	case 2657: //Terrifying Darkness
		return 58;
	case 2658: //Lightning Surge
		return 58;
	case 2662: //Storm of Lightning
		return 58;
	case 2663: //Clash of Will
		return 58;
	case 2664: //Frostcall
		return 14;
	case 2665: //Wintercall
		return 14;
	case 2669: //Storm of Ice
		return 14;
	case 2670: //Rebuke the Dead
		return 124;
	case 2678: //Fungal Vengeance
		return 75;
	case 2710: //Trickster's Torment
		return 38;
	case 2711: //Trickster's TormentSK
		return 38;
	case 2717: //Mental Corruption Strike
		return 29;
	case 2720: //Spirit of Lightning Strike
		return 58;
	case 2721: //Spirit of Blizzard Strike
		return 14;
	case 2722: //Spirit of Inferno Strike
		return 38;
	case 2723: //Spirit of Scorpion Strike
		return 75;
	case 2724: //Spirit of Vermin Strike
		return 29;
	case 2725: //Spirit of Wind Strike
		return 58;
	case 2726: //Spirit of Storm Strike
		return 38;
	case 2729: //Condemnation of Nife
		return 124;
	case 2732: //Molten Fist
		return 38;
	case 2734: //The Nexus
		return 57;
	case 2736: //SpellTheft1
		return 31;
	case 2742: //Purify Soul
		return 19;
	case 2750: //Rabid Bear
		return 48;
	case 2752: //Restore Companion
		return 42;
	case 2754: //Frenzied Burnout
		return 70;
	case 2757: //Wave of Revulsion
		return 37;
	case 2758: //Improved Familiar
		return 101;
	case 2759: //Undead Pact
		return 13;
	case 2761: //Dominating Gaze
		return 13;
	case 2762: //Disease Touch
		return 29;
	case 2763: //Poison Touch
		return 75;
	case 2764: //Call to Corpse
		return 64;
	case 2766: //Life Curse
		return 43;
	case 2767: //Dragon Force
		return 58;
	case 2768: //Grimling LT 30
		return 58;
	case 2770: //Rain of Spores
		return 75;
	case 2802: //Flurry of Pebbles
		return 0;
	case 2809: //Wave of Death
		return 14;
	case 2816: //Storm Tremor
		return 58;
	case 2822: //Upheaval
		return 58;
	case 2826: //Illusion: Vah Shir
		return 49;
	case 2833: //AdvisorNova
		return 38;
	case 2836: //Grimling Comet
		return 14;
	case 2842: //Shrieker Stun
		return 29;
	case 2858: //AcryliaKB
		return 0;
	case 2859: //Touch of Vinitras
		return 0;
	case 2877: //Moonfire
		return 14;
	case 2878: //Fireclaw
		return 38;
	case 2879: //Phantasmal Armor
		return 43;
	case 2880: //Remove Greater Curse
		return 19;
	case 2881: //Everlasting Breath
		return 64;
	case 2882: //Firetree's Familiar Enhancement
		return 71;
	case 2883: //Elnerick's Electrical Rending
		return 58;
	case 2884: //Garrison's Superior Sundering
		return 38;
	case 2885: //Funeral Pyre of Kelador
		return 38;
	case 2886: //Acumen of Dar Khura
		return 129;
	case 2887: //Mask of the Stalker
		return 59;
	case 2888: //Spirit of Flame
		return 71;
	case 2889: //Spirit of Flame Strike
		return 38;
	case 2890: //Spirit of Snow
		return 71;
	case 2891: //Spirit of Snow Strike
		return 14;
	case 2892: //Deathly Temptation
		return 17;
	case 2893: //Marzin's Mark
		return 112;
	case 2894: //Levitation
		return 55;
	case 2895: //Speed of the Brood
		return 41;
	case 2896: //Transon's Elemental Renewal
		return 42;
	case 2901: //Illumination
		return 0;
	case 2902: //Shissar Broodling Poison
		return 75;
	case 2908: //Banshee Wail
		return 58;
	case 2927: //Storm Tremor
		return 58;
	case 2936: //Ervaj's Lost Composition
		return 60;
	case 2941: //Savagery
		return 7;
	case 2942: //Sha's Advantage
		return 88;
	case 2943: //Translocate: Nexus
		return 57;
	case 2944: //Nexus Portal
		return 57;
	case 2945: //Nexus Gate
		return 57;
	case 2946: //Remove Curse
		return 19;
	case 2950: //Grol Baku Strike
		return 58;
	case 2951: //Grol Baku Strike
		return 58;
	case 2952: //Strike of the Grol Baku
		return 58;
	case 2956: //Fire Blast
		return 38;
	case 2957: //Water Blast
		return 14;
	case 2969: //Shadow Creep
		return 76;
	case 2973: //Ember Strike
		return 38;
	case 2984: //Lotus Spines
		return 29;
	case 2988: //Wave of Toxicity
		return 75;
	case 2991: //Deathly Ice
		return 14;
	case 2992: //Deathly Fire
		return 38;
	case 2993: //Deathly Spores
		return 75;
	case 2994: //Deathly Fever
		return 29;
	case 2995: //Deep Spores
		return 75;
	case 2996: //Claw of the Hunter
		return 58;
	case 2997: //Claw of the Beast
		return 38;
	case 2999: //Claw of Bestial Fury
		return 58;
	case 3000: //Devouring Nightmare
		return 0;
	case 3004: //Fist of Lava
		return 38;
	case 3005: //Ball of Lava
		return 38;
	case 3013: //Fiery Strike
		return 38;
	case 3017: //Mighty Bellow of Fire
		return 38;
	case 3018: //Nova Inferno
		return 38;
	case 3020: //Rain of Burning Fire
		return 38;
	case 3030: //Dreams of Thule
		return 35;
	case 3031: //Xegony's Phantasmal Guard
		return 44;
	case 3032: //Touch of Mujaki
		return 43;
	case 3034: //Aeldorb's Animation
		return 99;
	case 3035: //Neurotoxin
		return 75;
	case 3036: //Wrath of Ice
		return 14;
	case 3037: //Wrath of Fire
		return 38;
	case 3038: //Wrath of Wind
		return 58;
	case 3039: //Protection of the Wild
		return 21;
	case 3040: //Belt of Magi'Kot
		return 64;
	case 3041: //Blade of Walnan
		return 64;
	case 3042: //Fist of Ixiblat
		return 64;
	case 3043: //Blade of The Kedge
		return 64;
	case 3044: //Girdle of Magi'Kot
		return 64;
	case 3045: //Talisman of Return
		return 109;
	case 3047: //Kazad`s Mark
		return 112;
	case 3051: //Fiery Assault
		return 38;
	case 3057: //Tidal Freeze
		return 14;
	case 3063: //Illusion: Froglok
		return 49;
	case 3066: //Requiem of Time
		return 88;
	case 3069: //Seething Hatred
		return 58;
	case 3071: //Insipid Dreams
		return 38;
	case 3100: //Mark of Retaliation
		return 64;
	case 3107: //Cry of Fire
		return 16;
	case 3129: //Call of Sky Strike
		return 58;
	case 3130: //Call of Sky Strike
		return 58;
	case 3131: //Call of Fire Strike
		return 38;
	case 3132: //Call of Fire Strike
		return 38;
	case 3133: //Cry of Fire Strike
		return 38;
	case 3134: //Portal of Knowledge
		return 116;
	case 3135: //Hammer of Divinity
		return 110;
	case 3136: //Hammer of Souls
		return 110;
	case 3156: //Torrent of Brambles
		return 58;
	case 3162: //Wind Strike
		return 58;
	case 3163: //Storm Avalanche
		return 58;
	case 3164: //Froglok Misery
		return 29;
	case 3167: //Strike of Marr
		return 58;
	case 3172: //Denial
		return 58;
	case 3176: //Butchery
		return 0;
	case 3177: //Prayer of Pain
		return 58;
	case 3178: //Vallon's Quickening
		return 41;
	case 3179: //Spirit of Rellic Strike
		return 58;
	case 3180: //Knowledge Portal
		return 116;
	case 3181: //Translocate: Knowledge
		return 116;
	case 3182: //Ring of Knowledge
		return 116;
	case 3183: //Knowledge Gate
		return 116;
	case 3184: //Circle of Knowledge
		return 116;
	case 3185: //Flight of Eagles
		return 65;
	case 3186: //Yaulp VI
		return 7;
	case 3187: //Sermon of Penitence
		return 124;
	case 3188: //Rod of Mystical Transvergance
		return 109;
	case 3189: //Tears of Arlyxir
		return 38;
	case 3190: //Crusader`s Touch
		return 19;
	case 3191: //Shock of Magic
		return 58;
	case 3192: //Earthen Roots
		return 83;
	case 3194: //Greater Fetter
		return 83;
	case 3195: //Greater Immobilize
		return 83;
	case 3196: //Petrifying Earth
		return 83;
	case 3197: //Pacification
		return 11;
	case 3198: //Flameshield of Ro
		return 21;
	case 3199: //Arcane Rune
		return 84;
	case 3205: //Summon Platinum Choker
		return 107;
	case 3206: //Summon Runed Mantle
		return 107;
	case 3207: //Summon Sapphire Bracelet
		return 107;
	case 3208: //Summon Spiked Ring
		return 107;
	case 3209: //Summon Glowing Bauble
		return 107;
	case 3210: //Summon Jewelry Bag
		return 107;
	case 3221: //Shattering Glass
		return 58;
	case 3222: //Web of Glass
		return 58;
	case 3223: //Shards of Glass
		return 58;
	case 3227: //Shroud of Chaos
		return 16;
	case 3229: //Boggle
		return 53;
	case 3232: //Karana's Renewal
		return 42;
	case 3233: //Tnarg`s Mending
		return 42;
	case 3234: //Protection of the Nine
		return 46;
	case 3235: //Focus of Soul
		return 87;
	case 3236: //no spell
		return 75;
	case 3237: //Burnout V
		return 70;
	case 3238: //Destroy Summoned
		return 111;
	case 3239: //Planar Renewal
		return 42;
	case 3240: //Speed of Vallon
		return 41;
	case 3241: //Night`s Dark Terror
		return 16;
	case 3242: //Guard of Druzzil
		return 80;
	case 3243: //Teleport
		return 64;
	case 3244: //Greater Decession
		return 64;
	case 3245: //Force of Akilae
		return 97;
	case 3246: //Shackles of Tunare
		return 83;
	case 3247: //Aura of the Crusader
		return 87;
	case 3255: //Wrath of the Wild
		return 21;
	case 3256: //Wrath of the Wild
		return 21;
	case 3257: //Wrath of the Wild
		return 21;
	case 3258: //Eldritch Rune
		return 84;
	case 3259: //Eldritch Rune
		return 84;
	case 3260: //Eldritch Rune
		return 84;
	case 3264: //Allegiant Familiar
		return 101;
	case 3265: //Servant of Ro
		return 102;
	case 3266: //Servant of Ro
		return 102;
	case 3267: //Servant of Ro
		return 102;
	case 3271: //Guardian of the Forest
		return 48;
	case 3272: //Guardian of the Forest
		return 48;
	case 3273: //Guardian of the Forest
		return 48;
	case 3274: //Virulent Paralysis
		return 83;
	case 3275: //Virulent Paralysis
		return 83;
	case 3276: //Virulent Paralysis
		return 83;
	case 3281: //Servant's Bolt
		return 38;
	case 3282: //Boastful Bellow
		return 58;
	case 3289: //Frenzy of Spirit
		return 48;
	case 3290: //Hobble of Spirits
		return 71;
	case 3291: //Paragon of Spirit
		return 59;
	case 3295: //Legacy of Bracken
		return 21;
	case 3296: //Faith
		return 46;
	case 3297: //Radiant Cure1
		return 19;
	case 3298: //Radiant Cure2
		return 19;
	case 3299: //Radiant Cure3
		return 19;
	case 3300: //Shield of the Arcane
		return 87;
	case 3301: //Force Shield
		return 84;
	case 3302: //Shield of Maelin
		return 87;
	case 3303: //Blood of Thule
		return 75;
	case 3304: //Legacy of Zek
		return 103;
	case 3305: //Rune of Death
		return 70;
	case 3306: //Saryrn's Kiss
		return 33;
	case 3308: //Death's Silence
		return 35;
	case 3309: //Embracing Darkness
		return 58;
	case 3310: //Saryrn's Companion
		return 103;
	case 3311: //Seduction of Saryrn
		return 17;
	case 3312: //Touch of Death
		return 42;
	case 3314: //Child of Bertoxxulous
		return 103;
	case 3315: //Dark Plague
		return 29;
	case 3316: //Word of Terris
		return 13;
	case 3317: //Ward of Xegony
		return 98;
	case 3318: //Firebolt of Tallon
		return 38;
	case 3319: //Sun Storm
		return 38;
	case 3320: //Servant of Marr
		return 105;
	case 3321: //Black Steel
		return 58;
	case 3322: //Child of Ro
		return 102;
	case 3323: //Maelstrom of Thunder
		return 58;
	case 3324: //Rathe's Son
		return 100;
	case 3325: //Sun Vortex
		return 38;
	case 3326: //Resistant Armor
		return 80;
	case 3327: //Tears of Ro
		return 38;
	case 3328: //Lure of Thunder
		return 58;
	case 3329: //Elemental Barrier
		return 80;
	case 3330: //Draught of Ro
		return 38;
	case 3331: //Lure of Ro
		return 38;
	case 3332: //Tears of Marr
		return 14;
	case 3333: //Telekin
		return 97;
	case 3334: //Draught of Thunder
		return 58;
	case 3335: //Agnarr's Thunder
		return 58;
	case 3336: //Draught of E`ci
		return 14;
	case 3337: //Iceflame of E`ci
		return 64;
	case 3338: //Harvest of Druzzil
		return 61;
	case 3339: //Strike of Solusek
		return 38;
	case 3341: //Apathy
		return 35;
	case 3342: //Howl of Tashan
		return 81;
	case 3343: //Rune of Zebuxoruk
		return 84;
	case 3344: //Imbue Nightmare
		return 50;
	case 3345: //Strangle
		return 58;
	case 3346: //Imbue Storm
		return 50;
	case 3347: //Beckon
		return 13;
	case 3348: //Torment of Scio
		return 58;
	case 3349: //Insanity
		return 58;
	case 3350: //Tranquility
		return 59;
	case 3351: //Uproar
		return 84;
	case 3352: //Imbue Earth
		return 50;
	case 3353: //Imbue Air
		return 50;
	case 3354: //Sleep
		return 35;
	case 3355: //Command of Druzzil
		return 13;
	case 3356: //Imbue Fire
		return 50;
	case 3357: //Imbue Water
		return 50;
	case 3358: //Bliss
		return 35;
	case 3359: //Word of Morell
		return 35;
	case 3360: //Voice of Quellious
		return 59;
	case 3361: //Silent Song of Quellious
		return 11;
	case 3362: //Rizlona's Call of Flame
		return 41;
	case 3363: //Tuyen`s Chant of the Plague
		return 29;
	case 3364: //Druzzil's Disillusionment
		return 31;
	case 3365: //Melody of Mischief
		return 88;
	case 3366: //Saryrn's Scream of Pain
		return 58;
	case 3367: //Tuyen`s Chant of Fire
		return 38;
	case 3368: //Psalm of Veeshan
		return 21;
	case 3369: //Dreams of Terris
		return 35;
	case 3370: //Tuyen`s Chant of Venom
		return 75;
	case 3371: //Call of the Banshee
		return 13;
	case 3372: //Chorus of Marr
		return 44;
	case 3373: //Tuyen`s Chant of Ice
		return 14;
	case 3374: //Warsong of Zek
		return 41;
	case 3375: //Harmony of Sound
		return 81;
	case 3376: //Lullaby of Morell
		return 35;
	case 3377: //True Spirit
		return 104;
	case 3378: //Agility of the Wrulan
		return 2;
	case 3379: //Spear of Torment
		return 75;
	case 3380: //Cloud of Grummus
		return 88;
	case 3381: //Ancestral Guard
		return 6;
	case 3382: //Endurance of the Boar
		return 94;
	case 3383: //Talisman of the Wrulan
		return 2;
	case 3384: //Talisman of the Tribunal
		return 80;
	case 3385: //Tears of Saryrn
		return 75;
	case 3386: //Malicious Decay
		return 81;
	case 3387: //Malosinia
		return 81;
	case 3388: //Strength of the Diaku
		return 96;
	case 3389: //Talisman of the Boar
		return 94;
	case 3390: //Velium Strike
		return 14;
	case 3391: //Talisman of Alacrity
		return 41;
	case 3392: //Talisman of the Diaku
		return 96;
	case 3393: //Tiny Terror
		return 64;
	case 3394: //Breath of Ultor
		return 29;
	case 3395: //Malos
		return 81;
	case 3396: //Blood of Saryrn
		return 75;
	case 3397: //Focus of the Seventh
		return 87;
	case 3398: //Quiescence
		return 32;
	case 3399: //Ferine Avatar
		return 7;
	case 3400: //Festering Darkness
		return 58;
	case 3401: //Touch of Volatis
		return 43;
	case 3403: //Aura of Pain
		return 76;
	case 3405: //Terror of Thule
		return 53;
	case 3406: //Aura of Darkness
		return 76;
	case 3408: //Zevfeer's Bite
		return 33;
	case 3410: //Voice of Thule
		return 128;
	case 3411: //Aura of Hate
		return 76;
	case 3413: //Touch of Innoruuk
		return 43;
	case 3415: //Nature's Rebuke
		return 16;
	case 3416: //Nature's Rebuke Strike
		return 111;
	case 3417: //Spirit of the Predator
		return 7;
	case 3418: //Frozen Wind
		return 14;
	case 3419: //Call of the Rathe
		return 21;
	case 3420: //Cry of Thunder
		return 16;
	case 3421: //Cry of Thunder Strike
		return 58;
	case 3422: //Ward of Nife
		return 16;
	case 3423: //Ward of Nife Strike
		return 124;
	case 3424: //Pious Might
		return 16;
	case 3425: //Pious Might Strike
		return 58;
	case 3426: //Quellious' Word of Serenity
		return 97;
	case 3427: //Wave of Marr
		return 42;
	case 3428: //Deny Undead
		return 124;
	case 3429: //Touch of Nife
		return 42;
	case 3430: //Light of Nife
		return 77;
	case 3431: //Brushfire
		return 38;
	case 3432: //Brell's Stalwart Shield
		return 47;
	case 3433: //Replenishment
		return 43;
	case 3434: //Storm's Fury
		return 58;
	case 3435: //Hand of Ro
		return 81;
	case 3436: //Winter's Storm
		return 14;
	case 3437: //Immolation of Ro
		return 38;
	case 3438: //Karana's Rage
		return 58;
	case 3439: //Nature's Might
		return 96;
	case 3440: //Ro's Illumination
		return 81;
	case 3441: //Blessing of Replenishment
		return 43;
	case 3442: //E'ci's Frosty Breath
		return 81;
	case 3443: //Nature's Infusion
		return 42;
	case 3444: //Protection of Seasons
		return 80;
	case 3445: //Command of Tunare
		return 13;
	case 3446: //Swarming Death
		return 58;
	case 3447: //Savage Roots
		return 83;
	case 3448: //Shield of Bracken
		return 21;
	case 3449: //Summer's Flame
		return 38;
	case 3450: //Brackencoat
		return 21;
	case 3451: //Blessing of the Nine
		return 46;
	case 3452: //Winter's Frost
		return 14;
	case 3453: //Mask of the Forest
		return 59;
	case 3454: //Infusion of Spirit
		return 96;
	case 3455: //Healing of Sorsha
		return 42;
	case 3456: //Spiritual Vigor
		return 47;
	case 3457: //Spirit of Arag
		return 104;
	case 3458: //Arag`s Celerity
		return 70;
	case 3459: //Spirit of Rellic
		return 71;
	case 3460: //Spiritual Dominion
		return 44;
	case 3461: //Spirit of Sorsha
		return 104;
	case 3462: //Sha's Revenge
		return 88;
	case 3463: //Ferocity
		return 7;
	case 3464: //The Silent Command
		return 97;
	case 3465: //Supernal Remedy
		return 77;
	case 3466: //Symbol of Kazad
		return 112;
	case 3467: //Virtue
		return 1;
	case 3468: //Destroy Undead
		return 124;
	case 3469: //Mark of Kings
		return 64;
	case 3470: //Ward of Gallantry
		return 6;
	case 3471: //Word of Replenishment
		return 42;
	case 3472: //Blessing of Reverence
		return 91;
	case 3473: //Catastrophe
		return 58;
	case 3474: //Armor of the Zealot
		return 87;
	case 3475: //Supernal Elixir
		return 32;
	case 3476: //Condemnation
		return 58;
	case 3477: //Mark of the Righteous
		return 21;
	case 3478: //Hammer of Damnation
		return 110;
	case 3479: //Hand of Virtue
		return 1;
	case 3480: //Supernal Light
		return 42;
	case 3481: //Tarnation
		return 97;
	case 3482: //Sound of Might
		return 97;
	case 3483: //Elemental Silence
		return 35;
	case 3484: //Call of the Arch Mage
		return 13;
	case 3485: //Supernal Cleansing
		return 32;
	case 3486: //Maelstrom of Ro
		return 21;
	case 3487: //Strength of Tunare
		return 47;
	case 3488: //Pact of Hate
		return 17;
	case 3489: //Blood of Hate
		return 75;
	case 3490: //Cloak of Luclin
		return 87;
	case 3491: //Spear of Decay
		return 29;
	case 3492: //Scorpion Venom
		return 75;
	case 3493: //Frost Spear
		return 14;
	case 3494: //Luggald Blood
		return 109;
	case 3498: //Gallenite's Lifetap Test
		return 58;
	case 3560: //Spear of Pain
		return 29;
	case 3561: //Spear of Disease
		return 29;
	case 3562: //Spear of Plague
		return 29;
	case 3564: //Burning Arrow
		return 38;
	case 3565: //Flaming Arrow
		return 38;
	case 3566: //Tuyen`s Chant of Poison
		return 75;
	case 3567: //Tuyen`s Chant of Disease
		return 29;
	case 3568: //Ice Spear
		return 14;
	case 3569: //Frost Shard
		return 14;
	case 3570: //Ice Shard
		return 14;
	case 3571: //Torbas' Poison Blast
		return 75;
	case 3572: //Torbas' Venom Blast
		return 75;
	case 3573: //Shock of Venom
		return 75;
	case 3574: //Blast of Venom
		return 75;
	case 3575: //Blessing of Piety
		return 91;
	case 3576: //Blessing of Faith
		return 91;
	case 3577: //Wave of Life
		return 42;
	case 3578: //Brell's Steadfast Aegis
		return 47;
	case 3579: //Share Form of the Great Wolf
		return 65;
	case 3580: //Spirit of Ash
		return 48;
	case 3581: //O`Keils Levity
		return 55;
	case 3582: //Elemental Cloak
		return 80;
	case 3583: //Tiny Companion
		return 64;
	case 3584: //Refresh Summoning
		return 42;
	case 3585: //Entrancing Lights
		return 35;
	case 3586: //Illusion: Scaled Wolf
		return 65;
	case 3587: //Planar Strike
		return 58;
	case 3589: //Ethereal Strike
		return 58;
	case 3591: //Imbue Disease
		return 50;
	case 3592: //Imbue Valor
		return 50;
	case 3593: //Imbue War
		return 50;
	case 3594: //Imbue Torment
		return 50;
	case 3595: //Imbue Justice
		return 50;
	case 3601: //Harmony of Nature
		return 11;
	case 3618: //Eclipse Aura Strike
		return 58;
	case 3619: //Eclipse Aura Strike
		return 58;
	case 3621: //Frost Claw
		return 14;
	case 3623: //Burning Barb
		return 38;
	case 3624: //Anger
		return 0;
	case 3626: //Tendrils of Fire
		return 38;
	case 3630: //Time Lapse
		return 58;
	case 3645: //Sting of Ayonae
		return 58;
	case 3646: //Bite of Bertoxxulous
		return 29;
	case 3648: //Time Snap
		return 58;
	case 3650: //Dark Empathy Recourse
		return 0;
	case 3651: //Wind of Marr
		return 44;
	case 3665: //Curtain Call
		return 38;
	case 3668: //Pawn's Plight
		return 29;
	case 3670: //Queen's Checkmate
		return 14;
	case 3681: //Aria of Innocence
		return 19;
	case 3682: //Aria of Asceticism
		return 19;
	case 3683: //Ethereal Cleansing
		return 32;
	case 3684: //Light of Life
		return 77;
	case 3685: //Comatose
		return 64;
	case 3686: //Blood of Pain
		return 75;
	case 3687: //Swarm of Pain
		return 58;
	case 3688: //Icewind
		return 14;
	case 3689: //Malaria
		return 29;
	case 3690: //Bond of the Wild
		return 70;
	case 3691: //Bond of the Wild R.
		return 0;
	case 3692: //Temperance
		return 1;
	case 3693: //Pure Blood
		return 19;
	case 3694: //Stoicism
		return 32;
	case 3695: //Frost Zephyr
		return 81;
	case 3696: //Leviathan Eyes
		return 129;
	case 3697: //Scryer's Trespass
		return 60;
	case 3699: //Primal Remedy
		return 42;
	case 3700: //Elemental Empathy
		return 70;
	case 3702: //Auspice
		return 33;
	case 3704: //Soul Empathy
		return 0;
	case 3706: //Frozen Harpoon
		return 14;
	case 3748: //Insipid Decay
		return 29;
	case 3753: //Torrent of Agony
		return 76;
	case 3764: //Rain of Bile
		return 75;
	case 3785: //Hate's Fury
		return 58;
	case 3792: //Circle of Stonebrunt
		return 67;
	case 3793: //Stonebrunt Portal
		return 67;
	case 3794: //Ring of Stonebrunt
		return 67;
	case 3795: //Stonebrunt Gate
		return 67;
	case 3796: //Mind Tap
		return 60;
	case 3799: //Dismal Wind
		return 97;
	case 3803: //Pique
		return 58;
	case 3806: //Distraction
		return 53;
	case 3809: //Reclamation
		return 43;
	case 3810: //Eruption
		return 38;
	case 3811: //Vision Shift
		return 129;
	case 3833: //Translocate: Stonebrunt
		return 67;
	case 3834: //Healing Water
		return 42;
	case 3842: //Blood of Nadox
		return 19;
	case 3847: //Cloak of Khala Dun
		return 48;
	case 3848: //Tortured Memory II
		return 38;
	case 3849: //Alter Plane: Hate II
		return 116;
	case 3854: //Form of Protection
		return 48;
	case 3855: //Form of Defense
		return 48;
	case 3856: //Form of Endurance
		return 48;
	case 3857: //Form of Rejuvenation
		return 48;
	case 3861: //Pestilence Shock
		return 16;
	case 3864: //Soul Claw
		return 16;
	case 3876: //Frozen Shards
		return 14;
	case 3877: //Nightmares
		return 58;
	case 3878: //Time Rend
		return 58;
	case 3881: //Hand of Retribution
		return 0;
	case 3909: //Clinging Clay
		return 88;
	case 3910: //Flames of Condemnation
		return 38;
	case 3921: //Guide Evacuation
		return 64;
	case 3975: //Force of Akera
		return 97;
	case 3976: //Draught of Lightning
		return 58;
	case 3981: //Mass Clarify Mana
		return 64;
	case 3982: //Mass Crystallize Mana
		return 64;
	case 3983: //Mass Distill Mana
		return 64;
	case 3984: //Mass Enchant Adamantite
		return 34;
	case 3985: //Mass Enchant Brellium
		return 34;
	case 3986: //Mass Enchant Clay
		return 34;
	case 3987: //Mass Enchant Electrum
		return 34;
	case 3988: //Mass Enchant Gold
		return 34;
	case 3989: //Mass Enchant Mithril
		return 34;
	case 3990: //Mass Enchant Platinum
		return 34;
	case 3991: //Mass Enchant Silver
		return 34;
	case 3992: //Mass Enchant Steel
		return 34;
	case 3993: //Mass Enchant Velium
		return 34;
	case 3994: //Mass Imbue Amber
		return 50;
	case 3995: //Mass Imbue Black Pearl
		return 50;
	case 3996: //Mass Imbue Black Sapphire
		return 50;
	case 3997: //Mass Imbue Diamond
		return 50;
	case 3998: //Mass Imbue Emerald
		return 50;
	case 3999: //Mass Imbue Fire Opal
		return 50;
	case 4000: //Mass Imbue Ivory
		return 50;
	case 4001: //Mass Imbue Jade
		return 50;
	case 4002: //Mass Imbue Opal
		return 50;
	case 4003: //Mass Imbue Peridot
		return 50;
	case 4004: //Mass Imbue Plains Pebble
		return 50;
	case 4005: //Mass Imbue Rose Quartz
		return 50;
	case 4006: //Mass Imbue Ruby
		return 50;
	case 4007: //Mass Imbue Sapphire
		return 50;
	case 4008: //Mass Imbue Topaz
		return 50;
	case 4009: //Mass Purify Mana
		return 64;
	case 4010: //Mass Thicken Mana
		return 64;
	case 4011: //Kindle
		return 70;
	case 4017: //Illusion: Guktan
		return 49;
	case 4018: //RytanGuard1
		return 10;
	case 4019: //RytanGuard2
		return 10;
	case 4020: //RytanGuard3
		return 10;
	case 4021: //RytanGuard4
		return 10;
	case 4022: //RytanBoltTest
		return 38;
	case 4027: //Summon Wooden Bracelet
		return 107;
	case 4028: //Summon Stone Bracelet
		return 107;
	case 4029: //Summon Iron Bracelet
		return 107;
	case 4030: //Summon Steel Bracelet
		return 107;
	case 4049: //Circle of Cooling
		return 80;
	case 4050: //Circle of Warmth
		return 80;
	case 4051: //Talisman of Purity
		return 80;
	case 4052: //Talisman of Vitality
		return 80;
	case 4053: //Blessing of Temperance
		return 1;
	case 4054: //Spirit of the Shrew
		return 65;
	case 4055: //Pack Shrew
		return 65;
	case 4056: //Remove Minor Curse
		return 19;
	case 4057: //Remove Lesser Curse
		return 19;
	case 4058: //Feral Pack
		return 65;
	case 4059: //Call of Ice
		return 16;
	case 4062: //Dark Temptation
		return 17;
	case 4063: //Call of Darkness
		return 17;
	case 4064: //Austerity
		return 1;
	case 4065: //Blessing of Austerity
		return 1;
	case 4066: //Ice Meteor
		return 14;
	case 4067: //Ward of Calrena
		return 92;
	case 4068: //Guard of Calrena
		return 92;
	case 4069: //Protection of Calrena
		return 92;
	case 4070: //Magi Ward
		return 92;
	case 4071: //Mana Ward
		return 92;
	case 4072: //Deception
		return 49;
	case 4073: //Ward of Alendar
		return 92;
	case 4074: //Guard of Alendar
		return 92;
	case 4075: //Protection of Alendar
		return 92;
	case 4076: //Bulwark of Alendar
		return 92;
	case 4077: //Ordinance
		return 13;
	case 4078: //Wind of the Desert
		return 38;
	case 4079: //Ward of Calliav
		return 10;
	case 4080: //Guard of Calliav
		return 10;
	case 4081: //Protection of Calliav
		return 10;
	case 4082: //Summon: Orb of Exploration
		return 109;
	case 4083: //Rizlona's Embers
		return 91;
	case 4084: //Rizlona's Fire
		return 91;
	case 4085: //Forpar's Aria of Affliction
		return 91;
	case 4086: //Forpar's Psalm of Pain
		return 91;
	case 4087: //Forpar's Verse of Venom
		return 91;
	case 4088: //Ward of Vie
		return 62;
	case 4089: //Guard of Vie
		return 62;
	case 4090: //Protection of Vie
		return 62;
	case 4091: //Bulwark of Vie
		return 62;
	case 4092: //Curse
		return 58;
	case 4093: //Odium
		return 58;
	case 4094: //Anathema
		return 58;
	case 4095: //Bane
		return 58;
	case 4096: //Dark Soul
		return 58;
	case 4097: //Imprecation
		return 58;
	case 4098: //Horror
		return 58;
	case 4099: //Bounce
		return 78;
	case 4100: //Reflect
		return 78;
	case 4101: //Scythe of Innoruuk
		return 124;
	case 4102: //Scythe of Darkness
		return 124;
	case 4103: //Scythe of Death
		return 124;
	case 4104: //Vengeance of the Wild
		return 38;
	case 4105: //Vengeance of Nature
		return 38;
	case 4106: //Vengeance of Tunare
		return 38;
	case 4107: //Feral Form
		return 65;
	case 4108: //Aura of Reverence
		return 91;
	case 4109: //Guidance
		return 1;
	case 4110: //Burning Sand
		return 38;
	case 4111: //Fire Swarm
		return 58;
	case 4112: //Call of the Muse
		return 91;
	case 4210: //Fufil`s Diminishing Dirge
		return 58;
	case 4239: //Breathless Mist
		return 64;
	case 4240: //Essence of Concealment
		return 51;
	case 4241: //Weightless Mist
		return 55;
	case 4242: //Mist of the Wolf
		return 65;
	case 4252: //Xalirilan's Lesser Appraisal
		return 85;
	case 4253: //Xalirilan's Appraisal
		return 85;
	case 4254: //Xalirilan's Greater Appraisal
		return 85;
	case 4255: //Wuggan's Lesser Appraisal
		return 85;
	case 4256: //Wuggan's Appraisal
		return 85;
	case 4257: //Wuggan's Greater Appraisal
		return 85;
	case 4258: //Iony's Lesser Augury
		return 85;
	case 4259: //Iony's Augury
		return 85;
	case 4260: //Iony's Greater Augury
		return 85;
	case 4261: //Reebo's Lesser Augury
		return 85;
	case 4262: //Reebo's Augury
		return 85;
	case 4263: //Reebo's Greater Augury
		return 85;
	case 4264: //Xalirilan's Lesser Discombobulation
		return 26;
	case 4265: //Xalirilan's Discombobulation
		return 26;
	case 4266: //Xalirilan's Greater Discombobulation
		return 26;
	case 4267: //Wuggan's Lesser Discombobulation
		return 26;
	case 4268: //Wuggan's Discombobulation
		return 26;
	case 4269: //Wuggan's Greater Discombobulation
		return 26;
	case 4270: //Iony's Lesser Exorcism
		return 26;
	case 4271: //Iony's Exorcism
		return 26;
	case 4272: //Iony's Greater Exorcism
		return 26;
	case 4273: //Reebo's Lesser Exorcism
		return 26;
	case 4274: //Reebo's Exorcism
		return 26;
	case 4275: //Reebo's Greater Exorcism
		return 26;
	case 4276: //Xalirilan's Lesser Extrication
		return 73;
	case 4277: //Xalirilan's Extrication
		return 73;
	case 4278: //Xalirilan's Greater Extrication
		return 73;
	case 4279: //Wuggan's Lesser Extrication
		return 73;
	case 4280: //Wuggan's Extrication
		return 73;
	case 4281: //Wuggan's Greater Extrication
		return 73;
	case 4282: //Iony's Lesser Cleansing
		return 73;
	case 4283: //Iony's Cleansing
		return 73;
	case 4284: //Iony's Greater Cleansing
		return 73;
	case 4285: //Reebo's Lesser Cleansing
		return 73;
	case 4286: //Reebo's Cleansing
		return 73;
	case 4287: //Reebo's Greater Cleansing
		return 73;
	case 4291: //Aura of Quellious
		return 59;
	case 4350: //Transmute Hunter's Dagger
		return 110;
	case 4351: //Transmute Hunter's Barbs
		return 110;
	case 4352: //Transmute Wayfarer's Bread
		return 110;
	case 4353: //Transmute Wayfarer's Tonic
		return 110;
	case 4354: //Transmute Traveler's Bandage
		return 110;
	case 4355: //Transmute Wayfarer's Wine
		return 110;
	case 4356: //Bite of Ykesha
		return 58;
	case 4357: //Strike of Ykesha
		return 58;
	case 4358: //Force of Ykesha
		return 58;
	case 4359: //Wrath of Ykesha
		return 58;
	case 4360: //Rujarkian Breath
		return 58;
	case 4361: //Rujarkian Mist
		return 58;
	case 4362: //Rujarkian Poison
		return 58;
	case 4363: //Rujarkian Bile
		return 58;
	case 4364: //Rujarkian Venom
		return 58;
	case 4365: //Heated Blade
		return 58;
	case 4366: //Burning Blade
		return 58;
	case 4367: //Blazing Blade
		return 58;
	case 4368: //Flaming Blade
		return 58;
	case 4369: //Inferno Blade
		return 58;
	case 4370: //Vampire Touch
		return 58;
	case 4371: //Vampire Claw
		return 58;
	case 4372: //Vampire Talon
		return 58;
	case 4373: //Vampire Fangs
		return 58;
	case 4374: //Vampire Kiss
		return 58;
	case 4375: //Chill
		return 58;
	case 4376: //Icicle Strike
		return 58;
	case 4377: //Icicle Claw
		return 58;
	case 4378: //Vox' Bite
		return 58;
	case 4379: //Permafrost
		return 58;
	case 4380: //Mirror I
		return 78;
	case 4381: //Mirror II
		return 78;
	case 4382: //Mirror III
		return 78;
	case 4383: //Mirror IV
		return 78;
	case 4384: //Guard I
		return 62;
	case 4385: //Guard II
		return 62;
	case 4386: //Guard III
		return 62;
	case 4387: //Guard IV
		return 62;
	case 4388: //Spell Guard I
		return 62;
	case 4389: //Spell Guard II
		return 62;
	case 4390: //Spell Guard III
		return 62;
	case 4391: //Spell Guard IV
		return 62;
	case 4395: //Selo's Rhythm of Speed
		return 65;
	case 4408: //Color Cloud
		return 97;
	case 4413: //Golem Pulverize
		return 58;
	case 4414: //Rimebone Frost Burst
		return 14;
	case 4418: //Illusion: Frost Bone
		return 48;
	case 4489: //Taelosian Vengeance
		return 88;
	case 4492: //Geostrike
		return 14;
	case 4493: //Earth Wave
		return 38;
	case 4496: //Rock Storm
		return 38;
	case 4497: //Earth Shards
		return 14;
	case 4498: //Aggressive Discipline
		return 119;
	case 4499: //Defensive Discipline
		return 117;
	case 4500: //Holyforge Discipline
		return 117;
	case 4501: //Precision Discipline
		return 119;
	case 4502: //Voiddance Discipline
		return 117;
	case 4503: //Evasive Discipline
		return 117;
	case 4504: //Leechcurse Discipline
		return 118;
	case 4505: //Deadeye Discipline
		return 119;
	case 4506: //Trueshot Discipline
		return 117;
	case 4507: //Silentfist Discipline
		return 119;
	case 4508: //Ashenhand Discipline
		return 119;
	case 4509: //Whirlwind Discipline
		return 117;
	case 4510: //Stonestance Discipline
		return 117;
	case 4511: //Thunderkick Discipline
		return 119;
	case 4512: //Innerflame Discipline
		return 118;
	case 4513: //Hundred Fists Discipline
		return 118;
	case 4514: //Mighty Strike Discipline
		return 119;
	case 4515: //Nimble Discipline
		return 117;
	case 4516: //Deftdance Discipline
		return 117;
	case 4517: //Kinesthetics Discipline
		return 118;
	case 4518: //Sanctification Discipline
		return 118;
	case 4519: //Weapon Shield Discipline
		return 118;
	case 4520: //Unholy Aura Discipline
		return 117;
	case 4521: //Bestial Alignment I
		return 7;
	case 4522: //Bestial Alignment II
		return 7;
	case 4523: //Bestial Alignment III
		return 7;
	case 4524: //Bestial Alignment I
		return 7;
	case 4525: //Bestial Alignment II
		return 7;
	case 4526: //Bestial Alignment III
		return 7;
	case 4527: //Bestial Alignment I
		return 7;
	case 4528: //Bestial Alignment II
		return 7;
	case 4529: //Bestial Alignment III
		return 7;
	case 4530: //Bestial Alignment I
		return 7;
	case 4531: //Bestial Alignment II
		return 7;
	case 4532: //Bestial Alignment III
		return 7;
	case 4533: //Bestial Alignment I
		return 7;
	case 4534: //Bestial Alignment II
		return 7;
	case 4535: //Bestial Alignment III
		return 7;
	case 4536: //Frenzied Aura
		return 48;
	case 4537: //Icy Grasp
		return 71;
	case 4538: //Icy Grasp Strike
		return 58;
	case 4549: //Divine Avatar I
		return 7;
	case 4550: //Divine Avatar II
		return 7;
	case 4551: //Divine Avatar III
		return 7;
	case 4555: //Elemental Domination
		return 13;
	case 4567: //Aneuk Grasp
		return 58;
	case 4574: //Hynid Snap
		return 58;
	case 4578: //Turepta Crush
		return 58;
	case 4579: //Ukun Chains
		return 58;
	case 4585: //Resistant Discipline
		return 120;
	case 4586: //Puretone Discipline
		return 118;
	case 4587: //Fearless Discipline
		return 120;
	case 4588: //Infuriate
		return 68;
	case 4589: //Barrier
		return 22;
	case 4590: //Cover
		return 22;
	case 4591: //Guard
		return 22;
	case 4592: //Infallible
		return 22;
	case 4593: //Crippling Strike
		return 90;
	case 4595: //Muscle Shock
		return 68;
	case 4596: //Armor Slice
		return 90;
	case 4597: //Gauntlet Strike
		return 68;
	case 4598: //Head Bash
		return 68;
	case 4599: //Rally Cry
		return 40;
	case 4600: //Shin Kick
		return 68;
	case 4601: //Rage
		return 40;
	case 4602: //Power Slam
		return 90;
	case 4603: //Stomp
		return 68;
	case 4604: //Back Swing
		return 68;
	case 4605: //Slice
		return 68;
	case 4607: //Flurry
		return 68;
	case 4608: //Provoke
		return 115;
	case 4612: //Enrage
		return 118;
	case 4614: //Phantom Zephyr
		return 115;
	case 4615: //Fortitude
		return 22;
	case 4616: //Pain Tolerance
		return 22;
	case 4618: //Fortune
		return 90;
	case 4619: //Quick Feet
		return 22;
	case 4620: //Ton Po's Defense
		return 22;
	case 4621: //Focused Aura
		return 22;
	case 4622: //Overwhelm
		return 68;
	case 4623: //Tranquil Force
		return 22;
	case 4624: //Grapple
		return 90;
	case 4625: //Armor Crush
		return 90;
	case 4626: //Leg Sweep
		return 68;
	case 4627: //Nerve Strike
		return 90;
	case 4628: //Nerve Spasm
		return 90;
	case 4629: //Thunderkick
		return 68;
	case 4630: //Master's Fury
		return 40;
	case 4631: //Ashenhand
		return 68;
	case 4632: //Aura of Speed
		return 40;
	case 4633: //Whirlwind Kick
		return 68;
	case 4634: //Dragon Strike
		return 68;
	case 4635: //Tranquil Focus
		return 90;
	case 4636: //Rapid Jab
		return 68;
	case 4637: //Wind of Force
		return 68;
	case 4638: //Pain Strike
		return 90;
	case 4639: //Indirection
		return 90;
	case 4640: //Focus
		return 90;
	case 4641: //Reflexes
		return 22;
	case 4642: //Mental Block
		return 22;
	case 4643: //Proficiency
		return 68;
	case 4644: //Bind
		return 90;
	case 4645: //Armor Pierce
		return 90;
	case 4646: //Eye Gouge
		return 90;
	case 4647: //Tendon Slice
		return 68;
	case 4648: //Wrist Slice
		return 90;
	case 4649: //Assassin's Focus
		return 40;
	case 4650: //Lunge
		return 68;
	case 4651: //Direct Assault
		return 40;
	case 4652: //Vital Cut
		return 68;
	case 4653: //Blood Feast
		return 40;
	case 4654: //Blood Slice
		return 68;
	case 4655: //Energy Sap
		return 68;
	case 4656: //Mind Snap
		return 90;
	case 4657: //Burning Spasm
		return 90;
	case 4658: //Double Stab
		return 68;
	case 4659: //Sneak Attack
		return 115;
	case 4670: //Fortitude Discipline
		return 118;
	case 4671: //Protective Spirit Discipline
		return 117;
	case 4672: //Charge Discipline
		return 119;
	case 4673: //Counterattack Discipline
		return 117;
	case 4674: //Furious Discipline
		return 118;
	case 4675: //Fellstrike Discipline
		return 119;
	case 4676: //Duelist Discipline
		return 118;
	case 4677: //Blinding Speed Discipline
		return 119;
	case 4678: //Bestial Fury Discipline
		return 118;
	case 4679: //Energy Sap Recourse
		return 68;
	case 4680: //Cover Recourse
		return 22;
	case 4681: //Bellow
		return 115;
	case 4682: //Berate
		return 115;
	case 4683: //Phantom Wind
		return 115;
	case 4684: //Phantom Echo
		return 115;
	case 4685: //Thief's Vengeance
		return 115;
	case 4686: //Assassin's Strike
		return 115;
	case 4687: //Healing Will Discipline
		return 120;
	case 4688: //Stonewall Discipline
		return 117;
	case 4689: //Spirit of Rage Discipline
		return 120;
	case 4690: //Earthwalk Discipline
		return 117;
	case 4691: //Speed Focus Discipline
		return 118;
	case 4692: //Planeswalk Discipline
		return 120;
	case 4693: //Concentration Discipline
		return 117;
	case 4694: //Deadly Precision Discipline
		return 119;
	case 4695: //Twisted Chance Discipline
		return 118;
	case 4696: //Weapon Affinity Discipline
		return 120;
	case 4697: //Incite
		return 115;
	case 4698: //Phantom Call
		return 115;
	case 4699: //Bite of the Hounds
		return 58;
	case 4700: //Claw of the Beast
		return 58;
	case 4701: //Warhound's Affliction
		return 58;
	case 4704: //Blood Scream
		return 19;
	case 4706: //Mindless Rage
		return 40;
	case 4716: //Call of Rav
		return 13;
	case 4717: //Diseased Maw
		return 88;
	case 4721: //Focused Will Discipline
		return 120;
	case 4724: //Abysmal Replenishment
		return 44;
	case 4726: //Bite of Keras
		return 58;
	case 4741: //Reflection of Discord I
		return 78;
	case 4742: //Reflection of Discord II
		return 78;
	case 4743: //Reflection of Discord III
		return 78;
	case 4786: //Icy Grasp
		return 71;
	case 4788: //Feral Swipe
		return 68;
	case 4789: //Touch of the Divine
		return 52;
	case 4795: //Aura of Restoration
		return 43;
	case 4801: //Doppelganger Recourse
		return 86;
	case 4802: //Flames of Kesh`yk I
		return 71;
	case 4803: //Flames of Kesh`yk II
		return 71;
	case 4804: //Flames of Kesh`yk III
		return 71;
	case 4805: //Frost of Kesh`yk I
		return 71;
	case 4806: //Frost of Kesh`yk II
		return 71;
	case 4807: //Frost of Kesh`yk III
		return 71;
	case 4808: //Lightning of Kesh`yk I
		return 71;
	case 4809: //Lightning of Kesh`yk II
		return 71;
	case 4810: //Lightning of Kesh`yk III
		return 71;
	case 4811: //Flames of Kesh`yk Effect I
		return 71;
	case 4812: //Flames of Kesh`yk Effect II
		return 71;
	case 4813: //Flames of Kesh`yk Effect III
		return 71;
	case 4814: //Frost of Kesh`yk Effect I
		return 71;
	case 4815: //Frost of Kesh`yk Effect II
		return 71;
	case 4816: //Frost of Kesh`yk Effect III
		return 71;
	case 4817: //Lightning of Kesh`yk Effect I
		return 71;
	case 4818: //Lightning of Kesh`yk Effect II
		return 71;
	case 4819: //Lightning of Kesh`yk Effect III
		return 71;
	case 4820: //Rabid Companion I
		return 71;
	case 4821: //Rabid Companion II
		return 71;
	case 4822: //Rabid Companion III
		return 71;
	case 4841: //Aura of Fire
		return 43;
	case 4842: //Exultant Bellow I
		return 58;
	case 4843: //Exultant Bellow II
		return 58;
	case 4844: //Exultant Bellow III
		return 58;
	case 4845: //Exultant Bellow IV
		return 58;
	case 4846: //Exultant Bellow V
		return 58;
	case 4849: //Heartstopper
		return 88;
	case 4853: //Listless Strength
		return 30;
	case 4871: //War March of the Mastruq
		return 41;
	case 4872: //Echo of the Trusik
		return 91;
	case 4873: //Dark Echo
		return 78;
	case 4874: //Turepta Blood
		return 75;
	case 4875: //Trushar's Mending
		return 42;
	case 4876: //Trushar's Frost
		return 14;
	case 4877: //Apathy of the Nihil
		return 35;
	case 4878: //Bliss of the Nihil
		return 35;
	case 4879: //Madness of Ikkibi
		return 58;
	case 4880: //Holy Light
		return 42;
	case 4881: //Order
		return 58;
	case 4882: //Holy Elixir
		return 32;
	case 4883: //Sylvan Infusion
		return 42;
	case 4884: //Sylvan Fire
		return 38;
	case 4885: //Sylvan Embers
		return 38;
	case 4886: //Elemental Siphon
		return 17;
	case 4887: //Rock of Taelosia
		return 58;
	case 4888: //Monster Summoning IV
		return 64;
	case 4889: //Night Stalker
		return 33;
	case 4890: //Night Fire
		return 38;
	case 4891: //Night's Beckon
		return 33;
	case 4893: //Wave of Trushar
		return 42;
	case 4894: //Light of Order
		return 77;
	case 4895: //Holy Order
		return 16;
	case 4896: //Sylvan Light
		return 42;
	case 4897: //Sylvan Burn
		return 38;
	case 4898: //Sylvan Call
		return 16;
	case 4899: //Breath of Trushar
		return 32;
	case 4900: //Balance of the Nihil
		return 88;
	case 4901: //Daluda's Mending
		return 42;
	case 4902: //Mental Horror
		return 16;
	case 4903: //Black Shroud
		return 16;
	case 4904: //Miasmic spear
		return 29;
	case 4905: //Black Ice
		return 14;
	case 4906: //White Fire
		return 38;
	case 4907: //Telaka
		return 97;
	case 4908: //Mental Horror Strike
		return 29;
	case 4912: //Sylvan Call Strike
		return 38;
	case 4913: //Holy Order Strike
		return 58;
	case 4928: //Leg Strike
		return 89;
	case 4929: //Leg Cut
		return 89;
	case 4930: //Leg Slice
		return 89;
	case 4931: //Head Strike
		return 97;
	case 4932: //Head Pummel
		return 97;
	case 4933: //Head Crush
		return 97;
	case 4934: //Diversive Strike
		return 53;
	case 4935: //Distracting Strike
		return 53;
	case 4936: //Confusing Strike
		return 53;
	case 4937: //Corroded Axe
		return 110;
	case 4938: //Blunt Axe
		return 110;
	case 4939: //Steel Axe
		return 110;
	case 4940: //Bearded Axe
		return 110;
	case 4941: //Mithril Axe
		return 110;
	case 4942: //Balanced War Axe
		return 110;
	case 4943: //Bonesplicer Axe
		return 110;
	case 4944: //Fleshtear Axe
		return 110;
	case 4945: //Cold Steel Cleaving Axe
		return 110;
	case 4946: //Mithril Bloodaxe
		return 110;
	case 4947: //Rage Axe
		return 110;
	case 4948: //Bloodseekers Axe
		return 110;
	case 4949: //Battlerage Axe
		return 110;
	case 4950: //Deathfury Axe
		return 110;
	case 4957: //Shock of Discord
		return 16;
	case 4960: //Simmering Rage
		return 115;
	case 4961: //Bubbling Rage
		return 115;
	case 4962: //Boiling Rage
		return 115;
	case 4963: //Natimbi Gate
		return 113;
	case 4964: //Translocate: Natimbi
		return 113;
	case 4965: //Natimbi Portal
		return 113;
	case 4966: //Circle of Natimbi
		return 113;
	case 4967: //Ring of Natimbi
		return 113;
	case 4968: //Dark Arrow
		return 38;
	case 4970: //Prism Strike
		return 14;
	case 4971: //Ancient: Chaos Chant
		return 29;
	case 4972: //Ancient: Frozen Chaos
		return 14;
	case 4973: //Ancient: Chaos Censure
		return 58;
	case 4974: //Ancient: Chaos Frost
		return 14;
	case 4975: //Ancient: Chaos Madness
		return 58;
	case 4976: //Ancient: Chaos Vortex
		return 38;
	case 4977: //Ancient: Force of Chaos
		return 97;
	case 4978: //Ancient: Seduction of Chaos
		return 17;
	case 4979: //Ancient: Chaotic Pain
		return 17;
	case 4980: //Ancient: Burning Chaos
		return 38;
	case 4981: //Ancient: Strike of Chaos
		return 38;
	case 4982: //Ancient: Bite of Chaos
		return 33;
	case 4991: //Coordinated Strike
		return 0;
	case 4992: //Malevolent Assault
		return 58;
	case 4993: //Malevolent Vex
		return 0;
	case 4994: //Searing Blood Arrow I
		return 88;
	case 4995: //Searing Blood Arrow II
		return 88;
	case 4996: //Searing Blood Arrow III
		return 88;
	case 4997: //Arrow of Renewal
		return 42;
	case 5000: //Righteous Assault
		return 0;
	case 5001: //Bury
		return 0;
	case 5002: //Mana Blast
		return 0;
	case 5003: //Impoverished Lifeblood
		return 0;
	case 5004: //Tamuik's Suggestion
		return 0;
	case 5005: //Tamuik's Ghastly Presence
		return 0;
	case 5006: //Tamuik's Spectral Step
		return 0;
	case 5007: //Curse of Tunik Tamuik
		return 0;
	case 5008: //Bane of Tunik Tamuik
		return 0;
	case 5009: //Unholy Barrage
		return 0;
	case 5010: //Strike of Glory
		return 0;
	case 5011: //Salve
		return 42;
	case 5012: //Spike of Disease
		return 29;
	case 5013: //SpellTheft2
		return 31;
	case 5014: //SpellTheft3
		return 31;
	case 5015: //Bellow of the Mastruq
		return 115;
	case 5016: //Ancient: Chaos Cry
		return 115;
	case 5017: //Kyv Strike
		return 115;
	case 5018: //Ancient: Chaos Strike
		return 115;
	case 5019: //Phantom Shadow
		return 115;
	case 5020: //Ancient: Phantom Chaos
		return 115;
	case 5022: //Dark Balance
		return 0;
	case 5023: //Chaos Epidemic
		return 0;
	case 5024: //Chaos Epidemic
		return 0;
	case 5027: //Battle Cry
		return 7;
	case 5028: //War Cry
		return 7;
	case 5029: //Battle Cry of Dravel
		return 7;
	case 5030: //War Cry of Dravel
		return 7;
	case 5031: //Battle Cry of the Mastruq
		return 7;
	case 5032: //Ancient: Cry of Chaos
		return 7;
	case 5033: //Berserker Rage
		return 7;
	case 5034: //Burning Rage Discipline
		return 119;
	case 5035: //Focused Fury Discipline
		return 119;
	case 5036: //Battle Sense Discipline
		return 117;
	case 5037: //Cleaving Rage Discipline
		return 119;
	case 5038: //Battle Focus Discipline
		return 118;
	case 5039: //Inspired Anger Discipline
		return 119;
	case 5040: //Reckless Discipline
		return 118;
	case 5041: //Blind Rage Discipline
		return 119;
	case 5042: //Indomitable Discipline
		return 118;
	case 5043: //Cleaving Anger Discipline
		return 118;
	case 5044: //Sprint Discipline
		return 120;
	case 5045: //Test1
		return 118;
	case 5046: //Test2
		return 115;
	case 5047: //Test3
		return 115;
	case 5048: //Test4
		return 115;
	case 5049: //Test5
		return 115;
	case 5050: //Test6
		return 118;
	case 5051: //Aura of Destruction
		return 0;
	case 5052: //Spirit's Touch
		return 0;
	case 5053: //Destructive Crush
		return 0;
	case 5054: //Wave of Destruction
		return 0;
	case 5055: //Creeping Fury
		return 0;
	case 5056: //Rampaging Force
		return 0;
	case 5057: //Barxt's Destructive Touch
		return 0;
	case 5058: //Barxt's Mental Corruption
		return 0;
	case 5059: //Wave of Absolute Power
		return 0;
	case 5060: //Discordant Light
		return 42;
	case 5063: //Mug
		return 68;
	case 5064: //Hastened Thoughts
		return 64;
	case 5070: //Armor Shatter
		return 0;
	case 5071: //Energy Siphon
		return 0;
	case 5073: //Soul Vortex
		return 0;
	case 5074: //Black Pox
		return 0;
	case 5094: //test ac
		return 118;
	case 5095: //Item Pet I
		return 99;
	case 5096: //Item Pet II
		return 99;
	case 5101: //Fire Shard
		return 64;
	case 5102: //Frost Hammer
		return 64;
	case 5103: //Flame Strike
		return 58;
	case 5104: //Frost Strike
		return 58;
	case 5105: //Geomantra
		return 92;
	case 5107: //Tainted Axe of Hatred
		return 110;
	case 5118: //Intoxicating Fury
		return 0;
	case 5119: //Force of Trusik's Rage
		return 0;
	case 5120: //Withering Destruction
		return 0;
	case 5125: //Venom Claw
		return 38;
	case 5127: //Prism Skin
		return 14;
	case 5133: //Elemental Draw
		return 17;
	case 5135: //SpellTheft4
		return 10;
	case 5148: //Massive Explosion
		return 0;
	case 5150: //Gloomingdeep Guard
		return 46;
	case 5190: //PvPSilTest1
		return 97;
	case 5191: //PvPSilTest2
		return 97;
	case 5192: //PvPSilTest3
		return 97;
	case 5193: //PvPSilTest4
		return 97;
	case 5194: //PvPSilTest5
		return 97;
	case 5195: //PvPSil2Test1
		return 97;
	case 5196: //PvPSil2Test2
		return 97;
	case 5197: //PvPSil2Test3
		return 97;
	case 5198: //PvPSil2Test4
		return 97;
	case 5199: //PvPSil2Test5
		return 97;
	case 5200: //PvPStunTest1
		return 71;
	case 5201: //PvPStunTest2
		return 71;
	case 5202: //PvPStunTest3
		return 71;
	case 5203: //PvPStunTest4
		return 71;
	case 5204: //PvPStunTest5
		return 71;
	case 5205: //5200 Strike
		return 71;
	case 5206: //5201 Strike
		return 71;
	case 5207: //5202 Strike
		return 71;
	case 5208: //5203 Strike
		return 71;
	case 5209: //5204 Strike
		return 71;
	case 5210: //PvPSnareTest1
		return 71;
	case 5211: //PvPSnareTest2
		return 71;
	case 5212: //PvPSnareTest3
		return 71;
	case 5213: //PvPSnareTest4
		return 71;
	case 5214: //PvPSnareTest5
		return 71;
	case 5215: //5210 Strike
		return 71;
	case 5216: //5211 Strike
		return 71;
	case 5217: //5212 Strike
		return 71;
	case 5218: //5213 Strike
		return 71;
	case 5219: //5214 Strike
		return 71;
	case 5220: //Jarsath Frenzy
		return 96;
	case 5221: //Rage of Xyzith
		return 58;
	case 5222: //Morternum
		return 33;
	case 5226: //Arias' Guard
		return 43;
	case 5250: //Confidence
		return 46;
	case 5251: //Pious Remedy
		return 77;
	case 5252: //Symbol of Balikor
		return 112;
	case 5253: //Ward of Valiance
		return 6;
	case 5254: //Shock of Wonder
		return 97;
	case 5255: //Sermon of Reproach
		return 124;
	case 5256: //Unswerving Hammer of Retribution
		return 99;
	case 5257: //Conviction
		return 1;
	case 5258: //Blessing of Devotion
		return 91;
	case 5259: //Pious Elixir
		return 32;
	case 5260: //Reproach
		return 58;
	case 5261: //Panoply of Vie
		return 62;
	case 5262: //Omen-Cleric-PH
		return 91;
	case 5263: //Omen-Cleric-PH
		return 64;
	case 5264: //Hammer of Reproach
		return 110;
	case 5265: //Pious Light
		return 42;
	case 5266: //Sound of Divinity
		return 97;
	case 5267: //Omen-Cleric-PH
		return 83;
	case 5268: //Desolate Undead
		return 124;
	case 5269: //Mark of the Blameless
		return 21;
	case 5270: //Word of Vivification
		return 42;
	case 5271: //Calamity
		return 58;
	case 5272: //Aura of Devotion
		return 91;
	case 5273: //Yaulp VII
		return 7;
	case 5274: //Placate
		return 11;
	case 5275: //Silent Dictation
		return 97;
	case 5276: //Armor of the Pious
		return 87;
	case 5277: //Balikor's Mark
		return 112;
	case 5278: //Hand of Conviction
		return 1;
	case 5279: //Ancient: Pious Conscience
		return 58;
	case 5280: //Direction
		return 46;
	case 5281: //Omen-Paladin-PH
		return 83;
	case 5282: //Touch of Piety
		return 42;
	case 5283: //Crusader's Purity
		return 19;
	case 5284: //Force of Piety
		return 97;
	case 5285: //Silvered Fury
		return 16;
	case 5286: //Spurn Undead
		return 124;
	case 5287: //Symbol of Jeron
		return 112;
	case 5288: //Pious Fury
		return 16;
	case 5289: //Light of Piety
		return 77;
	case 5290: //Hand of Direction
		return 46;
	case 5291: //Armor of the Champion
		return 87;
	case 5292: //Serene Command
		return 97;
	case 5293: //Pious Cleansing
		return 32;
	case 5294: //Bulwark of Piety
		return 6;
	case 5295: //Jeron's Mark
		return 112;
	case 5296: //Wave of Piety
		return 42;
	case 5297: //Brell's Brawny Bulwark
		return 47;
	case 5298: //Affirmation
		return 1;
	case 5299: //Ancient: Force of Jeron
		return 97;
	case 5300: //Nature Veil
		return 16;
	case 5301: //Displace Summoned
		return 111;
	case 5302: //Shield of Briar
		return 21;
	case 5303: //Locust Swarm
		return 58;
	case 5304: //Sylvan Water
		return 42;
	case 5305: //Guard of the Earth
		return 21;
	case 5306: //Strength of the Hunter
		return 47;
	case 5307: //Briarcoat
		return 21;
	case 5308: //Nature's Veil Parry
		return 83;
	case 5309: //Frost Wind
		return 14;
	case 5310: //Hunter's Vigor
		return 43;
	case 5311: //Nature's Denial
		return 16;
	case 5312: //Howl of the Predator
		return 7;
	case 5313: //Hearth Embers
		return 38;
	case 5314: //Nature's Balance
		return 31;
	case 5315: //Onyx Skin
		return 46;
	case 5316: //Tranquility of the Glade
		return 11;
	case 5317: //Ward of the Hunter
		return 21;
	case 5318: //Call of Lightning
		return 16;
	case 5319: //Ancient: North Wind
		return 14;
	case 5320: //Blood of Discord
		return 38;
	case 5321: //Dark Tendrils
		return 58;
	case 5322: //Dark Constriction
		return 29;
	case 5323: //Bond of Inruku
		return 33;
	case 5324: //Touch of Inruku
		return 43;
	case 5325: //Inruku's Bite
		return 33;
	case 5326: //Omen-SK-PH
		return 124;
	case 5327: //Shroud of Discord
		return 16;
	case 5328: //Theft of Pain
		return 76;
	case 5329: //Terror of Discord
		return 53;
	case 5330: //Blood of Inruku
		return 75;
	case 5331: //Son of Decay
		return 103;
	case 5332: //Rune of Decay
		return 70;
	case 5333: //Pact of Decay
		return 17;
	case 5334: //Spear of Muram
		return 29;
	case 5335: //Scythe of Inruku
		return 124;
	case 5336: //Dread Gaze
		return 53;
	case 5337: //Theft of Hate
		return 76;
	case 5338: //Touch of the Devourer
		return 43;
	case 5339: //Cloak of Discord
		return 87;
	case 5340: //Ancient: Bite of Muram
		return 33;
	case 5341: //Omen-Druid-PH
		return 83;
	case 5342: //Oaken Vigor
		return 43;
	case 5343: //Stormwatch
		return 58;
	case 5344: //Hand of the Sun
		return 81;
	case 5345: //Tempest Wind
		return 14;
	case 5346: //Earth Shiver
		return 58;
	case 5347: //Nature's Serenity
		return 11;
	case 5348: //Immolation of the Sun
		return 38;
	case 5349: //Hungry Vines
		return 89;
	case 5350: //Lion's Strength
		return 96;
	case 5351: //Sun's Corona
		return 81;
	case 5352: //Steeloak Skin
		return 46;
	case 5353: //Blessing of Oak
		return 43;
	case 5354: //Glacier Breath
		return 81;
	case 5355: //Chlorotrope
		return 42;
	case 5356: //Oaken Guard
		return 62;
	case 5357: //Wasp Swarm
		return 58;
	case 5358: //Nettle Shield
		return 21;
	case 5359: //Nature's Beckon
		return 13;
	case 5360: //Omen-Druid-PH
		return 83;
	case 5361: //Solstice Strike
		return 38;
	case 5362: //Nettlecoat
		return 21;
	case 5363: //Vengeance of the Sun
		return 38;
	case 5364: //Desolate Summoned
		return 111;
	case 5365: //Circle of Nettles
		return 21;
	case 5366: //Blessing of Steeloak
		return 46;
	case 5367: //Glitterfrost
		return 14;
	case 5368: //Mask of the Wild
		return 59;
	case 5369: //Ancient: Glacier Frost
		return 14;
	case 5370: //Luvwen's Aria of Serenity
		return 11;
	case 5371: //Vulka's Chant of Disease
		return 29;
	case 5372: //Bellow of Chaos
		return 58;
	case 5373: //Luvwen's Lullaby
		return 35;
	case 5374: //Verse of Vesagran
		return 6;
	case 5375: //Zuriki's Song of Shenanigans
		return 88;
	case 5376: //War March of Muram
		return 41;
	case 5377: //Cantata of Life
		return 44;
	case 5378: //Vulka's Chant of Poison
		return 75;
	case 5379: //Vulka's Chant of Frost
		return 14;
	case 5380: //Yelhun's Mystic Call
		return 91;
	case 5381: //Dirge of Metala
		return 89;
	case 5382: //Eriki's Psalm of Power
		return 41;
	case 5383: //Voice of the Vampire
		return 13;
	case 5384: //Chorus of Life
		return 44;
	case 5385: //Vulka's Chant of Flame
		return 38;
	case 5386: //Omen-Bard-PH
		return 81;
	case 5387: //Vulka's Lullaby
		return 35;
	case 5388: //Ancient: Call of Power
		return 41;
	case 5389: //Farrel's Companion
		return 104;
	case 5390: //Spirit of Sense
		return 2;
	case 5391: //Yoppa's Spear of Venom
		return 75;
	case 5392: //Putrid Decay
		return 81;
	case 5393: //Spirit of Perseverance
		return 43;
	case 5394: //Crippling Spasm
		return 30;
	case 5395: //Yoppa's Mending
		return 42;
	case 5396: //Wunshi's Focusing
		return 87;
	case 5397: //Ancestral Bulwark
		return 6;
	case 5398: //Spirit of Fortitude
		return 94;
	case 5399: //Talisman of Sense
		return 2;
	case 5400: //Vindictive Spirit
		return 88;
	case 5401: //Yoppa's Rain of Venom
		return 75;
	case 5402: //Spirit Veil
		return 51;
	case 5403: //Pained Memory
		return 17;
	case 5404: //Spirit of Might
		return 96;
	case 5405: //Talisman of Fortitude
		return 94;
	case 5406: //Talisman of Perseverance
		return 43;
	case 5407: //Shroud of Erana
		return 0;
	case 5408: //Ice Age
		return 14;
	case 5409: //Talisman of Might
		return 96;
	case 5410: //Pure Spirit
		return 19;
	case 5411: //Breath of Wunshi
		return 29;
	case 5412: //Curse of Sisslak
		return 58;
	case 5413: //Shroud of Erana Parry
		return 81;
	case 5414: //Blood of Yoppa
		return 75;
	case 5415: //Talisman of Wunshi
		return 87;
	case 5416: //Spiritual Serenity
		return 32;
	case 5417: //Champion
		return 7;
	case 5418: //Ancient: Ancestral Calling
		return 17;
	case 5419: //Soulspike
		return 43;
	case 5420: //Acikin
		return 75;
	case 5421: //Shadow Guard
		return 87;
	case 5422: //Omen-Nec-PH
		return 103;
	case 5423: //Chaos Plague
		return 29;
	case 5424: //Grip of Mori
		return 29;
	case 5425: //Glyph of Darkness
		return 70;
	case 5426: //Fang of Death
		return 33;
	case 5427: //Scent of Midnight
		return 81;
	case 5428: //Dull Pain
		return 84;
	case 5429: //Dark Hold
		return 35;
	case 5430: //Desecrating Darkness
		return 58;
	case 5431: //Lost Soul
		return 103;
	case 5432: //Dark Nightmare
		return 58;
	case 5433: //Chaos Venom
		return 75;
	case 5434: //Dark Possession
		return 17;
	case 5435: //Dark Salve
		return 42;
	case 5436: //Bulwark of Calliav
		return 10;
	case 5437: //Pyre of Mori
		return 38;
	case 5438: //Dark Assassin
		return 103;
	case 5439: //Word of Chaos
		return 13;
	case 5440: //Desolate Undead
		return 124;
	case 5441: //Ancient: Curse of Mori
		return 58;
	case 5442: //Icebane
		return 14;
	case 5443: //Ether Shield
		return 87;
	case 5444: //Tears of the Sun
		return 38;
	case 5445: //Lightningbane
		return 58;
	case 5446: //Spark of Fire
		return 38;
	case 5447: //Firebane
		return 38;
	case 5448: //Ether Skin
		return 84;
	case 5449: //Spark of Thunder
		return 58;
	case 5450: //Thundaka
		return 58;
	case 5451: //Circle of Thunder
		return 58;
	case 5452: //Spark of Lightning
		return 58;
	case 5453: //Ether Ward
		return 92;
	case 5454: //Meteor Storm
		return 38;
	case 5455: //Circle of Fire
		return 38;
	case 5456: //Telekara
		return 97;
	case 5457: //Spark of Ice
		return 14;
	case 5458: //Gelidin Comet
		return 14;
	case 5459: //Bulwark of Calrena
		return 92;
	case 5460: //Solist's Frozen Sword
		return 99;
	case 5461: //Gelid Rains
		return 14;
	case 5462: //Corona Flare
		return 38;
	case 5463: //Ancient: Core Fire
		return 38;
	case 5464: //Summon Calliav's Runed Mantle
		return 107;
	case 5465: //Summon Staff of the North Wind
		return 64;
	case 5466: //Fireskin
		return 21;
	case 5467: //Summon Fireblade
		return 64;
	case 5468: //Summon Calliav's Jeweled Bracelet
		return 107;
	case 5469: //Summon Calliav's Spiked Ring
		return 107;
	case 5470: //Summon Calliav's Glowing Bauble
		return 107;
	case 5471: //Summon Calliav's Steel Bracelet
		return 107;
	case 5472: //Elemental Aura
		return 87;
	case 5473: //Child of Wind
		return 98;
	case 5474: //Bolt of Jerikor
		return 38;
	case 5475: //Summon Calliav's Platinum Choker
		return 107;
	case 5476: //Phantom Shield
		return 44;
	case 5477: //Summon Dagger of the Deep
		return 64;
	case 5478: //Elemental Fury
		return 70;
	case 5479: //Rain of Jerikor
		return 38;
	case 5480: //Child of Water
		return 105;
	case 5481: //Burning Earth
		return 38;
	case 5482: //Omen-Mage-PH
		return 64;
	case 5483: //Summon Pouch of Jerikor
		return 107;
	case 5484: //Blade Strike
		return 58;
	case 5485: //Child of Fire
		return 102;
	case 5486: //Summon Sphere of Air
		return 109;
	case 5487: //Omen-Mage-PH
		return 35;
	case 5488: //Circle of Fireskin
		return 21;
	case 5489: //Summon Crystal Belt
		return 64;
	case 5490: //Desolate Summoned
		return 111;
	case 5491: //Renewal of Jerikor
		return 42;
	case 5492: //Pyrilen Skin
		return 21;
	case 5493: //Star Scream
		return 58;
	case 5494: //Bulwark of Calliav
		return 10;
	case 5495: //Child of Earth
		return 100;
	case 5496: //Star Strike
		return 38;
	case 5497: //Elemental Simulcram
		return 17;
	case 5498: //Ancient: Nova Strike
		return 38;
	case 5499: //Synapsis Spasm
		return 30;
	case 5500: //Ethereal Rune
		return 84;
	case 5501: //Omen-Enc-PH
		return 53;
	case 5502: //Mystic Shield
		return 87;
	case 5503: //Felicity
		return 35;
	case 5504: //Rune of Salik
		return 84;
	case 5505: //Salik's Animation
		return 99;
	case 5506: //Placate
		return 11;
	case 5507: //Speed of Salik
		return 41;
	case 5508: //Omen-Enc-PH
		return 80;
	case 5509: //Arcane Noose
		return 58;
	case 5510: //Compel
		return 13;
	case 5511: //Wake of Felicity
		return 35;
	case 5512: //Omen-Enc-PH
		return 58;
	case 5513: //Clairvoyance
		return 59;
	case 5514: //Mayhem
		return 84;
	case 5515: //Wall of Alendar
		return 92;
	case 5516: //Color Snap
		return 97;
	case 5517: //Circle of Alendar
		return 92;
	case 5518: //Psychosis
		return 58;
	case 5519: //True Name
		return 13;
	case 5520: //Euphoria
		return 35;
	case 5521: //Hastening of Salik
		return 41;
	case 5522: //Voice of Clairvoyance
		return 59;
	case 5523: //Ancient: Neurosis
		return 58;
	case 5524: //Omen-Bst-PH
		return 31;
	case 5525: //Omen-Bst-PH
		return 96;
	case 5526: //Healing of Mikkily
		return 42;
	case 5527: //Chimera Blood
		return 75;
	case 5528: //Muada's Mending
		return 42;
	case 5529: //Focus of Alladnu
		return 87;
	case 5530: //Spiritual Vitality
		return 47;
	case 5531: //Spirit of Alladnu
		return 104;
	case 5532: //Omen-Bst-PH
		return 41;
	case 5533: //Growl of the Beast
		return 70;
	case 5534: //Spirit of Irionu
		return 71;
	case 5535: //Glacier Spear
		return 14;
	case 5536: //Feral Vigor
		return 43;
	case 5537: //Spiritual Ascendance
		return 44;
	case 5538: //Spirit of Rashara
		return 104;
	case 5539: //Feral Guard
		return 10;
	case 5540: //Festering Malady
		return 29;
	case 5541: //Omen-Bst-PH
		return 88;
	case 5542: //Ferocity of Irionu
		return 7;
	case 5543: //Ancient: Savage Ice
		return 14;
	case 5554: //Cloud of Discord
		return 0;
	case 5555: //Bellow of Tunat'Muram
		return 0;
	case 5556: //Whirling Smash
		return 0;
	case 5560: //Blistering Rage
		return 115;
	case 5570: //Pillage Magic
		return 31;
	case 5571: //Tangle
		return 89;
	case 5572: //Entangle
		return 89;
	case 5573: //Corporeal Empathy Recourse
		return 56;
	case 5605: //test mana
		return 59;
	case 5731: //Circle of Barindu
		return 113;
	case 5732: //Barindu Portal
		return 113;
	case 5733: //Ring of Barindu
		return 113;
	case 5734: //Barindu Gate
		return 113;
	case 5735: //Translocate: Barindu
		return 113;
	case 5741: //Pyrilen Bolt
		return 0;
	case 5744: //Kiss of the Pyrilen
		return 0;
	case 5745: //Pyrilonis' Vengeance
		return 0;
	case 5760: //Gelaqua's Embrace
		return 0;
	case 5761: //Heart of Frost
		return 0;
	case 5874: //Advanced Dire Charm
		return 13;
	case 5875: //Advanced Dire Charm Animal
		return 13;
	case 5876: //Advanced Dire Charm Undead
		return 13;
	case 5919: //Death Peace
		return 64;
	case 5924: //Celestial Stun
		return 97;
	case 5931: //Embrace of Shadows
		return 51;
	case 5976: //Plagued Filth
		return 0;
	case 5979: //Infection of Pain
		return 0;
	case 5980: //Orb's Curse
		return 0;
	case 5989: //Gloom Toxin
		return 0;
	case 5990: //Shade Mantle
		return 0;
	case 5992: //Numbing Touch
		return 0;
	case 5993: //Girplan Chatter
		return 0;
	case 5994: //Body Slam
		return 0;
	case 5996: //Bazu Grip
		return 0;
	case 5997: //Pyrilen Ember
		return 0;
	case 5998: //Pyrilen Charm
		return 0;
	case 5999: //Pyronic Lash
		return 0;
	case 6000: //Pyrilen Fury
		return 0;
	case 6001: //Pyronic Assault
		return 0;
	case 6003: //Chimeran Laceration
		return 0;
	case 6004: //Chimeran Breath
		return 0;
	case 6005: //Infected Bite
		return 0;
	case 6006: //Gelidran Sleet
		return 0;
	case 6007: //Frostcicle
		return 0;
	case 6008: //Gelidran Hail
		return 0;
	case 6009: //Gelidran Stalagmite
		return 0;
	case 6010: //Freezing Touch
		return 0;
	case 6012: //Crushing Ice
		return 0;
	case 6013: //Ice Shards
		return 0;
	case 6014: //Feranic Grasp
		return 0;
	case 6016: //Feran Tentacle
		return 0;
	case 6017: //Darkbreath
		return 0;
	case 6019: //Clinging Apathy
		return 0;
	case 6020: //Wing Strike
		return 0;
	case 6021: //Deep Gouge
		return 0;
	case 6023: //Dragornian Malady
		return 0;
	case 6024: //Dragornian Venom
		return 0;
	case 6025: //Discordling Leap
		return 0;
	case 6026: //Discordling Ruin
		return 0;
	case 6027: //Chaotica
		return 0;
	case 6028: //Seething Bite
		return 0;
	case 6029: //Sinking Fangs
		return 0;
	case 6049: //Lightning Strike
		return 0;
	case 6050: //Static Pulse
		return 0;
	case 6051: //Fire Strike
		return 0;
	case 6052: //Ice Strike
		return 0;
	case 6053: //Bane of Dranik
		return 0;
	case 6054: //Bane of Dranik
		return 0;
	case 6055: //Bane of Dranik
		return 0;
	case 6065: //Murk Acid
		return 0;
	case 6066: //Murk Acid
		return 0;
	case 6103: //Nature's Denial Strike
		return 111;
	case 6105: //Silvered Fury Strike
		return 124;
	case 6120: //Phase Walk
		return 51;
	case 6121: //Shroud of Air
		return 51;
	case 6122: //Cloud of Indifference
		return 51;
	case 6123: //Cloak of Nature
		return 51;
	case 6124: //Shadow of Death
		return 51;
	case 6125: //Sun Cloak
		return 51;
	case 6126: //Hand of Order
		return 0;
	case 6128: //Gelidran Aura
		return 51;
	case 6140: //Ancient: Hallowed Light
		return 42;
	case 6141: //Ancient: Chlorobon
		return 42;
	case 6142: //Ancient: Wilslik's Mending
		return 42;
	case 6143: //Ancient: Touch of Orshilak
		return 43;
	case 6144: //Ancient: Voice of Muram
		return 13;
	case 6145: //Ancient: Veil of Pyrilonus
		return 21;
	case 6146: //Ancient: Spear of Gelaqua
		return 14;
	case 6152: //Vindictive Spirit Recourse
		return 62;
	case 6153: //Elemental Simulcram Recourse
		return 87;
	case 6154: //Hungry Vines Recourse
		return 62;
	case 6156: //Oaken Guard Parry
		return 53;
	case 6169: //Crippling Strike
		return 89;
	case 6170: //Mind Strike
		return 97;
	case 6171: //Baffling Strike
		return 53;
	case 6172: //Axe of the Destroyer
		return 110;
	case 6173: //Bazu Bellow
		return 115;
	case 6174: //Daggerfall
		return 115;
	case 6175: //Phantom Cry
		return 115;
	case 6176: //Slaughter Gate
		return 28;
	case 6177: //Translocate: Slaughter
		return 28;
	case 6178: //Slaughter Portal
		return 28;
	case 6179: //Circle of Slaughter
		return 28;
	case 6180: //Ring of Slaughter
		return 28;
	case 6181: //Bloodfields Gate
		return 28;
	case 6182: //Translocate: Bloodfields
		return 28;
	case 6183: //Bloodfields Portal
		return 28;
	case 6184: //Circle of Bloodfields
		return 28;
	case 6185: //Ring of Bloodfields
		return 28;
	case 6187: //Limit Test Hit
		return 43;
	case 6190: //Shocking Defense Discipline
		return 122;
	case 6191: //Aura of Runes Discipline
		return 120;
	case 6192: //Savage Onslaught Discipline
		return 121;
	case 6193: //Dreamwalk Discipline
		return 120;
	case 6194: //Rapid Kick Discipline
		return 121;
	case 6195: //Counterforce Discipline
		return 122;
	case 6196: //Deadly Aim Discipline
		return 122;
	case 6197: //Frenzied Stabbing Discipline
		return 121;
	case 6198: //Imperceptible Discipline
		return 120;
	case 6199: //Vengeful Flurry Discipline
		return 121;
	case 6200: //Unpredictable Rage Discipline
		return 122;
	case 6201: //Unflinching Will Discipline
		return 120;
	case 6202: //Stun Effect
		return 118;
	case 6203: //Rune Effect
		return 118;
	case 6204: //Slow Effect
		return 118;
	case 6205: //Poison DD Effect
		return 118;
	case 6206: //Lower Hate Effect
		return 118;
	case 6207: //Increase Damage Effect
		return 118;
	case 6209: //Test Doom Rune
		return 84;
	case 6233: //Harmonic Balance
		return 7;
	case 6265: //Divine Balance
		return 7;
	case 6330: //Chaotic Strike I
		return 58;
	case 6331: //Chaotic Strike II
		return 58;
	case 6332: //Chaotic Strike III
		return 58;
	case 6333: //Chaotic Strike IV
		return 58;
	case 6334: //Chaotic Strike V
		return 58;
	case 6335: //Chaotic Strike VI
		return 58;
	case 6336: //Chaotic Strike VII
		return 58;
	case 6337: //Life Sap I
		return 58;
	case 6338: //Life Sap II
		return 58;
	case 6339: //Life Sap III
		return 58;
	case 6340: //Life Sap IV
		return 58;
	case 6341: //Freezing Strike I
		return 58;
	case 6342: //Freezing Strike II
		return 58;
	case 6343: //Freezing Strike III
		return 58;
	case 6344: //Freezing Strike IV
		return 58;
	case 6345: //Freezing Strike V
		return 58;
	case 6346: //Freezing Strike VI
		return 58;
	case 6347: //Freezing Strike VII
		return 58;
	case 6348: //Freezing Strike VIII
		return 58;
	case 6349: //Freezing Strike IX
		return 58;
	case 6350: //Fiery Strike I
		return 58;
	case 6351: //Fiery Strike II
		return 58;
	case 6352: //Fiery Strike III
		return 58;
	case 6353: //Fiery Strike IV
		return 58;
	case 6354: //Fiery Strike V
		return 58;
	case 6355: //Fiery Strike VI
		return 58;
	case 6356: //Fiery Strike VII
		return 58;
	case 6357: //Fiery Strike VIII
		return 58;
	case 6358: //Fiery Strike IX
		return 58;
	case 6359: //Form of Defense I
		return 48;
	case 6360: //Form of Defense III
		return 48;
	case 6361: //Form of Protection I
		return 48;
	case 6362: //Form of Protection III
		return 48;
	case 6363: //Form of Endurance I
		return 48;
	case 6364: //Form of Endurance III
		return 48;
	case 6365: //Form of Rejuvenation I
		return 48;
	case 6366: //Form of Rejuvenation III
		return 48;
	case 6499: //Gelid Claw
		return 14;
	case 6500: //Stunning Strike
		return 97;
	case 6502: //Unpack Brewer's Still
		return 108;
	case 6505: //Blood Dream
		return 38;
	case 6512: //Lupine Rage
		return 58;
	case 6513: //Devour Enchantment
		return 31;
	case 6514: //Blessing of Rallos Zek
		return 34;
	case 6515: //Blessing of The Tribunal
		return 34;
	case 6516: //Blessing of Cazic Thule
		return 34;
	case 6517: //Blessing of Brell Serilis
		return 34;
	case 6518: //Blessing of Tunare
		return 34;
	case 6519: //Blessing of Innoruuk
		return 34;
	case 6520: //Blessing of Prexus
		return 34;
	case 6521: //Blessing of Mithaniel Marr
		return 34;
	case 6522: //Blessing of Erollisi Marr
		return 34;
	case 6523: //Blessing of Bertoxxulous
		return 34;
	case 6524: //Blessing of Solusek Ro
		return 34;
	case 6525: //Blessing of Karana
		return 34;
	case 6526: //Blessing of Bristlebane
		return 34;
	case 6527: //Blessing of Quellious
		return 34;
	case 6528: //Blessing of Rodcet Nife
		return 34;
	case 6529: //Blessing of Veeshan
		return 34;
	case 6530: //Words of the Sceptic
		return 34;
	case 6532: //Makural's Curse
		return 16;
	case 6533: //RB_reflect_test
		return 78;
	case 6534: //Makural's Torment
		return 38;
	case 6535: //Makural's TormentSK
		return 38;
	case 6563: //Mass Illusion: Human
		return 49;
	case 6564: //Mass Illusion: Barbarian
		return 49;
	case 6565: //Mass Illusion: Erudite
		return 49;
	case 6566: //Mass Illusion: Wood Elf
		return 49;
	case 6567: //Mass Illusion: Fier`dal
		return 49;
	case 6568: //Mass Illusion: High Elf
		return 49;
	case 6569: //Mass Illusion: Dark Elf
		return 49;
	case 6570: //Mass Illusion: Half-Elf
		return 49;
	case 6571: //Mass Illusion: Dwarf
		return 49;
	case 6572: //Mass Illusion: Troll
		return 49;
	case 6573: //Mass Illusion: Ogre
		return 49;
	case 6574: //Mass Illusion: Halfling
		return 49;
	case 6575: //Mass Illusion: Gnome
		return 49;
	case 6576: //Mass Illusion: Werewolf
		return 49;
	case 6577: //Mass Illusion: Froglok
		return 49;
	case 6578: //Mass Illusion: Imp
		return 49;
	case 6579: //Mass Illusion: Earth Elemental
		return 49;
	case 6580: //Mass Illusion: Air Elemental
		return 49;
	case 6581: //Mass Illusion: Fire Elemental
		return 49;
	case 6582: //Mass Illusion: Water Elemental
		return 49;
	case 6583: //Mass Illusion: Scarecrow
		return 49;
	case 6584: //Mass Illusion: Spirit Wolf
		return 49;
	case 6585: //Mass Illusion: Iksar
		return 49;
	case 6586: //Mass Illusion: Vah Shir
		return 49;
	case 6587: //Mass Illusion: Guktan
		return 49;
	case 6588: //Mass Illusion: Scaled Wolf
		return 49;
	case 6589: //Mass Illusion: Skeleton
		return 49;
	case 6590: //Mass Illusion: Dry Bone
		return 49;
	case 6591: //Mass Illusion: Frost Bone
		return 49;
	case 6592: //GM Bind Sight
		return 129;
	case 6656: //Spray of Venom
		return 75;
	case 6662: //Ward of Retribution
		return 16;
	case 6663: //Guard of Righteousness
		return 120;
	case 6664: //Earthen Shackles
		return 89;
	case 6665: //Serpent Vines
		return 89;
	case 6666: //Storm Blade
		return 16;
	case 6667: //Spirit of the Panther
		return 16;
	case 6668: //Shadow Orb
		return 18;
	case 6669: //Claw of Vox
		return 14;
	case 6670: //Summon: Molten Orb
		return 109;
	case 6671: //Rune of Rikkukin
		return 84;
	case 6672: //Growl of the Panther
		return 71;
	case 6673: //Soul Shield
		return 120;
	case 6674: //Storm Blade Strike
		return 38;
	case 6675: //Storm Blade Strike SK
		return 38;
	case 6676: //Magma Jet
		return 38;
	case 6677: //Shadow Orb Recourse
		return 109;
	case 6717: //Growl of the Panther
		return 7;
	case 6719: //Ward of Retribution Parry
		return 88;
	case 6724: //Panther Maw
		return 38;
	case 6725: //Cyclone Blade
		return 53;
	case 6726: //Assassin's Feint
		return 35;
	case 6727: //Dragon Fang
		return 25;
	case 6728: //Dragon Fang Strike
		return 72;
	case 6729: //Destroyer's Volley
		return 72;
	case 6730: //Ward of Vengeance
		return 16;
	case 6731: //Guard of Humility
		return 120;
	case 6732: //Earthen Embrace
		return 89;
	case 6733: //Mire Thorns
		return 89;
	case 6734: //Song of the Storm
		return 16;
	case 6735: //Spirit of the Leopard
		return 7;
	case 6736: //Soul Orb
		return 18;
	case 6737: //Claw of Frost
		return 14;
	case 6738: //Summon: Lava Orb
		return 109;
	case 6739: //Rune of the Scale
		return 84;
	case 6740: //Growl of the Leopard
		return 71;
	case 6741: //Soul Guard
		return 120;
	case 6742: //Song of the Storm Strike
		return 38;
	case 6743: //Song of the Storm Strike SK
		return 38;
	case 6744: //Lava Jet
		return 38;
	case 6745: //Soul Orb Recourse
		return 109;
	case 6747: //Growl of the Leopard
		return 7;
	case 6748: //Ward of Vengeance Parry
		return 35;
	case 6749: //Leopard Maw
		return 38;
	case 6750: //Whirlwind Blade
		return 53;
	case 6751: //Rogue's Ploy
		return 35;
	case 6752: //Leopard Claw
		return 25;
	case 6753: //Leopard Claw Strike
		return 72;
	case 6754: //Rage Volley
		return 72;
	case 6771: //Geomantra II
		return 92;
	case 6777: //Leopard Maw
		return 38;
	case 6778: //Leopard Maw SK
		return 38;
	case 6779: //Panther Maw
		return 38;
	case 6780: //Panther Maw SK
		return 38;
	case 6782: //Magma Blast
		return 0;
	case 6826: //Desolate Deeds
		return 88;
	case 6827: //Balance of Discord
		return 88;
	case 6828: //Sha's Legacy
		return 88;
	case 6839: //Static Strike
		return 58;
	case 6840: //Firestrike
		return 38;
	case 6841: //Bolt of Flame
		return 38;
	case 6842: //Cinder Bolt
		return 38;
	case 6843: //Anarchy
		return 58;
	case 6844: //Shock of Spikes
		return 58;
	case 6845: //Dismiss Summoned
		return 111;
	case 6846: //Dismiss Undead
		return 124;
	case 6847: //Blaze
		return 38;
	case 6848: //Shock of Poison
		return 75;
	case 6849: //Shock of Flame
		return 38;
	case 6850: //Chaos Flux
		return 58;
	case 6851: //Icestrike
		return 14;
	case 6852: //Icicle Shock
		return 14;
	case 6853: //Lifedraw
		return 43;
	case 6854: //Drain Soul
		return 43;
	case 6855: //Frost Shock
		return 14;
	case 6856: //Inferno Shock
		return 38;
	case 6857: //Lightning Shock
		return 58;
	case 6858: //Winter's Roar
		return 14;
	case 6859: //Spirit Tap
		return 43;
	case 6860: //Drain Spirit
		return 43;
	case 6861: //Shock of Ice
		return 14;
	case 6862: //Flame Shock
		return 38;
	case 6863: //Ice Shock
		return 14;
	case 6864: //Conflagration
		return 38;
	case 6865: //Expel Undead
		return 124;
	case 6866: //Rend
		return 58;
	case 6867: //Torbas' Acid Blast
		return 75;
	case 6868: //Frost
		return 14;
	case 6869: //Sunstrike
		return 38;
	case 6870: //Blast of Frost
		return 14;
	case 6871: //Shock of Fiery Blades
		return 38;
	case 6872: //Burning Arrow
		return 38;
	case 6873: //Nature's Renewal
		return 42;
	case 6874: //Spirit Salve
		return 42;
	case 6875: //Healing Light
		return 42;
	case 6876: //Forest's Renewal
		return 42;
	case 6877: //Kragg's Salve
		return 42;
	case 6878: //Greater Healing Light
		return 42;
	case 6899: //Flash Powder Explosion
		return 38;
	case 6902: //Ward of the Divine
		return 16;
	case 6903: //Ward of Rebuke
		return 16;
	case 6904: //Ward of the Divine Parry
		return 88;
	case 6905: //Ward of Rebuke Parry
		return 88;
	case 6906: //Spirit of the Puma
		return 16;
	case 6907: //Spirit of the Jaguar
		return 16;
	case 6908: //Puma Maw
		return 38;
	case 6909: //Puma Maw SK
		return 38;
	case 6910: //Elixir of Healing I
		return 32;
	case 6911: //Elixir of Healing II
		return 32;
	case 6912: //Elixir of Healing III
		return 32;
	case 6913: //Elixir of Healing IV
		return 32;
	case 6914: //Elixir of Healing V
		return 32;
	case 6915: //Elixir of Healing VI
		return 32;
	case 6916: //Elixir of Healing VII
		return 32;
	case 6917: //Elixir of Healing VIII
		return 32;
	case 6918: //Elixir of Healing IX
		return 32;
	case 6919: //Elixir of Healing X
		return 32;
	case 6920: //Healing Potion I
		return 32;
	case 6921: //Healing Potion II
		return 32;
	case 6922: //Healing Potion III
		return 32;
	case 6923: //Healing Potion IV
		return 32;
	case 6924: //Healing Potion V
		return 32;
	case 6925: //Healing Potion VI
		return 32;
	case 6926: //Healing Potion VII
		return 32;
	case 6927: //Healing Potion VIII
		return 32;
	case 6928: //Healing Potion IX
		return 32;
	case 6929: //Healing Potion X
		return 32;
	case 6930: //Elixir of Health I
		return 46;
	case 6931: //Elixir of Health II
		return 46;
	case 6932: //Elixir of Health III
		return 46;
	case 6933: //Elixir of Health IV
		return 46;
	case 6934: //Elixir of Health V
		return 46;
	case 6935: //Elixir of Health VI
		return 46;
	case 6936: //Elixir of Health VII
		return 46;
	case 6937: //Elixir of Health VIII
		return 46;
	case 6938: //Elixir of Health IX
		return 46;
	case 6939: //Elixir of Health X
		return 46;
	case 6940: //Elixir of Speed I
		return 41;
	case 6941: //Elixir of Speed II
		return 41;
	case 6942: //Elixir of Speed III
		return 41;
	case 6943: //Elixir of Speed IV
		return 41;
	case 6944: //Elixir of Speed V
		return 41;
	case 6945: //Elixir of Speed VI
		return 41;
	case 6946: //Elixir of Speed VII
		return 41;
	case 6947: //Elixir of Speed VIII
		return 41;
	case 6948: //Elixir of Speed IX
		return 41;
	case 6949: //Elixir of Speed X
		return 41;
	case 6950: //Elixir of Clarity I
		return 59;
	case 6951: //Elixir of Clarity II
		return 59;
	case 6952: //Elixir of Clarity III
		return 59;
	case 6953: //Elixir of Clarity IV
		return 59;
	case 6954: //Elixir of Clarity V
		return 59;
	case 6955: //Elixir of Clarity VI
		return 59;
	case 6956: //Elixir of Clarity VII
		return 59;
	case 6957: //Elixir of Clarity VIII
		return 59;
	case 6958: //Elixir of Clarity IX
		return 59;
	case 6959: //Elixir of Clarity X
		return 59;
	case 6960: //Grip of Zanivar
		return 29;
	case 6961: //Zanivar's Lifedraw
		return 43;
	case 6962: //Zanivar's Poison Bolt
		return 75;
	case 6963: //Minion of Zanivar
		return 103;
	case 6965: //Rampage of Rathkan
		return 0;
	case 6966: //Hurl of Rathkan
		return 58;
	case 6967: //Shock of Rathkan
		return 97;
	case 6968: //Lantern Bomb
		return 38;
	case 6969: //Flashpowder Bomb
		return 58;
	case 6973: //Intangibility
		return 52;
	case 6976: //Retch Weed
		return 58;
	case 6977: //Deistic Voice
		return 37;
	case 6978: //Deistic Bellow
		return 37;
	case 6979: //Deistic Howl
		return 37;
	case 6980: //Unholy Voice
		return 37;
	case 6981: //Unholy Bellow
		return 37;
	case 6982: //Unholy Howl
		return 37;
	case 6983: //Phobia
		return 37;
	case 6984: //Jitterskin
		return 37;
	case 6985: //Anxiety Attack
		return 37;
	case 6986: //Shadow Voice
		return 37;
	case 6987: //Shadow Bellow
		return 37;
	case 6988: //Shadow Howl
		return 37;
	case 6989: //Cower the Dead
		return 37;
	case 6990: //Death's Despair
		return 37;
	case 6991: //Revulsion of Death
		return 37;
	case 6992: //Eidolon Voice
		return 37;
	case 6993: //Eidolon Bellow
		return 37;
	case 6994: //Eidolon Howl
		return 37;
	case 6995: //Soulless Fear
		return 37;
	case 6996: //Soulless Panic
		return 37;
	case 6997: //Soulless Terror
		return 37;
	case 6998: //Instinctual Fear
		return 37;
	case 6999: //Instinctual Panic
		return 37;
	case 7000: //Instinctual Terror
		return 37;
	case 7001: //Angstlich's Echo of Terror
		return 37;
	case 7002: //Angstlich's Wail of Panic
		return 37;
	case 7003: //Circle of Dreams
		return 35;
	case 7004: //Guard of Piety
		return 120;
	case 7005: //Ichor Guard
		return 120;
	case 7168: //Obscuring Sporecloud
		return 51;
	case 7169: //Root of Weakness
		return 83;
	case 7170: //Rage of the Root
		return 95;
	case 7171: //Fungal Refreshment
		return 42;
	case 7172: //Spore Snooze
		return 35;
	case 7173: //Fungal Sheen
		return 0;
	case 7177: //Blind Fury I
		return 95;
	case 7178: //Blind Fury II
		return 95;
	case 7179: //Blind Fury III
		return 95;
	case 7180: //Orc Smash I
		return 97;
	case 7181: //Orc Smash II
		return 97;
	case 7182: //Orc Smash III
		return 97;
	case 7183: //Blood Rage I
		return 95;
	case 7184: //Blood Rage II
		return 95;
	case 7185: //Blood Rage III
		return 95;
	case 7186: //Dark Bellow I
		return 30;
	case 7187: //Dark Bellow II
		return 30;
	case 7188: //Dark Bellow III
		return 30;
	case 7189: //Wave of Fire
		return 38;
	case 7190: //Tide of Sloth I
		return 88;
	case 7191: //Tide of Sloth II
		return 88;
	case 7192: //Tide of Sloth III
		return 88;
	case 7193: //Fiery Surge I
		return 38;
	case 7194: //Fiery Surge II
		return 38;
	case 7195: //Fiery Surge III
		return 38;
	case 7199: //Soothing Remedy
		return 32;
	case 7200: //Orcish Regeneration I
		return 79;
	case 7201: //Orcish Regeneration II
		return 79;
	case 7202: //Orcish Regeneration III
		return 79;
	case 7203: //Weak Knees
		return 89;
	case 7204: //Complete Refreshment
		return 42;
	case 7205: //Hand of Darkness
		return 95;
	case 7206: //Shadowmend
		return 42;
	case 7207: //Soulmend
		return 0;
	case 7208: //Arachnae Scream
		return 97;
	case 7209: //Voice of Vule
		return 61;
	case 7210: //Speed of the Spider
		return 95;
	case 7211: //Skinwalker's Mindwave
		return 35;
	case 7212: //Dire Musings
		return 88;
	case 7213: //Thoughtraze
		return 58;
	case 7214: //Dark Messenger
		return 103;
	case 7215: //Bite of Night
		return 75;
	case 7216: //Chanted Doom
		return 58;
	case 7217: //Vile Spirit
		return 43;
	case 7218: //Spiteful Hex
		return 58;
	case 7219: //Eboncall
		return 58;
	case 7220: //Stormreaver
		return 58;
	case 7221: //Ethereal Carapace
		return 84;
	case 7222: //Master's Shadow
		return 92;
	case 7223: //Ice Spray
		return 14;
	case 7224: //Needling Annoyance
		return 53;
	case 7232: //Jaguar Maw
		return 38;
	case 7233: //Jaguar Maw SK
		return 38;
	case 7400: //Heal Wounds I
		return 42;
	case 7401: //Heal Wounds II
		return 42;
	case 7402: //Heal Wounds III
		return 42;
	case 7403: //Heal Wounds IV
		return 42;
	case 7404: //Heal Wounds V
		return 42;
	case 7405: //Heal Wounds VI
		return 42;
	case 7406: //Heal Wounds VII
		return 42;
	case 7407: //Heal Wounds VIII
		return 42;
	case 7408: //Heal Wounds IX
		return 42;
	case 7409: //Heal Wounds X
		return 42;
	case 7410: //Heal Wounds XI
		return 42;
	case 7411: //Heal Wounds XII
		return 42;
	case 7412: //Heal Wounds XIII
		return 42;
	case 7413: //Heal Wounds XIV
		return 42;
	case 7414: //Fire I
		return 38;
	case 7415: //Fire II
		return 38;
	case 7416: //Fire III
		return 38;
	case 7417: //Fire IV
		return 38;
	case 7418: //Fire V
		return 38;
	case 7419: //Fire VI
		return 38;
	case 7420: //Fire VII
		return 38;
	case 7421: //Fire VIII
		return 38;
	case 7422: //Fire IX
		return 38;
	case 7423: //Fire X
		return 38;
	case 7424: //Fire XI
		return 38;
	case 7425: //Fire XII
		return 38;
	case 7426: //Fire XIII
		return 38;
	case 7427: //Fire XIV
		return 38;
	case 7428: //Frost I
		return 38;
	case 7429: //Frost II
		return 38;
	case 7430: //Frost III
		return 38;
	case 7431: //Frost IV
		return 38;
	case 7432: //Frost V
		return 38;
	case 7433: //Frost VI
		return 38;
	case 7434: //Frost VII
		return 38;
	case 7435: //Frost VIII
		return 38;
	case 7436: //Frost IX
		return 38;
	case 7437: //Frost X
		return 38;
	case 7438: //Frost XI
		return 38;
	case 7439: //Frost XII
		return 38;
	case 7440: //Frost XIII
		return 38;
	case 7441: //Frost XIV
		return 38;
	case 7442: //Thunder I
		return 38;
	case 7443: //Thunder II
		return 38;
	case 7444: //Thunder III
		return 38;
	case 7445: //Thunder IV
		return 38;
	case 7446: //Thunder V
		return 38;
	case 7447: //Thunder VI
		return 38;
	case 7448: //Thunder VII
		return 38;
	case 7449: //Thunder VIII
		return 38;
	case 7450: //Thunder IX
		return 38;
	case 7451: //Thunder X
		return 38;
	case 7452: //Thunder XI
		return 38;
	case 7453: //Thunder XII
		return 38;
	case 7454: //Thunder XIII
		return 38;
	case 7455: //Thunder XIV
		return 38;
	case 7465: //Smoke Bomb I
		return 115;
	case 7466: //Smoke Bomb II
		return 115;
	case 7467: //Smoke Bomb III
		return 115;
	case 7468: //Smoke Bomb IV
		return 115;
	case 7469: //Smoke Bomb V
		return 115;
	case 7470: //Smoke Bomb VI
		return 115;
	case 7471: //Smoke Bomb VII
		return 115;
	case 7472: //Smoke Bomb VIII
		return 115;
	case 7473: //Smoke Bomb IX
		return 115;
	case 7474: //Smoke Bomb X
		return 115;
	case 7475: //Smoke Screen
		return 115;
	case 7476: //Pain Tolerance
		return 117;
	case 7477: //Cazic Touch II
		return 0;
	case 7478: //Destroy II
		return 0;
	case 7481: //Hamstring I
		return 89;
	case 7482: //Hamstring II
		return 89;
	case 7483: //Lesion I
		return 75;
	case 7484: //Lesion II
		return 75;
	case 7485: //Lesion III
		return 75;
	case 7486: //Lesion IV
		return 75;
	case 7487: //Lesion V
		return 75;
	case 7488: //Lesion VI
		return 75;
	case 7489: //Lesion VII
		return 75;
	case 7490: //Lesion VIII
		return 75;
	case 7491: //Lesion IX
		return 75;
	case 7492: //Lesion X
		return 75;
	case 7496: //Frost of the Ancients I
		return 75;
	case 7497: //Frost of the Ancients II
		return 75;
	case 7498: //Frost of the Ancients III
		return 75;
	case 7499: //Frost of the Ancients IV
		return 75;
	case 7500: //Frost of the Ancients V
		return 75;
	case 7501: //Frost of the Ancients VI
		return 75;
	case 7502: //Frost of the Ancients VII
		return 75;
	case 7503: //Frost of the Ancients VIII
		return 75;
	case 7504: //Frost of the Ancients IX
		return 75;
	case 7505: //Frost of the Ancients X
		return 75;
	case 7506: //Cure Poison I
		return 19;
	case 7507: //Cure Poison II
		return 19;
	case 7508: //Cure Poison III
		return 19;
	case 7509: //Cure Poison IV
		return 19;
	case 7510: //Cure Disease I
		return 19;
	case 7511: //Cure Disease II
		return 19;
	case 7512: //Cure Disease III
		return 19;
	case 7513: //Cure Disease IV
		return 19;
	case 7514: //Remove Curse I
		return 19;
	case 7515: //Remove Curse II
		return 19;
	case 7516: //Remove Curse III
		return 19;
	case 7517: //Remove Curse IV
		return 19;
	case 7518: //Play Dead I
		return 64;
	case 7519: //Play Dead II
		return 64;
	case 7520: //Play Dead III
		return 64;
	case 7521: //Play Dead IV
		return 64;
	case 7522: //Gore I
		return 97;
	case 7523: //Gore II
		return 97;
	case 7524: //Gore III
		return 97;
	case 7525: //Gore IV
		return 97;
	case 7526: //Gore V
		return 97;
	case 7527: //Gore VI
		return 97;
	case 7528: //War Bellow
		return 88;
	case 7529: //War Bellow Recourse
		return 7;
	case 7531: //Sleep I
		return 35;
	case 7532: //Sleep II
		return 35;
	case 7533: //Sleep III
		return 35;
	case 7534: //Sleep IV
		return 35;
	case 7535: //Sleep V
		return 35;
	case 7536: //Lethargy I
		return 88;
	case 7537: //Lethargy II
		return 88;
	case 7538: //Lethargy III
		return 88;
	case 7539: //Lethargy IV
		return 88;
	case 7540: //Lethargy V
		return 88;
	case 7541: //Plane Shift: Ethereal
		return 117;
	case 7542: //Plane Shift: Material
		return 118;
	case 7543: //Blink
		return 118;
	case 7545: //Swarm of Pain I
		return 75;
	case 7546: //Swarm of Pain II
		return 75;
	case 7547: //Swarm of Pain III
		return 75;
	case 7548: //Swarm of Pain IV
		return 75;
	case 7549: //Swarm of Pain V
		return 75;
	case 7550: //Swarm of Pain VI
		return 75;
	case 7551: //Swarm of Pain VII
		return 75;
	case 7552: //Swarm of Pain VIII
		return 75;
	case 7553: //Swarm of Pain IX
		return 75;
	case 7554: //Swarm of Pain X
		return 75;
	case 7555: //Fungal Malady I
		return 75;
	case 7556: //Fungal Malady II
		return 75;
	case 7557: //Fungal Malady III
		return 75;
	case 7558: //Fungal Malady IV
		return 75;
	case 7559: //Fungal Malady V
		return 75;
	case 7560: //Fungal Malady VI
		return 75;
	case 7561: //Fungal Malady VII
		return 75;
	case 7562: //Fungal Malady VIII
		return 75;
	case 7563: //Fungal Malady IX
		return 75;
	case 7564: //Fungal Malady X
		return 75;
	case 7565: //Ward of the Bear I
		return 47;
	case 7566: //Ward of the Bear II
		return 47;
	case 7567: //Ward of the Bear III
		return 47;
	case 7568: //Ward of the Wolf I
		return 65;
	case 7569: //Ward of the Wolf II
		return 2;
	case 7570: //Ward of the Wolf III
		return 2;
	case 7571: //Ward of the Tiger I
		return 39079;
	case 7572: //Ward of the Tiger II
		return 39079;
	case 7573: //Ward of the Tiger III
		return 64;
	case 7574: //Ward of the Crocodile I
		return 79;
	case 7575: //Ward of the Crocodile II
		return 79;
	case 7576: //Ward of the Crocodile III
		return 79;
	case 7577: //Ward of the Scaled Wolf I
		return 2;
	case 7578: //Ward of the Scaled Wolf II
		return 2;
	case 7579: //Ward of the Scaled Wolf III
		return 2;
	case 7580: //Ward of the Raptor I
		return 41;
	case 7581: //Ward of the Raptor II
		return 2;
	case 7582: //Ward of the Raptor III
		return 2;
	case 7583: //Ward of the Garou I
		return 2;
	case 7584: //Ward of the Garou II
		return 2;
	case 7585: //Ward of the Garou III
		return 2;
	case 7589: //Fire Skin I
		return 21;
	case 7590: //Fire Skin II
		return 21;
	case 7591: //Fire Skin III
		return 21;
	case 7592: //Fire Skin IV
		return 21;
	case 7593: //Fire Skin V
		return 21;
	case 7594: //Fire Skin VI
		return 21;
	case 7595: //Fire Skin VII
		return 21;
	case 7596: //Fire Skin VIII
		return 21;
	case 7597: //Fire Skin IX
		return 21;
	case 7598: //Fire Skin X
		return 21;
	case 7599: //Gargoyle Glance
		return 0;
	case 7701: //Weakening Roots
		return 83;
	case 7729: //Stealth
		return 51;
	case 7745: //Stunning Roar
		return 97;
	case 7746: //Whirlwind
		return 117;
	case 7762: //Maul I
		return 72;
	case 7763: //Maul II
		return 72;
	case 7764: //Maul III
		return 72;
	case 7765: //Maul IV
		return 72;
	case 7766: //Maul V
		return 72;
	case 7767: //Maul VI
		return 72;
	case 7768: //Maul VII
		return 72;
	case 7769: //Maul VIII
		return 72;
	case 7770: //Maul IX
		return 72;
	case 7771: //Maul X
		return 72;
	case 7772: //Maul XI
		return 72;
	case 7773: //Maul XII
		return 72;
	case 7774: //Maul XIII
		return 72;
	case 7775: //Maul XIV
		return 72;
	case 7776: //Mana Bolt I
		return 38;
	case 7777: //Mana Bolt II
		return 38;
	case 7778: //Mana Bolt III
		return 38;
	case 7779: //Mana Bolt IV
		return 38;
	case 7780: //Mana Bolt V
		return 38;
	case 7781: //Mana Bolt VI
		return 38;
	case 7782: //Mana Bolt VII
		return 38;
	case 7783: //Mana Bolt VIII
		return 38;
	case 7784: //Mana Bolt IX
		return 38;
	case 7785: //Mana Bolt X
		return 38;
	case 7786: //Mana Bolt XI
		return 38;
	case 7787: //Mana Bolt XII
		return 38;
	case 7788: //Mana Bolt XIII
		return 38;
	case 7789: //Mana Bolt XIV
		return 38;
	case 7790: //Spirit Sending
		return 64;
	case 7800: //Draygun's Touch
		return 75;
	case 7801: //Draygun's Touch
		return 75;
	case 7802: //Draygun's Touch
		return 75;
	case 7803: //Draygun's Touch
		return 75;
	case 7804: //Draygun's Touch
		return 75;
	case 7805: //Curse of the Nine
		return 75;
	case 7806: //Curse of the Nine
		return 75;
	case 7807: //Curse of the Nine
		return 75;
	case 7808: //Curse of the Nine
		return 75;
	case 7809: //Curse of the Nine
		return 75;
	case 7810: //Blood of the Shadowmane
		return 75;
	case 7811: //Blood of the Shadowmane
		return 75;
	case 7812: //Blood of the Shadowmane
		return 75;
	case 7813: //Blood of the Shadowmane
		return 75;
	case 7814: //Blood of the Shadowmane
		return 75;
	case 7815: //Theft of Rage
		return 75;
	case 7816: //Theft of Rage
		return 75;
	case 7817: //Theft of Rage
		return 75;
	case 7818: //Theft of Rage
		return 75;
	case 7819: //Theft of Rage
		return 75;
	case 7820: //Curse of the Hivequeen
		return 75;
	case 7821: //Curse of the Hivequeen
		return 75;
	case 7822: //Curse of the Hivequeen
		return 75;
	case 7823: //Curse of the Hivequeen
		return 75;
	case 7824: //Curse of the Hivequeen
		return 75;
	case 7838: //Form of Defense IV
		return 48;
	case 7839: //Form of Protection IV
		return 48;
	case 7840: //Form of Endurance IV
		return 48;
	case 7841: //Form of Rejuvenation IV
		return 48;
	case 7994: //Dread Pyre
		return 38;
	case 7995: //Call for Blood
		return 75;
	case 7996: //Call for Blood Recourse
		return 38;
	case 7999: //Corath Venom
		return 75;
	case 8000: //Commanding Voice
		return 64;
	case 8001: //Thief's eyes
		return 64;
	case 8002: //Fists of Wu
		return 64;
	case 8003: //Cry Havoc
		return 64;
	case 8004: //Death's Regret
		return 124;
	case 8005: //Bind Death
		return 124;
	case 8006: //Chromastrike
		return 72;
	case 8007: //Desperate Renewal
		return 42;
	case 8008: //Skin of the Reptile
		return 16;
	case 8009: //Skin of the Rep. Trigger
		return 42;
	case 8010: //Spore Spiral
		return 83;
	case 8011: //Dawnstrike
		return 38;
	case 8012: //Blessing of the Dawn
		return 38;
	case 8015: //Lingering Sloth
		return 16;
	case 8016: //Lingering Sloth Trigger
		return 16;
	case 8017: //Hungry Plague
		return 88;
	case 8018: //Breath of Antraygus
		return 75;
	case 8019: //Warder's Wrath
		return 117;
	case 8020: //Hail of Arrows
		return 72;
	case 8021: //Bestial Empathy
		return 104;
	case 8022: //Fickle Shadows
		return 124;
	case 8023: //Fickle Shadows Recourse
		return 38;
	case 8025: //Touch of Draygun
		return 43;
	case 8026: //Gift of Draygun
		return 47;
	case 8027: //Last Rites
		return 124;
	case 8028: //Last Rites Trigger
		return 124;
	case 8029: //Silent Piety
		return 64;
	case 8030: //Thousand Blades
		return 118;
	case 8031: //Creeping Dreams
		return 35;
	case 8032: //Mana Flare
		return 16;
	case 8033: //Mana Flare Strike
		return 71;
	case 8034: //Colored Chaos
		return 58;
	case 8035: //Echoing Madness
		return 35;
	case 8036: //Illusion: Orc
		return 48;
	case 8037: //Raging Servant
		return 102;
	case 8038: //Burning Aura
		return 16;
	case 8039: //Burning Vengeance
		return 16;
	case 8040: //Fickle Fire
		return 38;
	case 8041: //Clinging Frost
		return 14;
	case 8042: //Clinging Frost Trigger
		return 14;
	case 8043: //Ether Flame
		return 38;
	case 8044: //Mana Weave
		return 58;
	case 8045: //Mana Weave Recourse
		return 38;
	case 8075: //Fickle Fire Recourse
		return 38;
	case 8090: //Armor Cleave I
		return 81;
	case 8091: //Armor Cleave II
		return 81;
	case 8092: //Armor Cleave III
		return 81;
	case 8093: //Armor Cleave IV
		return 81;
	case 8094: //Armor Cleave V
		return 81;
	case 8095: //Armor Cleave VI
		return 81;
	case 8096: //Armor Cleave VII
		return 81;
	case 8097: //Armor Cleave VIII
		return 81;
	case 8098: //Armor Cleave IX
		return 81;
	case 8099: //Armor Cleave X
		return 81;
	case 8106: //Perfected Heal
		return 42;
	case 8114: //Shrieker Sonic Wave
		return 38;
	case 8115: //Shrieker Sonic Wave
		return 38;
	case 8116: //Shrieker Sonic Wave
		return 38;
	case 8117: //Nimbus Shrieker Wave
		return 38;
	case 8118: //Nimbus Shrieker Wave
		return 38;
	case 8119: //Nimbus Shrieker Wave
		return 38;
	case 8120: //Retch Spore
		return 58;
	case 8121: //Retch Spore
		return 58;
	case 8122: //Retch Spore
		return 58;
	case 8123: //Hammer Time
		return 58;
	case 8144: //Net
		return 89;
	case 8145: //Clinging Net
		return 89;
	case 8149: //Stealthy Getaway
		return 64;
	case 8153: //Eternal Thought
		return 59;
	case 8171: //Pure Thought I
		return 59;
	case 8172: //Pure Thought II
		return 59;
	case 8173: //Pure Thought III
		return 59;
	case 8174: //Pure Thought IV
		return 59;
	case 8175: //Pure Thought V
		return 59;
	case 8176: //Pure Thought VI
		return 59;
	case 8177: //Pure Thought VII
		return 59;
	case 8178: //Pure Thought VIII
		return 59;
	case 8179: //Pure Thought IX
		return 59;
	case 8180: //Pure Thought X
		return 59;
	case 8200: //Gift of Illsalin
		return 59;
	case 8201: //Guardian of Ro
		return 102;
	case 8202: //Guardian of Ro
		return 102;
	case 8203: //Guardian of Ro
		return 102;
	case 8204: //Guardian's Bolt I
		return 38;
	case 8210: //Feral Roar I
		return 115;
	case 8211: //Feral Roar II
		return 115;
	case 8212: //Feral Roar III
		return 115;
	case 8213: //Feral Roar IV
		return 115;
	case 8214: //Greater Rabid Bear
		return 48;
	case 8215: //Greater Rabid Bear
		return 48;
	case 8216: //Greater Rabid Bear
		return 48;
	case 8218: //Ancestral Guard
		return 62;
	case 8219: //Ancestral Guard
		return 62;
	case 8220: //Ancestral Guard
		return 62;
	case 8233: //Empathic Fury
		return 118;
	case 8234: //Empathic Fury
		return 118;
	case 8235: //Circle of Undershore
		return 5;
	case 8236: //Undershore Portal
		return 5;
	case 8237: //Ring of Undershore
		return 5;
	case 8238: //Undershore Gate
		return 5;
	case 8239: //Translocate: Undershore
		return 5;
	case 8267: //Feral Roar V
		return 115;
	case 8268: //Feral Roar VI
		return 115;
	case 8275: //Infection Test 1
		return 35;
	case 8276: //Infection Test 2
		return 35;
	case 8277: //Fling
		return 58;
	case 8278: //Fetter of Spirits
		return 71;
	case 8280: //Boon of Vitality I
		return 46;
	case 8281: //Boon of Vitality II
		return 46;
	case 8282: //Boon of Vitality III
		return 46;
	case 8283: //Boon of Vitality IV
		return 46;
	case 8284: //Boon of Vitality V
		return 46;
	case 8285: //Boon of Vitality VI
		return 46;
	case 8286: //Boon of Vitality VII
		return 46;
	case 8287: //Boon of Vitality VIII
		return 46;
	case 8288: //Boon of Vitality IX
		return 46;
	case 8289: //Boon of Vitality X
		return 46;
	case 8290: //Gift of Speed I
		return 41;
	case 8291: //Gift of Speed II
		return 41;
	case 8292: //Gift of Speed III
		return 41;
	case 8293: //Gift of Speed IV
		return 41;
	case 8294: //Gift of Speed V
		return 41;
	case 8295: //Gift of Speed VI
		return 41;
	case 8296: //Gift of Speed VII
		return 41;
	case 8297: //Gift of Speed VIII
		return 41;
	case 8298: //Gift of Speed IX
		return 41;
	case 8299: //Gift of Speed X
		return 41;
	case 8300: //Malaise I
		return 81;
	case 8301: //Malaise II
		return 81;
	case 8302: //Malaise III
		return 81;
	case 8303: //Malaise IV
		return 81;
	case 8304: //Malaise V
		return 81;
	case 8305: //Stun I
		return 97;
	case 8306: //Stun II
		return 97;
	case 8307: //Stun III
		return 97;
	case 8308: //Stun IV
		return 97;
	case 8309: //Stun V
		return 97;
	case 8310: //Gaze of the Beholder I
		return 13;
	case 8311: //Gaze of the Beholder II
		return 13;
	case 8312: //Gaze of the Beholder III
		return 13;
	case 8313: //Gaze of the Beholder IV
		return 13;
	case 8314: //Gaze of the Beholder V
		return 13;
	case 8315: //Gaze of the Beholder VI
		return 13;
	case 8316: //Gaze of the Beholder VII
		return 13;
	case 8317: //Gaze of the Beholder VIII
		return 13;
	case 8318: //Gaze of the Beholder IX
		return 13;
	case 8319: //Gaze of the Beholder X
		return 13;
	case 8320: //Gaze of the Beholder XI
		return 13;
	case 8321: //Gaze of the Beholder XII
		return 13;
	case 8322: //Gaze of the Beholder XIII
		return 13;
	case 8323: //Gaze of the Beholder XIV
		return 13;
	case 8324: //Pure Water I
		return 19;
	case 8325: //Pure Water II
		return 19;
	case 8326: //Pure Water III
		return 19;
	case 8327: //Pure Water IV
		return 19;
	case 8328: //Gale Force
		return 97;
	case 8329: //Fungal Regrowth I
		return 32;
	case 8330: //Fungal Regrowth II
		return 32;
	case 8331: //Fungal Regrowth III
		return 32;
	case 8332: //Fungal Regrowth IV
		return 32;
	case 8333: //Fungal Regrowth V
		return 32;
	case 8334: //Creeping Plague
		return 16;
	case 8335: //Creeping Plague Trigger
		return 16;
	case 8336: //Stunning Blow I
		return 97;
	case 8337: //Stunning Blow II
		return 97;
	case 8338: //Stunning Blow III
		return 97;
	case 8339: //Stunning Blow IV
		return 97;
	case 8340: //Stunning Blow V
		return 97;
	case 8341: //Dark Gift I
		return 17;
	case 8342: //Dark Gift II
		return 17;
	case 8343: //Dark Gift III
		return 17;
	case 8344: //Dark Gift IV
		return 17;
	case 8345: //Dark Gift V
		return 17;
	case 8346: //Dark Gift VI
		return 17;
	case 8347: //Dark Gift VII
		return 17;
	case 8348: //Dark Siphon I
		return 43;
	case 8349: //Dark Siphon II
		return 43;
	case 8350: //Dark Siphon III
		return 43;
	case 8351: //Dark Siphon IV
		return 43;
	case 8352: //Dark Siphon V
		return 43;
	case 8353: //Dark Siphon VI
		return 43;
	case 8354: //Dark Siphon VII
		return 43;
	case 8372: //Stone Skin I
		return 84;
	case 8373: //Stone Skin II
		return 84;
	case 8374: //Stone Skin III
		return 84;
	case 8375: //Stone Skin IV
		return 84;
	case 8376: //Stone Skin V
		return 84;
	case 8377: //Stone Skin VI
		return 84;
	case 8378: //Shadowed Dark Hold
		return 35;
	case 8379: //Shadowed Word of Chaos
		return 13;
	case 8380: //Shadowed Curse of Mori
		return 58;
	case 8381: //Shadowed Meteor Storm
		return 38;
	case 8382: //Shadowed Corona Flare
		return 38;
	case 8383: //Shadowed Core Fire
		return 38;
	case 8400: //Guardian's Bolt II
		return 38;
	case 8401: //Guardian's Bolt III
		return 38;
	case 8410: //Hand of Holy Vengeance I
		return 0;
	case 8411: //Hand of Holy Vengeance II
		return 0;
	case 8412: //Hand of Holy Vengeance III
		return 0;
	case 8413: //Hand of Holy Vengeance IV
		return 0;
	case 8414: //Hand of Holy Vengeance V
		return 0;
	case 8421: //Jailor's Fury
		return 58;
	case 8444: //Blinding Dust
		return 72;
	default:
		return 0;
	}
}
static inline std::string GetSpellCategoryName(DWORD categoryID)
{
	switch (categoryID)
	{
	case 1: return "Aegolism";
	case 2: return "Agility";
	case 3: return "Alliance";
	case 4: return "Animal";
	case 5: return "Antonica";
	case 6: return "Armor Class";
	case 7: return "Attack";
	case 8: return "Bane";
	case 9: return "Blind";
	case 10: return "Block";
	case 11: return "Calm";
	case 12: return "Charisma";
	case 13: return "Charm";
	case 14: return "Cold";
	case 15: return "Combat Abilities";
	case 16: return "Combat Innates";
	case 17: return "Conversions";
	case 18: return "Create Item";
	case 19: return "Cure";
	case 20: return "Damage Over Time";
	case 21: return "Damage Shield";
	case 22: return "Defensive";
	case 23: return "Destroy";
	case 24: return "Dexterity";
	case 25: return "Direct Damage ";
	case 26: return "Disarm Traps";
	case 27: return "Disciplines";
	case 28: return "Discord";
	case 29: return "Disease";
	case 30: return "Disempowering";
	case 31: return "Dispel";
	case 32: return "Duration Heals";
	case 33: return "Duration Tap";
	case 34: return "Enchant Metal";
	case 35: return "Enthrall";
	case 36: return "Faydwer";
	case 37: return "Fear";
	case 38: return "Fire";
	case 39: return "Fizzle Rate";
	case 40: return "Fumble";
	case 41: return "Haste";
	case 42: return "Heals";
	case 43: return "Health";
	case 44: return "Health/Mana";
	case 45: return "HP Buffs";
	case 46: return "HP type one";
	case 47: return "HP type two";
	case 48: return "Illusion: Other";
	case 49: return "Illusion: Player";
	case 50: return "Imbue Gem";
	case 51: return "Invisibility";
	case 52: return "Invulnerability";
	case 53: return "Jolt";
	case 54: return "Kunark";
	case 55: return "Levitate";
	case 56: return "Life Flow";
	case 57: return "Luclin";
	case 58: return "Magic";
	case 59: return "Mana";
	case 60: return "Mana Drain";
	case 61: return "Mana Flow";
	case 62: return "Melee Guard";
	case 63: return "Memory Blur";
	case 64: return "Misc";
	case 65: return "Movement";
	case 66: return "Objects";
	case 67: return "Odus";
	case 68: return "Offensive";
	case 69: return "Pet";
	case 70: return "Pet Haste";
	case 71: return "Pet Misc Buffs";
	case 72: return "Physical";
	case 73: return "Picklock";
	case 74: return "Plant";
	case 75: return "Poison";
	case 76: return "Power Tap";
	case 77: return "Quick Heal";
	case 78: return "Reflection";
	case 79: return "Regen";
	case 80: return "Resist Buff";
	case 81: return "Resist Debuffs";
	case 82: return "Resurrection";
	case 83: return "Root";
	case 84: return "Rune";
	case 85: return "Sense Trap";
	case 86: return "Shadowstep";
	case 87: return "Shielding";
	case 88: return "Slow";
	case 89: return "Snare";
	case 90: return "Special";
	case 91: return "Spell Focus";
	case 92: return "Spell Guard";
	case 93: return "Spellshield";
	case 94: return "Stamina";
	case 95: return "Statistic Buffs";
	case 96: return "Strength";
	case 97: return "Stun";
	case 98: return "Sum: Air";
	case 99: return "Sum: Animation";
	case 100: return "Sum: Earth";
	case 101: return "Sum: Familiar";
	case 102: return "Sum: Fire";
	case 103: return "Sum: Undead";
	case 104: return "Sum: Warder";
	case 105: return "Sum: Water";
	case 106: return "Summon Armor";
	case 107: return "Summon Focus";
	case 108: return "Summon Food/Water";
	case 109: return "Summon Utility";
	case 110: return "Summon Weapon";
	case 111: return "Summoned";
	case 112: return "Symbol";
	case 113: return "Taelosia";
	case 114: return "Taps";
	case 115: return "Techniques";
	case 116: return "The Planes";
	case 117: return "Timer 1";
	case 118: return "Timer 2";
	case 119: return "Timer 3";
	case 120: return "Timer 4";
	case 121: return "Timer 5";
	case 122: return "Timer 6";
	case 123: return "Transport";
	case 124: return "Undead";
	case 125: return "Utility Beneficial";
	case 126: return "Utility Detrimental";
	case 127: return "Velious";
	case 128: return "Visages";
	case 129: return "Vision";
	case 130: return "Wisdom/Intelligence";
	case 131: return "Traps";
	case 132: return "Auras";
	case 133: return "Endurance";
	case 134: return "Serpent's Spine";
	case 135: return "Corruption";
	case 136: return "Learning";
	case 137: return "Chromatic";
	case 138: return "Prismatic";
	case 139: return "Sum: Swarm";
	case 140: return "Delayed";
	case 141: return "Temporary";
	case 142: return "Twincast";
	case 143: return "Sum: Bodyguard";
	case 144: return "Humanoid";
	case 145: return "Haste/Spell Focus";
	case 146: return "Timer 7";
	case 147: return "Timer 8";
	case 148: return "Timer 9";
	case 149: return "Timer 10";
	case 150: return "Timer 11";
	case 151: return "Timer 12";
	case 152: return "Hatred";
	case 153: return "Fast";
	case 154: return "Illusion: Special";
	case 155: return "Timer 13";
	case 156: return "Timer 14";
	case 157: return "Timer 15";
	case 158: return "Timer 16";
	case 159: return "Timer 17";
	case 160: return "Timer 18";
	case 161: return "Timer 19";
	case 162: return "Timer 20";
	case 163: return "Alaris";
	default: return "Unknown";
	}
}
static inline std::string GetSpellSubCategoryName(DWORD subcategoryID)
{
	switch (subcategoryID)
	{
	case 1: return "Aegolism";
	case 2: return "Agility";
	case 3: return "Alliance";
	case 4: return "Animal";
	case 5: return "Antonica";
	case 6: return "Armor Class";
	case 7: return "Attack";
	case 8: return "Bane";
	case 9: return "Blind";
	case 10: return "Block";
	case 11: return "Calm";
	case 12: return "Charisma";
	case 13: return "Charm";
	case 14: return "Cold";
	case 15: return "Combat Abilities";
	case 16: return "Combat Innates";
	case 17: return "Conversions";
	case 18: return "Create Item";
	case 19: return "Cure";
	case 20: return "Damage Over Time";
	case 21: return "Damage Shield";
	case 22: return "Defensive";
	case 23: return "Destroy";
	case 24: return "Dexterity";
	case 25: return "Direct Damage ";
	case 26: return "Disarm Traps";
	case 27: return "Disciplines";
	case 28: return "Discord";
	case 29: return "Disease";
	case 30: return "Disempowering";
	case 31: return "Dispel";
	case 32: return "Duration Heals";
	case 33: return "Duration Tap";
	case 34: return "Enchant Metal";
	case 35: return "Enthrall";
	case 36: return "Faydwer";
	case 37: return "Fear";
	case 38: return "Fire";
	case 39: return "Fizzle Rate";
	case 40: return "Fumble";
	case 41: return "Haste";
	case 42: return "Heals";
	case 43: return "Health";
	case 44: return "Health/Mana";
	case 45: return "HP Buffs";
	case 46: return "HP type one";
	case 47: return "HP type two";
	case 48: return "Illusion: Other";
	case 49: return "Illusion: Player";
	case 50: return "Imbue Gem";
	case 51: return "Invisibility";
	case 52: return "Invulnerability";
	case 53: return "Jolt";
	case 54: return "Kunark";
	case 55: return "Levitate";
	case 56: return "Life Flow";
	case 57: return "Luclin";
	case 58: return "Magic";
	case 59: return "Mana";
	case 60: return "Mana Drain";
	case 61: return "Mana Flow";
	case 62: return "Melee Guard";
	case 63: return "Memory Blur";
	case 64: return "Misc";
	case 65: return "Movement";
	case 66: return "Objects";
	case 67: return "Odus";
	case 68: return "Offensive";
	case 69: return "Pet";
	case 70: return "Pet Haste";
	case 71: return "Pet Misc Buffs";
	case 72: return "Physical";
	case 73: return "Picklock";
	case 74: return "Plant";
	case 75: return "Poison";
	case 76: return "Power Tap";
	case 77: return "Quick Heal";
	case 78: return "Reflection";
	case 79: return "Regen";
	case 80: return "Resist Buff";
	case 81: return "Resist Debuffs";
	case 82: return "Resurrection";
	case 83: return "Root";
	case 84: return "Rune";
	case 85: return "Sense Trap";
	case 86: return "Shadowstep";
	case 87: return "Shielding";
	case 88: return "Slow";
	case 89: return "Snare";
	case 90: return "Special";
	case 91: return "Spell Focus";
	case 92: return "Spell Guard";
	case 93: return "Spellshield";
	case 94: return "Stamina";
	case 95: return "Statistic Buffs";
	case 96: return "Strength";
	case 97: return "Stun";
	case 98: return "Sum: Air";
	case 99: return "Sum: Animation";
	case 100: return "Sum: Earth";
	case 101: return "Sum: Familiar";
	case 102: return "Sum: Fire";
	case 103: return "Sum: Undead";
	case 104: return "Sum: Warder";
	case 105: return "Sum: Water";
	case 106: return "Summon Armor";
	case 107: return "Summon Focus";
	case 108: return "Summon Food/Water";
	case 109: return "Summon Utility";
	case 110: return "Summon Weapon";
	case 111: return "Summoned";
	case 112: return "Symbol";
	case 113: return "Taelosia";
	case 114: return "Taps";
	case 115: return "Techniques";
	case 116: return "The Planes";
	case 117: return "Timer 1";
	case 118: return "Timer 2";
	case 119: return "Timer 3";
	case 120: return "Timer 4";
	case 121: return "Timer 5";
	case 122: return "Timer 6";
	case 123: return "Transport";
	case 124: return "Undead";
	case 125: return "Utility Beneficial";
	case 126: return "Utility Detrimental";
	case 127: return "Velious";
	case 128: return "Visages";
	case 129: return "Vision";
	case 130: return "Wisdom/Intelligence";
	case 131: return "Traps";
	case 132: return "Auras";
	case 133: return "Endurance";
	case 134: return "Serpent's Spine";
	case 135: return "Corruption";
	case 136: return "Learning";
	case 137: return "Chromatic";
	case 138: return "Prismatic";
	case 139: return "Sum: Swarm";
	case 140: return "Delayed";
	case 141: return "Temporary";
	case 142: return "Twincast";
	case 143: return "Sum: Bodyguard";
	case 144: return "Humanoid";
	case 145: return "Haste/Spell Focus";
	case 146: return "Timer 7";
	case 147: return "Timer 8";
	case 148: return "Timer 9";
	case 149: return "Timer 10";
	case 150: return "Timer 11";
	case 151: return "Timer 12";
	case 152: return "Hatred";
	case 153: return "Fast";
	case 154: return "Illusion: Special";
	case 155: return "Timer 13";
	case 156: return "Timer 14";
	case 157: return "Timer 15";
	case 158: return "Timer 16";
	case 159: return "Timer 17";
	case 160: return "Timer 18";
	case 161: return "Timer 19";
	case 162: return "Timer 20";
	case 163: return "Alaris";
	default: return "Unknown";
	}
}
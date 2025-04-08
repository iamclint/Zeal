#pragma once
//some structure information was found here
//https://github.com/kevrgithub/eqmac/
//https://github.com/MMOBugs/EQMac

#include <Windows.h>
#include "vectors.h"
#define EQ_NUM_COMMANDS 277 // 0-276
#define EQ_NUM_BUFFS 15
#define EQ_NUM_SPELLS 4000
#define EQ_NUM_SPELL_GEMS 8
#define EQ_NUM_GROUP_MEMBERS 5
#define EQ_NUM_INVENTORY_SLOTS 21
#define EQ_NUM_INVENTORY_PACK_SLOTS 8
#define EQ_NUM_INVENTORY_BANK_SLOTS 8
#define EQ_NUM_SKILLS 74
#define EQ_NUM_SPELL_BOOK_SPELLS 512
#define EQ_NUM_SPAWNS 8192
#define EQ_NUM_GUILDS 512
#define EQ_NUM_LOOT_WINDOW_ITEMS 30
#define EQ_NUM_HOTBUTTONS 10
#define EQ_NUM_HOTBUTTONS_TOTAL 100
#define EQ_NUM_CONTAINER_SLOTS 10
#define GAMESTATE_CHARSELECT    1
#define GAMESTATE_SOMETHING     4
#define GAMESTATE_INGAME        5
#define GAMESTATE_PRECHARSELECT 6
#define GAMESTATE_LOGGINGIN     253
#define GAMESTATE_UNLOADING     255


#define USERCOLOR_SAY                   0xFF +   1 //  1  - Say
#define USERCOLOR_TELL                  0xFF +   2 //  2  - Tell
#define USERCOLOR_GROUP                 0xFF +   3 //  3  - Group
#define USERCOLOR_GUILD                 0xFF +   4 //  4  - Guild
#define USERCOLOR_OOC                   0xFF +   5 //  5  - OOC
#define USERCOLOR_AUCTION               0xFF +   6 //  6  - Auction
#define USERCOLOR_SHOUT                 0xFF +   7 //  7  - Shout
#define USERCOLOR_EMOTE                 0xFF +   8 //  8  - Emote
#define USERCOLOR_SPELLS                0xFF +   9 //  9  - Spells (meming, scribing, casting, etc.)
#define USERCOLOR_YOU_HIT_OTHER         0xFF +  10 //  10 - You hit other
#define USERCOLOR_OTHER_HIT_YOU         0xFF +  11 //  11 - Other hits you
#define USERCOLOR_YOU_MISS_OTHER        0xFF +  12 //  12 - You miss other
#define USERCOLOR_OTHER_MISS_YOU        0xFF +  13 //  13 - Other misses you
#define USERCOLOR_DUELS                 0xFF +  14 //  14 - Some broadcasts (duels)
#define USERCOLOR_SKILLS                0xFF +  15 //  15 - Skills (ups, non-combat use, etc.)
#define USERCOLOR_DISCIPLINES           0xFF +  16 //  16 - Disciplines or special abilities
#define USERCOLOR_UNUSED001             0xFF +  17 //  17 - Unused at this time
#define USERCOLOR_DEFAULT               0xFF +  18 //  18 - Default text and stuff you type
#define USERCOLOR_UNUSED002             0xFF +  19 //  19 - Unused at this time
#define USERCOLOR_MERCHANT_OFFER        0xFF +  20 //  20 - Merchant Offer Price
#define USERCOLOR_MERCHANT_EXCHANGE     0xFF +  21 //  21 - Merchant Buy/Sell
#define USERCOLOR_YOUR_DEATH            0xFF +  22 //  22 - Your death message
#define USERCOLOR_OTHER_DEATH           0xFF +  23 //  23 - Others death message
#define USERCOLOR_OTHER_HIT_OTHER       0xFF +  24 //  24 - Other damage other
#define USERCOLOR_OTHER_MISS_OTHER      0xFF +  25 //  25 - Other miss other
#define USERCOLOR_WHO                   0xFF +  26 //  26 - /who command
#define USERCOLOR_YELL                  0xFF +  27 //  27 - yell for help
#define USERCOLOR_NON_MELEE             0xFF +  28 //  28 - Hit for non-melee
#define USERCOLOR_SPELL_WORN_OFF        0xFF +  29 //  29 - Spell worn off
#define USERCOLOR_MONEY_SPLIT           0xFF +  30 //  30 - Money splits
#define USERCOLOR_LOOT                  0xFF +  31 //  31 - Loot message
#define USERCOLOR_RANDOM                0xFF +  32 //  32 - Dice Roll (/random)
#define USERCOLOR_OTHERS_SPELLS         0xFF +  33 //  33 - Others spells
#define USERCOLOR_SPELL_FAILURE         0xFF +  34 //  34 - Spell Failures (resists, fizzles, missing component, bad target, etc.)
#define USERCOLOR_CHAT_CHANNEL          0xFF +  35 //  35 - Chat Channel Messages
#define USERCOLOR_CHAT_1                0xFF +  36 //  36 - Chat Channel 1
#define USERCOLOR_CHAT_2                0xFF +  37 //  37 - Chat Channel 2
#define USERCOLOR_CHAT_3                0xFF +  38 //  38 - Chat Channel 3
#define USERCOLOR_CHAT_4                0xFF +  39 //  39 - Chat Channel 4
#define USERCOLOR_CHAT_5                0xFF +  40 //  40 - Chat Channel 5
#define USERCOLOR_CHAT_6                0xFF +  41 //  41 - Chat Channel 6
#define USERCOLOR_CHAT_7                0xFF +  42 //  42 - Chat Channel 7
#define USERCOLOR_CHAT_8                0xFF +  43 //  43 - Chat Channel 8
#define USERCOLOR_CHAT_9                0xFF +  44 //  44 - Chat Channel 9
#define USERCOLOR_CHAT_10               0xFF +  45 //  45 - Chat Channel 10
#define USERCOLOR_MELEE_CRIT            0xFF +  46 //  46 - Melee Crits
#define USERCOLOR_SPELL_CRIT            0xFF +  47 //  47 - Spell Crits
#define USERCOLOR_TOO_FAR_AWAY          0xFF +  48 //  48 - Too far away (melee)
#define USERCOLOR_NPC_RAMAGE            0xFF +  49 //  49 - NPC Rampage
#define USERCOLOR_NPC_FURRY             0xFF +  50 //  50 - NPC Furry
#define USERCOLOR_NPC_ENRAGE            0xFF +  51 //  51 - NPC Enrage
#define USERCOLOR_ECHO_SAY              0xFF +  52 //  52 - say echo
#define USERCOLOR_ECHO_TELL             0xFF +  53 //  53 - tell echo
#define USERCOLOR_ECHO_GROUP            0xFF +  54 //  54 - group echo
#define USERCOLOR_ECHO_GUILD            0xFF +  55 //  55 - guild echo
#define USERCOLOR_ECHO_OOC              0xFF +  56 //  56 - group echo
#define USERCOLOR_ECHO_AUCTION          0xFF +  57 //  57 - auction echo
#define USERCOLOR_ECHO_SHOUT            0xFF +  58 //  58 - shout echo
#define USERCOLOR_ECHO_EMOTE            0xFF +  59 //  59 - emote echo
#define USERCOLOR_ECHO_CHAT_1           0xFF +  60 //  60 - chat 1 echo
#define USERCOLOR_ECHO_CHAT_2           0xFF +  61 //  61 - chat 2 echo
#define USERCOLOR_ECHO_CHAT_3           0xFF +  62 //  62 - chat 3 echo
#define USERCOLOR_ECHO_CHAT_4           0xFF +  63 //  63 - chat 4 echo
#define USERCOLOR_ECHO_CHAT_5           0xFF +  64 //  64 - chat 5 echo
#define USERCOLOR_ECHO_CHAT_6           0xFF +  65 //  65 - chat 6 echo
#define USERCOLOR_ECHO_CHAT_7           0xFF +  66 //  66 - chat 7 echo
#define USERCOLOR_ECHO_CHAT_8           0xFF +  67 //  67 - chat 8 echo
#define USERCOLOR_ECHO_CHAT_9           0xFF +  68 //  68 - chat 9 echo
#define USERCOLOR_ECHO_CHAT_10          0xFF +  69 //  69 - chat 10 echo
#define USERCOLOR_RAID_SAY              0xFF +  72 //  72 - raid say
#define USERCOLOR_ECHO_AUTOSPLIT        0xFF +  73 

constexpr WORD kInvalidSpellId = 0xffff; // spell_id used when not casting or empty spell gem

namespace Zeal
{
	namespace EqEnums
	{
		enum EntityTypes
		{
			Player,
			NPC,
			NPCCorpse,
			PlayerCorpse,
			Unknown
		};
		enum CameraView
		{
			FirstPerson,
			ThirdPerson1,
			ThirdPerson2,
			ThirdPerson3,
			ThirdPerson4,
			CharacterSelect,
			ZealCam,
			TotalCameras
		};
		enum Stance
		{
			Sitting = 110,
			Ducking = 111,
			Standing = 100,
			Frozen = 102,
			Looting = 105,
			Feigned = 115,
			Dead = 120
		};
		enum ClassTypes
		{
			Warrior = 1,
			Cleric = 2,
			Paladin = 3,
			Ranger = 4,
			Shadowknight = 5,
			Druid = 6,
			Monk = 7,
			Bard = 8,
			Rogue = 9,
			Shaman = 10,
			Necromancer = 11,
			Wizard = 12,
			Magician = 13,
			Enchanter = 14,
			Beastlord = 15,
			Banker = 16,
			Merchant = 32
		};
		enum PetCommand
		{
			None = 0,
			Back = 1,
			GoAway = 2,
			Health = 4,
			Guard = 5,
			GuardMe = 6,
			Attack = 7,
			Follow = 8,
			Sit = 9,
			Stand = 10,
			Taunt = 11
		};
		enum SpellTargetType
		{
			/* 01 */	TargetOptional = 0x01,
			/* 02 */	AEClientV1 = 0x02,
			/* 03 */	GroupV1 = 0x03,
			/* 04 */	PBAE = 0x04,
			/* 05 */	Target = 0x05,
			/* 06 */	Self = 0x06,
			/* 07 */	// NOT USED
			/* 08 */	TargetedAE = 0x08,
			/* 09 */	Animal = 0x09,
			/* 10 */	Undead = 0x0a,
			/* 11 */	Summoned = 0x0b,
			/* 12 */	// NOT USED
			/* 13 */	Tap = 0x0d,
			/* 14 */	Pet = 0x0e,
			/* 15 */	Corpse = 0x0f,
			/* 16 */	Plant = 0x10,
			/* 17 */	UberGiant = 0x11, //special giant
			/* 18 */	UberDragon = 0x12, //special dragon
			/* 19 */	// NOT USED
			/* 20 */	TargetedAETap = 0x14,
			/* 21 */	// NOT USED
			/* 22 */	// NOT USED
			/* 23 */	// NOT USED
			/* 24 */	UndeadAE = 0x18,
			/* 25 */	SummonedAE = 0x19,
			/* 26 */	// NOT USED
			/* 27 */	// NOT USED
			/* 28 */	// NOT USED
			/* 29 */	// NOT USED
			/* 30 */	// NOT USED
			/* 31 */	// NOT USED
			/* 32 */	// NOT USED
			/* 33 */	// NOT USED
			/* 34 */	// NOT USED
			/* 35 */	// NOT USED
			/* 36 */	// NOT USED
			/* 37 */	// NOT USED
			/* 38 */	// NOT USED
			/* 39 */	// NOT USED
			/* 40 */	AEBard = 0x28,
			/* 41 */	GroupV2 = 0x29,
			/* 42 */	// NOT USED
			/* 43 */	ProjectIllusion = 0x2b, // Not found in spell data, used internally.

		};

		enum ItemEffect {
			ItemEffectCombatProc = 0,
			ItemEffectClick,
			ItemEffectWorn,
			ItemEffectExpendable,
			ItemEffectMustEquipClick,
			ItemEffectCanEquipClick,
			ItemEffectFocus,
			ItemEffectScroll,
			ItemEffectCount
		};

		namespace EquipSlot {  // Avoids 'Back' collision; enum class is inconvenient.
		enum EquipSlot
		{
			LeftEar = 0,
			Head,
			Face,
			RightEar,
			Neck,
			Shoulder,
			Arms,
			Back,
			LeftWrist,
			RightWrist,
			Range,
			Hands,
			Primary,
			Secondary,
			LeftFinger,
			RightFinger,
			Chest,
			Legs,
			Feet,
			Waist,
			Ammo
		};
		}  // namespace EquipGroup

		enum ItemTypes  // From EQMacEmu/common/item_data.h
		{
			/*9138*/	ItemType1HSlash = 0,
			/*9141*/	ItemType2HSlash,
			/*9140*/	ItemType1HPiercing,
			/*9139*/	ItemType1HBlunt,
			/*9142*/	ItemType2HBlunt,
			/*5504*/	ItemTypeBow,
			/*----*/	ItemTypeUnknown1,
			/*----*/	ItemTypeLargeThrowing,
			/*5505*/	ItemTypeShield,
			/*5506*/	ItemTypeScroll,
			/*5507*/	ItemTypeArmor,
			/*5508*/	ItemTypeMisc,			// a lot of random crap has this item use.
			/*7564*/	ItemTypeLockPick,
			/*----*/	ItemTypeUnknown2,
			/*5509*/	ItemTypeFood,
			/*5510*/	ItemTypeDrink,
			/*5511*/	ItemTypeLight,
			/*5512*/	ItemTypeCombinable,		// not all stackable items are this use...
			/*5513*/	ItemTypeBandage,
			/*----*/	ItemTypeSmallThrowing,
			/*----*/	ItemTypeSpell,			// spells and tomes
			/*5514*/	ItemTypePotion,
			/*----*/	ItemTypeFletchedArrows,
			/*0406*/	ItemTypeWindInstrument,
			/*0407*/	ItemTypeStringedInstrument,
			/*0408*/	ItemTypeBrassInstrument,
			/*0405*/	ItemTypePercussionInstrument,
			/*5515*/	ItemTypeArrow,
			/*----*/	ItemTypeUnknown4,
			/*5521*/	ItemTypeJewelry,
			/*----*/	ItemTypeSkull,
			/*5516*/	ItemTypeBook,
			/*5517*/	ItemTypeNote,
			/*5518*/	ItemTypeKey,
			/*----*/	ItemTypeCoin,
			/*5520*/	ItemType2HPiercing,
			/*----*/	ItemTypeFishingPole,
			/*----*/	ItemTypeFishingBait,
			/*5519*/	ItemTypeAlcohol,
			/*----*/	ItemTypeKey2,
			/*----*/	ItemTypeCompass,
			/*----*/	ItemTypeUnknown5,
			/*----*/	ItemTypePoison,			// might be wrong, but includes poisons
			/*----*/	ItemTypeUnknown6,
			/*----*/	ItemTypeUnknown7,
			/*5522*/	ItemTypeMartial,
			/*----*/	ItemTypeCount
		};

		enum SkillType  // From EQMacEmu/common/skills.h
		{
			/*13855*/	Skill1HBlunt = 0,
			/*13856*/	Skill1HSlashing,
			/*13857*/	Skill2HBlunt,
			/*13858*/	Skill2HSlashing,
			/*13859*/	SkillAbjuration,
			/*13861*/	SkillAlteration,
			/*13862*/	SkillApplyPoison,
			/*13863*/	SkillArchery,
			/*13864*/	SkillBackstab,
			/*13866*/	SkillBindWound,
			/*13867*/	SkillBash,
			/*13871*/	SkillBlock,
			/*13872*/	SkillBrassInstruments,
			/*13874*/	SkillChanneling,
			/*13875*/	SkillConjuration,
			/*13876*/	SkillDefense,
			/*13877*/	SkillDisarm,
			/*13878*/	SkillDisarmTraps,
			/*13879*/	SkillDivination,
			/*13880*/	SkillDodge,
			/*13881*/	SkillDoubleAttack,
			/*13882*/	SkillDragonPunch,
			/*13924*/	SkillTailRake = SkillDragonPunch, // Iksar Monk equivilent
			/*13883*/	SkillDualWield,
			/*13884*/	SkillEagleStrike,
			/*13885*/	SkillEvocation,
			/*13886*/	SkillFeignDeath,
			/*13888*/	SkillFlyingKick,
			/*13889*/	SkillForage,
			/*13890*/	SkillHandtoHand,
			/*13891*/	SkillHide,
			/*13893*/	SkillKick,
			/*13894*/	SkillMeditate,
			/*13895*/	SkillMend,
			/*13896*/	SkillOffense,
			/*13897*/	SkillParry,
			/*13899*/	SkillPickLock,
			/*13900*/	Skill1HPiercing,				// Changed in RoF2(05-10-2013)
			/*13903*/	SkillRiposte,
			/*13904*/	SkillRoundKick,
			/*13905*/	SkillSafeFall,
			/*13906*/	SkillSenseHeading,
			/*13908*/	SkillSinging,
			/*13909*/	SkillSneak,
			/*13910*/	SkillSpecializeAbjure,			// No idea why they truncated this one..especially when there are longer ones...
			/*13911*/	SkillSpecializeAlteration,
			/*13912*/	SkillSpecializeConjuration,
			/*13913*/	SkillSpecializeDivination,
			/*13914*/	SkillSpecializeEvocation,
			/*13915*/	SkillPickPockets,
			/*13916*/	SkillStringedInstruments,
			/*13917*/	SkillSwimming,
			/*13919*/	SkillThrowing,
			/*13920*/	SkillTigerClaw,
			/*13921*/	SkillTracking,
			/*13923*/	SkillWindInstruments,
			/*13854*/	SkillFishing,
			/*13853*/	SkillMakePoison,
			/*13852*/	SkillTinkering,
			/*13851*/	SkillResearch,
			/*13850*/	SkillAlchemy,
			/*13865*/	SkillBaking,
			/*13918*/	SkillTailoring,
			/*13907*/	SkillSenseTraps,
			/*13870*/	SkillBlacksmithing,
			/*13887*/	SkillFletching,
			/*13873*/	SkillBrewing,
			/*13860*/	SkillAlcoholTolerance,
			/*13868*/	SkillBegging,
			/*13892*/	SkillJewelryMaking,
			/*13901*/	SkillPottery,
			/*13898*/	SkillPercussionInstruments,
			/*13922*/	SkillIntimidation,
			/*13869*/	SkillBerserking,
			/*13902*/	SkillTaunt,
			/*00000*/	SkillCount					// move to last position of active enumeration labels
		};

	}
	namespace EqStructures
	{

		struct KeyboardModifiers
		{
			int Shift;
			int Ctrl;
			int Alt;
		};

		struct KeyboardInput
		{
			int forward;
			int back;
			int right;
			int left;
		};
		struct ZoneInfo
		{
			DWORD Unk;
			DWORD expansion;
			DWORD zone_id;
			char name_short[0x81];
			char name_long[0x80];
		};
		struct EQWorldData
		{
			BYTE Unknown[0x1C];
			ZoneInfo* Zones[225];
		};
		typedef struct _EQITEMCOMMONINFO
		{
			/* 0x00E4 */ INT8 Strength;       // STR
			/* 0x00E5 */ INT8 Stamina;        // STA
			/* 0x00E6 */ INT8 Charisma;       // CHA
			/* 0x00E7 */ INT8 Dexterity;      // DEX
			/* 0x00E8 */ INT8 Intelligence;   // INT
			/* 0x00E9 */ INT8 Agility;        // AGI
			/* 0x00EA */ INT8 Wisdom;         // WIS
			/* 0x00EB */ INT8 SaveMagic;      // SV MAGIC
			/* 0x00EC */ INT8 SaveFire;       // SV FIRE
			/* 0x00ED */ INT8 SaveCold;       // SV COLD
			/* 0x00EE */ INT8 SaveDisease;    // SV DISEASE
			/* 0x00EF */ INT8 SavePoison;     // SV POISON
			/* 0x00F0 */ INT16 Health;        // HP
			/* 0x00F2 */ INT16 Mana;          // Mana
			/* 0x00F4 */ INT16 ArmorClass;    // AC
			/* 0x00F6 */ BYTE Unknown0246[2];
			/* 0x00F8 */ BYTE Light;
			/* 0x00F9 */ BYTE AttackDelay;    // Atk Delay
			/* 0x00FA */ BYTE Damage;         // DMG
			/* 0x00FB */ BYTE IsStackableEx;  // Effect Type: 0=Combat 1=ClickyAny 2=Worn 3=Expendable 4=ClickyMustEquip 5=ClickyCanEquip
			/* 0x00FC */ BYTE Range;
			/* 0x00FD */ BYTE Skill;          // 21 = Expendable Potion
			/* 0x00FE */ BYTE Magic;
			/* 0x00FF */ BYTE CastingLevelEx;
			/* 0x0100 */ BYTE Material; // 0=None, 1=Leather, 2=Chain, 3=Plate, 4=Silk, etc
			/* 0x0101 */ BYTE Unknown0258[3];
			/* 0x0104 */ DWORD Color;
			/* 0x0108 */ BYTE Unknown0264[2];
			/* 0x010A */ WORD SpellIdEx;
			/* 0x010C */ WORD Classes; // bitwise flag
			/* 0x010E */ BYTE Unknown0270[2];
			/* 0x0110 */ WORD Races; // bitwise flag
			/* 0x0112 */ BYTE Unknown0274[2];
			/* 0x0114 */ BYTE IsStackable;
			/* 0x0115 */ BYTE CastingLevel; // also weapon proc level
			union
			{
				/* 0x0116 */ BYTE StackCount;
				/* 0x0116 */ BYTE Charges;
			};
			/* 0x0117 */ BYTE EffectType;
			/* 0x0118 */ WORD SpellId;
			/* 0x011A */ BYTE Unknown0123[10];
			/* 0x0124 */ union
			{
				struct {
					/* 0x0124 */ WORD SkillModId;
					/* 0x0126 */ INT8 SkillModPercent;
					/* 0x0127 */ BYTE Unknown0127;
				} SkillMod;
				/* 0x0124 */ int CastTime;
			};
			/* 0x0128 */ BYTE Unknown0128[28];
			/* 0x0144 */ BYTE RecLevel;  // Used in EQ_Character::AntiTwinkAdj.
			/* 0x0145 */ BYTE RecSkill;  // Client: Adjusted if Level < RecLevel, Skill < RecSkill.
			/* 0x0146 */ BYTE Unknown0146[22];
			/* 0x015C */ DWORD Deity;
			/* 0x0160 */ short RequiredLevel;
			/* 0x0162 */ WORD BardType;   // Bard Skill Type (instrument type)
			/* 0x0164 */ WORD BardValue;  // Bard Skill Amount (instrument modifier)
			/* 0x0166 */ BYTE Unknown0166[18];  // Total item struct size looks like 0x178.
		} EQITEMCOMMONINFO, * PEQITEMCOMMONINFO;
		typedef struct _EQITEMCONTAINERINFO  // EQ_Container class.
		{
			/* 0x00E4 */ struct _EQITEMINFO* Item[EQ_NUM_CONTAINER_SLOTS];
			/* 0x010C */ BYTE Combine;  // Type. 0-7 are bags, 9+ support combines.
			/* 0x010D */ BYTE Capacity; // num slots
			/* 0x010E */ BYTE IsOpen;
			/* 0x010F */ BYTE SizeCapacity;
			/* 0x0110 */ BYTE WeightReduction; // percent
			/* 0x0111 */ BYTE Unknown0111[3];  // Operator new of 0x114 bytes.
		} EQITEMCONTAINERINFO, * PEQITEMCONTAINERINFO;
		typedef struct _EQITEMBOOKINFO
		{
			/* 0x00E4 */ BYTE Unknown0228[3];
			/* 0x00E7 */ CHAR File[15];
			/* ...... */
		} EQITEMBOOKINFO, * PEQITEMBOOKINFO;
		typedef struct
		{
			/*0x000*/	CHAR Name[0x40];
			/*0x040*/	CHAR LoreName[0x50];
			/*0x090*/	CHAR IDFile[0x10];
			/*0x0a0*/	BYTE Unknown0a0[0xa];
			/*0x0aa*/	BYTE InstrumentType;//check/***
			/*0x0ab*/	BYTE Summoned;//check/***
			/*0x0ac*/	BYTE Lore;//check/***
			/*0x0ad*/	BYTE Magic;//check/***
			/*0x0ae*/	BYTE Weight;
			/*0x0af*/	BYTE NoRent;
			/*0x0b0*/	BYTE NoDrop;
			/*0x0b1*/	BYTE Size;
			/*0x0b2*/	BYTE Type;
			/*0x0b3*/	BYTE Unknown0x0b3;
			/*0x0b4*/	WORD ID;//ItemNumber
			/*0x0b6*/	WORD IconID;//IconNumber
			/*0x0b8*/	DWORD EquipSlot;
			/*0x0bc*/	DWORD EquipableSlots;
			/*0x0c0*/	DWORD Cost;
			/*0x0c4*/	DWORD Price;//*** needs confirm
			/*0x0c8*/	BYTE  Unknown0c4[0x1c];
			/*0x0E4*/  // memcpy size of 0xe4 in ItemDisplayWnd::SetItem().
		} EQITEMINFOBASE, * PEQITEMINFOBASE;
		typedef struct _EQITEMINFO : public EQITEMINFOBASE
		{
			union
			{
				/* 0x00E4 */ EQITEMCOMMONINFO    Common;    // EQITEMINFOBASE.Type == 0.
				/* 0x00E4 */ EQITEMCONTAINERINFO Container; // EQITEMINFOBASE.Type == 1.
				/* 0x00E4 */ EQITEMBOOKINFO      Book;      // EQITEMINFOBASE.Type == 2.
			};
			/* ...... */
		} EQITEMINFO, * PEQITEMINFO;

		struct ViewActor
		{
			/* 0x0000 */ DWORD Type;  // Checked if it is set to 0x18 in GetClickedActor()
			/* 0x0004 */ DWORD Unknown0004;
			/* 0x0008 */ DWORD Unknown0008;
			/* 0x000C */ DWORD Flags;  // Bit 0x40000000 = invisible
			/* 0x0010 */ Vec3 Position;
			/* 0x001C */ DWORD Unknown001C;
			/* 0x0020 */ DWORD Unknown0020;
			/* 0x0024 */ DWORD Unknown0024;
			/* 0x0028 */ DWORD RegionNumber;
			/* 0x002C */ struct _EQACTORCOLLISIONINFO* CollisionInfo;
			/* 0x0030 */ DWORD Unknown0030;
			/* 0x0034 */ FLOAT ScaleFactor;
			/* 0x0038 */ FLOAT BoundingRadius;
			/* 0x003C */ BYTE Unknown003C[36];
			/* 0x0060 */ struct Entity* Entity;
		};
		struct EQDAGINFO
		{
			/* 0x0000 */ DWORD Unknown0000;
			/* 0x0004 */ PCHAR Name; // x_DAG, x_POINT_DAG, etc
			/* 0x0008 */ struct _EQSTRINGSPRITE* StringSprite;
			/* 0x000C */ struct _EQDAGINFO* Parent;
			/* 0x0010 */ struct _EQMODELINFO* ModelInfo;
			/* 0x0014 */ struct _EQTRACKINFO* Track1;
			/* 0x0018 */ struct _EQTRACKINFO* Track2;
			/* 0x001C */ BYTE Unknown001C[0x3c];  // Possibly used in t3dGetCurrentDagFrameTransform.
			/* 0x0058 */ float ObjectToWorldTransform[9];  // Copied out in t3dGetCurrentDagObjectToWorldTransform.
			/* 0x007C */ Vec3 Position;
			/* 0x0088 */ BYTE Unknown0088[96];
			/* 0x00E8 */ DWORD NumChildren;
			/* 0x00EC */ struct _EQDAGCHILDREN* Children;
			/* ...... */
		};

		struct _EQMODELINFO
		{
			/* 0x0000 */ BYTE Unknown0000[64];
			/* 0x0040 */ DWORD NumSkinsAttachedToHierarchicalSprite;
			/* ...... */
		};
				typedef struct _EQARGBCOLOR {
			union {
				struct {
					BYTE B;
					BYTE G;
					BYTE R;
					BYTE A;
				};
				DWORD ARGB;
			};
			_EQARGBCOLOR(BYTE _R, BYTE _G, BYTE _B, BYTE _A) : A(_A), R(_R), G(_G), B(_B) {};
			_EQARGBCOLOR(DWORD _ARGB) : ARGB(_ARGB) {};
			_EQARGBCOLOR() : A{}, R{}, G{}, B{} {};

		} EQARGBCOLOR;
		typedef struct _EQSTRINGSPRITE
		{
			static constexpr int kMagicValidValue = 0x51;  // Magic value that means valid.
			/* 0x0000 */ DWORD MagicValue;  // Set to kMagicValidValue in valid StringSprites.
			/* 0x0004 */ DWORD Unknown0004;
			/* 0x0008 */ DWORD Unknown0008;
			/* 0x000C */ DWORD Unknown000C;
			/* 0x0010 */ DWORD Unknown0010;
			/* 0x0014 */ PVOID Unknown0014;      // Set as param_3 in s3dCreateStringSprite (ref counter?)
			/* 0x0018 */ PCHAR Text;
			/* 0x001C */ DWORD TextLength;
			/* 0x0020 */ DWORD Unknown0020;      // Set as param_5 in s3dCreateStringSprite
			/* 0x0024 */ DWORD MaxScaleFactor1;  // s3dSetStringSpriteMaxScaleFactor
			/* 0x0028 */ FLOAT MaxScaleFactor2;
			/* 0x002C */ FLOAT MaxScaleFactor3;
			/* 0x0030 */ DWORD IsYonClipEnabled;  // s3dSetStringSpriteYonClip
			/* 0x0034 */ DWORD YonClipDistance;
			/* 0x0038 */ FLOAT Unknown0038;      // Set to DAT_100db1a8 in s3dCreateStringSprite
			/* 0x003C */ DWORD Width;
			/* 0x0040 */ DWORD Height;
			/* 0x0044 */ FLOAT Unknown0044;  // Set to 0.5 * sqrt(width^2 + height^2) * Unknown0038
			/* 0x0048 */ EQARGBCOLOR Color;  // s3dSetStringSpriteTint
		} EQSTRINGSPRITE, * PEQSTRINGSPRITE;
		struct ActorInfo
		{
			//UINT ModifyAttackSpeed()
			//{
			//	return reinterpret_cast<UINT(__thiscall*)(ActorInfo*, UINT, INT)>(0x50A039)(this, );
			//}
			/* 0x0000 */ ViewActor* ViewActor_;
			/* 0x0004 */ struct _EQLIGHTINFO* LightInfo; // PointLight
			/* 0x0008 */ char ActorDef[64]; // xxx_ACTORDEF string (HUM_ACTORDEF, ELM_ACTORDEF, etc)
			/* 0x0048 */ FLOAT Z;
			/* 0x004C */ FLOAT ZCeiling; // Z axis of the ceiling or first collision above player
			/* 0x0050 */ PVOID Unknown0050; // EqMobileEmitter*
			/* 0x0054 */ DWORD Unknown0054;
			/* 0x0058 */ DWORD PhysicsTimer;
			/* 0x005C */ DWORD LastTick; // Last 1000ms Tick
			/* 0x0060 */ DWORD Last6000msTick;
			/* 0x0064 */ DWORD UnknownTimer4;
			/* 0x0068 */ DWORD AttackTimer;
			/* 0x006C */ DWORD DeathTimer;
			/* 0x0070 */ DWORD Unknown0070;
			/* 0x0074 */ DWORD UnknownTimer5;
			/* 0x0078 */ DWORD UnknownTimer6;
			/* 0x007C */ DWORD UnknownTimer7;
			/* 0x0080 */ WORD LevitationMovementCounter; // loops from 0 to 512 while levitating, causes up/down movement, 0xFFFF = Not Levitating
			/* 0x0082 */ WORD DrunkMovementCounter; // loops from 0 to 512 while drunk, causes left/right movement, 0xFFFF = Not Drunk
			/* 0x0084 */ WORD LevitationSize;
			/* 0x0086 */ WORD DrunkMovementSize;
			/* 0x0088 */ FLOAT DrunkMovementModifier; // how far left/right the player moves while drunk
			/* 0x008C */ FLOAT LevitationMovementModifier; // how far up/down the player moves while levitating
			/* 0x0090 */ BYTE IsAffectedByGravity; // gravity is enabled for the player (disabled while levitating)
			/* 0x0091 */ BYTE LastAttack; // equals 0, 11 or 13
			/* 0x0092 */ BYTE Unknown0092;
			/* 0x0093 */ BYTE Unknown0093;
			/* 0x0094 */ PVOID Unknown0094; // pointer, static address 0x006EC6E8 has same value
			/* 0x0098 */ PVOID Unknown0098; // pointer, static address 0x006EC6E8 has same value
			/* 0x009C */ PVOID Unknown009C; // pointer, static address 0x006EC6E8 has same value
			/* 0x00A0 */ BYTE IsSwimmingUnderwater;
			/* 0x00A1 */ BYTE SwimmingWaterType; // 5,6,9,10 = Water, 7 = Lava, 8 = Freezing Water (7 and 8 cause damage!)
			/* 0x00A2 */ BYTE SwimmingFeetTouchingWater; // 0 = False, 5 = True
			/* 0x00A3 */ BYTE SwimmingUnknown1; // 0 = False, 5 = True
			/* 0x00A4 */ BYTE SwimmingUnknown2;
			/* 0x00A5 */ BYTE SwimmingUnknown3;
			/* 0x00A6 */ BYTE SwimmingUnknown4;
			/* 0x00A7 */ BYTE SwimmingUnknown5;
			/* 0x00A8 */ FLOAT MovementFriction1; // used for sliding on ice or slippery surfaces (Default = 0.625)
			/* 0x00AC */ FLOAT MovementFriction2; // used for sliding on ice or slippery surfaces (Default = 0.8000000119)
			/* 0x00B0 */ FLOAT JumpHeightModifier; // how high the player jumps (Default = 1.350000024)
			/* 0x00B4 */ FLOAT Unknown00B4;
			/* 0x00B8 */ FLOAT Unknown00B8;
			/* 0x00BC */ FLOAT MovementSpeedModifier; // how much slower/faster you move
			/* 0x00C0 */ BYTE Unknown00C0[0xc];
			/* 0x00CC */ DWORD CastingTimeout;  // Set in CastSpell as current time + casting time.
			/* 0x00D0 */ BYTE Unknown00D0[0x8];
			/* 0x00D8 */ DWORD RecastTimeout[EQ_NUM_SPELL_GEMS]; // Set in the OP_MemorizeSpell handler.
			/* 0x00F8 */ BYTE Unknown00F8[0x68];
			/* 0x0160 */ DWORD FizzleTimeout;   // Set in the OP_MemorizeSpell handler.
			/* 0x0164 */ BYTE Unknown0164[0x20];
			/* 0x0184 */ DWORD Animation;
			/* 0x0188 */ BYTE Unknown0188[0xc];
			/* 0x0194 */ struct Entity* Entity0194;  // Entity is linked in UpdatePlayerVisibility.
			/* 0x0198 */ struct Entity* Entity0198;  // Appears to be a horse entity in UpdatePlayerVisiblity.
			/* 0x019C */ BYTE Unknown019c[24];
			/* 0x01B4 */ DWORD IsInvisible; // NPCs only? used by /hidecorpses command
			/* 0x01B8 */ BYTE Unknown01B8[10];
			/* 0x01C2 */ SHORT PetID;
			/* 0x01C4 */ BYTE Unknown01C4[156];
			/* 0x0260 */ DWORD IsHoldingBoth;
			/* 0x0264 */ DWORD IsHoldingSecondary;
			/* 0x0268 */ DWORD IsHoldingPrimary;
			/* 0x026C */ BYTE Unknown026C[4];
			/* 0x0270 */ BYTE Unknown0270[4];
			/* 0x0274 */ BYTE IsInvited;  // Set to True in EQPlayer::SetInvited()
			/* 0x0275 */ BYTE Unknown0275[3];
			/* 0x0278 */ BYTE Unknown0278[4];
			/* 0x027C */ WORD CastingSpellId;  // Set in StartCasting. Set to -1 (kInvalidSpell) in stop.
			/* 0x027E */ BYTE CastingSpellGemNumber; // StartCasting:Gem #, OP_MemorizeSpell handler: 0xff. Stop: 0x0.
			/* 0x027F */ BYTE Unknown027F;
			/* 0x0280 */ BYTE Unknown0280[4];
			/* 0x0284 */ struct _EQMODELINFO* ModelInfo;
			/* 0x0288 */ EQDAGINFO* DagHeadPoint;
			/* 0x028C */ EQDAGINFO* DagHead;
			/* 0x0290 */ EQDAGINFO* DagUnknown;
			/* 0x0294 */ EQDAGINFO* DagRightPoint;
			/* 0x0298 */ EQDAGINFO* DagLeftPoint;
			/* 0x029C */ EQDAGINFO* DagShieldPoint;
			/* 0x02A0 */ BYTE Unknown02A0[128];
			/* 0x0320 */ BYTE MovementType; // 0 = None, 4 = Walking, 6 = Running, 7 = Swimming
			/* 0x0321 */ BYTE Unknown0321[12];
			/* 0x032D */ BYTE IsMovingTimer; // 0 = Moving, 1-6 = Recently Stopped Moving, 200 = Not Moving
			/* 0x032E */ BYTE Unknown032E[0xa];
			/* 0x0338 */ BYTE AAAbilities[0xe3];  // Current AA value (used by EQ_Character::GetAbility().
			/* 0x041B */ BYTE Unknown041B[0x9];
			/* 0x0424 */ DWORD CastingSpellCastTime;  // Set in StartCasting. Used in GetSpellCastingTime().
			/* 0x0428 */ BYTE Unknown0428[0x10];
			/* 0x0438 */ DWORD IsLookingForGroup;
			/* 0x043C */ DWORD IsTrader;
			/* ...... */
		};
		typedef struct _EQINVENTORY
		{
			/* 00 */ struct _EQITEMINFO* EarLeft;
			/* 01 */ struct _EQITEMINFO* Head;
			/* 02 */ struct _EQITEMINFO* Face;
			/* 03 */ struct _EQITEMINFO* EarRight;
			/* 04 */ struct _EQITEMINFO* Neck;
			/* 05 */ struct _EQITEMINFO* Shoulders;
			/* 06 */ struct _EQITEMINFO* Arms;
			/* 07 */ struct _EQITEMINFO* Back;
			/* 08 */ struct _EQITEMINFO* WristLeft;
			/* 09 */ struct _EQITEMINFO* WristRight;
			/* 10 */ struct _EQITEMINFO* Ranged;
			/* 11 */ struct _EQITEMINFO* Hands;
			/* 12 */ struct _EQITEMINFO* Primary;
			/* 13 */ struct _EQITEMINFO* Secondary;
			/* 14 */ struct _EQITEMINFO* RingLeft;
			/* 15 */ struct _EQITEMINFO* RingRight;
			/* 16 */ struct _EQITEMINFO* Chest;
			/* 17 */ struct _EQITEMINFO* Legs;
			/* 18 */ struct _EQITEMINFO* Feet;
			/* 19 */ struct _EQITEMINFO* Waist;
			/* 20 */ struct _EQITEMINFO* Ammo;
		} EQINVENTORY, * PEQINVENTORY;
		struct _EQBUFFINFO
		{
			/* 0x0000 */ BYTE BuffType; // Active buffs have a non-zero value
			/* 0x0001 */ BYTE CasterLevel; // level of player who casted the buff
			/* 0x0002 */ BYTE Modifier; // divide by 10 to get Bard song modifier
			/* 0x0003 */ BYTE Activated; // Copied from spell data to buff struct.  Only a few spells have this set to 1, the rest are 0
			/* 0x0004 */ WORD SpellId;
			/* 0x0006 */ WORD Ticks; //  duration in ticks ; seconds = ticks * 6
			/* 0x0008 */ WORD Counters; // rune amount, poison/disease/curse counters
			/* 0x000A */
		};
		struct EQZONEINFO
		{   // OP_NewZone 0x4058 performs a memcpy of the first 0x23c bytes of NewZone_struct packet data to 0x00798784.
			/* 0x0000 */ CHAR PlayerName[0x40];
			/* 0x0040 */ CHAR ShortName[0x20];
			/* 0x0060 */ CHAR LongName[0x80];
			/* 0x00E0 */ BYTE Unknown00E0[150];
			/* 0x0176 */ BYTE Type;
			/* 0x0177 */ BYTE FogColorRed[4];    // Set to 0x0c if 0184 and 0194 set to 0.
			/* 0x017B */ BYTE FogColorGreen[4];  // Set to 0x0c if 0184 and 0194 set to 0.
			/* 0x017F */ BYTE FogColorBlue[4];   // Set to 0x0c if 0184 and 0194 set to 0.
			/* 0x0183 */ BYTE Unknown0183;
			/* 0x0184 */ FLOAT Unknown0184;  // Set to 500.0 if 0184 and 0194 set to 0.
			/* 0x0188 */ FLOAT Unknown0188;
			/* 0x018C */ FLOAT Unknown018C;
			/* 0x0190 */ FLOAT Unknown0190;
			/* 0x0194 */ FLOAT Unknown0194;  // Set to 600.0 if 0184 and 0194 set to 0.
			/* 0x0198 */ FLOAT Unknown0198;
			/* 0x019C */ FLOAT Unknown019C;
			/* 0x01A0 */ FLOAT Unknown01A0;
			/* 0x01A4 */ FLOAT Gravity;
			/* 0x01A8 */ BYTE TimeType;  // 0 = indoor dungeon, 1 = outdoor, 2 = outdoor city, 3 = indoor city, 4 = nosky outdoor city, 5 = outdoor dungeon
			/* 0x01A9 */ BYTE Unknown01A9;
			/* 0x01AA */ BYTE Unknown01AA;
			/* 0x01AB */ BYTE Unknown01AB;
			/* 0x01AC */ BYTE Unknown01AC;
			/* 0x01AD */ BYTE Unknown01AD[45];
			/* 0x01DA */ BYTE SkyType;
			/* 0x01DB */ BYTE Unknown01DB[9];
			/* 0x01E4 */ FLOAT ExperienceMultiplier;
			/* 0x01E8 */ FLOAT SafeCoordsY; // CDisplay::MoveLocalPlayerToSafeCoords
			/* 0x01EC */ FLOAT SafeCoordsX;
			/* 0x01F0 */ FLOAT SafeCoordsZ;
			/* 0x01F4 */ FLOAT Unknown01F4;
			/* 0x01F8 */ FLOAT Unknown01F8;
			/* 0x01FC */ FLOAT MinClip; // draw distance (minimum Far Clip Plane), set to max(50, value).
			/* 0x0200 */ FLOAT MaxClip; // draw distance (maximum Far Clip Plane), set to 4*(0x0194) if < 51 or 4*0x01fc
			/* ...... */
		};
		struct EQCHARINFO  // EqMacEmu/common/patches/mac_structs.h::PlayerProfile_Struct
		{
			float encum_factor()
			{
				return reinterpret_cast<float(__thiscall*)(EQCHARINFO*)>(0x4bb9c7)(this);
			}
			short max_mana()
			{
				return reinterpret_cast<short(__thiscall*)(EQCHARINFO*)>(0x4B9483)(this);
			} 
			short mana()
			{
				return reinterpret_cast<short(__thiscall*)(EQCHARINFO*)>(0x4b9450)(this);
			}
			short str()
			{
				return reinterpret_cast<short(__thiscall*)(EQCHARINFO*)>(0x004bafcb)(this);
			}
			short dex()
			{
				return reinterpret_cast<short(__thiscall*)(EQCHARINFO*)>(0x004bb34e)(this);
			}
			short compute_to_hit(char skill_type)
			{
				return reinterpret_cast<short(__thiscall*)(EQCHARINFO*, char)>(0x004b9b96)(this, skill_type);
			}
			short offense(char skill_type)  // skill_type == -1 defaults to hand-to-hand.
			{
				return reinterpret_cast<short(__thiscall*)(EQCHARINFO*, char)>(0x004b9dda)(this, skill_type);
			}
			short i_have_skill(int skill_type)
			{
				return reinterpret_cast<short(__thiscall*)(EQCHARINFO*, char)>(0x004bfff5)(this, skill_type);
			}
			int skill(int skill_type)
			{
				return reinterpret_cast<short(__thiscall*)(EQCHARINFO*, char)>(0x004b8efd)(this, skill_type);
			}
			short ac(bool apply_cap)
			{
				return reinterpret_cast<short(__thiscall*)(EQCHARINFO*, bool)>(0x004b9b8a)(this, apply_cap);
			}
			int compute_defense()
			{
				return reinterpret_cast<int(__thiscall*)(EQCHARINFO*)>(0x004b9d19)(this);
			}
			short total_spell_affects(int category, bool unknown1, short* unknown2)
			{
				return reinterpret_cast<short(__thiscall*)(EQCHARINFO*, int, bool, short*)>(0x004c6b6d)(this, category, unknown1, unknown2);
			}
			short total_item_spell_affects(int category)
			{
				return reinterpret_cast<short(__thiscall*)(EQCHARINFO*, int)>(0x004c1235)(this, category);
			}
			bool is_spell_affecting_pc(short spell_id)
			{
				return reinterpret_cast<short(__thiscall*)(EQCHARINFO*, short)>(0x004c70e7)(this, spell_id);
			}
			int cast(UINT gem, WORD spell_id, int* item, short item_slot)
			{
				return reinterpret_cast<int(__thiscall*)(EQCHARINFO*, UINT, WORD, int*, short)>(0x4c483b)(this, gem, spell_id, item, item_slot);
			}
			void stop_cast(UINT reason, WORD spell_id)
			{
				return reinterpret_cast<void(__thiscall*)(EQCHARINFO*, UINT, WORD)>(0x4cb510)(this, reason, spell_id);
			}
			bool can_i_see_invis() // CanISeeInvis()
			{
				return reinterpret_cast<bool(__thiscall*)(EQCHARINFO*)>(0x004c0d02)(this);
			}
			_EQBUFFINFO* GetBuff(WORD buff_slot) { // Supports 0..29
				return reinterpret_cast<_EQBUFFINFO*(__thiscall*)(EQCHARINFO*, WORD)>(0x004C465A)(this, buff_slot);
			}
			int GetMaxBuffs() {
				return reinterpret_cast<BYTE(__thiscall*)(EQCHARINFO*)>(0x004C4637)(this);
			}
			/* 0x0000 */ BYTE Unknown0000[2];
			/* 0x0002 */ CHAR Name[64]; // [0x40]
			/* 0x0042 */ CHAR LastName[70]; // [0x46] ; surname or title
			/* 0x0088 */ WORD Gender; // EQ_GENDER_x
			/* 0x008A */ WORD Race; // EQ_RACE_x
			/* 0x008C */ BYTE Class; // EQ_CLASS_x
			/* 0x008D */ BYTE Unknown008D;
			/* 0x008E */ WORD Unknown008E;
			/* 0x0090 */ WORD Level;
			/* 0x0092 */ WORD Unknown0092;
			/* 0x0094 */ DWORD Experience; // EXP
			/* 0x0098 */ WORD PracticePoints; // Training window
			/* 0x009A */ WORD Mana;
			/* 0x009C */ WORD BaseHP;
			/* 0x009E */ WORD StunnedState; // EQ_STUNNED_STATE_x
			/* 0x00A0 */ WORD BaseSTR;
			/* 0x00A2 */ WORD BaseSTA;
			/* 0x00A4 */ WORD BaseCHA;
			/* 0x00A6 */ WORD BaseDEX;
			/* 0x00A8 */ WORD BaseINT;
			/* 0x00AA */ WORD BaseAGI;
			/* 0x00AC */ WORD BaseWIS;
			/* 0x00AE */ BYTE Unknown00AE[438];
			/* 0x0264 */ _EQBUFFINFO Buff[EQ_NUM_BUFFS];
			/* 0x02FA */ BYTE Unknown02FA[1080];
			/* 0x0732 */ WORD SpellBook[EQ_NUM_SPELL_BOOK_SPELLS];
			/* 0x0B32 */ SHORT MemorizedSpell[EQ_NUM_SPELL_GEMS]; // spell gem spell ids
			/* 0x0B42 */ BYTE Unknown0B42[14];
			/* 0x0B50 */ WORD Unknown0B50;
			/* 0x0B52 */ WORD Unknown0B52;
			/* 0x0B54 */ Vec3 ZoneEnter;
			/* 0x0B60 */ FLOAT Unknown0060;
			/* 0x0B64 */ BYTE StandingState; // EQ_STANDING_STATE_x
			/* 0x0B65 */ BYTE Unknown0B65[3];
			/* 0x0B68 */ DWORD Platinum;
			/* 0x0B6C */ DWORD Gold;
			/* 0x0B70 */ DWORD Silver;
			/* 0x0B74 */ DWORD Copper;
			/* 0x0B78 */ DWORD BankPlatinum;
			/* 0x0B7C */ DWORD BankGold;
			/* 0x0B80 */ DWORD BankSilver;
			/* 0x0B84 */ DWORD BankCopper;
			/* 0x0B88 */ DWORD CursorPlatinum; // currency held on the mouse cursor
			/* 0x0B8C */ DWORD CursorGold;
			/* 0x0B90 */ DWORD CursorSilver;
			/* 0x0B94 */ DWORD CursorCopper;
			/* 0x0B98 */ BYTE Unknown0B98[16];
			/* 0x0BA8 */ WORD Skills[EQ_NUM_SKILLS];
			/* 0x0C3C */ BYTE Unknown0C3C[64];
			/* 0x0C7C */ WORD Vision1;
			/* 0x0C7E */ BYTE Unknown0C7E[12];
			/* 0x0C8A */ WORD Vision2;
			/* 0x0C8C */ BYTE Unknown0C8C[120];
			/* 0x0D04 */ DWORD IsSwimmingUnderwater;
			/* 0x0D08 */ BYTE Unknown0D08[4];
			/* 0x0D0C */ BYTE Unknown0D0C[4];
			/* 0x0D10 */ BYTE IsAutoSplitEnabled;
			/* 0x0D11 */ BYTE Unknown0D11[43];
			/* 0x0D3C */ DWORD Expansions;  // Bit field: 1 = Kun, 2 = Vel, 4 = Luc, 8 = PoP.
			/* 0x0D40 */ BYTE Unknown0D40[20];
			/* 0x0D54 */ DWORD Hunger;
			/* 0x0D58 */ DWORD Thirst;
			/* 0x0D5C */ _EQBUFFINFO BuffsExtIndexMinus15[2]; // The client uses this field to read from 'BuffsExt[n-15]' by passing BuffsExtIndexMinus15[n] to this array, which is the same struct offset. Usage not recommended.
			/* 0x0D70 */ DWORD ZoneId;
			/* 0x0D74 */ Entity* SpawnInfo;
			/* 0x0D78 */  _EQITEMINFO* CursorItem;
			union
			{
				/* 0x0D7C */ struct _EQINVENTORY Inventory;
				/* 0x0D7C */ struct _EQITEMINFO* InventoryItem[EQ_NUM_INVENTORY_SLOTS];
			};
			/* 0x0DD0 */ struct _EQITEMINFO* InventoryPackItem[EQ_NUM_INVENTORY_PACK_SLOTS];
			/* 0x0DF0 */ BYTE Unknown0DF0[2];
			/* 0x0DF2 */ _EQBUFFINFO BuffsExt[15]; // non-PCs have access to 15 more buff slots. We can maybe use this space for the short-buff window in the future
			/* 0x0E88 */ WORD BuffCasterId[30]; // The entity ID who put buffs 1-30 on this unit
			/* 0x0EC4 */ DWORD ZoneBoundId;
			/* 0x0EC8 */ DWORD Unknown0EC8;
			/* 0x0ECC */ DWORD Unknown0ECC;
			/* 0x0ED0 */ DWORD Unknown0ED0;
			/* 0x0ED4 */ DWORD ZoneBirthId;
			/* 0x0ED8 */ FLOAT ZoneBoundY;
			/* 0x0EDC */ DWORD Unknown0EDC;
			/* 0x0EE0 */ DWORD Unknown0EE0;
			/* 0x0EE4 */ DWORD Unknown0EE4;
			/* 0x0EE8 */ FLOAT ZoneBirthY;
			/* 0x0EEC */ FLOAT ZoneBoundX;
			/* 0x0EF0 */ DWORD Unknown0EF0;
			/* 0x0EF4 */ DWORD Unknown0EF4;
			/* 0x0EF8 */ DWORD Unknown0EF8;
			/* 0x0EFC */ FLOAT ZoneBirthX;
			/* 0x0F00 */ FLOAT ZoneBoundZ;
			/* 0x0F04 */ DWORD Unknown0F04;
			/* 0x0F08 */ DWORD Unknown0F08;
			/* 0x0F0C */ DWORD Unknown0F0C;
			/* 0x0F10 */ FLOAT ZoneBirthZ;
			/* 0x0F14 */ BYTE Unknown0F14[1080];
			/* 0x134C */ WORD Deity;
			/* 0x134E */ short GuildId;
			/* 0x1350 */ BYTE Unknown1350[8];
			/* 0x1358 */ BYTE Unknown1358;
			/* 0x1359 */ BYTE Unknown1359;
			/* 0x135A */ BYTE Unknown135A;
			/* 0x135B */ BYTE Unknown135B;
			/* 0x135C */ BYTE Unknown135C;
			/* 0x135D */ BYTE Unknown135D;
			/* 0x135E */ BYTE Stamina; // yellow endurance bar ; 100 = Empty, 0 = Full
			/* 0x135F */ BYTE Unknown135F;
			/* 0x1360 */ BYTE Unknown1360;
			/* 0x1361 */ BYTE AnonymousState;
			/* 0x1362 */ BYTE Unknown1362;
			/* 0x1363 */ BYTE GuildStatus; // guild rank
			/* 0x1364 */ BYTE Drunkness; // 0 = Not Drunk, counts down over time
			/* 0x1365 */ BYTE Unknown1365[451];
			/* 0x1528 */ DWORD AlternateAdvancementExperience;
			/* 0x152C */ BYTE Unknown152C[476];
			/* 0x1708 */ BYTE AirSupply; // air remaining while swimming underwater
			/* 0x1709 */ BYTE Unknown1709[2475];

			// Note: The unmodified client EQCHARINFO has 8 bank slots with the rest of the structure
			//       effectively unused to the original size of 0x2104. If the Quarm eqgame.dll patch
			//       is active, the structure is modified to the layout below. The code must check
			//       for the presence of the patch before accessing beyond EQ_NUM_INVENTORY_BANK_SLOTS.
			//       Currently this is the get_num_total_bank_slots() which returns 60 if patched.
			// Original structure:
			// 0x20B4 */ struct _EQITEMINFO* InventoryBankItem[EQ_NUM_INVENTORY_BANK_SLOTS];
			// 0x20D4 */ DWORD UnusedSpace[12]; (Index '4' is set to 1 but unused, patched out).
			// };  /* 0x2104 (total size) */
			// Patched layout:
			/* 0x20B4 */ struct _EQITEMINFO* InventoryBankItem[30];
			/* 0x212C */ struct _EQITEMINFO* SharedBankItem[30];
		};  /* 0x21A4 (total size) */

		struct Everquest
		{
			BYTE IsOkToTransact()
			{
				return reinterpret_cast<BYTE(__thiscall*)(Everquest*)>(0x54825C)(this);
			}
			/*0x000*/ BYTE Unknown0000[0x288];
			/*0x288*/ void* ChannelServerApi; // Pointer to ChannelServerApi object.
			/*0x28C*/ BYTE Unknown028c[0x320];
			/*0x5AC*/ int game_state;
		};

		struct Entity
		{
			void ChangeStance(BYTE new_stance)
			{
				if (this && this->StandingState != new_stance)
					reinterpret_cast<void(__thiscall*)(Entity*, unsigned char)>(0x50be3c)(this, new_stance);
			}
			UINT ModifyAttackSpeed(UINT raw_delay, UINT is_bow)
			{
				return reinterpret_cast<UINT(__thiscall*)(void* eq_player, UINT raw_delay, UINT is_bow)>(0x0050a039)(this , raw_delay, is_bow);
			}
			/* 0x0000 */ BYTE StructType; // If != 0x03, this struct is invalid (e.g. bad ActorInfo pointer).
			/* 0x0001 */ CHAR Name[30]; // [0x1E]
			/* 0x001F */ BYTE Unknown001F[37];
			/* 0x0044 */ DWORD ZoneId; // EQ_ZONE_ID_x
			/* 0x0048 */ Vec3 Position;
			/* 0x0054 */ FLOAT Heading; // camera view left/right, yaw
			/* 0x0058 */ FLOAT Unk;
			/* 0x005C */ FLOAT MovementSpeed;
			/* 0x0060 */ FLOAT MovementSpeedY;
			/* 0x0064 */ FLOAT MovementSpeedX;
			/* 0x0068 */ FLOAT MovementSpeedZ;
			/* 0x006C */ FLOAT MovementSpeedHeading;
			/* 0x0070 */ FLOAT Unknown0070;
			/* 0x0074 */ FLOAT Pitch; // camera view up/down
			/* 0x0078 */ Entity* Prev;
			/* 0x007C */ Entity* Next;
			/* 0x0080 */ PVOID Unknown0080;
			/* 0x0084 */ ActorInfo* ActorInfo;
			/* 0x0088 */ EQCHARINFO* CharInfo;
			/* 0x008C */ FLOAT CameraHeightOffset;
			/* 0x0090 */ FLOAT ModelHeightOffset;
			/* 0x0094 */ WORD SpawnId;
			/* 0x0096 */ WORD PetOwnerSpawnId; // spawn id of the owner of this pet spawn
			/* 0x0098 */ INT HpMax;
			/* 0x009C */ INT HpCurrent;  // Can go negative (unconscious).
			/* 0x00A0 */ short GuildId;  // -1 is unguilded
			/* 0x00A2 */ BYTE Unknown00A2[6];
			/* 0x00A8 */ BYTE Type; // EQ_SPAWN_TYPE_x
			/* 0x00A9 */ BYTE Class; // EQ_CLASS_x
			/* 0x00AA */ WORD Race;  // EQ_RACE_x
			/* 0x00AC */ BYTE Gender; // EQ_GENDER_x
			/* 0x00AD */ BYTE Level;
			/* 0x00AE */ BYTE IsHidden; // 0 = Visible, 1 = Invisible
			/* 0x00AF */ BYTE IsSneaking; // sneaking or snared ; 0 = Normal Movement Speed, 1 = Slow Movement Speed
			/* 0x00B0 */ BYTE IsPlayerKill; // PVP flagged with red name by Priest of Discord
			/* 0x00B1 */ BYTE StandingState; // EQ_STANDING_STATE_x
			/* 0x00B2 */ BYTE LightType; // EQ_LIGHT_TYPE_x
			/* 0x00B3 */ BYTE Face;
			/* 0x00B4 */ WORD EquipmentMaterialType[7]; // EQ_EQUIPMENT_MATERIAL_TYPE_x ; Head,Chest,Arms,Wrist,Hands,Legs,Feet
			/* 0x00C2 */ WORD EquipmentPrimaryItemType; // EQ_EQUIPMENT_ITEM_TYPE_x ; Primary
			/* 0x00C4 */ WORD EquipmentSecondaryItemType; // EQ_EQUIPMENT_ITEM_TYPE_x ; Secondary
			/* 0x00C6 */ WORD Unknown00C6;
			/* 0x00C8 */ DWORD EquipmentMaterialColor[9];
			/* 0x00EC */ BYTE Texture; // Body texture. For players, 0xFF means show-equipment textures.
			/* 0x00F0 */ FLOAT Height; // model height or size/scale (shrink, grow, etc)
			/* 0x00F4 */ FLOAT Unknown00F4;
			/* 0x00F8 */ FLOAT Unknown00F8;
			/* 0x00FC */ FLOAT Unknown00FC;
			/* 0x0100 */ FLOAT MovementBackwardSpeedMultiplier; // modifies how fast you move backwards (Default = 0.4600000083)
			/* 0x0104 */ FLOAT MovementForwardSpeedMultiplier; // modifies how fast you move forwards (Default = 0.6999999881)
			/* 0x0108 */ BYTE IsLinkDead; // LD
			/* 0x0109 */ BYTE IsGameMaster; // GM
			/* 0x010A */ WORD LevitationState; // EQ_LEVITATION_STATE_x
			/* 0x010C */ DWORD TargetType; // EQ_SPAWN_TARGET_TYPE_x
			/* 0x0110 */ DWORD Unknown0110;
			/* 0x0114 */ DWORD AnonymousState; // EQ_ANONYMOUS_STATE_x, /anonymous and /roleplay
			/* 0x0118 */ DWORD Unknown0118;
			/* 0x011C */ DWORD IsAwayFromKeyboard; // AFK
			/* 0x0120 */ BYTE Unknown0120[4];
			/* 0x0124 */ DWORD AlternateAdvancementRank; // AA points title value (0-3) (Venerable, Baroness, etc)
			/* 0x0128 */ BYTE Unknown0128[4];
			/* 0x012C */ CHAR LastName[22]; // surname or title
			/* 0x0142 */ BYTE Unknown0142[10];
			/* 0x014C */ WORD GuildStatus; // guild rank
			/* 0x014E */ WORD Deity; // EQ_DEITY_x
			/* 0x0150 */ BYTE Unknown0150;
			/* 0x0151 */ BYTE HairColor;
			/* 0x0152 */ BYTE BeardColor;
			/* 0x0153 */ BYTE Unknown0153[2];
			/* 0x0155 */ BYTE HairStyle;
			/* 0x0156 */ BYTE BeardStyle;
			/* 0x0157 */ BYTE Unknown0157[5];
			/* 0x015C */ DWORD Unknown015C;
			/* 0x0160 */ DWORD Unknown0160;
			/* 0x0164 */ DWORD Unknown0164;
			/* 0x0168 */
		};
		struct GuildInfo
		{
			/* 0x0000 */ CHAR Name[32];
			/* 0x0032 */ BYTE Unknown0032[64];
		};
		struct GuildName
		{			
			union
			{
				char name[512][0x60];
				struct {
					struct GuildInfo Guild[512];
				};
			};
		};
		struct GroupInfo
		{
			// Unlike RaidInfo this is not treated as a monolithic structure in the code
			// but this is the memory layout for the variables below.
			/* 0x0000 */ BYTE IsValidList[EQ_NUM_GROUP_MEMBERS];  // 1 = Valid group member, 0 = empty.
		    /* 0x0005 */ CHAR Names[EQ_NUM_GROUP_MEMBERS][64];
			/* 0x0145 */ CHAR Unknown0143[3];  // Alignment padding probably.
			/* 0x0148 */ struct Entity* EntityList[EQ_NUM_GROUP_MEMBERS];
			/* 0x015C */ CHAR LeaderName[64];  // Empty string is ungrouped.

			bool is_in_group() const { return (LeaderName[0] != 0); }
		};
		struct RaidMember // starts at 0x791518, size 0x00D0
		{			
			static constexpr int kRaidUngrouped = 0xffffffff;
			/* 0x0000 */ CHAR Name[64];
			/* 0x0040 */ CHAR PlayerLevel[8];  // Level in text.
			/* 0x0048 */ CHAR Class[64];       // Class text label.
			/* 0x0088 */ BYTE Unknown0088[64];
			/* 0x00C8 */ USHORT ClassValue;    // Binary enum value of Class
			/* 0x00CA */ BYTE Unknown00CA;
			/* 0x00CB */ BYTE IsGroupLeader; // 0: not leader 1: leader
			/* 0x00CC */ DWORD GroupNumber; // FFFFFFFF if ungrouped
		};
		struct RaidInfo {
			static constexpr int kRaidMaxMembers = 72;
			static constexpr int kRaidMaxLooters = 9;  // Calculated from memset of 0x240 bytes.
			/* 0x0000 */ CHAR Unknown0000[72];  // Set to 0 at reset.
			/* 0x0048 */ RaidMember MemberList[kRaidMaxMembers];
			/* 0x3ac8 */ DWORD Id;
			/* 0x3acc */ DWORD MemberCount;    // # of raid members
			/* 0x3ad0 */ CHAR LeaderName[64];  // Name of raid leader
			/* 0x3b10 */ CHAR Unknown3B10[64];
			/* 0x3b50 */ DWORD Unknown3B50;    // Set to 1 when empty, 4 when programmed?
			/* 0x3b54 */ CHAR Unknown3B54;
			/* 0x3b55 */ CHAR IsLeader;        // 1 = Leader of raid
			/* 0x3b56 */ CHAR Unknown3B56[2];
			/* 0x3b58 */ DWORD Unknown3B58;    // Set to 0xffffffff at reset.
			/* 0x3b5c */ DWORD LootType;       // Raid loot 
			/* 0x3b60 */ CHAR LooterNames[kRaidMaxLooters][64];  // See 0x240 cleared to zero.

			bool is_in_raid() const { return (MemberCount > 0); }
		};
		struct MouseDelta
		{
			INT16 x;
			INT16 y;
		};
		struct CameraSetting
		{
			/* 0x0000 */ Vec3 Position;
			/* 0x00C */ FLOAT Heading; // Yaw
			/* 0x010 */ FLOAT Pitch;
			/* 0x01C */ FLOAT WhoKnows;
		};
		struct CameraInfo
		{
			/* 0x0000 */ DWORD Unknown0000;
			/* 0x0004 */ DWORD RegionNumber;
			/* 0x0008 */ Vec3 Position;
			/* 0x0014 */ FLOAT Heading; // Yaw
			/* 0x0018 */ FLOAT Pitch;
			/* 0x001C */ FLOAT Roll;
			/* 0x0020 */ FLOAT FieldOfView;
			/* 0x0024 */ FLOAT AspectRatio;
			/* 0x0028 */ FLOAT Unknown0028;
			/* 0x002C */ FLOAT Unknown002C;
			/* 0x0030 */ FLOAT DrawDistance;
			/* 0x0034 */ BYTE Unknown0034[20];
			/* 0x0048 */ DWORD ResolutionWidth;
			/* 0x004C */ DWORD ResolutionHeight;
			/* 0x0050 */ BYTE Unknown0050[12];
			/* 0x005C */ FLOAT Unknown005C;
			/* 0x0060 */ FLOAT Unknown0060;
			/* 0x0064 */ FLOAT ResolutionWidthHalf;
			/* 0x0068 */ FLOAT ResolutionHeightHalf;
			/* 0x006C */ FLOAT Yaw;
			/* 0x0070 */ FLOAT Unk;
			/* 0x0074 */ FLOAT sPitch;
			/* ...... */
		};

		struct Cam
		{
			/* 0x0000 */float zoom;
			/* 0x0004 */float heading;
			/* 0x0008 */float height;
			/* 0x000c */float unk3;
			/* 0x0010 */float pitch;
			/* 0x0014 */float unk5;
			/* 0x0018 */float unk6;
		};
		struct ActorLocation
		{
			Vec3 Position;
			float heading;
			float pitch;
			float unk3;
			float unk4;
			float unk5;
			float unk6;
			float unk7;
		};
		struct SPELL {
			/*0x000*/   DWORD   ID;
			/*0x004*/   FLOAT   Range;
			/*0x008*/   FLOAT   AERange;
			/*0x00c*/	FLOAT	PushBack;
			/*0x010*/   BYTE    Unknown0x00c[0x04];
			/*0x014*/   DWORD   CastTime;
			/*0x018*/   DWORD   FizzleTime;
			/*0x01c*/   DWORD   RecastTime;
			/*0x020*/   DWORD   DurationType;       //DurationFormula on Lucy 
			/*0x024*/   DWORD   DurationValue1;
			/*0x028*/   BYTE   Unknown0x028[0x4];
			/*0x02c**/  WORD	Mana;

			/*0x02e*/   short    Base[0x0c];  //has to be signed for  ShowSpellSlotInfo to work      //Base1-Base12 
			/*0x046**/   short    Max[0x0c];  //has to be signed for  ShowSpellSlotInfo to work        //Max1-Max12 

			/*0x05e**/  WORD   BookIcon;
			/*0x060**/  WORD   GemIcon;

			/*0x062**/   WORD   ReagentId[0x4];     //ReagentId1-ReagentId4 
			/*0x06a**/   WORD   ReagentCount[0x4];  //ReagentCount1-ReagentCount4 
			/*0x072*/   BYTE	Unknown0x072[0x8];
			/*0x07a**/   BYTE	Calc[0x0c];         //Calc1-Calc12 
			/*0x086*/   BYTE	LightType;
			/*0x087*/   BYTE	SpellType;          //0=detrimental, 1=Beneficial, 2=Beneficial, Group Only
			/*0x088*/   BYTE	Unknown0x088;
			/*0x089**/	BYTE	Resist; //0=un 1=mr 2=fr 3=cr 4=pr 5=dr 6=chromatic
			/*0x08a**/	BYTE   Attrib[0xc];
			/*0x096**/   BYTE    TargetType;         // 03=Group v1, 04=PB AE, 05=Single, 06=Self, 08=Targeted AE, 0e=Pet, 28=AE PC v2, 29=Group v2
			/*0x097**/   BYTE	FizzleAdj;
			/*0x098**/   BYTE	Skill;
			/*0x099*/   BYTE    Location;            // 01=Outdoors, 02=dungeons, ff=Any 
			/*0x09a*/   BYTE	Environment;
			/*0x09b*/   BYTE	TimeOfDay;		     // 0=any, 1=day only, 2=night only
			/*0x09c*/	BYTE	ClassLevel[0x10];    // per class using EQCHARINFO.Class (EQ_CLASS_X) as offset.
			/*0x0a7*/   BYTE    Unknown0x14f[0x10];
			/*0x0bc**/   BYTE    CastingAnim;
			/*0x0bd*/	BYTE	Unknown0x0bd[0x13];
			/*0x0d0**/   CHAR* Name;
			/*0x0d4**/   CHAR* Target;
			/*0x0d8**/   CHAR* Extra;			// This is 'Extra' from Lucy (portal shortnames etc)
			/*0x0dc**/   CHAR* Unknown0x0dc;
			/*0x0e0**/   CHAR* Unknown0x0e0;
			/*0x0e4**/   CHAR* CastOnYou;
			/*0x0e8**/   CHAR* CastOnAnother;
			/*0x0ec**/   CHAR* WearOff;
			/*0x0f0*/   BYTE	Unknown0x0f0[0x18];
			/*0x108**/   DWORD	Icon; //Icon index in spells tgas
			/*0x10c**/   BYTE	ResistAdj;
			/*0x10d*/   BYTE	Unknown0x10d[0xb];
			/*0x114*/
		};
		struct SPELLMGR {
			 SPELL* Spells[EQ_NUM_SPELLS];
		};
		struct EQAltAbility {  // operator new of 0x38 bytes.
			/*0x00*/ DWORD ShortLabel1Id;  // Set to 0xffffffff or to string table ID.
			/*0x04*/ DWORD ShortLabel2Id;  // Set to 0xffffffff or to string table ID.
			/*0x08*/ DWORD LabelId;        // AA ability label string table ID.
			/*0x0c*/ DWORD DescriptionId;  // AA description string table ID.
			/*0x10*/ WORD  Unknown10;  // Set to 0x33 or 0x37 or 0x3b etc.
			/*0x12*/ WORD  Cost;  // Set to 1,5, 3, 6, 9 etc.
			/*0x14*/ WORD  Unknown14;  // Set to 0 or 0xffff or 0x13 etc.
			/*0x16*/ WORD  Unknown16;  // Set to 0 or 3.
			/*0x18*/ BYTE  Unknown18;  // Set to 1 or 3 or 5. (ranks?)
			/*0x19*/ BYTE  Unknown19;
			/*0x1a*/ WORD  Unknown1a;  // Set to 1 or 2.
			/*0x1c*/ BYTE  Unknown1c;  // Set to 0 or 1.
			/*0x1d*/ BYTE  Unknown1d;
			/*0x1e*/ WORD  Unknown1e;  // Set to 0xffff or 0xab2.
			/*0x20*/ BYTE  Unknown20;  // Set to 0.
			/*0x21*/ BYTE  Unknown21;  // Set to 0 or 1.
			/*0x22*/ WORD  Unknown22;
			/*0x24*/ DWORD Unknown24;  // Set to 0.
			/*0x28*/ WORD  Unknown28;  // Set to 0.
			/*0x2a*/ WORD  Unknown2a;  // Set to 0.
			/*0x2c*/ WORD  Unknown2c;  // Set to 0.
			/*0x2e*/ BYTE  Unknown2e;  // Set to 0 or 1.
			/*0x2f*/ BYTE  Unknown2f;
			/*0x30*/ DWORD Unknown30;  // Set to 0 or 7 etc.
			/*0x34*/ DWORD Unknown34;  // Set to 0.
		};

		struct AltAdvManager {  // operator new of 0x38c bytes.
			static AltAdvManager* get_manager() {
				return *reinterpret_cast<AltAdvManager**>(0x0063d724); 
			}

			int CalculateMitigationBoost(EQCHARINFO* char_info, int softcap) {
				return reinterpret_cast<int(__thiscall*)(AltAdvManager*, EQCHARINFO *, int)>(0x004b7b48)(this, char_info, softcap);
			}

			// The single class member is just an array of 227 pointers.
			EQAltAbility* abilities[0xe3];
		};


		struct EQCommand
		{
			static constexpr int kNumCommands = 0x116;  // The array has 0x116 entries.
			int string_id;  // For localization table.
			const char* name;  // Internal name.
			const char* localized_name;  // Loaded in LoadString from str.txt files.
			int fn;  // Command function pointer.
			short gm_command;  // If != 0, needs entity.IsGameMaster to be set.
			short category;  // Help command category type.
		};
	}
}

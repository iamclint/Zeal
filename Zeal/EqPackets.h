#pragma once
#include <Windows.h>

namespace Zeal
{
    namespace Packets
    {
        enum opcodes
        {
            DeathDamage = 0x404A,
            Damage = 0x4058,
            PrintNonMeleeDamage = 0x4236,
            CorpseDrag = 0x4114,
            CorpseDrop = 0x1337,
            TargetMouse = 0x4162,
            RequestTrade = 0x40D1,
            WearChange = 0x4092,
            Illusion = 0x4091
        };
        struct TradeRequest_Struct {
            /*000*/	UINT16 to_id;
            /*002*/	UINT16 from_id;
            /*004*/
        };
        struct CorpseDrag_Struct
        {
            /*000*/ char CorpseName[64];
            /*064*/ char DraggerName[64];
            /*128*/ UINT8 Unknown128[24];
            /*152*/
        };
        struct Damage_Struct
        {
            /*000*/	UINT16	target;
            /*002*/	UINT16	source;
            /*004*/	UINT16	type;
            /*006*/	INT16	spellid;
            /*008*/	INT16	damage;
            /*00A*/	float	force;
            /*00E*/	float	sequence; // see above notes in Action_Struct
            /*012*/	float	pushup_angle; // associated with force.  Sine of this angle, multiplied by force, will be z push.
            /*016*/
        };
        struct Death_Struct
        {
            /*000*/	UINT16	spawn_id;
            /*002*/	UINT16	killer_id;
            /*004*/	UINT16	corpseid;
            /*006*/	UINT8	spawn_level;
            /*007*/	UINT8   unknown007;
            /*008*/	INT16	spell_id;
            /*010*/	UINT8	attack_skill;
            /*011*/	UINT8   unknonw011;
            /*012*/	INT32	damage;
            /*016*/	UINT8   is_PC;
            /*017*/	UINT8   unknown015[3];
            /*020*/
        };
        struct ClientTarget_Struct
        {
            /*000*/	UINT16	new_target; // Target spawn ID.
            /*002*/
        };
        struct Tint_Struct {
            union {
                struct {
                    UINT8 Blue;
                    UINT8 Green;
                    UINT8 Red;
                    UINT8 UseTint;	// if there's a tint this is FF
                };
                UINT32 Color;
            };
        };
        struct WearChange_Struct
        {
            /*000*/ UINT16 spawn_id;
            /*002*/ UINT8  wear_slot_id; // 0=Head 1=Chest 2=Arms 3=Wrist 4=Gloves 5=Legs 6=Boots 7=MH 8=OH
            /*003*/ UINT8  align03;
            /*004*/ UINT16 material;     // Armor Material or IT### number in (Item->IDFile)
            /*006*/ UINT16 align06;
            /*008*/ Tint_Struct color;
        };
        struct Illusion_Struct {
            /*000*/ WORD    spawnid;
            /*002*/ short   race;
            /*004*/ BYTE    gender;
            /*005*/ BYTE    texture;
            /*006*/ BYTE    helmtexture;
            /*007*/ BYTE    unknown007; // Usually 0xFF. I believe the client can sometimes use this as the hi-byte of a 2-byte helmtexture though.
            /*008*/ short   face;
            /*010*/ BYTE    hairstyle;
            /*011*/ BYTE    haircolor;
            /*012*/ BYTE    beard;
            /*013*/ BYTE    beardcolor;
            /*014*/ short   unknown_void;
            /*016*/ int     size;
            /*020*/
        };
    }
}

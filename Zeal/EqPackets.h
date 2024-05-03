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
            RequestTrade = 0x40D1
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
            /*006*/	UINT16	spellid;
            /*008*/	UINT16	damage;
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
    }
}
#pragma once
#include "EqStructures.h"
#include "memory.h"
#include <map>
namespace Zeal
{
	namespace EqUI
	{
		struct BaseVTable
		{
			/*0000*/ LPVOID IsValid; 
			/*0004*/ LPVOID  Deconstructor; 
			/*0008*/ LPVOID  DrawNC; 
			/*000C*/ LPVOID  PostDraw; 
			/*0010*/ LPVOID  PostDraw2; 
			/*0014*/ LPVOID  DrawCursor; 
			/*0018*/ LPVOID  OnResize; 
			/*001C*/ LPVOID  PostDraw3; 
			/*0020*/ LPVOID  DrawBackground; 
			/*0024*/ LPVOID  DrawTooltip; 
			/*0028*/ LPVOID  GetMinimizedRect; 
			/*002C*/ LPVOID  DrawTitleBar; 
			/*0030*/ LPVOID  GetCursorToDisplay; 
			/*0034*/ LPVOID  HandleLButtonDown; 
			/*0038*/ LPVOID  HandleLButtonUp; 
			/*003C*/ LPVOID  HandleLButtonHeld; 
			/*0040*/ LPVOID  HandleLButtonUpAfterHeld; 
			/*0044*/ LPVOID  HandleRButtonDown; 
			/*0048*/ LPVOID  HandleRButtonUp; 
			/*004C*/ LPVOID  HandleRButtonHeld; 
			/*0050*/ LPVOID  HandleRButtonUpAfterHeld; 
			/*0054*/ LPVOID  HandleWheelButtonDown; 
			/*0058*/ LPVOID  HandleWheelButtonUp; 
			/*005C*/ LPVOID  HandleMouseMove; 
			/*0060*/ LPVOID  HandleWheelMove; 
			/*0064*/ LPVOID  HandleKeyboardMsg1; 
			/*0068*/ LPVOID  HandleKeyboardMsg2; 
			/*006C*/ LPVOID  OnDragDrop; 
			/*0070*/ LPVOID  GetDragDropCursor; 
			/*0074*/ LPVOID  QueryDropOK; 
			/*0078*/ LPVOID  OnClickStick; 
			/*007C*/ LPVOID  GetDragDropCursor2; 
			/*0080*/ LPVOID  QueryClickStickDropOK; 
			/*0084*/ LPVOID  WndNotification; 
			/*0088*/ LPVOID  Deactivate; 
			/*008C*/ LPVOID  ShowWindow; 
			/*0090*/ LPVOID  OnMove; 
			/*0094*/ LPVOID  OnResize2; 
			/*0098*/ LPVOID  OnMinimizeBox; 
			/*009C*/ LPVOID  OnTileBox; 
			/*00A0*/ LPVOID  CXWnd_OnShowANDPostDraw; 
			/*00A4*/ LPVOID  OnSetFocus; 
			/*00A8*/ LPVOID  OnSetFocus2; 
			/*00AC*/ LPVOID  OnProcessFrame; 
			/*00B0*/ LPVOID  OnVScroll; 
			/*00B4*/ LPVOID  OnHScroll; 
			/*00B8*/ LPVOID  OnBroughtToTop; 
			/*00BC*/ LPVOID  CXWnd_OnSetFocus; 
			/*00C0*/ LPVOID  CXWnd_OnSetFocus2; 
			/*00C4*/ LPVOID  CXWnd_RequestDockInfo; 
			/*00C8*/ LPVOID  CXWnd_GetTooltip; 
			/*00CC*/ LPVOID  CXWnd_HitTest; 
			/*00D0*/ LPVOID  CXWnd_GetHitTestRect; 
			/*00D4*/ LPVOID  CXWnd_GetInnerRect; 
			/*00D8*/ LPVOID  CXWnd_GetClientRect; 
			/*00DC*/ LPVOID  CXWnd_GetMinSize; 
			/*00E0*/ LPVOID  CXWnd_GetUntileSize; 
			/*00E4*/ LPVOID  IsPointTransparent; 
			/*00E8*/ LPVOID  SetDrawTemplate; 
			/*00EC*/ LPVOID  SetWindowTextA; 
			/*00F0*/ LPVOID  SetVScrollPos; 
			/*00F4*/ LPVOID  SetAttributesFromSidl; 
			/*00F8*/ LPVOID  DefineImages; 
		};
		struct ItemDisplayVtable
		{
			BaseVTable basic;
			/*0x0FC*/ LPVOID  Unknown1;
			/*0x100*/ LPVOID  Unknown2;
			/*0x104*/ LPVOID  Activate;
			/*0x108*/ LPVOID  Unknown3;
		};
		struct ContextMenuVTable
		{
			BaseVTable basic;
			/*0x0FC*/ LPVOID  OnHeaderClick;
			/*0x100*/ LPVOID  DrawColumnSeparators;
			/*0x104*/ LPVOID  DrawSeparator;
			/*0x108*/ LPVOID  DrawLine;
			/*0x10c*/ LPVOID  DrawHeader;
			/*0x110*/ LPVOID  DrawItem;
			/*0x114*/ LPVOID  DeleteAll;
			/*0x118*/ LPVOID  Compare;
			/*0x11c*/ LPVOID  Sort;
		};
		struct SpellGem {
			/*0x134*/ BYTE Unknown0x134[0x168];
			/*0x168*/ DWORD   spellicon;//same as in lucys db if this is equal to FFFFFFFF there is no spell memmed in this slot...
			/*0x16c*/ DWORD   spellstate;// 1 = cast in progress or refreshtime not met or gem is empty 2 means we ducked or aborted cast, 0 means its ok to cast
			/*0x170*/
		};
		struct pCXSTR
		{
			/*0x00*/   DWORD   Font;            // maybe, dont know.  04 = Window 01 = button
			/*0x04*/   DWORD   MaxLength;
			/*0x08*/   DWORD   Length;
			/*0x0c*/   BOOL    Encoding; // 0: ASCII, 1:Unicode
			/*0x10*/   PCRITICAL_SECTION pLock;
			/*0x14*/   CHAR    Text[1]; // Stub, can be anywhere from Length to MaxLength (which is how much is malloc'd to this CXStr)
		};
		struct CXSTR {
			CXSTR() { Data = nullptr; };

			CXSTR(const char* data)
			{
				reinterpret_cast<void(__thiscall*)(const CXSTR*, const char*)>(0x575F30)(this, data);
			}
			CXSTR(std::string data)
			{
				reinterpret_cast<void(__thiscall*)(const CXSTR*, const char*)>(0x575F30)(this, data.c_str());
			}
			void Assure(int length, int encoding)
			{
				reinterpret_cast<void(__thiscall*)(const CXSTR*, int, int)>(0x575A60)(this, length, encoding);
				
			}
			void FreeRep()
			{
				reinterpret_cast<void(__thiscall*)(const CXSTR*, pCXSTR*)>(0x575DC0)(this, Data);
			}
			pCXSTR* Data;
		};

		struct CXRect
		{
			int Left;
			int Top;
			int Right;
			int Bottom;
			CXRect() {}
			CXRect(int l, int t, int r, int b) { Left = l; Top = t; Right = r; Bottom = b; }
		};

		struct EQFONT
		{
			/* 0x0000 */ DWORD Unknown0000;
			/* 0x0004 */ DWORD Size;
		};

		struct ARGBCOLOR {
			union {
				struct {
					BYTE B;
					BYTE G;
					BYTE R;
					BYTE A;
				};
				DWORD ARGB;
			};
			ARGBCOLOR(BYTE _R, BYTE _G, BYTE _B, BYTE _A) : A(_A), R(_R), G(_G), B(_B) {};
			ARGBCOLOR(DWORD _ARGB) : ARGB(_ARGB) {};
			ARGBCOLOR() : A{}, R{}, G{}, B{} {};

		};
		struct ControlTemplate
		{
			char Unknown0x0[0x20];
			pCXSTR* Item;
		};
		struct BasicWnd
		{
			//BasicWnd() {};
			void SetFocus()
			{
				reinterpret_cast<void(__thiscall*)(const BasicWnd*)>(0x572290)(this);
			}
			void SetupCustomVTable()
			{
				BaseVTable* newtbl = new BaseVTable();
				mem::copy((int)newtbl, (int)vtbl, sizeof(BaseVTable));
				vtbl = newtbl;
				mem::unprotect_memory(vtbl, sizeof(BaseVTable));
			}
			void show(bool unkown, bool visible)
			{
				reinterpret_cast<void(__thiscall*)(const BasicWnd*, bool, bool)>(0x572310)(this, unkown, visible);
			}
			BasicWnd* GetChildItem(CXSTR name)
			{
				return reinterpret_cast<BasicWnd*(__thiscall*)(const BasicWnd*, CXSTR)>(0x570320)(this, name);
			}
			void CreateChildren()
			{
				reinterpret_cast<BasicWnd*(__thiscall*)(const BasicWnd*)>(0x56f4f0)(this);
			}
			int WndNotification(int a1, int a2, int a3)
			{
				return reinterpret_cast<int(__thiscall*)(const BasicWnd*, int,int,int)>(vtbl->WndNotification)(this, a1, a2, a3);
			}

			/* 0x0000 */ BaseVTable* vtbl;
			/* 0x0004 */ DWORD MouseHoverTimer;
			/* 0x0008 */ DWORD Unknown0008;
			/* 0x000C */ DWORD Unknown000C;
			/* 0x0010 */ BYTE Unknown0010;
			/* 0x0011 */ BYTE Unknown0011;
			/* 0x0012 */ BYTE IsLocked;
			/* 0x0013 */ BYTE Unknown0013;
			/* 0x0014 */ PVOID Unknown0014;
			/* 0x0018 */ DWORD Unknown0018;
			/* 0x001C */ struct EQWND* ParentWnd;
			/* 0x0020 */ struct EQWND* FirstChildWnd;
			/* 0x0024 */ struct EQWND* NextSiblingWnd;
			/* 0x0028 */ BYTE HasChildren;
			/* 0x0029 */ BYTE HasSiblings;
			/* 0x002A */ BYTE Unknown0030[2];
			/* 0x002C */ DWORD Flags;
			/* 0x0030 */ CXRect Location;
			/* 0x0040 */ CXRect LocationPlaceholder; // used when minimizing the window
			/* 0x0050 */ BYTE IsVisible; // show
			/* 0x0051 */ BYTE IsEnabled;
			/* 0x0052 */ BYTE IsMinimized;
			/* 0x0053 */ BYTE Unknown0053;
			/* 0x0054 */ BYTE IsOpen;
			/* 0x0055 */ BYTE Unknown0055;
			/* 0x0056 */ BYTE IsMouseOver; // mouse is hovering over
			/* 0x0057 */ BYTE Unknown0057;
			/* 0x0058 */ DWORD WindowStyleFlags;
			/* 0x005C */ EQFONT* FontPointer;
			/* 0x0060 */ CXSTR Text;
			/* 0x0064 */ CXSTR ToolTipText;
			/* 0x0068 */ ARGBCOLOR TextColor;
			/* 0x006C */ ARGBCOLOR ToolTipTextColor;
			/* 0x006C */ BYTE Unknown0068[20];
			/* 0x0084 */ CXSTR XmlToolTipText;
			/* 0x0088 */ BYTE Unknown0088[22];
			/* 0x009E */ BYTE AlphaTransparency;
			/* 0x009F */ BYTE Unknown009F;
			/* 0x00A0 */ BYTE ZLayer;
			/* 0x00A1 */ BYTE Unknown00A1[7];
			/* 0x00A8 */ DWORD DrawTemplate;
			/*0x0ac*/   BYTE    Unknown0x0ac[0x4];
			/*0x0b0*/   DWORD   ZLayer2;
			/*0x0b4*/   BYTE   Unknown0x0b4[0x28];
			/*0x0dc*/   DWORD   FadeTickCount;
			/*0x0e0*/   BYTE    Unknown0x0f8; /* CXWnd::StartFade */
			/*0x0e1*/   BYTE    Unknown0x0f9; /* CXWnd::StartFade */
			/*0x0e2*/   BYTE    Unknown0x0fa;
			/*0x0e3*/   BYTE    Unknown0x0fb;
			/*0x0e4*/   DWORD   Unknown0x0fc; /* CXWnd::StartFade, CXWnd::Minimize */
			/*0x0e8*/   DWORD   VScrollMax;
			/*0x0ec*/   DWORD   VScrollPos;
			/*0x0f0*/   DWORD   HScrollMax;
			/*0x0f4*/   DWORD   HScrollPos;
			/*0x0f8*/   BYTE    ValidCXWnd;
			/*0x0f9*/   BYTE    Unused0x0f9[0x3];
			/*0x0fc*/   union {

				struct pCXSTR* SidlText;
				DWORD Items;
			};
			/*0x100*/   union {

				struct _CXSTR* SidlScreen;
				DWORD   SlotID;
				DWORD	Caret_Start;
				DWORD	ItemCount;
			};
			union {
				/*0x104*/   LPVOID SidlPiece; /* CScreenPieceTemplate (important) */
				DWORD Caret_End;
			};
			/*0x108*/   BYTE    Checked;
			/*0x109*/   BYTE    Highlighted;
			/*0x10a*/   BYTE    Unused0x10a[0x2];
			/*0x10c*/   DWORD   TextureAnim; /* used in CSidlScreenWnd::AddButtonToRadioGroup */
			/*0x110*/   CXSTR   InputText;
		};
		struct EQWND : BasicWnd
		{
			EQWND() {};
		
			/*0x114*/   DWORD   Selector;
			/*0x118*/   DWORD   PushToSelector;
			union
			{
				/*0x11c*/   DWORD   EnableINIStorage;
				/*0x11c*/   int   SelectedIndex;
			};
			/*0x120*/   union {

				struct _CXSTR* INIStorageName;
				struct _EQINVSLOT* pEQInvSlot;
			};
			/*0x124*/   DWORD   Unknown0x124; /* CTextureAnimation */
			/*0x128*/   DWORD   Unknown0x128; /* CTextureAnimation */
			/*0x12c*/   DWORD   ContextMenu; /* CTextureAnimation its an id for the menu*/
			/*0x130*/	BYTE    Unknown0x130; /* CTextureAnimation */
			/*0x131*/	BYTE    Unknown0x131; /* CTextureAnimation */
			/*0x132*/	BYTE    Unknown0x132; /* CTextureAnimation */
			/*0x133*/	BYTE    Unknown0x133; /* CTextureAnimation */
		};
		struct RaidWnd : EQWND
		{
			/*0x134*/	BYTE    ToggleState;
		};
		struct HotButton : EQWND
		{
			int GetPage() { return *(int*)0x7f69f6; }
			BYTE GetType(int button_index) { return *(BYTE*)(0x7f6862 + (button_index + (GetPage() * 0xA))); }
		};

		struct ListWnd : EQWND
		{
			ListWnd() {};
			int AddString(std::string str)
			{
				return reinterpret_cast<int (__thiscall*)(const ListWnd*, CXSTR, UINT, UINT, UINT)>(0x5797A0)(this, CXSTR(str), 0xffffffff, 0, 0);
			}
			void SetItemText(std::string str, int row, int column)
			{
				reinterpret_cast<void(__thiscall*)(const ListWnd*, int, int, CXSTR)>(0x579DC0)(this, row, column, CXSTR(str));
			}
			void SetItemData(int row) //not sure why this is needed
			{
				reinterpret_cast<void(__thiscall*)(const ListWnd*, int, int)>(0x579D70)(this, row, row);
			}
			void Sort(int col) 
			{
				reinterpret_cast<void(__thiscall*)(const ListWnd*, int)>(0x57cb00)(this,col);
			}
			void DeleteAll()
			{
				reinterpret_cast<void(__thiscall*)(const ListWnd*)>(0x579530)(this);
			}
			int GetItemData(int row)
			{
				return reinterpret_cast<int (__thiscall*)(const ListWnd*, int)>(0x578E80)(this, row);
			}
			ListWnd* GetItemText(CXSTR* buffer, int row, int col)
			{
				return reinterpret_cast<ListWnd*(__thiscall*)(const ListWnd*, CXSTR*, int, int)>(0x578ed0)(this, buffer, row, col);
			}
		};

		struct ItemDisplayWnd : EQWND
		{
			ItemDisplayWnd() {};
			void SetupCustomVTable()
			{
				ItemDisplayVtable* newtbl = new ItemDisplayVtable();
				mem::copy((int)newtbl, (int)vtbl, sizeof(ItemDisplayVtable));
				vtbl = (BaseVTable*)newtbl;
				mem::unprotect_memory(vtbl, sizeof(ItemDisplayVtable));
				
			}
			void Activate()
			{
				reinterpret_cast<void(__thiscall*)(const ItemDisplayWnd*)>(0x423606)(this);
			}
			void Unk3()
			{
				reinterpret_cast<void(__thiscall*)(const ItemDisplayWnd*)>(0x423606)(this);
			}
			void SetItem(struct Zeal::EqStructures::_EQITEMINFO* Item, bool unk)
			{
				reinterpret_cast<void(__thiscall*)(const ItemDisplayWnd*, struct Zeal::EqStructures::_EQITEMINFO *, bool)>(0x423640)(this, Item, unk);
			}
			void SetSpell(short spell_id, bool unk, int unk2)
			{
				reinterpret_cast<void(__thiscall*)(const ItemDisplayWnd*, short, bool, int)>(0x425957)(this, spell_id, unk, unk2);
			}


			/* 0x0134 */ struct EQWND* ItemDescription; // the item stats text window
			/* 0x0138 */ BYTE Unknown0138[4];
			/* 0x013C */ struct EQWND* IconBtn; // the item stats text window
			/* 0x0140 */ BYTE Unknown0140[4];
			/* 0x0144 */ CXSTR DisplayText; // the item name is the title text
			/* 0x0148 */ CXSTR WindowTitle; // the item stats text
			/* 0x014C */ Zeal::EqStructures::_EQITEMINFO Item;
			//BYTE Filler[0xEC];

		};
		struct SliderWnd : EQWND
		{
			SliderWnd() {};
			void SetupCustomVTable()
			{
				BaseVTable* newtbl = new BaseVTable();
				mem::copy((int)newtbl, (int)vtbl, sizeof(BaseVTable));
				vtbl = (BaseVTable*)newtbl;
				mem::unprotect_memory(vtbl, sizeof(BaseVTable));

			}
			/* 0x0134 */ int current_val;
			/* 0x0138 */ BYTE Unknown0138[4];
			/* 0x013C */ int max_val; 
			/* 0x0140 */ int val3;
			/* 0x0144 */ BYTE Unknown0144[0xC]; // the item name is the title text
		};
		struct TradeWnd : public EQWND
		{
			/* 0x0134 */ DWORD Unk1[39];
			/* 0x01D0 */ Zeal::EqStructures::_EQITEMINFO* Item[8];
		};
		struct LootWnd : public EQWND
		{
			void RequestLootSlot(UINT slot, BYTE inventory)
			{
				reinterpret_cast<void(__thiscall*)(const LootWnd*, UINT, bool)>(0x426b02)(this, slot, inventory);
			}

			
			/* 0x0134 */ DWORD Unk1;
			/* 0x0138 */ DWORD ItemSlotIndex[EQ_NUM_LOOT_WINDOW_ITEMS];
			/* 0x01B0 */ DWORD Timer;
			/* 0x01B4 */ PVOID Unknown01B4;
			/* 0x01B8 */ Zeal::EqStructures::_EQITEMINFO* Item[EQ_NUM_LOOT_WINDOW_ITEMS];
		};

		struct EditWnd : public BasicWnd
		{
			//void ReplaceSelection(const char* data, int length)
			//{
			//	reinterpret_cast<void(__thiscall*)(const EditWnd*, const char*, int)>(0x5a41b0)(this, data, length);
			//}
			void ReplaceSelection(CXSTR data, bool filter_input)
			{
				reinterpret_cast<void(__thiscall*)(const EditWnd*, CXSTR, int)>(0x5a3f00)(this, data, filter_input);
			}
			int GetInputLength()
			{
				return this->InputText.Data->Length - (this->item_link_count * 9);
			}
			void SetText(CXSTR data)
			{
				reinterpret_cast<void(__thiscall*)(const EditWnd*, CXSTR)>(0x5a3d00)(this, data);
			}
			void AddItemTag(int item_id, const char* name)
			{
				reinterpret_cast<void(__thiscall*)(const EditWnd*, int, const char*)>(0x5a2920)(this, item_id, name);
			}
			/* 0x0114 */ DWORD LinkStartIndex[10]; 
			/* 0x013C */ DWORD LinkEndIndex[10];
			/* 0x0164 */ BYTE UnknownBytes[0xA0];
			/* 0x0204 */ DWORD item_link_count;
			/* 0x0208 */ BYTE uk1[3];
			/* 0x020B */ BYTE SomeFlag;
			/* 0x020C */ DWORD uk2;
			/* 0x0210 */ DWORD AlphaNumericOnly;
		};
		struct ChatWnd : public EQWND
		{
			/*0x134*/ EditWnd* unk;
			/*0x138*/ EditWnd* edit;
		};
		struct EQKey
		{
			/* 0x0000 */ UINT32 code;
			/* 0x0004 */ int timestamp;
			/* 0x0008 */ int unk;
			/* 0x0010 */ int isDown;
			/* 0x0014 */ int whoknows;
		};

		struct CTextureFont
		{
			int DrawWrappedText(CXSTR str, CXRect rect1, CXRect rect2, unsigned long color, unsigned short unk1, int unk2) const {

				return reinterpret_cast<int(__thiscall*)(const CTextureFont*, CXSTR, int, int , int, CXRect, unsigned long, unsigned short, int)>(0x5A4970)(this, str, rect1.Top, rect1.Left, rect1.Right, rect2, color, unk1, unk2);
			}
			int GetHeight() { return reinterpret_cast<int(__thiscall*)(const CTextureFont*)>(0x5A4930)(this); }
		};
		struct CXWndManager
		{
			//get font 
			CTextureFont* GetFont(int index) { return reinterpret_cast<CTextureFont*(__thiscall*)(const CXWndManager*, int)>(0x538EAA)(this, index); }
			/* 0x0000 */ DWORD Unknown0x0;
			/* 0x0004 */ DWORD Unknown0x4;
			/* 0x0008 */ DWORD Unknown0x8;
			/* 0x000C */ DWORD Unknown0xC;
			/* 0x0010 */ DWORD Unknown0x10;
			/* 0x0014 */ EQKey LastKey;
			/* 0x0028 */ EditWnd* ActiveEdit;
			/* 0x002C */ int unknown1;
			/* 0x0030 */ EQWND* Focused;
			/* 0x0034 */ BYTE unknown34[0x8];
			/* 0x0040 */ EQWND* Hovered;
				
		};



		class ContextMenu
		{
		public:
			ContextMenu(int cxwnd, int a1, CXRect r)
			{
				reinterpret_cast<void(__thiscall*)(const ContextMenu*, int, int, CXRect)>(0x417785)(this, cxwnd, a1, r);
				SetupCustomVTable();
			}
			void AddSeparator() const {
				reinterpret_cast<void(__thiscall*)(const ContextMenu*)>(0x417A41)(this);
					
			}
			void EnableMenuItem(int index, bool toggle)
			{
				reinterpret_cast<void(__thiscall*)(const ContextMenu*, int, bool)>(0x417a93)(this, index, toggle);
			}
			void EnableLine(int index, bool toggle) //this one doesn't forcibly change the color
			{
				reinterpret_cast<void(__thiscall*)(const ContextMenu*, int, bool)>(0x579f90)(this, index, toggle);
			}
			int AddMenuItem(CXSTR data, int index, bool disp_activated=false) const {
				return reinterpret_cast<int(__thiscall*)(const ContextMenu*, CXSTR, int, bool)>(0x417910)(this, data, index, disp_activated);
			}
			void SetItemColor(int index, ARGBCOLOR color)
			{
				reinterpret_cast<void(__thiscall*)(const ContextMenu*, int, int, ARGBCOLOR)>(0x579eb0)(this, index, 0x1, color);
				reinterpret_cast<void(__thiscall*)(const ContextMenu*, int, int, ARGBCOLOR)>(0x579eb0)(this, index, 0x2, color);
			}
			static void __fastcall Deconstructor(int t, int unused, int f)
			{
				return; //handle the deconstruction in our code
			}
			void SetupCustomVTable()
			{
				ContextMenuVTable* newtbl = new ContextMenuVTable();
				mem::copy((int)newtbl, (int)fnTable, sizeof(ContextMenuVTable));
				fnTable = newtbl;
				//fnTable->basic.Deconstructor = Deconstructor;//if you can make visual studio use a class member here or get the address of it without it complaining, go ahead
			}
			void RemoveAllMenuItems()
			{
				reinterpret_cast<void(__thiscall*)(const ContextMenu*)>(0x417a7f)(this);
			}
			void Deconstruct(int a)
			{
				reinterpret_cast<void(__thiscall*)(const ContextMenu*, int)>(this->fnTable->basic.Deconstructor)(this,a);
			}
			/*0x000*/   ContextMenuVTable* fnTable;
			/*0x004*/   DWORD   Unknown0x004; /* set to 0 in CXWnd::Refade*/
			/*0x008*/   DWORD   TimeMouseOver; /* "Delay" in ini*/
			/*0x00c*/   DWORD   FadeDuration; /* "Duration" in ini*/
			/*0x010*/   BYTE    FadeToAlpha; /* set to 1 in CXWnd::Refade */
			/*0x011*/   BYTE    Unknown0x011; /* Faded? */
			/*0x012*/   BYTE    Locked;
			/*0x013*/   BYTE    Unknown0x013;
			/*0x014*/   BYTE    Clickable;
			/*0x015*/   BYTE    Unknown0x015;
			/*0x016*/   BYTE    Unknown0x016;
			/*0x017*/   BYTE    Unknown0x017;
			/*0x018*/   BYTE    Unknown0x018[0x04];
			/*0x01c*/   struct  _CSIDLWND* pParentWindow; /* If this is NULL, coordinates are absolute...*/
			/*0x020*/   struct  _CSIDLWND* pChildren;
			/*0x024*/   struct  _CSIDLWND* pSiblings;  /* its a tree.*/
			/*0x028*/   BYTE    HasChildren; /*CXWnd__GetFirstChildWnd*/
			/*0x029*/   BYTE    HasSiblings;/*CXWnd__GetNextSib*/
			/*0x02a*/   BYTE    Unknown0x02a[0x2];
			/*0x02c*/   DWORD   XMLIndex;
			/*0x030*/   RECT    Location;
			/*0x040*/   RECT    OldLocation;
			/*0x050*/   BYTE    dShow;
			/*0x051*/   BYTE    Enabled;
			/*0x052*/   BYTE    Minimized;
			/*0x053*/   BYTE    Unknown0x053; /*ontilebox*/
			/*0x054*/   BYTE    Unknown0x054;
			/*0x055*/   BYTE    Unknown0x055;
			/*0x056*/   BYTE    MouseOver;
			/*0x057*/   BYTE    Unknown0x057;
			/*0x058*/   DWORD   WindowStyle; /* bit 1 - vertical scroll, bit 2 - horizontal scroll, bit 4 - title bar?, bit 8 - border*/
			/*0x05c*/   EQFONT*   TextureFont; /*its a CTextureFont* */
			/*0x060*/   struct _CXSTR* WindowText;
			/*0x064*/   struct _CXSTR* Tooltip;
			/*0x068*/   DWORD   UnknownCW; /* CXWnd::SetLookLikeParent*/
			/*0x06c*/   ARGBCOLOR BGColor; /* "BGTint.Red", green, blue*/
			/*0x070*/   DWORD    Unknown0x070;
			/*0x074*/   BYTE    Unknown0x074[0x4];
			/*0x078*/   FLOAT    Unknown0x078;
			/*0x07C*/   BYTE    Unknown0x07C[0x4];
			/*0x080*/   DWORD   BGType; /* "BGType" in ini */
			/*0x084*/   struct _CXSTR* XMLToolTip;
			/*0x088*/   BYTE    Unknown0x088[0x14];
			/*0x09c*/   BYTE    Alpha; /* "Alpha" in ini */
			/*0x09d*/   BYTE    Fades; /* "Fades" in ini */
			/*0x09e*/   BYTE    Unknown0x0aa;
			/*0x09f*/   BYTE    Unknown0x0ab;
			/*0x0a0*/   BYTE    Unknown0x0a0[0x8];
			/*0x0a8*/   LPVOID  DrawTemplate;
			/*0x0ac*/   BYTE    Unknown0x0ac[0x4];
			/*0x0b0*/   DWORD   ZLayer;
			/*0x0b4*/   BYTE   Unknown0x0b4[0x28];
			/*0x0dc*/   DWORD   FadeTickCount;
			/*0x0e0*/   BYTE    Unknown0x0f8; /* CXWnd::StartFade */
			/*0x0e1*/   BYTE    Unknown0x0f9; /* CXWnd::StartFade */
			/*0x0e2*/   BYTE    Unknown0x0fa;
			/*0x0e3*/   BYTE    Unknown0x0fb;
			/*0x0e4*/   DWORD   Unknown0x0fc; /* CXWnd::StartFade, CXWnd::Minimize */
			/*0x0e8*/   DWORD   VScrollMax;
			/*0x0ec*/   DWORD   VScrollPos;
			/*0x0f0*/   DWORD   HScrollMax;
			/*0x0f4*/   DWORD   HScrollPos;
			/*0x0f8*/   BYTE    ValidCXWnd;
			/*0x0f9*/   BYTE    Unused0x0f9[0x3];
			/*0x0fc*/   union {
				struct _CXSTR* SidlText;
				DWORD Items;
			};
			/*0x100*/   union {
				struct _CXSTR* SidlScreen;
				DWORD   SlotID;
			};
			/*0x104*/   LPVOID SidlPiece; /* CScreenPieceTemplate (important) */
			/*0x108*/   BYTE    Checked;
			/*0x109*/   BYTE    Highlighted;
			/*0x10a*/   BYTE    Unused0x10a[0x2];
			/*0x10c*/   DWORD   TextureAnim; /* used in CSidlScreenWnd::AddButtonToRadioGroup */
			/*0x110*/   struct _CXSTR* InputText;
			/*0x114*/   DWORD   Selector;
			/*0x118*/   DWORD   PushToSelector;
			/*0x11c*/   int   SelectedIndex;
			/*0x120*/   union {
				struct _CXSTR* INIStorageName;
				struct _EQINVSLOT* pEQInvSlot;
			};
			/*0x124*/   DWORD   Unknown0x124; /* CTextureAnimation */
			/*0x128*/   DWORD   Unknown0x128; /* CTextureAnimation */
			/*0x12c*/   DWORD  ContextMenu1; /* CTextureAnimation its an id for the menu*/
			/*0x130*/	int* Unknown0x130; /* CTextureAnimation */
			/*0x134*/	BYTE Unknown0x134;
			/*0x135*/	BYTE Unknown0x135;
			/*0x136*/	BYTE Unknown0x136;
			/*0x137*/	BYTE Unknown0x137;
			/*0x138*/	BYTE Unknown0x138;
			/*0x139*/	BYTE Unknown0x139;
			/*0x13a*/	BYTE Unknown0x13a;
			/*0x13b*/	BYTE Unknown0x13b;
			/*0x13c*/	int* Unknown0x13c;
			/*0x140*/	int* Unknown0x140;
			/*0x144*/	BYTE Unknown0x144;
			/*0x145*/	BYTE Unknown0x145;
			/*0x146*/	BYTE Unknown0x146;
			/*0x147*/	BYTE Unknown0x147;
			/*0x148*/	int* Unknown0x148;
			/*0x14c*/	DWORD Unknown0x14c;
			/*0x150*/	int* Unknown0x150;
			/*0x154*/	DWORD Unknown0x154;
			/*0x158*/	int* Unknown0x158;
			/*0x15c*/	DWORD Unknown0x15c;
			/*0x160*/	int* Unknown0x160;
			/*0x164*/	DWORD Unknown0x164;
			/*0x168*/	int* Unknown0x168;
			/*0x16c*/	DWORD Unknown0x16c;
			/*0x170*/	int* Unknown0x170;
			/*0x174*/	DWORD Unknown0x174;
			/*0x178*/	int* Unknown0x178;
			/*0x17c*/	DWORD Unknown0x17c;

		};

		struct CXPoint
		{
			int x;
			int y;
			CXPoint(int _x, int _y) : x(_x), y(_y) {};
		};

		class CChatManager : public EQWND
		{
		public: //this class is a complete hack lol
			struct ChatWnd* GetActiveChatWindow() const {
				return reinterpret_cast<struct ChatWnd* (__thiscall*)(const CChatManager*)>(0x41114A)(this);
			}
			EQWND* ChatWindows[32];
			DWORD MaxChatWindows;
		};


		class CContextMenuManager
		{
		public: //this class is a complete hack lol
			int GetDefaultMenuIndex() const {
				return reinterpret_cast<int(__thiscall*)(const CContextMenuManager*)>(0x4137C2)(this);
			}
			int AddMenu(ContextMenu* context_menu) const {
				return reinterpret_cast<int(__thiscall*)(const CContextMenuManager*, ContextMenu*)>(0x417ED4)(this, context_menu);
			}
			int PopupMenu(int index, CXPoint pt, EQWND* menu)
			{
				return reinterpret_cast<int(__thiscall*)(const CContextMenuManager*, int, CXPoint, EQWND*)>(0x41822D)(this, index, pt, menu);
			}
			int RemoveMenu(int menu_index, bool remove_children)
			{
				return reinterpret_cast<int(__thiscall*)(const CContextMenuManager*, int,bool)>(0x417E1B)(this, menu_index, remove_children);
			}
			/*0x0000*/ BYTE Unknown0x0000[0x128];//yeah i know its a window...
			/*0x0128*/ void* Menus[0x400];
			/*0x1128*/ DWORD MenuCount;
		};

		class SpellBookWnd : public EQWND
		{
		public:
			void BeginMemorize(int book_index, int gem_index, bool unsure) const
			{
				reinterpret_cast<void(__thiscall*)(const EQWND*, int, int, bool)>(0x434a05)(this, book_index, gem_index, unsure);
			}
			void OpenBook() const
			{
				reinterpret_cast<void(__thiscall*)(const EQWND*)>(0x43441F)(this);
			}
		};

		class SpellGemsWnd
		{
		public:
			void Forget(int index) const
			{
				reinterpret_cast<void(__thiscall*)(const SpellGemsWnd*, int)>(0x40a662)(this, index);
			}
			void UpdateSpellGems(int index) const
			{
				reinterpret_cast<void(__thiscall*)(const SpellGemsWnd*, int)>(0x40a8b7)(this, index);
			}
			EQWND sidl;
			/*0x134*/ BYTE Unknown0x134[0x08];
			/*0x13C*/ SpellGem* SpellSlots[0x8];
			/*0x15C*/ EQWND* SpellBook;
		};

		struct pInstWindows
		{
			CContextMenuManager* ContextMenuManager;  // 0x63D5CC
			CChatManager* ChatManager;  // 0x63D5D0
			EQWND* uknownWnd1;  // 0x63D5D4
			EQWND* CharacterSelect;  // 0x63D5D8
			EQWND* FacePick;  // 0x63D5DC
			ItemDisplayWnd* ItemWnd;  // 0x63D5E0
			EQWND* Note;  // 0x63D5E4
			EQWND* Help;  // 0x63D5E8
			EQWND* Book;  // 0x63D5EC
			EQWND* PetInfo;  // 0x63D5F0
			EQWND* Train;  // 0x63D5F4
			EQWND* Skills;  // 0x63D5F8
			EQWND* SkillsSelect;  // 0x63D5FC
			EQWND* Friends;  // 0x63D600
			EQWND* AA;  // 0x63D604
			EQWND* Group;  // 0x63D608
			EQWND* Loadskin;  // 0x63D60C
			EQWND* Alarm;  // 0x63D610
			EQWND* MusicPlayer;  // 0x63D614
			RaidWnd* Raid;  // 0x63D618
			EQWND* RaidOptions;  // 0x63D61C
			EQWND* Breath;  // 0x63D620
			EQWND* Target;  // 0x63D624
			HotButton* HotButton;  // 0x63D628
			EQWND* ColorPicker;  // 0x63D62C
			EQWND* Player;  // 0x63D630
			EQWND* Options;  // 0x63D634
			EQWND* BuffWindowNORMAL;  // 0x63D638
			EQWND* CharacterCreation;  // 0x63D63C
			EQWND* CursorAttachment;  // 0x63D640
			EQWND* Casting;  // 0x63D644
			SpellGemsWnd* SpellGems;  // 0x63D648
			SpellBookWnd* SpellBook;  // 0x63D64C
			EQWND* Inventory;  // 0x63D650
			EQWND* Bank;  // 0x63D654
			EQWND* Quantity;  // 0x63D658
			LootWnd* Loot;  // 0x63D65C
			EQWND* Actions;  // 0x63D660
			EQWND* Merchant;  // 0x63D664
			TradeWnd* Trade;  // 0x63D668
			EQWND* Selector;  // 0x63D66C
			EQWND* Bazaar;  // 0x63D670
			EQWND* BazaarSearch;  // 0x63D674
			EQWND* Give;  // 0x63D678
			EQWND* Tracking;  // 0x63D67C
			EQWND* Inspect;  // 0x63D680
			EQWND* SocialEdit;  // 0x63D684
			EQWND* Feedback;  // 0x63D688
			EQWND* BugReport;  // 0x63D68C
			EQWND* VideoModes;  // 0x63D690
			EQWND* TextEntry;  // 0x63D694
			EQWND* FileSelection;  // 0x63D698
			EQWND* Compass;  // 0x63D69C
			EQWND* PlayerNotes;  // 0x63D6A0
			EQWND* GemsGame;  // 0x63D6A4
			EQWND* TimeLeft;  // 0x63D6A8
			EQWND* PetitionQ;  // 0x63D6AC
			EQWND* Soulmark;  // 0x63D6B0
			void* InvSlotMgr;  // 0x63D6B4
			void* ContainerMgr;  // 0x63D6B8
		};



	}
}
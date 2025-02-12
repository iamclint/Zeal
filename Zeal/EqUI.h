#pragma once
#include "EqStructures.h"
#include "memory.h"
#include <map>
namespace Zeal
{
	namespace EqUI
	{
		static HANDLE* Heap = (HANDLE*)0x0080b420;

		struct BaseVTable  // Equivalent to CXWnd's VTable in eqmac.exe
		{
			/*0000*/ LPVOID  IsValid;
			/*0004*/ LPVOID  Deconstructor; 
			/*0008*/ LPVOID  DrawNC;
			/*000C*/ LPVOID  Draw;
			/*0010*/ LPVOID  PostDraw;
			/*0014*/ LPVOID  DrawCursor; 
			/*0018*/ LPVOID  DrawChildItem;
			/*001C*/ LPVOID  DrawCaret;
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
			/*0064*/ LPVOID  TranslateKeyboardMsg;
			/*0068*/ LPVOID  HandleKeyboardMsg;
			/*006C*/ LPVOID  OnDragDrop; 
			/*0070*/ LPVOID  GetDragDropCursor; 
			/*0074*/ LPVOID  QueryDropOK; 
			/*0078*/ LPVOID  OnClickStick; 
			/*007C*/ LPVOID  OnClickStickCursor;
			/*0080*/ LPVOID  QueryClickStickDropOK; 
			/*0084*/ LPVOID  WndNotification; 
			/*0088*/ LPVOID  Deactivate; 
			/*008C*/ LPVOID  OnShow;
			/*0090*/ LPVOID  OnMove;
			/*0094*/ LPVOID  OnResize;
			/*0098*/ LPVOID  OnMinimizeBox;
			/*009C*/ LPVOID  OnTileBox;
			/*00A0*/ LPVOID  OnTile;
			/*00A4*/ LPVOID  OnSetFocus;
			/*00A8*/ LPVOID  OnKillFocus;
			/*00AC*/ LPVOID  OnProcessFrame; 
			/*00B0*/ LPVOID  OnVScroll; 
			/*00B4*/ LPVOID  OnHScroll; 
			/*00B8*/ LPVOID  OnBroughtToTop; 
			/*00BC*/ LPVOID  OnActivate;
			/*00C0*/ LPVOID  AboutToDeleteWnd;
			/*00C4*/ LPVOID  RequestDockInfo;
			/*00C8*/ LPVOID  GetTooltip;
			/*00CC*/ LPVOID  HitTest;
			/*00D0*/ LPVOID  GetHitTestRect;
			/*00D4*/ LPVOID  GetInnerRect;
			/*00D8*/ LPVOID  GetClientRect;
			/*00DC*/ LPVOID  GetMinSize;
			/*00E0*/ LPVOID  GetUntileSize;
			/*00E4*/ LPVOID  IsPointTransparent; 
			/*00E8*/ LPVOID  SetDrawTemplate; 
			/*00EC*/ LPVOID  SetWindowTextA; 
			/*00F0*/ LPVOID  SetVScrollPos; 
			/*00F4*/ LPVOID  SetAttributesFromSidl; 
			/*00F8*/ LPVOID  OnReloadSidl;
		};
		struct CXPoint
		{
			int x;
			int y;
			CXPoint(int _x, int _y) : x(_x), y(_y) {};
		};
		struct CXRect
		{
			int Left = 0;
			int Top = 0;
			int Right = 0;
			int Bottom = 0;
			//CXRect() {}
			//CXRect(int l, int t, int r, int b) { Left = l; Top = t; Right = r; Bottom = b; }
		};
		struct SidlScreenWndVTable : BaseVTable  // VTable for EQWND
		{
			/*00FC*/ LPVOID  LoadIniInfo;
			/*0100*/ LPVOID  StoreIniInfo;
		};
		struct ButtonWndVTable : BaseVTable
		{
			/*0x0FC*/ LPVOID  SetRadioGroup;
		};
		struct ItemDisplayVTable : SidlScreenWndVTable
		{
			/*0x104*/ LPVOID  Activate;
		};
		struct ContextMenuVTable : BaseVTable // Not a SidlScreenWnd derived class.  CListWnd has the same layout.
		{
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
			/*0x000*/ BYTE    Unknown0000[0x168];
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
				if (Data)
					reinterpret_cast<void(__thiscall*)(const CXSTR*, pCXSTR*)>(0x575DC0)(this, Data);
			}
			char* CastToCharPtr() const
			{
				return reinterpret_cast<char*(__thiscall*)(const CXSTR*)>(0x577E80)(this);
			}
			void Append(const std::string data)
			{
				reinterpret_cast<void(__thiscall*)(const CXSTR*, const char*)>(0x577310)(this, data.c_str());
			}
			void Set(const std::string data)
			{
				reinterpret_cast<void(__thiscall*)(const CXSTR*, const char*)>(0x576190)(this, data.c_str());
			}
			void Set(const char* data)
			{
				reinterpret_cast<void(__thiscall*)(const CXSTR*, const char*)>(0x576190)(this, data);
			}
			operator std::string() const {
				const char* result = CastToCharPtr();
				if (result)
					return std::string(result);
				else
					return "";
			}
			//~CXSTR()
			//{
			//	FreeRep();
			//}
			pCXSTR* Data;
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
		struct ListWndColInfo  // CListWnd::SetItemText strides by 7 * 4 bytes for each row.
		{
			DWORD Unknown0x00;
			int ColCount;  // Used in SetItemText as the number of columns.
			DWORD Unknown0x08[5];  // Total size of 0x1c.
		};
		struct BasicWnd  // Equivalent to CXWnd in eqmac.exe.
		{
			//BasicWnd() {};
			void Deconstruct(bool delete_me)
			{
				reinterpret_cast<void(__thiscall*)(const BasicWnd*, bool delete_me)>(vtbl->Deconstructor)(this, delete_me);
			}
			void SetFocus()
			{
				reinterpret_cast<void(__thiscall*)(const BasicWnd*)>(0x572290)(this);
			}
			
			void BringToFront()
			{
				reinterpret_cast<void(__thiscall*)(const BasicWnd*, int)>(0x573a80)(this, 1);
			}

			// The client BasicWnd constructor (CXWnd::CXWnd()) sets the vtbl to point at a default
			// BaseVTable located at 0x005eaa94. The constructors of other BasicWnd derived classes
			// can have different default tables (contents and a few extra entries), so make a new 
			// copy of the initial table to allow modifications.
			void SetupCustomVTable(size_t size = sizeof(BaseVTable))
			{
				BYTE* newtbl = new BYTE[size];
				mem::copy((int)newtbl, (int)vtbl, size);
				mem::unprotect_memory(newtbl, size);
				vtbl = reinterpret_cast<BaseVTable*>(newtbl);
			}
			void DeleteCustomVTable(BaseVTable* original = nullptr) 
			{
				delete[] reinterpret_cast<BYTE*>(vtbl);
				vtbl = original ? original : reinterpret_cast<BaseVTable*>(0x005eaa94);  // ~CXWnd().
			}
			void show(int make_visible_flag, bool bring_to_top)
			{
				reinterpret_cast<void(__thiscall*)(const BasicWnd*, int, bool)>(0x572310)(this, make_visible_flag, bring_to_top);
			}
			BasicWnd* GetChildItem(CXSTR name, bool log_error = true)
			{
				if (!log_error)
					mem::write<BYTE>(0x570378, 0xEB); //jump passed the ui error logging
				BasicWnd* wnd =  reinterpret_cast<BasicWnd*(__thiscall*)(const BasicWnd*, CXSTR)>(0x570320)(this, name);
				if (!log_error)
					mem::write<BYTE>(0x570378, 0x75); //restore the ui error logging
				return wnd;
			}
			void CreateChildren()
			{
				reinterpret_cast<BasicWnd*(__thiscall*)(const BasicWnd*)>(0x56f4f0)(this);
			}
			int WndNotification(int a1, int a2, int a3)
			{
				return reinterpret_cast<int(__thiscall*)(const BasicWnd*, int,int,int)>(vtbl->WndNotification)(this, a1, a2, a3);
			}
			void MinimizeToggle()
			{
				reinterpret_cast<void(__thiscall*)(const BasicWnd*)>(vtbl->OnMinimizeBox)(this);
			}
			CXRect GetScreenRect()
			{
				return reinterpret_cast<CXRect(__thiscall*)(const BasicWnd*)>(0x005751C0)(this);
			}
			void DrawTooltipAtPoint(int left, int top)
			{
				reinterpret_cast<void(__thiscall*)(const BasicWnd*, int, int)>(0x00574800)(this, left, top);
			}
			void LeftClickDown(int mouse_x, int mouse_y, unsigned int flags = 0)
			{
				reinterpret_cast<int(__thiscall*)(const BasicWnd*, int x, int y, uint32_t)>(vtbl->HandleLButtonDown)(this, mouse_x, mouse_y, flags);
			}
			void LeftClickUp(int mouse_x, int mouse_y, unsigned int flags = 0)
			{
				reinterpret_cast<int(__thiscall*)(const BasicWnd*, int x, int y, unsigned)>(vtbl->HandleLButtonUp)(this, mouse_x, mouse_y, flags);
			}

			/* 0x0000 */ BaseVTable* vtbl;
			/* 0x0004 */ DWORD MouseHoverTimer;
			/* 0x0008 */ DWORD FadeDelay;
			/* 0x000C */ DWORD FadeDuration;
			/* 0x0010 */ BYTE FadedAlpha;  // Alpha transparency value when faded
			/* 0x0011 */ BYTE IsNotFaded;  // Set to 0 when faded, 1 when not faded
			/* 0x0012 */ BYTE IsLocked;
			/* 0x0013 */ BYTE LockEnable;  // Enable Lock option in CContextMenuManager::WarnDefaultMenu.
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
			/* 0x0088 */ BYTE Unknown0088[20];
			/* 0x009C */ BYTE UnfadedAlpha;  // Alpha transparency value when active
			/* 0x009D */ BYTE Unknown009d;
			/* 0x009E */ BYTE AlphaTransparency;  // Current Alpha transparency
			/* 0x009F */ BYTE Unknown009F;
			/* 0x00A0 */ BYTE ZLayer;
			/* 0x00A1 */ BYTE Unknown00A1[7];
			/* 0x00A8 */ DWORD DrawTemplate;
			/*0x0ac*/   BYTE    Unknown0x0ac[0x4];
			/*0x0b0*/   DWORD   ZLayer2;
			/*0x0b4*/   BYTE   Unknown0x0b4[0x28];
			/*0x0dc*/   DWORD   FadeTickCount;
			/*0x0e0*/   BYTE    Unknown0x0e0; /* CXWnd::StartFade */
			/*0x0e1*/   BYTE    Unknown0x0e1; /* CXWnd::StartFade */
			/*0x0e2*/   BYTE    Unknown0x0e2;
			/*0x0e3*/   BYTE    Unknown0x0e3;
			/*0x0e4*/   DWORD   Unknown0x0e4; /* CXWnd::StartFade, CXWnd::Minimize */
			/*0x0e8*/   DWORD   VScrollMax;
			/*0x0ec*/   DWORD   VScrollPos;
			/*0x0f0*/   DWORD   HScrollMax;
			/*0x0f4*/   DWORD   HScrollPos;
			/*0x0f8*/   BYTE    ValidCXWnd;
			/*0x0f9*/   BYTE    Unused0x0f9[0x3];
			/*0x0fc*/   union {
				struct ListWndColInfo* ColInfoArray;  // Row-length array of info structures.
				struct pCXSTR* SidlText;
				DWORD Items;
				struct ListWnd* CmbListWnd;
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
			~EQWND() {};
			EQWND() {};

			// The client EqWnd constructor (CSidlScreenWnd()) sets the vtbl to point at a default
			// SidlScreenWndVTable located at 0x005ea98c. The constructors of other derived classes
			// may have different default tables, so make a new specific copy to allow modification.
			// Note that if the derived class has an extended vtable size, use the BasicWnd versions
			// of these that support different sizes.
			void SetupCustomVTable()
			{
				BasicWnd::SetupCustomVTable(sizeof(SidlScreenWndVTable));
			}

			// This should only be called immediately before destruction of custom EQWND classes
			// created with SetupCustomVTable().
			void DeleteCustomVTable()
			{
				auto* sidl_default_vtbl = reinterpret_cast<SidlScreenWndVTable*>(0x005ea98c);
				BasicWnd::DeleteCustomVTable(sidl_default_vtbl);
			}
		
			/*0x114*/   DWORD   Selector;
			/*0x118*/   DWORD   PushToSelector;
			union
			{
				/*0x11c*/   DWORD   EnableINIStorage;
				/*0x11c*/   int   SelectedIndex;
			};
			/*0x120*/   union {

				CXSTR INIStorageName;
				struct _EQINVSLOT* pEQInvSlot;
			};
			/*0x124*/   struct CTextureAnimation*  BackgroundTexture;
			/*0x128*/   DWORD   Unknown0x128; /* CTextureAnimation */
			/*0x12c*/   DWORD   ContextMenu;  /* CTextureAnimation its an id for the menu*/
			/*0x130*/	BYTE    Unknown0x130; /* CTextureAnimation */
			/*0x131*/	BYTE    Unknown0x131; /* CTextureAnimation */
			/*0x132*/	BYTE    Unknown0x132; /* CTextureAnimation */
			/*0x133*/	BYTE    Unknown0x133; /* CTextureAnimation */
		};
		struct BuffWindowButton : EQWND
		{
			/* 0x0134 */ int Unknown1;
			/* 0x0138 */ struct CTextureAnimation* BuffIcon; // Background stored in 0x124
		};
		struct BuffWindow : EQWND
		{
			/* 0x0134 */ BYTE Unknown00134; // Initialization flag
			/* 0x0135 */ BYTE Unknown00135; // Initialization flag
			/* 0x0136 */ BYTE Unknown00136;
			/* 0x0137 */ BYTE Unknown00137;
			/* 0x0138 */ struct CTextureAnimation* BlueIconBackground;
			/* 0x013C */ struct CTextureAnimation* RedIconBackground;
			/* 0x0140 */ struct CTextureAnimation* CTextureAnimations[EQ_NUM_BUFFS];
			/* 0x017C */ BuffWindowButton* BuffButtonWnd[EQ_NUM_BUFFS];
			/* 0x01B8 */ DWORD NextRefreshTime;
			/* 0x01BC */ DWORD Width;
			/* 0x01C0 */ DWORD Height;
			/* End    */
		};
		struct CharSelect : EQWND
		{
			/*0x134*/	BYTE    Activated;   // Set to 1 when activated and 0 in Deactivate().
			/*0x135*/	BYTE    Unknown0x135[0x3B];
			/*0x170*/   BYTE    Rotate;
			/*0x171*/   BYTE    Explore;
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

		struct ComboWnd : BasicWnd  // Uses a BaseVTable.
		{
			void DeleteAll()
			{
				reinterpret_cast<int(__thiscall*)(const ComboWnd*)>(0x5a18e0)(this);
			}
			void InsertChoice(CXSTR data)
			{
				reinterpret_cast<void(__thiscall*)(const ComboWnd*, CXSTR)>(0x5A1750)(this, data);
			}
			void SetChoice(int index)
			{
				reinterpret_cast<void(__thiscall*)(const ComboWnd*, int)>(0x5A1860)(this, index);
			}
		};
		struct ListWnd : BasicWnd  // Note ListWnd has same vtable structure as ContextMenu, so this declaration is incomplete.
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
			/* 0x0114 */  BYTE Unknown0114[0x08];
			/* 0x011c */  int  SelectedIndex;
			// Incomplete list.
		};

		struct ItemDisplayWnd : EQWND
		{
		private:
			ItemDisplayWnd() {};  // Either use Create() or assign a pointer to an existing class. 

		public:
			static constexpr uint32_t kDefaultVTableAddr = 0x005e5a98;

			// Inspired by ui_manager::create_sidl to use the same heap for destructor deletion support.
			static ItemDisplayWnd* Create(BasicWnd* parent_wnd = nullptr) {
				ItemDisplayWnd* wnd = reinterpret_cast<ItemDisplayWnd*>(HeapAlloc(*Zeal::EqUI::Heap, 0, sizeof(ItemDisplayWnd)));
				mem::set((int)wnd, 0, sizeof(ItemDisplayWnd));
				reinterpret_cast<void(__fastcall*)(ItemDisplayWnd*, int, BasicWnd*)>(0x00423331)(wnd, 0, parent_wnd);
				return wnd;
			}

			void Destroy()
			{
				reinterpret_cast<void(__thiscall*)(const ItemDisplayWnd*, bool)>(this->vtbl->Deconstructor)(this, true);
			}

			// The client ItemDisplayWnd constructor (CItemDisplayWnd::CItemDisplayWnd()) sets the vtbl to point at a
			// default ItemDisplayVTable located at 0x005e5a98. Copy that table over in order to modify it.
			void SetupCustomVTable()
			{
				BasicWnd::SetupCustomVTable(sizeof(ItemDisplayVTable));
			}
			// This should only be called immediately before destruction of custom ItemDisplayWnd classes
			// created with SetupCustomVTable().
			void DeleteCustomVTable()
			{ 
				auto* item_display_default_vtbl = reinterpret_cast<ItemDisplayVTable*>(kDefaultVTableAddr);
				BasicWnd::DeleteCustomVTable(item_display_default_vtbl);
			}
			void Activate()
			{
				auto* vtable = static_cast<ItemDisplayVTable*>(vtbl);
				reinterpret_cast<void(__thiscall*)(const ItemDisplayWnd*)>(vtable->Activate)(this);
			}
			void Deactivate()
			{
				reinterpret_cast<void(__thiscall*)(const ItemDisplayWnd*)>(vtbl->Deactivate)(this);
			}

			/* 0x0134 */ struct EQWND* ItemDescription; // the item stats text window
			/* 0x0138 */ BYTE Unknown0138[4];
			/* 0x013C */ struct EQWND* IconBtn; // the item icon window
			/* 0x0140 */ BYTE IsActivated;  // 1 if activated, 0 if deactivated
			/* 0x0141 */ BYTE Unknown0140[3];
			/* 0x0144 */ CXSTR DisplayText; // the item name is the title text
			/* 0x0148 */ CXSTR WindowTitle; // the item stats text
			/* 0x014C */ Zeal::EqStructures::EQITEMINFOBASE Item;  // Copied into in SetItem() if flag set.
			/* 0x0230 */ BYTE ItemValid;  // Set to 1 if Item was populated in SetItem() else 0.
			/* 0x0231 */ BYTE unknown0231[11];  // Operator new has a total size of 0x23c bytes.
		};
		struct InvSlotWnd: BasicWnd  // Operator new of 0x12C bytes.
		{
			static constexpr BaseVTable* default_vtable = reinterpret_cast<BaseVTable*>(0x005eb774);
			int HandleLButtonUp(int mouse_x, int mouse_y, unsigned int flags)
			{
				return reinterpret_cast<int(__thiscall*)(InvSlotWnd*, int, int, unsigned int)>(0x005a7b10)(this, mouse_x, mouse_y, flags);
			}
			int HandleRButtonUp(int mouse_x, int mouse_y, unsigned int flags)
			{
				return reinterpret_cast<int(__thiscall*)(InvSlotWnd*, int, int, unsigned int)>(0x005a7e80)(this, mouse_x, mouse_y, flags);
			}
			/* 0x0114 */ BYTE Unknown0114;
			/* 0x0115 */ BYTE Unknown0015; // Passed in as param_3 to InvSlot::HandleLButtonUp().
			/* 0x0116 */ BYTE Unknown0116;
			/* 0x0117 */ BYTE Unknown0117;
			/* 0x0118 */ struct InvSlot* invSlot;
			/* 0x011C */ DWORD Unknown011c;
			/* 0x0120 */ DWORD IsActive;  // Set to 1 when mouse has triggered an interaction, 0 when released.
			/* 0x0124 */ DWORD Unknown0124;
			/* 0x0128 */ DWORD Unknown0128;
		};
		struct InvSlot  // Operator new of 0x14 bytes.
		{
			int HandleLButtonUp() {
				int flag = this->invSlotWnd ? this->invSlotWnd->Unknown0015 : 0;
				return reinterpret_cast<int(__thiscall*)(InvSlot*, int unused_x, int unused_y, BYTE flag)>(0x421E48)(this, 0, 0, flag);
			}
			/* 0x0000 */ void* destructor;  // Pointer to pointer to CInvSlot::~CInvSlot.
			/* 0x0004 */ InvSlotWnd* invSlotWnd;  // Points back to "parent".
			/* 0x0008 */ void* textureAnimation;  // Also copied to invSlotWnd->TextureAnimation
			/* 0x000C */ DWORD Index;             // Index to CInvSlotMgr->invSlots[i]. This changes as containers are opened/closed.
			/* 0x0010 */ Zeal::EqStructures::EQITEMINFOBASE* Item;
		};
		struct MerchantWnd : EQWND  // operator_new(0x3e4)
		{
			/*0x134*/  BYTE   Activated;         // 1 = activated, 0 = deactivated
			/*0x135*/  BYTE   Unknown0x135[0xb];
			/*0x140*/  DWORD  Unknown0x140[0x50];  // Likely ItemInfo*. Deleted when deactivated.
			/*0x280*/  DWORD  Unknown0x280;
			/*0x284*/  DWORD  InventoryItemSlot;
			/*0x288*/  Zeal::EqStructures::EQITEMINFO** ItemInfo;  // Item used in selling.
			/*0x28C*/  EQWND* MerchantName;      // MW_MerchantName
			/*0x290*/  EQWND* SelectedItem;      // MW_SelectedItem
			/*0x294*/  EQWND* BuyButton;         // MW_Buy_Button
			/*0x298*/  EQWND* SellButton;        // MW_Sell_Button
			/*0x29c*/  EQWND* InvSlotWnd[0x50];  // Unsure if InvSlot vs InvSlotWnd
			/*0x3dc*/  EQWND* DoneButton;
			/*0x3e0*/  EQWND* MerchantSlotsWnd;  // MerchantSlotsWnd
		};
		struct SliderWnd : BasicWnd // Uses a BaseVTable.
		{
			SliderWnd() {};
			/* 0x0114 */ BYTE Unknown0114[0x20];
			/* 0x0134 */ int current_val;
			/* 0x0138 */ BYTE Unknown0138[4];
			/* 0x013C */ int max_val; 
			/* 0x0140 */ int val3;
			/* 0x0144 */ BYTE Unknown0144[0xC]; // the item name is the title text. Operator new has a total size of 0x150 bytes.
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

		struct EditWnd : public BasicWnd  // Note: this definition has a truncated vtbl.
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
		// onetimehero 09-17-03
		// CContainerWnd
		// Actual Size 0x17C in eqmac
		class  ContainerWnd : public EQWND {
		public:
			/*0x134*/ DWORD   something;     // dont know maybe type or a counter/ID?;
			/*0x138*/ Zeal::EqStructures::EQITEMINFO* pContainerInfo;  // Type, capacity, etc.
			/*0x13c*/ InvSlotWnd* pSlotWnds[0x0a];
			/*0x164*/ EQWND* pCombine;
			/*0x168*/ EQWND* pDone;
			/*0x16c*/ EQWND* pIcon;
			/*0x170*/ EQWND* A_DragItem;
			/*0x174*/ EQWND* pLabel;
			/*0x178*/ BYTE Unknown0x178[4];
			/*0x17c*/
		};

		// Actual Size 0x54 in eqgame.
		class ContainerMgr {
		public:
			/*0x000*/   DWORD pvfTable; // NOT based on CXWnd.  Contains only destructor
			/*0x004*/   ContainerWnd* pPCContainers[0x11];  // All open containers, including World, in order of opening...
			/*0x048**/  DWORD*   pWorldItems;            // Pointer to the contents of the world   If NULL, world container isn't open;
			/*0x04c*/   DWORD Unknown0x04c;            // in the future this is ID of container in zone, starts at one (zero?) and goes up.
			/*0x050*/   DWORD dwTimeSpentWithWorldContainerOpen;  // Cumulative counter?
			/*0x054*/
		};

		struct CInvSlotMgr {
		public:
			InvSlot* FindInvSlot(int slot_id) {
				return reinterpret_cast<InvSlot*(__thiscall*)(CInvSlotMgr*, int)>(0x423010)(this, slot_id);
			}

			/* 0x000 */ DWORD* Unknonwn0000;
			/* 0x004 */ InvSlot* InvSlots[450];
			/* 0x70C */ DWORD NumInvSlots;
		};

		struct ColorPickerWnd : public EQWND
		{
			void Activate(BasicWnd* wnd, DWORD color)
			{
				reinterpret_cast<void(__thiscall*)(const ColorPickerWnd*, BasicWnd*, DWORD)>(0x414F2A)(this, wnd, color);
			};
			void SetCurrentcolor(DWORD color)
			{
				reinterpret_cast<void(__thiscall*)(const ColorPickerWnd*, DWORD)>(0x414F87)(this, color);
			};
		};
		struct EQKey
		{
			/* 0x0000 */ UINT32 code;
			/* 0x0004 */ int timestamp;
			/* 0x0008 */ int unk;
			/* 0x000C */ int isDown;
			/* 0x0010 */ int whoknows;
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
			/* 0x0034 */ DWORD Unknown34;
			/* 0x0038 */ DWORD Unknown38;
			/* 0x003C */ EQWND* Hovered;
			/* 0x0040 */ DWORD Unknown40[3];
			/* 0x004C */ int AbsMouseX;
			/* 0x0050 */ int AbsMouseY;
			/* 0x0054 */ BYTE CapsLockState;
			/* 0x0055 */ BYTE ShiftKeyState;    // GetKeyboardFlags() bit 0x01
			/* 0x0056 */ BYTE ControlKeyState;  // GetKeyboardFlags() bit 0x02
			/* 0x0057 */ BYTE AltKeyState;      // GetKeyboardFlags() bit 0x04
			/* 0x0058 */ BYTE unknown58;        // GetKeyboardFlags() bit 0x08
			/* 0x0059 */ BYTE unknown59[3];
			/* 0x005C */ DWORD unknown5C;   // Accessed in ShowCursor
			/* 0x0060 */ DWORD unknown60[0x50/4];   // Accessed in ShowCursor
			/* 0x00B0 */ DWORD unknownB0;   // Accessed in ShowCursor
			/* 0x00B4 */ DWORD unknownB4[2];
			/* 0x00BC */ DWORD unknownBC;
			/* 0x00C0 */ DWORD unknownC0;
			/* 0x00C4 */ DWORD unknownC4;  // Pointer.
			/* 0x00C8 */ DWORD unknownC8;


		};

		class ContextMenu
		{
		private:
			ContextMenu() {}  // Either use Create() or assign a pointer to an existing class.

			static void __fastcall CustomDestructor(void* menu, int unusedEDX, bool delete_me)
			{
				BasicWnd* wnd = reinterpret_cast<BasicWnd*>(menu);  // ContextMenu inherits from BasicWnd.
				delete reinterpret_cast<ContextMenuVTable*>(wnd->vtbl);
				wnd->vtbl = reinterpret_cast<BaseVTable*>(0x005e4a24);  // Set back to default ContextMenu::vtable.
				reinterpret_cast<void(__thiscall*)(const BasicWnd*, bool)>(wnd->vtbl->Deconstructor)(wnd, delete_me);
			};

		public:
			// Inspired by ui_manager::create_sidl to use the same heap for destructor deletion support.
			// Useful for classes that require custom vtables but not custom destructors in that table.
			static ContextMenu* Create(int cxwnd, int a1, CXRect r) {
				ContextMenu* menu = reinterpret_cast<ContextMenu*>(HeapAlloc(*Zeal::EqUI::Heap, 0, sizeof(ContextMenu)));
				if (!menu)
					return nullptr;
				mem::set((int)menu, 0, sizeof(ContextMenu));
				reinterpret_cast<void(__fastcall*)(ContextMenu*, int, int, int, CXRect)>(0x417785)(menu, 0, cxwnd, a1, r);
				ContextMenuVTable* newtbl = new ContextMenuVTable();
				if (!newtbl)
					return nullptr;
				mem::copy((int)newtbl, (int)menu->fnTable, sizeof(ContextMenuVTable));
				mem::unprotect_memory(newtbl, sizeof(ItemDisplayVTable));
				menu->fnTable = newtbl;
				menu->fnTable->Deconstructor = CustomDestructor;
				return menu;
			}
			void Destroy()
			{
				reinterpret_cast<void(__thiscall*)(const ContextMenu*, bool)>(this->fnTable->Deconstructor)(this, true);
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
			int AddMenuItem(CXSTR data, int index, bool disp_activated=false, bool has_children = false) const {
				return reinterpret_cast<int(__thiscall*)(const ContextMenu*, CXSTR, int, bool)>(0x417910)(this, data, has_children ? index | 0x80000000 : index, disp_activated);
			}
			void SetItemColor(int index, ARGBCOLOR color)
			{
				reinterpret_cast<void(__thiscall*)(const ContextMenu*, int, int, ARGBCOLOR)>(0x579eb0)(this, index, 0x1, color);
				reinterpret_cast<void(__thiscall*)(const ContextMenu*, int, int, ARGBCOLOR)>(0x579eb0)(this, index, 0x2, color);
			}
			void RemoveAllMenuItems()
			{
				reinterpret_cast<void(__thiscall*)(const ContextMenu*)>(0x417a7f)(this);
			}
			void CheckMenuItem(int row, bool check, bool uncheck_other_rows = false)
			{
				reinterpret_cast<void(__thiscall*)(const ContextMenu*, int, bool, bool)>(0x417ae8)(this, row, check, uncheck_other_rows);
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

		class CChatManager// : public EQWND
		{
		public: //this class is a complete hack lol
			struct ChatWnd* GetActiveChatWindow() const {
				return reinterpret_cast<struct ChatWnd* (__thiscall*)(const CChatManager*)>(0x41114A)(this);
			}
			void CreateChatWindow() const {
				reinterpret_cast<void (__thiscall*)(const CChatManager*)>(0x410C5A)(this);
			}
			void FreeChatWindow(struct ChatWnd* wnd) const {
				reinterpret_cast<void(__thiscall*)(const CChatManager*, struct ChatWnd*)>(0x41110C)(this, wnd);
			}
			void CreateChatWindow(const char* name, int language , int default_channel, int chat_channel, const char* tell_target, int font) const {
				reinterpret_cast<void(__thiscall*)(const CChatManager*, const char* name, int language, int default_channel, int chat_channel, const char* tell_target, int font)>(0x410e84)(this, name,language,default_channel,chat_channel, tell_target,font);
			}
			/*0x000*/	ChatWnd* ChatWindows[32];
			/*0x080*/	DWORD MaxChatWindows;
			/*0x084*/	int ActiveChatWnd;
			/*0x088*/	int AlwaysChatHereIndex;
			/*0x08C*/   int unknown;
			/*0x090*/   ChatWnd* ChannelMapWnd[0x29];
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
			static constexpr SidlScreenWndVTable* default_vtable = reinterpret_cast<SidlScreenWndVTable*>(0x005e6e48);

			void BeginMemorize(int book_index, int gem_index, bool unsure) const
			{
				reinterpret_cast<void(__thiscall*)(const EQWND*, int, int, bool)>(0x434a05)(this, book_index, gem_index, unsure);
			}
			void OpenBook() const
			{
				reinterpret_cast<void(__thiscall*)(const EQWND*)>(0x43441F)(this);
			}
			void StopSpellBookAction() const  // Aborts memorization or scribing.
			{
				reinterpret_cast<void(__thiscall*)(const EQWND*)>(0x00435531)(this);
			}
			int WndNotification(const BasicWnd* src_wnd, int param_2, void* param_3)
			{
				return reinterpret_cast<int(__thiscall*)(SpellBookWnd*, const BasicWnd*, int, void*)>
					(0x004345cb)(this, src_wnd, param_2, param_3);
			}
			void DisplaySpellInfo(const BasicWnd* src_wnd)
			{
				reinterpret_cast<int(__thiscall*)
					(SpellBookWnd*, const BasicWnd*)>(0x00435234)(this, src_wnd);
			}

		};

		class SpellGemsWnd : public EQWND  // Aka CastSpellWnd in eqmac
		{
		public:
			static constexpr SidlScreenWndVTable* default_vtable = reinterpret_cast<SidlScreenWndVTable*>(0x005e41ac);

			void Forget(int index) const
			{
				reinterpret_cast<void(__thiscall*)(const SpellGemsWnd*, int)>(0x40a662)(this, index);
			}
			void UpdateSpellGems(int index) const
			{
				reinterpret_cast<void(__thiscall*)(const SpellGemsWnd*, int)>(0x40a8b7)(this, index);
			}
			int WndNotification(const BasicWnd* src_wnd, int param_2, void* param_3)
			{
				return reinterpret_cast<int(__thiscall*)(SpellGemsWnd*, const BasicWnd*, int, void*)>
														(0x0040a32a)(this, src_wnd, param_2, param_3);
			}
			void HandleSpellInfoDisplay(const BasicWnd* src_wnd)
			{
				reinterpret_cast<int(__thiscall*)
										(SpellGemsWnd*, const BasicWnd*)>(0x0040a480)(this, src_wnd);
			}
			/*0x134*/ BYTE Unknown0x134[0x08];
			/*0x13C*/ SpellGem* SpellSlots[0x8];
			/*0x15C*/ EQWND* SpellBook;
		};

		struct pInstWindows
		{
			CContextMenuManager* ContextMenuManager;  // 0x63D5CC
			CChatManager* ChatManager;  // 0x63D5D0
			EQWND* uknownWnd1;  // 0x63D5D4
			CharSelect* CharacterSelect;  // 0x63D5D8
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
			ColorPickerWnd* ColorPicker;  // 0x63D62C
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
			CInvSlotMgr* InvSlotMgr;  // 0x63D6B4
			ContainerMgr* ContainerMgr;  // 0x63D6B8
		};



	}
}
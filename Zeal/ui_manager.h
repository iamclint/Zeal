#pragma once
#include "ui_options.h"
#include "ui_bank.h"
#include "ui_loot.h"
#include "ui_guild.h"
#include "ui_raid.h"
#include "ui_hotbutton.h"
#include "ui_group.h"
#include "ui_inputdialog.h"
#include "hook_wrapper.h"
#include "memory.h"
#include "EqUI.h"
class UIElement
{
protected:
	std::string elementType; // Protected so derived classes can access it directly.
public:
	struct Location {
		int x=0;
		int y=0;
	};

	struct Size {
		int x=0;
		int y=0;
	};
	struct Color {
		int r=255;
		int g=255;
		int b=255;
	};
	std::string item;
	std::string screenID;
	Location location;
	Size size;
	bool relativePosition=true;
	Color textcolor;
	bool styleVScroll=false;
	bool styleHScroll=false;
	bool styleTransparent=false;
	bool autoStretch=false;
	bool topAnchorToTop=false;
	bool bottomAnchorToTop = false;
	bool LeftAnchorToLeft = false;
	bool RightAnchorToLeft = false;
	bool noWrap = false;
	bool alignCenter = false;
	bool alignRight = false;
	std::string text;
	std::string toolTip;
	std::string toXML()
	{
		std::ostringstream oss;
		oss << "<" << elementType << " item=\"" << item << "\">\n";
		oss << "  <ScreenID>" << screenID << "</ScreenID>\n";
		oss << "  <Location>\n";
		oss << "    <X>" << location.x << "</X>\n";
		oss << "    <Y>" << location.y << "</Y>\n";
		oss << "  </Location>\n";
		oss << "  <Size>\n";
		oss << "    <CX>" << size.x << "</CX>\n";
		oss << "    <CY>" << size.y << "</CY>\n";
		oss << "  </Size>\n";
		oss << "  <TextColor>\n";
		oss << "    <R>" << textcolor.r << "</R>\n";
		oss << "    <G>" << textcolor.g << "</G>\n";
		oss << "    <B>" << textcolor.b << "</B>\n";
		oss << "  </TextColor>\n";
		oss << "  <Text>" << text << "</Text>\n";
		oss << "  <Style_VScroll>" << (styleVScroll ? "true" : "false") << "</Style_VScroll>\n";
		oss << "  <Style_HScroll>" << (styleHScroll ? "true" : "false") << "</Style_HScroll>\n";
		oss << "  <Style_Transparent>" << (styleTransparent ? "true" : "false") << "</Style_Transparent>\n";
		oss << "  <TooltipReference>" << toolTip << "</TooltipReference>\n";
		oss << "  <NoWrap>" << (noWrap ? "true" : "false") <<  "</NoWrap>\n";
		oss << "  <AlignCenter>" << (alignCenter ? "true" : "false") << "</AlignCenter>\n";
		oss << "  <AlignRight>" << (alignRight ? "true" : "false") << "</AlignRight>\n";
		oss << ElementSpecificXML() << "\n";
		oss << "</" << elementType << ">\n";
		return oss.str();
	}

	virtual std::string ElementSpecificXML() const {
		std::ostringstream oss;
		oss << "<UIElement item=\"" << item << "\">\n";
		oss << "  <ScreenID>" << screenID << "</ScreenID>\n";
		oss << "</UIElement>\n";
		return oss.str();
	}
	UIElement(const std::string& type, const std::string& id) : elementType(type), item(id), screenID(id) {}

};

class UICombobox : public UIElement {
public:
	std::string drawTemplate;
	int listHeight;
	bool styleBorder;
	std::vector<std::string> choices;
	UICombobox(const std::string& id) : UIElement("Combobox", id) {}
	std::string ElementSpecificXML() const override {
		std::ostringstream oss;
		oss << "  <DrawTemplate>" << drawTemplate << "</DrawTemplate>\n";
		oss << "  <ListHeight>" << listHeight << "</ListHeight>\n";
		oss << "  <Button>BDT_Combo</Button>\n";
		oss << "  <Style_Border>" << (styleBorder ? "true" : "false") << "</Style_Border>\n";
		for (const auto& choice : choices)
			oss << "  <Choices>" << choice << "</Choices>\n";
		return oss.str();
	}
};
class UILabel : public UIElement
{
	UILabel(const std::string& id) : UIElement("Label", id) {}
	//the most basic element there is
	std::string ElementSpecificXML() const override {
		return "";
	}
};
class UIButton : public UIElement {
public:

	UIButton(const std::string& id) : UIElement("Button", id) {}
	struct ButtonDrawTemplate {
		std::string normal = "A_BtnNormal";
		std::string pressed = "A_BtnPressed";
		std::string flyby = "A_BtnFlyby";
		std::string disabled = "A_BtnDisabled";
		std::string pressedFlyby = "A_BtnPressedFlyby";
	};

	bool styleCheckbox;
	
	ButtonDrawTemplate buttonDrawTemplate;

	// Method to generate XML from class members
	std::string ElementSpecificXML() const override {
		std::ostringstream oss;

		oss << "  <Style_Checkbox>" << (styleCheckbox ? "true" : "false") << "</Style_Checkbox>\n";
		oss << "  <ButtonDrawTemplate>\n";
		oss << "    <Normal>" << buttonDrawTemplate.normal << "</Normal>\n";
		oss << "    <Pressed>" << buttonDrawTemplate.pressed << "</Pressed>\n";
		oss << "    <Flyby>" << buttonDrawTemplate.flyby << "</Flyby>\n";
		oss << "    <Disabled>" << buttonDrawTemplate.disabled << "</Disabled>\n";
		oss << "    <PressedFlyby>" << buttonDrawTemplate.pressedFlyby << "</PressedFlyby>\n";
		oss << "  </ButtonDrawTemplate>";
		return oss.str();
	}
};
class UIScreen : public UIElement
{
public:
	std::vector<UIElement*> Elements; //screens can contain tabboxes or any other element as pieces
	UIScreen(const std::string& id) : UIElement("Screen", id) {}
	std::string drawTemplate = "WDT_Def";
	bool titlebar;
	bool closebox;
	bool minimizebox;
	bool border;
	bool sizable;
	std::string ElementSpecificXML() const override
	{

		std::ostringstream oss;
		oss << "  <DrawTemplate>" << drawTemplate << "</DrawTemplate>\n";
		oss << "  <Style_Titlebar>" << (titlebar ? "true" : "false") << "</Style_Titlebar>\n";
		oss << "  <Style_Closebox>" << (closebox ? "true" : "false") << "</Style_Closebox>\n";
		oss << "  <Style_Minimizebox>" << (minimizebox ? "true" : "false") << "</Style_Minimizebox>\n";
		oss << "  <Style_Border>" << (border ? "true" : "false") << "</Style_Border>\n";
		oss << "  <Style_Sizable>" << (sizable ? "true" : "false") << "</Style_Sizable>\n";

		for (auto& ele : Elements)
		{
			oss << "  <Pieces>" << ele->screenID << "</Pieces>\n";
		}
		return oss.str();
	};
};
class UIPage : public UIElement
{
public:
	UIPage(const std::string& id) : UIElement("Page", id) {}
	std::vector<UIElement*> Elements; //screens can contain tabboxes or any other element as pieces
	std::string ElementSpecificXML() const override
	{
		std::ostringstream oss;
		for (auto& ele : Elements)
		{
			oss << "  <Pieces>" << ele->screenID << "</Peices>\n";
		}
		return oss.str();
	};
};
class TabBox : public UIElement
{
public:
	TabBox(const std::string& id) : UIElement("Page", id) {}
	std::vector<UIElement*> Elements; //can contain pages
	std::string tabBorderTemplate = "FT_DefTabBorder";
	std::string pageBorderTemplate = "FT_DefPageBorder";
	std::string ElementSpecificXML() const override
	{
		std::ostringstream oss;
		for (auto& ele : Elements)
		{
			oss << "  <Pages>" << ele->screenID << "</Pages>\n";
		}
		return oss.str();
	};
};
class UIContainer
{
public:
	std::vector<UIElement*> Elements; //this should contain screens
	std::string toXML()
	{
		std::ostringstream oss;
		oss << "<?xml version=\"1.0\" encoding=\"us-ascii\"?>\n";
		oss << "<XML ID=\"EQInterfaceDefinitionLanguage\">\n";
		oss << "<Schema xmlns=\"EverQuestData\" xmlns:dt=\"EverQuestDataTypes\" />\n";
		for (auto& ele : Elements)
		{
			oss << ele->toXML();
		}
		oss << "</XML>";
		return oss.str();
	};
};

class ui_manager
{
public:
	static constexpr const char* ui_path = "uifiles\\zeal\\";
	std::string GetUIIni();

	Zeal::EqUI::SliderWnd* GetSlider(std::string name);
	Zeal::EqUI::BasicWnd* GetCheckbox(std::string name);
	Zeal::EqUI::BasicWnd* GetButton(std::string name);
	Zeal::EqUI::BasicWnd* GetCombo(std::string name);
	std::function<void(Zeal::EqUI::SliderWnd*, int)> GetSliderCallback(Zeal::EqUI::SliderWnd* wnd);
	std::function<void(Zeal::EqUI::BasicWnd*, int)> GetComboCallback(Zeal::EqUI::BasicWnd* wnd);
	std::function<void(Zeal::EqUI::BasicWnd*)> GetButtonCallback(Zeal::EqUI::BasicWnd* wnd);
	std::function<void(Zeal::EqUI::BasicWnd*)> GetCheckboxCallback(Zeal::EqUI::BasicWnd* wnd);

	Zeal::EqUI::BasicWnd* clicked_button = nullptr;
	std::unordered_map<Zeal::EqUI::BasicWnd*, std::unordered_map<std::string, Zeal::EqUI::BasicWnd*>> WindowChildren;
	Zeal::EqUI::BasicWnd* GetChild(Zeal::EqUI::BasicWnd* parent, std::string name);
	void SetLabelValue(std::string name, const char* format, ...);
	void SetSliderValue(std::string name, int value);
	void SetSliderValue(std::string name, float value);
	void SetComboValue(std::string name, int value);
	void SetChecked(std::string name, bool checked);
	Zeal::EqUI::BasicWnd* AddButtonCallback(Zeal::EqUI::BasicWnd* wnd, std::string name, std::function<void(Zeal::EqUI::BasicWnd*)> callback, bool log_errors=true);
	Zeal::EqUI::BasicWnd* AddCheckboxCallback(Zeal::EqUI::BasicWnd* wnd, std::string name, std::function<void(Zeal::EqUI::BasicWnd*)> callback, bool log_errors = true);
	Zeal::EqUI::BasicWnd* AddSliderCallback(Zeal::EqUI::BasicWnd* wnd, std::string name, std::function<void(Zeal::EqUI::SliderWnd*, int)> callback, int max_val=100, bool log_errors = true);
	Zeal::EqUI::BasicWnd* AddComboCallback(Zeal::EqUI::BasicWnd* wnd, std::string name, std::function<void(Zeal::EqUI::BasicWnd*, int)> callback, bool log_errors = true);
	void AddLabel(Zeal::EqUI::BasicWnd* wnd, std::string name, bool log_errors = true);
	void AddListItems(Zeal::EqUI::ComboWnd* wnd, const std::vector<std::string> data);
	void AddListItems(Zeal::EqUI::ListWnd* wnd, const std::vector<std::vector<std::string>> data);
	void AddListItems(Zeal::EqUI::ListWnd* wnd, const std::vector<std::string> data);
	Zeal::EqUI::EQWND* CreateSidlScreenWnd(const std::string& name);
	void WriteTemporaryUI(const std::string& file_path, std::string ui_path);
	void RemoveTemporaryUI(const std::string& file_path);
	void AddXmlInclude(const std::string& name);
	ui_manager(class ZealService* zeal, class IO_ini* ini);
	bool AlreadyLoadedXml(std::string name);
	std::shared_ptr<ui_options> options = nullptr;
	std::shared_ptr<ui_bank> bank = nullptr;
	std::shared_ptr<ui_loot> loot = nullptr;
	std::shared_ptr<ui_guild> guild = nullptr;
	std::shared_ptr<ui_raid> raid = nullptr;
	std::shared_ptr<ui_hotbutton> hotbutton = nullptr;
	std::shared_ptr<ui_group> group = nullptr;
	std::shared_ptr<ui_inputdialog> inputDialog = nullptr;
	std::vector<std::string> included_files;
	void CreateTmpXML();
	void CreateTestXML();
private:
	std::vector<std::string> XMLIncludes;
	std::unordered_map<std::string, Zeal::EqUI::BasicWnd*> checkbox_names;
	std::unordered_map<std::string, Zeal::EqUI::BasicWnd*> button_names;
	std::unordered_map<Zeal::EqUI::BasicWnd*, std::function<void(Zeal::EqUI::BasicWnd*)>> checkbox_callbacks;
	std::unordered_map<Zeal::EqUI::BasicWnd*, std::function<void(Zeal::EqUI::BasicWnd*)>> button_callbacks;
	std::unordered_map<Zeal::EqUI::SliderWnd*, std::function<void(Zeal::EqUI::SliderWnd*, int)>> slider_callbacks;
	std::unordered_map<Zeal::EqUI::BasicWnd*, std::function<void(Zeal::EqUI::BasicWnd*, int)>> combo_callbacks;
	std::unordered_map<std::string, Zeal::EqUI::SliderWnd*> slider_names;
	
	std::unordered_map<std::string, Zeal::EqUI::BasicWnd*> combo_names;
	std::unordered_map<std::string, Zeal::EqUI::BasicWnd*> label_names;
	
	void CleanUI();
};


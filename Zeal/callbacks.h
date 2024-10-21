#pragma once
#include "EqStructures.h"
#include "EqPackets.h"
#include "hook_wrapper.h"
#include "memory.h"
#include <functional>
#include <unordered_map>
#include "EqUI.h"
#include <Windows.h>
enum class callback_type
{
	MainLoop,
	Zone,
	CleanUI,
	Render,
	CharacterSelect,
	InitUI,
	EndMainLoop,
	WorldMessage,
	SendMessage_,
	ExecuteCmd,
	Delayed,
	RenderUI,
	EndScene,
	AddDeferred,
	DXReset,
	DXResetComplete,
	EntitySpawn,
	EntityDespawn,
	AddOutputText,
	ReportSuccessfulHit,
	DeactivateUI
};
class CallbackManager
{
public:
	void AddGeneric(std::function<void()> callback_function, callback_type fn = callback_type::MainLoop);
	void AddPacket(std::function<bool(UINT, char*, UINT)> callback_function, callback_type fn = callback_type::WorldMessage);
	void AddCommand(std::function<bool(UINT, BOOL)> callback_function, callback_type fn = callback_type::ExecuteCmd);
	void AddDelayed(std::function<void()> callback_function, int ms);
	void AddEntity(std::function<void(struct Zeal::EqStructures::Entity*)> callback_function, callback_type cb);
	void AddOutputText(std::function<void(struct Zeal::EqUI::ChatWnd*& wnd, std::string& msg, UINT channel)> callback_function);
	void AddReportSuccessfulHit(std::function<void(struct Zeal::EqStructures::Entity* source, struct Zeal::EqStructures::Entity* target, WORD type, short spell_id, short damage, int heal, char output_text)> callback_function);
	void invoke_ReportSuccessfulHit(struct Zeal::Packets::Damage_Struct* dmg, int heal, char output_text);
	void invoke_player(struct Zeal::EqStructures::Entity* ent, callback_type cb);
	void invoke_generic(callback_type fn);
	bool invoke_packet(callback_type fn, UINT opcode, char* buffer, UINT len);
	bool invoke_command(callback_type fn, UINT opcode, bool state);
	void invoke_outputtext(struct Zeal::EqUI::ChatWnd*& wnd, std::string& msg, UINT channel);
	void invoke_delayed();
	CallbackManager(class ZealService* zeal);
	~CallbackManager();
	void eml();
private:
	std::vector<std::pair<ULONGLONG, std::function<void()>>> delayed_functions;
	std::unordered_map<callback_type, std::vector<std::function<void()>>> generic_functions;
	std::unordered_map<callback_type, std::vector<std::function<bool(UINT, char*, UINT)>>> packet_functions;
	std::unordered_map<callback_type, std::vector<std::function<bool(UINT, BOOL)>>> cmd_functions;
	std::unordered_map<callback_type, std::vector<std::function<void(struct Zeal::EqStructures::Entity*)>>> player_spawn_functions;
	std::vector<std::function<void(struct Zeal::EqUI::ChatWnd*& wnd,std::string& msg, UINT channel)>> output_text_functions;
	std::vector<std::function<void(struct Zeal::EqStructures::Entity* source, struct Zeal::EqStructures::Entity* victim, WORD type, short spell_id, short damage, int heal, char output_text)>> ReportSuccessfulHit_functions;
};


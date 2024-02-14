#pragma once
#include "framework.h"

class ZealService
{
public:
	HMODULE	this_module = nullptr;
	std::shared_ptr<HookWrapper> hooks = nullptr;
	std::shared_ptr<CycleTarget> cycle_target_hook = nullptr;
	std::shared_ptr<MouseZoom> mouse_zoom_hook = nullptr;
	std::shared_ptr<looting> looting_hook = nullptr;
	std::shared_ptr<labels> labels_hook = nullptr;
	std::shared_ptr<Binds> binds_hook = nullptr;
	std::shared_ptr<Commands> commands_hook = nullptr;
	std::shared_ptr<MainLoop> main_loop_hook = nullptr;


	std::shared_ptr<Experience> experience = nullptr;

	bool exit = false;
	void apply_patches();
	ZealService();
	~ZealService();

	//static data/functions to get a base ptr since some hook callbacks don't have the information required
	static ZealService* ptr_service;
	static ZealService* get_instance();
};


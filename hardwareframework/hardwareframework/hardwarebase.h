#pragma once
#include "common.h"
class hardwarebaseconfig;
class hardwarebase
{
public:
	string hardware_name;
	hardwaretype hardware_type;
	hardwarevender hardware_vender;
	bool init_ok;
	string hardware_version;

	virtual bool hardware_load(hardwarebaseconfig* config) = 0;
	virtual bool hardware_unload() = 0;
};


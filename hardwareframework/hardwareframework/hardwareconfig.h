#pragma once
#include "common.h"
class hardwareconfig
{
public:
	hardwareconfig() = default;
	hardwareconfig(string _name,hardwaretype _type ,hardwarevender _vender,string _version): hardware_type(hardwaretype::none_card),
	                                                                                         hardware_vender(hardwarevender::none){}

	hardwareconfig(const hardwareconfig& config):hardware_name(config.hardware_name),
												hardware_type(config.hardware_type),
												hardware_vender(config.hardware_vender),
												hardware_version(config.hardware_version){}

	hardwareconfig& operator= (const hardwareconfig& config)
	{
		hardware_name = config.hardware_name;
		hardware_type = config.hardware_type;
		hardware_vender = config.hardware_vender;
		hardware_version = config.hardware_version;
			
		return *this;
	}
public:
	string hardware_name;
	hardwaretype hardware_type;
	hardwarevender hardware_vender;
	string hardware_version;
};


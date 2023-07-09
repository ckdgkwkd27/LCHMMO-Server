#pragma once
#include "Zone.h"

class ZoneManager
{
public:
	std::unordered_map<ZoneIDType, Zone*> ZoneMap;
};


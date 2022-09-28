#pragma once
#include "imgui.h"
#include "interface.h"
#include <string>
#include <iostream>
#include "atomiclib.h"
#include <vector>

class interfaceM
{
public:
	std::string s{ "656" };
	interfaceM();
	void gui(std::string atom1);
};


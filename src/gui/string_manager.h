#pragma once
#include "../common/stdafx.h"
#include "../globals.h"
#include <map>

class StringManager {
public:
	bool loadLanguage(TLanguage language);
	std::string getText(std::string label);

private:
	std::map<std::string, std::string> m_texts;
};
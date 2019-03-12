#pragma once
#include "common/stdafx.h"
#include "../globals.h"
#include <map>

class StringManager {
public:
	bool loadLanguage(ELanguage language);
	std::string getText(const std::string& label);

private:
	std::map<std::string, std::string> m_texts;
};
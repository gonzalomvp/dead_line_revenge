#pragma once

#include <map>

class CStringManager {
public:
	bool loadLanguage(ELanguage language);
	std::string getText(const std::string& label);
	float calculateTextHalfWidth(const std::string& _sText);

private:
	std::map<std::string, std::string> m_texts;
};
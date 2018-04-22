#include "../common/stdafx.h"
#include "string_manager.h"
#include <fstream>


bool StringManager::loadLanguage(TLanguage language) {
	char* filename;
	switch (language) {
		case EEnglish:
			filename = "data/english.txt";
			break;
		case ESpanish:
			filename = "data/spanish.txt";
			break;
		default:
			return false;
	}
	std::ifstream file(filename, std::ios::binary);
	if(!file.is_open()) return false;
	std::string key;
	std::string value;
	while (getline(file, key, ' ') && getline(file, value, '\n')) {
		m_texts[key] = value;
	}
	file.close();

	return true;
}

std::string StringManager::getText(std::string label) {
	return m_texts[label];
}
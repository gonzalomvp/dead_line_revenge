#include "../common/stdafx.h"
#include "string_manager.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include <fstream>

using namespace rapidjson;

bool StringManager::loadLanguage(TLanguage language) {
	char* filename;
	switch (language) {
		case EEnglish:
			filename = "data/english.json";
			break;
		case ESpanish:
			filename = "data/spanish.json";
			break;
		default:
			return false;
	}
	FILE* file = fopen(filename, "r");
	if (!file) return false;

	fseek(file, 0, SEEK_END);
	std::vector<char> fileData(ftell(file));
	fseek(file, 0, SEEK_SET);
	FileReadStream is(file, fileData.data(), fileData.size());
	Document doc;
	doc.ParseStream(is);

	for (Value::ConstMemberIterator itr = doc.MemberBegin(); itr != doc.MemberEnd(); ++itr) {
		m_texts[itr->name.GetString()] = itr->value.GetString();
	}

	fclose(file);
	return true;
}

std::string StringManager::getText(std::string label) {
	return m_texts[label];
}
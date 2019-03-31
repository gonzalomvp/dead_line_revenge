#include "common/stdafx.h"
#include "StringManager.h"

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include <fstream>

using namespace rapidjson;

float CStringManager::calculateTextHalfWidth(const std::string& _sLabel) {
	return getText(_sLabel).length() / 2.0f * 16.0f;
}

bool CStringManager::loadLanguage(ELanguage language) {
	m_texts.clear();
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

std::string CStringManager::getText(const std::string& label) {
	std::string convertedText = label;
	if (m_texts.count(label)) {
		convertedText = m_texts[label];
	}
	return convertedText;
}
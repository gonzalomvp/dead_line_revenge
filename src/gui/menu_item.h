#pragma once

#include "../common/stdafx.h"
#include "../common/sys.h"
#include "../common/core.h"
#include "../common/font.h"
#include "../globals.h"
#include "../engine/graphics_engine.h"

#include <vector>

class MenuItem {
public:
	MenuItem(std::string name, vec2 pos, std::string text) : m_name(name), m_pos(pos), m_text(text), m_hasFocus(false), m_selectedOption(-1) { m_gfxText = new Text("", vmake(0.0f, 0.0f), 1); }
	~MenuItem() { g_graphicsEngine->removeGfxEntity(m_gfxText); delete m_gfxText; }

	std::string getName() { return m_name; }
	void setFocus(bool hasFocus) { m_hasFocus = hasFocus; }
	void addOption(std::string option) { m_options.push_back(option); m_selectedOption = 0; }
	void nextOption();
	void init();
	void activate();
	void deactivate();
	void run();

protected:
	vec2 m_pos;
	std::string m_name;
	std::string m_text;
	Text* m_gfxText;
	bool m_hasFocus;
	bool m_active;
	std::vector<std::string> m_options;
	int m_selectedOption;
};
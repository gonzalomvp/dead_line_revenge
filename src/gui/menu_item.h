#pragma once

#include "../common/stdafx.h"
#include "../common/sys.h"
#include "../common/core.h"
#include "../common/font.h"
#include "../globals.h"
#include "../engine/graphics_engine.h"


class MenuItem {
public:
	MenuItem(std::string name, vec2 pos, std::string text, std::string value = "") : m_name(name), m_pos(pos), m_text(text), m_selected(false), m_value(value) { m_gfxText = new Text(""); this->activate(); }
	~MenuItem() { g_graphicsEngine->removeGfxEntity(m_gfxText); delete m_gfxText; }

	std::string getName() { return m_name; }
	void setSelected(bool selected) { m_selected = selected; }
	void setValue(std::string value) { m_value = value; }
	void activate();
	void deactivate();
	void run();

protected:
	vec2 m_pos;
	std::string m_name;
	std::string m_text;
	std::string m_value;
	Text* m_gfxText;
	bool m_selected;
	bool m_active;
};
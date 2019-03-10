#pragma once

#include "button.h"
#include "checkbox.h"
#include "control.h"
#include "slider.h"

#include <map>

class Text;

//=============================================================================
// MenuItem class
//=============================================================================
class MenuItem {
public:
	MenuItem(const std::string& name, const std::string& text, const vec2& pos) : m_name(name), m_text(text), m_pos(pos), m_gfxText(nullptr), m_hasFocus(false), m_selectedOption(0) {}
	~MenuItem();

	void init      ();
	void activate  ();
	void deactivate();
	void run       ();
	void nextOption();
	
	void        addOption(const std::string& option) { m_options.push_back(option); }
	std::string getName  () const                    { return m_name;               }
	void        setFocus (bool hasFocus)             { m_hasFocus = hasFocus;       }
private:
	std::string              m_name;
	std::string              m_text;
	vec2                     m_pos;
	Text*                m_gfxText;
	bool                     m_hasFocus;
	size_t                   m_selectedOption;
	std::vector<std::string> m_options;
};

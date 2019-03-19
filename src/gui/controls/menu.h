#pragma once

#include "gui/controls/control.h"

#include <map>

class CMenu : public CControl {
public:
	CMenu(const std::string& _sName, const vec2& _v2Pos, const vec2& _v2Size, bool _bIsActive = true) : CControl(_sName, _v2Pos, _v2Size, _bIsActive) {}
	~CMenu();

	virtual void activate  ()                  override;
	virtual void deactivate()                  override;
	virtual void run       (float _fDeltaTime) override;

	void addControl (CControl* _pControl) { m_vControls.push_back(_pControl); }
	CControl* getControlByName(const std::string& _sName);

private:
	std::vector<CControl*> m_vControls;
};

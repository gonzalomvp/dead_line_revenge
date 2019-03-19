#pragma once

#include "gui/controls/control.h"

#include <vector>

class CText;

class CLabel : public CControl {
public:
	CLabel(const std::string& _sName, const vec2& _v2Pos, const vec2& _v2Size, const std::string& _sText, bool _bIsActive = true);
	~CLabel();

	void setText(const std::string& _sText) { m_sText = _sText; }

	// Control
	virtual void init      ()                                    override;
	virtual void run       (float _fDeltaTime)                   override;
	virtual void activate  ()                                    override;
	virtual void deactivate()                                    override;

private:
	std::string m_sText;
	CText*      m_pLabelText;
};
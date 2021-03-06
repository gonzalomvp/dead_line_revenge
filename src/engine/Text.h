#pragma once

#include "engine/GraphicEntity.h"

class CText : public CGraphicEntity {
public:
	CText(const std::string& _sText, vec2 _v2Pos, int _iPriority) : CGraphicEntity(_v2Pos, _iPriority), m_sText(_sText) {}

	std::string getText() const                    { return m_sText;   }
	void        setText(const std::string& _sText) { m_sText = _sText; }

	// CGraphicEntity
	virtual void render() override;

private:
	std::string m_sText;
};
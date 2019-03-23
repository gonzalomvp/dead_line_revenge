#pragma once

#include "components/component.h"

class CHUDMessageComponent : public Component {
public:
	CHUDMessageComponent(Entity* _pOwner, const std::string& _sMessageText)
	: Component(_pOwner)
	, m_sMessageText(_sMessageText)
	, m_sMessage(nullptr)
	{}

	~CHUDMessageComponent();

	// CComponent
	virtual void init() override;

private:
	std::string m_sMessageText;
	CText*      m_sMessage;
};
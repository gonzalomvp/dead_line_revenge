#include "common/stdafx.h"
#include "Selector.h"

void CSelector::onEnter() {
	CComposite::onEnter();

	m_uCurrentChild = 0;
}

CBehavior::EStatus CSelector::onUpdate(float _fDeltaTime) {
	while (m_uCurrentChild < m_vChildBehaviors.size()) {
		EStatus eChildStatus = m_vChildBehaviors[m_uCurrentChild]->run(_fDeltaTime);
		if (eChildStatus != EStatus::EFail) {
			return eChildStatus;
		}
		++m_uCurrentChild;
	}
	return EStatus::EFail;
}


#include "common/stdafx.h"
#include "Sequence.h"

void CSequence::onEnter() {
	CComposite::onEnter();

	m_uCurrentChild = 0;
}

CBehavior::EStatus CSequence::onUpdate(float _fDeltaTime) {
	while (m_uCurrentChild < m_vChildBehaviors.size()) {
		EStatus eChildStatus = m_vChildBehaviors[m_uCurrentChild]->run(_fDeltaTime);
		if (eChildStatus != EStatus::ESuccess) {
			return eChildStatus;
		}
		++m_uCurrentChild;
	}
	return EStatus::ESuccess;
}


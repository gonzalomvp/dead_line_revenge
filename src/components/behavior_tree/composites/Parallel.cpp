#include "common/stdafx.h"
#include "Parallel.h"

CBehavior::EStatus CParallel::onUpdate(float _fDeltaTime) {
	EStatus eReturnStatus = EStatus::ESuccess;
	for (size_t i = 0; i < m_vChildBehaviors.size(); ++i) {
		EStatus eChildStatus = m_vChildBehaviors[i]->run(_fDeltaTime);
		if (eChildStatus != EStatus::ESuccess) {
			eReturnStatus = eChildStatus;
			if (eChildStatus != EStatus::ERunning) {
				break;
			}
		}
	}

	if (eReturnStatus == EStatus::EFail) {
		for (size_t i = 0; i < m_vChildBehaviors.size(); ++i) {
			if (m_vChildBehaviors[i]->getStatus() == EStatus::ERunning) {
				m_vChildBehaviors[i]->abort();
			}
		}
	}
	return eReturnStatus;
}

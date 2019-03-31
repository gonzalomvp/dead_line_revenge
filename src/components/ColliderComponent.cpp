#include "common/stdafx.h"
#include "ColliderComponent.h"

#include "entities/Entity.h"
#include "messages/Message.h"

void CColliderComponent::receiveMessage(TMessage* _pMessage) {
	CComponent::receiveMessage(_pMessage);
	if (!m_bIsActive)
		return;

	ASSERT(_pMessage && m_pOwner);

	if (TMessageCheckCollision* pMessage = dynamic_cast<TMessageCheckCollision*>(_pMessage)) {
		bool bIsOverlapping = false;
		int iCollisionDamage = 0;

		vec2 v2Center = m_pOwner->getPos();
		vec2 v2Size = m_pOwner->getSize();

		CEntity* pOther = pMessage->pOther;

		// First entity check
		if (pMessage->pOther) {
			pMessage->eType = m_eType;
			pMessage->v2Center = v2Center;
			pMessage->v2Size = v2Size;
			pMessage->iDamage = m_iDamage;
			pMessage->iChannelMask = m_iChannelMask;
			pMessage->iChannelMaskResponse = m_iChannelMaskResponse;
			pMessage->pOther = nullptr;
			pOther->receiveMessage(pMessage);

			// Second entity response
			bIsOverlapping = pMessage->bIsOverlapping;
			iCollisionDamage = pMessage->iDamage;
		}

		// Second entity check
		else if ((m_iChannelMaskResponse & pMessage->iChannelMask) || (pMessage->iChannelMaskResponse & m_iChannelMask)) {
			bIsOverlapping = pMessage->bIsOverlapping;
			iCollisionDamage = pMessage->iDamage;
			if (!bIsOverlapping) {
				switch (m_eType) {
					case ECircleCollider:
						switch (pMessage->eType) {
							case ECircleCollider:
								bIsOverlapping = checkCircleCircle(v2Center, v2Size.x * 0.5f, pMessage->v2Center, pMessage->v2Size.x * 0.5f);
								break;
							case ERectCollider:
								bIsOverlapping = checkCircleRect(v2Center, v2Size.x * 0.5f, vsub(pMessage->v2Center, vscale(pMessage->v2Size, 0.5f)), pMessage->v2Size);
								break;
						}
						break;
					case ERectCollider:
						switch (pMessage->eType) {
							case ECircleCollider:
								bIsOverlapping = checkCircleRect(pMessage->v2Center, pMessage->v2Size.x * 0.5f, vsub(v2Center, vscale(v2Size, 0.5f)), v2Size);
								break;
							case ERectCollider:
								bIsOverlapping = checkRectRect(vsub(v2Center, vscale(v2Size, 0.5f)), v2Size, vsub(pMessage->v2Center, vscale(pMessage->v2Size, 0.5f)), pMessage->v2Size);
								break;
						}
						break;
				}
				if (bIsOverlapping) {
					bIsOverlapping = false;
					if (m_iChannelMaskResponse & pMessage->iChannelMask) {
						bIsOverlapping = true;
					}
					if (pMessage->iChannelMaskResponse & m_iChannelMask) {
						pMessage->bIsOverlapping = true;
						pMessage->iDamage = m_iDamage;
					}
				}
			}
		}
		if (bIsOverlapping) {
			TMessageChangeLife mgsChangeLife;
			mgsChangeLife.iDeltaLife = iCollisionDamage;
			m_pOwner->receiveMessage(&mgsChangeLife);
		}
	}
}

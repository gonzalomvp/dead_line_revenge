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

		Entity* pOther = pMessage->other;

		// First entity check
		if (pMessage->other) {
			pMessage->type = m_eType;
			pMessage->center = v2Center;
			pMessage->size = v2Size;
			pMessage->deltaLife = m_iDamage;
			pMessage->collisionChannel = m_iChannelMask;
			pMessage->collisionChannelsResponse = m_iChannelMaskResponse;
			pMessage->other = nullptr;
			pOther->receiveMessage(pMessage);

			// Second entity response
			bIsOverlapping = pMessage->overlap;
			iCollisionDamage = pMessage->deltaLife;
		}

		// Second entity check
		else if ((m_iChannelMaskResponse & pMessage->collisionChannel) || (pMessage->collisionChannelsResponse & m_iChannelMask)) {
			bIsOverlapping = pMessage->overlap;
			iCollisionDamage = pMessage->deltaLife;
			if (!bIsOverlapping) {
				switch (m_eType) {
					case ECircleCollider:
						switch (pMessage->type) {
							case ECircleCollider:
								bIsOverlapping = checkCircleCircle(v2Center, v2Size.x * 0.5f, pMessage->center, pMessage->size.x * 0.5f);
								break;
							case ERectCollider:
								bIsOverlapping = checkCircleRect(v2Center, v2Size.x * 0.5f, vsub(pMessage->center, vscale(pMessage->size, 0.5f)), pMessage->size);
								break;
						}
						break;
					case ERectCollider:
						switch (pMessage->type) {
							case ECircleCollider:
								bIsOverlapping = checkCircleRect(pMessage->center, pMessage->size.x * 0.5f, vsub(v2Center, vscale(v2Size, 0.5f)), v2Size);
								break;
							case ERectCollider:
								bIsOverlapping = checkRectRect(vsub(v2Center, vscale(v2Size, 0.5f)), v2Size, vsub(pMessage->center, vscale(pMessage->size, 0.5f)), pMessage->size);
								break;
						}
						break;
				}
				if (bIsOverlapping) {
					bIsOverlapping = false;
					if (m_iChannelMaskResponse & pMessage->collisionChannel) {
						bIsOverlapping = true;
					}
					if (pMessage->collisionChannelsResponse & m_iChannelMask) {
						pMessage->overlap = true;
						pMessage->deltaLife = m_iDamage;
					}
				}
			}
		}
		if (bIsOverlapping) {
			TMessageChangeLife mgsChangeLife;
			mgsChangeLife.deltaLife = iCollisionDamage;
			m_pOwner->receiveMessage(&mgsChangeLife);
		}
	}
}

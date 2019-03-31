#include "common/stdafx.h"
#include "Sprite.h"

void CSprite::render() {
	CORE_RenderCenteredRotatedSprite(m_v2Pos, m_v2Size, DEG2RAD(m_fAngle), m_uTexture, rgbamake(255, 255, 255, static_cast<int>(255 * m_fAlpha)));
}
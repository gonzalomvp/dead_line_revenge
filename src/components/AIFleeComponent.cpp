#include "common/stdafx.h"
#include "AIFleeComponent.h"

#include "messages/message.h"
#include "scenes/world.h"

namespace {
	const float s_fFleeAngle           = 45.0f;
	const float s_fFleeMinWallDistance = 20.0f;
}

void CAIFleeComponent::run(float _fDeltaTime) {
	Component::run(_fDeltaTime);
	if (!m_bIsActive)
		return;

	//ASSERT(m_pOwner && g_pWorld && g_pWorld->getPlayer());

	//MessageGetTransform messageSelfPos;
	//m_pOwner->receiveMessage(&messageSelfPos);
	//MessageGetTransform messagePlayerPos;
	//g_pWorld->getPlayer()->receiveMessage(&messagePlayerPos);

	////Move in opposite direction
	//vec2 v2PlayerToEnemy = vsub(messageSelfPos.pos, messagePlayerPos.pos);

	////Keep min distance with player
	//if (vlen2(v2PlayerToEnemy) < m_fRange * m_fRange) {
	//	vec2 v2MoveDir = vmake(0.0f, 0.0f);
	//	float fAngle1 = vangle(v2PlayerToEnemy);
	//	float fAngle2 = fAngle1;

	//	//Find the best direction for evasion using different angles
	//	while (vlen2(v2MoveDir) == 0.0f) {
	//		fAngle1 += s_fFleeAngle;
	//		fAngle2 -= s_fFleeAngle;

	//		//Calculate the distance to the screen limits with both directions
	//		vec2 v2IntersectiontWithWall1 = calculatIntersectionWithWorldLimits(messageSelfPos.pos, fAngle1);
	//		float fDist2ToWall1 = vlen2(vsub(v2IntersectiontWithWall1, messageSelfPos.pos));
	//		vec2 v2IntersectiontWithWall2 = calculatIntersectionWithWorldLimits(messageSelfPos.pos, fAngle2);
	//		float fDist2ToWall2 = vlen2(vsub(v2IntersectiontWithWall2, messageSelfPos.pos));

	//		//Pick a direction if it is not too close to the screen limits
	//		if (fDist2ToWall1 > fDist2ToWall2 && fDist2ToWall1 > s_fFleeMinWallDistance * s_fFleeMinWallDistance) {
	//			v2MoveDir = vsub(v2IntersectiontWithWall1, messageSelfPos.pos);
	//		}
	//		else if (fDist2ToWall2 > fDist2ToWall1 && fDist2ToWall2 > s_fFleeMinWallDistance * s_fFleeMinWallDistance) {
	//			v2MoveDir = vsub(v2IntersectiontWithWall2, messageSelfPos.pos);
	//		}
	//	}

	//	MessageSetTransform msgSetTransform;
	//	msgSetTransform.pos = vadd(messageSelfPos.pos, vscale(vnorm(v2MoveDir), m_fSpeed));
	//	msgSetTransform.size = messageSelfPos.size;
	//	m_pOwner->receiveMessage(&msgSetTransform);
	//}
}

vec2 CAIFleeComponent::calculatIntersectionWithWorldLimits(const vec2& _v2Position, float _fAngle) {
	vec2 v2MoveDir = vunit(DEG2RAD(_fAngle));
	vec2 v2Intersection1;
	vec2 v2Intersection2;

	if (v2MoveDir.y > 0) {
		LineLineIntersect(_v2Position, vadd(_v2Position, v2MoveDir), vmake(0, WORLD_HEIGHT), vmake(WORLD_WIDTH, WORLD_HEIGHT), v2Intersection1);
	}
	else {
		LineLineIntersect(_v2Position, vadd(_v2Position, v2MoveDir), vmake(0, 0), vmake(WORLD_WIDTH, 0), v2Intersection1);
	}
		
	if (v2MoveDir.x > 0) {
		LineLineIntersect(_v2Position, vadd(_v2Position, v2MoveDir), vmake(WORLD_WIDTH, 0), vmake(WORLD_WIDTH, WORLD_HEIGHT), v2Intersection2);
	}
	else {
		LineLineIntersect(_v2Position, vadd(_v2Position, v2MoveDir), vmake(0, 0), vmake(0, WORLD_HEIGHT), v2Intersection2);
	}
		
	float fDist2UpDownMargin = vlen2(vsub(v2Intersection1, _v2Position));
	float fDist2LeftRight = vlen2(vsub(v2Intersection2, _v2Position));
	if (fDist2UpDownMargin < fDist2LeftRight) {
		return v2Intersection1;
	}
	else {
		return v2Intersection2;
	}
}

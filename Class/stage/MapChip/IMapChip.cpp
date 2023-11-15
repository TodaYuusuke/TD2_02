#pragma once
#include "IMapChip.h"

bool IMapChip::CheckCollision(LWP::Math::Vector3 checkPos, LWP::Math::Vector3* fixVector) {
	// Y座標をチェックする -> 0以上なら通常の当たり判定 / 未満なら地面の当たり判定
	if (checkPos.y >= 0.0f) {
		return CheckMapChipCollision(checkPos, fixVector);
	}
	else {
		return CheckGroundCollision(checkPos, fixVector);
	}
}


bool IMapChip::IsMapChipCollision() {
	return false;
}
bool IMapChip::CheckMapChipCollision(LWP::Math::Vector3 checkPos, LWP::Math::Vector3* fixVector) {
	return false;
}


bool IMapChip::IsGroundCollision() {
	return true;
}
bool IMapChip::CheckGroundCollision(LWP::Math::Vector3 checkPos, LWP::Math::Vector3* fixVector) {
	return true;
}
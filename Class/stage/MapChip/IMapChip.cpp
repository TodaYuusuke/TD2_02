#pragma once
#include "IMapChip.h"

using namespace LWP::Math;

bool IMapChip::CheckCollision(Vector3 checkPos, Vector3* fixVector) {
	// Y座標をチェックする -> 0以上なら通常の当たり判定 / 未満なら地面の当たり判定
	if (checkPos.y >= 0.0f) {
		return CheckMapChipCollision(checkPos, fixVector);
	}
	else {
		return CheckGroundCollision(checkPos, fixVector);
	}
}

bool IMapChip::IsToGrow() {
	return false;
}

void IMapChip::GrawUp() {
	// ライトが当てられているときに呼び出される処理
}


bool IMapChip::IsMapChipCollision() {
	return false;
}
bool IMapChip::CheckMapChipCollision(Vector3 checkPos, Vector3* fixVector) {
	if (!IsMapChipCollision()) { return false; }

	// 一番小さい値を修正ベクトルとして渡させる
	float fixMoveX = checkPos.x < model_->transform.scale.x / 2.0f ? -checkPos.x : model_->transform.scale.x - checkPos.x;
	float fixMoveZ = checkPos.z < model_->transform.scale.z / 2.0f ? -checkPos.z : model_->transform.scale.z - checkPos.z;

	// 補正値が0の時は何もせずfalseを返す
	if (fixMoveX == 0.0f || fixMoveZ == 0.0f) {
		return false;
	}

	if (fabsf(fixMoveX) < fabsf(fixMoveZ)) {
		fixVector->x = fixMoveX;
	}
	else {
		fixVector->z = fixMoveZ;
	}
	
	return true;
}


bool IMapChip::IsGroundCollision() {
	return true;
}
bool IMapChip::CheckGroundCollision(Vector3 checkPos, Vector3* fixVector) {
	if (!IsGroundCollision()) { return false; }

	// 修正用のベクトルを計算
	fixVector->y = checkPos.y < model_->transform.scale.y / 2.0f ? -checkPos.y : model_->transform.scale.y - checkPos.y;

	// 一番小さい値を修正ベクトルとして渡させる
	float fixMoveX = checkPos.x < model_->transform.scale.x / 2.0f ? -checkPos.x : model_->transform.scale.x - checkPos.x;
	float fixMoveZ = checkPos.z < model_->transform.scale.z / 2.0f ? -checkPos.z : model_->transform.scale.z - checkPos.z;

	// 補正値が0の時は何もせずfalseを返す
	if (fixMoveX == 0.0f || fixMoveZ == 0.0f) {
		return false;
	}

	if (fabsf(fixMoveX) < fabsf(fixMoveZ)) {
		fixVector->x = fixMoveX;
	}
	else {
		fixVector->z = fixMoveZ;
	}

	return true;
}

void IMapChip::OnActive() {
	model_->isActive = true;
}
void IMapChip::OffActive() {
	model_->isActive = false;
}
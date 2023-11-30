#include "StageSelectPlayer.h"
#include "../stage/Stage.h"

using namespace LWP::Input;
using namespace LWP::Math;

void StageSelectPlayer::Action() {
	// 常に歩くアニメーション
	UpAndDownMotion(1.5f);

	if (isMove == true) {
		UpdateMoveNoHaveArmAnimation();
	}

	//左腕
	rightUpperArmModel_->transform.translation.x = 0.1f;
	rightUpperArmModel_->transform.translation.y = 0.02f;
	rightUpperArmModel_->transform.rotation.z = 0.5f;

	//右腕 :ランタン持っている腕
	leftUpperArmModel_->transform.translation.x = -0.0f;
	leftUpperArmModel_->transform.translation.y = 0.0f;
	leftUpperArmModel_->transform.rotation.y = -1.5f;


	// キーを押したらステージ選択の結果を送る
	isSendAllow = (Keyboard::GetTrigger(DIK_SPACE) || Pad::GetTrigger(0, XBOX_RB));
}

void StageSelectPlayer::UpdateDerved(Stage* stage) {
	selectedStageLevel = stage->CheckNearStageSelectMapChip(model_->transform.GetWorldPosition());
}
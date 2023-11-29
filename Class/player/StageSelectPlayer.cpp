#include "StageSelectPlayer.h"
#include "../stage/Stage.h"

using namespace LWP::Input;
using namespace LWP::Math;

void StageSelectPlayer::Action() {
	// キーを押したらステージ選択の結果を送る
	isSendAllow = (Keyboard::GetTrigger(DIK_SPACE) || Pad::GetTrigger(0, XBOX_RB));
}

void StageSelectPlayer::UpdateDerved(Stage* stage) {
	selectedStageLevel = stage->CheckNearStageSelectMapChip(model_->transform.GetWorldPosition());
}
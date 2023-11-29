#include "StageSelectPlayer.h"
#include "../stage/Stage.h"

using namespace LWP::Input;
using namespace LWP::Math;

void StageSelectPlayer::Action() {
	// キーを押したらステージ選択
	if (Keyboard::GetTrigger(DIK_SPACE) || Pad::GetTrigger(0, XBOX_RB)) {
		
	}
}

void StageSelectPlayer::UpdateDerved(Stage* stage) {
	stage;
}

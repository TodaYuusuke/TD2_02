#pragma once
#include "Stage.h"

class StageSelect final 
	: public Stage {
public:	// ** メンバ関数 ** //

	// 初期化
	void Init();

	// プレイヤーがステージ選択マップチップの近くにいるか検証
	int CheckNearStageSelectMapChip(LWP::Math::Vector3 playerPosition);
};
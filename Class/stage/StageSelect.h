#pragma once
#include "Stage.h"

class Player;

class StageSelect final 
	: public Stage {
public:	// ** メンバ関数 ** //

	// 初期化
	void Init(Player* player);

	// プレイヤーがステージ選択マップチップの近くにいるか検証
	int CheckNearStageSelectMapChip(LWP::Math::Vector3 playerPosition) override;
};
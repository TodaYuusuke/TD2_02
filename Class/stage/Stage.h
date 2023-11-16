#pragma once
#include "MapChip/Derved.h"
#include <string>

class Stage final {
public:	// ** メンバ関数 ** //

	// 初期化（読み込むレベル）
	void Init(int level);
	// 更新
	void Update();

	// 当たり判定をチェックする関数
	bool CheckCollision(LWP::Math::Vector3 checkPos, LWP::Math::Vector3* fixVector, bool isPlayer);

	// プレイヤーのスタート地点を受け取る関数
	LWP::Math::Vector3 GetPlayerStartPosition() { return playerStartPosition; }

private: // ** メンバ変数 ** //

	// マップチップ
	std::vector<std::vector<IMapChip*>> mapChip_;

	// プレイヤーのスタート地点
	LWP::Math::Vector3 playerStartPosition = {0.0f,0.0f,0.0f};
	// マップチップ共通のスケール
	float commonScale = 1.0f;

private: // ** プライベートな関数 ** //
};
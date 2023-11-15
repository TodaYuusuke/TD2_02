#pragma once
#include "MapChip/Derved.h"
#include <string>

class Stage final {
public:	// ** メンバ関数 ** //

	// 初期化（読み込むレベル）
	void Init(int level);
	// 更新
	void Update();


private: // ** メンバ変数 ** //

	// マップチップ
	std::vector<std::vector<IMapChip*>> mapChip_;

	// マップチップ共通のスケール
	float commonScale = 1.0f;

private: // ** プライベートな関数 ** //
};
#pragma once
#include <Adapter.h>
#include "Player.h"

// 前方宣言
class Stage;

class StageSelectPlayer final
	: public Player {
public: // ** メンバ関数 ** //

	int GetSelectedStageLevel() { return selectedStageLevel; }

private: // ** プライベートな関数 ** //

	// ランタンに関する処理
	void Action() override;	
	
	// 派生先用更新処理
	void UpdateDerved(Stage* stage) override;

private: // ** メンバ変数 ** //

	int selectedStageLevel = -1;
};
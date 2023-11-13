#pragma once
#include <Adapter.h>
#include "../Engine/scene/IScene.h"

#include "../stage/Stage.h"
#include "../player/Lantern.h"

class GameScene final
	: public IScene {
public:

	// コンストラクタの定義も自由に可
	GameScene(int level);
	GameScene() = default;

	//*** 純粋仮想関数の実体宣言 ***//

	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;

	// scene遷移したい場合、ISceneポインタ型のnextSceneに次のシーンの実体を代入

private: //*** これより先に必要な処理や変数を記述 ***//

	// ステージのレベル
	const int level_;
	// ステージデータ
	Stage stage_;

	// ランタン
	Lantern lantern_;
};
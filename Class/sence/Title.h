#pragma once
#include <Adapter.h>
#include "../Engine/scene/IScene.h"

class Title final
	: public IScene {
public:

	// コンストラクタの定義も自由に可
	Title() = default;

	//*** 純粋仮想関数の実体宣言 ***//

	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;

	// scene遷移したい場合、ISceneポインタ型のnextSceneに次のシーンの実体を代入

private: //*** これより先に必要な処理や変数を記述 ***//
	LWP::Primitive::Sphere* sphere;
	// プレイヤーモデル
	LWP::Object::WorldTransform player_;
	LWP::Primitive::Mesh* headModel_ = nullptr;				//頭
	LWP::Primitive::Mesh* bodyModel_ = nullptr;				//上半身
	LWP::Primitive::Mesh* leftForeArmModel_ = nullptr;		//左腕　手から肘
	LWP::Primitive::Mesh* leftUpperArmModel_ = nullptr;		//左腕　肘から肩
	LWP::Primitive::Mesh* rightForeArmModel_ = nullptr;		//右腕　手から肘
	LWP::Primitive::Mesh* rightUpperArmModel_ = nullptr;	//右腕　肘から肩
	LWP::Primitive::Mesh* footModel_ = nullptr;				//下半身
};
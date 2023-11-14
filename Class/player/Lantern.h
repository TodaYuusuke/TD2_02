#pragma once
#include <Adapter.h>

// 前方宣言
class Stage;

/// <summary>
/// ランタン
/// </summary>
class Lantern final {
public: // ** メンバ関数 ** //

	// 初期化
	void Init();
	// 更新
	void Update(Stage* stage);


	/// <summary>
	/// ランタンを掴む
	/// </summary>
	/// <returns>ランタンを追従させる用のペアレントポインタ</returns>
	void Grab(LWP::Object::WorldTransform* trasnform);

	/// <summary>
	/// ランタンを投げる
	/// </summary>
	/// <param name="direction">投げる方向のベクトル（正規化済みを送ること）</param>
	void Throw(LWP::Math::Vector2 direction);


private: // ** メンバ変数 ** //

	// モデル
	LWP::Primitive::Mesh* handleModel_;
	LWP::Primitive::Mesh* model_;
	// 光源
	LWP::Object::PointLight* light_;

	// 掴まれているフラグ
	bool isGrabed;
};
#pragma once
#include <Adapter.h>

/// <summary>
/// ランタン
/// </summary>
class Lantern final {
public: // ** メンバ関数 ** //

	// 初期化
	void Init();
	// 更新
	void Update();


	/// <summary>
	/// ランタンを掴む
	/// </summary>
	/// <returns>ランタンを追従させる用のペアレントポインタ</returns>
	LWP::Object::WorldTransform* Grab();

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
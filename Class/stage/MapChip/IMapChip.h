#pragma once
#include <Adapter.h>

/// <summary>
/// マップチップ1つ分のクラス
/// </summary>
class IMapChip {
public:	// ** メンバ関数 ** //

	~IMapChip() = default;

	// 初期化
	virtual void Init(LWP::Math::Vector3 position, float scale) = 0;
	// 更新
	virtual void Update() = 0;

	/// <summary>
	/// 当たり判定をチェックする関数
	/// </summary>
	/// <param name="checkPos">チェックする座標</param>
	/// <param name="fixVector">埋まっている分を修正するベクトル</param>
	/// <returns></returns>
	virtual bool CheckCollision(LWP::Math::Vector3 checkPos, LWP::Math::Vector3* fixVector);

	// 座標をセット
	void SetPosition(LWP::Math::Vector3 pos) { if (model_) { model_->transform.translation = pos; } }
	// スケールをセット
	void SetScale(float scale) { if (model_) { model_->transform.scale = { scale,scale,scale }; } }
	
	// モデルの変数を受け取る
	LWP::Primitive::Mesh* GetModel() { if (model_) { return model_; } }
	void SetModel(LWP::Primitive::Mesh* model) { model_ = model; }


protected: // ** メンバ変数 ** //

	// モデル
	LWP::Primitive::Mesh* model_;

protected: // ** プライベートな関数 ** //

	// このマス自体に対する当たり判定があるかどうか
	virtual bool IsMapChipCollision();
	bool CheckMapChipCollision(LWP::Math::Vector3 checkPos, LWP::Math::Vector3* fixVector);
	// このマスの地面に対する当たり判定
	virtual bool IsGroundCollision();
	bool CheckGroundCollision(LWP::Math::Vector3 checkPos, LWP::Math::Vector3* fixVector);

};

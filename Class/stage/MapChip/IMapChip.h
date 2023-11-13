#pragma once
#include <Adapter.h>

/// <summary>
/// マップチップ1つ分のクラス
/// </summary>
class IMapChip {
public:	// ** メンバ関数 ** //

	~IMapChip() = default;

	// 初期化
	virtual void Init(LWP::Math::Vector3 position) = 0;
	// 更新
	virtual void Update() = 0;


protected: // ** メンバ変数 ** //

	// モデル
	LWP::Primitive::Mesh* model_;
};

#pragma once
#include "IMapChip.h"

// マップチップを定義
enum class Mapchip : int {
	Floor = 00,	// 何もなし（床）
	Hole = 01,	// 穴（光を通す、落ちる）
	Wall = 02,	// 壁（光を遮る）
	VineWall = 10,	// 壁になるツタ
	VineFloor = 11,	// 足場になるツタを生やすブロック
	GrownVineFloor = 21,	// 成長して足場になったツタ
	Start = 98,	// スタート地点
	Candle = 99	// ろうそく（クリア条件	）
};

class Floor :public IMapChip {
public: // ** メンバ関数 ** //
	void Init(LWP::Math::Vector3 position, float scale) override;
	void Update() override;
	bool IsMapChipCollision() override;
	bool IsGroundCollision() override;
};
class Hole :public IMapChip {
public: // ** メンバ関数 ** //
	void Init(LWP::Math::Vector3 position, float scale) override;
	void Update() override;
	bool IsMapChipCollision() override;
	bool IsGroundCollision() override;
};
class Wall :public IMapChip {
public: // ** メンバ関数 ** //
	void Init(LWP::Math::Vector3 position, float scale) override;
	void Update() override;
	bool IsMapChipCollision() override;
	bool IsGroundCollision() override;
};
class VineWall :public IMapChip {
public: // ** メンバ関数 ** //
	void Init(LWP::Math::Vector3 position, float scale) override;
	void Update() override;
	bool IsMapChipCollision() override;
	bool IsGroundCollision() override;
};
class VineFloor :public IMapChip {
public: // ** メンバ関数 ** //
	void Init(LWP::Math::Vector3 position, float scale) override;
	void Update() override;
	bool IsMapChipCollision() override;
	bool IsGroundCollision() override;
};
class Candle :public IMapChip {
public: // ** メンバ関数 ** //
	void Init(LWP::Math::Vector3 position, float scale) override;
	void Update() override;
	bool IsMapChipCollision() override;
	bool IsGroundCollision() override;
};
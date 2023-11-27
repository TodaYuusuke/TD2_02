#pragma once
#include "IMapChip.h"

// マップチップを定義
enum class Mapchip : int {
	Floor = 00,	// 何もなし（床）
	Hole = 01,	// 穴（光を通す、落ちる）
	Wall = 02,	// 壁（光を遮る）
	VineWall = 10,	// 壁になるツタ
	VineFloor = 11,	// 足場になるツタを生やすブロック
	DarkVineFloor = 12,	// 足場になるツタが生えてるブロック（照らすと死ぬ）
	DarkVineLeaf = 13, // 上のマップチップから生えてる葉っぱ
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
	void GrawUp() override;

	// 足場になる葉っぱのモデル
	LWP::Primitive::Mesh* leafModel_;
	// 成長済みフラグ
	int isGrew_ = 0;
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
	bool IsToGrow() override { return true; }
	void GrawUp() override;

private:
	// 成長する木のモデル
	LWP::Primitive::Mesh* treeModel_;
	// 成長済みフラグ
	int isGrew_ = 0;
};
class VineFloor :public IMapChip {
public: // ** メンバ関数 ** //
	void Init(LWP::Math::Vector3 position, float scale) override;
	void Update() override;
	bool IsMapChipCollision() override;
	bool IsGroundCollision() override;
	bool IsToGrow() override { return true; }
	void GrawUp() override;

private:
	// 成長済みフラグ
	int isGrew_ = 0;
};
class DarkVineFloor :public IMapChip {
public: // ** メンバ関数 ** //
	void Init(LWP::Math::Vector3 position, float scale) override;
	void Update() override;
	bool IsMapChipCollision() override;
	bool IsGroundCollision() override;
	bool IsToGrow() override { return true; }
	void GrawUp() override;

	// 何フレーム照らし続けたら死ぬか
	const int kFrameUntilDeath = 60;
	// 成長済みフラグ
	int isGrew_ = 0;
};
class DarkVineLeaf :public IMapChip {
public: // ** メンバ関数 ** //
	void Init(LWP::Math::Vector3 position, float scale) override;
	void Update() override;
	bool IsMapChipCollision() override;
	bool IsGroundCollision() override;

	// すでに枯れたフラグ
	bool isDead_ = false;
};
class Start :public IMapChip {
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
	
	// 点火済みフラグ
	bool isIgnited = false;

private:
	// 火をつけるモデル
	LWP::Primitive::Mesh* candleModel_;
	// 光源
	LWP::Object::PointLight* light_;
};
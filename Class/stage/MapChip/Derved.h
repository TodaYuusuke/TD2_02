#pragma once
#include "IMapChip.h"

class Player;

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
	Candle = 99, // ろうそく（クリア条件	）
	// 100以降は-100した値のレベルのステージセレクト
	Stage1 = 101,
	Stage2,
	Stage3,
	Stage4,
	Stage5,
	Stage6,
	Stage7,
	Stage8,
	Stage9,
	Stage10,
	Stage11,
	Stage12,
	Stage13,
	Stage14,
	OutLine = 999,
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

	void OnActive() override;
	void OffActive() override;

	// それぞれの向きに対応して葉の向きを変える
	void SetFlowerRotateY(float rotateY) { leafModel_->transform.rotation.y = rotateY; }
	// 波打って見えるようにアニメーションフレームをわずかにずらす
	void SetBeginAnimationFrame(int beginFrame) { startAnimationFrame_ = beginFrame; }

private: // ** プライベートな関数 ** //
	// t = 
	// b = 開始の値
	// c = 開始との差分
	// d = 
	float easeOut(float t, float b, float c, float d) {
		t /= d;
		return -c * t * (t - 2.0f) + b;
	}

public:
	// 足場になる葉っぱのモデル
	LWP::Primitive::Mesh* leafModel_;
	// 成長済みフラグ
	int isGrew_ = 0;

	// 照らされている時間
	int lightingTime_ = 0;

	// 揺れるアニメーション
	bool isAnimation_ = false;
	// アニメーション時間
	int swayFrame_;

	int waitFrame_ = 0;
	int startAnimationFrame_ = 0;

	// 振幅
	float maxAmplitude_;

	// 定数
	// 1往復するまでの時間
	const int swayEndFrame_ = 120;
	// 1フレーム当たりの変化量
	const float kCycleSpeed_ = 2 * M_PI / swayEndFrame_;
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

	void OnActive() override;
	void OffActive() override;

private: // ** プライベートな関数 ** //
	// t = 
	// b = 開始の値
	// c = 開始との差分
	// d = 
	float easeOut(float t, float b, float c, float d) {
		t /= d;
		return -c * t * (t - 2.0f) + b;
	}

private:
	// 成長する木のモデル
	LWP::Primitive::Mesh* treeModel_;
	// 成長済みフラグ
	int isGrew_ = 0;
	// 光の当たっている時間
	int lightingTime_;
	// 
	bool isAnimation_ = false;
};
class VineFloor :public IMapChip {
public: // ** メンバ関数 ** //
	void Init(LWP::Math::Vector3 position, float scale) override;
	void Update() override;
	bool IsMapChipCollision() override;
	bool IsGroundCollision() override;
	bool IsToGrow() override { return true; }
	void GrawUp() override;

	void OnActive() override;
	void OffActive() override;

private:
	// 茎のモデル
	LWP::Primitive::Mesh* stemModel_;
	// 花のモデル
	LWP::Primitive::Mesh* flowerModel_;

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
private:
	// 光源
	LWP::Object::PointLight* light_;
};
class DarkVineLeaf :public IMapChip {
public: // ** メンバ関数 ** //
	void Init(LWP::Math::Vector3 position, float scale) override;
	void Update() override;
	bool IsMapChipCollision() override;
	bool IsGroundCollision() override;

	//void OnActive() override;
	//void OffActive() override;

	// 葉の向きを設定
	void SetRotateY(float rotateY) { model_->transform.rotation.y = rotateY; }
	// 波打って見えるようにアニメーションフレームをわずかにずらす
	void SetBeginAnimationFrame(int beginFrame) { swayFrame_ += beginFrame; }

	// すでに枯れたフラグ
	bool isDead_ = false;

private:
	// アニメーション時間
	int swayFrame_;

	// 振幅
	float maxAmplitude_;

	// 定数
	// 1往復するまでの時間
	const int swayEndFrame_ = 120;
	// 1フレーム当たりの変化量
	const float kCycleSpeed_ = 2 * M_PI / swayEndFrame_;
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

	void OnActive() override;
	void OffActive() override;
	
	// 点火済みフラグ
	bool isIgnited = false;

private:
	// 火をつけるモデル
	LWP::Primitive::Mesh* candleModel_;
	// 光源
	LWP::Object::PointLight* light_;
};
class OutLineWall :public IMapChip {
public: // ** メンバ関数 ** //
	void Init(LWP::Math::Vector3 position, float scale) override;
	void Update() override;
	bool IsMapChipCollision() override;
	bool IsGroundCollision() override;

	void OnActive() override;
	void OffActive() override;

private:
	// 木の数
	static const int kMaxTree = 4;

	// 幹のモデル
	LWP::Primitive::Mesh* trunkModel_[kMaxTree];
	// 葉のモデル
	LWP::Primitive::Mesh* leavesModel_[kMaxTree][3];
};
class StageMap :public IMapChip {
public: // ** メンバ関数 ** //
	void Init(LWP::Math::Vector3 position, float scale) override;
	void Update() override;
	bool IsMapChipCollision() override;
	bool IsGroundCollision() override;

	void OnActive() override;
	void OffActive() override;
	
	// プレイヤーが近くにいるときに呼ばれる関数
	void NearPlayer();

	// 自身が持つステージ番号
	int stageNum;
	// 方向追従用のプレイヤーポインタ
	Player* player;

private:
	// 木の数
	static const int kMaxTree = 9;
	
	// 幹のモデル
	LWP::Primitive::Mesh* trunkModel_[kMaxTree];
	// 葉のモデル
	LWP::Primitive::Mesh* leavesModel_[kMaxTree][3];
	// 看板のモデル
	LWP::Primitive::Mesh* signModel_;

	// 光の当たっている時間
	int lightingTime_;

private: // ** 関数 ** //
	// t = 
	// b = 開始の値
	// c = 開始との差分
	// d = 
	float easeOut(float t, float b, float c, float d) {
		t /= d;
		return -c * t * (t - 2.0f) + b;
	}

	// 球面線形補間（あとでエンジンのUtilityに持ってく）
	LWP::Math::Vector3 Slerp(const LWP::Math::Vector3& v1, const LWP::Math::Vector3& v2, float t) {
		return v1 * (1 - t) + v2 * t;
	}
};
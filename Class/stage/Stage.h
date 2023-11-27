#pragma once
#include "MapChip/Derved.h"
#include "Flower.h"
#include <string>

class Stage final {
public:	// ** メンバ関数 ** //

	// 初期化（読み込むレベル）
	void Init(int level);
	// 更新
	void Update();

	// 当たり判定をチェックする関数
	bool CheckCollision(LWP::Math::Vector3 checkPos, LWP::Math::Vector3* fixVector, bool isPlayer, bool isGrabLantern);

	// 光の当たり判定
	void CheckLightCollision(LWP::Math::Vector3 center, float radius);

	// プレイヤーのスタート地点を受け取る関数
	LWP::Math::Vector3 GetPlayerStartPosition() { return playerStartPosition; }
	// プレイヤーの現在地点をセットする関数
	void SetPlayerData(LWP::Math::Vector3 pos, LWP::Math::Vector3 rot) {
		playerCurrentPosition_ = pos;
		playerCurrentRotation_ = rot;
	}

private: // ** メンバ変数 ** //

	// マップチップ
	std::vector<std::vector<IMapChip*>> mapChip_;
	// 花
	Flower flower_;

	// プレイヤーのスタート地点
	LWP::Math::Vector3 playerStartPosition = {0.0f,0.0f,0.0f};
	// プレイヤーの現在地
	LWP::Math::Vector3 playerCurrentPosition_ = { 0.0f,0.0f,0.0f };
	// プレイヤーのY回転
	LWP::Math::Vector3 playerCurrentRotation_ = { 0.0f,0.0f,0.0f };
	// マップチップ共通のスケール
	float commonScale = 1.0f;

private: // ** プライベートな関数 ** //

	// 光の当たり判定用の、ベクトルとマップチップの交差判定
	bool IsIntersecting(LWP::Math::Vector2 start, LWP::Math::Vector2 end, LWP::Math::Vector3 mapChipTransform);
	// プレイヤーとの交差判定
	bool IsPlayerIntersecting(LWP::Math::Vector2 start, LWP::Math::Vector2 end);

	// Vector2の内積を求める関数
	float CrossProduct(LWP::Math::Vector2 v1, LWP::Math::Vector2 v2);
};
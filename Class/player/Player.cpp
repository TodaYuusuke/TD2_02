#include "Player.h"
#include "../stage/Stage.h"

using namespace LWP::Input;
using namespace LWP::Math;

void Player::Init(LWP::Object::Camera* camera) {
	model_ = LWP::Resource::LoadModel("Player/Player.obj");
	model_->material.enableLighting = true;
	model_->commonColor = new LWP::Utility::Color(LWP::Utility::ColorPattern::BLUE);
	grabPosition_.translation = { 0.15f,0.3f,0.2f };

	camera_ = camera;
	cameraGoalRotation_ = camera_->transform.rotation;

	// ランタンを初期化
	lantern_.Init();

	// 親子関係を登録
	grabPosition_.Parent(&model_->transform);
	lantern_.Grab(&grabPosition_);
}

void Player::Update(Stage* stage) {
#if _DEBUG
	ImGui::Begin("Player");
	if (ImGui::TreeNode("PlayerModel")) {
		model_->DebugGUI();
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("GrabPosition")) {
		grabPosition_.DebugGUI();
		ImGui::TreePop();
	}
	ImGui::DragFloat3("CameraOffset", &cameraOffset_.x, 0.01f);
	ImGui::End();
#endif


	// 移動処理
	Move();

	// 当たり判定

	// ランタン更新
	lantern_.Update(stage);

	// カメラを追従させる
	FollowCameraTurn();
}

void Player::Move() {
	// 移動する向き
	Vector3 dir = { 0.0f,0.0f,0.0f };

	// キーボードでの移動
	if (Keyboard::GetPress(DIK_W)) {
		dir.z += 1.0f;
	}
	if (Keyboard::GetPress(DIK_S)) {
		dir.z -= 1.0f;
	}
	if (Keyboard::GetPress(DIK_D)) {
		dir.x += 1.0f;
	}
	if (Keyboard::GetPress(DIK_A)) {
		dir.x -= 1.0f;
	}

	// コントローラーでの移動
	//dir.x += Pad::GetLStick().x;
	//dir.y -= Pad::GetLStick().y;	// Yは逆

	// 正規化してから使用
	Matrix4x4 cameraRotationMatrix = Matrix4x4::CreateRotateXYZMatrix({ 0.0f, camera_->transform.rotation.y, 0.0f });
	dir = dir.Normalize() * kPlayerSpeed * cameraRotationMatrix;
	model_->transform.translation += dir;

	// 向きを設定
	if (dir.Length() > 0.0f) {
		// 目的の角度
		Vector3 goalRotation = { 0.0f, 0.0f, 0.0f };
		// Y軸周りの角度
		goalRotation.y = std::atan2f(dir.x, dir.z);
		// X軸周りの角度
		goalRotation.x = std::atan2f(-dir.y, Vector3{ dir.x, 0.0f, dir.z }.Length());

		// 現在の角度と目標の角度を比較し、逆回転の場合に調整
		if (std::abs(model_->transform.rotation.y - goalRotation.y) > M_PI) {
			if (model_->transform.rotation.y > goalRotation.y) {
				model_->transform.rotation.y -= static_cast<float>(2.0f * M_PI);
			}
			else {
				model_->transform.rotation.y += static_cast<float>(2.0f * M_PI);
			}
		}

		// 角度を適応
		model_->transform.rotation = Slerp(model_->transform.rotation, goalRotation, 0.2f);
	}
}

void Player::ChangeFollowCamera() {
	isFollowingCamera_ = !isFollowingCamera_;
	// ゴールを更新
	cameraGoalRotation_ = camera_->transform.rotation;
}

// 線形補完
Vector3 Player::Lerp(const Vector3& v1, const Vector3& v2, float t) {
	return v1 + ((v2 - v1) * t);
}
Vector3 Player::Slerp(const Vector3& v1, const Vector3& v2, float t) {
	return v1 * (1 - t) + v2 * t;
}

void Player::FollowCameraTurn() {
	if (!isFollowingCamera_) { return; }	// フラグがfalseのときは追従の計算をしない

	// 回転する向き
	Vector2 dir = { 0.0f,0.0f };

	// キーボードでの回転
	if (Keyboard::GetPress(DIK_UP)) {
		dir.x += 1.0f;
	}
	if (Keyboard::GetPress(DIK_DOWN)) {
		dir.x -= 1.0f;
	}
	if (Keyboard::GetPress(DIK_LEFT)) {
		dir.y -= 1.0f;
	}
	if (Keyboard::GetPress(DIK_RIGHT)) {
		dir.y += 1.0f;
	}

	// コントローラーでの回転
	//dir.x += Pad::GetRStick().x;
	//dir.y -= Pad::GetRStick().y;	// Yは逆

	// 正規化してから使用
	dir = dir.Normalize();
	// 次の目標回転角を計算
	cameraGoalRotation_ += Vector3{ dir.x* kFollowCameraSpeed, dir.y* kFollowCameraSpeed, 0.0f };
	//goalRotation += { dir.x* kFollowCameraSpeed, dir.y* kFollowCameraSpeed, 0.0f };

	// カメラから追従対象に対する角度を求める
	cameraGoalTranslation_ = model_->transform.translation + (cameraOffset_ * Matrix4x4::CreateRotateXYZMatrix(cameraGoalRotation_));

	//camera_->transform.translation = goalTranslation;
	//camera_->transform.rotation = goalRotation;
	
	// 徐々に追従するように適応
	camera_->transform.translation = Lerp(camera_->transform.translation, cameraGoalTranslation_, 0.07f);
	camera_->transform.rotation = Lerp(camera_->transform.rotation, cameraGoalRotation_, 0.06f);

	//camera_->transform.translation = Lerp(camera_->transform.translation, goalTranslation, 0.02f);
	//goalRotation = Lerp({ 0.0f,0.0f,0.0f }, goalRotation, 0.02f);
	//camera_->transform.rotation += goalRotation;
}
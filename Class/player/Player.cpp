#include "Player.h"
#include "../stage/Stage.h"

using namespace LWP::Input;
using namespace LWP::Math;

void Player::Init(Vector3 startPosition, LWP::Object::Camera* camera) {
#if _DEBUG
	model_ = LWP::Resource::LoadModel("Player/LowPolyPlayer.obj");
#else
	model_ = LWP::Resource::LoadModel("Player/Player.obj");
#endif

	headModel_ = LWP::Resource::LoadModel("Player/player_head.obj");
	bodyModel_ = LWP::Resource::LoadModel("Player/player_body.obj");
	footModel_ = LWP::Resource::LoadModel("Player/player_foot.obj");
	rightUpperArmModel_ = LWP::Resource::LoadModel("Player/player_right_upperArm.obj");
	leftUpperArmModel_ = LWP::Resource::LoadModel("Player/player_left_upperArm.obj");
	rightForeArmModel_ = LWP::Resource::LoadModel("Player/player_right_foreArm.obj");
	leftForeArmModel_ = LWP::Resource::LoadModel("Player/player_left_foreArm.obj");



	model_->transform.translation = startPosition;
	bodyModel_->transform.translation = startPosition;
	

	headModel_->material.enableLighting = true;
	bodyModel_->material.enableLighting = true;
	footModel_->material.enableLighting = true;
	rightUpperArmModel_->material.enableLighting = true;
	leftUpperArmModel_->material.enableLighting = true;
	rightForeArmModel_->material.enableLighting = true;
	leftForeArmModel_->material.enableLighting = true;

	

	startPosition_ = startPosition;
	model_->transform.scale = { 0.5f,0.5f,0.5f };
	model_->material.enableLighting = true;
	model_->commonColor = new LWP::Utility::Color(LWP::Utility::ColorPattern::BLUE);
	grabPosition_.translation = { 0.0f,0.35f,0.35f };
	model_->isActive = false;

	camera_ = camera;
	cameraGoalRotation_ = camera_->transform.rotation;

	// ランタンを初期化
	lantern_.Init();

	// 親子関係を登録
	grabPosition_.Parent(&model_->transform);
	lantern_.Grab(&grabPosition_);

	headModel_->transform.Parent(&bodyModel_->transform);
	footModel_->transform.Parent(&bodyModel_->transform);
	leftUpperArmModel_->transform.Parent(&bodyModel_->transform);
	rightUpperArmModel_->transform.Parent(&bodyModel_->transform);

	leftForeArmModel_->transform.Parent(&leftUpperArmModel_->transform);
	rightForeArmModel_->transform.Parent(&rightUpperArmModel_->transform);
}

void Player::Update(Stage* stage) {
#if _DEBUG
	ImGui::Begin("Player");
	if (ImGui::TreeNode("PlayerModel")) {
		model_->DebugGUI();
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("PlayerHeadModel")) {
		headModel_->DebugGUI();
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("PlayerVodyModel")) {
		bodyModel_->DebugGUI();
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("PlayerFootModel")) {
		footModel_->DebugGUI();
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("PlayerLeftForeArmModel")) {
		leftForeArmModel_->DebugGUI();
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("PlayerLeftUpperArmModel")) {
		leftUpperArmModel_->DebugGUI();
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("PlayerRightForeArmModel")) {
		rightForeArmModel_->DebugGUI();
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("PlayerRightUpperArmModel")) {
		rightUpperArmModel_->DebugGUI();
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
	// 行動更新処理
	Action();

	// 当たり判定を取る座標4点
	Vector3 checkPos[4] = {
		model_->transform.translation - (Vector3{0.3f,0.0f,0.3f} / 2.0f),
		model_->transform.translation - (Vector3{-0.3f,0.0f,0.3f} / 2.0f),
		model_->transform.translation - (Vector3{0.3f,0.0f,-0.3f} / 2.0f),
		model_->transform.translation - (Vector3{-0.3f,0.0f,-0.3f} / 2.0f)
	};

#if _DEBUG	// 当たり判定表示用の球
	/*static LWP::Primitive::Sphere* s[4] = {
		LWP::Primitive::CreateInstance<LWP::Primitive::Sphere>(),
		LWP::Primitive::CreateInstance<LWP::Primitive::Sphere>(),
		LWP::Primitive::CreateInstance<LWP::Primitive::Sphere>(),
		LWP::Primitive::CreateInstance<LWP::Primitive::Sphere>()
	};*/
#endif

	// 判定がなくなるまで無限ループ
	bool isHit = true;

	// 回りの判定をとる
	while (isHit) {
		isHit = false;
		for (int i = 0; i < 4; i++) {
#if _DEBUG	// 当たり判定表示用の球
			//s[i]->transform = checkPos[i];
			//s[i]->Radius(0.02f);
#endif
			Vector3 fixVector = { 0.0f,0.0f,0.0f };
			bool result = stage->CheckCollision(checkPos[i], &fixVector, true);
			// ヒットしていればフラグをtrueに
			if (result) {
				isHit = result;

				// もしY軸補正がされた場合
				if (fixVector.y > 0.0f) {
					// ある程度のY軸補正より大きい場合は無視する
					if (fixVector.y > -kGravities) {
						fixVector.y = 0.0f;
					}
					else {
						fixVector.x = 0.0f;
						fixVector.z = 0.0f;
						gravitiesAT = 0.0f;
					}
				}

				for (int j = 0; j < 4; j++) {
					checkPos[j] += fixVector;
				}

				model_->transform.translation += fixVector;
			}
		}
	}

	// プレイヤーが一定以上下に落ちたとき -> 元の座標に戻す
	if (model_->transform.translation.y < -2.0f) {
		isFollowingCamera_ = false;
		if (model_->transform.translation.y < -8.0f) {
			model_->transform.translation = startPosition_;
			gravitiesAT = 0.0f;
			isFollowingCamera_ = true;
		}
	}

	// プレイヤーの座標をステージに保持させる
	stage->SetPlayerData(model_->transform.GetWorldPosition(), model_->transform.rotation);

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
	


	// 構えているときは動かない
	if (behavior_ != Behavior::ReadyToThrow && behavior_ != Behavior::Throwing) {
		model_->transform.translation += dir;

		
		bodyModel_->transform.translation= model_->transform.translation;
	
	
		

		// 歩いないときの処理
		if (dir.Length() == 0.0f) {
			lantern_.WaitSwingAmplitude();
		}
		else {
			lantern_.MoveSwingAmplitude();
		}
	}

	// 重力を付与
	gravitiesAT += kGravities;
	model_->transform.translation.y += gravitiesAT;
	
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

		bodyModel_->transform.rotation = Slerp(model_->transform.rotation, goalRotation, 0.2f);
	
	}
}

void Player::Action() {
	switch (behavior_) {
		case Behavior::GrabLantern:	
			if (Keyboard::GetTrigger(DIK_SPACE)) {
				behavior_ = Behavior::ReadyToThrow;
			}
			
			//左腕 :ランタン持っている腕
			leftUpperArmModel_->transform.rotation.y = -2.0f;
			leftUpperArmModel_->transform.translation.x = 0.04f;
			leftUpperArmModel_->transform.translation.y = -0.02f;

			//右腕
			rightUpperArmModel_->transform.translation.x = 0.1f;
			rightUpperArmModel_->transform.translation.y = 0.04f;
			rightUpperArmModel_->transform.rotation.z = 0.6f;

			break;

		case Behavior::ReadyToThrow:
			if (Keyboard::GetTrigger(DIK_C)) {
				behavior_ = Behavior::GrabLantern;
			}
			else if (Keyboard::GetRelease(DIK_SPACE)) {
				behavior_ = Behavior::Throwing;
				lantern_.Throw(model_->transform.rotation);
			}
			break;

		case Behavior::Throwing:
			behavior_ = Behavior::NoHave;
			break;

		case Behavior::NoHave:
			if (Keyboard::GetTrigger(DIK_SPACE)) {
				if ((model_->transform.translation - lantern_.GetWorldPosition()).Length() <= 0.3f) {
					behavior_ = Behavior::Throwing;
					lantern_.Grab(&grabPosition_);
					behavior_ = Behavior::GrabLantern;
				}
			}

			//左腕
			leftUpperArmModel_->transform.rotation.y = 0.0f;
			leftUpperArmModel_->transform.translation.x = 0.1f;
			leftUpperArmModel_->transform.translation.y = 0.04f;
			leftUpperArmModel_->transform.rotation.z = 0.6f;

			break;

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
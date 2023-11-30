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
	camera_->transform.rotation.x = 0.65f;
	cameraGoalRotation_ = camera_->transform.rotation;
	camera_->transform.translation = model_->transform.translation + (cameraOffset_ * Matrix4x4::CreateRotateXYZMatrix(cameraGoalRotation_));
	cameraGoalTranslation_ = camera_->transform.translation;

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

	InitializeFloatingGimmick();

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

	InitArmPosition();

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
			bool isGrab = !(behavior_ == Behavior::NoHave);
			bool result = stage->CheckCollision(checkPos[i], &fixVector, true, isGrab);
			// ヒットしていればフラグをtrueに
			if (result) {
				isHit = result;

				// もしY軸補正がされた場合
				if (fixVector.y > 0.0f) {
					// ある程度のY軸補正より大きい場合は無視する
					if (fixVector.y > 0.1f) {
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

	// プレイヤーが一定以上下に落ちたとき
	if (model_->transform.translation.y < -2.0f) {
		isFollowingCamera_ = false;
		if (model_->transform.translation.y < -6.0f) {
			model_->transform.translation = startPosition_;
			gravitiesAT = 0.0f;
			isFollowingCamera_ = true;
		}
	}

	// プレイヤーの座標をステージに保持させる
	stage->SetPlayerData(model_->transform.translation, model_->transform.rotation);

	// ランタン更新
	lantern_.Update(stage);

	// カメラを追従させる
	FollowCameraTurn();

	// 派生先用更新処理
	UpdateDerved(stage);
}

bool Player::IsRetry() {
	// プレイヤーが一定以上下に落ちたとき
	if (model_->transform.translation.y < -2.0f || lantern_.GetWorldPosition().y < -2.0f) {
		isFollowingCamera_ = false;
		if (model_->transform.translation.y < -4.0f || lantern_.GetWorldPosition().y < -4.0f) {
			return true;	// リトライを要求
		}
	}

	return false;
}

void Player::Move() {
	// 平行移動をするかのフラグ
	bool isParallelMoving = false;
	if (Keyboard::GetPress(DIK_LSHIFT) || Pad::GetPress(0, XBOX_LB)) {
		isParallelMoving = true;
	}

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
	dir.x += Pad::GetLStick(0).x;
	dir.z += Pad::GetLStick(0).y;

	// 平行移動のとき、Z成分を削除
	if (isParallelMoving) {
		dir.z = 0.0f;
	}

	// 長さが1を超えている場合 -> 正規化してから使用
	dir = dir.Length() > 1.0f ? dir.Normalize() : dir;
	Matrix4x4 cameraRotationMatrix = isParallelMoving ? Matrix4x4::CreateRotateXYZMatrix({ 0.0f, model_->transform.rotation.y, 0.0f }) : Matrix4x4::CreateRotateXYZMatrix({ 0.0f, camera_->transform.rotation.y, 0.0f });
	dir = dir * kPlayerSpeed * cameraRotationMatrix;
	
	// 構えているときは動かない
	if (behavior_ != Behavior::ReadyToThrow && behavior_ != Behavior::Throwing) {
		model_->transform.translation += dir;
		bodyModel_->transform.translation = model_->transform.translation;

		// 歩いないときの処理
		if (dir.Length() == 0.0f) {
			lantern_.WaitSwingAmplitude();
			isMove = false;
		}
		else {
			lantern_.MoveSwingAmplitude();
			isMove = true;
		}
	}

	// 重力を付与
	gravitiesAT += kGravities;
	model_->transform.translation.y += gravitiesAT;
	
	// 向きを設定
	if (dir.Length() > 0.0f) {

		UpAndDownMotion(2.5f);

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
		if (!isParallelMoving) {
			model_->transform.rotation = Slerp(model_->transform.rotation, goalRotation, 0.2f);
			bodyModel_->transform.rotation = Slerp(model_->transform.rotation, goalRotation, 0.2f);
		}
	}
}

void Player::Action() {
	switch (behavior_) {
		case Behavior::GrabLantern:	
			if (Keyboard::GetTrigger(DIK_SPACE) || Pad::GetTrigger(0, XBOX_RB)) {
				behavior_ = Behavior::ReadyToThrow;
			}

			UpAndDownMotion(1.5f);

			if (isMove == true) {
				UpdateMoveNoHaveArmAnimation();
			}

			//左腕
			rightUpperArmModel_->transform.translation.x = 0.1f;
			rightUpperArmModel_->transform.translation.y = 0.02f;
			rightUpperArmModel_->transform.rotation.z= 0.5f;

			//右腕 :ランタン持っている腕
			leftUpperArmModel_->transform.translation.x = -0.0f;
			leftUpperArmModel_->transform.translation.y = 0.0f;
			leftUpperArmModel_->transform.rotation.y = -1.5f;
		
			break;

		case Behavior::ReadyToThrow:
			if (Keyboard::GetTrigger(DIK_C) || Pad::GetTrigger(0, XBOX_B)) {
				behavior_ = Behavior::GrabLantern;
			}
			else if (Keyboard::GetRelease(DIK_SPACE) || Pad::GetRelease(0, XBOX_RB)) {
				behavior_ = Behavior::Throwing;
				lantern_.Throw(model_->transform.rotation);
			}

			bodyModel_->transform.rotation.x = -0.05f;
			bodyModel_->transform.translation.y = -0.03f;

			//左腕
			rightUpperArmModel_->transform.translation.x = -0.01f;
			rightUpperArmModel_->transform.rotation.y= 1.5f;

			//右腕 
			leftUpperArmModel_->transform.translation.x = 0.08f;
			leftUpperArmModel_->transform.translation.y = -0.01f;
			leftUpperArmModel_->transform.translation.z = 0.04f;

			leftUpperArmModel_->transform.rotation.x = -0.2f;
			leftUpperArmModel_->transform.rotation.y = 1.5f;
			leftUpperArmModel_->transform.rotation.z = -0.05f;

			break;

		case Behavior::Throwing:
			behavior_ = Behavior::NoHave;
			break;

		case Behavior::NoHave:
			// 自動で取得
			if ((model_->transform.translation - lantern_.GetWorldPosition()).Length() <= 0.3f) {
				behavior_ = Behavior::Throwing;
				lantern_.Grab(&grabPosition_);
				behavior_ = Behavior::GrabLantern;
			}


			if (isMove == true) {
				UpdateMoveNoHaveArmAnimation();
			}


			UpAndDownMotion(1.5f);

			//左腕
			rightUpperArmModel_->transform.translation.x = 0.1f;
			rightUpperArmModel_->transform.translation.y = 0.02f;
			rightUpperArmModel_->transform.rotation.z = 0.5f;

			//右腕 
			leftUpperArmModel_->transform.translation.x = -0.1f;
			leftUpperArmModel_->transform.translation.y = 0.02f;
			leftUpperArmModel_->transform.rotation.z = -0.5f;

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

	// カメラのズーム段階切り替え
	if (Keyboard::GetTrigger(DIK_PGUP) || Pad::GetTrigger(0, XBOX_DPAD_UP)) {
		cameraOffsetMultiply--;
	}
	if (Keyboard::GetTrigger(DIK_PGDN) || Pad::GetTrigger(0, XBOX_DPAD_DOWN)) {
		cameraOffsetMultiply++;
	}
	cameraOffsetMultiply = std::clamp<int>(cameraOffsetMultiply, 1, 3);
	cameraOffset_.z = (static_cast<float>(cameraOffsetMultiply) * -2.0f) - 2.0f;

	// 回転する向き
	Vector2 dir = { 0.0f,0.0f };

	// キーボードでの回転
	if (Keyboard::GetPress(DIK_UP)) {
		dir.x += 1.0f;
	}
	if (Keyboard::GetPress(DIK_DOWN)) {
		dir.x -= 1.0f;
	}
	if (Keyboard::GetPress(DIK_RIGHT)) {
		dir.y += 1.0f;
	}
	if (Keyboard::GetPress(DIK_LEFT)) {
		dir.y -= 1.0f;
	}

	// コントローラーでの回転
	dir.x += Pad::GetRStick(0).y;
	dir.y += Pad::GetRStick(0).x;

	// 長さが1を超えている場合 -> 正規化してから使用
	dir = dir.Length() > 1.0f ? dir.Normalize() : dir;
	// 次の目標回転角を計算
	cameraGoalRotation_ += Vector3{ dir.x* kFollowCameraSpeed, dir.y* kFollowCameraSpeed, 0.0f };
	// 角度制限
	cameraGoalRotation_.x = std::clamp<float>(cameraGoalRotation_.x, 0.1f, 1.3f);

	// カメラから追従対象に対する角度を求める
	cameraGoalTranslation_ = model_->transform.translation + (cameraOffset_ * Matrix4x4::CreateRotateXYZMatrix(cameraGoalRotation_));

	// 徐々に追従するように適応
	camera_->transform.translation = Lerp(camera_->transform.translation, cameraGoalTranslation_, 0.07f);
	camera_->transform.rotation = Lerp(camera_->transform.rotation, cameraGoalRotation_, 0.06f);
}

void Player::UpdateDerved(Stage* stage) {
	// 何もしない
	stage;
}

void Player::InitArmPosition()
{
	rightUpperArmModel_->transform.translation = { 0,0,0 };
	leftUpperArmModel_->transform.translation = { 0,0,0 };
	rightForeArmModel_->transform.translation = { 0,0,0 };
	leftForeArmModel_->transform.translation = { 0,0,0 };

	rightUpperArmModel_->transform.rotation = { 0,0,0 };
	leftUpperArmModel_->transform.rotation = { 0,0,0 };
	rightForeArmModel_->transform.rotation = { 0,0,0 };
	leftForeArmModel_->transform.rotation = { 0,0,0 };
}

void Player::InitializeFloatingGimmick() {

	//浮遊ギミックの媒介変数
	UpdownParameter_ = 0.0f;
	swingParameter_ = 0.0f;
	//浮遊移動のサイクル<frame>
	uint16_t cycle_ = 60;
	//浮遊の振動<m>
	amplitude_ = 0.01f;
	swing_ = 0.4f;
	throw_ = 0.1f;
}


void Player::UpAndDownMotion(float time)
{
	//1フレームでのパラメータ加算値
	const float step = time * 3.14f / cycle_;
	//パラメータを１ステップ分加算
	UpdownParameter_ += step;
	//2πを超えたら０に戻す
	UpdownParameter_ = std::fmod(UpdownParameter_, 2.0f * 3.14f);
	//浮遊を座標に反映
	bodyModel_->transform.translation.y = std::sin(UpdownParameter_) * amplitude_;
}

void Player::UpdateMoveNoHaveArmAnimation()
{

	//1フレームでのパラメータ加算値
	const float swinStep = 3.0f * 3.14f / cycle_;
	//パラメータを１ステップ分加算
	swingParameter_ += swinStep;
	//2πを超えたら０に戻す
	swingParameter_ = std::fmod(swingParameter_, 2.0f * 3.14f);

	rightUpperArmModel_->transform.rotation.y = std::sin(swingParameter_) * swing_;

	float leftArmOffset = (3.14f / 3.0f) * cycle_;  // 左腕を右腕よりも遅らせるためにオフセットを設定
	leftUpperArmModel_->transform.rotation.y = std::sin(swingParameter_ - leftArmOffset) * swing_;
}
#include "Lantern.h"
#include <random>

void Lantern::Init() {
	handleModel_ = LWP::Resource::LoadModel("Lantern/Lantern_Handle.obj");
	handleModel_->transform.scale = { 0.1f,0.1f,0.1f };
	handleModel_->name = "Handle";
	model_ = LWP::Resource::LoadModel("Lantern/Lantern.obj");
	model_->name = "Hontai";
	light_ = LWP::Object::CreateInstance<LWP::Object::PointLight>();
	light_->color = LWP::Utility::Color::KelvinToRGB(2300);
	light_->radius = 3.0f;
	light_->decay = 1.5f;
	light_->isActive = true;
	light_->name = "Lantern";

	// �e�q�֌W���Z�b�g
	model_->transform.Parent(&handleModel_->transform);
	light_->transform.Parent(&model_->transform);

	// ���C�g�̂ӂ�܂��̏�����
	LightBehaviorNormalInit();
	// �����^���̗h��̏����ݒ�
	WaitSwingAmplitude();
}

void Lantern::Update() {
	// �����^���̗h��鏈��
	SwingUpdate();

	// ���C�g�̂ӂ�܂�
	LightBehavior();

#if _DEBUG
	ImGui::Begin("Lantern");
	if (ImGui::TreeNode("Handle")) {
		handleModel_->DebugGUI();
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Hontai")) {
		model_->DebugGUI();
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Light")) {
		light_->DebugGUI();
		ImGui::TreePop();
	}
	ImGui::DragFloat("cycleSpeed", &hontaiRotateCycleFrame_, 1);
	ImGui::DragFloat("amplitude", &hontaiMaxAmplitude_, 0.01f);
	ImGui::DragFloat("factorX", &factorX_, 0.01f);
	ImGui::DragFloat("factorZ", &factorZ_, 0.01f);
	ImGui::End();
#endif
}

LWP::Object::WorldTransform* Lantern::Grab() {
	return &handleModel_->transform;
}

void Lantern::LightBehavior() {
	// ������
	if (behaviorRequest_) {
		//  �U��܂���ύX
		behavior_ = behaviorRequest_.value();
		switch (behavior_) {
		case Behavior::kNormal:
		default:
			LightBehaviorNormalInit();
			break;
			// �U���s��
		case Behavior::kFlicker:
			LightBehaviorFlickerInit();
			break;
		}
		// �U��܂����N�G�X�g�����Z�b�g
		behaviorRequest_ = std::nullopt;
	}

	// �X�V����
	switch (behavior_) {
	case Behavior::kNormal:
	default:
		LightBehaviorNormalUpdate();
		break;
		// �U���s��
	case Behavior::kFlicker:
		LightBehaviorFlickerUpdate();
		break;
	}
}

void Lantern::LightBehaviorNormalInit() {
	// ���̋�����߂�
	light_->intensity = 1.0f;
	// �U��
	lightAmplitude_ = 0;
	// ����
	intensityCycle_ = 0;
	// 1�������t���[��
	intensityCycleFrame_ = 60;

	// ����������̏�����
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());

	// ���݂̃t���[��
	lightNormal_.currentFrame = 0;
	// �I���t���[���𗐐��Ő���
	std::uniform_real_distribution<float> distribution(600.0f, 900.0f);
	lightNormal_.EndFrame = 60;//static_cast<int>(distribution(randomEngine));
}

void Lantern::LightBehaviorNormalUpdate() {
	if (lightNormal_.currentFrame >= lightNormal_.EndFrame) {
		// �_�łɈڍs
		behaviorRequest_ = Behavior::kFlicker;
	}

	// �U���̍ő�l&�ŏ��l
	const float kMaxAmplitude = 0.2f;
	// ����(2�b��1����)
	const float kCycleSpeed = M_PI / intensityCycleFrame_;
	intensityCycle_ += kCycleSpeed;

	//�U�����v�Z
	lightAmplitude_ = kMaxAmplitude * sinf(intensityCycle_);
	// light�Ɍv�Z���ʂ𔽉f
	light_->intensity = 1.0f - lightAmplitude_;

	lightNormal_.currentFrame++;
}

void Lantern::LightBehaviorFlickerInit() {
	// ���̋�����߂�
	light_->intensity = 1.0f;
	lightAmplitude_ = 0;
	intensityCycle_ = 0;
	// 1�������t���[��
	intensityCycleFrame_ = 16;

	// ����������̏�����
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());

	// �I���t���[���𗐐��Ő���
	std::uniform_int_distribution<> distribution(1,2);
	flickerCount_ = distribution(randomEngine);

	// ���݂̃t���[��
	lightFlicker_.currentFrame = 0;
	// ����_�ł����邩��ݒ�
	lightFlicker_.EndFrame = intensityCycleFrame_ / 2 * flickerCount_;
}

void Lantern::LightBehaviorFlickerUpdate() {
	if (lightFlicker_.currentFrame >= lightFlicker_.EndFrame) {
		// �ʏ��ԂɈڍs
		behaviorRequest_ = Behavior::kNormal;
	}

	// �U���̍ő�l&�ŏ��l
	const float kMaxAmplitude = 0.5f;
	// 8�t���[����1����
	const float kCycleSpeed = M_PI / (intensityCycleFrame_ / 2);
	intensityCycle_ += kCycleSpeed;

	//�U�����v�Z
	lightAmplitude_ = kMaxAmplitude * sinf(intensityCycle_);
	// light�Ɍv�Z���ʂ𔽉f
	light_->intensity = 1.0f - fabs(lightAmplitude_);

	lightFlicker_.currentFrame++;
}

void Lantern::SwingUpdate() {

	// ����(2�b��1����)
	const float kCycleSpeed = 2 * M_PI / hontaiRotateCycleFrame_;
	hontaiRotateCycle_ += kCycleSpeed;

	model_->transform.rotation.x = factorX_ * hontaiMaxAmplitude_ * sinf(hontaiRotateCycle_);
	model_->transform.rotation.z = factorZ_ * hontaiMaxAmplitude_ / 2 * sinf(hontaiRotateCycle_ + (M_PI / 2));
}

void Lantern::WaitSwingAmplitude() {
	// �U���̍ő�l&�ŏ��l
	hontaiMaxAmplitude_ = M_PI / 32.0f;
	hontaiRotateCycleFrame_ = 180;
	// �U���̌W��
	factorX_ = 1;
	factorZ_ = 1;
}

void Lantern::MoveSwingAmplitude() {
	// �U���̍ő�l&�ŏ��l
	hontaiMaxAmplitude_ = M_PI / 4.0f;
	hontaiRotateCycleFrame_ = 60;
	// �U���̌W��
	factorX_ = 1;
	factorZ_ = 1;
}
#pragma once
#include <Adapter.h>
#include <optional>

/// <summary>
/// �����^��
/// </summary>
class Lantern final {
public: // ** �����o�֐� ** //

	// ������
	void Init();
	// �X�V
	void Update();


	/// <summary>
	/// �����^����͂�
	/// </summary>
	/// <returns>�����^����Ǐ]������p�̃y�A�����g�|�C���^</returns>
	LWP::Object::WorldTransform* Grab();

	/// <summary>
	/// �����^���𓊂���
	/// </summary>
	/// <param name="direction">����������̃x�N�g���i���K���ς݂𑗂邱�Ɓj</param>
	void Throw(LWP::Math::Vector2 direction);

	// ���C�g�̐U��܂�
	void LightBehavior();

	// �ʏ펞�̏�����
	void LightBehaviorNormalInit();
	// �ʏ펞�̍X�V
	void LightBehaviorNormalUpdate();
	// ���̓_�ŏ�����
	void LightBehaviorFlickerInit();
	// ���̓_�ōX�V
	void LightBehaviorFlickerUpdate();

	// �����^���̗h��鋓��
	void SwingUpdate();
	// �����Ă��Ȃ��Ƃ��̗h�ꕝ
	void WaitSwingAmplitude();
	// �����Ă���Ƃ��̗h�ꕝ
	void MoveSwingAmplitude();

private: // ** �����o�ϐ� ** //

	// ���f��
	LWP::Primitive::Mesh* handleModel_;
	LWP::Primitive::Mesh* model_;
	// ����
	LWP::Object::PointLight* light_;

	// �͂܂�Ă���t���O
	bool isGrabed;

	// �U��܂�
	enum class Behavior {
		kNormal,	 // �ʏ���
		kFlicker // �����`�J�`�J���
	};
	Behavior behavior_ = Behavior::kNormal;
	// ���̐U��܂����N�G�X�g
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	// �e�ӂ�܂��ň�������
	struct BehaviorFrame {
		int currentFrame = 0;  // ���݂̃t���[��
		int EndFrame;		   // �I���t���[��
	};
	// �ʏ��Ԃ̃��C�g
	BehaviorFrame lightNormal_;
	// �_�Ŏ��̃��C�g
	BehaviorFrame lightFlicker_;

	// ���C�g�̓_�ł̐U��
	float lightAmplitude_;
	// ���C�g�̓_�ł̎���
	float intensityCycle_;
	// 1���������t���[���ōs����
	int intensityCycleFrame_;
	// ���C�g�̓_�ŉ�(�ʏ��Ԃł͎g�p���Ȃ�)
	int flickerCount_;

	// �����^���̗h�ꕝ
	float hontaiMaxAmplitude_;
	// �h��̎���
	float hontaiRotateCycle_;
	// 1��������܂ł̎���
	float hontaiRotateCycleFrame_;

	float factorX_;
	float factorZ_;
};
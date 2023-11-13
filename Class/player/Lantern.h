#pragma once
#include <Adapter.h>

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


private: // ** �����o�ϐ� ** //

	// ���f��
	LWP::Primitive::Mesh* handleModel_;
	LWP::Primitive::Mesh* model_;
	// ����
	LWP::Object::PointLight* light_;

	// �͂܂�Ă���t���O
	bool isGrabed;
};
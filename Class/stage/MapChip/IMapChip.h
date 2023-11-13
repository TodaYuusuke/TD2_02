#pragma once
#include <Adapter.h>

/// <summary>
/// �}�b�v�`�b�v1���̃N���X
/// </summary>
class IMapChip {
public:	// ** �����o�֐� ** //

	~IMapChip() = default;

	// ������
	virtual void Init(LWP::Math::Vector3 position) = 0;
	// �X�V
	virtual void Update() = 0;


protected: // ** �����o�ϐ� ** //

	// ���f��
	LWP::Primitive::Mesh* model_;
};

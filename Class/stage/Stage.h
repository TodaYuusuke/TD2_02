#pragma once
#include "MapChip/Derved.h"
#include <string>

class Stage final {
public:	// ** �����o�֐� ** //

	// �������i�ǂݍ��ރ��x���j
	void Init(int level);
	// �X�V
	void Update();


private: // ** �����o�ϐ� ** //

	// �}�b�v�`�b�v
	std::vector<std::vector<IMapChip*>> mapChip_;


private: // ** �v���C�x�[�g�Ȋ֐� ** //
};
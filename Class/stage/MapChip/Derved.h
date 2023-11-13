#pragma once
#include "IMapChip.h"

// �}�b�v�`�b�v���`
enum class Mapchip : int {
	Floor = 00,	// �����Ȃ��i���j
	Hole = 01,	// ���i����ʂ��A������j
	Wall = 02,	// �ǁi�����Ղ�j
	VineWall = 10,	// �ǂɂȂ�c�^
	VineFloor = 11,	// ����ɂȂ�c�^
	Start = 98,	// �X�^�[�g�n�_
	Candle = 99	// �낤�����i�N���A����	�j
};

class Floor :public IMapChip {
public: // ** �����o�֐� ** //
	void Init(LWP::Math::Vector3 position) override;
	void Update() override;
};
class Hole :public IMapChip {
public: // ** �����o�֐� ** //
	void Init(LWP::Math::Vector3 position) override;
	void Update() override;
};
class Wall :public IMapChip {
public: // ** �����o�֐� ** //
	void Init(LWP::Math::Vector3 position) override;
	void Update() override;
};
class VineWall :public IMapChip {
public: // ** �����o�֐� ** //
	void Init(LWP::Math::Vector3 position) override;
	void Update() override;
};
class VineFloor :public IMapChip {
public: // ** �����o�֐� ** //
	void Init(LWP::Math::Vector3 position) override;
	void Update() override;
};
class Candle :public IMapChip {
public: // ** �����o�֐� ** //
	void Init(LWP::Math::Vector3 position) override;
	void Update() override;
};
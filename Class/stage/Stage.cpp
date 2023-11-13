#include "Stage.h"
#include <fstream>
#include <sstream>

using namespace LWP::Math;

void Stage::Init(int level) {
	// ���x���̃X�e�[�W�f�[�^��ǂݍ���
	std::ifstream ifs("resources/stage/level" + std::to_string(level) + ".csv");
	std::string line;	// 1�s���̃f�[�^
	int y = 0;

	// �S�s�ǂނ܂Ń��[�v
	while (std::getline(ifs, line)) {
		// 1�񐶐�
		mapChip_.push_back(std::vector<IMapChip*>());
		// ','�ň��̏�����؂�A�P���C���X�^���X����
		std::istringstream stream(line);
		std::string field;
		while (std::getline(stream, field, ',')) {
			switch (static_cast<Mapchip>(std::atoi(field.c_str()))) {
				case Mapchip::Floor:
				default:
					mapChip_[y].push_back(new Floor());
					break;
				case Mapchip::Hole:
					mapChip_[y].push_back(new Hole());
					break;
				case Mapchip::Wall:
					mapChip_[y].push_back(new Wall());
					break;
				case Mapchip::VineWall:
					mapChip_[y].push_back(new VineWall());
					break;
				case Mapchip::VineFloor:
					mapChip_[y].push_back(new VineFloor());
					break;
				case Mapchip::Start:
					mapChip_[y].push_back(new Floor());
					break;
				case Mapchip::Candle:
					mapChip_[y].push_back(new Candle());
					break;
			}

			// ������
			Vector3 position = {
				static_cast<float>(mapChip_[y].size()),
				0.0f,
				static_cast<float>(y)
			};
			mapChip_[y].back()->Init(position);
		}
		// ������炷
		y++;
	}
}

void Stage::Update() {

}
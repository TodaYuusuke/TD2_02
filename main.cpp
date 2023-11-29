#include <Adapter.h>
#include "Class/sence/GameScene.h"
#include "Class/sence/StageSelectScene.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	LWP::System::Run(new StageSelectScene());
	return 0;
}
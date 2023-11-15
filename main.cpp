#include <Adapter.h>
#include "Class/sence/GameScene.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	LWP::System::Run(new GameScene(1));
	return 0;
}
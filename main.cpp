#include <Adapter.h>
#include "Class/sence/Title.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	LWP::System::Run(new Title(true));
	return 0;
}
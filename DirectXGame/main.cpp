#include "MyGame.h"

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	std::unique_ptr<MyGame> gameManager = std::make_unique<MyGame>();
	gameManager->Run();
	return 0;
}
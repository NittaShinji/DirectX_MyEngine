#include "GameManager.h"

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{	
	std::unique_ptr<GameManager> gameManager = std::make_unique<GameManager>();
	return 0;
}
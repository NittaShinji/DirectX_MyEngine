#include "MyGame.h"

//Windows�A�v���ł̃G���g���[�|�C���g(main�֐�)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{	
	std::unique_ptr<MyGame> gameManager = std::make_unique<MyGame>();
	gameManager->Run();
	return 0;
}
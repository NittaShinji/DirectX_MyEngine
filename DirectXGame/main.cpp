#include "GameManager.h"

//Windows�A�v���ł̃G���g���[�|�C���g(main�֐�)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{	
	std::unique_ptr<GameManager> gameManager = std::make_unique<GameManager>();
	return 0;
}
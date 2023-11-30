#include "GameTimer.h"
#include "TextureManager.h"

void GameTimer::Initialize()
{
	inGameTime_ = 0;
	resultTime_ = 0;
	highScoreTime_ = 0;
	LoadNumTexture();
	TextureManager::GetInstance()->LoadTexture("numbers.png");
	Vector2 texTotalSize = TextureManager::GetInstance()->GetTexSize("numbers.png");
	Vector2 texSize = texTotalSize;
	texSize.x = texSize.x / totalNumber;
	texSize.y = texSize.y;	

	//InGameInitialize();
	ResultInitialize();
}

void GameTimer::InGameInitialize()
{
	Vector2 texTotalSize = TextureManager::GetInstance()->GetTexSize("numbers.png");
	Vector2 texSize = texTotalSize;
	texSize.x = texSize.x / 10;
	for(int i = 0; i < inGameDigits; i++)
	{
		inGameNum[i] = std::make_unique<Sprite>();
		// 数字の初期化
		inGameNum[i]->Initialize("numbers.png", Vector2((i * texSize.x), texSize.y));
	}
}

void GameTimer::ResultInitialize()
{
	Vector2 texTotalSize = TextureManager::GetInstance()->GetTexSize("numbers.png");
	Vector2 texSize = texTotalSize;
	texSize.x = texSize.x / 10;
	for(int i = 0; i < resultDigits; i++)
	{
		resultNum[i] = std::make_unique<Sprite>();
		// 数字の初期化
		resultNum[i]->Initialize("numbers.png", Vector2((i * texSize.x), texSize.y));
	}

	for(int i = 0; i < resultDigits; i++)
	{
		resultNum[i]->SetTextureClipSize(texSize);
		resultNum[i]->SetSize(texSize);
	}
}

void GameTimer::Update(bool isStart)
{
	for(int i = 0; i < resultDigits; i++)
	{
		resultNum[i]->matUpdate();
	}

	if(isStart == true)
	{
		ResultNumberUpdate();
	}
	else
	{
		for(size_t i = 0; i < resultDigits; i++)
		{
			resultDisPlaytime[i] = { 0 };
			SetNumber(resultDisPlaytime[i], resultNum[i].get());
		}
	}
}

void GameTimer::Reset()
{
	inGameTime_ = 0;
	resultTime_ = 0;
	highScoreTime_ = 0;
}

void GameTimer::LoadNumTexture()
{
	TextureManager::GetInstance()->LoadTexture("number/0.png");
	TextureManager::GetInstance()->LoadTexture("number/1.png");
	TextureManager::GetInstance()->LoadTexture("number/2.png");
	TextureManager::GetInstance()->LoadTexture("number/3.png");
	TextureManager::GetInstance()->LoadTexture("number/4.png");
	TextureManager::GetInstance()->LoadTexture("number/5.png");
	TextureManager::GetInstance()->LoadTexture("number/6.png");
	TextureManager::GetInstance()->LoadTexture("number/7.png");
	TextureManager::GetInstance()->LoadTexture("number/8.png");
	TextureManager::GetInstance()->LoadTexture("number/9.png");
}

void GameTimer::NumberUpdate()
{
	for(int i = 0; i < resultDigits; i++)
	{
		//num[i]->SetTextureLeftTop(Vector2(texSize.x * i, texSize.y * i));
		//num[i]->SetTextureClipSize(texSize);
		//num[i]->SetSize(texSize);
	}
}

void GameTimer::Draw()
{
	for(int i = 0; i < resultDigits; i++)
	{
		resultNum[i]->Update();
	}
	for(int i = 0; i < resultDigits; i++)
	{
		resultNum[i]->Draw("numbers.png");
	}

	//resultNum[0]->Update();
	//resultNum[0]->Draw("numbers.png");

}

void GameTimer::InGameNumberUpdate()
{
	//t 経過時間	b最初の位置	c移動量	d移動時間
	if(inGameTime_ <= 1000)
	{
		inGameTime_++;
	}

	//--4桁目(千の位を表示)
	inGameDisPlayTime_[0] = (inGameTime_ % 10000) / 1000;
	//--3桁目(百の位を表示)
	inGameDisPlayTime_[1] = (inGameTime_ % 1000) / 100;
	//--2桁目(十の位を表示)
	inGameDisPlayTime_[2] = (inGameTime_ % 100) / 10;
	//--1桁目(1の位を表示)
	inGameDisPlayTime_[3] = (inGameTime_ % 10) / 1;

	for(size_t i = 0; i < inGameDigits; i++)
	{
		SetNumber(inGameDisPlayTime_[i], inGameNum[i].get());
	}
}

void GameTimer::ResultNumberUpdate()
{
	//t 経過時間	b最初の位置	c移動量	d移動時間
	if(resultTime_ <= 1000)
	{
		resultTime_++;
	}

	//--7桁目(10万の位を表示)
	resultDisPlaytime[0] = (resultTime_ % 10000000) / 1000000;
	//--6桁目(万の位を表示)
	resultDisPlaytime[1] = (resultTime_ % 1000000) / 100000;
	//--5桁目(千の位を表示)
	resultDisPlaytime[2] = (resultTime_ % 100000) / 10000;
	//--4桁目(百の位を表示)
	resultDisPlaytime[3] = (resultTime_ % 10000) / 1000;
	//--3桁目(十の位を表示)
	resultDisPlaytime[4] = (resultTime_ % 1000) / 100;
	//--2桁目(1の位を表示)
	resultDisPlaytime[5] = (resultTime_ % 100) / 10;
	//--1桁目(1の位を表示)
	resultDisPlaytime[6] = (resultTime_ % 10) / 1;

	for(size_t i = 0; i < resultDigits; i++)
	{
		SetNumber(resultDisPlaytime[i], resultNum[i].get());
	}
	
	/*Vector2 texTotalSize = TextureManager::GetInstance()->GetTexSize("numbers.png");
	Vector2 texSize = texTotalSize;
	texSize.x = texSize.x / totalNumber;

	if(resultDisPlaytime[6] == 0)
	{
		resultNum[0]->SetTextureLeftTop(texSize * 0);
	}
	else if(resultDisPlaytime[6] == 1)
	{
		resultNum[0]->SetTextureLeftTop(texSize * 1);
	}
	else if(resultDisPlaytime[6] == 2)
	{
		resultNum[0]->SetTextureLeftTop(texSize * 2);
	}
	else if(resultDisPlaytime[6] == 3)
	{
		resultNum[0]->SetTextureLeftTop(texSize * 3);
	}
	else if(resultDisPlaytime[6] == 4)
	{
		resultNum[0]->SetTextureLeftTop(texSize * 4);
	}
	else if(resultDisPlaytime[6] == 5)
	{
		resultNum[0]->SetTextureLeftTop(texSize * 5);
	}
	else if(resultDisPlaytime[6] == 6)
	{
		resultNum[0]->SetTextureLeftTop(texSize * 6);
	}
	else if(resultDisPlaytime[6] == 7)
	{
		resultNum[0]->SetTextureLeftTop(texSize * 7);
	}
	else if(resultDisPlaytime[6] == 8)
	{
		resultNum[0]->SetTextureLeftTop(texSize * 8);
	}
	else if(resultDisPlaytime[6] == 9)
	{
		resultNum[0]->SetTextureLeftTop(texSize * 9);
	}*/
}

void GameTimer::SetNumber(int number, Sprite* sprite)
{
	Vector2 texTotalSize = TextureManager::GetInstance()->GetTexSize("numbers.png");
	Vector2 texSize = texTotalSize;
	//一枚あたりの横サイズ
	texSize.x = texSize.x / totalNumber;

	if(number == 0)
	{
		sprite->SetTextureLeftTop(texSize * 0);
	}
	else if(number == 1)
	{
		sprite->SetTextureLeftTop(texSize * 1);
	}
	else if(number == 2)
	{
		sprite->SetTextureLeftTop(texSize * 2);
	}
	else if(number == 3)
	{
		sprite->SetTextureLeftTop(texSize * 3);
	}
	else if(number == 4)
	{
		sprite->SetTextureLeftTop(texSize * 4);
	}
	else if(number == 5)
	{
		sprite->SetTextureLeftTop(texSize * 5);
	}
	else if(number == 6)
	{
		sprite->SetTextureLeftTop(texSize * 6);
	}
	else if(number == 7)
	{
		sprite->SetTextureLeftTop(texSize * 7);
	}
	else if(number == 8)
	{
		sprite->SetTextureLeftTop(texSize * 8);
	}
	else if(number == 9)
	{
		sprite->SetTextureLeftTop(texSize * 9);
	}
}

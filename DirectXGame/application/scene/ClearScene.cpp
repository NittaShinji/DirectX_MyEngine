#include "ClearScene.h"
#include "SceneManager.h"

void ClearScene::Initialize()
{
	end_ = std::make_unique<Sprite>();
	SpriteCommon::GetInstance()->LoadTexture("end.png");
	XMFLOAT2 titlePosition = { 0,0 };
	XMFLOAT2 titleSize = { 1280,720 };
	end_->Initialize(titlePosition, titleSize);
	//シェーダー読み込み
	SpriteCommon::GetInstance()->ShaderLoad();
	SpriteCommon::GetInstance()->SemiTransparent();
}

void ClearScene::Update()
{
	//アンカーポイントの設定
	XMFLOAT2 anchorPoint = { 0.0f,0.0f };
	end_->SetAnchorPoint(anchorPoint);
	end_->matUpdate();

	if(keys_->ReleasedKeyMoment(DIK_RETURN))
	{
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}
}

void ClearScene::Draw()
{
	SpriteCommon::GetInstance()->BeforeDraw();
	SpriteCommon::GetInstance()->Update();
	end_->Draw("end.png");
}

#include "GameScene.h"
#include <string>

using namespace std;
using namespace DirectX;

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	delete object3d_;
	delete nObject3d_;
	delete title_;
	delete spriteCommon_;
}

void GameScene::Initialize(DirectXBasic* directXBasic)
{
	directXBasic_ = directXBasic;

	//------------画像読み込み----------
	title_ = new Sprite;
	spriteCommon_ = new SpriteCommon;
	//スプライト関係初期化
	spriteCommon_->Initialize(directXBasic_);
	Sprite::StaticInitialize(spriteCommon_);
	//画像ロード
	title_->LoadTexture(0, "reimu.png");
	//個々の画像を初期化
	XMFLOAT2 titlePosition = { 400,400 };
	XMFLOAT2 titleSize = { 100,100 };
	title_->Initialize(titlePosition, titleSize);
	//シェーダー読み込み
	spriteCommon_->ShaderLoad();
	//スプライト用のパイプラインステートを用意
	spriteCommon_->SemiTransparent();

	//------------モデル読み込み----------
	Model::StaticInitialize(directXBasic_);
	Object3d::StaticInitialize(directXBasic_);

	//モデル読み込み
	const string testModelName = "triangle_tex";
	const string testModelName2 = "triangle_tex2";
	const string testModelName3 = "pillar";

	Model::Load(testModelName);
	Model::Load(testModelName2);
	Model::Load(testModelName3);

	XMFLOAT3 position = { 0,0,0 };
	XMFLOAT3 position2 = { 30,0,0 };
	XMFLOAT3 position3 = { -30,0,0 };

	//3Dオブジェクト
	object3d_ = new Object3d(testModelName, position);
	nObject3d_ = new Object3d(testModelName2, position2);
	sObject3d_ = new Object3d(testModelName3, position3);
}

void GameScene::Update()
{

	XMFLOAT2 anchorPoint = title_->GetAnchorPoint();
	anchorPoint = { 0.0f,0.0f };
	title_->SetAnchorPoint(anchorPoint);
	title_->matUpdate();

	object3d_->Update();
	nObject3d_->Update();
	sObject3d_->Update();

}

void GameScene::Draw()
{
	//モデル読み込み
	object3d_->BeforeDraw();
	object3d_->Draw();
	nObject3d_->Draw();
	sObject3d_->Draw();

	//画像読み込み
	spriteCommon_->BeforeDraw();
	spriteCommon_->Update();
	title_->Draw();

}

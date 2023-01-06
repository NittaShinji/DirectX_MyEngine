#include "GameScene.h"
#include <string>

using namespace std;
using namespace DirectX;

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	delete object3d;
	delete nObject3d;
}

void GameScene::Initialize(DirectXBasic* directXBasic)
{
	directXBasic_ = directXBasic;

	Model::StaticInitialize(directXBasic_);
	Object3d::StaticInitialize(directXBasic_);

	//モデル読み込み
	const string testModelName = "triangle_tex";
	const string testModelName2 = "triangle_tex2";

	Model::Load(testModelName);
	Model::Load(testModelName2);

	//testModel.Load(testModelName);
	
	XMFLOAT3 position = { 0,0,0 };
	XMFLOAT3 position2 = { 30,0,0 };

	//3Dオブジェクト
	object3d = new Object3d(testModelName, position);
	nObject3d = new Object3d(testModelName2, position2);

	//object3d->SetModel(testModelName);
	/*testModel->SetName(testModelName);
	testModel->SetInfomation(*Model::GetMODELVALUE(testModelName));*/


}

void GameScene::Update()
{
	object3d->Update();
	nObject3d->Update();
}

void GameScene::Draw()
{
	object3d->BeforeDraw();
	object3d->Draw();
	nObject3d->Draw();
}

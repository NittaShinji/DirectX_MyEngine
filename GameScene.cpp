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
	//testModel->Load(testModelName);
	
	XMFLOAT3 position = { 0,0,0 };
	XMFLOAT3 position2 = { 30,0,0 };

	//3Dオブジェクト
	//object3d->SetModel(testModelName);
	object3d = new Object3d(testModelName, position);
	/*testModel->SetName(testModelName);
	testModel->SetInfomation(*Model::GetMODELVALUE(testModelName));*/

	//nObject3d = new Object3d("triangle_tex2", directXBasic_, position2);

}

void GameScene::Update()
{
	object3d->Update();
	//nObject3d->Update();

}

void GameScene::Draw()
{
	object3d->BeforeDraw();
	object3d->Draw();
	//nObject3d->Draw();
}

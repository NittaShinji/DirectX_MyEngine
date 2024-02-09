#include "BaseScene.h"
using namespace NsEngine;

DirectXBasic* BaseScene::directXBasic_ = nullptr;
ImGuiManager* BaseScene::imGuiManager_ = nullptr;

void BaseScene::StaticInitialize(DirectXBasic* directXBasic, ImGuiManager* imGuiManager)
{
	directXBasic_ = directXBasic;
	imGuiManager_ = imGuiManager;
}

void BaseScene::Initialize(){}

void BaseScene::Update(){}

void BaseScene::Finalize(){}

void BaseScene::Draw(){}

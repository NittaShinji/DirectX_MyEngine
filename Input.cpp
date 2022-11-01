#include <cstdint>
#include <wrl.h>
#include <memory>
#include "Input.h"
using namespace Microsoft::WRL;

void Input::Initialize(WindowsAPI* winApi)
{
	KeyInput::Create();
	//‰Šú‰»
	keyInput_ = KeyInput::GetInstance();
	keyInput_->Initialize(winApi);
}

void Input::Update()
{
	keyInput_->SaveFrameKey();
}

void Input::Delete()
{
	keyInput_->destroy();
}

Input::Input(){};
Input::~Input() {};


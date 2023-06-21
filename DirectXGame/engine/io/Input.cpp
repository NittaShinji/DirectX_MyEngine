#include <cstdint>
#include <wrl.h>
#include <memory>
#include "Input.h"
using namespace Microsoft::WRL;

void Input::Initialize(WindowsAPI* winApi)
{
	//‰Šú‰»
	KeyInput::GetInstance();
	KeyInput::GetInstance()->Initialize(winApi);
}

void Input::Update()
{
	KeyInput::KeyUpdate();
}

Input::Input(){};
Input::~Input() {};


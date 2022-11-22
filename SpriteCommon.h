#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <DirectXMath.h>
#include "DirectXBasic.h"
using namespace DirectX;

class SpriteCommon
{
public:
	
	//������
	void Initialize(DirectXBasic* directXBasic);

	//�Q�b�^�[
	DirectXBasic* GetDirectXBasic(){ return directXBasic_; };

private:

	DirectXBasic* directXBasic_ = nullptr;

	//// ���_�f�[�^�S�̂̃T�C�Y
	//UINT sizeVB;

	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_ = nullptr;
};
#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <DirectXMath.h>
#include "SpriteCommon.h"
using namespace DirectX;

//�X�v���C�g
class Sprite
{
public:

	//������
	void Initialize(SpriteCommon* spriteCommon);

private:

	SpriteCommon* spriteCommon_ = nullptr;
	DirectXBasic* directXBasic_ = nullptr;

	struct Vertex
	{
		XMFLOAT3 pos;		//xyz���W
		XMFLOAT3 normal;	//�@���x�N�g��
		XMFLOAT2 uv;		//�摜����xyz���W
	};

	HRESULT result_;
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_ = nullptr;

	// ���_�f�[�^�S�̂̃T�C�Y
	UINT sizeVB;

};

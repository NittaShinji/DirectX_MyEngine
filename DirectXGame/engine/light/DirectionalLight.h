#pragma once
#include <DirectXMath.h>
#include <wrl.h>
#include <d3d12.h>
#include "Vector3.h"

class DirectionalLight
{
private: //�G�C���A�X�e���v���[�g

	//Microsoft::WRL::���ȗ�
	template<class T>using Comptr = Microsoft::WRL::ComPtr<T>;
	//DirectX���ȗ�
	//using Vector2 = DirectX::Vector2;
	//using Vector3 = DirectX::Vector3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: //�T�u�N���X

	//�萔�o�b�t�@�p�f�[�^�\����
	struct  ConstBufferData
	{
		XMVECTOR lightv;		//���C�g�֕�����\���x�N�g��
		Vector3 lightColor;	//���C�g�̐F
		bool active;	//�L���t���O
	};

public: //�A�N�Z�b�T

	//���C�g�������Z�b�g
	void SetLightDir(const XMVECTOR& lightDir) 
	{
		//���K�����ăZ�b�g
		this->lightDir_ = DirectX::XMVector3Normalize(lightDir);
		dirty_ = true;
	}

	//���C�g�F�̃Z�b�g
	void SetLightColor(const Vector3& lightColor)
	{
		this->lightColor_ = lightColor;
		dirty_ = true;
	}

	XMVECTOR GetLightDir() { return lightDir_; };
	Vector3 GetLightColor() { return lightColor_; };

	/// <summary>
	/// �L���t���O���Z�b�g
	/// </summary>
	/// <param name="active">�L���t���O</param>
	inline void SetActive(bool active) { this->active_ = active; }

	/// <summary>
	/// �L���`�F�b�N
	/// </summary>
	/// <returns>�L���t���O</returns>
	inline bool IsActive() { return active_; }

private: //�����o�ϐ�

	//�萔�o�b�t�@
	Comptr<ID3D12Resource> constBuff_;
	//���C�g��������(�P�ʃx�N�g��)
	XMVECTOR lightDir_ = { 1,0,0,0};
	//���C�g�F
	Vector3 lightColor_ = { 1,1,1 };
	//�L���t���O
	bool active_ = false;
	//�_�[�e�B�t���O(�l�ɕύX���������Ƃ������萔�o�b�t�@�ɓ]��)
	bool dirty_ = false;
};

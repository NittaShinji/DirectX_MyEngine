#pragma once
#include <DirectXMath.h>
#include <wrl.h>
#include <d3d12.h>

class DirectionalLight
{

private:
	//Microsoft::WRL::���ȗ�
	template<class T>using Comptr = Microsoft::WRL::ComPtr<T>;
	//DirectX���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: //�T�u�N���X

	//�萔�o�b�t�@�p�f�[�^�\����
	struct  ConstBufferData
	{
		XMVECTOR lightv;		//���C�g�֕�����\���x�N�g��
		XMFLOAT3 lightColor;	//���C�g�̐F
		bool active;	//�L���t���O
	};

public: //�����o�֐�

	//���C�g�������Z�b�g
	void SetLightDir(const XMVECTOR& lightDir){	this->lightDir = lightDir;}
	//���C�g�F�̃Z�b�g
	void SetLightColor(const XMFLOAT3& lightColor){	this->lightColor = lightColor; }

	XMVECTOR GetLightDir() { return lightDir; };
	XMFLOAT3 GetLightColor() { return lightColor; };


	/// <summary>
	/// �L���t���O���Z�b�g
	/// </summary>
	/// <param name="active">�L���t���O</param>
	inline void SetActive(bool active) { this->active = active; }

	/// <summary>
	/// �L���`�F�b�N
	/// </summary>
	/// <returns>�L���t���O</returns>
	inline bool IsActive() { return active; }


private: //�����o�ϐ�

	//�萔�o�b�t�@
	Comptr<ID3D12Resource> constBuff;
	//���C�g��������(�P�ʃx�N�g��)
	XMVECTOR lightDir = { 1,0,0,0};
	//���C�g�F
	XMFLOAT3 lightColor = { 1,1,1 };
	//�L���t���O
	bool active = false;
	//�_�[�e�B�t���O
	bool dirty = false;
	
};

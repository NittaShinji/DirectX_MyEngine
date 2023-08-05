#pragma once
#include <DirectXMath.h>
#include <wrl.h>
#include <d3d12.h>
#include "MathUtillity.h"

using namespace MathUtillty;

class DirectionalLight
{
private: //�G�C���A�X�e���v���[�g

	//Microsoft::WRL::���ȗ�
	template<class T>using Comptr = Microsoft::WRL::ComPtr<T>;
	//DirectX���ȗ�

public: //�T�u�N���X

	//�萔�o�b�t�@�p�f�[�^�\����
	struct  ConstBufferData
	{
		Vector3 lightv;		//���C�g�֕�����\���x�N�g��
		float pad1;
		Vector3 lightColor;	//���C�g�̐F
		bool active;	//�L���t���O
	};

public: //�A�N�Z�b�T

	//���C�g�������Z�b�g
	void SetLightDir(const Vector3& lightDir,const float upVec)
	{
		//���K�����ăZ�b�g
		this->lightDir_ = Vector3Normalize(lightDir);
		this->upVec_ = upVec_;
		dirty_ = true;
	}

	//���C�g�F�̃Z�b�g
	void SetLightColor(const Vector3& lightColor)
	{
		this->lightColor_ = lightColor;
		dirty_ = true;
	}

	Vector3 GetLightDir() { return lightDir_; };
	float GetUpVec() { return upVec_; }

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
	Vector3 lightDir_ = {1,0,0};
	float upVec_ = 0.0f;
	//���C�g�F
	Vector3 lightColor_ = { 1,1,1 };
	//�L���t���O
	bool active_ = false;
	//�_�[�e�B�t���O(�l�ɕύX���������Ƃ������萔�o�b�t�@�ɓ]��)
	bool dirty_ = false;
};

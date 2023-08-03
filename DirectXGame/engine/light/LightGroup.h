
#pragma once
#include "DirectionalLight.h"
#include <DirectXMath.h>
#include <wrl.h>
#include <d3d12.h>
#include "Vector2.h"
#include "Vector3.h"

///<summary>
/// ���C�g
///<summary>
class LightGroup
{

private: //�G�C���A�X

	//Microsoft::WRL::���ȗ�
	template <class T> using Comptr = Microsoft::WRL::ComPtr<T>;
	//DirectX::���ȗ�
	//using Vector2 = DirectX::Vector2;
	//using Vector3 = DirectX::Vector3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;


public: //�ÓI�����o�ϐ�

	/// <summary>
	/// �ÓI������
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	static void StaticInitialize(ID3D12Device *device);

public: //�����o�֐�

	
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();
	
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ID3D12GraphicsCommandList *cmdList, UINT rootParameterIndex);

	/// <summary>
	/// �C���X�^���X����
	/// </summary>
	/// <retums>�C���X�^���X</retums>
	static LightGroup* Create();

	/// <summary>
	/// �萔�o�b�t�@�]��
	/// </summary>
	void TransferConstBuffer();

	/// <summary>
	/// �����̃��C�g�F���Z�b�g
	/// </summary>
	/// <param name="color">���C�g�F</param>
	void SetAmbientColor(const Vector3& color);

	/// <summary>
	/// ���s�����̗L���t���O���Z�b�g
	/// </summary>
	///  <param name="index">���C�g�ԍ�</param>
	///  <param name="active">�L���t���O</param>
	void SetDirLightActive(int32_t index, bool active);

	/// <summary>
	/// ���s�����̃��C�g���Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightDir">���C�g����</param>
	void SetDirLightDir(int32_t index, const XMVECTOR&lightDir);

	/// <summary>
	/// ���s�����̃��C�g�F���Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightColor">���C�g�F</param>
	void SetDirLightColor(int32_t index, const Vector3&lightColor);

	/// <summary>
	/// �W���̃��C�g�̐ݒ�
	/// </summary>
	void DefaultLightSetting();

	void SetPointLightActive(int32_t index, bool active);
	void SetPointLightPos(int32_t index, const Vector3 &lightPos);
	void SetPointLightColor(int32_t index, const Vector3 &lightColor);
	void SetPointLightAtten(int32_t index, const Vector3 &lightAtten);

	void SetSpotLightActive(int32_t index, bool active);
	void SetSpotLightDir(int32_t index, const XMVECTOR &lightDir);
	void SetSpotLightPos(int32_t index, const Vector3 &lightPos);
	void SetSpotLightColor(int32_t index, const Vector3 &lightColor);
	void SetSpotLightAtten(int32_t index, const Vector3 &lightAtten);
	void SetSpotLightFactorAngle(int32_t index, const Vector2 &lightFactorAngle);

	void SetCircleShadowActive(int32_t index, bool active);
	void SetCircleShadowCasterPos(int32_t index, const Vector3& casterPos);
	void SetCircleShadowDir(int32_t index, const XMVECTOR& lightDir);
	void SetCircleShadowDistanceCasterLight(int32_t index, float &distanceCasterLight);
	void SetCircleShadowAtten(int32_t index, const Vector3& lightAtten);
	void SetCircleShadowFactorAngle(int32_t index, const Vector2& lightFactorAngle);


public: //�萔
	//���C�g�̐�
	static const int32_t kDirLightNum_ = 3;
	//�_�����̐�
	static const int32_t kPointLightNum_ = 3;
	//�X�|�b�g���C�g�̐�
	static const int32_t kSpotLightNum_ = 3;
	//�ۉe�̐�
	static const int32_t kCircleShadowNum_ = 1;


public: //�T�u�N���X

	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		//�����̐F
		Vector3 ambientColor;
		float pad1;
		//���s�����p
		DirectionalLight::ConstBufferData dirLights[kDirLightNum_];
		//�_�����p
		//PointLight::ConstBufferData pointLights[PointLightNum];
		////�X�|�b�g���C�g�p
		//SpotLight::ConstBufferData spotLights[SpotLightNum];
		////�ۉe�p
		//CircleShadow::ConstBufferData circleShadows[CircleShadowNum];
	};


private: //�ÓI�����o�ϐ�

	//�f�o�C�X
	static ID3D12Device *device_;

private: //�����o�ϐ�

	//�萔�o�b�t�@
	Comptr<ID3D12Resource> constBuff_;
	//�����̐F
	Vector3 ambientColor_ = { 1,1,1};
	//���s�����̔z��
	DirectionalLight dirLights_[kDirLightNum_];
	//�_�����̔z��
	//PointLight pointLights[PointLightNum];
	////�X�|�b�g���C�g�z��
	//SpotLight spotLights[SpotLightNum];
	////�ۉe�̔z��
	//CircleShadow circleShadows[CircleShadowNum];
	//�_�[�e�B�t���O
	bool dirty_ = false;

};



#pragma once
#include "DirectionalLight.h"
//#include "PointLight.h"
//#include "SpotLight.h"
//#include "CircleShadow.h"
#include <DirectXMath.h>
#include <wrl.h>
#include <d3d12.h>

///<summary>
/// ���C�g
///<summary>
class LightGroup
{

private: //�G�C���A�X

	//Microsoft::WRL::���ȗ�
	template <class T> using Comptr = Microsoft::WRL::ComPtr<T>;
	//DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
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
	void SetAmbientColor(const XMFLOAT3& color);

	/// <summary>
	/// ���s�����̗L���t���O���Z�b�g
	/// </summary>
	///  <param name="index">���C�g�ԍ�</param>
	///  <param name="active">�L���t���O</param>
	void SetDirLightActive(int index, bool active);

	/// <summary>
	/// ���s�����̃��C�g���Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightDir">���C�g����</param>
	void SetDirLightDir(int index, const XMVECTOR &lightDir);

	/// <summary>
	/// ���s�����̃��C�g�F���Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightColor">���C�g�F</param>
	void SetDirLightColor(int index, const XMFLOAT3 &lightColor);

	/// <summary>
	/// �W���̃��C�g�̐ݒ�
	/// </summary>
	void DefaultLightSetting();

	void SetPointLightActive(int index, bool active);
	void SetPointLightPos(int index, const XMFLOAT3 &lightPos);
	void SetPointLightColor(int index, const XMFLOAT3 &lightColor);
	void SetPointLightAtten(int index, const XMFLOAT3 &lightAtten);

	void SetSpotLightActive(int index, bool active);
	void SetSpotLightDir(int index, const XMVECTOR &lightDir);
	void SetSpotLightPos(int index, const XMFLOAT3 &lightPos);
	void SetSpotLightColor(int index, const XMFLOAT3 &lightColor);
	void SetSpotLightAtten(int index, const XMFLOAT3 &lightAtten);
	void SetSpotLightFactorAngle(int index, const XMFLOAT2 &lightFactorAngle);

	void SetCircleShadowActive(int index, bool active);
	void SetCircleShadowCasterPos(int index, const XMFLOAT3 &casterPos);
	void SetCircleShadowDir(int index, const XMVECTOR &lightDir);
	void SetCircleShadowDistanceCasterLight(int index, float &distanceCasterLight);
	void SetCircleShadowAtten(int index, const XMFLOAT3 &lightAtten);
	void SetCircleShadowFactorAngle(int index, const XMFLOAT2 &lightFactorAngle);


public: //�萔
	//���C�g�̐�
	static const int DirLightNum = 3;
	//�_�����̐�
	static const int PointLightNum = 3;
	//�X�|�b�g���C�g�̐�
	static const int SpotLightNum = 3;
	//�ۉe�̐�
	static const int CircleShadowNum = 1;


public: //�T�u�N���X

	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		//�����̐F
		XMFLOAT3 ambientColor;
		float pad1;
		//���s�����p
		DirectionalLight::ConstBufferData dirLights[DirLightNum];
		//�_�����p
		//PointLight::ConstBufferData pointLights[PointLightNum];
		////�X�|�b�g���C�g�p
		//SpotLight::ConstBufferData spotLights[SpotLightNum];
		////�ۉe�p
		//CircleShadow::ConstBufferData circleShadows[CircleShadowNum];
	};


private: //�ÓI�����o�ϐ�

	//�f�o�C�X
	static ID3D12Device *device;

private: //�����o�ϐ�

	//�萔�o�b�t�@
	Comptr<ID3D12Resource> constBuff_;
	//�����̐F
	XMFLOAT3 ambientColor_ = { 1,1,1};
	//���s�����̔z��
	DirectionalLight dirLights_[DirLightNum];
	//�_�����̔z��
	//PointLight pointLights[PointLightNum];
	////�X�|�b�g���C�g�z��
	//SpotLight spotLights[SpotLightNum];
	////�ۉe�̔z��
	//CircleShadow circleShadows[CircleShadowNum];
	//�_�[�e�B�t���O
	bool dirty_ = false;

};


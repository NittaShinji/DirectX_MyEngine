#pragma once
#include "DirectXBasic.h"
#include "Model.h"
#include "Sprite.h"
#include "Camera.h"
#include "Input.h"
#include "LightGroup.h"
#include "CollisionInfo.h"
#include <vector>
#include <string>

class BaseCollider;

class Object3d
{
private:

	//nameSpace
	using XMFLOAT = DirectX::XMFLOAT3;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMMATRIX = DirectX::XMMATRIX;

	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:

	//�萔�o�b�t�@�p�f�[�^�\����(���W�n)b0
	struct ConstBufferDateTransform
	{
		XMMATRIX viewProjection;
		//���[���h�s��
		XMMATRIX worldMatrix;
		//�J�������W(���[���h�s��)
		XMFLOAT3 cameraPos;
		float pad1;
	};

	//�萔�o�b�t�@�p�f�[�^�\����(�}�e���A��)b1
	struct ConstBufferDataMaterial
	{
		XMFLOAT3 ambient;	//�A���r�G���g�W��
		float pad1;
		XMFLOAT3 diffuse;
		float pad2;
		XMFLOAT3 specular;
		float alpha;
	};

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Object3d() = default;

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~Object3d();

	static std::unique_ptr<Object3d> Create(const std::string& path);

	static void StaticInitialize(DirectXBasic* directXBasic);
	virtual void Initialize();
	virtual void Update(Camera* camera);
	static void BeforeDraw();

	void AfterDraw();
	virtual void Draw();
	void SetModel(const std::string& path);

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	/// <param name="info">�Փˏ��</param>
	virtual void OnCollision(const CollisionInfo& info) {}

protected:	//�����o�ϐ�

	//�N���X��(�f�o�b�O�p)
	const char* name = nullptr;
	//�R���C�_�[
	BaseCollider* collider = nullptr;

	static DirectXBasic* directXBasic_;
	static KeyInput* keys_;
	static LightGroup* lightGroup_;
	Model model_;
	Camera* camera_ = nullptr;

	//�萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform_;
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial_;

	//�萔�o�b�t�@�̃}�b�s���O�p�|�C���^
	ConstBufferDateTransform* constMapTransform_ = nullptr;
	ConstBufferDataMaterial* constMapMaterial_ = nullptr;

	//�X�P�[��
	XMFLOAT3 scale_;
	//��]�p
	XMFLOAT3 rotation_;
	//���s�ړ�
	XMFLOAT3 transform_;

	XMMATRIX matScale_, matRot_, matTrans_;

	//���[���h�ϊ��s��
	XMMATRIX matWorld_;

	//�r���[�s��
	XMMATRIX matView_;
	//�ˉe�s��
	XMMATRIX matProjection_;
	//�J�������W
	XMFLOAT3 cameraPos_;

	//�e�I�u�W�F�N�g�̃|�C���^

	ID3DBlob* vsBlob_ = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
	ID3DBlob* psBlob_ = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ID3DBlob* errorBlob_ = nullptr; // �G���[�I�u�W�F�N�g

	static Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState_;
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;

	//�O���t�B�b�N�X�p�C�v���C��
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc_{};

	//SRV�̍ő��
	static const size_t kMaxSRVCount_ = 2056;

	//�e�N�X�`���o�b�t�@
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, kMaxSRVCount_> textureBuffers_;

	//�f�t�H���g�e�N�X�`���i�[�f�B���N�g��
	static std::string kDefaultTextureDirectoryPath_;

	bool colorFlag_ = false;

	static const UINT kRenderTexNum = 2;

	XMFLOAT3 color_ = { 1,1,1};

	//����
	int32_t attribute_;

public:

	//�Q�b�^�[
	ConstBufferDateTransform* GetConstMapTransform() { return constMapTransform_; };

	XMFLOAT3 GetWorldPos() const;
	/// <summary>
	/// ���[���h�s��̎擾
	/// </summary>
	/// <returns>���[���h�s��</returns>
	const XMMATRIX& GetMatWorld() { return matWorld_; }

	//���f�����擾
	//const Model& GetModel() { return model_; }
	Model* GetModel() { return &model_; }
	bool GetColorFlag() { return colorFlag_; }
	int32_t GetAttribute() { return attribute_; }


	void SetTransform(const XMFLOAT3& pos) { transform_ = pos; };
	void SetRotation(const XMFLOAT3& rotate) { rotation_ = rotate; };
	void SetScale(const XMFLOAT3& scale) { scale_ = scale; }

	void SetMatTrans(const XMMATRIX& matTrans) { matTrans_ = matTrans; }
	void SetMatRot(const XMMATRIX& matRot) { matRot_ = matRot; }
	void SetMatScale(const XMMATRIX& matScale) { matScale_ = matScale; }

	void SetColorFlag(bool colorFlag) { colorFlag_ = colorFlag; }
	void SetColor(XMFLOAT3 color) { color_ = color; }
	//void SetColorA(XMFLOAT4 colorA) { colorA_ = colorA; }
	void SetAmbient(XMFLOAT3 color);
	void SetAttribute(int32_t attribute) { attribute_ = attribute; }

	/// <summary>
	/// �R���C�_�[�̃Z�b�g
	/// </summary>
	/// <param name="collider">�R���C�_�[</param>
	void SetCollider(BaseCollider* collider);

	//�e���v���[�g�R���X�g���N�^
	template <typename Type1>
	//�萔�o�b�t�@�̐���
	ComPtr<ID3D12Resource> CrateConstBuff(Type1* directXBasic_);


public: //�ÓI�����o�֐�

	static void SetLightGroup(LightGroup* lightGroup) { Object3d::lightGroup_ = lightGroup; }

};

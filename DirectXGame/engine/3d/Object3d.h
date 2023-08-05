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
#include "Vector3.h"

class BaseCollider;

class Object3d
{
private: //�G�C���A�X

	//�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:

	//�萔�o�b�t�@�p�f�[�^�\����(���W�n)b0
	struct ConstBufferDateTransform
	{
		Matrix4 viewProjection;
		//���[���h�s��
		Matrix4 worldMatrix;
		//�J�������W(���[���h�s��)
		Vector3 cameraPos;
		float pad1;
	};

	//�萔�o�b�t�@�p�f�[�^�\����(�}�e���A��)b1
	struct ConstBufferDataMaterial
	{
		Vector3 ambient;	//�A���r�G���g�W��
		float pad1;
		Vector3 diffuse;
		float pad2;
		Vector3 specular;
		float alpha;
	};

	struct GSOutput
	{
		Vector3 pos;		// xyz���W
		Vector3 normal;	//�@���x�N�g��
		Vector2 uv;		// uv���W
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
	Vector3 scale_;
	//��]�p
	Vector3 rotation_;
	//���s�ړ�
	Vector3 transform_;

	Matrix4 matScale_, matRot_, matTrans_;

	//���[���h�ϊ��s��
	Matrix4 matWorld_;

	//�r���[�s��
	Matrix4 matView_;

	//�ˉe�s��
	Matrix4 matProjection_;

	//�J�������W
	Vector3 cameraPos_;

	//�e�I�u�W�F�N�g�̃|�C���^

	ID3DBlob* vsBlob_ = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
	ID3DBlob* psBlob_ = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ID3DBlob* gsBlob_ = nullptr;	//�W�I���g���V�F�[�_�[�I�u�W�F�N�g
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

	Vector3 color_ = { 1,1,1};

	//����
	int32_t attribute_;

public:

	//�Q�b�^�[
	ConstBufferDateTransform* GetConstMapTransform() { return constMapTransform_; };

	Vector3 GetWorldPos() const;
	/// <summary>
	/// ���[���h�s��̎擾
	/// </summary>
	/// <returns>���[���h�s��</returns>
	const Matrix4& GetMatWorld() { return matWorld_; }

	//���f�����擾
	//const Model& GetModel() { return model_; }
	Model* GetModel() { return &model_; }
	bool GetColorFlag() { return colorFlag_; }
	int32_t GetAttribute() { return attribute_; }


	void SetTransform(const Vector3& pos) { transform_ = pos; };
	void SetRotation(const Vector3& rotate) { rotation_ = rotate; };
	void SetScale(const Vector3& scale) { scale_ = scale; }

	void SetMatTrans(const Matrix4& matTrans) { matTrans_ = matTrans; }
	void SetMatRot(const Matrix4& matRot) { matRot_ = matRot; }
	void SetMatScale(const Matrix4& matScale) { matScale_ = matScale; }

	void SetColorFlag(bool colorFlag) { colorFlag_ = colorFlag; }
	void SetColor(Vector3 color) { color_ = color; }
	void SetAmbient(Vector3 color);
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

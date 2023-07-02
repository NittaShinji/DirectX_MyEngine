#pragma once
#include "DirectXBasic.h"
#include "Model.h"
#include "Sprite.h"
#include "Camera.h"
#include "Input.h"
#include "LightGroup.h"
#include <vector>
#include <string>

class Object3d
{
	//nameSpace
	using XMFLOAT = DirectX::XMFLOAT3;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMMATRIX = DirectX::XMMATRIX;

	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:

	//Object3d(DirectXBasic* directXBasic,XMFLOAT3 position);
	Object3d();
	~Object3d();

	static void StaticInitialize(DirectXBasic* directXBasic);
	void Initialize(const std::string& path, const XMFLOAT3& Modelscale, const XMFLOAT3& position, const XMFLOAT3& rotation);
	void Update(Camera* camera);
	//void BeforeDraw();
	static void BeforeDraw();

	void AfterDraw();
	void Draw();
	void SetModel(const std::string& path);
	void Create(Model* model);
	void CrateConstBuffandMapping();

private:

	static DirectXBasic* directXBasic_;
	static KeyInput* keys_;
	static LightGroup* lightGroup_;
	Model model_;
	Sprite* sprite_;
	Camera* camera_ = nullptr;
	
	//�萔�o�b�t�@�p�f�[�^�\����b0
	struct ConstBufferDateTransform
	{
		//XMMATRIX mat;	//3D�ϊ��s��

		XMMATRIX viewProjection;
		//���[���h�s��
		XMMATRIX worldMatrix;
		//�J�������W(���[���h�s��)
		XMFLOAT3 cameraPos;
		float pad1;
		//XMFLOAT4 color;	//�F(RGBA)
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

	//�萔�o�b�t�@�p�f�[�^�\����(���C�g���)b2
	struct ConstBufferLightDate
	{
		XMFLOAT3 lightv;
		float pad1;
		XMFLOAT3 lightcolor;
		float pad2;
	};

	struct Vertex
	{
		XMFLOAT3 pos;		// xyz���W
		XMFLOAT3 normal;	//�@���x�N�g��
		XMFLOAT2 uv;		// uv���W
	};
	
	//�萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform_;
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial_;
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffLight_;
	
	//�萔�o�b�t�@�̃}�b�s���O�p�|�C���^
	ConstBufferDateTransform* constMapTransform_ = nullptr;
	ConstBufferDataMaterial* constMapMaterial_ = nullptr;
	ConstBufferLightDate* constMapLight_ = nullptr;

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

public:

	//�Q�b�^�[
	ConstBufferDateTransform* GetConstMapTransform() { return constMapTransform_; };

	XMFLOAT3 GetWorldPos() const ;
	void SetTransform(const XMFLOAT3& pos) { transform_ = pos; };
	void SetRotation(const XMFLOAT3& rotate) { rotation_ = rotate; };
	void SetScale(const XMFLOAT3& scale) { scale_ = scale; }

	void SetMatTrans(const XMMATRIX& matTrans) { matTrans_ = matTrans; }
	void SetMatRot(const XMMATRIX& matRot) { matRot_ = matRot; }
	void SetMatScale(const XMMATRIX& matScale) { matScale_ = matScale; }


	void SetColorFlag(bool colorFlag) { colorFlag_ = colorFlag; }

	//�e���v���[�g�R���X�g���N�^
	template <typename Type1>
	//�萔�o�b�t�@�̐���
	ComPtr<ID3D12Resource> CrateConstBuff(Type1* directXBasic_);


public: //�ÓI�����o�֐�

	static void SetLightGroup(LightGroup* lightGroup) { Object3d::lightGroup_ = lightGroup; }

};

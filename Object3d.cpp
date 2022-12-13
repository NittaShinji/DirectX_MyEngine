#include "Object3d.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <assert.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

using namespace std;

std::vector<Object3d::Vertex> Object3d::vertices_;
std::vector<unsigned short> Object3d::indices_;


void Object3d::StaticInitialize(Microsoft::WRL::ComPtr<ID3D12Device> device, int window_width, int window_hieght)
{

}

void Object3d::Create()
{

}

void Object3d::InitializeObject3d(DirectXBasic* directXBasic)
{
	directXBasic_ = directXBasic;

	HRESULT result;

	//�萔�o�b�t�@�̃q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//�萔�o�b�t�@�̃��\�[�X�ݒ�
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//�萔�o�b�t�@�̐���
	result = directXBasic_->GetDevice()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform));
	assert(SUCCEEDED(result));

	//�萔�o�b�t�@�̃}�b�s���O
	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);
	assert(SUCCEEDED(result));
}

void Object3d::Initialize()
{
	//std::vector<Vertex> v = 
	//{
	//	{ -0.5f, -0.5f, 0.0f },{0,0,0}, // ���� �C���f�b�N�X0
	//	{ -0.5f, +0.5f, 0.0f },{0,0,0}, // ���� �C���f�b�N�X1
	//	{ +0.5f, -0.5f, 0.0f },{0,0,0}, // �E�� �C���f�b�N�X2
	//	{ +0.5f, +0.5f, 0.0f },{0,0,0}, // �E�� �C���f�b�N�X3
	//}

	//vertices_ =
	//{
	//	{ -0.5f, -0.5f, 0.0f }, // ���� �C���f�b�N�X0
	//	{ -0.5f, +0.5f, 0.0f }, // ���� �C���f�b�N�X1
	//	{ +0.5f, -0.5f, 0.0f }, // �E�� �C���f�b�N�X2
	//	{ +0.5f, +0.5f, 0.0f }, // �E�� �C���f�b�N�X3
	//};
	
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

	float angle = 0.0f;	//�J�����̉�]�p

	//�ˉe�ϊ��s��
	matProjection =
		XMMatrixPerspectiveFovLH(
			XMConvertToRadians(45.0f),				//�㉺��p45�x
			(float)directXBasic_->GetWinWidth() / directXBasic_->GetWinHeight(),	//�A�X�y�N�g��(��ʉ���/��ʏc��)
			0.1f, 1000.0f							//�O�[,���[
		);

	scale = { 1.0f,1.0f,1.0f };
	rotation = { 0.0f,0.0f,0.0f };
	position = { 0.0f,0.0f,0.0f };

	////�z����̑S�I�u�W�F�N�g�ɑ΂���
	//for (int i = 0; i < _countof(object3ds); i++)
	//{
	//	//������
	//	InitializeObject3d(&object3ds[i]);

	//	//�e�\���̃T���v��
	//	//�擪�ȊO�Ȃ�
	//	if (i > 0)
	//	{
	//		//�e�I�u�W�F�N�g�̐ݒ�
	//		object3ds[i].parent = &object3ds[0];
	//		//�e�I�u�W�F�N�g�ɑ΂���傫��
	//		object3ds[i].scale = { 1.0f,1.0f,1.0f };
	//		//�e�I�u�W�F�N�g�ɑ΂����]
	//		object3ds[i].rotation = { 0.0f,0.0f,0.0f };
	//		//�e�I�u�W�F�N�g�ɑ΂���Z����-8.0���炷
	//		//object3ds[i].position = { 0.0f,0.0f,-8.0f };
	//	}

	//	object3ds[1].position = { 10.0f,0.0f,0.0f };
	//	object3ds[2].position = { 20.0f,0.0f,0.0f };
	//	object3ds[3].position = { -10.0f,0.0f,0.0f };
	//	object3ds[4].position = { -20.0f,0.0f,0.0f };
	//	object3ds[5].position = { 0.0f,10.0f,0.0f };
	//	object3ds[6].position = { 0.0f,-10.0f,0.0f };
	//	object3ds[7].position = { 0.0f,-20.0f,0.0f };
	//	object3ds[8].position = { 10.0f,-30.0f,0.0f };
	//	object3ds[9].position = { 10.0f,-40.0f,0.0f };
	//	object3ds[10].position = { -10.0f,-30.0f,0.0f };
	//	object3ds[11].position = { -10.0f,-40.0f,0.0f };
	//}

}

void Object3d::Update()
{
	//���[���h�ϊ��s��
	XMMATRIX matWorld;

	XMMATRIX matScale;	//�X�P�[�����O�s��
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);

	XMMATRIX matRot;	//��]�s��
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ((rotation.z));	//Z�������45�x��]
	matRot *= XMMatrixRotationX((rotation.x));	//X�������15�x��]
	matRot *= XMMatrixRotationY((rotation.y));	//Y�������30�x��]

	XMMATRIX matTrans;	//���s�ړ��s��
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);	//(-50,0,0)���s�ړ�

	matWorld = XMMatrixIdentity();	//�P�ʍs��������ĕό`�����Z�b�g
	matWorld *= matScale;	//���[���h�s��ɃX�P�[�����O�𔽉f
	matWorld *= matRot;		//���[���h�s��ɉ�]�𔽉f
	matWorld *= matTrans;	//���[���h�s��ɕ��s�ړ��𔽉f

	//�萔�o�b�t�@�Ƀf�[�^�]��
	constMapTransform->mat = matWorld * matView * matProjection;
}

void Object3d::Draw()
{
	// �V�U�[��`
	D3D12_RECT scissorRect{};
	scissorRect.left = 0; // �؂蔲�����W��
	scissorRect.right = scissorRect.left + directXBasic_->GetWinWidth(); // �؂蔲�����W�E
	scissorRect.top = 0; // �؂蔲�����W��
	scissorRect.bottom = scissorRect.top + directXBasic_->GetWinHeight(); // �؂蔲�����W��
	// �V�U�[��`�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
	directXBasic_->GetCommandList()->RSSetScissorRects(1, &scissorRect);

	//�p�C�v���C���̃Z�b�g
	directXBasic_->GetCommandList()->SetPipelineState(pipelineState.Get());
	//���[�g�V�O�l�`���̃Z�b�g
	directXBasic_->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());
	//�v���~�e�B�u�g�|���W�[�̃Z�b�g
	directXBasic_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
	directXBasic_->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
	//�C���f�b�N�X�o�b�t�@�r���[�̐ݒ�R�}���h
	directXBasic_->GetCommandList()->IASetIndexBuffer(&ibView);
	////0�Ԓ萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	//directXBasic_ ->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());
	//0�Ԓ萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	directXBasic_ ->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());

	// �`��R�}���h
	directXBasic_->GetCommandList()->DrawIndexedInstanced((UINT)indices_.size(), 1, 0, 0, 0); // �S�Ă̒��_���g���ĕ`��

	////1�Ԓ萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	//commandList->SetGraphicsRootConstantBufferView(2, constBuffTransform1->GetGPUVirtualAddress());
	//// �`��R�}���h
	//commandList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);; // �S�Ă̒��_���g���ĕ`��
}

void Object3d::CreateModel()
{
	//�t�@�C���X�g���[��
	std::ifstream file;
	// .OBJ�t�@�C�����J��
	file.open("Resources/triangle/triangle.obj");
	//�t�@�C���I�[�v�����s���`�F�b�N
	assert(!file.fail());

	vector<XMFLOAT3>positions;	//���_���W
	vector<XMFLOAT3>normals;	//�@���x�N�g��
	vector<XMFLOAT3>texcoords;	//�e�N�X�`��UV

	//1�s���ǂݍ���
	string line;
	while (getline(file, line))
	{
		//1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		std::istringstream line_stream(line);

		//���p�X�y�[�X��؂�ōs�̐擪��������擾
		string key;
		getline(line_stream, key, ' ');

		//�擪������"v"�Ȃ璸�_���W
		if (key == "v")
		{
			//X,Y,Z���W�ǂݍ���
			XMFLOAT3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			//���W�f�[�^�ɒǉ�
			positions.emplace_back(position);
			//���_�f�[�^�ɒǉ�
			Vertex vertex{};
			vertex.pos = position;
			vertices_.emplace_back(vertex);
		}

		//�擪������"f"�Ȃ�|���S��(�O�p�`)
		if (key == "f")
		{
			//���p�X�y�[�X��؂�ōs�̑�����ǂݍ���
			string index_string;
			while (getline(line_stream,index_string,' '))
			{
				std::istringstream index_stream(index_string);
				unsigned short indexPosition;
				index_stream >> indexPosition;
				//���_�C���f�b�N�X�ɒǉ�
				indices_.emplace_back(indexPosition - 1);
			}
		}

	}

	file.close();

#pragma region �萔�o�b�t�@

	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};				//GPU�ւ̓]���p
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;	//256�o�C�g�A���C�������g
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//�萔�o�b�t�@�̐���
	result_ = directXBasic_->GetDevice()->CreateCommittedResource(
		&cbHeapProp,//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial));
	assert(SUCCEEDED(result_));

	//�萔�o�b�t�@�̃}�b�s���O
	result_ = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);//�}�b�s���O
	assert(SUCCEEDED(result_));

#pragma endregion


	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	//UINT sizeVB = static_cast<UINT>(sizeof(XMFLOAT3) * _countof(vertices));
	UINT sizeVB =
		static_cast<UINT>(sizeof(Vertex) * vertices_.size());


	//�J���[�̏������݂Ɠ]��
	constMapMaterial->color = XMFLOAT4(0, 1, 0, 0.5f);

	// ���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{}; // �q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p

	// ���\�[�X�ݒ�
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // ���_�f�[�^�S�̂̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// ���_�o�b�t�@�̐���
	//ComPtr<ID3D12Resource> vertBuff_ = nullptr;
	result_ = directXBasic_->GetResult();
	result_ = directXBasic_->GetDevice()->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff_));
	assert(SUCCEEDED(result_));

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	//Vertex* vertMap = nullptr;
	Vertex* vertMap = nullptr;
	result_ = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result_));
	// �S���_�ɑ΂���
	std::copy(vertices_.begin(), vertices_.end(), vertMap);

	// �q���������
	vertBuff_->Unmap(0, nullptr);

#pragma region ���_�o�b�t�@�r���[�̍쐬

	// GPU���z�A�h���X
	vbView.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	vbView.SizeInBytes = sizeVB;
	// ���_1���̃f�[�^�T�C�Y
	vbView.StrideInBytes = sizeof(vertices_[0]);

#pragma endregion
	
	//�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices_.size());

	//���\�[�X�ݒ�
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB;	//�@�C���f�b�N�X��񂪓��镪�̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//�C���f�b�N�X�o�b�t�@�̐ݒ�
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff;
	result_ = directXBasic_->GetDevice()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff)
	);

	//�C���f�b�N�X�o�b�t�@���}�b�s���O
	uint16_t* indexMap = nullptr;
	result_ = indexBuff->Map(0, nullptr, (void**)&indexMap);
	//�S�C���f�b�N�X�ɑ΂���
	std::copy(indices_.begin(), indices_.end(), indexMap);

	//�}�b�s���O����
	indexBuff->Unmap(0, nullptr);

#pragma region �C���f�b�N�X�o�b�t�@�r���[�̍쐬

	//�C���f�b�N�X�o�b�t�@�r���[�̍쐬
	D3D12_INDEX_BUFFER_VIEW ibView{};
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

#pragma endregion

	ID3DBlob* vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
	ID3DBlob* psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ID3DBlob* errorBlob = nullptr; // �G���[�I�u�W�F�N�g
	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result_ = D3DCompileFromFile(
		L"Resources/shaders/BasicVS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);
	// �G���[�Ȃ�
	if (FAILED(result_)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}
	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result_ = D3DCompileFromFile(
		L"Resources/shaders/BasicPS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob);
	// �G���[�Ȃ�
	if (FAILED(result_)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}

	// ���_���C�A�E�g
	inputLayout = 
	{
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		}, // (1�s�ŏ������ق������₷��)
		{
			//�@���x�N�g��(1�s�ŏ������ق����₷��)
			"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		// uv���W
		{
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},

	};

	// �O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	// �V�F�[�_�[�̐ݒ�
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

	// �T���v���}�X�N�̐ݒ�
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	// ���X�^���C�U�̐ݒ�
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // �J�����O���Ȃ�
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // �|���S�����h��Ԃ�
	pipelineDesc.RasterizerState.DepthClipEnable = true; // �[�x�N���b�s���O��L����
	
	// ���_���C�A�E�g�̐ݒ�
	pipelineDesc.InputLayout.pInputElementDescs = &inputLayout.at(0);
	pipelineDesc.InputLayout.NumElements = inputLayout.size();
	// �}�`�̌`��ݒ�
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// ���̑��̐ݒ�
	pipelineDesc.NumRenderTargets = 1; // �`��Ώۂ�1��
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255�w���RGBA
	pipelineDesc.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	//�����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;// RBGA�S�Ẵ`�����l����`��

	//�A���t�@�l�̌v�Z���̐ݒ�
	blenddesc.BlendEnable = true;					//�u�����h��L���ɂ���
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//���Z
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		//�\�[�X�̒l��100%�g��(������`�悵�悤�Ƃ��Ă���F)
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//�f�X�g�̒l��  0%�g��(���ɕ`����Ă���F)
	
	//����������
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;				//���Z
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;			//�\�[�X�̃A���t�@�l
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;	//1.0f - �\�[�X�̃A���t�@�l

	//RootSignatureSet();

	//���[�g�p�����[�^�̐ݒ�
	D3D12_ROOT_PARAMETER rootParam = {};
	rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//�萔�o�b�t�@�r���[
	rootParam.Descriptor.ShaderRegister = 0;					//�萔�o�b�t�@�ԍ�
	rootParam.Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_�[���猩����


	// ���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = &rootParam;	//���[�g�p�����[�^�̐擪�A�h���X
	rootSignatureDesc.NumParameters = 1;	//���[�g�p�����[�^�[��

	// ���[�g�V�O�l�`���̃V���A���C�Y
	ID3DBlob* rootSigBlob = nullptr;
	result_ = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result_));
	result_ = directXBasic_->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result_));
	rootSigBlob->Release();
	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc.pRootSignature = rootSignature.Get();
	
	result_ = directXBasic_->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result_));
	
}

void Object3d::RootSignatureSet()
{
	
}

#include "Model.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;
using namespace DirectX;

DirectXBasic* Model::directXBasic_ = nullptr;
std::map<Model::MODELKEY, Model::MODELVALUE> Model::models_;

Model::Model()
{
	////�q�[�v�ݒ�
	//D3D12_HEAP_PROPERTIES cbHeapProp{};				//GPU�ւ̓]���p
	//cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	////���\�[�X�ݒ�
	//D3D12_RESOURCE_DESC cbResourceDesc{};
	//cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	//cbResourceDesc.Width = (sizeof(ConstBufferDateB1) + 0xff) & ~0xff;	//256�o�C�g�A���C�������g
	//cbResourceDesc.Height = 1;
	//cbResourceDesc.DepthOrArraySize = 1;
	//cbResourceDesc.MipLevels = 1;
	//cbResourceDesc.SampleDesc.Count = 1;
	//cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	////�萔�o�b�t�@�̐���
	//result_ = directXBasic_->GetDevice()->CreateCommittedResource(
	//	&cbHeapProp,//�q�[�v�ݒ�
	//	D3D12_HEAP_FLAG_NONE,
	//	&cbResourceDesc,//���\�[�X�ݒ�
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&infomation_.constBuffB1));
	//assert(SUCCEEDED(result_));

	////�萔�o�b�t�@�̃}�b�s���O
	//result_ = infomation_.constBuffB1->Map(0, nullptr, (void**)&infomation_.constMapTransform);//�}�b�s���O
	//assert(SUCCEEDED(result_));
}

void Model::Load(const std::string& path, DirectXBasic* directXBasic)
{
	Model model;
	model.name_ = path;

	//�t�@�C���X�g���[��
	std::ifstream file;
	// .OBJ�t�@�C�����J��
	file.open("Resources/triangle/triangle.obj");
	//�t�@�C���I�[�v�����s���`�F�b�N
	assert(!file.fail());

	vector<XMFLOAT3>positions;	//���_���W
	vector<XMFLOAT3>normals;	//�@���x�N�g��
	vector<XMFLOAT2>texcoords;	//�e�N�X�`��UV

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
			model.infomation_.vertices_.emplace_back(vertex);
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
				model.infomation_.indices_.emplace_back(indexPosition - 1);
			}
		}
		
		//
		////�擪������"vt"�Ȃ�e�N�X�`��
		//if (key == "vt")
		//{
		//	////UV�����ǂݍ���
		//	XMFLOAT2 texcoord{};
		//	line_stream >> texcoord.x;
		//	line_stream >> texcoord.y;
		//	//V�������]
		//	texcoord.y = 1.0f - texcoord.y;
		//	//�e�N�X�`�����W�f�[�^�ɒǉ�
		//	texcoords.emplace_back(texcoord);
		//}
		////�擪������"vf"�Ȃ�@���x�N�g��
		//if (key == "vn")
		//{
		//	////X,Y,Z�����ǂݍ���
		//	XMFLOAT3 normal{};
		//	line_stream >> normal.x;
		//	line_stream >> normal.y;
		//	line_stream >> normal.z;
		//	//�@���x�N�g���f�[�^�ɒǉ�
		//	normals.emplace_back(normal);
		//}

		////�擪������"f"�Ȃ�|���S��(�O�p�`)
		//if (key == "f")
		//{
		//	//���p�X�y�[�X��؂�ōs�̑�����ǂݍ���
		//	string index_string;
		//	while (getline(line_stream,index_string,' '))
		//	{
		//		//���_�C���f�b�N�X�P���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		//		std::istringstream index_stream(index_string);
		//		unsigned short indexPosition, indexNormal, indexTexcoord;
		//		index_stream >> indexPosition;
		//		index_stream.seekg(1, ios_base::cur);	//�X���b�V�����΂�
		//		index_stream >> indexTexcoord;
		//		index_stream.seekg(1, ios_base::cur);	//�X���b�V�����΂�
		//		index_stream >> indexNormal;
		//		//���_�f�[�^�ǉ�
		//		Vertex vertex{};
		//		vertex.pos = positions[indexPosition - 1];
		//		vertex.normal = normals[indexNormal - 1];
		//		vertex.uv = texcoords[indexTexcoord - 1];
		//		model.infomation_.vertices_.emplace_back(vertex);
		//		//�C���f�b�N�X�f�[�^�̒ǉ�
		//		model.infomation_.indices_.emplace_back((unsigned short)model.infomation_.indices_.size());
		//	}
		//}

	}

	file.close();

	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * model.infomation_.vertices_.size());

	// ���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{}; // �q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p

	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};

	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // ���_�f�[�^�S�̂̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// ���_�o�b�t�@�̐���
	HRESULT result;
	result = directXBasic_->GetResult();
	result = directXBasic_->GetDevice()->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&model.infomation_.vertBuff_));
	assert(SUCCEEDED(result));

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	Vertex* vertMap = nullptr;
	result = model.infomation_.vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// �S���_�ɑ΂���
	std::copy(model.infomation_.vertices_.begin(), model.infomation_.vertices_.end(), vertMap);
	
	// �q���������
	model.infomation_.vertBuff_->Unmap(0, nullptr);

	//�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * model.infomation_.indices_.size());
	
	//���\�[�X�ݒ�
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB;	//�@�C���f�b�N�X��񂪓��镪�̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//�C���f�b�N�X�o�b�t�@�̐ݒ�
	result = directXBasic_->GetDevice()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&model.infomation_.indexBuff)
	);

	//�C���f�b�N�X�o�b�t�@���}�b�s���O
	uint16_t* indexMap = nullptr;
	result = model.infomation_.indexBuff->Map(0, nullptr, (void**)&indexMap);
	//�S�C���f�b�N�X�ɑ΂���
	std::copy(model.infomation_.indices_.begin(), model.infomation_.indices_.end(), indexMap);

	//�}�b�s���O����
	model.infomation_.indexBuff->Unmap(0, nullptr);

#pragma region ���_�o�b�t�@�r���[�̍쐬

	// GPU���z�A�h���X
	model.infomation_.vbView.BufferLocation = model.infomation_.vertBuff_->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	model.infomation_.vbView.SizeInBytes = sizeVB;
	// ���_1���̃f�[�^�T�C�Y
	model.infomation_.vbView.StrideInBytes = sizeof(infomation_.vertices_[0]);

#pragma endregion

#pragma region �C���f�b�N�X�o�b�t�@�r���[�̍쐬

	//�C���f�b�N�X�o�b�t�@�r���[�̍쐬
	//D3D12_INDEX_BUFFER_VIEW ibView{};
	model.infomation_.ibView.BufferLocation = model.infomation_.indexBuff->GetGPUVirtualAddress();
	model.infomation_.ibView.Format = DXGI_FORMAT_R16_UINT;
	model.infomation_.ibView.SizeInBytes = sizeIB;

#pragma endregion

	////�f�X�N���v�^�q�[�v�̐ݒ�
	//D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	//srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	//srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; // �V�F�[�_�[���猩����悤��
	//srvHeapDesc.NumDescriptors = kMaxSRVCount;

	////�ݒ��{��SRV�p�f�X�N���v�^�q�[�v�𐶐�
	//result = directXBasic_->GetDevice()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
	//assert(SUCCEEDED(result));

	////SRV�q�[�v�̐擪�n���h�����擾
	//D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

	////�f�X�N���v�^�̃T�C�Y���擾
	//UINT incrementSize = directXBasic_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	////�擾�����T�C�Y���g�p���ăn���h����i�߂�
	//for (uint32_t i = 0; i < textureIndex_; i++)
	//{
	//	srvHandle.ptr += incrementSize;
	//}

	////�V�F�[�_�[���\�[�X�r���[�̐ݒ�
	//D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; //�ݒ�\����
	//srvDesc.Format = resDesc.Format;//RGBA float
	//srvDesc.Shader4ComponentMapping =
	//	D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	//srvDesc.Texture2D.MipLevels = resDesc.MipLevels;

	////�n���h���̎w���ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	//directXBasic_->GetDevice()->CreateShaderResourceView(textureBuffers_[textureIndex_].Get(), &srvDesc, srvHandle);



	models_.insert_or_assign(model.name_, model.infomation_);
}

void Model::Update()
{
	/*ConstBufferDateB1* constMap1 = nullptr;
	
	HRESULT result = infomation_.constBuffB1->Map(0, nullptr, (void**)&constMap1);
	constMap1->ambient = infomation_.material_.ambient;
	constMap1->diffuse = infomation_.material_.diffuse;
	constMap1->specular= infomation_.material_.specular;
	constMap1->alpha = infomation_.material_.alpha;
	infomation_.constBuffB1->Unmap(0, nullptr);*/
}

void Model::Draw(ID3D12DescriptorHeap* srvHeapHandle)
{
	
}

void Model::StaticInitialize(DirectXBasic* directXBasic)
{
	directXBasic_ = directXBasic;
}

const Model::MODELVALUE* Model::GetMODELVALUE(const MODELKEY path)
{
	return &models_.at(path);
}

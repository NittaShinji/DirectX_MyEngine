#include "Model.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <DirectXTex.h>
#include <D3dx12.h>

using namespace std;
using namespace DirectX;

DirectXBasic* Model::directXBasic_ = nullptr;
//std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, Model::kMaxSRVCount> Model::textureBuffers_;
//uint32_t Model::textureIndex_;
std::map<Model::MODELKEY, Model::MODELVALUE> Model::models_;
//std::string Model::kDefaultTextureDirectoryPath_ = "Resources/";
//D3D12_CPU_DESCRIPTOR_HANDLE Model::srvHandle;
uint32_t Model::textureIndex_ = 0;
D3D12_CPU_DESCRIPTOR_HANDLE Model::srvHandle;

void Model::StaticInitialize(DirectXBasic* directXBasic)
{
	directXBasic_ = directXBasic;
}

void Model::Load(const std::string& path)
{
	Model model;
	model.name_ = path;

	//�t�@�C���X�g���[��
	std::ifstream file;
	// .OBJ�t�@�C�����J��
	const string modelFileName = model.name_ + ".obj";
	const string directoryPath = "Resources/" + model.name_ + "/";
	
	file.open(directoryPath + modelFileName);

	//�t�@�C���I�[�v�����s���`�F�b�N
	assert(!file.fail());

	vector<XMFLOAT3>positions;	//���_���W
	vector<XMFLOAT3>normals;	//�@���x�N�g��
	vector<XMFLOAT2>texcoords;	//�e�N�X�`��UV

	//�C���f�b�N�X�J�E���g�p�̕ϐ�
	//int indexCountTex = 0;

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
			//�E��n�̃��f���f�[�^������n�ɕϊ�(X���])
			//position.x = -position.x;
			//���W�f�[�^�ɒǉ�
			positions.emplace_back(position);
		}
		
		//�擪������"vt"�Ȃ�e�N�X�`��
		if (key == "vt")
		{
			////UV�����ǂݍ���
			XMFLOAT2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			//V�������]
			texcoord.y = 1.0f - texcoord.y;
			//�e�N�X�`�����W�f�[�^�ɒǉ�
			texcoords.emplace_back(texcoord);
		}
		//�擪������"vf"�Ȃ�@���x�N�g��
		if (key == "vn")
		{
			////X,Y,Z�����ǂݍ���
			XMFLOAT3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			//�@���x�N�g���f�[�^�ɒǉ�
			normals.emplace_back(normal);
		}

		//�擪������"f"�Ȃ�|���S��(�O�p�`)
		if (key == "f")
		{
			//int faceIndexCount = 0;
			//���p�X�y�[�X��؂�ōs�̑�����ǂݍ���
			string index_string;
			//std::array<uint16_t, 4> tempIndices;
			while (getline(line_stream,index_string,' '))
			{
				//���_�C���f�b�N�X�P���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				index_stream >> indexPosition;
				index_stream.seekg(1, ios_base::cur);	//�X���b�V�����΂�
				index_stream >> indexTexcoord;
				index_stream.seekg(1, ios_base::cur);	//�X���b�V�����΂�
				index_stream >> indexNormal;
				//���_�f�[�^�ǉ�
				Vertex vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				model.infomation_.vertices_.emplace_back(vertex);
				//�C���f�b�N�X�f�[�^�̒ǉ�
				model.infomation_.indices_.emplace_back((unsigned short)model.infomation_.indices_.size());

				//assert(faceIndexCount < 4 && "5�p�`�|���S���ȏ�͔�Ή��ł�");

				//// �C���f�b�N�X�f�[�^�̒ǉ�
				//tempIndices[faceIndexCount] = indexCountTex;

				//indexCountTex++;
				//faceIndexCount++;
			}

			//// �C���f�b�N�X�f�[�^�̏����ύX�Ŏ��v��聨�����v���ϊ�
			//model.infomation_.vertices_.emplace_back(tempIndices[0]);
			//model.infomation_.vertices_.emplace_back(tempIndices[2]);
			//model.infomation_.vertices_.emplace_back(tempIndices[1]);
			//// �l�p�`�Ȃ�O�p�`��ǉ�
			//if(faceIndexCount == 4)
			//{
			//	model.infomation_.vertices_.emplace_back(tempIndices[0]);
			//	model.infomation_.vertices_.emplace_back(tempIndices[3]);
			//	model.infomation_.vertices_.emplace_back(tempIndices[2]);
			//}
		}

		if (key == "mtllib")
		{
			//�}�e���A���̃t�@�C���ǂݍ���
			string fileName;
			line_stream >> fileName;
			//�}�e���A���̓ǂݍ���
			LoadMaterial(directoryPath, fileName,model);
		}
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
	
	//���f��map�ւ̑}��
	models_.insert_or_assign(model.name_, model.infomation_);
}

void Model::Update(){}

void Model::LoadMaterial(const std::string& directoryPath, const std::string& fileName, Model& model)
{
	const string defaultResourcePath = "Resources/";

	//�t�@�C���X�g���[��
	std::ifstream file;
	//�}�e���A���t�@�C�����J��
	file.open(directoryPath + fileName);
	//�t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail())
	{
		assert(0);
	}

	//1�s���ǂݍ���
	string line;
	while (getline(file, line))
	{
		//1�s���̕�������X�g���[���ɕϊ�
		std::istringstream line_stream(line);
		//���p�X�y�[�X��؂�ōs�̐擪��������擾
		string key;
		getline(line_stream, key, ' ');

		//�擪�̃^�u�����͖�������
		if (key[0] == '\t')
		{
			//�擪�̕������폜
			key.erase(key.begin());
		}

		//�擪������newmtl�Ȃ�}�e���A����
		if (key == "newmtl")
		{
			//�}�e���A�����ǂݍ���
			line_stream >> model.infomation_.material_.name;
		}
		//�擪������Ka�Ȃ�A���r�G���g�F
		if (key == "Ka")
		{
			line_stream >> model.infomation_.material_.ambient.x;
			line_stream >> model.infomation_.material_.ambient.y;
			line_stream >> model.infomation_.material_.ambient.z;
		}

		//�擪������Kd�Ȃ�f�B�t���[�Y�F
		if (key == "Kd")
		{
			line_stream >> model.infomation_.material_.diffuse.x;
			line_stream >> model.infomation_.material_.diffuse.y;
			line_stream >> model.infomation_.material_.diffuse.z;
		}

		//�擪������Kd�Ȃ�X�y�L�����[�F
		if (key == "Ks")
		{
			line_stream >> model.infomation_.material_.specular.x;
			line_stream >> model.infomation_.material_.specular.y;
			line_stream >> model.infomation_.material_.specular.z;
		}

		//�擪������map_kd�Ȃ�e�N�X�`���t�@�C����
		if (key == "map_Kd")
		{
			//�e�N�X�`���̃t�@�C�����ǂݍ���
			line_stream >> model.infomation_.material_.textureFilename;
			//�e�N�X�`���ǂݍ���
			LoadTexture(directoryPath, model.infomation_.material_.textureFilename,model);
		}
		else
		{
			model.infomation_.material_.textureFilename = "white1x1.png";
			//�e�N�X�`���ǂݍ���
			LoadTexture(defaultResourcePath, "white1x1.png", model);
		}
	}

	//�t�@�C�������
	file.close();
}

void Model::LoadTexture(const std::string& directoryPath, const std::string& fileName, Model& model)
{
	//�t�@�C���p�X������
	string filePath = directoryPath + fileName;

	//���j�R�[�h������ɕϊ�����
	wchar_t wfilePath[128];
	//int iBufferSize
	MultiByteToWideChar(CP_ACP, 0, filePath.c_str(), -1, wfilePath, _countof(wfilePath));

	//�摜�t�@�C���̗p��
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	HRESULT result = LoadFromWICFile(
		wfilePath, WIC_FLAGS_NONE,
		&metadata, scratchImg);

	ScratchImage mipChain{};
	//�~�j�}�b�v����
	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result))
	{
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	//�ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	metadata.format = MakeSRGB(metadata.format);

	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty =
		D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	//CD3DX12_HEAP_PROPERTIES textureHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width; // ��
	textureResourceDesc.Height = (UINT)metadata.height; // ��
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

	//�e�N�X�`���o�b�t�@�̐���
	result = directXBasic_->GetDevice()->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		//IID_PPV_ARGS(&textureBuffers_[0]));
		IID_PPV_ARGS(&model.infomation_.textureBuffers_[textureIndex_]));

	textureHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p

	//�S�~�j�}�b�v�ɂ���
	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		//�~�j�}�b�v���x�����w�肵�ăC���[�W���擾
		const Image* img = scratchImg.GetImage(i, 0, 0);

		//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��

		result = model.infomation_.textureBuffers_[textureIndex_]->WriteToSubresource(

			(UINT)i,
			nullptr,
			img->pixels,
			(UINT)img->rowPitch,
			(UINT)img->slicePitch
		);

		assert(SUCCEEDED(result));
	}

	//�f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; // �V�F�[�_�[���猩����悤��
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	//�ݒ��{��SRV�p�f�X�N���v�^�q�[�v�𐶐�
	result = directXBasic_->GetDevice()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&model.infomation_.srvHeap));
	assert(SUCCEEDED(result));

	//SRV�q�[�v�̐擪�n���h�����擾
	srvHandle = model.infomation_.srvHeap->GetCPUDescriptorHandleForHeapStart();

	//�f�X�N���v�^�̃T�C�Y���擾
	UINT incrementSize = directXBasic_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//�擾�����T�C�Y���g�p���ăn���h����i�߂�
	for (uint32_t i = 0; i < textureIndex_; i++)
	{
		srvHandle.ptr += incrementSize;
	}

	//�V�F�[�_�[���\�[�X�r���[�̐ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; //�ݒ�\����
	srvDesc.Format = textureResourceDesc.Format;//RGBA float
	srvDesc.Shader4ComponentMapping =
		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

	//�n���h���̎w���ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	directXBasic_->GetDevice()->CreateShaderResourceView(model.infomation_.textureBuffers_[textureIndex_].Get(), &srvDesc, srvHandle);
	
	//�摜�ԍ���i�߂�
	//textureIndex_++;
}

//�����L�[(�p�X)����l������
const Model::MODELVALUE* Model::GetMODELVALUE(const MODELKEY path)
{
	return &models_.at(path);
}

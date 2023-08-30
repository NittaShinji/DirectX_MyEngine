#include "Model.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <DirectXTex.h>
#include <D3dx12.h>
#include "Mesh.h"

using namespace std;
using namespace DirectX;

DirectXBasic* Model::directXBasic_ = nullptr;
std::map<Model::MODELKEY, Model::MODELVALUE> Model::sModels_;
uint32_t Model::sTextureIndex_ = 0;
D3D12_CPU_DESCRIPTOR_HANDLE Model::sSrvHandle_;

void Model::StaticInitialize(DirectXBasic* directXBasic)
{
	directXBasic_ = directXBasic;
}

void Model::Load(const std::string& path)
{
	Mesh mesh{};
	Model model;
	model.name_ = path;


	//�t�@�C���X�g���[��
	std::ifstream file;
	// .OBJ�t�@�C�����J��
	const string modelFileName = model.name_ + ".obj";
	const string directoryPath = "Resources/Model/" + model.name_ + "/";
	
	file.open(directoryPath + modelFileName);

	//�t�@�C���I�[�v�����s���`�F�b�N
	assert(!file.fail());

	vector<Vector3>positions;	//���_���W
	vector<Vector3>normals;	//�@���x�N�g��
	vector<Vector2>texcoords;	//�e�N�X�`��UV

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
			Vector3 position{};
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
			Vector2 texcoord{};
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
			Vector3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			//�@���x�N�g���f�[�^�ɒǉ�
			normals.emplace_back(normal);
		}

		//�擪������"f"�Ȃ�|���S��(�O�p�`)
		if (key == "f")
		{
			//���p�X�y�[�X��؂�ōs�̑�����ǂݍ���
			string index_string;
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
				Mesh::Vertex vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];

				//�C���f�b�N�X�f�[�^�ǉ�
				mesh.SetVertices(vertex);
				mesh.SetIndices(static_cast<unsigned short>(mesh.GetIndices().size()));
			}
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

	//�o�b�t�@�̐���
	mesh.CrateBuffer();
	model.infomation_.meshes.emplace_back(mesh);

	//���f��map�ւ̑}��
	sModels_.insert_or_assign(model.name_, model.infomation_);
}

void Model::Update(){}

void Model::LoadMaterial(const std::string& directoryPath, const std::string& fileName, Model& model)
{
	const string defaultResourcePath = "Resources/Sprite/";

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
			line_stream >> model.infomation_.material.name;
		}
		//�擪������Ka�Ȃ�A���r�G���g�F
		if (key == "Ka")
		{
			line_stream >> model.infomation_.material.ambient.x;
			line_stream >> model.infomation_.material.ambient.y;
			line_stream >> model.infomation_.material.ambient.z;
		}

		//�擪������Kd�Ȃ�f�B�t���[�Y�F
		if (key == "Kd")
		{
			line_stream >> model.infomation_.material.diffuse.x;
			line_stream >> model.infomation_.material.diffuse.y;
			line_stream >> model.infomation_.material.diffuse.z;
		}

		//�擪������Kd�Ȃ�X�y�L�����[�F
		if (key == "Ks")
		{
			line_stream >> model.infomation_.material.specular.x;
			line_stream >> model.infomation_.material.specular.y;
			line_stream >> model.infomation_.material.specular.z;
		}

		//�擪������map_kd�Ȃ�e�N�X�`���t�@�C����
		if (key == "map_Kd")
		{
			//�e�N�X�`���̃t�@�C�����ǂݍ���
			line_stream >> model.infomation_.material.textureFilename;
			//�e�N�X�`���ǂݍ���
			LoadTexture(directoryPath, model.infomation_.material.textureFilename,model);
		}
		else 
		{
			model.infomation_.material.textureFilename = "white1x1.png";
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
		IID_PPV_ARGS(&model.infomation_.textureBuffers[sTextureIndex_]));

	textureHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p

	//�S�~�j�}�b�v�ɂ���
	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		//�~�j�}�b�v���x�����w�肵�ăC���[�W���擾
		const Image* img = scratchImg.GetImage(i, 0, 0);

		//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		result = model.infomation_.textureBuffers[sTextureIndex_]->WriteToSubresource(

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
	srvHeapDesc.NumDescriptors = kMaxSRVCount_;

	//�ݒ��{��SRV�p�f�X�N���v�^�q�[�v�𐶐�
	result = directXBasic_->GetDevice()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&model.infomation_.srvHeap));
	assert(SUCCEEDED(result));

	//SRV�q�[�v�̐擪�n���h�����擾
	sSrvHandle_ = model.infomation_.srvHeap->GetCPUDescriptorHandleForHeapStart();

	//�f�X�N���v�^�̃T�C�Y���擾
	UINT incrementSize = directXBasic_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//�擾�����T�C�Y���g�p���ăn���h����i�߂�
	for (uint32_t i = 0; i < sTextureIndex_; i++)
	{
		sSrvHandle_.ptr += incrementSize;
	}

	//�V�F�[�_�[���\�[�X�r���[�̐ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; //�ݒ�\����
	srvDesc.Format = textureResourceDesc.Format;//RGBA float
	srvDesc.Shader4ComponentMapping =
		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

	//�n���h���̎w���ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	directXBasic_->GetDevice()->CreateShaderResourceView(model.infomation_.textureBuffers[sTextureIndex_].Get(), &srvDesc, sSrvHandle_);
	
	//�摜�ԍ���i�߂�
	//sTextureIndex_++;
}

//�����L�[(�p�X)����l������
const Model::MODELVALUE* Model::GetMODELVALUE(const MODELKEY path)
{
	return &sModels_.at(path);
}

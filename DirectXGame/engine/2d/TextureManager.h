#pragma once
#include "DirectXBasic.h"
#include "Vector4.h"
#include <wrl.h>
#include <array>
#include <map>

class TextureManager final
{
private:

	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:

	TextureManager();
	~TextureManager();

public:

	static TextureManager* GetInstance()
	{
		static TextureManager textureManager;
		return &textureManager;
	}

	//�ÓI������
	static void StaticInitialize(ID3D12Device* device);

	//������
	void Initialize();

	//�e�N�X�`���ǂݍ���
	static void LoadTexture(const std::string& fileName);

	//�e�N�X�`������
	void TexMapping(int32_t texWidth, int32_t texHeight, Vector4 color, const std::string& fileName);

public:

	//SRV�̍ő��
	static const size_t kMaxSRVCount_ = 2056;

private:

	//�V�F�[�_�[���\�[�X�p�̃f�X�N���v�^�q�[�v
	static ComPtr<ID3D12DescriptorHeap> srvHeap_;

	//�e�N�X�`���o�b�t�@
	static std::array<ComPtr<ID3D12Resource>, kMaxSRVCount_> textureBuffers_;

	//�f�t�H���g�e�N�X�`���i�[�f�B���N�g��
	static std::string kDefaultTextureDirectoryPath_;

	//�e�N�X�`���ԍ�
	static uint32_t sTextureIndex_;
	//�摜�Ɍ��ѕt�����e�N�X�`���ԍ��i�[�pmap
	static std::map<const std::string, uint32_t, std::less<>> textureMap_;

	static ID3D12Device* device_;
	static ID3D12GraphicsCommandList* cmdList_;

public:

	//�R�s�[�R���X�g���N�^�𖳌�
	TextureManager(const TextureManager& textureManager) = delete;
	//������Z�q�𖳌�
	TextureManager& operator= (const TextureManager& textureManager) = delete;

public:

	//�Q�b�^�[
	ID3D12DescriptorHeap* GetSRVHeap() const { return srvHeap_.Get(); };
	const std::map<const std::string, uint32_t, std::less<>>& GetTextureMap() const { return textureMap_; }
};


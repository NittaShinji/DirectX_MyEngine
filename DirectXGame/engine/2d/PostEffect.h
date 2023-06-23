#pragma once
#include "Sprite.h"

class PostEffect :
    public Sprite
{
public:

    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    PostEffect();

    /// <summary>
    /// ������
    /// </summary>
    void Initialize(DirectXBasic* directXBasic);

    /// <summary>
    /// �`��R�}���h�̔��s
    /// </summary>
    /// <param name="cmdList">�R�}���h���X�g</param>
    void Draw(const std::string& fileName);

    /// <summary>
    /// �p�C�v���C������
    /// </summary>
    void CreateGraphicsPipelineState();

    /// <summary>
    /// �V�[���`��O����
    /// </summary>
    /// <param name="cmdList"></param>
    void PreDrawScene();

    /// <summary>
    /// �V�[���`��㏈��
    /// </summary>
    /// <param name="cmdList"></param>
    void PostDrawScene();

    void LoadTexture(const std::string& fileName);

private:

    DirectXBasic* directXBasic_;

    //�e�N�X�`���o�b�t�@
    ComPtr<ID3D12Resource> texBuff;

    //SRV�̍ő��
    static const size_t kMaxSRVCount_ = 2056;
    //�e�N�X�`���o�b�t�@
    static std::array<ComPtr<ID3D12Resource>, kMaxSRVCount_> textureBuffers_;

    //�[�x�o�b�t�@
    ComPtr<ID3D12Resource> depthBuff;
    //���_�o�b�t�@
    ComPtr<ID3D12Resource> vertBuff_ = nullptr;

    //SRV�p�f�X�N���v�^�q�[�v
    ComPtr<ID3D12DescriptorHeap> descHeapSRV;
    //RTN�p�f�X�N���v�^�q�[�v
    ComPtr<ID3D12DescriptorHeap> descHeapRTV;
    //DSV�p�f�X�N���v�^�q�[�v
    ComPtr<ID3D12DescriptorHeap> descHeapDSV;

    //�F�p�̒萔�o�b�t�@
    ComPtr<ID3D12Resource> constBuffMaterial_ = nullptr;
    //���W�p�̒萔�o�b�t�@
    ComPtr<ID3D12Resource> constBuffTransform_ = nullptr;

    //�O���t�B�b�N�X�p�C�v���C��
    ComPtr<ID3D12PipelineState> pipelineState_;
    //���[�g�V�O�l�`��
    ComPtr<ID3D12RootSignature> rootSignature;

    //���_�z��
    //std::array<Vertex, kVertexCount_> vertices_{};
    Vertex vertices[kVertexCount_];

    // ���_�o�b�t�@�r���[�̍쐬
    D3D12_VERTEX_BUFFER_VIEW vbView_{};

    //��ʃN���A�J���[
    static const float clearColor[4];

    //�F(RGBA)
    XMFLOAT4 color_ = { 1,1,1,1 };

    //�e�N�X�`���ԍ�
    static uint32_t sTextureIndex_;
    //�摜�Ɍ��ѕt�����e�N�X�`���ԍ��i�[�pmap
    static std::map<const std::string, uint32_t, std::less<>> textureMap_;
    //�f�t�H���g�e�N�X�`���i�[�f�B���N�g��
    static std::string kDefaultTextureDirectoryPath_;


};


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
    void Initialize();

    /// <summary>
    /// �`��R�}���h�̔��s
    /// </summary>
    /// <param name="cmdList">�R�}���h���X�g</param>
    void Draw(const std::string& fileName);

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



private:

    //�e�N�X�`���o�b�t�@
    ComPtr<ID3D12Resource> texBuff;
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

    //���_�z��
    //std::array<Vertex, kVertexCount_> vertices_{};
    Vertex vertices[kVertexCount_];

    // ���_�o�b�t�@�r���[�̍쐬
    D3D12_VERTEX_BUFFER_VIEW vbView_{};

    //��ʃN���A�J���[
    static const float clearColor[4];

    //�F(RGBA)
    XMFLOAT4 color_ = { 1,1,1,1 };

};


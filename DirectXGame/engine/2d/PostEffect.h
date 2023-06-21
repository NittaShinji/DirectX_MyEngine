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

    //SRV�p�f�X�N���v�^�q�[�v
    ComPtr<ID3D12DescriptorHeap> descHeapSRV;
    //RTN�p�f�X�N���v�^�q�[�v
    ComPtr<ID3D12DescriptorHeap> descHeapRTV;
    //DSV�p�f�X�N���v�^�q�[�v
    ComPtr<ID3D12DescriptorHeap> descHeapDSV;

    //��ʃN���A�J���[
    static const float clearColor[4];
};


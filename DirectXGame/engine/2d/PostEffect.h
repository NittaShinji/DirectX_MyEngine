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
    /// �`��R�}���h�̔��s
    /// </summary>
    /// <param name="cmdList">�R�}���h���X�g</param>
    void Draw(const std::string& fileName);

private:

    //ComPtr<ID3D12PipelineState> pipelineState_ = nullptr;
    //ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
};


#pragma once
#include "Sprite.h"
class PostEffect :
    public Sprite
{
public:

    /// <summary>
    /// コンストラクタ
    /// </summary>
    PostEffect();

    /// <summary>
    /// 描画コマンドの発行
    /// </summary>
    /// <param name="cmdList">コマンドリスト</param>
    void Draw(const std::string& fileName);

private:

    //ComPtr<ID3D12PipelineState> pipelineState_ = nullptr;
    //ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
};


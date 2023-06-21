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
    /// 初期化
    /// </summary>
    void Initialize();

    /// <summary>
    /// 描画コマンドの発行
    /// </summary>
    /// <param name="cmdList">コマンドリスト</param>
    void Draw(const std::string& fileName);

private:

    //テクスチャバッファ
    ComPtr<ID3D12Resource> texBuff;

    //SRV用デスクリプタヒープ
    ComPtr<ID3D12DescriptorHeap> descHeapSRV;

    //ComPtr<ID3D12PipelineState> pipelineState_ = nullptr;
    //ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
};


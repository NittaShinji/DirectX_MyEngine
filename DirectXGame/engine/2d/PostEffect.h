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

    /// <summary>
    /// シーン描画前処理
    /// </summary>
    /// <param name="cmdList"></param>
    void PreDrawScene();

    /// <summary>
    /// シーン描画後処理
    /// </summary>
    /// <param name="cmdList"></param>
    void PostDrawScene();



private:

    //テクスチャバッファ
    ComPtr<ID3D12Resource> texBuff;
    //深度バッファ
    ComPtr<ID3D12Resource> depthBuff;
    //頂点バッファ
    ComPtr<ID3D12Resource> vertBuff_ = nullptr;

    //SRV用デスクリプタヒープ
    ComPtr<ID3D12DescriptorHeap> descHeapSRV;
    //RTN用デスクリプタヒープ
    ComPtr<ID3D12DescriptorHeap> descHeapRTV;
    //DSV用デスクリプタヒープ
    ComPtr<ID3D12DescriptorHeap> descHeapDSV;

    //色用の定数バッファ
    ComPtr<ID3D12Resource> constBuffMaterial_ = nullptr;
    //座標用の定数バッファ
    ComPtr<ID3D12Resource> constBuffTransform_ = nullptr;

    //頂点配列
    //std::array<Vertex, kVertexCount_> vertices_{};
    Vertex vertices[kVertexCount_];

    // 頂点バッファビューの作成
    D3D12_VERTEX_BUFFER_VIEW vbView_{};

    //画面クリアカラー
    static const float clearColor[4];

    //色(RGBA)
    XMFLOAT4 color_ = { 1,1,1,1 };

};


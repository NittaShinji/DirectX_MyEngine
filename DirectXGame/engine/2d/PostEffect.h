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
    void Initialize(DirectXBasic* directXBasic);

    /// <summary>
    /// 描画コマンドの発行
    /// </summary>
    /// <param name="cmdList">コマンドリスト</param>
    void Draw(const std::string& fileName);

    /// <summary>
    /// パイプライン生成
    /// </summary>
    void CreateGraphicsPipelineState();

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

    void LoadTexture(const std::string& fileName);

private:

    DirectXBasic* directXBasic_;

    //テクスチャバッファ
    ComPtr<ID3D12Resource> texBuff;

    //SRVの最大個数
    static const size_t kMaxSRVCount_ = 2056;
    //テクスチャバッファ
    static std::array<ComPtr<ID3D12Resource>, kMaxSRVCount_> textureBuffers_;

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

    //グラフィックスパイプライン
    ComPtr<ID3D12PipelineState> pipelineState_;
    //ルートシグネチャ
    ComPtr<ID3D12RootSignature> rootSignature;

    //頂点配列
    //std::array<Vertex, kVertexCount_> vertices_{};
    Vertex vertices[kVertexCount_];

    // 頂点バッファビューの作成
    D3D12_VERTEX_BUFFER_VIEW vbView_{};

    //画面クリアカラー
    static const float clearColor[4];

    //色(RGBA)
    XMFLOAT4 color_ = { 1,1,1,1 };

    //テクスチャ番号
    static uint32_t sTextureIndex_;
    //画像に結び付いたテクスチャ番号格納用map
    static std::map<const std::string, uint32_t, std::less<>> textureMap_;
    //デフォルトテクスチャ格納ディレクトリ
    static std::string kDefaultTextureDirectoryPath_;


};


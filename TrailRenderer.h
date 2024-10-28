#pragma once
#include "stdint.h"
#include "Trail.h"
#include <d3d12.h>
#include <d3dcompiler.h>  // シェーダーコンパイルが必要なら追加
#include "Camera.h"
#include "Vector4.h"
#include "ObjectAttribute.h"
#include "GameSpeed.h"
#include <forward_list>

class TrailRenderer
{

private:

	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:

	//頂点情報
	struct Vertex
	{
		Vector3 pos;		// xyz座標
		float scale;		// 大きさ
		Vector4 color;		// 色
		float rotate;		// 回転
	};

	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		Matrix4 viewProjection;	// ３Ｄ変換行列
		Matrix4 matBillboard;	// ビルボード行列
	};

public:
    TrailRenderer() {}; //コンストラクタ（Initalizeを呼び出すだけ）
    void Initialize(ID3D12Device* device);  //初期化
	void Update(Camera* camera);
    void Draw();        //描画
	void InitializeGraphicsPipeline();
	//頂点バッファの生成
	void CreateVertBuff();

	//テンプレートコンストラクタ
	template <typename Type1>
	//定数バッファの生成
	ComPtr<ID3D12Resource> CrateConstBuff(Type1* directXBasic_);


    //void Render(uint32_t vertexNum, ID3D11Buffer* vertexBuffer, ID3D11ShaderResourceView* texture);

private:

	//定数バッファのマッピング用ポインタ
	ConstBufferData* constMapData_ = nullptr;

	//グラフィックスパイプライン
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc_{};
	//パープラインステート
	static ComPtr<ID3D12PipelineState> pipelineState_;

	// テクスチャバッファ
	ComPtr<ID3D12Resource> texbuff_;

	//頂点データ配列
	std::vector<Vertex> vertices_;

	// デスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> descHeap_;
	// デスクリプタサイズ
	static UINT descriptorHandleIncrementSize_;

	//デスクリプタヒープハンドル
	static D3D12_CPU_DESCRIPTOR_HANDLE sSrvHandle_;
	//テクスチャ番号
	static uint32_t sTextureIndex_;

	// シェーダリソースビューのハンドル(CPU)
	static D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV_;
	// シェーダリソースビューのハンドル(CPU)
	static D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV_;

	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootSignature_;
	//SRVの最大個数
	static const size_t kMaxSRVCount_ = 2056;
	static const UINT kRenderTexNum = 2;

	//コマンドリスト
	static ID3D12GraphicsCommandList* cmdList_;
	//デバイス
	static ID3D12Device* device_;
	//DirectXの基礎情報
	DirectXBasic* directXBasic_ = nullptr;

	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView_ = {};

protected:

	//ジオメトリシェーダーオブジェクト
	// ビュー行列
	Matrix4 matView_;
	// 射影行列
	Matrix4 matProjection_;
	//定数バッファ
	ComPtr<ID3D12Resource> constBuff_;
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff_;

	// 頂点数
	static const int kVertexCount = 1024;

	//表示する画像名
	std::string trailFileName_;

	//パーティクル配列
	std::forward_list<Trail> trailes_;


    //bool isInitialized = false;
    //ID3D11VertexShader* vertexShader;
    //ID3D11PixelShader* pixelShader;
    //ID3D11BlendState* blendState;
    //ID3D11RasterizerState* rasterizer;
    //ID3D11DepthStencilState* depthStencil;
    //ID3D11InputLayout* layout;

};


#include "Model.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;
using namespace DirectX;

DirectXBasic* Model::directXBasic_ = nullptr;
std::map<Model::MODELKEY, Model::MODELVALUE> Model::models_;

Model::Model()
{
	////ヒープ設定
	//D3D12_HEAP_PROPERTIES cbHeapProp{};				//GPUへの転送用
	//cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	////リソース設定
	//D3D12_RESOURCE_DESC cbResourceDesc{};
	//cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	//cbResourceDesc.Width = (sizeof(ConstBufferDateB1) + 0xff) & ~0xff;	//256バイトアラインメント
	//cbResourceDesc.Height = 1;
	//cbResourceDesc.DepthOrArraySize = 1;
	//cbResourceDesc.MipLevels = 1;
	//cbResourceDesc.SampleDesc.Count = 1;
	//cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	////定数バッファの生成
	//result_ = directXBasic_->GetDevice()->CreateCommittedResource(
	//	&cbHeapProp,//ヒープ設定
	//	D3D12_HEAP_FLAG_NONE,
	//	&cbResourceDesc,//リソース設定
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&infomation_.constBuffB1));
	//assert(SUCCEEDED(result_));

	////定数バッファのマッピング
	//result_ = infomation_.constBuffB1->Map(0, nullptr, (void**)&infomation_.constMapTransform);//マッピング
	//assert(SUCCEEDED(result_));
}

void Model::Load(const std::string& path, DirectXBasic* directXBasic)
{
	Model model;
	model.name_ = path;

	//ファイルストリーム
	std::ifstream file;
	// .OBJファイルを開く
	file.open("Resources/triangle/triangle.obj");
	//ファイルオープン失敗をチェック
	assert(!file.fail());

	vector<XMFLOAT3>positions;	//頂点座標
	vector<XMFLOAT3>normals;	//法線ベクトル
	vector<XMFLOAT2>texcoords;	//テクスチャUV

	//1行ずつ読み込む
	string line;
	while (getline(file, line))
	{
		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		//半角スペース区切りで行の先頭文字列を取得
		string key;
		getline(line_stream, key, ' ');

		//先頭文字列が"v"なら頂点座標
		if (key == "v")
		{
			//X,Y,Z座標読み込み
			XMFLOAT3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			//座標データに追加
			positions.emplace_back(position);
			//頂点データに追加
			Vertex vertex{};
			vertex.pos = position;
			model.infomation_.vertices_.emplace_back(vertex);
		}

		//先頭文字列が"f"ならポリゴン(三角形)
		if (key == "f")
		{
			//半角スペース区切りで行の続きを読み込む
			string index_string;
			while (getline(line_stream,index_string,' '))
			{
				std::istringstream index_stream(index_string);
				unsigned short indexPosition;
				index_stream >> indexPosition;
				//頂点インデックスに追加
				model.infomation_.indices_.emplace_back(indexPosition - 1);
			}
		}
		
		//
		////先頭文字列が"vt"ならテクスチャ
		//if (key == "vt")
		//{
		//	////UV成分読み込み
		//	XMFLOAT2 texcoord{};
		//	line_stream >> texcoord.x;
		//	line_stream >> texcoord.y;
		//	//V方向反転
		//	texcoord.y = 1.0f - texcoord.y;
		//	//テクスチャ座標データに追加
		//	texcoords.emplace_back(texcoord);
		//}
		////先頭文字列が"vf"なら法線ベクトル
		//if (key == "vn")
		//{
		//	////X,Y,Z成分読み込み
		//	XMFLOAT3 normal{};
		//	line_stream >> normal.x;
		//	line_stream >> normal.y;
		//	line_stream >> normal.z;
		//	//法線ベクトルデータに追加
		//	normals.emplace_back(normal);
		//}

		////先頭文字列が"f"ならポリゴン(三角形)
		//if (key == "f")
		//{
		//	//半角スペース区切りで行の続きを読み込む
		//	string index_string;
		//	while (getline(line_stream,index_string,' '))
		//	{
		//		//頂点インデックス１個分の文字列をストリームに変換して解析しやすくする
		//		std::istringstream index_stream(index_string);
		//		unsigned short indexPosition, indexNormal, indexTexcoord;
		//		index_stream >> indexPosition;
		//		index_stream.seekg(1, ios_base::cur);	//スラッシュを飛ばす
		//		index_stream >> indexTexcoord;
		//		index_stream.seekg(1, ios_base::cur);	//スラッシュを飛ばす
		//		index_stream >> indexNormal;
		//		//頂点データ追加
		//		Vertex vertex{};
		//		vertex.pos = positions[indexPosition - 1];
		//		vertex.normal = normals[indexNormal - 1];
		//		vertex.uv = texcoords[indexTexcoord - 1];
		//		model.infomation_.vertices_.emplace_back(vertex);
		//		//インデックスデータの追加
		//		model.infomation_.indices_.emplace_back((unsigned short)model.infomation_.indices_.size());
		//	}
		//}

	}

	file.close();

	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * model.infomation_.vertices_.size());

	// 頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{}; // ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用

	// リソース設定
	D3D12_RESOURCE_DESC resDesc{};

	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // 頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 頂点バッファの生成
	HRESULT result;
	result = directXBasic_->GetResult();
	result = directXBasic_->GetDevice()->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&model.infomation_.vertBuff_));
	assert(SUCCEEDED(result));

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	Vertex* vertMap = nullptr;
	result = model.infomation_.vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	std::copy(model.infomation_.vertices_.begin(), model.infomation_.vertices_.end(), vertMap);
	
	// 繋がりを解除
	model.infomation_.vertBuff_->Unmap(0, nullptr);

	//インデックスデータ全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * model.infomation_.indices_.size());
	
	//リソース設定
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB;	//　インデックス情報が入る分のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//インデックスバッファの設定
	result = directXBasic_->GetDevice()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&model.infomation_.indexBuff)
	);

	//インデックスバッファをマッピング
	uint16_t* indexMap = nullptr;
	result = model.infomation_.indexBuff->Map(0, nullptr, (void**)&indexMap);
	//全インデックスに対して
	std::copy(model.infomation_.indices_.begin(), model.infomation_.indices_.end(), indexMap);

	//マッピング解除
	model.infomation_.indexBuff->Unmap(0, nullptr);

#pragma region 頂点バッファビューの作成

	// GPU仮想アドレス
	model.infomation_.vbView.BufferLocation = model.infomation_.vertBuff_->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	model.infomation_.vbView.SizeInBytes = sizeVB;
	// 頂点1つ分のデータサイズ
	model.infomation_.vbView.StrideInBytes = sizeof(infomation_.vertices_[0]);

#pragma endregion

#pragma region インデックスバッファビューの作成

	//インデックスバッファビューの作成
	//D3D12_INDEX_BUFFER_VIEW ibView{};
	model.infomation_.ibView.BufferLocation = model.infomation_.indexBuff->GetGPUVirtualAddress();
	model.infomation_.ibView.Format = DXGI_FORMAT_R16_UINT;
	model.infomation_.ibView.SizeInBytes = sizeIB;

#pragma endregion

	////デスクリプタヒープの設定
	//D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	//srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	//srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; // シェーダーから見えるように
	//srvHeapDesc.NumDescriptors = kMaxSRVCount;

	////設定を本にSRV用デスクリプタヒープを生成
	//result = directXBasic_->GetDevice()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
	//assert(SUCCEEDED(result));

	////SRVヒープの先頭ハンドルを取得
	//D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

	////デスクリプタのサイズを取得
	//UINT incrementSize = directXBasic_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	////取得したサイズを使用してハンドルを進める
	//for (uint32_t i = 0; i < textureIndex_; i++)
	//{
	//	srvHandle.ptr += incrementSize;
	//}

	////シェーダーリソースビューの設定
	//D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; //設定構造体
	//srvDesc.Format = resDesc.Format;//RGBA float
	//srvDesc.Shader4ComponentMapping =
	//	D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	//srvDesc.Texture2D.MipLevels = resDesc.MipLevels;

	////ハンドルの指す位置にシェーダーリソースビュー作成
	//directXBasic_->GetDevice()->CreateShaderResourceView(textureBuffers_[textureIndex_].Get(), &srvDesc, srvHandle);



	models_.insert_or_assign(model.name_, model.infomation_);
}

void Model::Update()
{
	/*ConstBufferDateB1* constMap1 = nullptr;
	
	HRESULT result = infomation_.constBuffB1->Map(0, nullptr, (void**)&constMap1);
	constMap1->ambient = infomation_.material_.ambient;
	constMap1->diffuse = infomation_.material_.diffuse;
	constMap1->specular= infomation_.material_.specular;
	constMap1->alpha = infomation_.material_.alpha;
	infomation_.constBuffB1->Unmap(0, nullptr);*/
}

void Model::Draw(ID3D12DescriptorHeap* srvHeapHandle)
{
	
}

void Model::StaticInitialize(DirectXBasic* directXBasic)
{
	directXBasic_ = directXBasic;
}

const Model::MODELVALUE* Model::GetMODELVALUE(const MODELKEY path)
{
	return &models_.at(path);
}

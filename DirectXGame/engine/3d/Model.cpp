#include "Model.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <DirectXTex.h>
#include <D3dx12.h>

using namespace std;
using namespace DirectX;

DirectXBasic* Model::directXBasic_ = nullptr;
//std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, Model::kMaxSRVCount> Model::textureBuffers_;
//uint32_t Model::textureIndex_;
std::map<Model::MODELKEY, Model::MODELVALUE> Model::sModels_;
//std::string Model::kDefaultTextureDirectoryPath_ = "Resources/";
//D3D12_CPU_DESCRIPTOR_HANDLE Model::srvHandle;
uint32_t Model::sTextureIndex_ = 0;
D3D12_CPU_DESCRIPTOR_HANDLE Model::sSrvHandle_;

void Model::StaticInitialize(DirectXBasic* directXBasic)
{
	directXBasic_ = directXBasic;
}

void Model::Load(const std::string& path)
{
	Model model;
	model.name_ = path;

	//ファイルストリーム
	std::ifstream file;
	// .OBJファイルを開く
	const string modelFileName = model.name_ + ".obj";
	const string directoryPath = "Resources/" + model.name_ + "/";
	
	file.open(directoryPath + modelFileName);

	//ファイルオープン失敗をチェック
	assert(!file.fail());

	vector<XMFLOAT3>positions;	//頂点座標
	vector<XMFLOAT3>normals;	//法線ベクトル
	vector<XMFLOAT2>texcoords;	//テクスチャUV

	//インデックスカウント用の変数
	//int indexCountTex = 0;

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
			//右手系のモデルデータを左手系に変換(X反転)
			//position.x = -position.x;
			//座標データに追加
			positions.emplace_back(position);
		}
		
		//先頭文字列が"vt"ならテクスチャ
		if (key == "vt")
		{
			////UV成分読み込み
			XMFLOAT2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			//V方向反転
			texcoord.y = 1.0f - texcoord.y;
			//テクスチャ座標データに追加
			texcoords.emplace_back(texcoord);
		}

		//先頭文字列が"vf"なら法線ベクトル
		if (key == "vn")
		{
			////X,Y,Z成分読み込み
			XMFLOAT3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			//法線ベクトルデータに追加
			normals.emplace_back(normal);
		}

		//先頭文字列が"f"ならポリゴン(三角形)
		if (key == "f")
		{
			//int faceIndexCount = 0;
			//半角スペース区切りで行の続きを読み込む
			string index_string;
			//std::array<uint16_t, 4> tempIndices;
			while (getline(line_stream,index_string,' '))
			{
				//頂点インデックス１個分の文字列をストリームに変換して解析しやすくする
				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				index_stream >> indexPosition;
				index_stream.seekg(1, ios_base::cur);	//スラッシュを飛ばす
				index_stream >> indexTexcoord;
				index_stream.seekg(1, ios_base::cur);	//スラッシュを飛ばす
				index_stream >> indexNormal;
				//頂点データ追加
				Vertex vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				model.infomation_.vertices.emplace_back(vertex);
				//インデックスデータの追加
				model.infomation_.indices.emplace_back((unsigned short)model.infomation_.indices.size());

				//assert(faceIndexCount < 4 && "5角形ポリゴン以上は非対応です");

				//// インデックスデータの追加
				//tempIndices[faceIndexCount] = indexCountTex;

				//indexCountTex++;
				//faceIndexCount++;
			}

			//// インデックスデータの順序変更で時計回り→反時計回り変換
			//model.infomation_.vertices_.emplace_back(tempIndices[0]);
			//model.infomation_.vertices_.emplace_back(tempIndices[2]);
			//model.infomation_.vertices_.emplace_back(tempIndices[1]);
			//// 四角形なら三角形を追加
			//if(faceIndexCount == 4)
			//{
			//	model.infomation_.vertices_.emplace_back(tempIndices[0]);
			//	model.infomation_.vertices_.emplace_back(tempIndices[3]);
			//	model.infomation_.vertices_.emplace_back(tempIndices[2]);
			//}
		}

		if (key == "mtllib")
		{
			//マテリアルのファイル読み込み
			string fileName;
			line_stream >> fileName;
			//マテリアルの読み込み
			LoadMaterial(directoryPath, fileName,model);
		}
	}

	file.close();

	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * model.infomation_.vertices.size());

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
		IID_PPV_ARGS(&model.infomation_.vertBuff)); 
	assert(SUCCEEDED(result));

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	Vertex* vertMap = nullptr;
	result = model.infomation_.vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	std::copy(model.infomation_.vertices.begin(), model.infomation_.vertices.end(), vertMap);
	
	// 繋がりを解除
	model.infomation_.vertBuff->Unmap(0, nullptr);

	//インデックスデータ全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * model.infomation_.indices.size());
	
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
	std::copy(model.infomation_.indices.begin(), model.infomation_.indices.end(), indexMap);

	//マッピング解除
	model.infomation_.indexBuff->Unmap(0, nullptr);

#pragma region 頂点バッファビューの作成

	// GPU仮想アドレス
	model.infomation_.vbView.BufferLocation = model.infomation_.vertBuff->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	model.infomation_.vbView.SizeInBytes = sizeVB;
	// 頂点1つ分のデータサイズ
	model.infomation_.vbView.StrideInBytes = sizeof(infomation_.vertices[0]);

#pragma endregion

#pragma region インデックスバッファビューの作成

	//インデックスバッファビューの作成
	//D3D12_INDEX_BUFFER_VIEW ibView{};
	model.infomation_.ibView.BufferLocation = model.infomation_.indexBuff->GetGPUVirtualAddress();
	model.infomation_.ibView.Format = DXGI_FORMAT_R16_UINT;
	model.infomation_.ibView.SizeInBytes = sizeIB;

#pragma endregion
	
	//モデルmapへの挿入
	sModels_.insert_or_assign(model.name_, model.infomation_);
}

void Model::Update(){}

void Model::LoadMaterial(const std::string& directoryPath, const std::string& fileName, Model& model)
{
	const string defaultResourcePath = "Resources/";

	//ファイルストリーム
	std::ifstream file;
	//マテリアルファイルを開く
	file.open(directoryPath + fileName);
	//ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0);
	}

	//1行ずつ読み込む
	string line;
	while (getline(file, line))
	{
		//1行分の文字列をストリームに変換
		std::istringstream line_stream(line);
		//半角スペース区切りで行の先頭文字列を取得
		string key;
		getline(line_stream, key, ' ');

		//先頭のタブ文字は無視する
		if (key[0] == '\t')
		{
			//先頭の文字を削除
			key.erase(key.begin());
		}

		//先頭文字列がnewmtlならマテリアル名
		if (key == "newmtl")
		{
			//マテリアル名読み込み
			line_stream >> model.infomation_.material.name;
		}
		//先頭文字列がKaならアンビエント色
		if (key == "Ka")
		{
			line_stream >> model.infomation_.material.ambient.x;
			line_stream >> model.infomation_.material.ambient.y;
			line_stream >> model.infomation_.material.ambient.z;
		}
		else 
		{
			model.infomation_.material.ambient.x = 1.0f;
			model.infomation_.material.ambient.y = 1.0f;
			model.infomation_.material.ambient.z = 1.0f;
		}

		//先頭文字列がKdならディフューズ色
		if (key == "Kd")
		{
			line_stream >> model.infomation_.material.diffuse.x;
			line_stream >> model.infomation_.material.diffuse.y;
			line_stream >> model.infomation_.material.diffuse.z;
		}
		else 
		{
			model.infomation_.material.diffuse.x = 1.0f;
			model.infomation_.material.diffuse.y = 1.0f;
			model.infomation_.material.diffuse.z = 1.0f;
		}

		//先頭文字列がKdならスペキュラー色
		if (key == "Ks")
		{
			line_stream >> model.infomation_.material.specular.x;
			line_stream >> model.infomation_.material.specular.y;
			line_stream >> model.infomation_.material.specular.z;
		}
		else 
		{
			model.infomation_.material.specular.x = 1.0f;
			model.infomation_.material.specular.y = 1.0f;
			model.infomation_.material.specular.z = 1.0f;
		}

		//先頭文字列がmap_kdならテクスチャファイル名
		if (key == "map_Kd")
		{
			//テクスチャのファイル名読み込み
			line_stream >> model.infomation_.material.textureFilename;
			//テクスチャ読み込み
			LoadTexture(directoryPath, model.infomation_.material.textureFilename,model);
		}
		else 
		{
			model.infomation_.material.textureFilename = "white1x1.png";
			//テクスチャ読み込み
			LoadTexture(defaultResourcePath, "white1x1.png", model);
		}
	}

	//ファイルを閉じる
	file.close();
}

void Model::LoadTexture(const std::string& directoryPath, const std::string& fileName, Model& model)
{
	//ファイルパスを結合
	string filePath = directoryPath + fileName;

	//ユニコード文字列に変換する
	wchar_t wfilePath[128];
	MultiByteToWideChar(CP_ACP, 0, filePath.c_str(), -1, wfilePath, _countof(wfilePath));

	//画像ファイルの用意
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	HRESULT result = LoadFromWICFile(
		wfilePath, WIC_FLAGS_NONE,
		&metadata, scratchImg);

	ScratchImage mipChain{};
	//ミニマップ生成
	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result))
	{
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	//読み込んだディフューズテクスチャをSRGBとして扱う
	metadata.format = MakeSRGB(metadata.format);

	//ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty =
		D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	//CD3DX12_HEAP_PROPERTIES textureHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	
	//リソース設定
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width; // 幅
	textureResourceDesc.Height = (UINT)metadata.height; // 幅
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

	//テクスチャバッファの生成
	result = directXBasic_->GetDevice()->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		//IID_PPV_ARGS(&textureBuffers_[0]));
		IID_PPV_ARGS(&model.infomation_.textureBuffers[sTextureIndex_]));

	textureHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用

	//全ミニマップについて
	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		//ミニマップレベルを指定してイメージを取得
		const Image* img = scratchImg.GetImage(i, 0, 0);

		//テクスチャバッファにデータ転送

		result = model.infomation_.textureBuffers[sTextureIndex_]->WriteToSubresource(

			(UINT)i,
			nullptr,
			img->pixels,
			(UINT)img->rowPitch,
			(UINT)img->slicePitch
		);

		assert(SUCCEEDED(result));
	}

	//デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; // シェーダーから見えるように
	srvHeapDesc.NumDescriptors = kMaxSRVCount_;

	//設定を本にSRV用デスクリプタヒープを生成
	result = directXBasic_->GetDevice()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&model.infomation_.srvHeap));
	assert(SUCCEEDED(result));

	//SRVヒープの先頭ハンドルを取得
	sSrvHandle_ = model.infomation_.srvHeap->GetCPUDescriptorHandleForHeapStart();

	//デスクリプタのサイズを取得
	UINT incrementSize = directXBasic_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//取得したサイズを使用してハンドルを進める
	for (uint32_t i = 0; i < sTextureIndex_; i++)
	{
		sSrvHandle_.ptr += incrementSize;
	}

	//シェーダーリソースビューの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; //設定構造体
	srvDesc.Format = textureResourceDesc.Format;//RGBA float
	srvDesc.Shader4ComponentMapping =
		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

	//ハンドルの指す位置にシェーダーリソースビュー作成
	directXBasic_->GetDevice()->CreateShaderResourceView(model.infomation_.textureBuffers[sTextureIndex_].Get(), &srvDesc, sSrvHandle_);
	
	//画像番号を進める
	//textureIndex_++;
}

//検索キー(パス)から値を検索
const Model::MODELVALUE* Model::GetMODELVALUE(const MODELKEY path)
{
	return &sModels_.at(path);
}

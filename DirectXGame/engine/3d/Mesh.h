#pragma once
#include "DirectXBasic.h"
#include "Vector2.h"
#include <DirectXMath.h>
#include <vector>
#include <wrl.h>

class Mesh
{

private:

	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	//DirectX���ȗ�
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	//using Vector2 = DirectX::Vector2;
	using XMMATRIX = DirectX::XMMATRIX;

public:

	//���_���
	struct Vertex
	{
		XMFLOAT3 pos;		// xyz���W
		XMFLOAT3 normal;	//�@���x�N�g��
		Vector2 uv;		// uv���W
	};

public:

	static void StaticInitialize(DirectXBasic* directXbasic);

	//���_�����Z�b�g
	void SetVertices(const Vertex& vertex);

	//���_�C���f�b�N�X���Z�b�g
	void SetIndices(const unsigned short& index);

	void CrateBuffer();

	/// <summary>
	/// ���_�z����擾
	/// </summary>
	/// <returns>���_�z��</returns>
	inline const std::vector<Vertex>& GetVertices() { return vertices_; }

	/// <summary>
	/// �C���f�b�N�X�z����擾
	/// </summary>
	/// <returns>�C���f�b�N�X�z��</returns>
	inline const std::vector<unsigned short>& GetIndices() { return indices_; }

	/// <summary>
	/// ���_�o�b�t�@�擾
	/// </summary>
	/// <returns>���_�o�b�t�@</returns>
	inline const D3D12_VERTEX_BUFFER_VIEW& GetVBView() { return vbView_; }

	/// <summary>
	/// �C���f�b�N�X�o�b�t�@�擾
	/// </summary>
	/// <returns>�C���f�b�N�X�o�b�t�@</returns>
	inline const D3D12_INDEX_BUFFER_VIEW& GetIBView() { return ibView_; }

private:

	static DirectXBasic* directXBasic_;

	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff_;
	// �C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff_;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView_ = {};
	// �C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView_ = {};
	//���_�f�[�^�z��
	std::vector<Vertex> vertices_;
	//���_�C���f�b�N�X�z��
	std::vector<unsigned short> indices_;

};


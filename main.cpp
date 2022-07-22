#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <vector>
#include <string>
#include <random>
#include <DirectXMath.h>
#include <DirectXTex.h>
#include <wrl.h>
using namespace DirectX;
using namespace Microsoft::WRL;
#include <d3dcompiler.h>
#define DIRECTINPUT_VERSION 0x0800 //DirectInput�̃o�[�W�����w��
#include <dinput.h>
#include "KeyInput.h"

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")


void CreateConstantBuffer();
long ConstantBufferResult(long result, ID3D12Device* device, D3D12_HEAP_PROPERTIES cbHeapProp,
	D3D12_RESOURCE_DESC cbResourceDesc, ID3D12Resource* constBuffTransform);

//�萔�o�b�t�@�p�f�[�^�\����(�}�e���A��)
struct ConstBufferDataMaterial
{
	XMFLOAT4 color;	//�F(RGBA)
};

//�萔�o�b�t�@�p�f�[�^�\����(3D�ϊ��s��)
struct ConstBufferDataTransform
{
	XMMATRIX mat;	// 3D�ϊ��s��
};

struct Object3d
{
	//�萔�o�b�t�@(�s��p)
	ComPtr<ID3D12Resource> constBuffTransform;
	//�萔�o�b�t�@�}�b�v(�s��p)
	ConstBufferDataTransform* constMapTransform;

	//�A�t�B���ϊ����
	XMFLOAT3 scale = { 1,1,1 };
	XMFLOAT3 rotation = { 0,0,0 };
	XMFLOAT3 position = { 0,0,0 };
	//���[���h�ϊ��s��
	XMMATRIX matworld;
	//�e�I�u�W�F�N�g�̃|�C���^
	Object3d* parent = nullptr;

};

void InitializeObject3d(Object3d* object, ID3D12Device* device);
void UpdateObject3d(Object3d* object, XMMATRIX& matview, XMMATRIX& matProjection);
void DrawObject3d(Object3d* object, ID3D12GraphicsCommandList* commandList, D3D12_VERTEX_BUFFER_VIEW& vbView,
	D3D12_INDEX_BUFFER_VIEW& ibView, UINT numIndices);

//�E�B���h�E�v���V�[�W��
LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//���b�Z�[�W�ɉ����ăQ�[���ŗL�̏������s��
	switch (msg)
	{
		//�E�C���h�E���j�����ꂽ
	case WM_DESTROY:
		//OS�ɑ΂��āA�A�v���̏I����`����
		PostQuitMessage(0);
		return 0;
	}

	//�W���̃��b�Z�[�W�������s��
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

//Windows�A�v���ł̃G���g���[�|�C���g(main�֐�)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
#pragma region WindowsAPI����������
#pragma region �E�C���h�E�̐ݒ� (P01_01)
	//�@�Q�[���S�̂̏�����
	//�E�C���h�E�T�C�Y
	const int window_width = 1280; //����
	const int window_height = 720; //�c��

	// �E�B���h�E�N���X�̐ݒ�
	WNDCLASSEX w{};
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc; // �E�B���h�E�v���V�[�W����ݒ�
	w.lpszClassName = L"DirectXGame"; // �E�B���h�E�N���X��
	w.hInstance = GetModuleHandle(nullptr); // �E�B���h�E�n���h��
	w.hCursor = LoadCursor(NULL, IDC_ARROW); // �J�[�\���w��

	//�@�E�C���h�E�N���X��OS�ɓo�^����
	RegisterClassEx(&w);
	// �E�C���h�E�T�C�Y{X���W�@Y���W�@�����@�c��}
	RECT wrc = { 0,0,window_width,window_height };
	//�����ŃT�C�Y��␳����
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);
#pragma endregion 

#pragma region �E�C���h�E�̐��� (P01_01)
	// �E�B���h�E�I�u�W�F�N�g�̐���
	HWND hwnd = CreateWindow(w.lpszClassName, // �N���X��
		L"DirectXGame", // �^�C�g���o�[�̕���
		WS_OVERLAPPEDWINDOW, // �W���I�ȃE�B���h�E�X�^�C��
		CW_USEDEFAULT, // �\��X���W(OS�ɔC����)
		CW_USEDEFAULT, // �\��Y���W(OS�ɔC����)
		wrc.right - wrc.left, // �E�B���h�E����
		wrc.bottom - wrc.top, // �E�B���h�E�c��
		nullptr, // �e�E�B���h�E�n���h��
		nullptr, // ���j���[�n���h��
		w.hInstance, // �Ăяo���A�v���P�[�V�����n���h��
		nullptr); // �I�v�V����
		// �E�B���h�E��\����Ԃɂ���
	ShowWindow(hwnd, SW_SHOW);

#pragma endregion 

	MSG msg{}; // ���b�Z�[�W
#pragma endregion WindowsAPI����������
#pragma region DirectX����������
	//DirectX���������� ��������

#ifdef _DEBUG
//�f�o�b�O���C���[���I����
	ComPtr<ID3D12Debug1> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		debugController->EnableDebugLayer();
		debugController->SetEnableSynchronizedCommandQueueValidation(TRUE);
	}
#endif

	HRESULT result;
	
	ComPtr<ID3D12Device> device;
	ComPtr<IDXGIFactory7> dxgiFactory;
	ComPtr<IDXGISwapChain4> swapChain;
	ComPtr<ID3D12CommandAllocator> commandAllocator;
	//ComPtr<ID3D12GraphicsCommandList> commandList;
	ComPtr<ID3D12CommandQueue> commandQueue;
	ComPtr<ID3D12DescriptorHeap> rtvHeap;

	ID3D12GraphicsCommandList* commandList = nullptr;

#pragma region �A�_�v�^�̗�(P01_02)

	// DXGI�t�@�N�g���[�̐���
	result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	assert(SUCCEEDED(result));
	// �A�_�v�^�[�̗񋓗p
	std::vector<ComPtr<IDXGIAdapter4>> adapters;

	// �����ɓ���̖��O�����A�_�v�^�[�I�u�W�F�N�g������
	ComPtr<IDXGIAdapter4> tmpAdapter;
	// �p�t�H�[�}���X���������̂��珇�ɁA�S�ẴA�_�v�^�[��񋓂���
	for (UINT i = 0;
		dxgiFactory->EnumAdapterByGpuPreference(i,
			DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
			IID_PPV_ARGS(&tmpAdapter)) != DXGI_ERROR_NOT_FOUND;
		i++) {
		// ���I�z��ɒǉ�����
		adapters.push_back(tmpAdapter);
	}

	// �Ó��ȃA�_�v�^��I�ʂ���
	for (size_t i = 0; i < adapters.size(); i++) {
		DXGI_ADAPTER_DESC3 adapterDesc;
		// �A�_�v�^�[�̏����擾����
		adapters[i]->GetDesc3(&adapterDesc);
		// �\�t�g�E�F�A�f�o�C�X�����
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			// �f�o�C�X���̗p���ă��[�v�𔲂���
			tmpAdapter = adapters[i];
			break;
		}
	}

#pragma endregion
#pragma region �f�o�C�X�̐���(P01_02)

	// �Ή����x���̔z��
	D3D_FEATURE_LEVEL levels[] = {
	D3D_FEATURE_LEVEL_12_1,
	D3D_FEATURE_LEVEL_12_0,
	D3D_FEATURE_LEVEL_11_1,
	D3D_FEATURE_LEVEL_11_0,
	};

	D3D_FEATURE_LEVEL featureLevel;
	for (size_t i = 0; i < _countof(levels); i++) {
		// �̗p�����A�_�v�^�[�Ńf�o�C�X�𐶐�
		result = D3D12CreateDevice(tmpAdapter.Get(), levels[i],
			IID_PPV_ARGS(&device));
		if (result == S_OK) {
			// �f�o�C�X�𐶐��ł������_�Ń��[�v�𔲂���
			featureLevel = levels[i];
			break;
		}
	}

#pragma endregion 


#ifdef _DEBUG
	ComPtr<ID3D12InfoQueue> infoQueue;
	if (SUCCEEDED(device->QueryInterface(IID_PPV_ARGS(&infoQueue))))
	{
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
	}
#endif

#pragma region �R�}���h���X�g��GPU�ɂ܂Ƃ߂Ė��߂𑗂邽�� (P01_02)

	// �R�}���h�A���P�[�^�𐶐�
	result = device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&commandAllocator));
	assert(SUCCEEDED(result));
	// �R�}���h���X�g�𐶐�
	result = device->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		commandAllocator.Get(), nullptr,
		IID_PPV_ARGS(&commandList));
	assert(SUCCEEDED(result));

#pragma endregion
#pragma region �R�}���h�L���[�̐���(P01_02)


	//�R�}���h�L���[�̐ݒ�
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	//�R�}���h�L���[�𐶐�
	result = device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue));
	assert(SUCCEEDED(result));

#pragma endregion 
#pragma region �X���b�v�`�F�[�� (P01_02)

	// �X���b�v�`�F�[���̐ݒ�
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Width = 1280;
	swapChainDesc.Height = 720;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // �F���̏���
	swapChainDesc.SampleDesc.Count = 1; // �}���`�T���v�����Ȃ�
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER; // �o�b�N�o�b�t�@�p
	swapChainDesc.BufferCount = 2; // �o�b�t�@����2�ɐݒ�
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // �t���b�v��͔j��
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	//IDXGISwapChain��ComPtr��p��
	ComPtr<IDXGISwapChain1> swapChain1;

	// �X���b�v�`�F�[���̐���
	result = dxgiFactory->CreateSwapChainForHwnd(
		commandQueue.Get(), hwnd, &swapChainDesc, nullptr, nullptr,&swapChain1);
	assert(SUCCEEDED(result));

	//��������IDXGISwapChain1�̃I�u�W�F�N�g��IDXGISwapChain4�ɕϊ�����
	swapChain1.As(&swapChain);

#pragma endregion 
#pragma region �f�X�N���v�^�q�[�v�̐��� (P01_02)
	// �f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // �����_�[�^�[�Q�b�g�r���[
	rtvHeapDesc.NumDescriptors = swapChainDesc.BufferCount; // ���\��2��
	// �f�X�N���v�^�q�[�v�̐���
	device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap));

#pragma endregion
#pragma region �o�b�N�o�b�t�@(P01_02)

	// �o�b�N�o�b�t�@
	std::vector<ComPtr<ID3D12Resource>> backBuffers;

	backBuffers.resize(swapChainDesc.BufferCount);
#pragma endregion 
#pragma region �����_�[�^�[�Q�[�g�r���[(RTV)�̐��� (P01_02)

	// �X���b�v�`�F�[���̑S�Ẵo�b�t�@�ɂ��ď�������
	for (size_t i = 0; i < backBuffers.size(); i++) {
		// �X���b�v�`�F�[������o�b�t�@���擾
		swapChain->GetBuffer((UINT)i, IID_PPV_ARGS(&backBuffers[i]));
		// �f�X�N���v�^�q�[�v�̃n���h�����擾
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
		// �����\���ŃA�h���X�������
		rtvHandle.ptr += i * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
		// �����_�[�^�[�Q�b�g�r���[�̐ݒ�
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
		// �V�F�[�_�[�̌v�Z���ʂ�SRGB�ɕϊ����ď�������
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		// �����_�[�^�[�Q�b�g�r���[�̐���
		device->CreateRenderTargetView(backBuffers[i].Get(), &rtvDesc, rtvHandle);
	}

#pragma endregion 

#pragma region	�[�x�o�b�t�@�̃��\�[�X�ݒ�

	D3D12_RESOURCE_DESC depthResourceDesc{};
	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = window_width;								//�����_�[�^�[�Q�b�g�ɍ��킹��
	depthResourceDesc.Height = window_height;							//�����_�[�^�[�Q�b�g�ɍ��킹��
	depthResourceDesc.DepthOrArraySize = 1;
	depthResourceDesc.Format = DXGI_FORMAT_D32_FLOAT;					//�[�x�l�t�H�[�}�b�g
	depthResourceDesc.SampleDesc.Count = 1;
	depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;	//�f�v�X�X�e���V��

	//�[�x�l�p�q�[�v�v���p�e�B
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	//�[�x�l�̃N���A�ݒ�
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;		//�[�x�l1.0f(�ő�l)�ŃN���A
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT; //�[�x�l�t�H�[�}�b�g

#pragma endregion 

#pragma region �[�x�o�b�t�@�̃��\�[�X�ݒ�

	ComPtr<ID3D12Resource> depthBuff;
	//ID3D12Resource* depthBuff = nullptr;
	
	result = device->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,//�[�x�l�������݂Ɏg�p
		&depthClearValue,
		IID_PPV_ARGS(&depthBuff)
	);

#pragma endregion 

#pragma region �t�F���X (P01_02)

	// �t�F���X�̐���
	ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal = 0;
	result = device->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
#pragma endregion 
#pragma region �L�[���� (P02_03)

	//�L�[���̓N���X�̍쐬
	KeyInput* keyInput = new KeyInput();

	keyInput->Initialize(w.hInstance, hwnd);
#pragma endregion

	//DirectX���������� �����܂�
#pragma endregion DirectX����������

#pragma region �`�揉��������

#pragma region �萔�o�b�t�@�p�f�[�^�\���̂�錾

#pragma endregion
	// 3D�I�u�W�F�N�g�̐�
	const size_t kObjectCount = 50;
	// 3D�I�u�W�F�N�g�̔z��
	Object3d object3ds[kObjectCount];

#pragma region �}�e���A���p�̒萔�o�b�t�@�̏�����(P03_02)

	//�萔�o�b�t�@��GPU���\�[�X�̃|�C���^
	ComPtr<ID3D12Resource> constBuffMaterial;
	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};				//GPU�ւ̓]���p
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;	//256�o�C�g�A���C�������g
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//�萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&cbHeapProp,//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial));
	assert(SUCCEEDED(result));

#pragma endregion

#pragma region �}�e���A���p�̒萔�o�b�t�@�̓]��(P03_02)
	//�萔�o�b�t�@�̃}�b�s���O
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);//�}�b�s���O
	assert(SUCCEEDED(result));

	float colorR = 1.0f;
	float colorG = 1.0f;
	float colorB = 1.0f;

	//�l���������ނƎ����I�ɓ]�������
	constMapMaterial->color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

#pragma endregion
	// �}�`�̌`��ω��t���O
	bool formchange = 0;

#pragma region �O�����p�̒萔�o�b�t�@�̏�����(P05_02)

	//�萔�o�b�t�@��GPU���\�[�X�̃|�C���^
	ComPtr<ID3D12Resource> constBuffTransform0;
	//�萔�o�b�t�@�̃}�b�s���O�p�|�C���^
	ConstBufferDataTransform* constMapTransform0 = nullptr;

	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp2{};				//GPU�ւ̓]���p
	cbHeapProp2.Type = D3D12_HEAP_TYPE_UPLOAD;
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc2{};
	cbResourceDesc2.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc2.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;	//256�o�C�g�A���C�������g
	cbResourceDesc2.Height = 1;
	cbResourceDesc2.DepthOrArraySize = 1;
	cbResourceDesc2.MipLevels = 1;
	cbResourceDesc2.SampleDesc.Count = 1;
	cbResourceDesc2.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//�萔�o�b�t�@�̐���
	//result = device->CreateCommittedResource(
	//	&cbHeapProp2,//�q�[�v�ݒ�
	//	D3D12_HEAP_FLAG_NONE,
	//	&cbResourceDesc2,//���\�[�X�ݒ�
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&constBuffTransform0));
	//assert(SUCCEEDED(result));

#pragma endregion

#pragma region �O�����p�̒萔�o�b�t�@�̓]��(P05_02)

	//�萔�o�b�t�@�̃}�b�s���O
	//ConstBufferDataTransform* constMapTransform = nullptr;
	//result = constBuffTransform0->Map(0, nullptr, (void**)&constMapTransform0);//�}�b�s���O
	//assert(SUCCEEDED(result));

	//�P�ʍs�����
	//constMapTransform0->mat = XMMatrixIdentity();
	////�O��̎��Ōv�Z�����s��
	//constMapTransform->mat.r[0].m128_f32[0] = 2.0f / window_width;		//�E�C���h�E����
	//constMapTransform->mat.r[1].m128_f32[1] = -2.0f	/ window_height;	//�E�C���h�E�c��
	////��ʔ����̕��s�ړ�(05_02_P32)
	//constMapTransform->mat.r[3].m128_f32[0] = -1.0f;	
	//constMapTransform->mat.r[3].m128_f32[1] = 1.0f;

	//���s���e�ϊ�
	//constMapTransform->mat = XMMatrixOrthographicOffCenterLH(0.0f, window_width, window_height, 0.0f, 0.0f, 1.0f);


	//�r���[�ϊ��s��
	XMMATRIX matView;
	XMFLOAT3 eye(0, 0, -100);	//���_���W
	XMFLOAT3 target(0, 0, 0);	//�����_���W
	XMFLOAT3 up(0, 1, 0);		//������x�N�g��
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

	float angle = 0.0f;	//�J�����̉�]�p

	//�ˉe�ϊ��s��
	XMMATRIX matProjection =
		XMMatrixPerspectiveFovLH(
			XMConvertToRadians(45.0f),				//�㉺��p45�x
			(float)window_width / window_height,	//�A�X�y�N�g��(��ʉ���/��ʏc��)
			0.1f, 1000.0f							//�O�[,���[
		);

	//�X�P�[�����O�{��
	XMFLOAT3 scale;
	//��]�p
	XMFLOAT3 rotation;
	//���W
	XMFLOAT3 position;

	scale = { 1.0f,1.0f,1.0f };
	rotation = { 0.0f,0.0f,0.0f };
	position = { 0.0f,0.0f,0.0f };

	//�O��̎��Ōv�Z�����s��
	//XMMATRIX oldVer = XMMatrixIdentity();
	//oldVer.r[0].m128_f32[0] = 2.0f / window_width;		//�E�C���h�E����
	//oldVer.r[1].m128_f32[1] = -2.0f	/ window_height;	//�E�C���h�E�c��
	////��ʔ����̕��s�ړ�(05_02_P32)
	//oldVer.r[3].m128_f32[0] = -1.0f;
	//oldVer.r[3].m128_f32[1] = 1.0f;
	////����̊֐��Ōv�Z�����s��
	//XMMATRIX newVer = XMMatrixOrthographicOffCenterLH(0.0f, window_width, window_height, 0.0f, 0.0f, 1.0f);
	//constMapTransform->mat = XMMatrixOrthographicOffCenterLH(0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f);

#pragma endregion

#pragma region ��ڂ̃I�u�W�F�N�g�p�̒萔�o�b�t�@

	//�萔�o�b�t�@��GPU���\�[�X�̃|�C���^
	ComPtr<ID3D12Resource> constBuffTransform1;
	//�萔�o�b�t�@�̃}�b�s���O�p�|�C���^
	ConstBufferDataTransform* constMapTransform1 = nullptr;

	//�萔�o�b�t�@�̐���
	//result = device->CreateCommittedResource(
	//	&cbHeapProp2,//�q�[�v�ݒ�
	//	D3D12_HEAP_FLAG_NONE,
	//	&cbResourceDesc2,//���\�[�X�ݒ�
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&constBuffTransform1));
	//assert(SUCCEEDED(result));

	//�萔�o�b�t�@�̃}�b�s���O
	//result = constBuffTransform1->Map(0, nullptr, (void**)&constMapTransform1);//�}�b�s���O
	//assert(SUCCEEDED(result));

	//�X�P�[�����O�{��
	XMFLOAT3 scale1;
	//��]�p
	XMFLOAT3 rotation1;
	//���W
	XMFLOAT3 position1;

	scale1 = { 1.0f,1.0f,1.0f };
	rotation1 = { 0.0f,0.0f,0.0f };
	position1 = { 0.0f,0.0f,0.0f };

#pragma endregion

	//�z����̑S�I�u�W�F�N�g�ɑ΂���
	for (int i = 0; i < _countof(object3ds); i++)
	{
		//������
		InitializeObject3d(&object3ds[i], device.Get());

		//�e�\���̃T���v��
		//�擪�ȊO�Ȃ�
		if (i > 0)
		{
			//�e�I�u�W�F�N�g�̐ݒ�
			object3ds[i].parent = &object3ds[0];
			//�e�I�u�W�F�N�g�ɑ΂���傫��
			object3ds[i].scale = { 1.0f,1.0f,1.0f };
			//�e�I�u�W�F�N�g�ɑ΂����]
			object3ds[i].rotation = { 0.0f,0.0f,0.0f };
			//�e�I�u�W�F�N�g�ɑ΂���Z����-8.0���炷
			//object3ds[i].position = { 0.0f,0.0f,-8.0f };
		}

		object3ds[1].position = { 10.0f,0.0f,0.0f };
		object3ds[2].position = { 20.0f,0.0f,0.0f };
		object3ds[3].position = { -10.0f,0.0f,0.0f };
		object3ds[4].position = { -20.0f,0.0f,0.0f };
		object3ds[5].position = { 0.0f,10.0f,0.0f };
		object3ds[6].position = { 0.0f,-10.0f,0.0f };
		object3ds[7].position = { 0.0f,-20.0f,0.0f };
		object3ds[8].position = { 10.0f,-30.0f,0.0f };
		object3ds[9].position = { 10.0f,-40.0f,0.0f };
		object3ds[10].position = { -10.0f,-30.0f,0.0f };
		object3ds[11].position = { -10.0f,-40.0f,0.0f };
	}

#pragma	region ���_�f�[�^(P02_01)

	//���_�f�[�^�\����
	struct Vertex
	{
		XMFLOAT3 pos;		//xyz���W
		XMFLOAT3 normal;	//�@���x�N�g��
		XMFLOAT2 uv;		//�摜����xyz���W
	};

	// ���_�f�[�^
	Vertex vertices[] = {
		//	x		y	 z			u	v
		// �O
		{{ -5.0f, -5.0f, -5.0f },{}, {0.0f,1.0f}},// ���� 
		{{ -5.0f, 5.0f, -5.0f }, {}, {0.0f,0.0f}},// ���� 
		{{ 5.0f, -5.0f, -5.0f }, {}, {1.0f,1.0f}},// �E�� 
		{{ 5.0f, 5.0f, -5.0f }, {}, {1.0f,0.0f}},// �E�� 
		// ��(�O�ʂ�Z���W�̕������t)
		{{ -5.0f, -5.0f, 5.0f }, {}, {0.0f,1.0f}},// ���� 
		{{ -5.0f, 5.0f, 5.0f }, {}, {0.0f,0.0f}},// ���� 
		{{ 5.0f, -5.0f, 5.0f }, {}, {1.0f,1.0f}},// �E�� 
		{{ 5.0f, 5.0f, 5.0f }, {}, {1.0f,0.0f}},// �E�� 

		// ��
		{{ -5.0f, -5.0f, -5.0f },{}, {0.0f,1.0f}},// ���� 
		{{ -5.0f, 5.0f, -5.0f },{}, {0.0f,0.0f}},// ���� 
		{{ -5.0f, -5.0f, 5.0f },{}, {1.0f,1.0f}},// �E�� 
		{{ -5.0f, 5.0f, 5.0f }, {},{1.0f,0.0f}},// �E�� 

		// �E(���ʂ�X���W�̕������t)
		{{ 5.0f, -5.0f, -5.0f },{}, {0.0f,1.0f}},// ���� 
		{{ 5.0f, 5.0f, -5.0f },{}, {0.0f,0.0f}},// ���� 
		{{ 5.0f, -5.0f, 5.0f },{}, {1.0f,1.0f}},// �E�� 
		{{ 5.0f, 5.0f, 5.0f }, {},{1.0f,0.0f}},// �E�� 

		// ��
		{{ -5.0f, -5.0f, -5.0f },{}, {0.0f,1.0f}},// ���� 
		{{ -5.0f, -5.0f, 5.0f },{}, {0.0f,0.0f}},// ���� 
		{{ 5.0f, -5.0f, -5.0f },{}, {1.0f,1.0f}},// �E�� 
		{{ 5.0f, -5.0f, 5.0f },{}, {1.0f,0.0f}},// �E�� 
		// ��(���ʂ�Y���W�̕������t)
		{{ -5.0f, 5.0f, -5.0f },{}, {0.0f,1.0f}},// ���� 
		{{ -5.0f, 5.0f, 5.0f },{},{0.0f,0.0f}},// ���� 
		{{ 5.0f, 5.0f, -5.0f },{}, {1.0f,1.0f}},// �E�� 
		{{ 5.0f, 5.0f, 5.0f },{}, {1.0f,0.0f}},// �E�� 

	};

	//uint16_t indices[] =
	//{
	//	0,1,2, //�O�p�`1��
	//	1,2,3, //�O�p�`2��
	//};

	unsigned short indices[] =
	{
		//�O
		0,1,2,		//�O�p�`1��
		2,1,3,		//�O�p�`2��
		//��(�O�̖ʂ�4���Z)
		6,5,4,		//�O�p�`3��
		6,7,5,		//�O�p�`4��

		//��
		10,9,8,		//�O�p�`5��
		10,11,9,	//�O�p�`6��
		//�E
		12,13,14,	//�O�p�`7��
		14,13,15,	//�O�p�`8��

		//��
		18,17,16,	//�O�p�`5��
		18,19,17,	//�O�p�`6��
		//��
		20,21,22,	//�O�p�`7��
		22,21,23,	//�O�p�`8��
	};

	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));

#pragma endregion 

#pragma region ���_�o�b�t�@�̊m��(P02_01)

	// ���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{}; // �q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p

	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // ���_�f�[�^�S�̂̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// ���_�o�b�t�@�̐���
	ComPtr<ID3D12Resource> vertBuff;
	//ID3D12Resource* vertBuff = nullptr;
	result = device->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

#pragma endregion 

#pragma region �@���̌v�Z

	for (int i = 0; i < _countof(indices) / 3; i++)
	{
		//�O�p�`1���ƂɌv�Z���Ă���
		//�O�p�`�̃C���f�b�N�X�����o���āA�ꎞ�I�ȕϐ��ɓ����
		unsigned short indices0 = indices[i * 3 + 0];
		unsigned short indices1 = indices[i * 3 + 1];
		unsigned short indices2 = indices[i * 3 + 2];
		//�O�p�`���\�����钸�_���W���x�N�g���ɑ��
		XMVECTOR p0 = XMLoadFloat3(&vertices[indices0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[indices1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[indices2].pos);
		//p0��p1�x�N�g���Ap0��p2�x�N�g�����v�Z(�x�N�g���̌��Z)
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);
		//�O�ς͗������琂���ȃx�N�g��
		XMVECTOR normal = XMVector3Cross(v1, v2);
		//���K��(������1�ɂ���)
		normal = XMVector3Normalize(normal);
		//���߂��@���𒸓_�f�[�^�ɑ��
		XMStoreFloat3(&vertices[indices0].normal, normal);
		XMStoreFloat3(&vertices[indices1].normal, normal);
		XMStoreFloat3(&vertices[indices2].normal, normal);
	}

#pragma endregion 

#pragma region ���_�o�b�t�@�ւ̃f�[�^�]��(P02_01)

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	Vertex* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// �S���_�ɑ΂���
	for (int i = 0; i < _countof(vertices); i++) {
		vertMap[i] = vertices[i]; // ���W���R�s�[
	}

	// �q���������
	vertBuff->Unmap(0, nullptr);
#pragma endregion

#pragma region �C���f�b�N�X�o�b�t�@�̐ݒ�(P03_04)
	//�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));

	//���\�[�X�ݒ�
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB;	//�@�C���f�b�N�X��񂪓��镪�̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//�C���f�b�N�X�o�b�t�@�̐ݒ�
	ComPtr<ID3D12Resource> indexBuff;
	result = device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff)
	);

#pragma endregion

#pragma region �C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��(P03_04)

	//�C���f�b�N�X�o�b�t�@���}�b�s���O
	uint16_t* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	//�S�C���f�b�N�X�ɑ΂���
	for (int i = 0; i < _countof(indices); i++)
	{
		indexMap[i] = indices[i];	//�C���f�b�N�X���R�s�[
	}

	//�}�b�s���O����
	indexBuff->Unmap(0, nullptr);

#pragma endregion

#pragma region �C���f�b�N�X�o�b�t�@�r���[�̍쐬(P03_04)

	//�C���f�b�N�X�o�b�t�@�r���[�̍쐬
	D3D12_INDEX_BUFFER_VIEW ibView{};
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

#pragma endregion

#pragma region ���_�o�b�t�@�r���[�̍쐬(P02_01)
	// ���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	// GPU���z�A�h���X
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	vbView.SizeInBytes = sizeVB;
	// ���_1���̃f�[�^�T�C�Y
	vbView.StrideInBytes = sizeof(vertices[0]);

#pragma endregion
#pragma region ���_�V�F�[�_�t�@�C���̓ǂݍ��݂ƃR���p�C��(P02_01)

	ID3DBlob* vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
	ID3DBlob* psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ID3DBlob* errorBlob = nullptr; // �G���[�I�u�W�F�N�g
	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"BasicVS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);
	// �G���[�Ȃ�
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}

#pragma endregion

#pragma region �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��(P02_01)

	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"BasicPS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob);

	// �G���[�Ȃ�
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}
#pragma endregion

#pragma region ���_���C�A�E�g(P02_01)
	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{
			//xyz���W
		"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, //�ǂꂮ�炢�̗ʂ𑗂邩
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},

		{
			//�@���x�N�g��(1�s�ŏ������ق����₷��)
			"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},

		// uv���W
		{
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		//���W�ȊO�ɁA�F�A�e�N�X�`��UV�Ȃǂ�n���ꍇ�͂���ɑ�����
	};
#pragma endregion

#pragma region �摜�ǂݍ���
	//�摜�t�@�C���̗p��
	TexMetadata metadata{};
	ScratchImage scratchImg{};
	// WIC�e�N�X�`���̃��[�h
	result = LoadFromWICFile(
		L"Resources/dan.jpg",	//�uResources�v�t�H���_�́udan.jpg�v
		WIC_FLAGS_NONE,
		&metadata, scratchImg);

	ScratchImage mipChain{};
	//�~�j�}�b�v����
	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result))
	{
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	//�ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	metadata.format = MakeSRGB(metadata.format);

#pragma endregion

#pragma region �摜�ǂݍ��ݓ񖇖�

	//2���ڗp�ɕʂ̕ϐ���p�ӂ��Ă���
	TexMetadata metadata2{};
	ScratchImage scratchImg2{};
	// WIC�e�N�X�`���̃��[�h
	result = LoadFromWICFile(
		L"Resources/reimu.png",
		WIC_FLAGS_NONE,
		&metadata2, scratchImg2);

	ScratchImage mipChain2{};
	//�~�j�}�b�v����
	result = GenerateMipMaps(
		scratchImg2.GetImages(), scratchImg2.GetImageCount(), scratchImg2.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain2);
	if (SUCCEEDED(result))
	{
		scratchImg2 = std::move(mipChain2);
		metadata2 = scratchImg2.GetMetadata();
	}

	//�ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	metadata2.format = MakeSRGB(metadata2.format);


#pragma endregion

#pragma region �摜�ǂݍ��ݎO����

	//2���ڗp�ɕʂ̕ϐ���p�ӂ��Ă���
	TexMetadata metadata3{};
	ScratchImage scratchImg3{};
	// WIC�e�N�X�`���̃��[�h
	result = LoadFromWICFile(
		L"Resources/tomas.png",
		WIC_FLAGS_NONE,
		&metadata3, scratchImg3);

	ScratchImage mipChain3{};
	//�~�j�}�b�v����
	result = GenerateMipMaps(
		scratchImg3.GetImages(), scratchImg3.GetImageCount(), scratchImg3.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain3);
	if (SUCCEEDED(result))
	{
		scratchImg3 = std::move(mipChain3);
		metadata3 = scratchImg3.GetMetadata();
	}

	//�ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	metadata3.format = MakeSRGB(metadata3.format);


#pragma endregion

#pragma region �摜�C���[�W�f�[�^�̍쐬(������)


	//�摜�C���[�W�f�[�^�̍쐬

	////�������s�N�Z����
	//const size_t textureWidth = 256;
	////�c�����s�N�Z����
	//const size_t textureHeight = 256;
	////�z��̗v�f��
	//const size_t imageDateCount = textureWidth * textureHeight;
	////�@�摜�C���[�W�f�[�^�z��
	//XMFLOAT4* imageDate = new XMFLOAT4[imageDateCount];

	//�`�������W���1�p�̕ϐ�
	/*int countNumber = 0;
	int changeColor = 0;*/

	//�`�������W���2
	//�����V�[�h������
	std::random_device seed_gen;
	//�����Z���k�E�c�C�X�^�[�̗����G���W��
	std::mt19937_64 engine(seed_gen());
	//�����͈͂̎w��
	std::uniform_real_distribution<float> dist(0, 1);

	////�S�s�N�Z���̐F��������
	//for (size_t i = 0; i < imageDateCount; i++)
	//{
	//	//���s����
	//	//imageDate[i].x = 1.0f; // R
	//	//imageDate[i].y = 0.0f; // G
	//	//imageDate[i].z = 0.0f; // B
	//	//imageDate[i].w = 1.0f; // A

	//	//���K���
	//	//imageDate[i].x = 0.0f; // R
	//	//imageDate[i].y = 1.0f; // G
	//	//imageDate[i].z = 0.0f; // B
	//	//imageDate[i].w = 1.0f; // A

	//	//�`�������W���1
	//	//if (countNumber < 10)
	//	//{
	//	//	countNumber++;
	//	//}

	//	//if (countNumber >= 10)
	//	//{
	//	//	if (changeColor == 0)
	//	//	{
	//	//		changeColor = 1;
	//	//	}
	//	//	else if (changeColor == 1)
	//	//	{
	//	//		changeColor = 0;
	//	//	}

	//	//	countNumber = 0;
	//	//}

	//	//if (changeColor == 0)
	//	//{
	//	//	imageDate[i].x = 1.0f; // R
	//	//	imageDate[i].y = 0.0f; // G
	//	//	imageDate[i].z = 0.0f; // B
	//	//	imageDate[i].w = 1.0f; // A
	//	//}
	//	//else if(changeColor == 1)
	//	//{
	//	//	imageDate[i].x = 0.0f; // R
	//	//	imageDate[i].y = 0.0f; // G
	//	//	imageDate[i].z = 1.0f; // B
	//	//	imageDate[i].w = 0.0f; // A
	//	//}

	//	//�`�������W���2
	//	imageDate[i].x = dist(engine); // R
	//	imageDate[i].y = dist(engine); // G
	//	imageDate[i].z = dist(engine); // B
	//	imageDate[i].w = 1.0f; // A
	//	
	//}

#pragma endregion 

#pragma region �e�N�X�`���o�b�t�@�̐ݒ�(P04_02)

	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty =
		D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width; // ��
	textureResourceDesc.Height = (UINT)metadata.height; // ��
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

#pragma endregion

#pragma region �񖇖ڂ̃e�N�X�`���o�b�t�@�̐ݒ�(P04_02)

	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC textureResourceDesc2{};
	textureResourceDesc2.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc2.Format = metadata2.format;
	textureResourceDesc2.Width = metadata2.width; // ��
	textureResourceDesc2.Height = (UINT)metadata2.height; // ��
	textureResourceDesc2.DepthOrArraySize = (UINT16)metadata2.arraySize;
	textureResourceDesc2.MipLevels = (UINT16)metadata2.mipLevels;
	textureResourceDesc2.SampleDesc.Count = 1;

#pragma endregion

#pragma region �O���ڂ̃e�N�X�`���o�b�t�@�̐ݒ�(P04_02)

	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC textureResourceDesc3{};
	textureResourceDesc3.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc3.Format = metadata3.format;
	textureResourceDesc3.Width = metadata3.width; // ��
	textureResourceDesc3.Height = (UINT)metadata3.height; // ��
	textureResourceDesc3.DepthOrArraySize = (UINT16)metadata3.arraySize;
	textureResourceDesc3.MipLevels = (UINT16)metadata3.mipLevels;
	textureResourceDesc3.SampleDesc.Count = 1;

#pragma endregion

#pragma region �e�N�X�`���o�b�t�@�̐���(P04_02)
	//�e�N�X�`���o�b�t�@�̐���
	ComPtr<ID3D12Resource> texBuff;
	result = device->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff));
#pragma endregion

#pragma region �񖇖ڂ̃e�N�X�`���o�b�t�@�̐���(P04_02)
	//�e�N�X�`���o�b�t�@�̐���
	ComPtr<ID3D12Resource> texBuff2;
	result = device->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc2,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff2));
#pragma endregion

#pragma region �O���ڂ̃e�N�X�`���o�b�t�@�̐���(P04_02)
	//�e�N�X�`���o�b�t�@�̐���
	ComPtr<ID3D12Resource> texBuff3;
	result = device->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc3,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff3));
#pragma endregion




#pragma region �e�N�X�`���o�b�t�@�Ƀf�[�^�]��(P04_03_27)

	//�S�~�j�}�b�v�ɂ���
	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		//�~�j�}�b�v���x�����w�肵�ăC���[�W���擾
		const Image* img = scratchImg.GetImage(i, 0, 0);
		//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		result = texBuff->WriteToSubresource(
			(UINT)i,
			nullptr,
			img->pixels,
			(UINT)img->rowPitch,
			(UINT)img->slicePitch
		);
		assert(SUCCEEDED(result));
	}

	//�񖇖ڂ̑S�~�j�}�b�v�ɂ���
	for (size_t i = 0; i < metadata2.mipLevels; i++)
	{
		//�~�j�}�b�v���x�����w�肵�ăC���[�W���擾
		const Image* img2 = scratchImg2.GetImage(i, 0, 0);
		//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		result = texBuff2->WriteToSubresource(
			(UINT)i,
			nullptr,
			img2->pixels,
			(UINT)img2->rowPitch,
			(UINT)img2->slicePitch
		);
		assert(SUCCEEDED(result));
	}

	//�O���ڂ̑S�~�j�}�b�v�ɂ���
	for (size_t i = 0; i < metadata3.mipLevels; i++)
	{
		//�~�j�}�b�v���x�����w�肵�ăC���[�W���擾
		const Image* img3 = scratchImg3.GetImage(i, 0, 0);
		//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		result = texBuff3->WriteToSubresource(
			(UINT)i,
			nullptr,
			img3->pixels,
			(UINT)img3->rowPitch,
			(UINT)img3->slicePitch
		);
		assert(SUCCEEDED(result));
	}

	//result = texBuff->WriteToSubresource(
	//	0,
	//	nullptr,	//�S�̈�փR�s�[
	//	imageDate,	//���f�[�^�A�h���X
	//	sizeof(XMFLOAT4) * textureWidth,	// 1���C���T�C�Y
	//	sizeof(XMFLOAT4) * imageDateCount	//�@�S�T�C�Y
	//);

	//delete[] imageDate;

#pragma endregion

#pragma region �f�X�N���v�^�q�[�v����(04_02)

	//SRV�̍ő��
	const size_t kMaxSRVCount = 2056;

	//�f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; // �V�F�[�_�[���猩����悤��
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	//�ݒ��{��SRV�p�f�X�N���v�^�q�[�v�𐶐�
	ID3D12DescriptorHeap* srvHeap = nullptr;
	result = device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(result));
#pragma endregion
#pragma region �f�X�N���v�^�n���h��
	//SRV�q�[�v�̐擪�n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();
#pragma endregion

#pragma region �[�x�r���[�p�̃f�X�N���v�^�q�[�v���쐬

	// �f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;	//�[�x�r���[��1��
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;	//�f�v�X�X�e���V���r���[
	ID3D12DescriptorHeap* dsvHeap = nullptr;
	// �f�X�N���v�^�q�[�v�̍쐬
	result = device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));

#pragma endregion

#pragma region �[�x�X�e���V���r���[�̐���

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;	//�[�x�l�t�H�[�}�b�g
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView(
		depthBuff.Get(),
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart()
	);

#pragma endregion

#pragma region �V�F�[�_�[���\�[�X�r���[�ݒ�(P04_03)
	//�V�F�[�_�[���\�[�X�r���[�̐ݒ�

	//�摜�ؑ֗p�̃t���O
	int changeImage = 1;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; //�ݒ�\����
	srvDesc.Format = textureResourceDesc.Format;//RGBA float
	srvDesc.Shader4ComponentMapping =
		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

	//�n���h���̎w���ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	device->CreateShaderResourceView(texBuff.Get(), &srvDesc, srvHandle);
#pragma endregion

#pragma region �񖇖ڂ̉摜�p�V�F�[�_�[���\�[�X�r���[�ݒ�(P06_06)

	//�f�X�N���v�^�̃T�C�Y���擾
	UINT incrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//�擾�����T�C�Y���g�p���ăn���h����i�߂�
	srvHandle.ptr += incrementSize;

	//�V�F�[�_�[���\�[�X�r���[�̐ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc2{}; //�ݒ�\����
	srvDesc2.Format = textureResourceDesc2.Format;//RGBA float
	srvDesc2.Shader4ComponentMapping =
		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc2.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc2.Texture2D.MipLevels = textureResourceDesc2.MipLevels;

	//�n���h���̎w���ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	device->CreateShaderResourceView(texBuff2.Get(), &srvDesc2, srvHandle);

#pragma endregion

#pragma region �O���ڂ̉摜�p�V�F�[�_�[���\�[�X�r���[�ݒ�(P06_06)

	//�f�X�N���v�^�̃T�C�Y���擾
	UINT incrementSize2 = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//�擾�����T�C�Y���g�p���ăn���h����i�߂�
	srvHandle.ptr += incrementSize2;

	//�V�F�[�_�[���\�[�X�r���[�̐ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc3{}; //�ݒ�\����
	srvDesc3.Format = textureResourceDesc3.Format;//RGBA float
	srvDesc3.Shader4ComponentMapping =
		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc3.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc3.Texture2D.MipLevels = textureResourceDesc3.MipLevels;

	//�n���h���̎w���ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	device->CreateShaderResourceView(texBuff3.Get(), &srvDesc3, srvHandle);

#pragma endregion




#pragma region �O���t�B�b�N�X�p�C�v���C���ݒ�(P02_02_P04)

	// �O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	// �V�F�[�_�[�̐ݒ�
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

	// �T���v���}�X�N�̐ݒ�
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�

	// ���X�^���C�U�̐ݒ�
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK; // �w�ʂ��J�����O
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // �|���S�����h��Ԃ�
	pipelineDesc.RasterizerState.DepthClipEnable = true; // �[�x�N���b�s���O��L����

#pragma region �u�����h�ݒ�(03_01)
	// �u�����h�X�e�[�g
	//pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask
	//	= D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA�S�Ẵ`�����l����`��
	//�����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;// RBGA�S�Ẵ`�����l����`��

	//�A���t�@�l�̌v�Z���̐ݒ�
	blenddesc.BlendEnable = true;					//�u�����h��L���ɂ���
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//���Z
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		//�\�[�X�̒l��100%�g��(������`�悵�悤�Ƃ��Ă���F)
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//�f�X�g�̒l��  0%�g��(���ɕ`����Ă���F)

	//RGB�l�̌v�Z���̐ݒ�(���Z����)
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;	//���Z
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;	//�\�[�X�̒l��100%�g��
	//blenddesc.DestBlend = D3D12_BLEND_ONE;	//�f�X�g�̒l��100%�g��

	//���Z����
	//blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;	//�f�X�g����\�[�X�����Z
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;				//�\�[�X�̒l��100%�g��
	//blenddesc.DestBlend = D3D12_BLEND_ONE;			//�f�X�g�̒l��100%�g��

	//�F���]
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;			//���Z
	//blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;	//1.0f - �f�X�g�J���[�̒l
	//blenddesc.DestBlend = D3D12_BLEND_ZERO;			//�g��Ȃ�

	//����������
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;				//���Z
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;			//�\�[�X�̃A���t�@�l
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;	//1.0f - �\�[�X�̃A���t�@�l

#pragma endregion 

	// ���_���C�A�E�g�̐ݒ�
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// ���̑��̐ݒ�
	pipelineDesc.NumRenderTargets = 1; // �`��Ώۂ�1��
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255�w���RGBA
	pipelineDesc.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	//�f�v�X�X�e���V���X�e�[�g�̐ݒ�
	pipelineDesc.DepthStencilState.DepthEnable = true;	//�[�x�e�X�g���s��
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	//�������݋���
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;		//��������΍��i
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;		//�[�x�l�t�H�[�}�b�g

#pragma region �f�X�N���v�^�����W�̐ݒ�

	//�f�X�N���v�^�����W�̐ݒ�
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;			//��x�̕`��Ɏg���e�N�X�`����1���Ȃ̂�1
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

#pragma endregion 
#pragma region ���[�g�p�����[�^�̐ݒ�(P03_02)(05_02_P16) 

	//���Ԃ͊֌W�Ȃ�

	//���[�g�p�����[�^�̐ݒ�
	D3D12_ROOT_PARAMETER rootParams[3] = {};
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//�萔�o�b�t�@�r���[
	rootParams[0].Descriptor.ShaderRegister = 0;					//�萔�o�b�t�@�ԍ�
	rootParams[0].Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_�[���猩����
	//�e�N�X�`�������W�X�^0��
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//���
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;			//�f�X�N���v�^�����W
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						//�f�X�N���v�^�����W��
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;				//�S�ẴV�F�[�_�[���猩����

	//�萔�o�b�t�@1��
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//�萔�o�b�t�@�r���[
	rootParams[2].Descriptor.ShaderRegister = 1;					//�萔�o�b�t�@�ԍ�
	rootParams[2].Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_�[���猩����

	//�e�N�X�`���T���v���[�̐ݒ�
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//���J��Ԃ�(�^�C�����O)
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//�c�J��Ԃ�(�^�C�����O)
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//���s�J��Ԃ�(�^�C�����O)
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//�{�[�_�[�̎��͍�
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//�S�ă��j�A���
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									//�~�j�}�b�v�ő�l
	samplerDesc.MinLOD = 0.0f;												//�~�j�}�b�v�ŏ��l
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			//�s�N�Z���V�F�[�_�[����̂ݎg�p�\

#pragma endregion 
#pragma region ���[�g�V�O�l�`���̐ݒ�(P02_01_P38)

	// ���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> rootSignature;
	
	// ���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;						//���[�g�p�����[�^�̐擪�A�h���X
	rootSignatureDesc.NumParameters = _countof(rootParams);			//���[�g�p�����[�^�[��
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	// ���[�g�V�O�l�`���̃V���A���C�Y
	ComPtr<ID3DBlob> rootSigBlob;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));
	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc.pRootSignature = rootSignature.Get();
#pragma endregion 

	// �p�C�v�����X�e�[�g�̐���
	ComPtr<ID3D12PipelineState> pipelineState;
	result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));


#pragma endregion 
	// �Q�[�����[�v
	while (true) {

#pragma region �E�C���h�E���b�Z�[�W����
		//�Q�[�������s���Ă���ԁA�J��Ԃ��s������
		// �u���b�N���̓y�[�W�E�����Q��
		// ���b�Z�[�W������?
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg); // �L�[���̓��b�Z�[�W�̏���
			DispatchMessage(&msg); // �v���V�[�W���Ƀ��b�Z�[�W�𑗂�
		}
		// �~�{�^���ŏI�����b�Z�[�W��������Q�[�����[�v�𔲂���
		if (msg.message == WM_QUIT) {
			break;
		}
#pragma endregion �E�C���h�E���b�Z�[�W����
#pragma region DirectX���t���[������
		// DirectX���t���[������ ��������
		keyInput->SaveFrameKey();



#pragma region ���_�o�b�t�@�ւ̃f�[�^�]�� (P02_01)
		if (keyInput->HasPushedKey(DIK_1))
		{
			/*if (formchange == 0)
			{
				vertices[3] = { +0.5f, +0.5f, 0.0f };
				vertices[4] = { +0.5f, -0.5f, 0.0f };
				vertices[5] = { -0.5f, +0.5f, 0.0f };
				if (keyInput->ReleasedKeyMoment(DIK_1))
				{
					formchange = 1;
				}
			}
			else
			{
				vertices[3] = { -0.5f, -0.5f, 0.0f };
				vertices[4] = { -0.5f, +0.5f, 0.0f };
				vertices[5] = { +0.5f, -0.5f, 0.0f };
				if (keyInput->ReleasedKeyMoment(DIK_1))
				{
					formchange = 0;
				}
			}*/
			// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
			Vertex* vertMap = nullptr;
			result = vertBuff->Map(0, nullptr, (void**)&vertMap);
			assert(SUCCEEDED(result));
			// �S���_�ɑ΂���
			for (int i = 0; i < _countof(vertices); i++) {
				vertMap[i] = vertices[i]; // ���W���R�s�[
			}
			// �q���������
			vertBuff->Unmap(0, nullptr);
		}
#pragma endregion

#pragma region �^�[�Q�b�g�̎�����܂��J����(P05_04)
		if (keyInput->HasPushedKey(DIK_D) || keyInput->HasPushedKey(DIK_A))
		{
			if (keyInput->HasPushedKey(DIK_D)) { angle += XMConvertToRadians(1.0f); }
			else if (keyInput->HasPushedKey(DIK_A)) { angle -= XMConvertToRadians(1.0f); }

			//angle���W�A������Y���܂��ɉ�]�B���a��-100
			eye.x = -100 * sinf(angle);
			eye.z = -100 * cosf(angle);

			//�r���[�ϊ��s����ĕҐ�
			matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
		}

#pragma endregion

#pragma region �}�`��A�����ē�����

		//�����ꂩ�̃L�[�������Ă�����
		if (keyInput->HasPushedKey(DIK_UP) || keyInput->HasPushedKey(DIK_DOWN) ||
			keyInput->HasPushedKey(DIK_RIGHT) || keyInput->HasPushedKey(DIK_LEFT))
		{
			//���W���ړ����鏈��(Z���W)
			if (keyInput->HasPushedKey(DIK_UP))
			{
				position.y += 1.0f;
				position1.y += 2.0f;

			}
			else if (keyInput->HasPushedKey(DIK_DOWN))
			{
				position.y -= 1.0f;
				position1.y -= 2.0f;

			}
			if (keyInput->HasPushedKey(DIK_RIGHT))
			{

				position1.x += 2.0f;
			}
			else if (keyInput->HasPushedKey(DIK_LEFT))
			{

				position1.x -= 2.0f;

			}
		}

		if (keyInput->HasPushedKey(DIK_UP) || keyInput->HasPushedKey(DIK_DOWN) ||
			keyInput->HasPushedKey(DIK_RIGHT) || keyInput->HasPushedKey(DIK_LEFT))
		{
			if (keyInput->HasPushedKey(DIK_UP)) { object3ds[0].position.y += 1.0f; }
			else if (keyInput->HasPushedKey(DIK_DOWN)) { object3ds[0].position.y -= 1.0f; }
			if (keyInput->HasPushedKey(DIK_RIGHT)) { object3ds[0].position.x += 1.0f; }
			else if (keyInput->HasPushedKey(DIK_LEFT)) { object3ds[0].position.x -= 1.0f; }

			if (keyInput->HasPushedKey(DIK_A)) { object3ds[0].rotation.z += 0.1f; }
			else if (keyInput->HasPushedKey(DIK_D)) { object3ds[0].rotation.z -= 0.1f; }
		}

		if (keyInput->HasPushedKey(DIK_1) || keyInput->PushedKeyMoment(DIK_1))
		{
			changeImage = 1;
		}
		else if (keyInput->HasPushedKey(DIK_2) || keyInput->PushedKeyMoment(DIK_2))
		{
			changeImage = 2;
		}
		else if (keyInput->HasPushedKey(DIK_3) || keyInput->PushedKeyMoment(DIK_3))
		{
			changeImage = 3;
		}


#pragma endregion

#pragma region ���[���h�ϊ��s�� 

		for (size_t i = 0; i < _countof(object3ds); i++)
		{
			UpdateObject3d(&object3ds[i], matView, matProjection);
		}

#pragma region ��ڂ̃I�u�W�F�N�g�̃��[���h�ϊ��s��

		////���[���h�ϊ��s��
		//XMMATRIX matWorld;

		//XMMATRIX matScale;	//�X�P�[�����O�s��
		//matScale = XMMatrixScaling(scale.x, scale.y, scale.z);

		//XMMATRIX matRot;	//��]�s��
		//matRot = XMMatrixIdentity();
		//matRot *= XMMatrixRotationZ((rotation.z));	//Z�������45�x��]
		//matRot *= XMMatrixRotationX((rotation.x));	//X�������15�x��]
		//matRot *= XMMatrixRotationY((rotation.y));	//Y�������30�x��]

		//XMMATRIX matTrans;	//���s�ړ��s��
		//matTrans = XMMatrixTranslation(position.x, position.y, position.z);	//(-50,0,0)���s�ړ�

		//matWorld = XMMatrixIdentity();	//�P�ʍs��������ĕό`�����Z�b�g
		//matWorld *= matScale;	//���[���h�s��ɃX�P�[�����O�𔽉f
		//matWorld *= matRot;		//���[���h�s��ɉ�]�𔽉f
		//matWorld *= matTrans;	//���[���h�s��ɕ��s�ړ��𔽉f

		////�萔�o�b�t�@�Ƀf�[�^�]��
		//constMapTransform0->mat = matWorld * matView * matProjection;

#pragma endregion

#pragma region ��ڂ̃I�u�W�F�N�g�̃��[���h�ϊ��s��

		////���[���h�ϊ��s��
		//XMMATRIX matWorld1;
		//matWorld1 = XMMatrixIdentity();

		////�X�P�[�����O�s��
		//XMMATRIX matScale1 = XMMatrixScaling(1.0f,1.0f,1.0f);
		//XMMATRIX matRot1 = XMMatrixRotationY(XM_PI/4.0f);
		//XMMATRIX matTrans1 = XMMatrixTranslation(position1.x, position1.y, position1.z);

		////���[���h�s�������
		//matWorld1 = matScale1 * matRot1 * matTrans1;	
		//
		////�萔�o�b�t�@�Ƀf�[�^�]��
		//constMapTransform1->mat = matWorld1 * matView * matProjection;


#pragma endregion

#pragma endregion



#pragma region �`�������W���

		//if (keyInput->PushedKeyMoment(DIK_2))
		//{
		//	int WiREFRAMEFlag = 1;
		//	WiREFRAMEFlag--;
		//	if (WiREFRAMEFlag <= 0)
		//	{
		//		if (pipelineDesc.RasterizerState.FillMode == D3D12_FILL_MODE_WIREFRAME)
		//		{
		//			pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		//			WiREFRAMEFlag = 20;
		//			result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
		//			assert(SUCCEEDED(result));
		//		}

		//		else if (pipelineDesc.RasterizerState.FillMode == D3D12_FILL_MODE_SOLID)
		//		{
		//			pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
		//			WiREFRAMEFlag = 20;
		//			result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
		//			assert(SUCCEEDED(result));
		//		}
		//	}

		//}

#pragma endregion

#pragma region ���\�[�X�o���A1 (P01_03)
		// �o�b�N�o�b�t�@�̔ԍ����擾(2�Ȃ̂�0�Ԃ�1��)
		UINT bbIndex = swapChain->GetCurrentBackBufferIndex();
		// 1.���\�[�X�o���A�ŏ������݉\�ɕύX
		D3D12_RESOURCE_BARRIER barrierDesc{};
		barrierDesc.Transition.pResource = backBuffers[bbIndex].Get(); // �o�b�N�o�b�t�@���w��
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT; // �\����Ԃ���
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // �`���Ԃ�
		commandList->ResourceBarrier(1, &barrierDesc);

#pragma endregion 

#pragma region �`���w��R�}���h (P01_03)
		// 2.�`���̕ύX
		// �����_�[�^�[�Q�b�g�r���[�̃n���h�����擾
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
		rtvHandle.ptr += bbIndex * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);


		//�[�x�X�e���V���r���[�p�̃f�X�N���v�^�q�[�v�̃n���h�����擾
		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();
		commandList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

#pragma endregion

#pragma region ��ʃN���A�R�}���h�E�w�i�F�ύX(P01_03)

		// 3.��ʃN���A R G B A
		FLOAT clearColor[] = { 0.1f,0.25f,0.5f,0.0f }; // ���ۂ��F

		//�w�i�F��ύX
		//keyInput->ChangeColor(clearColor);
		//if (keyInput->HasPushedKey(DIK_SPACE))
		//{
		//	//��ʃN���A�J���[�̐��l������������
		//	clearColor[0] = 1.0f;
		//	clearColor[1] = 0.1f;
		//}

		commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
		commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
#pragma endregion
#pragma region �O���t�B�b�N�R�}���h
		// 4.�`��R�}���h��������

#pragma region �r���[�|�[�g�ݒ�R�}���h

		// �r���[�|�[�g�ݒ�R�}���h
		D3D12_VIEWPORT viewport;

		//1�ڂ̃r���[�|�[�g��ݒ� ����
		viewport.Width = window_width;      //����
		viewport.Height = window_height;	//�c��
		viewport.TopLeftX = 0;					//����X
		viewport.TopLeftY = 0;					//����Y
		viewport.MinDepth = 0.0f;				//�ő�[�x
		viewport.MaxDepth = 1.0f;				//�ŏ��[�x

		// �r���[�|�[�g�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
		commandList->RSSetViewports(1, &viewport);
#pragma endregion

#pragma region �V�U�[��`�̐ݒ�R�}���h

		// �V�U�[��`
		D3D12_RECT scissorRect;
		scissorRect.left = 0; // �؂蔲�����W��
		scissorRect.right = scissorRect.left + window_width; // �؂蔲�����W�E
		scissorRect.top = 0; // �؂蔲�����W��
		scissorRect.bottom = scissorRect.top + window_height; // �؂蔲�����W��

		// �V�U�[��`�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
		commandList->RSSetScissorRects(1, &scissorRect);

#pragma endregion

#pragma region �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h(P02_01)

		// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
		//�����炱�̃��[���ŕ`����n�߂܂���
		commandList->SetPipelineState(pipelineState.Get());
		commandList->SetGraphicsRootSignature(rootSignature.Get());
#pragma endregion

#pragma region SRV�ݒ�R�}���h


		//SRV�q�[�v�̐ݒ�R�}���h
		commandList->SetDescriptorHeaps(1, &srvHeap);
		// SRV�q�[�v�̐擪�n���h�����擾(SRV���w���Ă���͂�)
		D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();
		// SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^1�Ԃɐݒ�

		if (changeImage == 1)
		{
			srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();
		}
		else if (changeImage == 2)
		{
			srvGpuHandle.ptr += incrementSize;
		}
		else if (changeImage == 3)
		{
			srvGpuHandle.ptr += (incrementSize + incrementSize2);
		}

		commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

#pragma endregion 

#pragma region ���̑��̐ݒ�R�}���h

		// �v���~�e�B�u�`��̐ݒ�R�}���h
		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // �O�p�`���X�g

		//// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
		//commandList->IASetVertexBuffers(0, 1, &vbView);

		////�C���f�b�N�X�o�b�t�@�r���[�̐ݒ�R�}���h
		//commandList->IASetIndexBuffer(&ibView);

		//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
		commandList->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());

		//�S�I�u�W�F�N�g�ɂ��ď���
		for (int i = 0; i < _countof(object3ds); i++)
		{
			DrawObject3d(&object3ds[i], commandList, vbView, ibView, _countof(indices));
		}

		////0�Ԓ萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
		//commandList->SetGraphicsRootConstantBufferView(2, constBuffTransform0->GetGPUVirtualAddress());
		//// �`��R�}���h
		//commandList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);; // �S�Ă̒��_���g���ĕ`��

		////1�Ԓ萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
		//commandList->SetGraphicsRootConstantBufferView(2, constBuffTransform1->GetGPUVirtualAddress());
		//// �`��R�}���h
		//commandList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);; // �S�Ă̒��_���g���ĕ`��



#pragma endregion 
#pragma endregion �O���t�B�b�N�R�}���h
		// 4.�`��R�}���h�����܂�
#pragma region ���\�[�X�o���A2(P01_03)
		// 5.���\�[�X�o���A��߂�
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // �`���Ԃ���
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT; // �\����Ԃ�
		commandList->ResourceBarrier(1, &barrierDesc);
#pragma endregion
		// DirectX���t���[������ �����܂�
#pragma endregion  DirectX���t���[������

#pragma region ��ʓ���ւ�

#pragma region �R�}���h�̃t���b�V��(P01_03)

		// ���߂̃N���[�Y
		result = commandList->Close();
		assert(SUCCEEDED(result));
		// �R�}���h���X�g�̎��s
		ID3D12CommandList* commandLists[] = { commandList};
		commandQueue->ExecuteCommandLists(1, commandLists);
		// ��ʂɕ\������o�b�t�@���t���b�v(���\�̓��ւ�)
		result = swapChain->Present(1, 0);
		assert(SUCCEEDED(result));
#pragma endregion 

#pragma region �R�}���h�����҂�(P01_03)
		// �R�}���h�̎��s������҂�
		commandQueue->Signal(fence.Get(), ++fenceVal);
		if (fence->GetCompletedValue() != fenceVal) {
			HANDLE event = CreateEvent(nullptr, false, false, nullptr);
			fence->SetEventOnCompletion(fenceVal, event);
			WaitForSingleObject(event, INFINITE);
			CloseHandle(event);
		}
		// �L���[���N���A
		result = commandAllocator->Reset();
		assert(SUCCEEDED(result));
		// �ĂуR�}���h���X�g�𒙂߂鏀��
		result = commandList->Reset(commandAllocator.Get(), nullptr);
		assert(SUCCEEDED(result));
#pragma endregion 

#pragma endregion ��ʓ���ւ�

	}

#pragma region WindowsAPI��n��
	//�B�Q�[���S�̂̏I������
	// �E�B���h�E�N���X��o�^����
	UnregisterClass(w.lpszClassName, w.hInstance);

	delete keyInput;

#pragma endregion WindowsAPI��n��
	return 0;

}

void InitializeObject3d(Object3d* object, ID3D12Device* device)
{
	HRESULT result;

	//�萔�o�b�t�@�̃q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//�萔�o�b�t�@�̃��\�[�X�ݒ�
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//�萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&object->constBuffTransform));
	assert(SUCCEEDED(result));

	//�萔�o�b�t�@�̃}�b�s���O
	result = object->constBuffTransform->Map(0, nullptr, (void**)&object->constMapTransform);
	assert(SUCCEEDED(result));
}

void UpdateObject3d(Object3d* object, XMMATRIX& matview, XMMATRIX& matProjection)
{
	XMMATRIX matScale, matRot, matTrans;

	//�X�P�[���A��]�A���s�ړ��̌v�Z
	matScale = XMMatrixScaling(object->scale.x, object->scale.y, object->scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(object->rotation.z);
	matRot *= XMMatrixRotationX(object->rotation.x);
	matRot *= XMMatrixRotationY(object->rotation.y);
	matTrans = XMMatrixTranslation(object->position.x, object->position.y, object->position.z);

	object->matworld = XMMatrixIdentity();
	object->matworld *= matScale;
	object->matworld *= matRot;
	object->matworld *= matTrans;

	//�e�I�u�W�F�N�g�������
	if (object->parent != nullptr)
	{
		//�e�I�u�W�F�N�g�̃��[���h�s����|����
		object->matworld *= object->parent->matworld;
	}

	//�萔�o�b�t�@�փf�[�^�]��
	object->constMapTransform->mat = object->matworld * matview * matProjection;
}

void DrawObject3d(Object3d* object, ID3D12GraphicsCommandList* commandList, D3D12_VERTEX_BUFFER_VIEW& vbView,
	D3D12_INDEX_BUFFER_VIEW& ibView, UINT numIndices)
{
	//���_�o�b�t�@�̐ݒ�
	commandList->IASetVertexBuffers(0, 1, &vbView);
	//�C���f�b�N�X�o�b�t�@�̐ݒ�
	commandList->IASetIndexBuffer(&ibView);
	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	commandList->SetGraphicsRootConstantBufferView(2, object->constBuffTransform->GetGPUVirtualAddress());

	//�`��R�}���h
	commandList->DrawIndexedInstanced(numIndices, 1, 0, 0, 0);
}

//�萔�o�b�t�@�̐���
//long ConstantBufferResult(long& result, ID3D12Device* device, D3D12_HEAP_PROPERTIES& cbHeapProp,
//	D3D12_RESOURCE_DESC& cbResourceDesc, ID3D12Resource* constBuffTransform)
//{
//
//	assert(device != nullptr && constBuffTransform != nullptr);
//
//	return result = device->CreateCommittedResource(
//		&cbHeapProp,//�q�[�v�ݒ�
//		D3D12_HEAP_FLAG_NONE,
//		&cbResourceDesc,//���\�[�X�ݒ�
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&constBuffTransform));
//	assert(SUCCEEDED(result));
//}

//void CreateConstantBuffer(ID3D12Resource* constBuffMaterial)
//{
//	if(constBuffMaterial == nullptr)
//	//�q�[�v�ݒ�
//	D3D12_HEAP_PROPERTIES cbHeapProp{};
//	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
//	//���\�[�X�ݒ�
//	D3D12_RESOURCE_DESC cbResourceDesc{};
//	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//	cbResourceDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;	//256�o�C�g�A���C�������g
//	cbResourceDesc.Height = 1;
//	cbResourceDesc.DepthOrArraySize = 1;
//	cbResourceDesc.MipLevels = 1;
//	cbResourceDesc.SampleDesc.Count = 1;
//	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
//}
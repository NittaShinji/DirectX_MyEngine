#include "DirectXBasic.h"
#include "dxgidebug.h"
#include <cassert>
#include <vector>
#include <string>
#include <thread>
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

using namespace Microsoft::WRL;

void DirectXBasic::Initialize(WindowsAPI* winApi)
{
	//NULL�`�F�b�N
	assert(winApi);

	//�����o�ϐ��ɃR�s�[
	this->winApi_ = winApi;

	//FPS�Œ�
	InitializeFixFPS();

	//�f�o�C�X
	InitializeDevice();
	//�R�}���h�֘A
	InitializeCommand();
	//�X���b�v�`�F�[��
	InitializeSwapChain();
	//�����_�[�^�[�Q�[�g�r���[(RTV)
	InitializeTargetView();
	//�[�x�o�b�t�@
	InitializeDepthBuffer();
	//�t�F���X
	InitializeFence();

#pragma region �f�X�N���v�^�q�[�v�̐��� (P01_02)
#pragma endregion
#pragma region �o�b�N�o�b�t�@(P01_02)
#pragma endregion 
#pragma region �t�F���X (P01_02)
#pragma endregion 
}

void DirectXBasic::BeforeDraw()
{
	// �o�b�N�o�b�t�@�̔ԍ����擾(2�Ȃ̂�0�Ԃ�1��)
	UINT bbIndex = swapChain_->GetCurrentBackBufferIndex();
#pragma region ���\�[�X�o���A�̕ύX�R�}���h (P01_03)
	
	// 1.���\�[�X�o���A�ŏ������݉\�ɕύX
	barrierDesc.Transition.pResource = backBuffers_[bbIndex].Get(); // �o�b�N�o�b�t�@���w��
	
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT; // �\����Ԃ���
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // �`���Ԃ�
	commandList_->ResourceBarrier(1, &barrierDesc);

#pragma endregion 

#pragma region �`���w��R�}���h (P01_03)
	// 2.�`���̕ύX
	// �����_�[�^�[�Q�b�g�r���[�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap_->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += bbIndex * device_->GetDescriptorHandleIncrementSize(rtvHeapDesc_.Type);

	//�[�x�X�e���V���r���[�p�̃f�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHeap_->GetCPUDescriptorHandleForHeapStart();
	commandList_->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

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

	commandList_->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	commandList_->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
#pragma endregion

#pragma region �r���[�|�[�g�ݒ�R�}���h

		// �r���[�|�[�g�ݒ�R�}���h
	D3D12_VIEWPORT viewport;

	//1�ڂ̃r���[�|�[�g��ݒ� ����
	viewport.Width = (float)winApi_->GetWinWidth();      //����
	viewport.Height = (float)winApi_->GetWinHeight();	//�c��
	viewport.TopLeftX = 0;					//����X
	viewport.TopLeftY = 0;					//����Y
	viewport.MinDepth = 0.0f;				//�ő�[�x
	viewport.MaxDepth = 1.0f;				//�ŏ��[�x

	// �r���[�|�[�g�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
	commandList_->RSSetViewports(1, &viewport);
#pragma endregion

#pragma region �V�U�[��`�̐ݒ�R�}���h

	// �V�U�[��`
	D3D12_RECT scissorRect;
	scissorRect.left = 0; // �؂蔲�����W��
	scissorRect.right = scissorRect.left + winApi_->GetWinWidth(); // �؂蔲�����W�E
	scissorRect.top = 0; // �؂蔲�����W��
	scissorRect.bottom = scissorRect.top + winApi_->GetWinHeight(); // �؂蔲�����W��

	// �V�U�[��`�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
	commandList_->RSSetScissorRects(1, &scissorRect);
#pragma endregion
}

void DirectXBasic::AfterDraw()
{
	// �o�b�N�o�b�t�@�̔ԍ����擾(2�Ȃ̂�0�Ԃ�1��)
	UINT bbIndex = swapChain_->GetCurrentBackBufferIndex();

#pragma region ���\�[�X�o���A�̕ύX�R�}���h (P01_03)
	// 5.���\�[�X�o���A��߂�
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // �`���Ԃ���
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT; // �\����Ԃ�
	commandList_->ResourceBarrier(1, &barrierDesc);
#pragma endregion 

#pragma region �R�}���h�̃t���b�V��(P01_03)

	// ���߂̃N���[�Y
	result_ = commandList_->Close();
	assert(SUCCEEDED(result_));
	// �R�}���h���X�g�̎��s
	ID3D12CommandList* commandLists[] = { commandList_.Get() };
	commandQueue_->ExecuteCommandLists(1, commandLists);
	// ��ʂɕ\������o�b�t�@���t���b�v(���\�̓��ւ�)
	result_ = swapChain_->Present(1, 0);
	assert(SUCCEEDED(result_));
#pragma endregion 

#ifdef _DEBUG

	if(FAILED(result_))
	{
		ComPtr<ID3D12DeviceRemovedExtendedData> dred;

		result_ = device_->QueryInterface(IID_PPV_ARGS(&dred));
		assert(SUCCEEDED(result_));

		// �����p�������擾
		D3D12_DRED_AUTO_BREADCRUMBS_OUTPUT autoBreadcrumbsOutput{};
		result_ = dred->GetAutoBreadcrumbsOutput(&autoBreadcrumbsOutput);
		assert(SUCCEEDED(result_));
	}

#endif

#pragma region �R�}���h�����҂�(P01_03)
	// �R�}���h�̎��s������҂�
	commandQueue_->Signal(fence.Get(), ++fenceVal);

	if (fence->GetCompletedValue() != fenceVal) {
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}
	// �L���[���N���A
	result_ = commandAllocator_->Reset();
	assert(SUCCEEDED(result_));
	// �ĂуR�}���h���X�g�𒙂߂鏀��
	result_ = commandList_->Reset(commandAllocator_.Get(), nullptr);
	assert(SUCCEEDED(result_));

#pragma endregion 

	//FPS�Œ�
	UpdateFixFPS();
}

void DirectXBasic::InitializeDevice()
{
#pragma region �A�_�v�^�̗�(P01_02)

#ifdef _DEBUG

	//�f�o�b�O���C���[���I����
	ComPtr<ID3D12Debug1> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		debugController->EnableDebugLayer();
		//debugController->SetEnableGPUBasedValidation(TRUE);
	}

	// DRED���|�[�g���I����
	ComPtr<ID3D12DeviceRemovedExtendedDataSettings> dredSettings;
	if(SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&dredSettings))))
	{
		dredSettings->SetAutoBreadcrumbsEnablement(D3D12_DRED_ENABLEMENT_FORCED_ON);
		dredSettings->SetPageFaultEnablement(D3D12_DRED_ENABLEMENT_FORCED_ON);
	}
	
#endif

	// DXGI�t�@�N�g���[�̐���
	result_ = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory_));
	assert(SUCCEEDED(result_));
	// �A�_�v�^�[�̗񋓗p
	std::vector<ComPtr<IDXGIAdapter4>> adapters;

	// �����ɓ���̖��O�����A�_�v�^�[�I�u�W�F�N�g������
	ComPtr<IDXGIAdapter4> tmpAdapter = nullptr;
	// �p�t�H�[�}���X���������̂��珇�ɁA�S�ẴA�_�v�^�[��񋓂���
	for (UINT i = 0;
		dxgiFactory_->EnumAdapterByGpuPreference(i,
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
		result_ = D3D12CreateDevice(tmpAdapter.Get(), levels[i],
			IID_PPV_ARGS(&device_));
		if (result_ == S_OK) {
			// �f�o�C�X�𐶐��ł������_�Ń��[�v�𔲂���
			featureLevel = levels[i];
			break;
		}
	}
#pragma endregion

#ifdef _DEBUG
	ComPtr<ID3D12InfoQueue> infoQueue;
	if (SUCCEEDED(device_->QueryInterface(IID_PPV_ARGS(&infoQueue))))
	{
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);

		 // �}������G���[
		D3D12_MESSAGE_ID denyIds[] = {
			
			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE };
		// �}������\�����x��
		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER filter{};
		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;
		// �w�肵���G���[�̕\����}������
		infoQueue->PushStorageFilter(&filter);

	}

#endif
}

void DirectXBasic::InitializeCommand()
{
#pragma region �R�}���h�A���P�[�^�ƃ��X�g�̐��� (P01_02)

	// �R�}���h�A���P�[�^�𐶐�
	result_ = device_->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&commandAllocator_));
	assert(SUCCEEDED(result_));
	// �R�}���h���X�g�𐶐�
	result_ = device_->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		commandAllocator_.Get(), nullptr,
		IID_PPV_ARGS(&commandList_));
	assert(SUCCEEDED(result_));

#pragma endregion

#pragma region �R�}���h�L���[�̐���(P01_02)
	//�R�}���h�L���[�̐ݒ�
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};

	//�R�}���h�L���[�𐶐�
	result_ = device_->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue_));
	assert(SUCCEEDED(result_));

#pragma endregion 
}

void DirectXBasic::InitializeSwapChain()
{
#pragma region �X���b�v�`�F�[�� (P01_02)
	// �X���b�v�`�F�[���̐ݒ�
	swapChainDesc.Width = 1280;
	swapChainDesc.Height = 720;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // �F���̏���
	swapChainDesc.SampleDesc.Count = 1; // �}���`�T���v�����Ȃ�
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER; // �o�b�N�o�b�t�@�p
	swapChainDesc.BufferCount = 2; // �o�b�t�@����2�ɐݒ�
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // �t���b�v��͔j��
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	//IDXGISwapChain��ComPtr��p��(�������ł̂ݎd�l)
	ComPtr<IDXGISwapChain1> swapChain1;

	// �X���b�v�`�F�[���̐���
	result_ = dxgiFactory_->CreateSwapChainForHwnd(
		commandQueue_.Get(), winApi_->GetHwndClass(), &swapChainDesc, nullptr, nullptr, &swapChain1);
	assert(SUCCEEDED(result_));

	//��������IDXGISwapChain1�̃I�u�W�F�N�g��IDXGISwapChain4�ɕϊ�����
	swapChain1.As(&swapChain_);
#pragma endregion 
}

void DirectXBasic::InitializeTargetView()
{
#pragma region �����_�[�^�[�Q�[�g�r���[(RTV)�̐��� (P01_02)
	// �f�X�N���v�^�q�[�v�̐ݒ�
	rtvHeapDesc_.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // �����_�[�^�[�Q�b�g�r���[
	rtvHeapDesc_.NumDescriptors = swapChainDesc.BufferCount; // ���\��2��
	// �f�X�N���v�^�q�[�v�̐���
	device_->CreateDescriptorHeap(&rtvHeapDesc_, IID_PPV_ARGS(&rtvHeap_));

	backBuffers_.resize(swapChainDesc.BufferCount);

	// �X���b�v�`�F�[���̑S�Ẵo�b�t�@�ɂ��ď�������
	for (size_t i = 0; i < backBuffers_.size(); i++) {

		// �X���b�v�`�F�[������o�b�t�@���擾
		swapChain_->GetBuffer((UINT)i, IID_PPV_ARGS(&backBuffers_[i]));
		// �f�X�N���v�^�q�[�v�̃n���h�����擾
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap_->GetCPUDescriptorHandleForHeapStart();
		// �����\���ŃA�h���X�������
		rtvHandle.ptr += i * device_->GetDescriptorHandleIncrementSize(rtvHeapDesc_.Type);
		// �����_�[�^�[�Q�b�g�r���[�̐ݒ�
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
		// �V�F�[�_�[�̌v�Z���ʂ�SRGB�ɕϊ����ď�������
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		// �����_�[�^�[�Q�b�g�r���[�̐���
		device_->CreateRenderTargetView(backBuffers_[i].Get(), &rtvDesc, rtvHandle);
		//device->CreateRenderTargetView(backBuffers[i].Get(), &rtvDesc, rtvHandle);
	}
#pragma endregion 
}

void DirectXBasic::InitializeDepthBuffer()
{
#pragma region �[�x�o�b�t�@�̃��\�[�X�ݒ�

#pragma region	�[�x�o�b�t�@�̃��\�[�X�ݒ�

	D3D12_RESOURCE_DESC depthResourceDesc{};

	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = winApi_->GetWinWidth();								//�����_�[�^�[�Q�b�g�ɍ��킹��
	depthResourceDesc.Height = winApi_->GetWinHeight();							//�����_�[�^�[�Q�b�g�ɍ��킹��
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

#pragma region �[�x�o�b�t�@����
	//�[�x�o�b�t�@����
	result_ = device_->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,//�[�x�l�������݂Ɏg�p
		&depthClearValue,
		IID_PPV_ARGS(&depthBuff_)
	);
#pragma endregion

#pragma region �[�x�r���[�p�̃f�X�N���v�^�q�[�v���쐬

	// �f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;	//�[�x�r���[��1��
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;	//�f�v�X�X�e���V���r���[
	//ID3D12DescriptorHeap* dsvHeap = nullptr;

	// �f�X�N���v�^�q�[�v�̍쐬
	result_ = device_->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap_));

#pragma endregion

#pragma region �[�x�X�e���V���r���[�̐���

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;	//�[�x�l�t�H�[�}�b�g
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device_->CreateDepthStencilView(
		depthBuff_.Get(),
		&dsvDesc,
		dsvHeap_->GetCPUDescriptorHandleForHeapStart()
	);

#pragma endregion
#pragma endregion 
}

void DirectXBasic::InitializeFence()
{
#pragma region �t�F���X
	// �t�F���X�̐���
	result_ = device_->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
#pragma endregion 
}

void DirectXBasic::InitializeFixFPS()
{
	//���ݎ��Ԃ��L�^
	reference_ = std::chrono::steady_clock::now();
}

void DirectXBasic::UpdateFixFPS()
{
#pragma region FPS�̌Œ�(�X�V����)
	//1/60�b�s�b�^���̎���
	const std::chrono::microseconds kMinTime(uint64_t(1000000.0f / 60.0f));
	//1/60�b���Z������
	const std::chrono::microseconds kMinCheckTime(uint64_t(1000000.0f / 65.0f));

	//���ݎ��Ԃ��擾
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	//�O��L�^����̌o�ߎ��Ԃ��L�^
	std::chrono::microseconds elapsed =
		std::chrono::duration_cast<std::chrono::microseconds>(now - reference_);

	//1/60�b�o���Ă��Ȃ��ꍇ
	if (elapsed < kMinCheckTime)
	{
		while (std::chrono::steady_clock::now() - reference_ < kMinTime)
		{
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
	}

	//���ݎ��Ԃ��L�^����
	reference_ = std::chrono::steady_clock::now();
#pragma endregion 
}
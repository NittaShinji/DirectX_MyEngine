#pragma once
#include <Windows.h>
#include <stdint.h>

//windowsAPI
class WindowsAPI final
{
public:

	//�R�s�[�R���X�g���N�^�𖳌��ɂ���
	WindowsAPI(const WindowsAPI& windowsApi) = delete;
	//������Z�q�𖳌�
	WindowsAPI& operator=(const WindowsAPI& windowsApi) = delete;

	static WindowsAPI* GetInstance();

	//������
	void Initialize();
	
	//�ÓI�����o�֐�
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	//�Q�b�^�[
	const WNDCLASSEX& GetWindosClass() const { return w_;}
	const HWND& GetHwndClass()const{ return hwnd_;}
	const RECT& GetWrcClass() const {return wrc_;}
	const HINSTANCE& GetHInstance()const { return w_.hInstance;}
	int32_t GetWinWidth() const { return kWindow_width_;}
	int32_t GetWinHeight() const { return kWindow_height_;}

	//�I��
	void Finalize();

	//���b�Z�[�W�̏���
	bool ProcessMessage();

	//�萔
	//�E�C���h�E�T�C�Y
	static constexpr int32_t kWindow_width_ = 1280; //����
	static constexpr int32_t kWindow_height_ = 720; //�c��

private:

	WindowsAPI() {};
	~WindowsAPI() {};

	// �E�B���h�E
	WNDCLASSEX w_{};
	// �E�B���h�E�I�u�W�F�N�g
	HWND hwnd_;
	// �E�C���h�E�T�C�Y{X���W�@Y���W�@�����@�c��}
	RECT wrc_;
};
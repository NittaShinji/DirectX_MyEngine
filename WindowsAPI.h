#pragma once
#include <Windows.h>
#include <stdint.h>


//windowsAPI
class WindowsAPI
{
public:
	//������
	void Initialize();
	
	//�ÓI�����o�֐�
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	//�Q�b�^�[
	WNDCLASSEX GetWindosClass() const { return w;}
	HWND GetHwndClass()const{ return hwnd;}
	RECT GetWrcClass() const {return wrc;}
	HINSTANCE GetHInstance()const { return w.hInstance;}
	int32_t GetWinWidth() const { return window_width;}
	int32_t GetWinHeight() const { return window_height;}

	//�I��
	void Finalize();

	//���b�Z�[�W�̏���
	bool ProcessMessage();

	//�萔
	//�E�C���h�E�T�C�Y
	static constexpr int32_t window_width = 1280; //����
	static constexpr int32_t window_height = 720; //�c��

private:

	// �E�B���h�E
	WNDCLASSEX w{};
	// �E�B���h�E�I�u�W�F�N�g
	HWND hwnd;
	// �E�C���h�E�T�C�Y{X���W�@Y���W�@�����@�c��}
	RECT wrc;
};
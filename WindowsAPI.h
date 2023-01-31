#pragma once
#include <Windows.h>


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
	int GetWinWidth() const { return window_width;}
	int GetWinHeight() const { return window_height;}

	//�I��
	void Finalize();

	//���b�Z�[�W�̏���
	bool ProcessMessage();

	//�萔
	//�E�C���h�E�T�C�Y
	static constexpr int window_width = 1280; //����
	static constexpr int window_height = 720; //�c��

private:

	// �E�B���h�E
	WNDCLASSEX w{};
	// �E�B���h�E�I�u�W�F�N�g
	HWND hwnd;
	// �E�C���h�E�T�C�Y{X���W�@Y���W�@�����@�c��}
	RECT wrc;
};
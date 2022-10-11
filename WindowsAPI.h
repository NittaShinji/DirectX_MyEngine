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
	HWND GetHwndClass() { return hwnd; }
	WNDCLASSEX GetWindosClass() { return w;}
	RECT GetWrcClass() {return wrc;}
	int GetWinWidth() { return window_width; }
	int GetWinHeight() { return window_height; }

	//���b�Z�[�W�̏���
	bool ProcessMessage();

private:

	// �E�B���h�E
	WNDCLASSEX w{};
	// �E�B���h�E�I�u�W�F�N�g
	HWND hwnd;
	// �E�C���h�E�T�C�Y{X���W�@Y���W�@�����@�c��}
	RECT wrc;
	//�E�C���h�E�T�C�Y
	const int window_width = 1280; //����
	const int window_height = 720; //�c��

};
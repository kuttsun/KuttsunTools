// �^�X�N�o�[�ƃX�^�[�g�{�^���̕\���Ɣ�\����؂�ւ���v���O����
//
#include "stdafx.h"
#include <windows.h>
#include <tchar.h>

int main()
{
	// �N���X���ƃE�B���h�E��
	LPCTSTR name[][2] = {
		{ _T("Shell_TrayWnd")	, NULL },		// �^�X�N�o�[
		{ _T("Button")			, NULL },	// Windows�{�^��
		//{ _T("DV2ControlHost")	, NULL}		// �X�^�[�g���j���[
	};
	BOOL mode = FALSE;

	for (int i = 0; i < sizeof(name) / sizeof(name[0]); i++) {

		HWND hWnd = FindWindow(name[i][0], name[i][1]);

		if (i == 0) {
			// �^�X�N�o�[�̏ꍇ
			mode = IsWindowVisible(hWnd);// ���݂̕\����Ԃ��擾
		}

		if (mode == FALSE) {
			// ���݃^�X�N�o�[����\���̏ꍇ
			ShowWindow(hWnd, SW_SHOW);
			EnableWindow(hWnd, TRUE);// �����ɂ���Ă���ꍇ������̂ŁA�L���ɂ��Ă���
		}
		else {
			// ���݃^�X�N�o�[���\������Ă���ꍇ
			ShowWindow(hWnd, SW_HIDE);
		}
	}

	return 0;
}
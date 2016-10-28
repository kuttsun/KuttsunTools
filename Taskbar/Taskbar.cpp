// タスクバーとスタートボタンの表示と非表示を切り替えるプログラム
//
#include "stdafx.h"
#include <windows.h>
#include <tchar.h>

int main()
{
	// クラス名とウィンドウ名
	LPCTSTR name[][2] = {
		{ _T("Shell_TrayWnd")	, NULL },		// タスクバー
		{ _T("Button")			, NULL },	// Windowsボタン
		//{ _T("DV2ControlHost")	, NULL}		// スタートメニュー
	};
	BOOL mode = FALSE;

	for (int i = 0; i < sizeof(name) / sizeof(name[0]); i++) {

		HWND hWnd = FindWindow(name[i][0], name[i][1]);

		if (i == 0) {
			// タスクバーの場合
			mode = IsWindowVisible(hWnd);// 現在の表示状態を取得
		}

		if (mode == FALSE) {
			// 現在タスクバーが非表示の場合
			ShowWindow(hWnd, SW_SHOW);
			EnableWindow(hWnd, TRUE);// 無効にされている場合があるので、有効にしておく
		}
		else {
			// 現在タスクバーが表示されている場合
			ShowWindow(hWnd, SW_HIDE);
		}
	}

	return 0;
}
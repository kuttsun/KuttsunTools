// デバイスマネージャーの機器に問題が発生しているかどうかチェックするサンプルプログラム
//

#include "stdafx.h"

#include <windows.h>
#include <stdio.h>
#include <setupapi.h>
#include <cfgmgr32.h>
#include <tchar.h>
#include <locale.h>

int main()
{
	BOOL lret = FALSE;// 一時的な戻り値
	TCHAR *check_device[] = { _T("Realtek High Definition Audio"), _T("USB 大容量記憶装置") };// チェックするデバイス

	HDEVINFO devinfo = NULL;// デバイス情報

	int dev_index = 0;

	SP_DEVINFO_DATA devinfo_data;	// デバイス詳細情報
	TCHAR device_name[MAX_PATH] = { 0 };		// デバイス名
									//CHAR dev_inst_id[MAX_PATH];		// デバイスインスタンスID
	DWORD reg_type = 0;				//
	DWORD byte_size = 0;			//

	_tsetlocale(LC_ALL, _T("Japanese")); // これが前処理

	// デバイス情報取得
	devinfo = SetupDiGetClassDevs(NULL, 0, 0, DIGCF_PRESENT | DIGCF_ALLCLASSES);
	if (devinfo == NULL) {
		_tprintf(_T("デバイス情報取得エラー\n"));
		goto EndFunc;
	}


	// デバイススキャン
	for (dev_index = 0; ; dev_index++) {

		// 初期化
		ZeroMemory(&devinfo_data, sizeof(SP_DEVINFO_DATA));
		devinfo_data.cbSize = sizeof(SP_DEVINFO_DATA);
		ZeroMemory(device_name, sizeof(device_name));

		// デバイス詳細情報取得
		lret = SetupDiEnumDeviceInfo(devinfo, dev_index, &devinfo_data);
		if (lret == FALSE) {
			// 終端検出で終了
			break;
		}

		// デバイス名(説明)の取得
		lret = SetupDiGetDeviceRegistryProperty(devinfo, &devinfo_data, SPDRP_DEVICEDESC, &reg_type, (BYTE*)device_name, sizeof(device_name), &byte_size);
		if (lret != FALSE) {
			// 取得成功

			int index = 0;

			_tprintf(_T("デバイス名[%s]\n"), device_name);

			// 監視対象のデバイス名と一致するかどうか調べる
			for (index = 0; index < sizeof(check_device) / sizeof(check_device[0]); index++) {
				if (_tcscmp(device_name, check_device[index]) == 0) {

					// デバイスのステータスを取得
					ULONG dev_status = 0;
					ULONG dev_problem = 0;
					CONFIGRET cr = CM_Get_DevNode_Status(&dev_status, &dev_problem, devinfo_data.DevInst, 0);

					if (cr == CR_SUCCESS) {
						// ステータスをチェックする
						if ((dev_status & DN_HAS_PROBLEM) != 0) {
							// 障害が発生している
							_tprintf(_T("デバイスNG[%s]\n"), check_device[index]);
						}
						else {
							_tprintf(_T("デバイスOK[%s]\n"), check_device[index]);
						}
					}
					else {
						_tprintf(_T("ステータス取得失敗[%s]\n"), check_device[index]);
					}
					break;
				}
			}
		}
	}

EndFunc:
	// デバイス情報解放
	if (devinfo != NULL) {
		SetupDiDestroyDeviceInfoList(devinfo);
	}

	return 0;
}


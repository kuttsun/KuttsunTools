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
	BOOL lret[SPDRP_MAXIMUM_PROPERTY] = { 0 };// 一時的な戻り値
	
	HDEVINFO devinfo = NULL;// デバイス情報

	int dev_index = 0;

	DWORD reg_type = 0;
	DWORD byte_size = 0;

	_tsetlocale(LC_ALL, _T("Japanese")); // これが前処理

	// デバイス情報取得
	devinfo = SetupDiGetClassDevs(NULL, 0, 0, DIGCF_PRESENT | DIGCF_ALLCLASSES);
	if (devinfo == NULL) {
		_tprintf(_T("デバイス情報取得エラー\n"));
		goto EndFunc;
	}

	// デバイススキャン
	for (dev_index = 0; ; dev_index++) {

		// デバイス詳細情報
		SP_DEVINFO_DATA devinfo_data = { 0 };
		devinfo_data.cbSize = sizeof(SP_DEVINFO_DATA);
		// デバイスの各プロパティ名
		TCHAR device_name[SPDRP_MAXIMUM_PROPERTY][MAX_PATH] = { 0 };

		// デバイス詳細情報取得
		if (SetupDiEnumDeviceInfo(devinfo, dev_index, &devinfo_data) == FALSE) {
			// 終端検出で終了
			break;
		}

		// デバイスの全プロパティを取得
		// https://docs.microsoft.com/ja-jp/windows/desktop/api/setupapi/nf-setupapi-setupdigetdeviceregistrypropertya
		{
			int property = 0;
			_tprintf(_T("----------%d----------\n"),dev_index);
			for (property = 0; property < SPDRP_MAXIMUM_PROPERTY; property++) {
				lret[property] = SetupDiGetDeviceRegistryProperty(devinfo, &devinfo_data, property, &reg_type, (BYTE*)device_name[property], sizeof(device_name[property])/sizeof(device_name[property][0]), &byte_size);
				if (lret[property]) {
					// 取得成功
					_tprintf(_T("デバイス情報[%02x]=%s\n"), property, device_name[property]);
				}
				else{
					_tprintf(_T("デバイス情報[%02x]=(取得失敗)\n"), property);
				}
			}
		}

		// デバイスのチェック
		{
			// チェックするデバイス
			TCHAR *check_device[] = { _T("Realtek High Definition Audio"), _T("USB 大容量記憶装置") };
			// チェックに使用するプロパティ
			int property = SPDRP_DEVICEDESC;

			if (lret[property] != FALSE) {
				// 取得に成功している

				int index = 0;

				// 監視対象のデバイス名と一致するかどうか調べる
				for (index = 0; index < sizeof(check_device) / sizeof(check_device[0]); index++) {
					if (_tcscmp(device_name[property], check_device[index]) == 0) {

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
	}

EndFunc:
	// デバイス情報解放
	if (devinfo != NULL) {
		SetupDiDestroyDeviceInfoList(devinfo);
	}

	return 0;
}


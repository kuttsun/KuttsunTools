// �f�o�C�X�}�l�[�W���[�̋@��ɖ�肪�������Ă��邩�ǂ����`�F�b�N����T���v���v���O����
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
	BOOL lret = FALSE;// �ꎞ�I�Ȗ߂�l
	TCHAR *check_device[] = { _T("Realtek High Definition Audio"), _T("USB ��e�ʋL�����u") };// �`�F�b�N����f�o�C�X

	HDEVINFO devinfo = NULL;// �f�o�C�X���

	int dev_index = 0;

	SP_DEVINFO_DATA devinfo_data;	// �f�o�C�X�ڍ׏��
	TCHAR device_name[MAX_PATH] = { 0 };		// �f�o�C�X��
									//CHAR dev_inst_id[MAX_PATH];		// �f�o�C�X�C���X�^���XID
	DWORD reg_type = 0;				//
	DWORD byte_size = 0;			//

	_tsetlocale(LC_ALL, _T("Japanese")); // ���ꂪ�O����

	// �f�o�C�X���擾
	devinfo = SetupDiGetClassDevs(NULL, 0, 0, DIGCF_PRESENT | DIGCF_ALLCLASSES);
	if (devinfo == NULL) {
		_tprintf(_T("�f�o�C�X���擾�G���[\n"));
		goto EndFunc;
	}


	// �f�o�C�X�X�L����
	for (dev_index = 0; ; dev_index++) {

		// ������
		ZeroMemory(&devinfo_data, sizeof(SP_DEVINFO_DATA));
		devinfo_data.cbSize = sizeof(SP_DEVINFO_DATA);
		ZeroMemory(device_name, sizeof(device_name));

		// �f�o�C�X�ڍ׏��擾
		lret = SetupDiEnumDeviceInfo(devinfo, dev_index, &devinfo_data);
		if (lret == FALSE) {
			// �I�[���o�ŏI��
			break;
		}

		// �f�o�C�X��(����)�̎擾
		lret = SetupDiGetDeviceRegistryProperty(devinfo, &devinfo_data, SPDRP_DEVICEDESC, &reg_type, (BYTE*)device_name, sizeof(device_name), &byte_size);
		if (lret != FALSE) {
			// �擾����

			int index = 0;

			_tprintf(_T("�f�o�C�X��[%s]\n"), device_name);

			// �Ď��Ώۂ̃f�o�C�X���ƈ�v���邩�ǂ������ׂ�
			for (index = 0; index < sizeof(check_device) / sizeof(check_device[0]); index++) {
				if (_tcscmp(device_name, check_device[index]) == 0) {

					// �f�o�C�X�̃X�e�[�^�X���擾
					ULONG dev_status = 0;
					ULONG dev_problem = 0;
					CONFIGRET cr = CM_Get_DevNode_Status(&dev_status, &dev_problem, devinfo_data.DevInst, 0);

					if (cr == CR_SUCCESS) {
						// �X�e�[�^�X���`�F�b�N����
						if ((dev_status & DN_HAS_PROBLEM) != 0) {
							// ��Q���������Ă���
							_tprintf(_T("�f�o�C�XNG[%s]\n"), check_device[index]);
						}
						else {
							_tprintf(_T("�f�o�C�XOK[%s]\n"), check_device[index]);
						}
					}
					else {
						_tprintf(_T("�X�e�[�^�X�擾���s[%s]\n"), check_device[index]);
					}
					break;
				}
			}
		}
	}

EndFunc:
	// �f�o�C�X�����
	if (devinfo != NULL) {
		SetupDiDestroyDeviceInfoList(devinfo);
	}

	return 0;
}


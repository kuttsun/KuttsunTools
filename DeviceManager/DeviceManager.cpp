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
	BOOL lret[SPDRP_MAXIMUM_PROPERTY] = { 0 };// �ꎞ�I�Ȗ߂�l
	
	HDEVINFO devinfo = NULL;// �f�o�C�X���

	int dev_index = 0;

	DWORD reg_type = 0;
	DWORD byte_size = 0;

	_tsetlocale(LC_ALL, _T("Japanese")); // ���ꂪ�O����

	// �f�o�C�X���擾
	devinfo = SetupDiGetClassDevs(NULL, 0, 0, DIGCF_PRESENT | DIGCF_ALLCLASSES);
	if (devinfo == NULL) {
		_tprintf(_T("�f�o�C�X���擾�G���[\n"));
		goto EndFunc;
	}

	// �f�o�C�X�X�L����
	for (dev_index = 0; ; dev_index++) {

		// �f�o�C�X�ڍ׏��
		SP_DEVINFO_DATA devinfo_data = { 0 };
		devinfo_data.cbSize = sizeof(SP_DEVINFO_DATA);
		// �f�o�C�X�̊e�v���p�e�B��
		TCHAR device_name[SPDRP_MAXIMUM_PROPERTY][MAX_PATH] = { 0 };

		// �f�o�C�X�ڍ׏��擾
		if (SetupDiEnumDeviceInfo(devinfo, dev_index, &devinfo_data) == FALSE) {
			// �I�[���o�ŏI��
			break;
		}

		// �f�o�C�X�̑S�v���p�e�B���擾
		// https://docs.microsoft.com/ja-jp/windows/desktop/api/setupapi/nf-setupapi-setupdigetdeviceregistrypropertya
		{
			int property = 0;
			_tprintf(_T("----------%d----------\n"),dev_index);
			for (property = 0; property < SPDRP_MAXIMUM_PROPERTY; property++) {
				lret[property] = SetupDiGetDeviceRegistryProperty(devinfo, &devinfo_data, property, &reg_type, (BYTE*)device_name[property], sizeof(device_name[property])/sizeof(device_name[property][0]), &byte_size);
				if (lret[property]) {
					// �擾����
					_tprintf(_T("�f�o�C�X���[%02x]=%s\n"), property, device_name[property]);
				}
				else{
					_tprintf(_T("�f�o�C�X���[%02x]=(�擾���s)\n"), property);
				}
			}
		}

		// �f�o�C�X�̃`�F�b�N
		{
			// �`�F�b�N����f�o�C�X
			TCHAR *check_device[] = { _T("Realtek High Definition Audio"), _T("USB ��e�ʋL�����u") };
			// �`�F�b�N�Ɏg�p����v���p�e�B
			int property = SPDRP_DEVICEDESC;

			if (lret[property] != FALSE) {
				// �擾�ɐ������Ă���

				int index = 0;

				// �Ď��Ώۂ̃f�o�C�X���ƈ�v���邩�ǂ������ׂ�
				for (index = 0; index < sizeof(check_device) / sizeof(check_device[0]); index++) {
					if (_tcscmp(device_name[property], check_device[index]) == 0) {

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
	}

EndFunc:
	// �f�o�C�X�����
	if (devinfo != NULL) {
		SetupDiDestroyDeviceInfoList(devinfo);
	}

	return 0;
}


// ��ʉ𑜓x��ύX����T���v���v���O����
//

#include "stdio.h"
#include "Windows.h"


int main(void)
{
	DEVMODE devMode = { 0 };

	// ���݂̐ݒ���擾
	if (EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode) == TRUE) {

		printf("%d,%d\n", devMode.dmPelsWidth, devMode.dmPelsHeight);

		devMode.dmPelsWidth = 1920;
		devMode.dmPelsHeight = 1024;
		devMode.dmBitsPerPel = 32;

		// �ݒ��ύX
		// https://msdn.microsoft.com/ja-jp/library/cc428309.aspx
		if (ChangeDisplaySettings(&devMode, 0) == DISP_CHANGE_SUCCESSFUL) {
			printf("success");
		}
	}


	return 0;
}
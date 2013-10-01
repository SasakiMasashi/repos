#include "SaveBitMap.h"


void SaveBitmap(HBITMAP hBitMap,
				const string& OutputFilePath)
{	
    CImage image;
    image.Attach(hBitMap);
	HRESULT Result = image.Save(OutputFilePath.c_str());
	if(!SUCCEEDED(Result))
	{
		MessageBox(NULL, "�摜�t�@�C���̏o�͂Ɏ��s���܂����B", "���s", MB_OK);
	}

    image.Detach();
}


void SaveBitmap(int iX, int iY, int iWidth, int iHeight,
				const string& OutputFilePath)
{
	// ��ʂ̃f�o�C�X�R���e�L�X�g�擾
	HDC hdcScreen = GetDC(0);

	// �X�N���[���V���b�g�ۑ��p�r�b�g�}�b�v�쐬
	HBITMAP bmpShot = CreateCompatibleBitmap(hdcScreen, iWidth, iHeight);

	// �r�b�g�}�b�v�`��p�f�o�C�X�R���e�L�X�g�쐬
	HDC hdcShot = CreateCompatibleDC(hdcScreen);

	// �f�o�C�X�R���e�L�X�g�Ƀr�b�g�}�b�v��ݒ�
	HBITMAP bmpOld = (HBITMAP)SelectObject(hdcShot, bmpShot);

	// ��ʏ�̗̈���r�b�g�}�b�v�ɕ`��
	BitBlt(hdcShot, 0, 0, iWidth, iHeight, hdcScreen, iX, iY, SRCCOPY);

	SaveBitmap(bmpShot, OutputFilePath);

	// ��ʂ̃f�o�C�X�R���e�L�X�g���
	ReleaseDC(NULL, hdcScreen);	
}
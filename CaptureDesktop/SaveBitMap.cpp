#include "SaveBitMap.h"


void SaveBitmap(HBITMAP hBitMap,
				const string& OutputFilePath)
{	
    CImage image;
    image.Attach(hBitMap);
	HRESULT Result = image.Save(OutputFilePath.c_str());
	if(!SUCCEEDED(Result))
	{
		MessageBox(NULL, "画像ファイルの出力に失敗しました。", "失敗", MB_OK);
	}

    image.Detach();
}


void SaveBitmap(int iX, int iY, int iWidth, int iHeight,
				const string& OutputFilePath)
{
	// 画面のデバイスコンテキスト取得
	HDC hdcScreen = GetDC(0);

	// スクリーンショット保存用ビットマップ作成
	HBITMAP bmpShot = CreateCompatibleBitmap(hdcScreen, iWidth, iHeight);

	// ビットマップ描画用デバイスコンテキスト作成
	HDC hdcShot = CreateCompatibleDC(hdcScreen);

	// デバイスコンテキストにビットマップを設定
	HBITMAP bmpOld = (HBITMAP)SelectObject(hdcShot, bmpShot);

	// 画面上の領域をビットマップに描く
	BitBlt(hdcShot, 0, 0, iWidth, iHeight, hdcScreen, iX, iY, SRCCOPY);

	SaveBitmap(bmpShot, OutputFilePath);

	// 画面のデバイスコンテキスト解放
	ReleaseDC(NULL, hdcScreen);	
}
#pragma once;
#include <string>
#include <windows.h>
#include "atlimage.h"
using std::string;

void SaveBitmap(HBITMAP hBitMap,
				const string& OutputFilePath);


void SaveBitmap(int iX, int iY, int iWidth, int iHeight,
				const string& OutputFilePath);

#include <windows.h>
#include "resource.h"
#include "SaveBitMap.h"
#include <string>
#include <Richedit.h>
#include <stdlib.h>
#include <shlobj.h>
#include <sstream>
#include <fcntl.h>	// _O_BINARY
#include <io.h>	// _setmode, _access
#pragma comment(lib, "shell32.lib")
using std::string;

BOOL CALLBACK MyDlgProc(HWND, UINT, WPARAM, LPARAM);
void InitDialog(HWND);
void SetFullScreenRange(HWND);
void OnCapture(HWND);
bool CreateFolderBySHCreateDirectoryEx(LPCTSTR pszFolder);

const string DefaultOutputFolder = "C:\\CaptureDesktop";

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
				   LPSTR lpsCmdLine, int nCmdShow)
{
	_setmode(_fileno(stdout), _O_BINARY);	// cout
	_setmode(_fileno(stderr), _O_BINARY);	// cerr
	setlocale(LC_ALL,"");

	DialogBox(hInstance, MAKEINTRESOURCE(IDD_CAPTUREDESKTOP), NULL, (DLGPROC)MyDlgProc);
	return 0;
}

BOOL CALLBACK MyDlgProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
	switch(msg)
	{
	case WM_INITDIALOG:
			InitDialog(hDlg);
			return TRUE;
	case WM_COMMAND:
		switch(LOWORD(wp))
		{
		case IDC_SCREENSIZE:
			SetFullScreenRange(hDlg);
			return TRUE;
		case IDC_CAPTURE:
			OnCapture(hDlg);
			return TRUE;			
		case IDCANCEL:
			EndDialog(hDlg, IDOK);
			return TRUE;
		}
	default:
		return FALSE;
	}
	return FALSE;
}

void InitDialog(HWND hDlg)
{
	//�摜�t�H�[�}�b�g�̑I�������Z�b�g
	HWND Combo = GetDlgItem(hDlg, IDC_FORMAT);
	SendMessage(Combo, CB_INSERTSTRING, 0, (LPARAM)"gif");
	SendMessage(Combo, CB_INSERTSTRING, 1, (LPARAM)"jpg");
	SendMessage(Combo, CB_INSERTSTRING, 2, (LPARAM)"bmp");
	SendMessage(Combo, CB_INSERTSTRING, 3, (LPARAM)"png");
	SendMessage(Combo, CB_SETCURSEL, (WPARAM)0, 0);

	//�S��ʂ͈̔͂��擾
	SetFullScreenRange(hDlg);

	//�f�t�H���g�̃t�H���_���w��
	SetDlgItemText(hDlg, IDC_OUTPUTFOLDER, DefaultOutputFolder.c_str());
}

void SetFullScreenRange(HWND hDlg)
{
	//�S��ʔ͈͂��Z�b�g
	//int MinX = 0;
	//int MinY = 0;
	//int MaxX = GetSystemMetrics(SM_CXSCREEN);
	//int MaxY = GetSystemMetrics(SM_CYSCREEN);

	int MinX = GetSystemMetrics(SM_XVIRTUALSCREEN);
	int MinY = GetSystemMetrics(SM_YVIRTUALSCREEN);
	int MaxX = MinX + GetSystemMetrics(SM_CXVIRTUALSCREEN);
	int MaxY = MinY + GetSystemMetrics(SM_CYVIRTUALSCREEN);

	char cMinX[255], cMinY[255], cMaxX[255], cMaxY[255];
	_itoa_s(MinX, cMinX, 255, 10);
	_itoa_s(MinY, cMinY, 255, 10);
	_itoa_s(MaxX, cMaxX, 255, 10);
	_itoa_s(MaxY, cMaxY, 255, 10);

	SetDlgItemText(hDlg, IDC_MIN_X, cMinX);
	SetDlgItemText(hDlg, IDC_MIN_Y, cMinY);
	SetDlgItemText(hDlg, IDC_MAX_X, cMaxX);
	SetDlgItemText(hDlg, IDC_MAX_Y, cMaxY);
}

void OnCapture(HWND hDlg)
{
	char cOutputFolder[MAX_PATH];
	char cOutputFileName[MAX_PATH];
	char cFormat[MAX_PATH];

	//�o�̓t�H���_���擾
	GetDlgItemText(hDlg, IDC_OUTPUTFOLDER, cOutputFolder, MAX_PATH);
	string sOutputFolder(cOutputFolder);
	if(sOutputFolder.empty())
	{
		MessageBox(NULL, "�o�͐�t�H���_����ł��B", "���s", MB_OK);
		return;
	}

	//�w�肳�ꂽ�t�H���_��������΍쐬����
	bool Success = CreateFolderBySHCreateDirectoryEx((LPCTSTR)sOutputFolder.c_str());
	if(!Success)
	{
		MessageBox(NULL, "�w�肳�ꂽ�t�H���_�̍쐬�Ɏ��s���܂����B", "���s", MB_OK);
		return;
	}

	//�o�̓t�@�C�������擾
	GetDlgItemText(hDlg, IDC_OUTPUTFILENAME, cOutputFileName, MAX_PATH);
	string sOutputFileName(cOutputFileName);
	if(sOutputFileName.empty())
	{
		//���݂̎������擾
		SYSTEMTIME Systime;
		GetLocalTime(&Systime);

		std::stringstream sst;
		sst << Systime.wYear << "." << Systime.wMonth << "." << Systime.wDay << "."
			<< Systime.wHour << "." << Systime.wMinute << "." << Systime.wSecond << "." << Systime.wMilliseconds;
		sOutputFileName = sst.str();;
	}

	//�o�̓t�H�[�}�b�g���擾
	GetDlgItemText(hDlg, IDC_FORMAT, cFormat, MAX_PATH);
	string sFormat(cFormat);

	//�o�̓t�@�C�������쐬
	string OutputFilePath = sOutputFolder + string("\\") + sOutputFileName + string(".") + sFormat;

	//�摜�o��
	char cMinX[255], cMinY[255], cMaxX[255], cMaxY[255];
	GetDlgItemText(hDlg, IDC_MIN_X, cMinX, 255);
	GetDlgItemText(hDlg, IDC_MIN_Y, cMinY, 255);
	GetDlgItemText(hDlg, IDC_MAX_X, cMaxX, 255);
	GetDlgItemText(hDlg, IDC_MAX_Y, cMaxY, 255);
	int MinX = atoi(cMinX);
	int MinY = atoi(cMinY);
	int MaxX = atoi(cMaxX);
	int MaxY = atoi(cMaxY);

	SaveBitmap(MinX, MinY, MaxX - MinX, MaxY - MinY, OutputFilePath);
}

bool	
CreateFolderBySHCreateDirectoryEx(LPCTSTR pszFolder)
{
	int Success = SHCreateDirectoryEx(NULL,pszFolder,NULL);
	if(Success == ERROR_SUCCESS
	|| Success == ERROR_FILE_EXISTS
	|| Success == ERROR_ALREADY_EXISTS)
	{
		return true;
	}
	return false;
}

#pragma once
#include <string>
#include <afxinet.h>
using namespace std;

namespace FileUtil
{

bool	SaveHttpFile(const string& FileName, 
					 const string& URL,
					 const string& LogFilePath) 
{
	CString strFileName = CString(FileName.c_str());
	CString strURL = CString(URL.c_str());

	CStdioFile			cFile;
	CStdioFile*			lpcNetFile;
	CInternetSession	cSec("Open URL And Save");

	ofstream Log(LogFilePath.c_str(), ios::app);
	if(!Log)
	{
		return false;
	}

	Log << "OpenURL: " << URL << "\n";
	
	try
	{
		lpcNetFile = cSec.OpenURL(strURL);
		if(lpcNetFile == NULL)
		{
			Log << "    URL��Open�Ɏ��s���܂����B\n";
			return	false;
		}
	}
	catch(...)
	{
		Log << "    URL��Open���ɗ�O���������܂����B\n";
		return false;
	}

	/*if(0 ==_access(FileName.c_str(), 0))
	{
		Log << "    ���ɕۑ���t�@�C�������݂��܂��B\n";
		return true;
	}*/

	BOOL status = cFile.Open(strFileName, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone);
	if(0 == status)
	{
		Log << "    URL�̕ۑ���t�@�C�����J���܂���ł����BFileName: " << strFileName << "\n";
		return false;
	}

	try
	{
		while(1)
		{
			char buf[1024];
			if(NULL == lpcNetFile->ReadString(buf, 1024))
			{
				break;
			}
			cFile.WriteString(buf);
		}
	}
	catch(...)
	{
		Log << "    URL�̕ۑ��Ɏ��s���܂����B\n";
		return false;
	}
	cFile.Close();

	return	true;
}

} // FileUtil
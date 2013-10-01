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
			Log << "    URLのOpenに失敗しました。\n";
			return	false;
		}
	}
	catch(...)
	{
		Log << "    URLのOpen中に例外が発生しました。\n";
		return false;
	}

	/*if(0 ==_access(FileName.c_str(), 0))
	{
		Log << "    既に保存先ファイルが存在します。\n";
		return true;
	}*/

	BOOL status = cFile.Open(strFileName, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone);
	if(0 == status)
	{
		Log << "    URLの保存先ファイルが開けませんでした。FileName: " << strFileName << "\n";
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
		Log << "    URLの保存に失敗しました。\n";
		return false;
	}
	cFile.Close();

	return	true;
}

} // FileUtil
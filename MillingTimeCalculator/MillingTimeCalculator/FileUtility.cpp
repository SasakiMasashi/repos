#include "FileUtility.h"
#include <Windows.h>

void 
SeparateFilePath(const std::string& FilePath,
				 std::string& FileFolderPath,
				 std::string& FileName)
{
	FileFolderPath.clear();
	FileName.clear();

	if(FilePath.empty())
	{
		return;
	}

	std::string::size_type pos = FilePath.rfind("\\");
	if(pos == std::string::npos)
	{
		return;
	}

	// FolderPath‚ðŽæ“¾
	if(pos != 0)
	{
		FileFolderPath = FilePath.substr(0, pos);
	}

	// FileName‚ðŽæ“¾
	if(pos != FilePath.size() - 1)
	{
		FileName = FilePath.substr(pos + 1);
	}
}

static string
GetFolderPathWithEscape(const string& FolderPath)
{
	if(FolderPath.empty())
	{
		return string();
	}

	if('\\' == FolderPath.at(FolderPath.size() - 1))
	{
		return FolderPath;
	}
	else
	{
		return FolderPath + "\\";
	}
}

string
GetFilePath(const string& FolderPath, const string& FileName)
{
	return GetFolderPathWithEscape(FolderPath) + FileName;
}

vector<std::string>
GetAllFileName(const std::string& FolderPath,
			   const std::string& FileNameWildCard)
{
	vector<string> vFileName;
	string SearchFilePath = GetFilePath(FolderPath, FileNameWildCard);

	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile((LPCSTR)SearchFilePath.c_str(), &fd);
	if(hFind == INVALID_HANDLE_VALUE)
	{
		return vector<string>();
	}
	
	do{
		if(!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			  && !(fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN))
		{
			vFileName.push_back(string(fd.cFileName));
		}
	}while(FindNextFile(hFind, &fd));

	FindClose(hFind);

	return vFileName;
}
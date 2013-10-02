#pragma once
#include <string>
#include <vector>
using namespace std;

void 
SeparateFilePath(const std::string& FilePath,
				 std::string& FileFolderPath,
				 std::string& FileName);

string
GetFilePath(const string& FolderPath, const string& FileName);

vector<std::string>
GetAllFileName(const std::string& FolderPath,
			   const std::string& FileNameWildCard = "*.*");
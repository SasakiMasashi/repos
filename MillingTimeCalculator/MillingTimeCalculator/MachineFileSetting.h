#pragma once
#include <string>
using namespace std;

void GetMachinePathSetting(const string& NCFilePath,
						   string& NCHeader,
						   string& NCFooter,
						   string& NCExtension);
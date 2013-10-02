#include "MachineFileSetting.h"

void GetMachinePathSetting(const string& NCFilePath,
						   string& NCHeader,
						   string& NCFooter,
						   string& NCExtension)
{
	NCHeader = "(NC_HEADER)";
	NCFooter = "(NC_FOOTER)";
	NCExtension = "nc";
	if(NCFilePath.empty())
	{
		return;
	}

	string::size_type pos = string::npos;
	if(string::npos != (pos = NCFilePath.find(".spf")))
	{
		if(pos == NCFilePath.size() - 4)
		{
			NCHeader = ";NC_HEADER";
			NCFooter = ";NC_FOOTER";
			NCExtension = "spf";
		}
	}
}
#include "Unique.h"
#include <Rpc.h>

namespace Util
{
	std::string getUUID()
	{
		unsigned char *p;
		UUID uuid;

		::UuidCreate(&uuid);                  // UUID生成
		::UuidToString(&uuid, &p);            // UUIDを文字列にする

		char str[38];
		strncpy_s(str, 38, (char*)p, sizeof(str));   // 自分で用意したバッファにコピー

		std::string stUUID = str;

		::RpcStringFree(&p);

		return stUUID;
	}

} // namespace Util
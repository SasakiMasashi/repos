#include "Unique.h"
#include <Rpc.h>

namespace Util
{
	std::string getUUID()
	{
		unsigned char *p;
		UUID uuid;

		::UuidCreate(&uuid);                  // UUID����
		::UuidToString(&uuid, &p);            // UUID�𕶎���ɂ���

		char str[38];
		strncpy_s(str, 38, (char*)p, sizeof(str));   // �����ŗp�ӂ����o�b�t�@�ɃR�s�[

		std::string stUUID = str;

		::RpcStringFree(&p);

		return stUUID;
	}

} // namespace Util
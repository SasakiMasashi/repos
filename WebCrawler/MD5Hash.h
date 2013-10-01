#pragma once
#include "md5.h"
#include <string>
#include <sstream>
using namespace std;

class MD5HashFunc
{
public:
	static string getHashValue(const string& st_base)
	{
		// 最後にスラッシュがない場合はここでつける
		// ここでやるのはおかしいけれど便宜上ここで行う
		string st = st_base;
		string::size_type pos = st.rfind("/");
		if(pos != (int)st.size() - 1)
		{
			st += string("/");
		}

		md5_context ctx;
		md5_starts(&ctx);

		unsigned char* buf = const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(st.c_str()));
		md5_update(&ctx, buf, (unsigned long)st.size());

		unsigned char tmpValue[16];

		md5_finish( &ctx, tmpValue );
		stringstream sst;

		
		for(size_t i = 0; i < 16; i++)
		{
			char str[100];
			sprintf_s(str, 100, "%02x", tmpValue[i]);
			sst << str;
		}

		ofstream fout("C:\\develop\\MyProject\\SearchEngine\\WebCrawler\\WebCrawler\\CrawledData\\MD5.txt", ios::app);
		if(fout.is_open())
		{
			fout << st << "\t\t" << sst.str() << "\n";
		}

		return sst.str();
	}
};
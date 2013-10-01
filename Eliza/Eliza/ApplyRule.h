#pragma once
#include <map>
#include <time.h>
using namespace std;

namespace Eliza
{
	class CRuleDatabaseAbstract;

	class ApplyRule
	{
	public:
		ApplyRule(const CRuleDatabaseAbstract* Database)
			: m_RuleDatabase(Database) 
		{
			srand((unsigned int )time(NULL));
		}

		string Do(const string& Line);

	private:
		const CRuleDatabaseAbstract* m_RuleDatabase;
	};
}
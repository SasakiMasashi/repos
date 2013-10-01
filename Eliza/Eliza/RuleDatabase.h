#pragma once
#include "Rule.h"
#include <map>
using namespace std;

namespace Eliza
{
	class CRuleDatabaseAbstract
	{
	public:
		virtual void CreateDatabase() = 0;

		const map<const char*, Rule*>& GetRuleDictionary() const { return m_RuleDictionary; }
		const Rule* GetDefaultRule() const { return m_DefaultRule; }

	protected:
		map<const char*, Rule*> m_RuleDictionary;
		Rule* m_DefaultRule;
	};

	class RuleDatabase : public CRuleDatabaseAbstract
	{
	public:
		RuleDatabase();
		~RuleDatabase();

		void CreateDatabase();
	};

	class RuleDatabase2 : public CRuleDatabaseAbstract
	{
	public:
		RuleDatabase2();
		~RuleDatabase2();

		void CreateDatabase();
	};
}
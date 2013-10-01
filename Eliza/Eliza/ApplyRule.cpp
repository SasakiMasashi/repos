#include <stdlib.h>
#include "ApplyRule.h"
#include "Rule.h"
#include "RuleDatabase.h"
#include "Utility.h"

namespace Eliza
{
	string 
	ApplyRule::Do(const string& Line)
	{
		const map<const char* ,Rule*>& RuleDictionary = m_RuleDatabase->GetRuleDictionary();
		map<const char* , Rule*>::const_iterator it = RuleDictionary.begin();
		vector<Rule*> vRule;
		vRule.reserve(RuleDictionary.size());

		// ヒットするルールを全て探す
		for(; it != RuleDictionary.end(); it++)
		{
			if(!it->first || !it->second)
				continue;

			if(string::npos == Line.find(string(it->first)))
				continue;

			vRule.push_back(it->second);
		}

		if(vRule.empty())
			return m_RuleDatabase->GetDefaultRule()->GetAnswer(string(), string());

		// ヒットしたルールからランダムに適用対象を選択
		int RuleId = GetId((int)vRule.size());
		const Rule& rule = *vRule[RuleId];

		string Before, After;
		TrimDecompose(Line, rule.GetRuleKey(), Before, After);

		// 選択されたルールからランダムに回答を選択
		return rule.GetAnswer(Before, After);
	}
}
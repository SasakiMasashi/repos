#pragma once
#include <set>
#include <string>
#include <vector>
using namespace std;

namespace Eliza
{
	class Rule
	{
	public:
		class AnswerCandidate
		{
		public:
			AnswerCandidate(const string& Candidate) : m_Candidate(Candidate) {}
			const string& GetCandidate() const { return m_Candidate; }
		private:
			string m_Candidate;
		};

	public:
		Rule(const string& RuleKey) : m_RuleKey(RuleKey) {}

	public:
		void AddAnswerCandidate(const string& Candidate);
		string GetAnswer(const string& Before, const string& After) const;
		
		const string& GetRuleKey() const { return m_RuleKey; }
	private:
		string m_RuleKey;
		vector<AnswerCandidate> m_vAnswerCandidate;
	};
}
#include "Rule.h"
#include "Utility.h"

namespace Eliza
{
	void Rule::AddAnswerCandidate(const string& Candidate)
	{
		m_vAnswerCandidate.push_back(AnswerCandidate(Candidate));
	}

	string Rule::GetAnswer(const string& Before, const string& After) const 
	{
		int id = GetId((int)m_vAnswerCandidate.size());
		string Answer = Replace(m_vAnswerCandidate[id].GetCandidate(), "__BeforeKey__", Before);
		return Replace(Answer, "__AfterKey__", After);
	}
}
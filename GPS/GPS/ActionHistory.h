#pragma once;
#include <vector>
#include "Action.h"
#include "Operation.h"
using namespace std;

namespace GPS
{
	class RunActionHistory
	{
	public:
		RunActionHistory() :m_Achieved(false), m_vOperation() {}
	public:
		//const vector<Action>& GetActions() const { return m_vAction; }
		//void Add(const Action& Act) { m_vAction.push_back(Act); }
		const vector<const Operation*>& GetOperations() const { return m_vOperation; }
		void Add(const Operation* Ope) { m_vOperation.push_back(Ope); }
		void SetAchievedFlag(const bool bAchieved) { m_Achieved = bAchieved; }
		bool GetAchievedFlag() const { return m_Achieved; }
	private:
		//vector<Action> m_vAction;
		vector<const Operation*> m_vOperation;
		bool m_Achieved;
	};
}
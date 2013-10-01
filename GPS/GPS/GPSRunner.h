#pragma once
#include <map>
#include "SetUtility.h"
#include "State.h"
#include "Action.h"
#include "Operation.h"
#include "ActionHistory.h"
#include <iostream>
#include "ExecutionManager.h"
using namespace std;

namespace GPS
{
	enum StatusType
	{
		GPS_NOERROR,
		GPS_ERROR
	};


	void ApplyOn(const Operation& Ope,
				 set<State>& States,
				 RunActionHistory& ActionHistory,
				 set<const Operation*>& SearchingOperations);

	bool AchieveAll(set<State>& States,
					  const set<State>& FinalStates,
					  const set<Operation>& Opes,
					  RunActionHistory& ActionHistory,
					  set<const Operation*>& SearchingOperations);

	void GetOpesWhichAddhasSpecifiedState(const State& SpecifiedState, 
										  const set<Operation>& Opes, 
										  set<Operation>& OpesWhichAddhasSpecifiedState,
										  set<const Operation*>& SearchingOperations);

	bool
	IsAllPreStatesRegistered(const Operation& op, const set<State>& set);

	bool Achieve(set<State>& States,
					  const State& FinalState,
					  const set<Operation>& Opes,
					  RunActionHistory& ActionHistory,
					  set<const Operation*>& SearchingOperations);

	class GPSRunner
	{

	public:
		StatusType
		Do(const set<State>& InitialStates,
				  const set<State>& FinalStates,
				  const set<Operation>& Opes,
				  RunActionHistory& ActionHistory);

		void 
		OutputResult(const set<State>& InitialStates,
							const set<State>& FinalState,
							RunActionHistory& ActionHistory, 
							ostream& out,
							ExecutionManager& ExeManager);

	public:
		void	SetXMLFilePath(const string& XMLFilePath) { m_XMLFilePath = XMLFilePath; }
		string	GetXMLFilePath() const { return m_XMLFilePath; }
	private:
		string	m_XMLFilePath;
	};
}
#pragma once
#include <string>
#include <set>
#include "Operation.h"
#include "ExecutionManager.h"
using namespace std;

namespace GPS
{
	bool LoadData(const string& XMLFilePath,
				  set<State>& InitialStates, 
				  set<State>& FinalStates, 
				  set<Operation>& Opes,
				  ExecutionManager& ExeManager);
	//bool LoadData2(const string& XMLFilePath,
	//			  set<State>& InitialStates, 
	//				set<State>& FinalStates, 
	//				set<Operation>& Opes);
}
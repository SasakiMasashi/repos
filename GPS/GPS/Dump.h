#pragma once
#include <set>
#include "State.h"
#include "Operation.h"
#include <ostream>
#include <fstream>
using namespace std;

namespace GPS
{
	void Dump(const set<State>& States, ostream& out);
	void Dump(const set<Operation>& Opes, ostream& out);
	//void Dump(const set<State>& States, ofstream& out);
	//void Dump(const set<Operation>& Opes, ofstream& out);
	//void Dump(const set<const State*>& States, ofstream& out);
	//void Dump(const set<const Operation*>& Opes, ofstream& out);
}
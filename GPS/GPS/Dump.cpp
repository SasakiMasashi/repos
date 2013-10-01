#include "Dump.h"

namespace GPS
{
	void Dump(const set<State>& States, ostream& out)
	{
		set<State>::const_iterator cit = States.begin();
		for(; cit != States.end(); cit++)
		{
			out << "\t State: " << cit->GetName().c_str() << endl;
		}
	}

	void Dump(const set<Operation>& Opes, ostream& out)
	{
		set<Operation>::const_iterator cit = Opes.begin();
		int count = 0;
		for(; cit != Opes.end(); cit++, count++)
		{
			out << "Operation: " << count + 1 << endl;

			// Action
			{
				out << "\t Action:    " << cit->GetAction().GetName().c_str() << endl;
			}

			// PreState
			{
				const set<State>& PreStates = cit->GetPreStates();
				set<State>::const_iterator cit_pre = PreStates.begin();
				for(; cit_pre != PreStates.end(); cit_pre++)
				{
					out << "\t PreState:  " << cit_pre->GetName().c_str() << endl;
				}
			}

			// AddState
			{
				const set<State>& AddStates = cit->GetAddStates();
				set<State>::const_iterator cit_add = AddStates.begin();
				for(; cit_add != AddStates.end(); cit_add++)
				{
					out << "\t AddState:  " << cit_add->GetName().c_str() << endl;
				}
			}

			// DelState
			{
				const set<State>& DelStates = cit->GetDelStates();
				set<State>::const_iterator cit_del = DelStates.begin();
				for(; cit_del != DelStates.end(); cit_del++)
				{
					out << "\t DelState:  " << cit_del->GetName().c_str() << endl;
				}
			}

		}
	}

	//void Dump(const set<State>& States, ofstream& out)
	//{
	//	set<State>::const_iterator cit = States.begin();
	//	for(; cit != States.end(); cit++)
	//	{
	//		out << "\t State: " << cit->GetName().c_str() << endl;
	//	}
	//}

	//void Dump(const set<Operation>& Opes, ofstream& out)
	//{
	//	set<Operation>::const_iterator cit = Opes.begin();
	//	int count = 0;
	//	for(; cit != Opes.end(); cit++, count++)
	//	{
	//		out << "Operation: " << count + 1 << endl;

	//		// Action
	//		{
	//			out << "\t Action:    " << cit->GetAction().GetName().c_str() << endl;
	//		}

	//		// PreState
	//		{
	//			const set<State>& PreStates = cit->GetPreStates();
	//			set<State>::const_iterator cit_pre = PreStates.begin();
	//			for(; cit_pre != PreStates.end(); cit_pre++)
	//			{
	//				out << "\t PreState:  " << cit_pre->GetName().c_str() << endl;
	//			}
	//		}

	//		// AddState
	//		{
	//			const set<State>& AddStates = cit->GetAddStates();
	//			set<State>::const_iterator cit_add = AddStates.begin();
	//			for(; cit_add != AddStates.end(); cit_add++)
	//			{
	//				out << "\t AddState:  " << cit_add->GetName().c_str() << endl;
	//			}
	//		}

	//		// DelState
	//		{
	//			const set<State>& DelStates = cit->GetDelStates();
	//			set<State>::const_iterator cit_del = DelStates.begin();
	//			for(; cit_del != DelStates.end(); cit_del++)
	//			{
	//				out << "\t DelState:  " << cit_del->GetName().c_str() << endl;
	//			}
	//		}

	//	}
	//}

	//void Dump(const set<const State*>& States, ofstream& out)
	//{
	//	set<const State*>::const_iterator cit = States.begin();
	//	for(; cit != States.end(); cit++)
	//	{
	//		out << "\t State: " << (*cit)->GetName().c_str() << endl;
	//	}
	//}

	//void Dump(const set<const Operation*>& Opes, ofstream& out)
	//{
	//	set<const Operation*>::const_iterator cit = Opes.begin();
	//	int count = 0;
	//	for(; cit != Opes.end(); cit++, count++)
	//	{
	//		out << "Operation: " << count + 1 << endl;

	//		// Action
	//		{
	//			out << "\t Action:    " << (*cit)->GetAction().GetName().c_str() << endl;
	//		}

	//		// PreState
	//		{
	//			const set<State>& PreStates = (*cit)->GetPreStates();
	//			set<State>::const_iterator cit_pre = PreStates.begin();
	//			for(; cit_pre != PreStates.end(); cit_pre++)
	//			{
	//				out << "\t PreState:  " << cit_pre->GetName() << endl;
	//			}
	//		}

	//		// AddState
	//		{
	//			const set<State>& AddStates = (*cit)->GetAddStates();
	//			set<State>::const_iterator cit_add = AddStates.begin();
	//			for(; cit_add != AddStates.end(); cit_add++)
	//			{
	//				out << "\t AddState:  " << cit_add->GetName() << endl;
	//			}
	//		}

	//		// DelState
	//		{
	//			const set<State>& DelStates = (*cit)->GetDelStates();
	//			set<State>::const_iterator cit_del = DelStates.begin();
	//			for(; cit_del != DelStates.end(); cit_del++)
	//			{
	//				out << "\t DelState:  " << cit_del->GetName().c_str() << endl;
	//			}
	//		}

	//	}
	//}
}
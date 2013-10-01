#pragma once
#include <set>
#include <vector>
#include "State.h"
#include "Action.h"
using namespace std;

namespace GPS
{
	class Operation
	{
	public:
		Operation(Action& Act, set<State>& PreStates, set<State>& AddStates, set<State>& DelStates)
			:m_Action(Act), m_PreStates(PreStates), m_AddStates(AddStates), m_DelStates(DelStates), 
			m_SetCommandLineFlag(false), m_CommandLine() {} 
		Operation(const Operation& ope)
			:m_Action(ope.GetAction()), m_PreStates(ope.GetPreStates()), m_AddStates(ope.GetAddStates()), 
			m_DelStates(ope.GetDelStates()), m_SetCommandLineFlag(ope.GetCommandLineFlag()), m_CommandLine(ope.GetCommandLineSt())
		{
		}

	public:
		bool	operator==(const Operation& ope)	const	{ return ( m_Action == ope.GetAction()); }
		bool	operator<(const Operation& ope)		const	{ return ( m_Action < ope.GetAction()); }
		Operation& operator=(const Operation& ope)			
		{ 
			m_Action = ope.GetAction(); 
			m_PreStates = ope.GetPreStates();
			m_AddStates = ope.GetAddStates(); 
			m_DelStates = ope.GetDelStates(); 
			return *this; 
		}

	public:
		const Action&		GetAction()		const { return m_Action; }
		const set<State>&	GetPreStates()	const { return m_PreStates; }
		const set<State>&	GetAddStates()	const { return m_AddStates; }
		const set<State>&	GetDelStates()	const { return m_DelStates; }
		void				SetCommandLine(const string& ExeFilePath)
		{
			m_SetCommandLineFlag = true;
			m_CommandLine = ExeFilePath;
		}
		string				GetCommandLineSt() const { return m_CommandLine; }
		bool				GetCommandLineFlag() const { return m_SetCommandLineFlag; }
		
	private:
		Action		m_Action;
		set<State>	m_PreStates;
		set<State>	m_AddStates;
		set<State>	m_DelStates;
		bool		m_SetCommandLineFlag;
		string		m_CommandLine;
	};
}
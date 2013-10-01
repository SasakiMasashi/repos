#pragma once
#include <string>
using namespace std;

namespace GPS
{
	class State
	{
	public:
		explicit State(const string& Name) : m_Name(Name) {}
		State(const State& st) : m_Name(st.GetName()) {}
	public:
		const string& GetName() const { return m_Name; }
	public:
		bool	operator==(const State& s)	const	{ return (0 == m_Name.compare(s.GetName())); }
		bool	operator<(const State& s)	const	{ return ( m_Name < s.GetName()) ; }
		State&	operator=(const State& s)			{ m_Name = s.GetName(); return *this; }
	public:
		string m_Name;
	};
}
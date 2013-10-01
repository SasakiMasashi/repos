#pragma once
#include <string>
using namespace std;

namespace GPS
{
	class Action
	{
	public:
		explicit Action(const string& Name) : m_Name(Name) {}
		Action(const Action& Act) : m_Name(Act.GetName()) {}
	public:
		const string& GetName() const { return m_Name; }
	public:
		bool	operator==(const Action& a) const	{ return (0 == m_Name.compare(a.GetName())); }
		bool	operator<(const Action& a)	const	{ return ( m_Name < a.GetName()); }
		Action& operator=(const Action& a)			{ m_Name = a.GetName(); return *this; }

	private:
		string m_Name;
	};
}
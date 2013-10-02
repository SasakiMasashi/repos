#pragma once
#include <vector>
using namespace std;

class Deletion
{
public:
	template<typename T>
	void operator()(T* ob)
	{
		delete ob;
	}
};

template<typename T>
class ContainerDeletor
{
public:
	explicit ContainerDeletor(T* P)
		:m_P(P)
	{}

	~ContainerDeletor()
	{
		for_each(m_P->begin(), m_P->end(), Deletion());
		m_P->clear();
	}

	T* m_P;
};
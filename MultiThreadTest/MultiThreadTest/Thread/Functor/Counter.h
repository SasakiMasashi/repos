#pragma once

#include "Thread/Core/IThreadFunctor.h"
#include <iostream>
#include <string>

namespace ThreadFunctor
{

class Counter : public IThreadFunctor
{
public:
	Counter(const size_t CountNum, const std::string& Message)
		:m_CountNum(CountNum),
		 m_Message(Message)
	{}

	void Run()
	{
		for(size_t i = 0; i < m_CountNum; ++i)
		{
			std::cout << m_Message << " : " << i << "\n";
		}
	}

private:
	size_t m_CountNum;
	std::string m_Message;
};

} // namespace ThreadFunctor
#pragma once
#include <string>
#include <boost/shared_ptr.hpp>
using namespace std;

static bool
ExistBaseFilePath(const string& url)
{
	return ((0 == url.find("http")) || (0 == url.find("https")));
}

class URL;
typedef boost::shared_ptr<URL> URLPointer; 
class URL
{
public:
	explicit URL(const string& url)
		:m_Url(url)
	{
	}

	bool operator<(const URL& url) const
	{
		return (this->get() < url.get());
	}

	void setParentURL(URLPointer pURL)
	{
		m_ParentURL = pURL;
	}

	void setBaseFilePath(const string& BaseFilePath)
	{
		if(BaseFilePath.empty())
		{
			return;
		}

		// BaseFilePath‚ÌÅŒã‚É‚Í/‚ð‚Â‚¯‚é
		string tmpBaseFilePath = BaseFilePath;
		string::size_type pos = BaseFilePath.rfind("/");
		if(pos != (int)BaseFilePath.size() - 1)
		{
			tmpBaseFilePath += "/";
		}

		m_BaseFilePath = tmpBaseFilePath;
		if(!ExistBaseFilePath(m_Url))
		{
			// url‚ÌÅ‰‚©‚ç‚Í/‚ð‚Í‚¸‚·
			string tmpUrl = m_Url;
			string::size_type pos2 = m_Url.find("/");
			if(pos2 == 0)
			{
				tmpUrl = m_Url.substr(1);
			}
			m_Url = m_BaseFilePath + tmpUrl;
		}
	}

	string getBaseFilePath() const
	{
		if(!m_BaseFilePath.empty())
		{
			return m_BaseFilePath;
		}

		if(m_ParentURL.get())
		{
			return m_ParentURL->getBaseFilePath();
		}
		return string();
	}

	string get() const
	{
		string url;
		if(!ExistBaseFilePath(m_Url))
		{
			url = getBaseFilePath() + m_Url;
		}
		else
		{
			url = m_Url; 
		}

		return url;
	}
private:
	string		m_Url;
	string		m_BaseFilePath;
	URLPointer	m_ParentURL;
};
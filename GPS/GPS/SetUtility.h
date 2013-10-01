#pragma once
#include <set>
#include <algorithm>
using namespace std;

template<typename T>
static bool
IsMember(const T& a, const set<T>& sa)
{
	return (sa.end() != sa.find(a));
}

template<typename T>
static bool
IsSubSet(const set<T>& s1, const set<T>& s2)
{
	set<T>::const_iterator it1 = s1.begin();
	for(; it1 != s1.end(); it1++){
		if(!IsMember(*it1, s2))
			return false;
	}
	return true;
}

template<typename T>
static void
AddSet(const set<T>& From, set<T>& To)
{
	To.insert(From.begin(), From.end());
}

template<typename T>
static void
DelSet(const set<T>& s1, set<T>& s2)
{
	set<T>::const_iterator it1 = s1.begin();
	for(; it1 != s1.end(); it1++){
		set<T>::iterator it2 = find(s2.begin(), s2.end(), *it1);
		if(s2.end() != it2){
			s2.erase(it2);
		}
	}	
}
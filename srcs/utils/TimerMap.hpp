#ifndef TIMERMAP_HPP
#define TIMERMAP_HPP

#include <map>
#include <vector>
#include <iostream>
#include <ctime>

template<typename T>
class TimerMap : public std::map<T, std::pair<std::time_t, int> >
{
public:
	void add(T value, int timer)
	{
		(*this)[value] = std::make_pair(std::time(NULL), timer);
	}

	void timeOutChecker(void (*fun)(T))
	{
		typename TimerMap<T>::iterator it = this->begin();
		typename TimerMap<T>::iterator ite = this->end();
		while (it != ite)
		{
			if (it->second.second != 0 && std::difftime(std::time(NULL), it->second.first) > it->second.second)
			{
				typename TimerMap<T>::iterator ittmp = it;
				ittmp++;
				remove(it->first, fun);
				it = ittmp;
			}
			if (it != ite)
				it++;
		}
	}

	void remove(T value, void (*fun)(T))
	{
		typename TimerMap<T>::iterator it = this->find(value);
		if (it != this->end())
		{
			fun(value);
			typename TimerMap<T>::iterator it2 = this->find(value);
			if (it2 != this->end())
			{
				this->erase(it2);
			}
		}
	}

	void clean(void (*fun)(T))
	{
		typename TimerMap<T>::iterator it = this->begin();
		typename TimerMap<T>::iterator ite = this->end();
		while (it != ite)
		{
			typename TimerMap<T>::iterator ittmp = it;
			ittmp++;
			remove(it->first, fun);
			it = ittmp;
			if (it != ite)
				it++;
		}
	}
};

#endif

#include "Tube.h"


Tube::Tube(int free_size, int nofreesize )
{
	m_free.set_CanUse();
	m_nofree.set_CanUse();

	m_free.set_Val(free_size);
	m_nofree.set_Val(nofreesize);
}

Tube::~Tube()
{
	m_free.~Sem_Pv();
	m_nofree.~Sem_Pv();
}

int Tube::size()
{
	lock_guard<mutex> lock(m_mu);
	return m_task_vec.size();
}

void Tube::put(shared_ptr<TubeImple> pt)
{
	m_free.sem_V();
	{
		lock_guard<mutex> lock(m_mu);
		m_task_vec.push_back(pt);
	}
	m_nofree.sem_P();
}

shared_ptr<TubeImple> Tube::get()
{
	m_nofree.sem_V();
	shared_ptr<TubeImple> ret;
	{
		lock_guard<mutex> lock(m_mu);
		ret = m_task_vec.front();
		m_task_vec.pop_front();
	}
	m_free.sem_P();
	return ret;
}

void Tube::stop()
{
    put(nullptr);
}

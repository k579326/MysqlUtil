

#pragma once

#include <algorithm>
#include <vector>
#include <memory>
#include "AutoLock.h"
#include <pthread.h>
#include <semaphore.h>
//#include <mysql/mysql.h>

using namespace std;

namespace pool
{

typedef enum _em_error_
{
    ERR_OK = 0,
    ERR_COMMON = -1,
    ERR_NO_SRC = 1
}em_error_t;



template <typename T>
class pool_element
{
public:
    pool_element() : used(false), sp(nullptr)
    {
    }

    bool used;
    shared_ptr<T> sp;
};

template <typename T>
class common_pool
{
public:
    common_pool(int size) : m_poolSize(size), m_inited(false) {}
    virtual ~common_pool() {}

    shared_ptr<T> pl_pick();
    void pl_back(shared_ptr<T>&& sp);

protected:
    int     init();
    void    uninit();
    
private:

    virtual shared_ptr<T> createRes() = 0;

	bool isReady()
	{
		return m_inited;
	}

    int initSyncSrc()
    {
        int ret = 0;
        if (pthread_cond_init(&m_cond, NULL) != 0)
        {
            return ERR_COMMON;
        }

        if (pthread_mutex_init(&m_mutex, NULL) != 0)
        {
            pthread_cond_destroy(&m_cond);
            return ERR_COMMON;
        }

        if (sem_init(&m_resourceSem, 0, m_poolSize) != 0)
        {
            pthread_cond_destroy(&m_cond);
            pthread_mutex_destroy(&m_mutex);
            return ERR_COMMON;
        }

        return 0;
    }

    void uninitSyncSrc()
    {
        pthread_cond_destroy(&m_cond);
        pthread_mutex_destroy(&m_mutex);
        sem_destroy(&m_resourceSem);
    }

private:
    bool m_inited;
    pthread_cond_t m_cond;
    pthread_mutex_t m_mutex;
    sem_t m_resourceSem;
    vector<pool_element<T>> m_container;
    const int m_poolSize;


	xLock m_selfLock;
};

template <typename T>
int common_pool<T>::init()
{
	AutoLock autolock(&m_selfLock);

	if (isReady())
	{
		return 0;
	}

    if (initSyncSrc() != 0)    
    {
        return ERR_COMMON;
    }

    m_container.reserve(m_poolSize);

    for (int i = 0; i < m_poolSize; i++)
    {
        pool_element<T> e;
        e.sp = createRes();
        e.used = false;
        if (e.sp == nullptr)
        {
            uninitSyncSrc();
            return ERR_COMMON;
        }
        m_container.push_back(std::move(e));
    }

    m_inited = true;
    return 0;
}

template <typename T>
void common_pool<T>::uninit()
{
	AutoLock autolock(&m_selfLock);

	if (!m_inited)
	{
		return;
	}

    uninitSyncSrc();
    m_container.clear();
    m_inited = false;
}

template <typename T>
shared_ptr<T> common_pool<T>::pl_pick()
{
    int ret = 0;
    shared_ptr<T> sp(nullptr);

	AutoLock autolock(&m_selfLock);

    if (!m_inited)
    {
        return nullptr;
    }

    ret = sem_trywait(&m_resourceSem);
    if (ret == -1)
    {/*  */
        return nullptr;
    }

    ret = pthread_mutex_lock(&m_mutex);
    if (ret != 0)
    {
		sem_post(&m_resourceSem);
        return nullptr;
    }

    auto it = find_if(m_container.begin(), m_container.end(), [&](const pool_element<T>& x)->bool { return x.used == false;});

    if (it != m_container.end())
    {
        sp = it->sp;
        it->used = true;
        
    }

    pthread_mutex_unlock(&m_mutex);

    return sp;
}

template <typename T>
void common_pool<T>::pl_back(shared_ptr<T>&& sp)
{
    int ret = ERR_OK;

	AutoLock autolock(&m_selfLock);

    if (!m_inited)
    {
        return;
    }

    if (pthread_mutex_lock(&m_mutex))
    {
        return;
    }

    shared_ptr<T> ptr_back = std::move(sp);
    auto it = find_if(m_container.begin(), m_container.end(), [&](const pool_element<T>& x)->bool{ return x.sp.get() == ptr_back.get(); });
    if (it != m_container.end())
    {
        it->used = false;
        sem_post(&m_resourceSem);
    }

    pthread_mutex_unlock(&m_mutex);

    return ;
}



}
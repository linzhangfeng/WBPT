#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <deque>
#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <stdio.h>

using namespace std;

template<class DataType>
class Queue
{
public:
    Queue():_nready(0)
    {
        pthread_mutex_init(&_mutex, NULL);
        pthread_cond_init(&_cond, NULL);
    }
    
    ~Queue()
    {
       
        pthread_mutex_lock(&_mutex); 
        _queue.clear();
        pthread_mutex_unlock(&_mutex);
        
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_cond);
    }
    
    void put_msg(DataType d)
    {
        pthread_mutex_lock(&_mutex); 
        _queue.push_back(d);
        _nready++;
        pthread_mutex_unlock(&_mutex);
		pthread_cond_signal(&_cond);
    }
	
    void push_msg(DataType d)
    {
        pthread_mutex_lock(&_mutex); 
        _queue.push_back(d);
        _nready++;
        pthread_mutex_unlock(&_mutex);
    }

    void get_msg(DataType &d)
    {
        pthread_mutex_lock(&_mutex); 
        while (_nready == 0)
              pthread_cond_wait(&_cond, &_mutex);
        _nready--;
        d = _queue.front();
        _queue.pop_front();
        pthread_mutex_unlock(&_mutex);
    }
	
    int pop_msg(DataType &d)
    {
        pthread_mutex_lock(&_mutex); 
        if (_nready == 0)
		{
			pthread_mutex_unlock(&_mutex);
			return -1;
		}
        _nready--;
        d = _queue.front();
        _queue.pop_front();
        pthread_mutex_unlock(&_mutex);
		
		return 0;
    }
    
    int get_msg(DataType &d, int sec)
    {
        timeval now;
        struct timespec timeout;
        int retval = 0;
        
        pthread_mutex_lock(&_mutex); 
        while (_nready == 0) {
            gettimeofday(&now, NULL);
            timeout.tv_sec = now.tv_sec + sec;
            timeout.tv_nsec = now.tv_usec * 1000;
            retval = pthread_cond_timedwait(&_cond, &_mutex, &timeout);
            if (retval == ETIMEDOUT) {
                pthread_mutex_unlock(&_mutex);
                return -1;
            }
        }
        _nready--;
        d = _queue.front();
        _queue.pop_front();
        pthread_mutex_unlock(&_mutex);
        return 0;
    }
    
    void reset_msg()
    {
        pthread_mutex_lock(&_mutex); 
        _queue.clear();
        _nready = 0;
        pthread_mutex_unlock(&_mutex);
    }
    
private:
	int 				_type;
    pthread_cond_t      _cond;
    int                 _nready;
    pthread_mutex_t     _mutex;
    deque<DataType>     _queue;
};

#endif    /* _QUEUE_H_ */

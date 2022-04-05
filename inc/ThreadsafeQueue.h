#ifndef THREADSAFEQUEUE_H
#define THREADSAFEQUEUE_H

#include <queue>
#include <pthread.h>
#include <semaphore.h>

template<typename T>
class ThreadsafeQueue
{
private:
        std::queue<T> innerQueue{};
        pthread_mutex_t queueLock;
        sem_t queueSizeSem;

public:
        ThreadsafeQueue()
        {
                pthread_mutex_init(&queueLock, nullptr);
                sem_init(&queueSizeSem, 0, 0);
        }

        void push(const T& value)
        {
                pthread_mutex_lock(&queueLock);
                innerQueue.push(value);
                pthread_mutex_unlock(&queueLock);
                sem_post(&queueSizeSem);
        }

        T pop()
        {
                sem_wait(&queueSizeSem);
                pthread_mutex_lock(&queueLock);
                T result = innerQueue.front();
                innerQueue.pop();
                pthread_mutex_unlock(&queueLock);
                return result;
        }

        ~ThreadsafeQueue()
        {
                pthread_mutex_destroy(&queueLock);
                sem_destroy(&queueSizeSem);
        }
};


#endif
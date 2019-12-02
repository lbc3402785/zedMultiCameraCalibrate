#include "zedcamerathread.h"

ZedCameraThread::ZedCameraThread(QObject *parent):QThread(parent)
{
    queue=nullptr;
}

void ZedCameraThread::run()
{
    if(queue){
        while(true){
            int data;
            queue->WaitAndPop(data);
            std::cout<<data<<std::endl<<std::flush;
        }
    }
}

ThreadSafeQueue<int> *ZedCameraThread::getQueue() const
{
    return queue;
}

void ZedCameraThread::setQueue(ThreadSafeQueue<int> *value)
{
    queue = value;
}

int ZedCameraThread::getId() const
{
    return id;
}

void ZedCameraThread::setId(int value)
{
    id = value;
}

#include "test.h"

Test::Test()
{

}

void Test::testMulCamThread()
{
    ThreadSafeQueue<int>* q=new ThreadSafeQueue<int>(5);
    ZedCameraThread* t1=new ZedCameraThread();
    t1->setId(1);
    t1->setQueue(q);
    t1->start();
    ZedCameraThread* t2=new ZedCameraThread();
    t2->setId(2);
    t2->setQueue(q);
    t2->start();
    ZedCameraThread* t3=new ZedCameraThread();
    t3->setId(3);
    t3->setQueue(q);
    t3->start();
    for (int i=0;i<10000;i++) {
        q->WaitAndPush(i);
    }
    std::cout<<"put data done!"<<std::endl<<std::flush;
}

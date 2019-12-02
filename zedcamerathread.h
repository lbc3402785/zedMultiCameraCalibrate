#ifndef ZEDCAMERATHREAD_H
#define ZEDCAMERATHREAD_H
#include <QThread>
#include <QTimer>
#include "threadsafequeue.h"
class ZedCameraThread: public QThread
{
    Q_OBJECT
public:
    ZedCameraThread(QObject *parent = 0);

    // QThread interface
    int getId() const;
    void setId(int value);

    ThreadSafeQueue<int> *getQueue() const;
    void setQueue(ThreadSafeQueue<int> *value);

protected:
    void run();
private:
    int id;
    ThreadSafeQueue<int>* queue;
};

#endif // ZEDCAMERATHREAD_H

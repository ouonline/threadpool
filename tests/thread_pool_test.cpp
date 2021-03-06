#include <iostream>
using namespace std;

#include <unistd.h>

#include "threadkit/threadpool.h"
using namespace outils;

class TestThreadTask : public JoinableThreadTask {

public:
    TestThreadTask(const string& msg) {
        m_is_finished = false;
        m_msg = msg;
    }

protected:
    shared_ptr<ThreadTask> Process() override {
        cout << "tid[" << pthread_self() << "], msg -> " << m_msg << endl;
        m_is_finished = true;
        return shared_ptr<ThreadTask>();
    }
    bool IsFinished() const override {
        return m_is_finished;
    }

private:
    bool m_is_finished;
    string m_msg;
};

static inline void EmptyDeleter(ThreadTask*) {}

int main(void) {
    ThreadPool tp;

    tp.AddThread(8);

    TestThreadTask task("Hello, world!");
    tp.AddTask(shared_ptr<ThreadTask>(&task, EmptyDeleter));

    tp.DelThread(2);
    sleep(1);
    cout << "thread num = " << tp.ThreadNum() << endl;

    tp.AddThread(5);
    sleep(1);
    cout << "thread num = " << tp.ThreadNum() << endl;

    task.Join();

    return 0;
}

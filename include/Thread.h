#pragma one

#include <functional>
#include <thread>
#include <memory>
#include <unistd.h>
#include <string>
#include <atomic>

#include "noncopyable.h"

class Thread : public noncopyable
{
public:
    using ThreadFunc = std::function<void()>;

    explicit Thread(ThreadFunc, const std::string &name = std::string());
    ~Thread();

    void start();
    void join();

    bool started() { return started_;}
    pid_t tid() const { return tid_;}
    const std::string &name() const { return name_;}


    static int numCreated() {return numCreated_;}
private:
    void setDefaultName();

    bool started_;
    bool joined_;
    pid_t tid_;
    ThreadFunc func_; // 线程回调函数
    std::string name_;
    std::shared_ptr<std::thread> thread_;
    static std::atomic_int numCreated_;
};
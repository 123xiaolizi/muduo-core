#include "CurrentThread.h"

namespace CurrentThread
{
    /*__thread是 GCC 扩展的线程局部存储（Thread-Local Storage, TLS）关键字，用于定义每个线程独有的全局变量*/
    __thread int t_cachedTid = 0;

    void cacheTid()
    {
        if (t_cachedTid == 0)
        {
            t_cachedTid = static_cast<pid_t>(::syscall(SYS_gettid));
        }
    }


}
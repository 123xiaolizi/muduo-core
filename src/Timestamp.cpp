#include "Timestamp.h"

Timestamp::Timestamp():microSecondsSinceEpoch_(0){}
// 防止隐式转换
explicit Timestamp::Timestamp(int64_t microSecondsSinceEpoch):microSecondsSinceEpoch_(microSecondsSinceEpoch){}

Timestamp Timestamp::now()
{
    return Timestamp(time(nullptr));
}

std::string Timestamp::toString() const
{
    char buf[128] = {0};
    /*time_t 本质是long int*/ 
    tm* tm_time = localtime(&microSecondsSinceEpoch_);
    /*snprintf是 C 标准库中的一个安全格式化输出函数，用于将格式化数据写入字符串缓冲区，防止缓冲区溢出*/
    snprintf(buf, 128, "%4d/%02d/%02d %02d:%02d:%02d",
            tm_time->tm_year + 1900,
            /*tm_mon表示月份，但其取值范围是 0 到 11，而不是 1 到 12*/
            tm_time->tm_mon + 1,
            tm_time->tm_mday,
            tm_time->tm_hour,
            tm_time->tm_min,
            tm_time->tm_sec);
    return buf;
}
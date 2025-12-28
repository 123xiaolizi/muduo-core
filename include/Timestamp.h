#pragma once

#include <string>
#include <time.h>

class Timestamp
{
public:
    Timestamp();
    //防止隐式转换
    explicit Timestamp(int64_t microSecondsSinceEpoch);
    static Timestamp now();
    std::string toString() const;

private:
    int64_t microSecondsSinceEpoch_;
};


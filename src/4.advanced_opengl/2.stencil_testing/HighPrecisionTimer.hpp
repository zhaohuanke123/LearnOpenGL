#ifndef HIGH_PRECISION_TIMER_HPP
#define HIGH_PRECISION_TIMER_HPP

#include <chrono>
#include <string>
#include <iostream>
#include <functional>

class HighPrecisionTimer {
private:
    std::string m_timerName;
    std::chrono::high_resolution_clock::time_point m_startTimePoint;
    bool m_stopped;

    // 回调函数，用于自定义输出方式
    std::function<void(const std::string &, long long)> m_callback;

public:
    // 构造函数，开始计时
    explicit HighPrecisionTimer(const std::string &name = "Timer",
                                std::function<void(const std::string &, long long)> callback = nullptr)
        : m_timerName(name), m_stopped(false), m_callback(callback) {
        m_startTimePoint = std::chrono::high_resolution_clock::now();
    }

    // 析构函数，如果还未停止则停止计时并输出结果
    ~HighPrecisionTimer() {
        if (!m_stopped) {
            Stop();
        }
    }

    // 手动停止计时
    void Stop() {
        auto endTimePoint = std::chrono::high_resolution_clock::now();

        // 计算时间差（纳秒）
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(endTimePoint - m_startTimePoint).count();

        m_stopped = true;

        // 如果有回调函数，调用回调函数
        if (m_callback) {
            m_callback(m_timerName, duration);
        } else {
            // 默认输出不同时间单位的结果
            std::cout << m_timerName << " 执行时间: " << std::endl;
            std::cout << "  " << duration << " 纳秒" << std::endl;
            // std::cout << "  " << duration / 1000.0 << " 微秒" << std::endl;
            // std::cout << "  " << duration / 1000000.0 << " 毫秒" << std::endl;
            // std::cout << "  " << duration / 1000000000.0 << " 秒" << std::endl;
        }
    }

    // 重置计时器
    void Reset() {
        m_stopped = false;
        m_startTimePoint = std::chrono::high_resolution_clock::now();
    }
};


// 宏定义：测量函数执行时间并输出函数名称
#define TIME_FUNCTION(func) \
    { \
        HighPrecisionTimer timer(#func); \
        func; \
    }

// 宏定义：为任意表达式计时，使用自定义名称
#define TIME_EXPR(name, expr) \
    { \
        HighPrecisionTimer timer(name); \
        expr; \
    }

// 宏定义：为可能带参数的函数调用计时
#define TIME_FUNCTION_CALL(func_call) \
    { \
        HighPrecisionTimer timer(#func_call); \
        func_call; \
    }

// 宏定义：使用行号创建唯一变量名，避免名称冲突
#define CONCAT_IMPL(a, b) a##b
#define CONCAT(a, b) CONCAT_IMPL(a, b)
#define TIME_BLOCK(name) HighPrecisionTimer CONCAT(timer_, __LINE__)(name)
#endif // HIGH_PRECISION_TIMER_HPP


/**
 *   \file     loger.h
 *   \version  0.01
 *   \date     2024.05.12
 */

#ifndef LOGER_H
#define LOGER_H

#include <QObject>
#include <chrono>
#include <iostream>


#define PROFILE_CONCAT_INTERNAL(X, Y) X##Y
#define PROFILE_CONCAT(X, Y) PROFILE_CONCAT_INTERNAL(X, Y)
#define UNIQUE_VAR_NAME_PROFILE PROFILE_CONCAT(profileGuard, __LINE__)
#define LOG_DURATION(x) LogDuration UNIQUE_VAR_NAME_PROFILE(x)


class LogDuration : public QObject
{
    Q_OBJECT
public:
    using Clock = std::chrono::steady_clock;

    LogDuration(const std::string& id) : id_(id)
    {
    }

    ~LogDuration()
    {
    }

    quint32 timeElapsed()
    {
        using namespace std::chrono;
        using namespace std::literals;

        const auto end_time = Clock::now();
        const auto dur = end_time - start_time_;

        std::cerr << id_ << ": "s << duration_cast<nanoseconds>(dur).count() << " ns"s << std::endl;

        return (duration_cast<nanoseconds>(dur).count());
    }


private:
    const std::string id_;
    const Clock::time_point start_time_ = Clock::now();
};

#endif // LOGER_H

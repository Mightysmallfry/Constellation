#ifndef CONSTELLATION_LOGGER
#define CONSTELLATION_LOGGER

// Standalone, header-only logger.
// Portable: only uses standard C++ headers available on Windows and Linux.
// Thread-safe: all writes are serialized behind a single mutex.
//
// Usage:
//   Logger::Instance().SetLevel(LogLevel::Debug);
//   Logger::Instance().SetLogFile("engine.log"); // optional
//   LOG_INFO("Simulation started with " << numCells << " cells");
//   LOG_ERROR("Failed to load mesh: " << path);

#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>

enum class LogLevel
{
    Trace = 0,
    Debug = 1,
    Info = 2,
    Warning = 3,
    Error = 4,
    Critical = 5,
    Off = 6
};

inline const char* ToString(LogLevel level)
{
    switch (level)
    {
        case LogLevel::Trace:    return "TRACE";
        case LogLevel::Debug:    return "DEBUG";
        case LogLevel::Info:     return "INFO";
        case LogLevel::Warning:  return "WARN";
        case LogLevel::Error:    return "ERROR";
        case LogLevel::Critical: return "CRITICAL";
        default:                 return "OFF";
    }
}

class Logger
{
public:
    // Singleton accessor. Construct-on-first-use avoids static init order issues.
    static Logger& Instance()
    {
        static Logger instance;
        return instance;
    }

    // Not copyable or movable — there is exactly one logger.
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    void SetLevel(LogLevel level)
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        m_MinLevel = level;
    }

    LogLevel GetLevel() const
    {
        return m_MinLevel;
    }

    // Opens (or creates) a file to additionally log to. Pass append=true to
    // keep prior contents; otherwise the file is truncated.
    bool SetLogFile(const std::string& path, bool append = true)
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        auto mode = std::ios::out | (append ? std::ios::app : std::ios::trunc);
        m_FileStream.open(path, mode);
        return m_FileStream.is_open();
    }

    void CloseLogFile()
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        if (m_FileStream.is_open())
        {
            m_FileStream.close();
        }
    }

    // Toggle whether logs also go to stdout/stderr (on by default).
    void SetConsoleOutput(bool enabled)
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        m_ConsoleEnabled = enabled;
    }

    // Core log entry point. Prefer the LOG_* macros below for call sites,
    // since they capture file/line and avoid formatting cost when a level
    // is filtered out.
    void Log(LogLevel level, const std::string& message, const char* file = nullptr, int line = 0)
    {
        if (level < m_MinLevel || m_MinLevel == LogLevel::Off)
        {
            return;
        }

        std::lock_guard<std::mutex> lock(m_Mutex);

        std::ostringstream line_stream;
        line_stream << "[" << TimestampNow() << "] "
                    << "[" << ToString(level) << "] ";

        if (file != nullptr)
        {
            line_stream << "[" << StripPath(file) << ":" << line << "] ";
        }

        line_stream << message;
        const std::string formatted = line_stream.str();

        if (m_ConsoleEnabled)
        {
            if (level >= LogLevel::Error)
            {
                std::cerr << formatted << std::endl;
            }
            else
            {
                std::cout << formatted << std::endl;
            }
        }

        if (m_FileStream.is_open())
        {
            m_FileStream << formatted << std::endl;
        }
    }

private:
    Logger() = default;

    ~Logger()
    {
        if (m_FileStream.is_open())
        {
            m_FileStream.close();
        }
    }

    static std::string TimestampNow()
    {
        using namespace std::chrono;

        const auto now = system_clock::now();
        const auto nowTimeT = system_clock::to_time_t(now);
        const auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

        std::tm tmBuf{};
#if defined(_WIN32)
        localtime_s(&tmBuf, &nowTimeT);
#else
        localtime_r(&nowTimeT, &tmBuf);
#endif
        std::ostringstream oss;
        oss << std::put_time(&tmBuf, "%Y-%m-%d %H:%M:%S")
            << '.' << std::setfill('0') << std::setw(3) << ms.count();
        return oss.str();
    }

    // Trims a full source path down to just the filename for readability.
    static std::string StripPath(const std::string& path)
    {
        const auto pos = path.find_last_of("/\\");
        return (pos == std::string::npos) ? path : path.substr(pos + 1);
    }

    LogLevel m_MinLevel = LogLevel::Info;
    bool m_ConsoleEnabled = true;
    std::ofstream m_FileStream;
    std::mutex m_Mutex;
};

// Convenience macros: build the message with a stream-style expression and
// only pay the formatting cost if the level is enabled. File/line are
// captured automatically for easier debugging.
#define LOG_TRACE(msg)    do { if (Logger::Instance().GetLevel() <= LogLevel::Trace)    { std::ostringstream _log_oss; _log_oss << msg; Logger::Instance().Log(LogLevel::Trace, _log_oss.str(), __FILE__, __LINE__); } } while (0)
#define LOG_DEBUG(msg)    do { if (Logger::Instance().GetLevel() <= LogLevel::Debug)    { std::ostringstream _log_oss; _log_oss << msg; Logger::Instance().Log(LogLevel::Debug, _log_oss.str(), __FILE__, __LINE__); } } while (0)
#define LOG_INFO(msg)     do { if (Logger::Instance().GetLevel() <= LogLevel::Info)     { std::ostringstream _log_oss; _log_oss << msg; Logger::Instance().Log(LogLevel::Info, _log_oss.str(), __FILE__, __LINE__); } } while (0)
#define LOG_WARNING(msg)  do { if (Logger::Instance().GetLevel() <= LogLevel::Warning)  { std::ostringstream _log_oss; _log_oss << msg; Logger::Instance().Log(LogLevel::Warning, _log_oss.str(), __FILE__, __LINE__); } } while (0)
#define LOG_ERROR(msg)    do { if (Logger::Instance().GetLevel() <= LogLevel::Error)    { std::ostringstream _log_oss; _log_oss << msg; Logger::Instance().Log(LogLevel::Error, _log_oss.str(), __FILE__, __LINE__); } } while (0)
#define LOG_CRITICAL(msg) do { if (Logger::Instance().GetLevel() <= LogLevel::Critical) { std::ostringstream _log_oss; _log_oss << msg; Logger::Instance().Log(LogLevel::Critical, _log_oss.str(), __FILE__, __LINE__); } } while (0)

#endif

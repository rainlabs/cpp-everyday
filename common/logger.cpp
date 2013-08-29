/**
 * common/common/src/logger.cpp
 * Class for logging
 *
 * @author Vladimir <Rain_Weezy> Zyablitskiy
 * @project common
 * @created 04.08.2013
 *
 */

#include "logger.hpp"

namespace common
{
    // @public

    Logger::Logger(std::string path_I)
    {
        mLogPath = path_I;
        mDebugFlag = false;
        mThreadFlag = true;
        setFileName("application.log");
        setTimeFormat("%Y-%m-%d %H:%M:%S");
    }

    Logger::~Logger()
    {
        mLogPath = nullptr;
    }

    void Logger::setTimeFormat(std::string format_I)
    {
        mTimeFormat = format_I;
    }

    void Logger::setFileName(std::string fileName_I)
    {
        mLogFileName = fileName_I;
    }

    void Logger::enableDebug(bool flag_I)
    {
        mDebugFlag = flag_I;
    }

    void Logger::enableWriteThread(bool flag_I)
    {
        mThreadFlag = flag_I;
    }

    void Logger::info(std::string message_I)
    {
        make_thread(Logger::info_msg, message_I);
    }

    void Logger::warning(std::string message_I)
    {
        make_thread(Logger::warning_msg, message_I);
    }

    void Logger::error(std::string message_I)
    {
        make_thread(Logger::error_msg, message_I);
    }

    void Logger::debug(std::string message_I)
    {
        if ( !mDebugFlag )
            return;
        make_thread(Logger::debug_msg, message_I);
    }

    // @protected

    std::string Logger::info_msg    = "    INFO   ";
    std::string Logger::warning_msg = "  WARNING  ";
    std::string Logger::error_msg   = "!!!ERROR!!!";
    std::string Logger::debug_msg   = "---DEBUG---";

    bool Logger::pathCheckAndCreate(std::string path_I)
    {
        if ( !pathCheck(path_I) )
            return pathCreate(path_I);
        return true;
    }

    std::string Logger::getDateTime(std::string format_I) const
    {
        time_t     now = time(0);
        struct tm  tstruct;
        char *     today = new char[30];
        tstruct = *localtime(&now);

        strftime(today, 30, format_I.c_str(), &tstruct);
        return today;
    }

    std::string Logger::getCurrentFileName() const
    {
        std::string fileName(mLogPath);
        fileName += "/" + mLogFileName;
        return fileName;
    }

    void Logger::make_thread(std::string type_I, std::string message_I)
    {
        if (mThreadFlag) {
            std::thread write_thread(&Logger::write, this, type_I, message_I);
            write_thread.detach();
        }
        else
            write(type_I, message_I);
    }

    // @private

    bool Logger::pathCheck(std::string path_I)
    {
        struct stat s;
        int status = stat(path_I.c_str(), &s); // check file exists, without check "is dir"

        if(status == 0)
            return true;

        return false;
    }

    bool Logger::pathCreate(std::string path_I)
    {
        return mkdir(path_I.c_str(), 0655) == 0;
    }

    bool Logger::write(std::string event_I, std::string message_I) const
    {
        std::ofstream file(getCurrentFileName(), std::fstream::out | std::fstream::app);
        if ( !file.is_open() ) {
            printf("couldn't open file \"%s\" in path \"%s\"!!!\n", mLogFileName.c_str(), mLogPath.c_str());
            return false;
        }

        std::string writeString(getDateTime(mTimeFormat));
        writeString += " [" + event_I + "] ";
        writeString += "| " + message_I + "\r\n";
        file << writeString;

        return true;
    }

}

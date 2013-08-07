/**
 * rainlabs/common/src/logger.cpp
 * Class for logging
 * 
 * @author Vladimir <Rain_Weezy> Zyablitskiy
 * @project rainlabs
 * @created 04.08.2013
 *
 */

#include "..\include\logger.hpp"

namespace rainlabs
{
    // @public

    Logger::Logger(const char * path_I)
    {
        mLogPath = new char[strlen(path_I)];
        strcpy(mLogPath, path_I);
        mDebugFlag = false;
        mThreadFlag = true;
        setFileName("application.log");
        setTimeFormat("%Y-%m-%d %H:%M:%S");
    };

    Logger::~Logger()
    {
        mLogPath = nullptr;
    }

    void Logger::setTimeFormat(const char * format_I)
    {
        mTimeFormat = new char[strlen(format_I)];
        strcpy(mTimeFormat, format_I);
    }

    void Logger::setFileName(const char * fileName_I)
    {
        mLogFileName = new char[strlen(fileName_I)];
        strcpy(mLogFileName, fileName_I);
    }

    void Logger::enableDebug(bool flag_I)
    {
        mDebugFlag = flag_I;
    }

    void Logger::enableWriteThread(bool flag_I)
    {
        mThreadFlag = flag_I;
    }

    void Logger::info(char * message_I)
    {
        make_thread(Logger::info_msg, message_I);
    }

    void Logger::warning(char * message_I)
    {
        make_thread(Logger::warning_msg, message_I);
    }

    void Logger::error(char * message_I)
    {
        make_thread(Logger::error_msg, message_I);
    }

    void Logger::debug(char * message_I)
    {
        if ( !mDebugFlag )
            return;
        make_thread(Logger::debug_msg, message_I);
    }

    // @protected

    const char * Logger::info_msg    = "    INFO   ";
    const char * Logger::warning_msg = "  WARNING  ";
    const char * Logger::error_msg   = "!!!ERROR!!!";
    const char * Logger::debug_msg   = "---DEBUG---";

    bool Logger::pathCheckAndCreate(char * path_I)
    {
        if ( !pathCheck(path_I) )
            return pathCreate(path_I);
        return true;
    }

    char * Logger::getDateTime(const char * format_I) const
    {
        time_t     now = time(0);
        struct tm  tstruct;
        char *     today = new char[30];
        tstruct = *localtime(&now);

        strftime(today, 30, format_I, &tstruct);
        return today;
    }

    char * Logger::getCurrentFileName() const
    {
        char * fileName = new char[strlen(mLogPath) + strlen(mLogFileName) + 1];
        strcpy(fileName, mLogPath);
        strcat(fileName, "/");
        strcat(fileName, mLogFileName);
        return fileName;
    }

    void Logger::make_thread(const char * type_I, char * message_I)
    {
        if (mThreadFlag) {
            std::thread write_thread(&Logger::write, this, type_I, message_I);
            write_thread.detach();
        }
        else
            write(type_I, message_I);
    }

    // @private

    bool Logger::pathCheck(char * path_I)
    {
        struct stat s;
        int status = stat(path_I, &s); // check file exists, without check "is dir"

        if(status == 0)
            return true;

        return false;
    }

    bool Logger::pathCreate(char * path_I)
    {
        return mkdir(path_I) == 0;
    }

    bool Logger::write(const char * event_I, char * message_I) const
    {
        std::ofstream file(getCurrentFileName(), std::fstream::out | std::fstream::app);
        if ( !file.is_open() ) {
            printf("couldn't open file \"%s\" in path \"%s\"!!!\n", mLogFileName, mLogPath);
            return false;
        }
        char * time = getDateTime((char *) mTimeFormat);

        char * out = new char[strlen(time) + strlen(event_I) + strlen(message_I) + 11];
        strcpy(out, time);
        strcat(out, " [");
        strcat(out, event_I);
        strcat(out, "] | ");
        strcat(out, message_I);
        strcat(out, "\r\n");
        file << out;

        return true;
    }

}
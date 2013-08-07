/**
 * rainlabs/common/src/daemon_logger.cpp
 * Class for daemon logging
 * 
 * @author Vladimir <Rain_Weezy> Zyablitskiy
 * @project rainlabs
 * @created 04.08.2013
 *
 */

#include "..\include\daemon_logger.hpp"

namespace rainlabs
{
    // @public

    DaemonLogger::DaemonLogger(const char * path_I) : Logger(path_I)
    {
        setTimeFormat("%H:%M:%S");
        setCurrentPathFormat("%Y-%m-%d");
        todayPathCreate();
    }

    DaemonLogger::~DaemonLogger()
    {
        mLogPath = nullptr;
    }

    void DaemonLogger::setCurrentPathFormat(const char * format_I)
    {
        mTodayPathFormat = new char[strlen(format_I)];
        strcpy(mTodayPathFormat, format_I);
    }

    // @protected

    char * DaemonLogger::getCurrentFileName() const
    {
        char * today = getDateTime((char *) mTodayPathFormat);
        char * fileName = new char[strlen(mLogPath) + strlen(today) + strlen(mLogFileName) + 4];
        strcpy(fileName, mLogPath);
        strcat(fileName, "/");
        strcat(fileName, today);
        strcat(fileName, "/");
        strcat(fileName, mLogFileName);
        return fileName;
    }

    // @private

    void DaemonLogger::todayPathCreate()
    {
        if( !pathCheckAndCreate(mLogPath) ) {
            printf("\nERROR for create directory: %s\n", mLogPath);
            return;
        }
        char * path = new char[strlen(mLogPath)+11];
        strcpy(path, mLogPath);
        strcat(path, "/");
        strcat(path, getDateTime(this->mTodayPathFormat));
        pathCheckAndCreate(path);
    }

}
/**
 * common/common/src/daemon_logger.cpp
 * Class for daemon logging
 *
 * @author Vladimir <Rain_Weezy> Zyablitskiy
 * @project common
 * @created 04.08.2013
 *
 */

#include "daemon_logger.hpp"

namespace common
{
    // @public

    DaemonLogger::DaemonLogger(std::string path_I) : Logger(path_I)
    {
        setTimeFormat("%H:%M:%S");
        setCurrentPathFormat("%Y-%m-%d");
        todayPathCreate();
    }

    DaemonLogger::~DaemonLogger()
    {
        mLogPath = nullptr;
    }

    void DaemonLogger::setCurrentPathFormat(std::string format_I)
    {
        mTodayPathFormat = format_I;
    }

    // @protected

    std::string DaemonLogger::getCurrentFileName() const
    {
        std::string fileName(mLogPath);
        fileName += "/" + getDateTime(mTodayPathFormat) + "/" + mLogFileName;
        return fileName;
    }

    // @private

    void DaemonLogger::todayPathCreate()
    {
        if( !pathCheckAndCreate(mLogPath) ) {
            printf("\nERROR for create directory: %s\n", mLogPath.c_str());
            return;
        }
        std::string path(mLogPath);
        path += "/" + getDateTime(this->mTodayPathFormat);
        pathCheckAndCreate(path);
    }

}

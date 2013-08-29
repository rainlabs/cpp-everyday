/**
 * common/common/include/daemon_logger.h
 * Class for daemon logging
 *
 * @author Vladimir <Rain_Weezy> Zyablitskiy
 * @project common
 * @created 04.08.2013
 *
 */

#ifndef _RAINLABS_DAEMON_LOGGER_H
#define _RAINLABS_DAEMON_LOGGER_H

#include <string.h>
#include <fstream>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
//#include <direct.h>
#include <thread>
#include "logger.hpp"

namespace common
{

    /**
     * @class    
     */
    class DaemonLogger : public Logger
    {
    public:
        /**
         * @param std::string
         */
        DaemonLogger(std::string);
        ~DaemonLogger(void);

        /**
         *       
         */
        void setCurrentPathFormat(std::string);
    protected:
        std::string mTodayPathFormat;  //  ,

        /**
         * @protected
         *     (     ,  )
         * @return std::string
         */
        std::string getCurrentFileName() const;
    private:
        /**
         * @private
         *     
         */
        void todayPathCreate();
    };

}
#endif /* _RAINLABS_DAEMON_LOGGER_H */

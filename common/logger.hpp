/**
 * common/common/include/logger.h
 * Class for logging
 *
 * @author Vladimir <Rain_Weezy> Zyablitskiy
 * @project common
 * @created 04.08.2013
 *
 */

#ifndef _RAINLABS_LOGGER_H
#define _RAINLABS_LOGGER_H

#include <string.h>
#include <fstream>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
//#include <direct.h>
#include <thread>

namespace common
{

    /**
     * @class application's message log
     */
    class Logger
    {
    public:
        /**
         * @param std::string path
         */
        Logger(std::string);
        ~Logger(void);

        /**
         * set log message format
         */
        void setTimeFormat(std::string);

        /**
         * set file name for logs
         */
        void setFileName(std::string);

        /**
         * enable/disable debug logs
         * default: false
         */
        void enableDebug(bool);

        /**
         * enable/disable run write method in new thread
         * default: true
         */
        void enableWriteThread(bool);

        /**
         * log level: info
         * @param std::string message
         */
        void info(std::string);
        /**
         * log level: warning
         * @param std::string message
         */
        void warning(std::string);
        /**
         * log level: error
         * @param std::string message
         */
        void error(std::string);
        /**
         * log level: debug
         * @param std::string message
         */
        void debug(std::string);
    protected:
        std::string mLogPath;                        // log path;                            default: N/A
        bool mDebugFlag;                        // true - write debug messages;         default: false
        bool mThreadFlag;                       // true - run "write" in new thread;    default: true
        std::string mTimeFormat;                     // time format for log message;         default: "%Y-%m-%d %H:%M:%S"
        std::string mLogFileName;                    // file name for logs;                  default: "application.log"

        static std::string info_msg;           // message type: info
        static std::string warning_msg;        // message type: warning
        static std::string error_msg;          // message type: error
        static std::string debug_msg;          // message type: debug

        /**
         * @protected
         * check and create path if needed
         * @param std::string path
         * @return bool true - success, false - fail
         */
        bool pathCheckAndCreate(std::string);
        /**
         * @protected
         * get formated datetime string
         * @param std::string format
         * @return std::string formated current datetime
         */
        std::string getDateTime(std::string) const;
        /**
         * @protected
         * get relative current file name
         * @return std::string relative current file name
         */
        virtual std::string getCurrentFileName() const;

        /**
         * create thread for write message
         * @param char * message type
         * @param char * message
         */
        void make_thread(std::string, std::string);
    private:
        /**
         * @private
         * check path exists
         * @param std::string path
         * @return bool true - exists, false - not exists
         */
        bool pathCheck(std::string);
        /**
         * @private
         * create a path
         * @param char * path
         * @return bool true - success, false - fail
         */
        bool pathCreate(std::string);
        /**
         * @private
         * write message into file
         * @param char * message type
         * @param char * message
         * @return bool true - success, false - fail
         */
        bool write(std::string, std::string) const;
    };

}
#endif /* _RAINLABS_LOGGER_H */

/**
 * rainlabs/common/include/logger.h
 * Class for logging
 * 
 * @author Vladimir <Rain_Weezy> Zyablitskiy
 * @project rainlabs
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
#include <direct.h>
#include <thread>

namespace rainlabs
{

    /**
     * @class application's message log
     */
    class Logger
    {
    public:
        /**
         * @param char * path
         */
        Logger(const char *);
        ~Logger(void);

        /**
         * set log message format
         */
        void setTimeFormat(const char *);

        /**
         * set file name for logs
         */
        void setFileName(const char *);

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
         * @param char * message
         */
        void info(char *);
        /**
         * log level: warning
         * @param char * message
         */
        void warning(char *);
        /**
         * log level: error
         * @param char * message
         */
        void error(char *);
        /**
         * log level: debug
         * @param char * message
         */
        void debug(char *);
    protected:
        char * mLogPath;                        // log path;                            default: N/A
        bool mDebugFlag;                        // true - write debug messages;         default: false
        bool mThreadFlag;                       // true - run "write" in new thread;    default: true
        char * mTimeFormat;                     // time format for log message;         default: "%Y-%m-%d %H:%M:%S"
        char * mLogFileName;                    // file name for logs;                  default: "application.log"

        static const char * info_msg;           // message type: info
        static const char * warning_msg;        // message type: warning
        static const char * error_msg;          // message type: error
        static const char * debug_msg;          // message type: debug

        /**
         * @protected
         * check and create path if needed
         * @param char * path
         * @return bool true - success, false - fail
         */
        bool pathCheckAndCreate(char *);
        /**
         * @protected
         * get formated datetime string
         * @param char * format
         * @return char * formated current datetime
         */
        char * getDateTime(const char *) const;
        /**
         * @protected
         * get relative current file name
         * @return char * relative current file name
         */
        virtual char * getCurrentFileName() const;

        /**
         * create thread for write message
         * @param char * message type
         * @param char * message
         */
        void make_thread(const char *, char *);
    private:
        /**
         * @private
         * check path exists
         * @param char * path
         * @return bool true - exists, false - not exists
         */
        bool pathCheck(char *);
        /**
         * @private
         * create a path
         * @param char * path
         * @return bool true - success, false - fail
         */
        bool pathCreate(char *);
        /**
         * @private
         * write message into file
         * @param char * message type
         * @param char * message
         * @return bool true - success, false - fail
         */
        bool write(const char *, char *) const;
    };

}
#endif /* _RAINLABS_LOGGER_H */
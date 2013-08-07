/**
 * rainlabs/common/include/daemon_logger.h
 * Class for daemon logging
 * 
 * @author Vladimir <Rain_Weezy> Zyablitskiy
 * @project rainlabs
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
#include <direct.h>
#include <thread>
#include "logger.hpp"

namespace rainlabs
{

    /**
     * @class Логер сообщений для демона
     */
    class DaemonLogger : public Logger
    {
    public:
        /**
         * @param char * путь до папки
         */
        DaemonLogger(const char *);
        ~DaemonLogger(void);

        /**
         * установить формат имени папки для текущего дня
         */
        void setCurrentPathFormat(const char *);
    protected:
        char * mTodayPathFormat;  // формат даты, для именования папки

        /**
         * @protected
         * Получить текущее имя файла (так как оно зависит от времени, собирается динамически)
         * @return char * текущее имя файла
         */
        char * getCurrentFileName() const;
    private:
        /**
         * @private
         * Создать папку с текущим днем
         */
        void todayPathCreate();
    };

}
#endif /* _RAINLABS_DAEMON_LOGGER_H */
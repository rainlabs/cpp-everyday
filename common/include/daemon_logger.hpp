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
     * @class ����� ��������� ��� ������
     */
    class DaemonLogger : public Logger
    {
    public:
        /**
         * @param char * ���� �� �����
         */
        DaemonLogger(const char *);
        ~DaemonLogger(void);

        /**
         * ���������� ������ ����� ����� ��� �������� ���
         */
        void setCurrentPathFormat(const char *);
    protected:
        char * mTodayPathFormat;  // ������ ����, ��� ���������� �����

        /**
         * @protected
         * �������� ������� ��� ����� (��� ��� ��� ������� �� �������, ���������� �����������)
         * @return char * ������� ��� �����
         */
        char * getCurrentFileName() const;
    private:
        /**
         * @private
         * ������� ����� � ������� ����
         */
        void todayPathCreate();
    };

}
#endif /* _RAINLABS_DAEMON_LOGGER_H */
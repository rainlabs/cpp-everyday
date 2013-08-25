/**
 * Class for active record connection to postgres database
 * @file include/postgre_driver.hpp
 * @author Vladimir <Rain_Weezy> Zyablitskiy
 * @date 24/08/2013
 */

#ifndef _RAINLABS_ACTIVE_RECORD_POSTGRE_DRIVER_H
#define _RAINLABS_ACTIVE_RECORD_POSTGRE_DRIVER_H

#include <iostream>
#include <string>
#include <map>
#include <pqxx/pqxx>
#include "driver.hpp"

namespace ActiveRecord
{
    class PostgreDriver : public Driver
    {
    public:
        PostgreDriver(const char *, const char *, const char *, const char *);
        ~PostgreDriver();

        std::map<std::string, std::string> getColumns(std::string);

        std::map<std::string, std::string> get(std::string, std::string);
        bool update(std::string, std::string, std::map<std::string, std::string>);
        bool destroy(std::string, std::string);
        bool insert(std::string, std::map<std::string, std::string>);
        void commit();

        bool connect();
        void disconnect();

    protected:
        pqxx::work * mWorker; // transaction

        bool tryQuery(std::string);

    };
}

#endif /* _RAINLABS_ACTIVE_RECORD_POSTGRE_DRIVER_H */

/**
 * Class for active record connection to postgres database
 * @file include/postgre_adapter.hpp
 * @author Vladimir <Rain_Weezy> Zyablitskiy
 * @date 24/08/2013
 */

#ifndef _RAINLABS_ACTIVE_RECORD_POSTGRE_DRIVER_H
#define _RAINLABS_ACTIVE_RECORD_POSTGRE_DRIVER_H

#include <iostream>
#include <string>
#include <map>
#include <pqxx/pqxx>
#include "adapter.hpp"

namespace ActiveRecord
{
    class PostgreAdapter : public Adapter
    {
    public:
        PostgreAdapter(const char *, const char *, const char *, const char *);
        ~PostgreAdapter();

        std::map<std::string, std::string> getColumns(std::string);
        std::string quote(std::string);

        std::vector< std::map<std::string, std::string> > get(std::string, std::map<std::string, std::string>);
        bool update(std::string, std::map<std::string, std::string>, std::map<std::string, std::string>);
        bool destroy(std::string, std::map<std::string, std::string>);
        bool insert(std::string, std::map<std::string, std::string>);
        void commit();
        void rollback();

        bool connect();
        void disconnect();

    protected:
        pqxx::work * mWorker; // transaction

        bool tryQuery(std::string);

        // TODO
        std::string makeWhere(std::map<std::string, std::string>);
        std::vector< std::map<std::string, std::string> > parseResponse(pqxx::result);

    };
}

#endif /* _RAINLABS_ACTIVE_RECORD_POSTGRE_DRIVER_H */

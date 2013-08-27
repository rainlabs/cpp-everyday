/**
 * Class for active record connection to database
 * @file include/adapter.hpp
 * @author Vladimir <Rain_Weezy> Zyablitskiy
 * @date 24/08/2013
 */

#ifndef _RAINLABS_ACTIVE_RECORD_DRIVER_H
#define _RAINLABS_ACTIVE_RECORD_DRIVER_H

#include <string>
#include <map>
#include "exception.hpp"
#include "../common.hpp"

namespace ActiveRecord
{
    class Adapter
    {
    public:
        Adapter(const char *, const char *, const char *, const char *);
        //Adapter(const Adapter&);
        virtual ~Adapter();

        /**
        * Set primary key
        */
        void setPrimaryKey (const char *);

        /**
        * Set logger
        */
        void setLogger (common::Logger *);

        /**
        * Get logger
        */
        common::Logger * getLogger ();

        /**
        * Get column info for table
        */
        virtual std::map<std::string, std::string> getColumns(std::string) = 0;

        /**
        * Get Limited records where
        */
        virtual std::map<std::string, std::string> get(std::string, std::map<std::string, std::string>) = 0;
        /**
        * Update records where
        */
        virtual bool update(std::string, std::map<std::string, std::string>, std::map<std::string, std::string>) = 0;
        /**
        * Delete records where
        */
        virtual bool destroy(std::string, std::map<std::string, std::string>) = 0;
        /**
        * Insert new record
        */
        virtual bool insert(std::string, std::map<std::string, std::string>) = 0;
        /**
        * Commit changes
        */
        virtual void commit() = 0;
        /**
          * Revert last changes
          */
        virtual void rollback() = 0;

        virtual bool connect() = 0;
        virtual void disconnect() = 0;
    protected:
        void* mConnect; // connector for database, need static_cast before use

        std::string mHost;
        std::string mDatabase;
        std::string mUsername;
        std::string mPassword;
        std::string mPrimaryKey;

        common::Logger* mLogger;

        void clear();
    };
};

#endif /* _RAINLABS_ACTIVE_RECORD_DRIVER_H */

/**
 * Class for active record connection to database
 * @file include/driver.hpp
 * @author Vladimir <Rain_Weezy> Zyablitskiy
 * @date 24/08/2013
 */

#ifndef _RAINLABS_ACTIVE_RECORD_DRIVER_H
#define _RAINLABS_ACTIVE_RECORD_DRIVER_H

#include <string>
#include <map>
#include "exception.hpp"

namespace ActiveRecord
{
    class Driver
    {
    public:
        Driver(const char *, const char *, const char *, const char *);
        //Driver(const Driver&);
        virtual ~Driver();

        /**
        * Set primary key
        */
        void setPrimaryKey (const char *);

        /**
        * Get column info for table
        */
        virtual std::map<std::string, std::string> getColumns(std::string) = 0;

        /**
        * Get 1 record by primary key
        */
        virtual std::map<std::string, std::string> get(std::string, std::string) = 0;
        /**
        * Update 1 record by primary key
        */
        virtual bool update(std::string, std::string, std::map<std::string, std::string>) = 0;
        /**
        * Delete 1 record by primary key
        */
        virtual bool destroy(std::string, std::string) = 0;
        /**
        * Insert new record
        */
        virtual bool insert(std::string, std::map<std::string, std::string>) = 0;
        /**
        * Commit changes
        */
        virtual void commit() = 0;

        virtual bool connect() = 0;
        virtual void disconnect() = 0;
    protected:
        void* mConnect; // connector for database, need static_cast before use

        std::string mHost;
        std::string mDatabase;
        std::string mUsername;
        std::string mPassword;
        std::string mPrimaryKey;

        void clear();
    };
};

#endif /* _RAINLABS_ACTIVE_RECORD_DRIVER_H */

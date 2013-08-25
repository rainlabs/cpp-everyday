/**
 * Class for active record connection to postgres database
 * @file src/postgre_driver.cpp
 * @author Vladimir <Rain_Weezy> Zyablitskiy
 * @date 24/08/2013
 */

#include "../include/postgre_driver.hpp"

namespace ActiveRecord
{
    PostgreDriver::PostgreDriver(const char * host_I, const char * database_I, const char * username_I, const char * password_I) : Driver(host_I, database_I, username_I, password_I)
    {
        mWorker = nullptr;
    };

    PostgreDriver::~PostgreDriver()
    {
        if (mConnect != nullptr) {
            disconnect();
            mConnect = nullptr;
        }

        if (mWorker != nullptr) {
            delete mWorker;
            mWorker = nullptr;
        }
        clear();
    }

    bool PostgreDriver::connect()
    {
        // Prepare connection string
        std::ostringstream conn("");
        conn
            << "host=" << mHost
            << " user=" << mUsername
            << " password=" << mPassword
            << " dbname=" << mDatabase;

        mConnect = new pqxx::connection(conn.str());
        mWorker = new pqxx::work(*static_cast<pqxx::connection*>(mConnect));

        if (mConnect != nullptr)
            return true;

        return false;
    };

    void PostgreDriver::disconnect()
    {
        static_cast<pqxx::connection*>(mConnect)->disconnect();
    };

    void PostgreDriver::commit()
    {
        mWorker->commit();
    }

    //SELECT column_name FROM information_schema.columns WHERE table_name = table_I
    std::map<std::string, std::string> PostgreDriver::getColumns(std::string table_I)
    {
        std::map<std::string, std::string> result;
        pqxx::result res;

        std::ostringstream query("");
        query
            << "SELECT column_name FROM information_schema.columns WHERE table_name = "
            << mWorker->quote(table_I);

        try {
            res = mWorker->exec(query.str());
        } catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
            return result;
        }

        if (res.size() > 0) {
            for(auto it = res.begin(); it != res.end(); it++) {
                //std::cout << (*it)[0].c_str() << std::endl;
                result[(*it)[0].c_str()] = "";
            }
        }
        return result;
    }

    // SELECT * FROM table_I WHERE id = primary_I LIMIT 1
    std::map<std::string, std::string> PostgreDriver::get(std::string table_I, std::string primary_I)
    {
        std::map<std::string, std::string> result;
        pqxx::result res;

        std::ostringstream query("");
        query
            << "SELECT * FROM "
            << table_I
            << " WHERE "
            << mPrimaryKey
            << " = "
            << mWorker->quote(primary_I)
            << " LIMIT 1";

        try {
            res = mWorker->exec(query.str());
        } catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
            return result;
        }

        if (res.size() == 1) {
            for(auto it = res[0].begin(); it != res[0].end(); it++) {
                //std::cout << it->name() << " = '" << it->c_str() << "'" << std::endl;
                result[it->name()] = it->c_str();
            }
        }
        // TODO: log

        return result;
    };

    bool PostgreDriver::update(std::string table_I, std::string primary_I, std::map<std::string, std::string> attributes_I)
    {
        std::ostringstream query("");
        query
            << "UPDATE "
            << table_I
            << " SET ";

        for (auto& it: attributes_I) {
            if (it != *attributes_I.begin())
                query << ", ";
            query
                << it.first
                << " = "
                << mWorker->quote(it.second);
        }

        query
            << " WHERE "
            << mPrimaryKey
            << " = "
            << mWorker->quote(primary_I);

        return tryQuery(query.str());
    }

    // DELETE FROM table_I WHERE id = primary_I
    bool PostgreDriver::destroy(std::string table_I, std::string primary_I)
    {
        std::ostringstream query("");
        query
            << "DELETE FROM "
            << table_I
            << " WHERE "
            << mPrimaryKey
            << " = "
            << mWorker->quote(primary_I);

        return tryQuery(query.str());
    }

    bool PostgreDriver::insert(std::string table_I, std::map<std::string, std::string> attributes_I)
    {
        std::ostringstream query("");
        query
            << "INSERT INTO "
            << table_I
            << " (";

        for (auto& it: attributes_I) {
            if(it != *attributes_I.begin())
                query << ", ";
            query << it.first;
        }
        query << ") VALUES (";
        for (auto& it: attributes_I) {
            if(it != *attributes_I.begin())
                query << ", ";
            query << mWorker->quote(it.second);
        }
        query << ")";

        std::string tmp = query.str();

        return tryQuery(query.str());
    }

    bool PostgreDriver::tryQuery(std::string query_I)
    {
        std::cout << query_I << std::endl;
        try {
            mWorker->exec(query_I);
        } catch (const std::exception& e) {
            //std::cout << e.what() << std::endl;
            // TODO: log
            return false;
        }

        return true;
    }
}

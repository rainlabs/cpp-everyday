/**
 * Class for active record connection to postgres database
 * @file src/postgre_adapter.cpp
 * @author Vladimir <Rain_Weezy> Zyablitskiy
 * @date 24/08/2013
 */

#include "postgre_adapter.hpp"

namespace ActiveRecord
{
    PostgreAdapter::PostgreAdapter(const char * host_I, const char * database_I, const char * username_I, const char * password_I) : Adapter(host_I, database_I, username_I, password_I)
    {
        mWorker = nullptr;
    }

    PostgreAdapter::~PostgreAdapter()
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

    bool PostgreAdapter::connect()
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
    }

    void PostgreAdapter::disconnect()
    {
        static_cast<pqxx::connection*>(mConnect)->disconnect();
    }

    void PostgreAdapter::commit()
    {
        mWorker->commit();
    }

    void PostgreAdapter::rollback()
    {
        mWorker->abort();
    }

    //SELECT column_name FROM information_schema.columns WHERE table_name = table_I
    std::map<std::string, std::string> PostgreAdapter::getColumns(std::string table_I)
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
            mLogger->error(const_cast<char*>(e.what()));
            return result;
        }

        if (res.size() > 0) {
            for(auto it = res.begin(); it != res.end(); it++) {
                std::cout << (*it)[0].c_str() << std::endl;
                result[(*it)[0].c_str()] = "";
            }
        }
        return result;
    }

    // SELECT * FROM table_I WHERE id = primary_I LIMIT 1
    std::map<std::string, std::string> PostgreAdapter::get(std::string table_I, std::map<std::string, std::string> where_I)
    {
        std::map<std::string, std::string> result;
        pqxx::result res;

        std::ostringstream query("");
        query
            << "SELECT * FROM "
            << table_I
            << " WHERE";

        for(auto& it: where_I) {
            if (it != *where_I.begin())
                query << " AND";
            query << " " << it.first << " = " << mWorker->quote(it.second);
        }

        try {
            res = mWorker->exec(query.str());
        } catch (const std::exception& e) {
            mLogger->error(const_cast<char*>(e.what()));
            return result;
        }

        if (res.size() > 0) {
            for(auto it: res.begin()) {
                std::cout << it.name() << " = '" << it.c_str() << "'" << std::endl;
                result[it.name()] = it.c_str();
            }
        }
        else {
            mLogger->info("Record not found!");
        }
        mLogger->debug( (char*)query.str().c_str() );

        return result;
    }

    bool PostgreAdapter::update(std::string table_I, std::map<std::string, std::string> attributes_I, std::map<std::string, std::string> where_I)
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

        query << " WHERE";

        for(auto& it: where_I) {
            if (it != *where_I.begin())
                query << " AND";
            query << " " << it.first << " = " << mWorker->quote(it.second);
        }

        return tryQuery(query.str());
    }

    // DELETE FROM table_I WHERE id = primary_I
    bool PostgreAdapter::destroy(std::string table_I, std::map<std::string, std::string> where_I)
    {
        std::ostringstream query("");
        query
            << "DELETE FROM "
            << table_I
            << " WHERE";

        for(auto& it: where_I) {
            if (it != *where_I.begin())
                query << " AND";
            query << " " << it.first << " = " << mWorker->quote(it.second);
        }

        return tryQuery(query.str());
    }

    bool PostgreAdapter::insert(std::string table_I, std::map<std::string, std::string> attributes_I)
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

    bool PostgreAdapter::tryQuery(std::string query_I)
    {
        std::cout << query_I << std::endl;
        try {
            pqxx::result res = mWorker->exec(query_I);
            mLogger->debug( (char*)query_I.c_str() );
        } catch (const std::exception& e) {
            mLogger->error(const_cast<char*>(e.what()));
            return false;
        }

        return true;
    }
}

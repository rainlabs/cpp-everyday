/**
 * Class for active record connection to database
 * @file src/adapter.cpp
 * @author Vladimir <Rain_Weezy> Zyablitskiy
 * @date 24/08/2013
 */

#include "adapter.hpp"

namespace ActiveRecord
{
    Adapter::Adapter(const char * host_I, const char * database_I, const char * username_I, const char * password_I)
    {
        mHost = host_I;
        mDatabase = database_I;
        mUsername = username_I;
        mPassword = password_I;
        mPrimaryKey = "id";

        mConnect = nullptr;
    }

    Adapter::~Adapter()
    {
        clear();
    }

    void Adapter::setPrimaryKey (const char * key_I)
    {
        mPrimaryKey = key_I;
    }

    void Adapter::setLogger (common::Logger * logger_I)
    {
        mLogger = logger_I;
    }

    common::Logger * Adapter::getLogger ()
    {
        return mLogger;
    }

    void Adapter::clear()
    {
        mHost.clear();
        mDatabase.clear();
        mUsername.clear();
        mPassword.clear();
        mPrimaryKey.clear();
    }
}

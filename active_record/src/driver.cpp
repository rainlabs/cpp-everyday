/**
 * Class for active record connection to database
 * @file src/driver.cpp
 * @author Vladimir <Rain_Weezy> Zyablitskiy
 * @date 24/08/2013
 */

#include "../include/driver.hpp"

namespace ActiveRecord
{
    Driver::Driver(const char * host_I, const char * database_I, const char * username_I, const char * password_I)
    {
        mHost = host_I;
        mDatabase = database_I;
        mUsername = username_I;
        mPassword = password_I;
        mPrimaryKey = "id";

        mConnect = nullptr;
    };

    Driver::~Driver()
    {
        clear();
    };

    void Driver::setPrimaryKey (const char * key_I)
    {
        mPrimaryKey = key_I;
    };

    void Driver::clear()
    {
        mHost.clear();
        mDatabase.clear();
        mUsername.clear();
        mPassword.clear();
        mPrimaryKey.clear();
    }
}

#ifndef _RAINLABS_ACTIVE_RECORD_EXCEPTION_H
#define _RAINLABS_ACTIVE_RECORD_EXCEPTION_H

#include <exception>
#include <string>

namespace ActiveRecord
{
    class Exception : public std::exception
    {
    public:
        Exception(std::string message_I) : mMessage(message_I) {}
        ~Exception() throw () {} // Updated
        const char* what() const throw() { return mMessage.c_str(); }
    protected:
        std::string mMessage;
    };

    class DriverNotDefined : public Exception
    {
    public:
        DriverNotDefined() : Exception("Driver not found: define driver first, before use model") {};
    };

    class TableNameNotDefined : public Exception
    {
    public:
        TableNameNotDefined() : Exception("Define database table name first, before use model") {};
    };

    class AttributeNotFound : public Exception
    {
    public:
        AttributeNotFound(std::string attribute_I) : Exception( "Attribute \"" + attribute_I + "\" not found in this model" ) {};
    };

    class RecordNotFound : public Exception
    {
    public:
        RecordNotFound(std::string primary_I) : Exception( "Record with primary key \"" + primary_I + "\" not found in this table" ) {};
    };

    class InvalidInsertData : public Exception
    {
    public:
        InvalidInsertData() : Exception( "Inserted data is invalid" ) {};
    };

}

#endif /* _RAINLABS_ACTIVE_RECORD_EXCEPTION_H */

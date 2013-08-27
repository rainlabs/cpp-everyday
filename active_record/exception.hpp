#ifndef _RAINLABS_ACTIVE_RECORD_EXCEPTION_H
#define _RAINLABS_ACTIVE_RECORD_EXCEPTION_H

#include <exception>
#include <string>
#include <map>

namespace ActiveRecord
{
    namespace Exception
    {
        class Base : public std::exception
        {
        public:
            Base(std::string message_I) : mMessage(message_I) {}
            ~Base() throw () {} // Updated
            const char* what() const throw() { return mMessage.c_str(); }
        protected:
            std::string mMessage;
        };

        class AdapterNotDefined : public Base
        {
        public:
            AdapterNotDefined() : Base("Adapter not found: define adapter first, before use model") {};
        };

        class TableNameNotDefined : public Base
        {
        public:
            TableNameNotDefined() : Base("Define database table name first, before use model") {};
        };

        class AttributeNotFound : public Base
        {
        public:
            AttributeNotFound(std::string table_I, std::string attribute_I) : Base( "Attribute \"" + attribute_I + "\" not found in table \"" + table_I + "\"" ) {};
        };

        class RecordNotFound : public Base
        {
        public:
            RecordNotFound(std::string table_I,  std::string primary_I) : Base( "Record with primary key \"" + primary_I + "\" not found in table \"" + table_I + "\"" ) {};
        };

        class InvalidInsertData : public Base
        {
        public:
            InvalidInsertData(std::string table_I, std::map<std::string, std::string> attributes_I) : Base("")
            {
                mMessage ="Invlid insert data:\n{\n";

                for(auto& it: attributes_I) {
                    if (it != *attributes_I.begin())
                        mMessage += ",\n";
                    mMessage += it.first + " : " + it.second;
                }
                mMessage += "\n}\n in table:\"" + table_I + "\"";
            }
        };
    }
}

#endif /* _RAINLABS_ACTIVE_RECORD_EXCEPTION_H */

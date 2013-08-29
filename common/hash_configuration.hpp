/**
 * common/common/include/hash_configuration.h
 * Template of configuration for hash values
 *
 * @author Vladimir <Rain_Weezy> Zyablitskiy
 * @project common
 * @created 04.08.2013
 *
 */

#ifndef _RAINLABS_HASH_CONFIGURATION_H
#define _RAINLABS_HASH_CONFIGURATION_H

#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include "configuration.hpp"

namespace common
{
    /**
     * @param T hash value type or std::string by default
     * @param func converter from char* to T or nullptr by default
     */
    class HashConfiguration : public Configuration< std::string >
    {
    public:
        HashConfiguration(std::string file_I) : Configuration< std::string >(file_I) {};
        HashConfiguration(std::string file_I, char * logPath_I) : Configuration< std::string >(file_I, logPath_I) {};
        HashConfiguration(std::string file_I, Logger * logger_I) : Configuration< std::string >(file_I, logger_I) {};

        /**
         * Get hash value at key
         * @param char * key
         * @return value_type value
         */
        std::string get(std::string key_I)
        {
            return get<std::string>(key_I);
        }

        template< typename T >
        T get(std::string key_I)
        {
            T result;

            if (mHashValues.count(key_I) == 0) {
                mLog->error( logMessage(("record not found with key: " + std::string(key_I)).c_str()) );
                return result;
            }

            std::istringstream temp(mHashValues[key_I]);
            temp >> result;

            return result;
        }

        /**
         * ALTERNATIVE: Get hash value at key
         * @param char * key
         * @return value_type value
         */
        std::string operator[](std::string key_I)
        {
            return get(key_I);
        }

        /**
         * @Override
         * Clear values, set default delimeter and read file
         */
        void load(void)
        {
            mDelimeter = "=";
            if (mHashValues.size() > 0)
                mHashValues.clear();
            Configuration::load();
        }

        /**
         * Clear values, set delimeter and read file
         */
        void load(std::string delimeter_I)
        {
            mDelimeter = delimeter_I;
            if (mHashValues.size() > 0)
                mHashValues.clear();
            Configuration::load();
        }
    protected:

        /**
          * @protected
          * strip string by delimeter
          */
        void strip(std::string line_I)
        {
            std::string key;
            std::string val;
            size_t pos = 0;

            pos = line_I.find(mDelimeter);
            key = line_I.substr(0, pos);
            val = line_I.substr(pos + mDelimeter.length());

            key.erase( std::remove_if(key.begin(), key.end(), ::isspace), key.end() );
            val.erase(std::remove_if(val.begin(), val.end(), ::isspace), val.end());

            mHashValues[key] = val;
        }

        /**
         * @Override
         * parse string line and fill hash
         */
        std::string convertFromString(char * line_I)
        {
            strip(line_I);
            return line_I;
        }

    private:
        std::map<std::string, std::string> mHashValues; // Hash for values
        std::string mDelimeter;               // Regex for strip pair key:value
    };

}
#endif /* _RAINLABS_HASH_CONFIGURATION_H */

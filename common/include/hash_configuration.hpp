/**
 * rainlabs/common/include/hash_configuration.h
 * Template of configuration for hash values
 * 
 * @author Vladimir <Rain_Weezy> Zyablitskiy
 * @project rainlabs
 * @created 04.08.2013
 *
 */

#ifndef _RAINLABS_ASSOC_CONFIGURATION_H
#define _RAINLABS_ASSOC_CONFIGURATION_H

#include <string>
#include <map>
#include <regex>
#include "configuration.hpp"

namespace rainlabs
{
    /**
     * @param T hash value type or std::string by default
     * @param func converter from char* to T or nullptr by default
     */
    template< typename T = std::string, T (* func)(const char *) = nullptr >
    class HashConfiguration : public Configuration< char * >
    {
    public:
        HashConfiguration(char * file_I) : Configuration< char * >(file_I) {};
        HashConfiguration(char * file_I, char * logPath_I) : Configuration< char * >(file_I, logPath_I) {};
        HashConfiguration(char * file_I, Logger * logger_I) : Configuration< char * >(file_I, logger_I) {};

        /**
         * Get hash value at key
         * @param char * key
         * @return value_type value
         */
        T get(char * key_I)
        {
            auto iFind = mHashValues.find( key_I );
            if (iFind == mHashValues.end()) {
                T result; // create and return fake struct
                mLog->error( logMessage(("record not found with key: " + std::string(key_I)).c_str()) );
                return result;
            }
            return iFind->second;
        }

        /**
         * ALTERNATIVE: Get hash value at key
         * @param char * key
         * @return value_type value
         */
        T operator[](char * key_I)
        {
            return get(key_I);
        }

        /**
         * @Override
         * Clear values, set default regex and read file
         */
        void load(void)
        {
            regexStrip.assign(getDefaultRegex(), std::regex::ECMAScript); 
            if (mHashValues.size() > 0)
                mHashValues.clear();
            Configuration::load();
        }

        /**
         * Clear values, set regex and read file
         */
        void load(char * regex_I)
        {
            regexStrip.assign(regex_I, std::regex::ECMAScript); 
            if (mHashValues.size() > 0)
                mHashValues.clear();
            Configuration::load();
        }
    protected:
        /**
         * @protected
         * convert from string to needed value type
         * if "func" is not defined, be sure to "T" have constructor with char * value 
         * @param char * value from file
         * @return value_type value in needed type
         */
        T convertToParameter(const char * value_I) const
        {
            if(func != nullptr)
                return func(value_I);
            return T(value_I); // call T-constructor to get correct T-object
        }
        /**
         * @Override
         * parse string line and fill hash
         */
        char * convertFromString(char * value_I)
        {
            std::smatch matcher;
            std::string stdVal(value_I);

            // find key
            std::regex_search(stdVal, matcher, regexStrip);
            std::string key = matcher.str();
            stdVal = matcher.suffix().str();

            // find value
            std::regex_search(stdVal, matcher, regexStrip);
            T val = convertToParameter(matcher.str().c_str());
            mHashValues[key] = val;
        
            return value_I;
        }

        /**
         * Default regex for strip line in file
         * strip white spaces, tabs and '=', not ideal, use only one '=' at line :) 
         */
        const char * getDefaultRegex()
        {
            return "[^ =\t]+";
        }
    private:
        std::map<std::string, T> mHashValues; // Hash for values
        std::regex regexStrip;                // Regex for strip pair key:value
    };

}
#endif /* _RAINLABS_ASSOC_CONFIGURATION_H */
/**
 * common/common/include/configuration.h
 * Simple template for configuration's files
 *
 * @author Vladimir <Rain_Weezy> Zyablitskiy
 * @project common
 * @created 04.08.2013
 *
 */

#ifndef _RAINLABS_CONFIGURATION_H
#define _RAINLABS_CONFIGURATION_H

#include <string.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "logger.hpp"

namespace common
{
    template< typename T >
    class Configuration
    {
    public:
        /**
         * @param std::string config file
         */
        Configuration(std::string configFile_I)
        {
            initialize(configFile_I, "config_log", getMaxConfigurationLineLength());
        }
        /**
         * @param std::string config file
         * @param std::string path for logs
         */
        Configuration(std::string configFile_I, std::string logPath_I)
        {
            initialize(configFile_I, logPath_I, getMaxConfigurationLineLength());
        }
        /**
         * @param std::string config file
         * @param Logger
         */
        Configuration(std::string configFile_I, Logger * logger_I)
        {
            initialize(configFile_I, logger_I, getMaxConfigurationLineLength());
        }
        /**
         * @param std::string config file
         * @param std::string path for logs
         * @param unsigned short max string length of file line
         */
        Configuration(std::string configFile_I, std::string logPath_I, unsigned short length_I)
        {
            initialize(configFile_I, logPath_I, length_I);
        }
        /**
         * @param std::string config file
         * @param Logger
         * @param unsigned short max string length of file line
         */
        Configuration(std::string configFile_I, Logger * logger_I, unsigned short length_I)
        {
            initialize(configFile_I, logger_I, length_I);
        }
        virtual ~Configuration(void)
        {
            mConfigFile = nullptr;
        }

        /**
         * Get configuration element
         * @return T structured line from config file
         */
        T operator[](int index) const
        {
            if (mValues.size() == 0)
                return (T) -1;

            int currentIndex = index;
            if (index >= mValues.size())
                currentIndex = mValues.size() - 1;
            return mValues[currentIndex];
        }

        /**
         * Clear values and read file
         */
        void load(void)
        {
            if (mValues.size() > 0 )
                mValues.clear();
            fileRead();
        }
    protected:
        Logger * mLog;                     // Logger instance;                 default: path - "config_log"
        std::vector<T> mValues;            // values from config file
        std::string mConfigFile;               // config file
        short mMaxStringLength;          // max string length of file line;

        /**
         * @protected
         * Abstract method
         * convert from string to T
         * @return T structured line from config file
         */
        virtual T convertFromString(char *) = 0;

        /**
         * Make log message with current class name and config file name
         */
        std::string logMessage(std::string msg_I) const
        {
            std::ostringstream message("");

            message
                << typeid(*this).name()
                << ": \""
                << mConfigFile
                << "\" "
                << msg_I;

            return message.str();
        }

        /**
         * Get max char count at one line in file
         */
        int getMaxConfigurationLineLength()
        {
            return 255;
        }
    private:
        /**
         * file reading
         */
        void fileRead(void)
        {
            std::ifstream file;
            char * paramLine;

            file.open(mConfigFile);
            if (!file.is_open()) {
                mLog->error(logMessage("file not found!"));
                return;
            }
            do {
                paramLine = new char[mMaxStringLength];
                file.getline(paramLine, mMaxStringLength);
                mValues.push_back(convertFromString(paramLine));
            } while( !file.eof() );
            file.close();

            // TODO: debug message here
            // values count and readed rows
            mLog->debug( logMessage( ( "readed " + std::to_string( mValues.size() ) + " line(s) from file" ).c_str() ) );
            if (mValues.size() == 0)
                mLog->info(logMessage("file is empty!"));
        }

        /**
         * @private
         * initialize elements
         * @param char * config file
         * @param char * path for logs
         * @param unsigned short max string length of file line
         */
        void initialize(std::string configFile_I, std::string log_path_I, unsigned short length_I)
        {
            mLog = new Logger(log_path_I);
            mConfigFile = configFile_I;
            mMaxStringLength = length_I;
        }
        /**
         * @private
         * initialize elements
         * @param char * config file
         * @param Logger instance of Logger class
         * @param unsigned short max string length of file line
         */
        void initialize(std::string configFile_I, Logger * loger_I, unsigned short length_I)
        {
            mLog = loger_I;
            mConfigFile = configFile_I;
            mMaxStringLength = length_I;
        }
    };

}

#endif /*_RAINLABS_CONFIGURATION_H */

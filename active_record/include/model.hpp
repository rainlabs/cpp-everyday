/**
 * Class for active record model
 * @file include/model.hpp
 * @author Vladimir <Rain_Weezy> Zyablitskiy
 * @date 24/08/2013
 */

#ifndef _RAINLABS_ACTIVE_RECORD_MODEL_H
#define _RAINLABS_ACTIVE_RECORD_MODEL_H

#include <string>
#include <vector>
#include <map>
#include "driver.hpp"
#include "exception.hpp"
#include "../../common.hpp"

namespace ActiveRecord
{
    template < typename T >
    class Model
    {
    public:
        // new model
        Model()
        {
            mAttributes = mDriver->getColumns(getTableName());

            mIsNew = true;
            mCommitFlag = true;
        }
        virtual ~Model() {};
        static T* create(std::map<std::string, std::string> attributes_I)
        {
            T* model = new T();

            try {
                model->initialize(attributes_I);
            } catch (const InvalidInsertData& e) {
                getLogger()->error( (char*) e.what());
                return nullptr;
            }


            return model;
        }

        /**
        * set connection to database
        */
        static void setDriver(Driver* driver_I)
        {
            if (driver_I != nullptr) {
                mDriver = driver_I;
            }
        }

        void autoCommit(bool flag_I)
        {
            mCommitFlag = flag_I;
        }

        void commit()
        {
            mDriver->commit();
        }

        /**
        * Find model by id
        */
        static T* find(std::string primary_I)
        {
            T* model = new T();

            try {
                model->initialize(primary_I);
            } catch (const RecordNotFound& e) {
                getLogger()->error( (char*) e.what());
                return nullptr;
            }

            return model;
        }

        // update model
        //bool save();
        void destroy()
        {
            mDriver->destroy(getTableName(), mAttributes[getPrimaryKey()]);
            if (mCommitFlag)
                commit();
        }
        //bool updateAttributes(std::map<std::string, std::string>);
        //bool updateAttribute(std::string, std::string);

        /**
        * Get all attributes
        */
        std::map<std::string, std::string> getAttributes()
        {
            return mAttributes;
        }
        /**
        * Get raw string value
        */
        std::string get(std::string key_I)
        {
            // TODO: exception
            return mAttributes[key_I];
        }

        /**
        * Get typed value
        * Need support oprerator<< for type
        */
        //template< typename T >
        //T get(std::string);

    protected:
        void initialize(std::string primary_I)
        {
            prepare();
            mAttributes = mDriver->get(getTableName(), primary_I);

            if (mAttributes.size() == 0) {
                throw RecordNotFound(primary_I);
            }

            mIsNew = false;
        }
        void initialize(std::map<std::string, std::string> attributes_I)
        {
            prepare();
            if (!mDriver->insert(getTableName(), attributes_I))
                throw InvalidInsertData();

            mAttributes = attributes_I;
            mIsNew = false;

            if (mCommitFlag)
                commit();
        }
        void prepare()
        {
            if(mDriver == nullptr) {
                throw DriverNotDefined();
            }

            if(getTableName().size() == 0) {
                throw TableNameNotDefined();
            }
        }

        virtual std::string getPrimaryKey()
        {
            return "id";
        }

        virtual std::string getTableName()
        {
            return "";
        }

        static rainlabs::Logger* getLogger()
        {
            if (mLogger == nullptr) {
                mLogger = new rainlabs::Logger("logs");
            }

            return mLogger;
        }

        /*
        virtual std::map<std::string, std::string> getDefaults()
        {
            return {
                {
                    "id", "0"
                }
            };
        }
        */

        static Driver* mDriver;
        static rainlabs::Logger* mLogger;
        bool mIsNew;
        bool mCommitFlag;
        std::map<std::string, std::string> mAttributes;
    };

    template< typename T >
    Driver* Model<T>::mDriver                   = nullptr;

    template< typename T >
    rainlabs::Logger* Model<T>::mLogger         = nullptr;
};

#endif /* _RAINLABS_ACTIVE_RECORD_MODEL_H */

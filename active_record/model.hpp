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
#include "adapter.hpp"
#include "exception.hpp"

namespace ActiveRecord
{
    template < typename T >
    class Model
    {
    public:
        // new model
        Model()
        {
            // FIXME need load default data
            //mAttributes = mAdapter->getColumns(getTableName());

            mIsNew = true;
            mCommitFlag = true;
        }

        virtual ~Model()
        {
            mAttributes.clear();
        }

        static T* create(std::map<std::string, std::string> attributes_I)
        {
            T* model = new T();

            model->initialize(attributes_I);

            return model;
        }

        /**
        * set connection to database
        */
        static void setAdapter(Adapter* adapter_I)
        {
            if (adapter_I != nullptr) {
                mAdapter = adapter_I;
            }
        }

        void autoCommit(bool flag_I)
        {
            mCommitFlag = flag_I;
        }

        void commit()
        {
            mAdapter->commit();
        }

        /**
        * Find model by id
        */
        static T* find(std::string primary_I)
        {
            T* model = new T();

            model->initialize(primary_I);

            return model;
        }

        static std::vector<T*> where(std::map<std::string, std::string> where_I)
        {
            std::vector< std::map<std::string, std::string> > values;
            std::vector< T* > result;
            T* tmp = new T();
            values = tmp->mAdapter->get(tmp->getTableName(), where_I);

            for(auto it = values.begin(); it != values.end(); it++) {
                tmp = new T( *it );
                result.push_back( tmp );
            }
            return result;
        }

        /**
          * TODO
          * get all records from table
          */
        static std::vector<T*> all()
        {
            std::map<std::string, std::string> blankWhere = {};
            return where(blankWhere);
        }

        /**
          * truncate table
          */
        static void destroyAll()
        {
            T* tmp = new T();
            std::map<std::string, std::string> blankWhere = {};

            tmp->mAdapter->destroy(tmp->getTableName(), blankWhere);

            if (tmp->mCommitFlag)
                tmp->commit();
        }

        /**
          * Remove record from table
          */
        void destroy()
        {
            std::map<std::string, std::string> where = { {getPrimaryKey(), mAttributes[getPrimaryKey()]} };
            mAdapter->destroy(getTableName(), where);
            if (mCommitFlag)
                commit();
            mIsNew = true;
        }

        /**
          * Update one attribute
          * @param key_I string table field
          * @param value_I string record value
          * @return bool
          */
        bool update(std::string key_I, std::string value_I)
        {
            std::map<std::string, std::string> attributes = { {key_I, value_I} };
            return update(attributes);
        }

        /**
          * Update many attributes
          * @param attributes_I map attributes hash
          * @return bool
          */
        bool update(std::map<std::string, std::string> attributes_I)
        {
            try {
                if (!mIsNew) {
                    std::map<std::string, std::string> where = { {getPrimaryKey(), mAttributes[getPrimaryKey()]} };
                    mAdapter->update(getTableName(), attributes_I, where);
                    if (mCommitFlag)
                        commit();
                    return true;
                }
            } catch (const std::exception& e) {
                return false;
            }

            return false;
        }

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
            if (mAttributes.count(key_I) == 0)
                throw Exception::AttributeNotFound(getTableName(), key_I);
            return mAttributes[key_I];
        }

        /**
        * Get typed value
        * Need support oprerator>> for type
        */
        template< typename C >
        C get(std::string key_I)
        {
            C result;
            std::istringstream iss(get(key_I));

            iss >> result;

            return result;
        }

        /**
          * Set 1 Attribute
          */
        void set(std::string key_I, std::string value_I)
        {
            mAttributes[key_I] = value_I;
        }

        /**
          * Set Many attributes
          */
        void set(std::map<std::string, std::string> attributes_I)
        {
            for(auto& it: attributes_I) {
                mAttributes[it.first] = it.second;
            }
        }

        /**
          * Save model (insert/update record)
          */
        bool save()
        {
            try {
                if(mIsNew) {
                    mAdapter->insert(getTableName(), mAttributes);
                    mIsNew = false;
                } else {
                    std::map<std::string, std::string> where = { {getPrimaryKey(), mAttributes[getPrimaryKey()]} };
                    mAdapter->update(getTableName(), mAttributes, where);
                }
            } catch (const std::exception& e) {
                return false;
            }

            return true;
        }

    protected:
        Model(std::map<std::string, std::string> attributes_I)
        {
            mAttributes = attributes_I;
            mIsNew = false;
            mCommitFlag = true;
        }

        void initialize(std::string primary_I)
        {
            prepare();
            std::map<std::string, std::string> where = { {getPrimaryKey(), primary_I} };
            std::vector< std::map<std::string, std::string> > response;
            response = mAdapter->get(getTableName(), where);

            if (response.size() != 1) {
                throw Exception::RecordNotFound(getTableName(), primary_I);
            }
            mAttributes = response[0];

            mIsNew = false;
        }
        void initialize(std::map<std::string, std::string> attributes_I)
        {
            prepare();
            if (!mAdapter->insert(getTableName(), attributes_I))
                throw Exception::InvalidInsertData(getTableName(), attributes_I);

            mAttributes = attributes_I;
            mIsNew = false;

            if (mCommitFlag)
                commit();
        }
        void prepare()
        {
            if(mAdapter == nullptr) {
                throw Exception::AdapterNotDefined();
            }

            if(getTableName().size() == 0) {
                throw Exception::TableNameNotDefined();
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

        static Adapter* mAdapter;
        bool mIsNew;
        bool mCommitFlag;
        std::map<std::string, std::string> mAttributes;
    };

    template< typename T >
    Adapter* Model<T>::mAdapter                   = nullptr;
};

#endif /* _RAINLABS_ACTIVE_RECORD_MODEL_H */

/**
 * Abstract class, simple template of Singleton
 * @file rainlabs/common/include/singleton.hpp
 * @author Vladimir <Rain_Weezy> Zyablitskiy
 * @date 09/08/2013
 *
 */

#ifndef _RAINLABS_SINGLETON_H
#define _RAINLABS_SINGLETON_H

namespace rainlabs
{
    template <typename T>
    class Singleton
    {
    public:
        virtual ~Singleton () {};
        static T* getInstance ()
        {
            if (mInstance == nullptr)
                mInstance = new T();
            return mInstance;
        }

        static bool DestroyInstance()
        {
            if(mInstance != nullptr)
            {
                delete mInstance;
                mInstance = nullptr;
                return true;
            }
            return false;
        }
    
    protected:
        Singleton () {};
        static T* mInstance;
    };
 
    template <typename T>
    T* Singleton<T>::mInstance = nullptr;
}

#endif /* _RAINLABS_SINGLETON_H */
#pragma once
#include <cassert>
#include <cstdlib>
#include <mutex>

    /////////////////////////////////////////////////
    // 说明: 单件类           
    /////////////////////////////////////////////////
    // 
    /**
     * 单件实现类, 参考loki中的SingletonHolder实现
     * 没有实现对单件生命周期的管理
     * 使用示例代码如下:
     class A : public TC_Singleton<A, CreateStatic,  DefaultLifetime>
     {
     public:
         A(){cout << "A" << endl;}
         ~A()
         {
             cout << "~A" << endl;
         }

         void test(){cout << "test A" << endl;}
     };
     *
     * 对象的创建方式由CreatePolicy指定, 有如下方式:
     * CreateUsingNew: 在堆中采用new创建
     * CreateStatic`: 在栈中采用static创建
     *
     * 对象生命周期管理由LifetimePolicy指定, 有如下方式:
     * DefaultLifetime:缺省声明周期管理
     *      如果单件对象已经析够, 但是还有调用, 会触发异常
     * PhoneixLifetime:不死生命周期
     *      如果单件对象已经析够, 但是还有调用, 会再创建一个
     * NoDestroyLifetime:不析够
     *      对象创建后不会调用析够函数析够
     * 通常采用实例中的方式就可以了
     */

     ////////////////////////////////////////////////////////////////
     // 定义CreatePolicy:定义对象创建策略:
     // 
    template<typename T>
    class CreateUsingNew
    {
    public:
        /**
         * 创建
         *
         * @return T*
         */
        static T* create()
        {
            return new T;
        }

        /**
         * 释放
         * @param t
         */
        static void destroy(T* t)
        {
            delete t;
        }
    };

    template<typename T>
    class CreateStatic
    {
    public:
        //最大的空间
        union MaxAlign
        {
            char t_[sizeof(T)];
            long double longDouble_;
        };

        /**
         * 创建
         *
         * @return T*
         */
        static T* create()
        {
            static MaxAlign t;
            return new(&t) T;
        }

        /**
         * 释放
         * @param t
         */
        static void destroy(T* t)
        {
            t->~T();
        }
    };

    ////////////////////////////////////////////////////////////////
    // 定义LifetimePolicy:定义对象的声明周期管理
    // 

    template<typename T>
    class DefaultLifetime
    {
    public:
        static void deadReference()
        {
            throw std::logic_error("singleton object has dead.");
        }

        static void scheduleDestruction(T*, void (*pFun)())
        {
            std::atexit(pFun);
        }
    };

    template<typename T>
    class PhoneixLifetime
    {
    public:
        static void deadReference()
        {
            _bDestroyedOnce = true;
        }

        static void scheduleDestruction(T*, void (*pFun)())
        {
            if (!_bDestroyedOnce)
                std::atexit(pFun);
        }
    private:
        static bool _bDestroyedOnce;
    };
    template <class T>
    bool PhoneixLifetime<T>::_bDestroyedOnce = false;

    template <typename T>
    struct NoDestroyLifetime
    {
        static void scheduleDestruction(T*, void (*)())
        {
        }

        static void deadReference()
        {
        }
    };

    //////////////////////////////////////////////////////////////////////
    // Singleton
    template
        <
        typename T,
        template<class> class CreatePolicy = CreateUsingNew,
        template<class> class LifetimePolicy = DefaultLifetime
        >
        class Singleton
    {
    public:
        typedef T  instance_type;
        typedef volatile T volatile_type;

        /**
         * 获取实例
         *
         * @return T*
         */
        static T* getInstance()
        {
            //加锁, 双check机制, 保证正确和效率
            if (!_pInstance)
            {
                _mutex.lock();
                if (!_pInstance)
                {
                    if (_destroyed)
                    {
                        LifetimePolicy<T>::deadReference();
                        _destroyed = false;
                    }
                    _pInstance = CreatePolicy<T>::create();
                    LifetimePolicy<T>::scheduleDestruction((T*)_pInstance, &destroySingleton);
                }
                _mutex.unlock();
            }

            return (T*)_pInstance;
        }

        virtual ~Singleton() {};

    protected:

        static void destroySingleton()
        {
            assert(!_destroyed);
            CreatePolicy<T>::destroy((T*)_pInstance);
            _pInstance = NULL;
            _destroyed = true;
        }
    protected:

        static std::mutex       _mutex;
        static volatile T*      _pInstance;
        static bool             _destroyed;

    protected:
        Singleton() {}
        Singleton(const Singleton&);
        Singleton& operator=(const Singleton&);
    };

    template <class T, template<class> class CreatePolicy, template<class> class LifetimePolicy>
    std::mutex Singleton<T, CreatePolicy, LifetimePolicy>::_mutex;

    template <class T, template<class> class CreatePolicy, template<class> class LifetimePolicy>
    bool Singleton<T, CreatePolicy, LifetimePolicy>::_destroyed = false;

    template <class T, template<class> class CreatePolicy, template<class> class LifetimePolicy>
    volatile T* Singleton<T, CreatePolicy, LifetimePolicy>::_pInstance = nullptr;




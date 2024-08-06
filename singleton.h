#ifndef SINGLETON_H
#define SINGLETON_H

#include "global.h"

template <typename T>
class Singleton {
public:
    static std::shared_ptr<T> GetInstance() {
        static std::once_flag s_flag;
        std::call_once(s_flag, [&](){
            /*为什么此处直接使用new来传递给shared_ptr，而不是用make_shared?*/
            /*因为make_shared会访问类构造函数，但是此时构造函数是protected*//*---->待考证*/
            instance_ = std::shared_ptr<T>(new T);
        });
        return instance_;
    }
    ~Singleton() {
        std:: cout << "Singleton deconstruct..." << std::endl;
    }

    void PrintAddress() {
        std::cout << instance_.get() << std::endl;
    }



protected:
    Singleton() = default;  // 设置为protected是为了子类能够构造这个基类
    Singleton(const Singleton<T>&) = delete;
    Singleton& operator=(const Singleton<T>&) = delete;

    static std::shared_ptr<T> instance_;
};

template <typename T>
std::shared_ptr<T> Singleton<T>::instance_ = nullptr;


#endif // SINGLETON_H

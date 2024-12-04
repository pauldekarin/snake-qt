#pragma once
#include <vector>
#include <functional>
#include <mutex>

template <class... Args>
class __observer: public Args...{
private:
    typedef std::function<void(Args...)> __recipient;

    std::vector<__recipient> recipients_;
    std::mutex mtx_;

public:
    __observer(){

    }
    ~__observer(){

    }

    void connect(const __recipient &__fn){
        this->recipients_.emplace_back(__fn);
    }

    void notify(const Args&... args){
        std::lock_guard<std::mutex> __lock(this->mtx_);
        for(const __recipient &__fn : this->recipients_){
            __fn(args...);
        }
    }
};
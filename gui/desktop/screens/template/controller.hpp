#pragma once

#include <QtCore/QObject>
#include <memory>

template <class _View, class _Model>
class __controller{
public:
    typedef _View view_type;
    typedef _Model model_type;
    typedef view_type* view_pointer;
    typedef model_type* model_pointer;

protected:
    std::shared_ptr<view_type> view_;
    std::shared_ptr<model_type> model_;

public:
    __controller(std::shared_ptr<view_type> __view, std::shared_ptr<model_type> __model):view_(__view), model_(__model){}
    __controller(view_type* __view, std::shared_ptr<model_type> __model):__controller(std::make_shared<view_type>(__view), __model){}
    __controller(std::shared_ptr<view_type> __view, model_type* __model):__controller(__view, std::make_shared<model_type>(__model)){}

    inline std::shared_ptr<view_type> view() const{ return this->view_; }
    inline std::shared_ptr<model_type> model() const{ return this->model_; }
};
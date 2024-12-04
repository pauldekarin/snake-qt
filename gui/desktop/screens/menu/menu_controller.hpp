#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QGraphicsBlurEffect>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>

#include "../../../../brick_game/snake/include/db.hpp"
#include "../template/controller.hpp"
#include "menu_view.hpp"

class __menu_contoller: public __controller<__menu_view, __db>{
public:
    __menu_contoller(std::shared_ptr<__menu_view> __view, std::shared_ptr<__db> __model):__controller(__view, __model){
        this->load_scores();
    }

    void load_scores(){
        this->view_->set_scores(this->model_->load_scores());
    }
};
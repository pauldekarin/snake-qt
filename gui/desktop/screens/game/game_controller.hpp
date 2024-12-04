#ifndef GAME_CONTROLLER_HPP
#define GAME_CONTROLLER_HPP

#include <QtWidgets/QWidget>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QGraphicsRectItem>
#include <QtWidgets/QGraphicsItem>

#include <QtCore/QTimer>
#include <QtGui/QKeyEvent>

#include "../../../../brick_game/snake/include/engine.hpp"

#include "../template/controller.hpp"
#include "game_view.hpp"

class __game_controller : public QObject, public __controller<__game_view, __engine>{
    Q_OBJECT

private:
    std::unique_ptr<QTimer> timer_;

signals:
    void __sg_finish();

private slots:
    void __loop(){
        this->view_->update_with_info(updateCurrentState());
    }

public:
    __game_controller(std::shared_ptr<__game_view> __view, std::shared_ptr<__engine> __model):__controller(__view, __model){
        this->timer_ = std::make_unique<QTimer>(this);
        QObject::connect(this->timer_.get(), &QTimer::timeout, this, &__game_controller::__loop);

        this->model_->finish.connect(std::bind(&__game_controller::finish, this));

        QObject::connect(this->view_.get(), &__game_view::__sg_key, this, &__game_controller::user_input);
    };

    void play(){
        this->timer_->start(60);
        this->model_->start();
    }

    void finish(){
        this->timer_->stop();
        emit this->__sg_finish();
    }

    void user_input(QKeyEvent *__evt){
        switch (__evt->key())
        {
        case Qt::Key_A:{
            userInput(UserAction_t::Left, false);
            break;
        }
        case Qt::Key_W:{
            userInput(UserAction_t::Up, false);
            break;
        }
        case Qt::Key_S:{
            userInput(UserAction_t::Down, false);
            break;
        }
        case Qt::Key_D:{
            userInput(UserAction_t::Right, false);
            break;
        }
        default:
            break;
        }
    }
    GameInfo_t __update(){
        return updateCurrentState();
    }
};

#endif
#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QGraphicsBlurEffect>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtGui/QPalette>
#include <QtGui/QPainter>
#include <QtGui/QPixmap>

#include "../../../../brick_game/snake/include/db.hpp"
#include "../../components/shadow/shadow.hpp"
#include "../../components/animated_button/animated_button.hpp"
class __menu_view : public QWidget, public std::enable_shared_from_this<__menu_view>{
    Q_OBJECT

private:
    std::unique_ptr<__shadowed_text> high_score_title_;

signals:
    void __sg_start();


private:
    

    void __setup_ui(){
        QFont retroFont = __settings::font().primary();

        retroFont.setPixelSize(48);
        __shadowed_text *title = new __shadowed_text();
        title->setText("SNAKE");
        title->setFont(retroFont);
        title->setAlignment(Qt::AlignCenter);

        
        retroFont.setPixelSize(16);
        high_score_title_->setAlignment(Qt::AlignCenter);
        high_score_title_->setFont(retroFont);

        retroFont.setPixelSize(36);
        __animated_button *__start_btn = new __animated_button();
        __start_btn->btn()->setText("PRESS");
        __start_btn->btn()->setFont(retroFont);

        QObject::connect(__start_btn->btn(), &QPushButton::clicked, this, [this](){
            emit __sg_start();
        });

        QVBoxLayout *__lt = new QVBoxLayout(this);
        __lt->addStretch(1);
        __lt->addWidget(title);
        __lt->addWidget(high_score_title_.get());
        __lt->addStretch(1);
        __lt->addWidget(__start_btn);
        __lt->addStretch(1);

        this->setLayout(__lt);

        
    }

public:
    __menu_view():
        high_score_title_(std::make_unique<__shadowed_text>())
    {
        __setup_ui();
    }

    ~__menu_view(){

    }

    void set_scores(int __scores){
        high_score_title_->setText("HIGH SCORE: " + QString::number(__scores));
    }
};

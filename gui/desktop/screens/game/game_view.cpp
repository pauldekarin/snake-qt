#include "game_view.hpp"

__game_view::__game_view( ):
view_(std::make_unique<QGraphicsView>()),
score_title_(std::make_unique<__shadowed_text>()),
level_title_(std::make_unique<__shadowed_text>())

{
    this->view_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->view_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->view_->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    QFont __font(score_title_->font());
    __font.setPixelSize(16);

    score_title_->setFont(__font);
    level_title_->setFont(__font);

    QGraphicsScene *__scene = new QGraphicsScene();
    
    this->view_->setScene(__scene);

    QVBoxLayout *__main_lt = new QVBoxLayout(this);

    QHBoxLayout *__ht = new QHBoxLayout();
    __ht->addWidget(this->score_title_.get(),0, Qt::AlignLeft);
    __ht->addWidget(this->level_title_.get(), 0, Qt::AlignRight);

    QVBoxLayout *__lt = new QVBoxLayout();
    __lt->addWidget(this->view_.get());

    __main_lt->addLayout(__ht);
    __main_lt->addLayout(__lt);
}

__game_view::~__game_view(){}
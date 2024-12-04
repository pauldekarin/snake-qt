#pragma once

#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QGraphicsDropShadowEffect>
#include <QtGui/QPainter>
#include <QtGui/QPainterPath>

class __shadowed_text: public QLabel{
public:
    explicit __shadowed_text(QWidget *__parent = nullptr):QLabel(__parent){
        QGraphicsDropShadowEffect *__effect = new QGraphicsDropShadowEffect();
        __effect->setBlurRadius(0);
        __effect->setOffset(4,4);

        setGraphicsEffect(__effect);
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        QLabel::paintEvent(event);
    }

};
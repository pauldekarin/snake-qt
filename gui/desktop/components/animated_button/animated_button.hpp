#pragma once

#include <QtWidgets/QPushButton>
#include <QtCore/QEvent>
#include <QtCore/QVariantAnimation>
#include <QtGui/QPainter>
#include <QtCore/QObject>
#include <QtWidgets/QVBoxLayout>

#include "../../settings.hpp"

class __animated_button: public QWidget{
    Q_OBJECT

    std::unique_ptr<QVariantAnimation> m_transition;
    std::unique_ptr<QPushButton> m_button;
    std::atomic<std::double_t> offset_ = 0;

signals:
    void __sg_offset(double);

private:
    void __animate_hover(bool __in){
        if(m_transition){
            m_transition->stop();
            m_transition.release();
        }

        m_transition = std::make_unique<QVariantAnimation>(this);
      
        m_transition->setDuration(80);
        m_transition->setStartValue(__in ? 0.0 : 1.0);
        m_transition->setEndValue(__in ? 1.0 : 0.0);
        m_transition->setEasingCurve(QEasingCurve::InOutExpo);

        QObject::connect(m_transition.get(), &QVariantAnimation::valueChanged, this, [this](const QVariant &__var){
            offset_.store(__var.toDouble());
            emit this->__sg_offset(__var.toDouble());
        });

        m_transition->start();
    }


protected:
    void enterEvent(QEnterEvent *__evt) override{
        __animate_hover(true);
        QWidget::enterEvent(__evt);
    }

    void leaveEvent(QEvent *__evt) override{
        __animate_hover(false);
        QWidget::leaveEvent(__evt);
    }


    void paintEvent(QPaintEvent *__evt) override {
        QWidget::paintEvent(__evt);
    }

public:
    explicit __animated_button(QWidget *__parent = nullptr):QWidget(__parent){
        QString s_sheet = QString(
        "%1{"
            "background-color:%2;"
            "border:2px solid black;"
        "}");

        QVBoxLayout *lt = new QVBoxLayout(this);
        lt->setSpacing(0);
        lt->setContentsMargins(0,0,0,0);

        m_button = std::make_unique<QPushButton>(this);
        m_button->setStyleSheet(s_sheet.arg("QPushButton", "#8b966e"));

        std::function<QWidget*(const char*, int)> __get_layer = [&](const char* __colour, int __h){
            QWidget *__layer = new QWidget();
            __layer->setStyleSheet(s_sheet.arg("QWidget", __colour));
            __layer->setFixedHeight(__h);

            QObject::connect(this, &__animated_button::__sg_offset, __layer, [__layer, __h](double __offs){
                __layer->setFixedHeight(__h -  __h * __offs / 5);
            });
            return __layer;
        };


        lt->addWidget(m_button.get());
        lt->addWidget(__get_layer("#adb88e", 6));
        lt->addWidget(__get_layer("#596247", 9));
    }
    
    QPushButton* btn() {
        return m_button.get();
    }

    void setPixelSize(int __sz){
        QFont __f(font());
        __f.setPixelSize(__sz);
        setFont(__f);
    }
};
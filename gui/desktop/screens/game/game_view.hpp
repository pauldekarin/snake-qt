#ifndef GAME_VIEW_HPP
#define GAME_VIEW_HPP

#include <QtWidgets/QWidget>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QGraphicsRectItem>
#include <QtWidgets/QGraphicsItem>
#include <QtCore/QObject>
#include <QtCore/QSettings>

#include <QtCore/QTimer>
#include <QtGui/QKeyEvent>

#include <type_traits>

#include "../../../../brick_game/snake/include/engine.hpp"
#include "../../settings.hpp"
#include "../../components/shadow/shadow.hpp"

class __game_view : public QWidget, public std::enable_shared_from_this<__game_view>{
    Q_OBJECT

signals:
    void __sg_key(QKeyEvent *__evt);


protected:
    void keyPressEvent(QKeyEvent *__evt) override{
        emit this->__sg_key(__evt);
    }

private:
    std::unique_ptr<QGraphicsView> view_;
    std::unique_ptr<__shadowed_text> score_title_;
    std::unique_ptr<__shadowed_text> level_title_;

    class __item_types{
    public:
        class __apple__{};
        class __snake_body__{};
        class __snake_head__{};
        class __world_back__{};
    };

    template <class _Type, typename = std::enable_if< std::is_base_of<__item_types, _Type>::value>>
    class __item : public QGraphicsRectItem{
        QSize padding_ = QSize(0,0);

    public:
        typedef _Type type;
        
        __item(const QPointF &__pos = QPointF(0,0), const QSize &__size = QSize(10,10)){
            this->setRect(
                __pos.x() + this->padding_.width(),
                __pos.y() + this->padding_.height(), 
                __size.width() - this->padding_.width(), 
                __size.height() - this->padding_.height()
            );
        }

        void paint(QPainter *__painter, __attribute__((unused)) const QStyleOptionGraphicsItem *__opt, __attribute__((unused)) QWidget *__wid = nullptr) override{
            QPen __default_pen;
            __default_pen.setDashPattern({2,2,4,8});

            __painter->setPen(__default_pen);
            __painter->setBrush(Qt::Dense7Pattern);

            if constexpr(std::is_same_v<type, __item_types::__apple__>){
                __draw_apple(__painter);
            }else if constexpr(std::is_same_v<type, __item_types::__snake_body__>){
                __draw_snake_body(__painter);
            }else if constexpr(std::is_same_v<type, __item_types::__snake_head__>){
                __draw__snake_head(__painter);
            }else if constexpr(std::is_same_v<type, __item_types::__world_back__>){
                __draw_world_back(__painter);
            }
            
        }
    
    private:
        void __draw_snake_body(QPainter *__pt){
            QColor acs_color = __settings::color().snake_body();
            QColor lt_shadow = __settings::color().light_shadow();

            QPen p(__pt->pen());
            p.setWidth(2);
            p.setColor(acs_color);

            __pt->setPen(p);
            __pt->drawRect(rect());

            QBrush br(__pt->brush());
            br.setColor(acs_color);

            __pt->setBrush(br);
            __pt->drawRect(rect().adjusted(4,4,-4,-4));

            p.setWidth(2);
            p.setColor(lt_shadow);

            __pt->setPen(p);
            __pt->drawRect(rect().adjusted(4,4,-4,-4));
        }

        void __draw__snake_head(QPainter *__pt){
            QColor acs_color = __settings::color().snake_head();
            QColor lt_shadow = __settings::color().light_shadow();

            QPen p(__pt->pen());
            p.setWidth(2);
            p.setColor(acs_color);

            __pt->setPen(p);
            __pt->drawRect(rect());

            QBrush br(__pt->brush());
            br.setColor(acs_color);

            __pt->setBrush(br);
            __pt->drawRect(rect().adjusted(4,4,-4,-4));

            p.setWidth(2);
            p.setColor(lt_shadow);

            __pt->setPen(p);
            __pt->drawRect(rect().adjusted(4,4,-4,-4));
        }   

        void __draw_world_back(__attribute__((unused)) QPainter *__pt){}

        void __draw_apple(QPainter *__pt){
            QPolygonF __octagon;
            QPointF __c = rect().center();
            qreal __r = std::min(rect().width(), rect().height()) / 2;
            int __s = 8;

            for(int i = 0; i < __s; i++){
                __octagon << QPointF(__c.x() + __r * std::cos(qDegreesToRadians((360.0 / __s) * i)), __c.y() + __r * std::sin(qDegreesToRadians((360.0 / __s) * i)));
            }

            QPen __p(__pt->pen());
            QBrush __br(__pt->brush());


            __p.setWidth(1);
            __p.setColor(__settings::color().apple());

            __br.setColor(__settings::color().apple());

            __pt->setBrush(__br);
            __pt->setPen(__p);
            __pt->drawPolygon(__octagon);
        }
    };

public:
    __game_view();
    ~__game_view();

    void update_with_info(const GameInfo_t &info_){
        __draw_stats(info_);
        __draw_field(info_);
    }


private:
    void __draw_stats(const GameInfo_t &__info){
        this->score_title_->setText("SCORES:" + QString::number(__info.score));
        this->level_title_->setText("LEVEL:" + QString::number(__info.level));
    }

    void __draw_field(const GameInfo_t &__info){
        QSize __block_size = QSize(
            this->view_->size().width() / WIDTH, 
            this->view_->size().height() / HEIGHT 
        );

        this->view_->scene()->clear();

        for(std::size_t __i = 0; __i < HEIGHT * WIDTH; __i++){
            std::size_t __y = __i / WIDTH;
            std::size_t __x = __i  % WIDTH;

            QPointF __pos ( 
                __x  * __block_size.width(), 
                __y * __block_size.height()
            );

            #ifdef __SNAKE__
            if(__info.field[__y][__x] == SNAKE_BODY){
                this->view_->scene()->addItem(new __item<__item_types::__snake_body__>(__pos, __block_size));
            }else if(__info.field[__y][__x] == SNAKE_HEAD){
                this->view_->scene()->addItem(new __item<__item_types::__snake_head__>(__pos, __block_size));
            }else if(__info.field[__y][__x] < 0){
                this->view_->scene()->addItem(new __item<__item_types::__apple__>(__pos, __block_size));
            }else if(__info.field[__y][__x] == 0){
                this->view_->scene()->addItem(new __item<__item_types::__world_back__>(__pos, __block_size));
            }
            #else
            if(__info.field[__y][__x] == 0){
                this->view_->scene()->addItem(new __item<__item_types::__snake_body__>(__pos, __block_size));
            }else if(__info.field[__y][__x] > 0){
                this->view_->scene()->addItem(new __item<__item_types::__snake_head__>(__pos, __block_size));
            }else if(__info.field[__y][__x] < 0){
                this->view_->scene()->addItem(new __item<__item_types::__apple__>(__pos, __block_size));
            }
            #endif
        }
        this->view_->scene()->update();
    }

};

#endif
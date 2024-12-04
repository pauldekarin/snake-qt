#pragma once

#include <QtCore/QDirIterator>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStackedWidget>
#include <QtGui/QFont>
#include <QtGui/QFontDatabase>
#include <QtCore/QtLogging>
#include <QtCore/QSettings>
#include <QtGui/QScreen>
#include <mutex>
#include <iostream>
#include <map>
#include <functional>

#include "screens/menu/menu_controller.hpp"
#include "screens/menu/menu_view.hpp"
#include "screens/game/game_controller.hpp"
#include "screens/game/game_view.hpp"
#include "styles/proxy.hpp"

#define PIXEL_SIZE 48

QColor f(const QColor& __begin,const QColor& __end, qreal __weight){
    int r = __begin.red() * __weight + (1 - __weight) * __end.red();
    int g = __begin.green() * __weight + (1 - __weight) * __end.green();
    int b = __begin.blue() * __weight + (1 - __weight) * __end.blue();
    int a = __begin.alpha() * __weight + (1 - __weight) * __end.alpha();
    return QColor(r,g,b,a);
}

class __main_win: public QMainWindow{
public:
    __main_win();
    
    void __display();

private:   
    std::mutex mtx_;

    std::shared_ptr<__game_controller> game_controller_;
    std::shared_ptr<__menu_contoller> menu_controller_;

    void __setup_window_settings();
    void __setup_styles();
    void __setup_default_font();
    void __setup_central_widget();
    void __setup_controller_connections();

protected:
    void resizeEvent(QResizeEvent *__evt) override{
        QMainWindow::resizeEvent(__evt);

        QPalette palette;
        QPixmap pixmap(width(), height() / 4);
        QPainter painter(&pixmap);

        qint64 __steps = 16;
        QRect rect = QRect(0, 0, width(), height() / __steps);
        QBrush brush(Qt::BrushStyle::SolidPattern);

        for (qint64 __step = 0; __step < __steps; __step++) {
            brush.setColor(f(QColor("#8C966E"), QColor("#696C61"), (1.0 / __steps) * __step));
            painter.setBrush(brush);
            painter.drawRect(rect);
            rect.translate(0, rect.height());
        }
        palette.setBrush(QPalette::ColorRole::Window, QBrush(pixmap));

        centralWidget()->setPalette(palette);
        centralWidget()->update();
    }
};
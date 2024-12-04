#pragma once

#include <QtCore/QSettings>
#include <QtGui/QColor>

class __settings{
private:
    __settings(){}
    ~__settings(){}

    class __clr{
    public:
        __clr(){}
        ~__clr(){}

        QColor snake_head(){
            return QColor(__settings::settings_->value("color/snake_head", "#6E7D96").toString());
        }

        QColor snake_body(){
            return QColor(__settings::settings_->value("color/snake_body", "#3b3e2d").toString());
        }

        QColor apple(){
            return QColor(__settings::settings_->value("color/apple", "#FFA091").toString());
        }

        QColor accent(){
            return QColor(__settings::settings_->value("color/accent", "#3b3e2d").toString());
        };

        QColor primary(){
            return QColor(__settings::settings_->value("color/primary", "#c4d0a2").toString());
        };

        QColor light_shadow(){
            return QColor(__settings::settings_->value("color/light_shadow", "#acb78d").toString());
        }

        QColor text(){
            return QColor(__settings::settings_->value("color/light_shadow", "#fafafa").toString());
        }
    };

    class __ft{
    public: 
        __ft(){}
        ~__ft(){}

        QFont primary(){
            return QFont(__settings::settings_->value("font/primary", "Press Start 2P").toString());
        }
    };

    static inline std::unique_ptr<QSettings> settings_ = std::unique_ptr<QSettings>(new QSettings("settings.ini", QSettings::NativeFormat));
    static inline std::unique_ptr<__clr> color_ = std::unique_ptr<__clr>(new __clr());
    static inline std::unique_ptr<__ft> font_ = std::unique_ptr<__ft>(new __ft());
    friend __clr;
    friend __ft;

public: 
    static __clr& color(){
        return *__settings::color_;
    };

    static __ft& font(){
        return *__settings::font_;
    }
};
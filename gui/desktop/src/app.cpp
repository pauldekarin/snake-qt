#include "../app.hpp"

__main_win::__main_win():
game_controller_(std::make_shared<__game_controller>(std::shared_ptr<__game_view>(new __game_view()), __engine::__instance())),
menu_controller_(std::make_shared<__menu_contoller>(std::shared_ptr<__menu_view>(new __menu_view()), __db::__instance()))
{   
    __setup_window_settings();
    __setup_default_font();
    __setup_styles();
    __setup_central_widget();
    __setup_controller_connections();
}   

void __main_win::__display(){
    this->show();
}

void __main_win::__setup_window_settings(){
    adjustSize();
    move(screen()->geometry().center() - frameGeometry().center());
}

void __main_win::__setup_styles(){
    QFile __style_file(":/styles/style.qss");
    if(__style_file.open(QFile::ReadOnly)){
        this->setStyleSheet(__style_file.readAll());
    }
    setStyle(new ProxyStyle());
}

void __main_win::__setup_central_widget() {
    QStackedWidget *__widget = new QStackedWidget();

    

    __widget->setAutoFillBackground(true);
    __widget->addWidget(this->menu_controller_->view().get());
    __widget->addWidget(this->game_controller_->view().get());

    this->setCentralWidget(__widget);
}


void __main_win::__setup_default_font(){
    QStringList __filters ={QStringLiteral("*.ttf"), QStringLiteral("*.otf")};
    QDirIterator __iter(QStringLiteral(":/fonts"), __filters,QDir::Files | QDir::Readable, QDirIterator::Subdirectories);
    while(__iter.hasNext()){
        QFontDatabase::addApplicationFont(__iter.next());
    }
    this->setFont(QFont("Press Start 2P"));
}

void __main_win::__setup_controller_connections(){
    QObject::connect(this->menu_controller_->view().get(), &__menu_view::__sg_start, [this](){
        std::lock_guard<std::mutex> __lock(this->mtx_);

        static_cast<QStackedWidget*>(this->centralWidget())->setCurrentIndex(1);
        this->game_controller_->play();
    });

    QObject::connect(this->game_controller_.get(), &__game_controller::__sg_finish, [this](){
        std::lock_guard<std::mutex> __lock(this->mtx_);

        static_cast<QStackedWidget*>(this->centralWidget())->setCurrentIndex(0);
        this->menu_controller_->load_scores();
    });
}

int main(int argc, char **argv){
    QApplication app_(argc, argv);

    __main_win win_;
    win_.__display();

    return app_.exec();
}
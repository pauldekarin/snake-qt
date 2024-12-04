#pragma once
#include <QtCore/QObject>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

#include <sstream>
#include <iostream>

class __db{

private:
    // static std::unique_ptr<__db> instance_;
    QSqlDatabase database_;

    __db():database_(QSqlDatabase::addDatabase("QSQLITE")){
        this->database_.setDatabaseName("db_snake");
        this->database_.setHostName("127.0.0.1");
        this->database_.setPort(5432);
        if(this->database_.isValid() && this->database_.open()){
            std::cout << "Database connected\n";

            QSqlQuery query(this->database_);
            query.prepare("CREATE TABLE IF NOT EXISTS player (scores INT DEFAULT 0 )");
            if(!query.exec()){

            }

            query.prepare("INSERT INTO player (scores) SELECT 0 WHERE NOT EXISTS (SELECT 1 FROM player)");
            if (!query.exec()) {
                std::cerr << "Error ensuring initial row: " << query.lastError().text().toStdString() << '\n';
            }

            query.finish();
        }
    }


public:
    ~__db(){
        this->database_.close();
    }
    static std::shared_ptr<__db> __instance(){
        static std::shared_ptr<__db> db_ = std::shared_ptr<__db>(new __db());
        return db_;
    }

    int load_scores(){
        QSqlQuery __q(this->database_);
        __q.prepare("SELECT scores FROM player LIMIT 1");
        if(__q.exec() && __q.next()){
            return __q.value(0).toInt();
        }else{
            std::cerr << __q.lastError().text().toStdString() << "\n";
            return 0;
        }
    }

    void store_scores(int __scores){
        QSqlQuery __q(this->database_);
        __q.prepare("UPDATE player SET scores = :scores");
        __q.bindValue(":scores", __scores);
        if(!__q.exec()){
            std::cerr << __q.lastError().text().toStdString() << "\n";
        }
    }
};


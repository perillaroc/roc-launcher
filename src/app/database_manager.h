#pragma once

#include <QObject>
#include <QtSql/QSqlDatabase>


class DatabaseManager: public QObject
{
    Q_OBJECT
public:
    DatabaseManager(QObject *parent=nullptr);

    void initDatabase();

private:
    void checkDatabase();

    void createTables();

    void createTableVersion();

    QSqlDatabase db_;
};

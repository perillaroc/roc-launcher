#pragma once

#include <QObject>
#include <QtSql/QSqlDatabase>


class DatabaseManager: public QObject
{
    Q_OBJECT
public:
    DatabaseManager(QObject *parent=nullptr);

    void initDatabase();

    void buildLinks();

    void queryLinks(const QString &str);

private:
    void checkDatabase();

    void createTables();
    void createTableVersion();
    void createTableLinks();
    void clearTableLinks();

    void buildLinksForDirPath(const QString &dir);

    QSqlDatabase db_;
};

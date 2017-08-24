#pragma once

#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QVector>


struct Link{
    int id_;
    QString name_;
    QString location_;
    QString icon_path_;
};


class DatabaseManager: public QObject
{
    Q_OBJECT
public:
    DatabaseManager(QObject *parent=nullptr);

    void initDatabase();

    void buildLinks();

    QVector<Link> queryLinks(const QString &str);

private:
    void checkDatabase();

    void createTables();
    void createTableVersion();
    void createTableLinks();
    void clearTableLinks();

    void buildLinksForDirPath(const QString &dir);

    QSqlDatabase db_;
};

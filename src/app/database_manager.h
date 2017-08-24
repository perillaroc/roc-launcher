#pragma once

#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QVector>

#include "link.h"

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

Q_DECLARE_METATYPE(Link)

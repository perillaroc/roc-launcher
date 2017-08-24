#include "database_manager.h"

#include <QCoreApplication>
#include <QFileInfo>
#include <QDirIterator>
#include <QProcessEnvironment>
#include <QDateTime>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QtDebug>

DatabaseManager::DatabaseManager(QObject *parent):
    QObject(parent)
{

}

void DatabaseManager::initDatabase()
{
    QString application_dir = QCoreApplication::applicationDirPath();
    QString data_dir_path = application_dir + "/../data";

    QString db_file_path = data_dir_path + "/core.db";

    QDir data_dir(data_dir_path);
    if(!data_dir.exists())
    {
        data_dir.mkpath(data_dir_path);
    }

    db_ = QSqlDatabase::addDatabase("QSQLITE");
    db_.setDatabaseName(db_file_path);
    if(!db_.open())
    {
        qCritical()<<"[DatabaseManager::initDatabase] init database failed";
        return;
    }

    checkDatabase();
}

void DatabaseManager::buildLinks()
{
    clearTableLinks();

    QStringList dir_list{
        QProcessEnvironment::systemEnvironment().value("ALLUSERSPROFILE") + "/Microsoft/Windows/Start Menu",
        QProcessEnvironment::systemEnvironment().value("APPDATA") + "/Microsoft/Windows/Start Menu"
    };

    foreach(QString dir, dir_list)
    {
        buildLinksForDirPath(dir);
    }
}

QVector<Link> DatabaseManager::queryLinks(const QString &str)
{
    QVector<Link> links;
    if(str.isEmpty())
    {
        return links;
    }
    QSqlQuery query(db_);
    QString sql = "SELECT * FROM links WHERE lower(name) LIKE :query "
            "ORDER BY (CASE WHEN lower(name) = :exact_query THEN 1 "
            "WHEN lower(name) LIKE :left_query THEN 2 ELSE 3 END)";
    query.prepare(sql);
    query.bindValue(":query", "%" + str + "%");
    query.bindValue(":exact_query", str);
    query.bindValue(":left_query", str + "%");
    if(!query.exec())
    {
        qDebug()<<"[DatabaseManager::queryLinks] db error:"<<query.lastError().driverText();
        return links;
    }

    while(query.next())
    {
        QSqlRecord record = query.record();
        int id = record.value("id").toInt();
        QString name = record.value("name").toString();
        QString location = record.value("location").toString();
        Link link;
        link.id_ = id;
        link.name_ = name;
        link.location_ = location;
        links.push_back(link);
    }

    return links;
}

void DatabaseManager::checkDatabase()
{
    // check version
    QSqlQuery query(db_);
    query.exec("SELECT name FROM sqlite_master WHERE type='table' AND name='version'");
    if(!query.next())
    {
        // create version
        createTableVersion();
    }

    // check links
    query.exec("SELECT name FROM sqlite_master WHERE type='table' AND name='links'");
    if(!query.next())
    {
        //create links
        createTableLinks();
    }
}

void DatabaseManager::createTableVersion()
{
    QSqlQuery query(db_);
    QString sql = "CREATE TABLE `version` (`id` INTEGER NOT NULL, `version` TEXT NOT NULL UNIQUE, PRIMARY KEY(`id`));";
    if(query.exec(sql))
    {
        qDebug()<<"[DatabaseManager::createTableVersion] create table version successfully.";
    }
    else
    {
        qCritical()<<"[DatabaseManager::createTableVersion] create table failed."<<db_.lastError().driverText();
    }

    QString version = "0.1.0";
    sql = "INSERT INTO version (id, version) VALUES (:id, :version)";
    query.prepare(sql);
    query.bindValue(":id", 1);
    query.bindValue(":version", version);
    if(!query.exec())
    {
        qCritical()<<"[DatabaseManager::createTableVersion] create table failed."<<db_.lastError().driverText();
    }
}

void DatabaseManager::createTableLinks()
{
    QSqlQuery query;
    QString sql = "CREATE TABLE `links` ("
                  "`id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
                  "`location` TEXT NOT NULL UNIQUE,"
                  "`name` TEXT NOT NULL,"
                  "`icon` TEXT,"
                  "`update_time` TEXT,"
                  "`visit` INTEGER"
                  ");";
    if(query.exec(sql))
    {
        qDebug()<<"[DatabaseManager::createTableLinks] create table links successfully.";
    }
    else
    {
        qCritical()<<"[DatabaseManager::createTableLinks] create table failed."<<db_.lastError().driverText();
    }
}

void DatabaseManager::clearTableLinks()
{
    QSqlQuery query;
    QString sql = "DELETE FROM links;";
    if(query.exec(sql))
    {
//        qDebug()<<"[DatabaseManager::clearTableLinks] clear table links successfully.";
    }
    else
    {
        qCritical()<<"[DatabaseManager::clearTableLinks] clear table failed."<<db_.lastError().driverText();
    }
}

void DatabaseManager::buildLinksForDirPath(const QString &dir)
{
    QStringList filters{"*.lnk"};
    QDirIterator it(dir, filters, QDir::Files, QDirIterator::Subdirectories);
    db_.transaction();
    while (it.hasNext()) {
        QString item = it.next();
        QFileInfo file_info{item};
        QString file_name = file_info.baseName();
        QString file_location = file_info.absoluteFilePath();
        QDateTime now = QDateTime::currentDateTime();
        QString update_time = now.toString("yyyy-MM-dd hh:mm:ss");
//        qDebug()<<file_name<<file_location<<update_time;

        QSqlQuery query(db_);
        QString sql = "INSERT INTO links (location, name, update_time, visit) VALUES "
                      "(:location, :name, :update_time, :visit);";
        query.prepare(sql);
        query.bindValue(":location", file_location);
        query.bindValue(":name", file_name);
        query.bindValue(":update_time", update_time);
        query.bindValue(":visit", 0);
        if(query.exec())
        {
            // qDebug()<<"[DatabaseManager::buildLinksForDirPath] insert successfully.";
        }
        else
        {
            qCritical()<<"[DatabaseManager::buildLinksForDirPath] insert table failed."
                      <<query.lastError().driverText() \
                      <<query.lastError().databaseText();
        }
    }
    db_.commit();
}

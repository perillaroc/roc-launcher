#include "database_manager.h"

#include <QCoreApplication>
#include <QFileInfo>
#include <QDir>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
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

void DatabaseManager::checkDatabase()
{
    // check version
    QSqlQuery query;
    query.exec("SELECT name FROM sqlite_master WHERE type='table' AND name='version'");
    if(!query.next())
    {
        // create version
        createTableVersion();
    }
}

void DatabaseManager::createTableVersion()
{
    QSqlQuery query;
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

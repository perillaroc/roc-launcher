#pragma once
#include <QString>
#include <QIcon>

struct Link{
    int id_;
    QString name_;
    QString location_;
    QPixmap icon_;

    bool isEmpty() const
    {
        return location_.isEmpty();
    }
};


#pragma once
#include <QString>

struct Link{
    int id_;
    QString name_;
    QString location_;
    QString icon_path_;

    bool isEmpty()
    {
        return location_.isEmpty();
    }
};


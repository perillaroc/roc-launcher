#pragma once

#include <QWidget>

namespace Ui {
class LauncherWidget;
}

class LauncherWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LauncherWidget(QWidget *parent = 0);
    ~LauncherWidget();

private:
    Ui::LauncherWidget *ui;
};

#endif // LAUNCHER_WIDGET_H

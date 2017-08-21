#pragma once

#include <QWidget>
#include <QPointer>
#include <QSystemTrayIcon>

QT_BEGIN_NAMESPACE
class QMenu;
QT_END_NAMESPACE

namespace Ui {
class LauncherWidget;
}

class LauncherWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LauncherWidget(QWidget *parent = 0);
    ~LauncherWidget();

protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void slotSystemTrayIconActivated(QSystemTrayIcon::ActivationReason reason);

private:
    void createSystemTray();
    void setupActions();

    Ui::LauncherWidget *ui;

    QPointer<QMenu> system_tray_menu_;
    QPointer<QSystemTrayIcon> system_tray_icon_;
};

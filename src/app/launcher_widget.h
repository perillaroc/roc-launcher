#pragma once

#include <QWidget>
#include <QPointer>
#include <QSystemTrayIcon>

QT_BEGIN_NAMESPACE
class QMenu;
class QStandardItemModel;
QT_END_NAMESPACE

class DatabaseManager;

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

    void slotInputTextChanged(const QString &text);

private:
    void createSystemTray();
    void setupActions();

    void showSystemTrayContextMenu();

    Ui::LauncherWidget *ui;

    QPointer<QMenu> system_tray_menu_;
    QPointer<QSystemTrayIcon> system_tray_icon_;

    QPointer<DatabaseManager> database_manager_;

    QPointer<QStandardItemModel> link_model_;
};

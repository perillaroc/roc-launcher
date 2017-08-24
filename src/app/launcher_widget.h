#pragma once

#include <QWidget>
#include <QPointer>
#include <QSystemTrayIcon>

#include "link.h"

QT_BEGIN_NAMESPACE
class QMenu;
class QStandardItemModel;
QT_END_NAMESPACE

class DatabaseManager;

namespace Ui {
class LauncherWidget;
}

enum LauncherItemRole{
    LinkRole = Qt::UserRole + 100
};

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
    void slotInputKeyDownPressed();
    void slotLinkClicked(const QModelIndex &index);

private:
    void createSystemTray();
    void setupActions();

    void showSystemTrayContextMenu();

    void selectLink(const Link &link);
    void selectLink();
    void openLink(const Link &link);

    Ui::LauncherWidget *ui;

    QPointer<QMenu> system_tray_menu_;
    QPointer<QSystemTrayIcon> system_tray_icon_;

    QPointer<DatabaseManager> database_manager_;

    QPointer<QStandardItemModel> link_model_;
    Link current_link_;
};

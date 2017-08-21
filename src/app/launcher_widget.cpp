#include "launcher_widget.h"
#include "ui_launcher_widget.h"

#include <QMenu>
#include <QCloseEvent>

LauncherWidget::LauncherWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LauncherWidget)
{
    ui->setupUi(this);
    createSystemTray();
    setupActions();

    system_tray_icon_->show();
    connect(system_tray_icon_, &QSystemTrayIcon::activated, this, &LauncherWidget::slotSystemTrayIconActivated);
}

LauncherWidget::~LauncherWidget()
{
    delete ui;
}

void LauncherWidget::closeEvent(QCloseEvent *event)
{
    if(system_tray_icon_->isVisible())
    {
        hide();
        event->ignore();
    }
}

void LauncherWidget::slotSystemTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        showNormal();
        break;
    default:
        break;
    }
}

void LauncherWidget::createSystemTray()
{
    system_tray_menu_ = new QMenu(this);
    system_tray_menu_->addAction(ui->action_show);
    system_tray_menu_->addAction(ui->action_option);
    system_tray_menu_->addSeparator();
    system_tray_menu_->addAction(ui->action_exit);

    system_tray_icon_ = new QSystemTrayIcon(this);
    system_tray_icon_->setIcon(this->windowIcon());
    system_tray_icon_->setContextMenu(system_tray_menu_);
}

void LauncherWidget::setupActions()
{
    connect(ui->action_exit, &QAction::triggered, [=](){
        QCoreApplication::exit();
    });
}

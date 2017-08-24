#include "launcher_widget.h"
#include "ui_launcher_widget.h"

#include <QMenu>
#include <QCloseEvent>
#include <QCoreApplication>
#include <QStandardItemModel>

#include "database_manager.h"

LauncherWidget::LauncherWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LauncherWidget),
    database_manager_{new DatabaseManager{this}},
    link_model_{new QStandardItemModel{this}}
{
    ui->setupUi(this);
    ui->link_view->setModel(link_model_);
    ui->link_view->hide();
    createSystemTray();
    setupActions();

    system_tray_icon_->show();
    connect(system_tray_icon_, &QSystemTrayIcon::activated, this, &LauncherWidget::slotSystemTrayIconActivated);

    connect(ui->input_edit, &QLineEdit::textChanged, this, &LauncherWidget::slotInputTextChanged);

//    QString application_dir = QCoreApplication::applicationDirPath();
//    QString data_dir = application_dir + "/../data";

    database_manager_->initDatabase();
    database_manager_->buildLinks();
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
    case QSystemTrayIcon::Context:
        showSystemTrayContextMenu();
        break;
    default:
        break;
    }
}

void LauncherWidget::slotInputTextChanged(const QString &text)
{
    QVector<Link> links = database_manager_->queryLinks(text);
    link_model_->clear();
    if(links.isEmpty())
    {
        ui->link_view->hide();
        return;
    }

    QStandardItem *root = link_model_->invisibleRootItem();
    foreach(Link link, links)
    {
        QStandardItem *item = new QStandardItem{link.name_};
        root->appendRow(item);
    }
    ui->link_view->show();
    ui->name_label->setText(root->child(0)->text());
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

    // NOTE: This is a bug in Windows 10. Context menu will show at the right bottom corner
    // no matter where the mouse is. show we should show the menu ourselves.
    // see slotSystemTrayIconActivated and showSystemTrayContextMenu
//    system_tray_icon_->setContextMenu(system_tray_menu_);
}

void LauncherWidget::setupActions()
{
    connect(ui->action_exit, &QAction::triggered, [=](){
        QCoreApplication::exit();
    });
}

void LauncherWidget::showSystemTrayContextMenu()
{
    system_tray_menu_->exec(QCursor::pos());
}

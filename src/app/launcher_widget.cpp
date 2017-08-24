#include "launcher_widget.h"
#include "ui_launcher_widget.h"

#include "database_manager.h"
#include "lnk_tool.h"

#include <QMenu>
#include <QCloseEvent>
#include <QCoreApplication>
#include <QStandardItemModel>
#include <QFileInfo>
#include <QProcess>
#include <QtConcurrent>

#include <QtDebug>

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
    connect(ui->input_edit, &QLineEdit::returnPressed, [=](){
        openLink(current_link_);
    });
    connect(ui->input_edit, &InputLinkLineEdit::signalKeyDownPressed, this, &LauncherWidget::slotInputKeyDownPressed);
    connect(ui->input_edit, &InputLinkLineEdit::signalKeyEscPressed, [=](){
        if(system_tray_icon_->isVisible())
        {
            hide();
        }
    });
    connect(ui->link_view, &QListView::activated, this, &LauncherWidget::slotLinkClicked);
    connect(ui->link_view, &QListView::pressed, this, &LauncherWidget::slotLinkClicked);

//    QString application_dir = QCoreApplication::applicationDirPath();
//    QString data_dir = application_dir + "/../data";

    QtConcurrent::run([=](){
        qDebug()<<"[LauncherWidget] run begin";
        database_manager_->initDatabase();
        database_manager_->buildLinks();
        qDebug()<<"[LauncherWidget] run end";
    });
    qDebug()<<"[LauncherWidget] create end.";
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
        ui->link_view->hide();
        ui->input_edit->setFocus();
        ui->input_edit->selectAll();
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
        selectLink();
        return;
    }

    QStandardItem *root = link_model_->invisibleRootItem();
    foreach(Link link, links)
    {
        QStandardItem *item = new QStandardItem{link.name_};
        item->setData(QVariant::fromValue<Link>(link), LauncherItemRole::LinkRole);
        item->setData(link.location_, Qt::ToolTipRole);
        item->setIcon(QIcon(link.icon_));
        root->appendRow(item);
    }
    ui->link_view->show();

    selectLink(links.first());
}

void LauncherWidget::slotInputKeyDownPressed()
{
    ui->link_view->setFocus();
}

void LauncherWidget::slotLinkClicked(const QModelIndex &index)
{
    QStandardItem *item = link_model_->itemFromIndex(index);
    Link link = item->data(LauncherItemRole::LinkRole).value<Link>();
    qDebug()<<"[LauncherWidget::slotLinkSelected] link selected:"<< link.location_;

    selectLink(link);
    openLink(link);
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

void LauncherWidget::selectLink(const Link &link)
{
   if(link.isEmpty())
    {
        selectLink();
        return;
    }
    current_link_ = link;
    ui->name_label->setText(link.name_);
    ui->icon_label->setPixmap(link.icon_);
}

void LauncherWidget::selectLink()
{
    current_link_ = Link();
    ui->name_label->setText("");
    ui->icon_label->setPixmap(QPixmap());
}

void LauncherWidget::openLink(const Link &link)
{
    if(system_tray_icon_->isVisible())
    {
        hide();
    }

    if(!QProcess::startDetached("cmd.exe", QStringList()<<"/C"<<link.location_))
    {
        qWarning()<<"[LauncherWidget::slotLinkSelected] start process failed";
    }
}

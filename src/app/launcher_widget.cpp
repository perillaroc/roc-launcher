#include "launcher_widget.h"
#include "ui_launcher_widget.h"

LauncherWidget::LauncherWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LauncherWidget)
{
    ui->setupUi(this);
}

LauncherWidget::~LauncherWidget()
{
    delete ui;
}

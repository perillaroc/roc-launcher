#include "link_list_view.h"
#include <QKeyEvent>

LinkListView::LinkListView(QWidget *parent):
    QListView{parent}
{

}

void LinkListView::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Up || event->key() == Qt::Key_Down)
    {
        QListView::keyPressEvent(event);
        return;
    }
    else
    {
        emit signalNotUpDownKeyPressed(event);
        event->ignore();
    }
}

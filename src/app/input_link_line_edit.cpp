#include "input_link_line_edit.h"

#include <QKeyEvent>

InputLinkLineEdit::InputLinkLineEdit(QWidget *parent):
    QLineEdit{parent}
{

}

void InputLinkLineEdit::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Down)
    {
        emit signalKeyDownPressed();
        e->ignore();
    }
    else if(e->key() == Qt::Key_Escape)
    {
        emit signalKeyEscPressed();
        e->ignore();
    }
    else
    {
        QLineEdit::keyPressEvent(e);
    }
}

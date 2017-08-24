#pragma once
#include <QLineEdit>


class InputLinkLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    InputLinkLineEdit(QWidget *parent);

signals:
    void signalKeyDownPressed();

protected:
    void keyPressEvent(QKeyEvent *e) override;
};

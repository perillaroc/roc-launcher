#pragma
#include <QListView>

class LinkListView : public QListView
{
    Q_OBJECT
public:
    LinkListView(QWidget *parent=nullptr);

signals:
    void signalUnhandledKeyPressed(QKeyEvent *event);

protected:
    void keyPressEvent(QKeyEvent *event) override;
};

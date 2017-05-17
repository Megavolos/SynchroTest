#ifndef DIFFGRAPHWINDOW_H
#define DIFFGRAPHWINDOW_H

#include <QWidget>
namespace Ui {
class diffGraphWindow;
}

class diffGraphWindow : public QWidget
{
    Q_OBJECT

public:
    explicit diffGraphWindow(QWidget *parent = 0);
    ~diffGraphWindow();

private:
    Ui::diffGraphWindow *ui;
};

#endif // DIFFGRAPHWINDOW_H

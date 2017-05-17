#include "diffgraphwindow.h"
#include "ui_diffgraphwindow.h"

diffGraphWindow::diffGraphWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::diffGraphWindow)
{
    ui->setupUi(this);
}

diffGraphWindow::~diffGraphWindow()
{
    delete ui;
}

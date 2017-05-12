#include "writetofiledialog.h"
#include "ui_writetofiledialog.h"

writeToFileDialog::writeToFileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::writeToFileDialog)
{
    ui->setupUi(this);
}

writeToFileDialog::~writeToFileDialog()
{
    delete ui;
}



bool writeToFileDialog::getWriteEnabledState()
{
    return writeEnabled;
}
QString writeToFileDialog::getPath()
{
    return path;
}


void writeToFileDialog::on_writeAllowed_toggled(bool checked)
{
    writeEnabled=checked;
}

void writeToFileDialog::on_openFolder_clicked()
{
    path=QFileDialog::getExistingDirectory(this, tr("Open Directory"),"/home",QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);

}

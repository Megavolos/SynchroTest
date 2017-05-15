#include "writetofiledialog.h"
#include "ui_writetofiledialog.h"

writeToFileDialog::writeToFileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::writeToFileDialog)
{
    ui->setupUi(this);
    file = new QFile();
    writeEnabled = true;
}

writeToFileDialog::~writeToFileDialog()
{
    delete ui;
    delete file;
}



bool writeToFileDialog::getWriteEnabledState()
{
    return writeEnabled;
}

void writeToFileDialog::setWriteEnabledState(bool state)
{
    writeEnabled=state;
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
    QString dir=QFileDialog::getExistingDirectory(this, tr("Open Directory"),"/home",QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
    for (int i=0; i<100; i++)
    {
        path=dir+"/"+QString::number(i)+".dat";
        if (!fileExists(path))
        {
            file->setFileName(path);

            break;
        }

    }

}
QFile* writeToFileDialog::getFileDevice()
{
    return file;
}

bool writeToFileDialog::fileExists(QString path) {
    QFileInfo check_file(path);
    // check if file exists and if yes: Is it really a file and no directory?
    if (check_file.exists() && check_file.isFile())
    {
        return true;
    }
    else
    {
        return false;
    }
}

#ifndef WRITETOFILEDIALOG_H
#define WRITETOFILEDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QFile>
#include <QFileInfo>
namespace Ui {
class writeToFileDialog;
}

class writeToFileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit writeToFileDialog(QWidget *parent = 0);
    ~writeToFileDialog();

    bool getWriteEnabledState();
    QString getPath();
    bool fileExists(QString path);
    QFile* getFileDevice();
    void setWriteEnabledState(bool state);

private slots:


    void on_writeAllowed_toggled(bool checked);

    void on_openFolder_clicked();

private:
    Ui::writeToFileDialog *ui;
    bool writeEnabled;
    QString path;
    QFile* file;
};

#endif // WRITETOFILEDIALOG_H

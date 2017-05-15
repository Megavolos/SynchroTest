#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    writeDialog = new writeToFileDialog(this);
    setupCOMport();
    setupQwt();

}

MainWindow::~MainWindow()
{
    delete ui;
    delete writeDialog;
}

void MainWindow::on_openComPortSettingsDialog_triggered()
{
    ComPortSettingsDialog comPortSettingsDialog(this);
    comPortSettingsDialog.exec();
    currentComPortName=comPortSettingsDialog.getSelectedPortName();
    savesettings(currentComPortName,115200,8,0,0,0);

}

void MainWindow::on_openWriteToFileDialog_triggered()
{
    writeDialog->exec();
    pathToWriteFiles=writeDialog->getPath();
    writeDialog->getFileDevice()->setFileName(pathToWriteFiles);


}

void MainWindow::setupCOMport()
{
    QThread *thread_New = new QThread;
    Port *PortNew = new Port();//Создаем обьект по классу
    PortNew->moveToThread(thread_New);//помешаем класс  в поток
    PortNew->thisPort.moveToThread(thread_New);//Помещаем сам порт в поток
    connect(PortNew, SIGNAL(error_(QString)), this, SLOT(error(QString)));//Лог ошибок
    connect(thread_New, SIGNAL(started()), PortNew, SLOT(process_Port()));//Переназначения метода run
    connect(PortNew, SIGNAL(finished_Port()), thread_New, SLOT(quit()));//Переназначение метода выход
    connect(thread_New, SIGNAL(finished()), PortNew, SLOT(deleteLater()));//Удалить к чертям поток
    connect(PortNew, SIGNAL(finished_Port()), thread_New, SLOT(deleteLater()));//Удалить к чертям поток
    connect(this,SIGNAL(savesettings(QString,int,int,int,int,int)),PortNew,SLOT(Write_Settings_Port(QString,int,int,int,int,int)));//Слот - ввод настроек!
    connect(ui->actionStart, SIGNAL(triggered()),PortNew,SLOT(ConnectPort()));
    connect(ui->actionStart, SIGNAL(triggered()),this,SLOT(startWriteToFile()));
    connect(ui->actionStop, SIGNAL(triggered()),PortNew,SLOT(DisconnectPort()));
    connect(ui->actionStop, SIGNAL(triggered()),this,SLOT(stopWtiteToFile()));

    connect(PortNew, SIGNAL(outPort(QByteArray)), this, SLOT(Print(QByteArray)));//Лог ошибок
    connect(this,SIGNAL(writeData(QByteArray)),PortNew,SLOT(WriteToPort(QByteArray)));
    thread_New->start();
    ui->actionStop->setChecked(true);
    ui->actionStop->setEnabled(false);
}
void MainWindow::error (QString err)
{
    QMessageBox message(this);
    message.setText(err);

    if (!err.contains("Открыт", Qt::CaseInsensitive))
    {
        if (!err.contains("Закрыт", Qt::CaseInsensitive)) message.exec();
        ui->actionStart->setChecked(false);
        stopWtiteToFile();
        ui->actionStop->setChecked(true);
        ui->actionStop->setEnabled(false);
        ui->actionStart->setEnabled(true);


        return;
    }
    ui->actionStop->setChecked(false);
    ui->actionStop->setEnabled(true);

    ui->actionStart->setChecked(true);
    ui->actionStart->setEnabled(false);
}

void MainWindow::startWriteToFile()
{
    if (!writeDialog->getPath().isEmpty() && writeDialog->getWriteEnabledState())
    {
        if ( writeDialog->getFileDevice()->open(QIODevice::ReadWrite) )
        {
            writeDialog->setWriteEnabledState(true);
            datastream.setDevice(writeDialog->getFileDevice());
        }
        else
        {
            qDebug("Error opening file");
            writeDialog->setWriteEnabledState(false);

        }
    }
    else
    {
        qDebug("Path is empty or Write disabled");
    }

}
void MainWindow::stopWtiteToFile()
{
    writeDialog->getFileDevice()->close();
}


void MainWindow::setupQwt()
{
    ui->qwtPlot->setCanvasBackground(Qt::black);
    ui->qwtPlot_2->setCanvasBackground(Qt::black);
    QwtPlotMagnifier *zoom_x = new QwtPlotMagnifier( ui->qwtPlot->canvas());
    QwtPlotMagnifier *zoom_y = new QwtPlotMagnifier( ui->qwtPlot->canvas() );
    QwtPlotMagnifier *zoom_x2 = new QwtPlotMagnifier( ui->qwtPlot_2->canvas());
    QwtPlotMagnifier *zoom_y2 = new QwtPlotMagnifier( ui->qwtPlot_2->canvas() );
    zoom_x->setMouseButton(Qt::MidButton);
    zoom_x->setWheelModifiers(Qt::ShiftModifier);
    zoom_x->setAxisEnabled(QwtPlot::xBottom, true);
    zoom_x->setAxisEnabled(QwtPlot::yRight ,false);
    zoom_x->setAxisEnabled(QwtPlot::yLeft ,false);
    zoom_y->setMouseButton(Qt::MidButton);
    zoom_y->setWheelModifiers(Qt::ControlModifier);
    zoom_y->setAxisEnabled(QwtPlot::xBottom,false);
    zoom_y->setAxisEnabled(QwtPlot::yRight,true);
    zoom_y->setAxisEnabled(QwtPlot::yLeft,true);
    zoom_x2->setMouseButton(Qt::MidButton);
    zoom_x2->setWheelModifiers(Qt::ShiftModifier);
    zoom_x2->setAxisEnabled(QwtPlot::xBottom, true);
    zoom_x2->setAxisEnabled(QwtPlot::yRight ,false);
    zoom_x2->setAxisEnabled(QwtPlot::yLeft ,false);
    zoom_y2->setMouseButton(Qt::MidButton);
    zoom_y2->setWheelModifiers(Qt::ControlModifier);
    zoom_y2->setAxisEnabled(QwtPlot::xBottom,false);
    zoom_y2->setAxisEnabled(QwtPlot::yRight,true);
    zoom_y2->setAxisEnabled(QwtPlot::yLeft,true);

    QwtPlotPanner *d_panner = new QwtPlotPanner(ui->qwtPlot->canvas() );
    d_panner->setMouseButton( Qt::RightButton );
    QwtPlotPanner *d_panner2 = new QwtPlotPanner(ui->qwtPlot_2->canvas() );
    d_panner2->setMouseButton( Qt::RightButton );
    curvesMems.append(new QwtPlotCurve("MEMS1"));
    curvesMems.append(new QwtPlotCurve("MEMS2"));
    curvesMems.at(0)->setPen(QPen(Qt::red,2,Qt::SolidLine));
    curvesMems.at(1)->setPen(QPen(Qt::green ,2,Qt::SolidLine));
    curvesPiezo.append(new QwtPlotCurve("PIEZO1"));
    curvesPiezo.append(new QwtPlotCurve("PIEZO1"));
    curvesPiezo.at(0)->setPen(QPen(Qt::red,2,Qt::SolidLine));
    curvesPiezo.at(1)->setPen(QPen(Qt::green ,2,Qt::SolidLine));

    curvesMems.at(0)->setSamples(x,*MEMS0.data());
    curvesMems.at(1)->setSamples(x,*MEMS1.data());
    curvesPiezo.at(0)->setSamples(x,*PIEZO0.data());
    curvesPiezo.at(1)->setSamples(x,*PIEZO1.data());
    curvesMems.at(0)->attach(ui->qwtPlot);
    curvesMems.at(1)->attach(ui->qwtPlot);
    curvesPiezo.at(0)->attach(ui->qwtPlot_2);
    curvesPiezo.at(1)->attach(ui->qwtPlot_2);
    MEMS0.setQwtPlotPointer(ui->qwtPlot);
    MEMS1.setQwtPlotPointer(ui->qwtPlot);
    PIEZO0.setQwtPlotPointer(ui->qwtPlot_2);
    PIEZO1.setQwtPlotPointer(ui->qwtPlot_2);

    ui->qwtPlot->setAxisAutoScale(QwtPlot::xBottom,true);
    ui->qwtPlot_2->setAxisAutoScale(QwtPlot::xBottom,true);



}

void MainWindow::Print(QByteArray data)
{
    bool ok;
    qreal MEMS0_angle,MEMS1_angle,PIEZO0_angle,PIEZO1_angle,MEMS_diff,PIEZO_diff;
    if (ui->actionStart->isChecked() &&startRecieved<4)
    {
        for (int i=0; i<data.size();i++)
        {
            if (data.at(i)==0x11 || data.at(i)==0x12 || data.at(i)==0x13 || data.at(i)==0x14)
            {
                startRecieved++;
                data.remove(i,1);
                i--;
                if (startRecieved==4) break;
            }

        }

    }
    if (startRecieved==4)
    {
        for (int i=0; i<data.size();i++)
        {
            if (writeDialog->getWriteEnabledState()) datastream<<data.at(i);
            switch (channelSwitch)  {
            case 0:
                MEMS0.addSample((quint8)data.at(i));
                if (MEMS0.isSignalPresent())
                {
                    if (MEMS0.isSignalEnd())
                    {
                        MEMS0.filter(ui->lpfMemsEdit->text());
                        curvesMems.at(0)->setSamples(*MEMS0.getTime(),*MEMS0.data());
                        MEMS0_angle=MEMS0.measure();
                        ui->MEMS0_angle_label->setText(QString::number(MEMS0_angle,'f',2));
                    }
                }
                break;
            case 1:
                PIEZO0.addSample((quint8)data.at(i));
                if (PIEZO0.isSignalPresent())
                {
                    if (PIEZO0.isSignalEnd())
                    {
                        PIEZO0.filter(ui->lpfPiezoEdit->text());
                        if(ui->IntegratePiezo->isChecked())
                        {
                            PIEZO0.integrate();
                        }
                        curvesPiezo.at(0)->setSamples(*PIEZO0.getTime(),*PIEZO0.data());
                        PIEZO0_angle=PIEZO0.measure();
                        ui->PIEZO0_angle_label->setText(QString::number(PIEZO0_angle,'f',2));
                    }
                }
                break;
            case 2:
                MEMS1.addSample((quint8)data.at(i));
                if (MEMS1.isSignalPresent())
                {
                    if (MEMS1.isSignalEnd())
                    {
                        MEMS1.filter(ui->lpfMemsEdit->text());
                        curvesMems.at(1)->setSamples(*MEMS1.getTime(),*MEMS1.data());
                        MEMS1_angle=MEMS1.measure();
                        ui->MEMS1_angle_label->setText(QString::number(MEMS1_angle,'f',2));
                        MEMS_diff=MEMS0_angle-MEMS1_angle;
                        ui->MEMS_diff_label->setText((QString::number(MEMS_diff,'f',2)));
                    }
                }
                break;
            case 3:
                PIEZO1.addSample((quint8)data.at(i));
                if (PIEZO1.isSignalPresent())
                {
                    if (PIEZO1.isSignalEnd())
                    {
                        PIEZO1.filter(ui->lpfPiezoEdit->text());
                        if(ui->IntegratePiezo->isChecked())
                        {
                            PIEZO1.integrate();
                        }
                        curvesPiezo.at(1)->setSamples(*PIEZO1.getTime(),*PIEZO1.data());
                        PIEZO1_angle=PIEZO1.measure();
                        ui->PIEZO1_angle_label->setText(QString::number(PIEZO1_angle,'f',2));
                        PIEZO_diff=PIEZO0_angle-PIEZO1_angle;
                        ui->PIEZO_diff_label->setText((QString::number(PIEZO_diff,'f',2)));


                    }
                }
                break;
            }
            channelSwitch++;

            if (channelSwitch==4) channelSwitch=0;
        }

    }
   // ui->consol->textCursor().insertText(data+'\r'); // Вывод текста в консоль
   // ui->consol->moveCursor(QTextCursor::End);//Scroll
}



void MainWindow::on_memsYMax_lineEdit_returnPressed()
{
    bool ok;
    qreal min_y, newmax;
    QwtInterval interval;

    interval = ui->qwtPlot->axisScaleDiv(QwtPlot::yLeft).interval();
    min_y=(int)interval.minValue();
    newmax = ui->memsYMax_lineEdit->text().toDouble(&ok);
    ui->qwtPlot->setAxisScale(QwtPlot::yLeft,min_y,newmax,ui->MEMSStep_lineEdit->text().toDouble(&ok) );
    ui->qwtPlot->replot();
}


void MainWindow::on_piezoYMax_lineEdit_returnPressed()
{
    bool ok;
    qreal min_y, newmax;
    QwtInterval interval;

    interval = ui->qwtPlot_2->axisScaleDiv(QwtPlot::yLeft).interval();
    min_y=(int)interval.minValue();
    newmax = ui->piezoYMax_lineEdit->text().toDouble(&ok);
    ui->qwtPlot_2->setAxisScale(QwtPlot::yLeft,min_y,newmax,ui->PIEZOStep_lineEdit->text().toDouble(&ok) );
    ui->qwtPlot_2->replot();
}

void MainWindow::on_PIEZOStep_lineEdit_returnPressed()
{
    on_piezoYMax_lineEdit_returnPressed();
}

void MainWindow::on_MEMSStep_lineEdit_returnPressed()
{
    on_memsYMax_lineEdit_returnPressed();

}
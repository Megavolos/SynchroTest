#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
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
    writeToFileDialog writeDialog(this);
    writeDialog.exec();
    pathToWriteFiles=writeDialog.getPath();

}

void MainWindow::setupCOMport()
{
    QThread *thread_New = new QThread;
    Port *PortNew = new Port();//Создаем обьект по классу
    PortNew->moveToThread(thread_New);//помешаем класс  в поток
    PortNew->thisPort.moveToThread(thread_New);//Помещаем сам порт в поток
    connect(PortNew, SIGNAL(error_(QString)), this, SLOT(Print(QString)));//Лог ошибок
    connect(thread_New, SIGNAL(started()), PortNew, SLOT(process_Port()));//Переназначения метода run
    connect(PortNew, SIGNAL(finished_Port()), thread_New, SLOT(quit()));//Переназначение метода выход
    connect(thread_New, SIGNAL(finished()), PortNew, SLOT(deleteLater()));//Удалить к чертям поток
    connect(PortNew, SIGNAL(finished_Port()), thread_New, SLOT(deleteLater()));//Удалить к чертям поток
    connect(this,SIGNAL(savesettings(QString,int,int,int,int,int)),PortNew,SLOT(Write_Settings_Port(QString,int,int,int,int,int)));//Слот - ввод настроек!
    connect(ui->actionStart, SIGNAL(triggered(bool)),PortNew,SLOT(ConnectPort()));
    connect(ui->actionStop, SIGNAL(triggered(bool)),PortNew,SLOT(DisconnectPort()));
    connect(PortNew, SIGNAL(outPort(QByteArray)), this, SLOT(Print(QByteArray)));//Лог ошибок
    connect(this,SIGNAL(writeData(QByteArray)),PortNew,SLOT(WriteToPort(QByteArray)));
    thread_New->start();
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

}

void MainWindow::Print(QByteArray data)
{
    bool ok;
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
            datastream<<data.at(i);
            switch (channelSwitch)  {
            case 0:
                MEMS0.addSample((quint8)data.at(i));
                if (MEMS0.isSignalPresent())
                {
                    if (MEMS0.isSignalEnd())
                    {
                        MEMS0.filter(ui->lpfMemsEdit->text());
                        curvesMems.at(0)->setSamples(*MEMS0.getTime(),*MEMS0.data());
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


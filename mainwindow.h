#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QDataStream>
#include <qwt_plot_canvas.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_panner.h>
#include <qwt_scale_div.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_picker.h>
#include <qwt_picker_machine.h>
#include <QtSerialPort/QSerialPortInfo>
#include <qmath.h>
#include <qwt_painter.h>
#include <comportsettingsdialog.h>
#include <writetofiledialog.h>
#include <port.h>
#include <qthread.h>
#include <sensor.h>
#include <qdebug.h>
#include <qmessagebox.h>
#include <diagramsettingswindow.h>
#include <diffgraphwindow.h>
#include <QSettings>
#include <zeroleveldialog.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString currentComPortName;
    QString pathToWriteFiles;
    QVector<QwtPlotCurve*> curvesPiezo;
    QVector<QwtPlotCurve*> curvesMems;
    QwtPlotMagnifier *magnifier;        //зумер
    QwtPlotMagnifier *zoom_x, *zoom_x1,*zoom_x2;
    QwtPlotMagnifier *zoom_y, *zoom_y1, *zoom_y2;
    QwtPlotPanner *d_panner,*d_panner1,*d_panner2;            //перемещатель
    QwtPlotGrid *grid,*grid1,*grid2;
    QVector<double> x;
    Sensor MEMS0,MEMS1,PIEZO0,PIEZO1;
    QFile file;
    QDataStream datastream;
    QVector<qreal> filter(QVector<qreal> *in, qreal coeff );
    QVector<qreal> integrate(QVector<qreal> *in, qreal zeroLevel);
    Port *PortNew;
    uchar startRecieved;
    uchar channelSwitch;
    qint32 receivedBytes;
    writeToFileDialog* writeDialog;
    quint8 resizeCounter;
    DiagramSettingsWindow* diagramSettings;
    diffGraphWindow*  diffWindow;
    zeroLevelDialog* zeroLevelWindow;
    QSettings *settings;
    bool syncronized;

    void setupCOMport(void);
    void setupQwt(void);
    void sync(QByteArray *data);
private slots:
    void on_openComPortSettingsDialog_triggered();

    void on_openWriteToFileDialog_triggered();
    void startWriteToFile();
    void stopWtiteToFile();
    void error (QString err);
    void Print(QByteArray data);


    void on_memsYMax_lineEdit_returnPressed();



    void on_piezoYMax_lineEdit_returnPressed();

    void on_PIEZOStep_lineEdit_returnPressed();

    void on_MEMSStep_lineEdit_returnPressed();

    void on_lpfPiezoEdit_returnPressed();

    void on_lpfMemsEdit_returnPressed();

    void on_MemsLimitChagned(double limit);
    void on_PiezoLimitChagned(double limit);
    void on_PiezoStepChagned(double step);
    void on_MemsStepChagned(double step);


    void on_openZeroLevelDialog_triggered();

signals:

    void savesettings(QString name, int baudrate, int DataBits, int Parity, int StopBits, int FlowControl);
    void writeData(const QByteArray &data);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

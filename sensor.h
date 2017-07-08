#ifndef SENSOR_H
#define SENSOR_H
#include <QObject>
#include <QVector>
#include <QPoint>
#include <qwt_plot.h>
#include <qmath.h>
#include <qwt_scale_div.h>
class Sensor : public QObject
{
    Q_OBJECT
    static double levelmax,levelmin;
    static int waitPositive, waitEnd;
public:

    Sensor();
    bool isMems;
    void setStart (bool state);
    void setSamplingFrequencyAllChannels(quint32 freq);
    quint32 getSamplingFrequencyOneChannel();
    bool getStart();
    void setStartLevel (qint16 level);
    bool isSignalPresent ();
    bool isSignalEnd();
    void filter();
    void integrate();
    QVector<qreal>* getTime();
    QVector<qreal>* data();
    void setQwtPlotPointer (QwtPlot* _plot);
    void addSample(quint8 sample);
    qreal measure();
    void clear();
    void setLPFCoeff (qreal coeff);
    void clearNoSignalData();
    bool isNotFalseSignal();
    void setFalseSignalLevel(quint8 level);
    void setIsMems(bool state);
    bool getSignalEnd();
    bool memsStart;
    bool memsEnd;
    bool searchMin();
    bool searchMax();
    bool clearMems(void);
    void setSignalEnd(bool state);
public slots:
    void setFSLSlot(int arg1);
    void setLevelMin(double level);
    void setLevelMax(double level);
    void setCalibr(double value);
    void SetWaitPositive(int value);
    void SetWaitEnd(int value);
private:
    bool start,signalEnd;
    quint32 samplingFrequencyAllChannels,wait;
    quint16 samplingFrequencyOneChannel,mincount,maxcount;
    QwtPlot* plot;
    qint16 startLevel;
    QVector<qreal> _data, x;
    qreal LPFcoeff;
    qreal peak, peakPrev,min,max,minPrev,maxPrev,calibr;


    quint8 falseSignalLevel;
};

#endif // SENSOR_H

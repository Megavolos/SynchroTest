#ifndef SIGNAL_H
#define SIGNAL_H
#include <QObject>
#include <QVector>
#include <QPoint>
#include <qwt_plot.h>
#include <qmath.h>
#include <qwt_scale_div.h>
class Signal : public QObject
{
    Q_OBJECT
public:

    Signal();
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
    void setSignalEnd(bool state);
public slots:
    void setFSLSlot(int arg1);

private:
    bool start,signalEnd;
    quint32 samplingFrequencyAllChannels;
    quint16 samplingFrequencyOneChannel;
    QwtPlot* plot;
    qint16 startLevel;
    QVector<qreal> _data, x;
    qreal LPFcoeff;
    qreal peak, peakPrev,min,max,minPrev,maxPrev;
    quint8 falseSignalLevel;
};

#endif // SIGNAL_H

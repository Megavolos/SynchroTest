#ifndef SIGNAL_H
#define SIGNAL_H
#include <QVector>

class Signal
{
public:
    Signal();
    void setStart (bool state);
    void setSamplingFrequencyAllChannels(quint32 freq);
    quint32 getSamplingFrequencyOneChannel();
    bool getStart();
    void setStartLevel (qint16 level);
    bool isSignalPresent ();
    bool isSignalEnd();
    void filter(QString text );
    void integrate();
    QVector<qreal>* getTime();
    QVector<qreal>* data();

    void addSample(quint8 sample);


private:
    bool start;
    quint32 samplingFrequencyAllChannels;
    quint16 samplingFrequencyOneChannel;

    qint16 startLevel;
    QVector<qreal> _data, x;
};

#endif // SIGNAL_H

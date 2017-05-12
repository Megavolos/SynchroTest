#ifndef PORT_H
#define PORT_H
#include <QObject>
#include <QtSerialPort/QSerialPort> //Обьявляем работу с портом

struct Settings {//Структура с настройками порта
    QString name;
    qint32 baudRate;
    QSerialPort::DataBits dataBits;
    QSerialPort::Parity parity;
    QSerialPort::StopBits stopBits;
    QSerialPort::FlowControl flowControl;
};
class Port : public QObject
{
    Q_OBJECT
public:
    explicit Port(QObject *parent = 0);
    ~Port();
    QSerialPort thisPort;
    Settings SettingsPort;
    void open();
    void close();
    void sendchar(uchar data);
    bool isOpened();
signals:
    void finished_Port(); //Сигнал закрытия класса
    void error_(QString err);//Сигнал ошибок порта
    void outPort(QByteArray data); //Сигнал вывода полученных данных
public slots:
    void  DisconnectPort(); // Слот отключения порта
    void ConnectPort(void); // Слот подключения порта
    void Write_Settings_Port(QString name, int baudrate, int DataBits, int Parity, int StopBits, int FlowControl);// Слот занесение настроек порта в класс
    void process_Port(); //Тело
    void WriteToPort(QByteArray data); // Слот от правки данных в порт
private slots:
    void handleError(QSerialPort::SerialPortError error);//Слот обработки ощибок
    void ReadInPort(); //Слот чтения из порта по ReadyRead
public:
};
#endif // PORT_H

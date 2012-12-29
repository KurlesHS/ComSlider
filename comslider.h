#ifndef COMSLIDER_H
#define COMSLIDER_H

#include <QMainWindow>
#include <QtAddOnSerialPort/serialport.h>
#include <QtAddOnSerialPort/serialportinfo.h>

QT_USE_NAMESPACE_SERIALPORT

namespace Ui {
class ComSlider;
}

class ComSlider : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit ComSlider(QWidget *parent = 0);
    ~ComSlider();

private slots:
    void onSpinBoxValueChanged(int value);
    void onSpinBoxRangeToValueChanged(int value);
    void onSpinBoxRangeFromValueChanged(int value);
    void onSilderValueChanged(int value);
    void onOpenPortButtonPushed();
    void onClosePortButtonPushed();
    void onPlusButtonPushed();
    void onMinusButtonPushed();
    void onReadyRead();

private:
    void writeInPort(const int &value);
    
private:
    Ui::ComSlider *ui;
    SerialPort *sp;
    quint16 valueFromPort;
    int numByte;
};

#endif // COMSLIDER_H

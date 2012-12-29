#include "comslider.h"
#include "ui_comslider.h"
#include <QDebug>

ComSlider::ComSlider(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ComSlider)
{
    ui->setupUi(this);
    ui->spinBoxRangeFrom->setRange(0, 65535);
    ui->spinBoxRangeTo->setRange(0, 65535);
    ui->spinBoxRangeFrom->setValue(0);
    ui->spinBoxRangeTo->setValue(65535);
    ui->spinBox->setRange(0, 65535);
    ui->horizontalSlider->setRange(0, 65535);

    sp = new SerialPort(this);
    connect(sp, SIGNAL(readyRead()), SLOT(onReadyRead()));
    connect(ui->spinBox, SIGNAL(valueChanged(int)), SLOT(onSpinBoxValueChanged(int)));
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), SLOT(onSilderValueChanged(int)));
    connect(ui->pushButtonClose, SIGNAL(clicked()), SLOT(onClosePortButtonPushed()));
    connect(ui->pushButtonOpen, SIGNAL(clicked()), SLOT(onOpenPortButtonPushed()));
    connect(ui->pushButtonMinus, SIGNAL(clicked()), SLOT(onMinusButtonPushed()));
    connect(ui->pushButtonPlus, SIGNAL(clicked()), SLOT(onPlusButtonPushed()));
    connect(ui->spinBoxRangeFrom, SIGNAL(valueChanged(int)), SLOT(onSpinBoxRangeFromValueChanged(int)));
    connect(ui->spinBoxRangeTo, SIGNAL(valueChanged(int)), SLOT(onSpinBoxRangeToValueChanged(int)));
    ui->spinBox->setValue(0);
    onSpinBoxValueChanged(0);
    foreach(const SerialPortInfo &si, SerialPortInfo::availablePorts())
        ui->comboBox->addItem(si.portName());
    ui->pushButtonClose->setEnabled(false);
    ui->pushButtonOpen->setEnabled(true);
}

ComSlider::~ComSlider()
{
    delete ui;
}

void ComSlider::onSpinBoxValueChanged(int value)
{
    ui->horizontalSlider->blockSignals(true);
    ui->horizontalSlider->setValue(value);
    ui->horizontalSlider->blockSignals(false);
    ui->pushButtonMinus->setEnabled(value != 0);
    ui->pushButtonPlus->setEnabled(value < 65535);
    writeInPort(value);

}

void ComSlider::onSpinBoxRangeToValueChanged(int value)
{
    if (value < ui->spinBoxRangeFrom->value())
    {
        value = ui->spinBoxRangeFrom->value();
        ui->spinBoxRangeTo->blockSignals(true);
        ui->spinBoxRangeTo->setValue(value);
        ui->spinBoxRangeTo->blockSignals(false);
    }
    ui->spinBox->setMaximum(value);
    ui->horizontalSlider->setMaximum(value);
}

void ComSlider::onSpinBoxRangeFromValueChanged(int value)
{
    if (value > ui->spinBoxRangeTo->value())
    {
        value = ui->spinBoxRangeTo->value();
        ui->spinBoxRangeFrom->blockSignals(true);
        ui->spinBoxRangeFrom->setValue(value);
        ui->spinBoxRangeFrom->blockSignals(false);
    }
    ui->spinBox->setMinimum(value);
    ui->horizontalSlider->setMaximum(value);
}

void ComSlider::onSilderValueChanged(int value)
{
    ui->spinBox->blockSignals(true);
    ui->spinBox->setValue(value);
    ui->spinBox->blockSignals(false);

    ui->pushButtonMinus->setEnabled(value != 0);
    ui->pushButtonPlus->setEnabled(value < 65535);
    writeInPort(value);
}

void ComSlider::onOpenPortButtonPushed()
{
    if (!sp->isOpen())
    {
        sp->setPort(ui->comboBox->currentText());
        if (sp->open(SerialPort::ReadWrite))
        {
            if (sp->setDataBits(SerialPort::Data8) &&
                    sp->setRate(9600) &&
                    sp->setStopBits(SerialPort::OneStop) &&
                    sp->setFlowControl(SerialPort::NoFlowControl) &&
                    sp->setParity(SerialPort::NoParity))
            {
                ui->pushButtonOpen->setEnabled(false);
                ui->pushButtonClose->setEnabled(true);
                valueFromPort = 0;
                numByte = 0;
                return;
            }
        }
    }
    ui->labelPortValue->setText(trUtf8("Не получилось открыть порт \"%0\"").arg(ui->comboBox->currentText()));
}

void ComSlider::onClosePortButtonPushed()
{
    if (sp->isOpen())
        sp->close();
    ui->pushButtonClose->setEnabled(false);
    ui->pushButtonOpen->setEnabled(true);
}

void ComSlider::onPlusButtonPushed()
{
    ui->spinBox->setValue(ui->spinBox->value() + 1);
}

void ComSlider::onMinusButtonPushed()
{
    ui->spinBox->setValue(ui->spinBox->value() - 1);
}

void ComSlider::onReadyRead()
{
    while(sp->bytesAvailable())
    {
        QByteArray arr;
        if (numByte == 1)
        {
            arr = sp->read(1);
            valueFromPort += (qint8)arr.at(0) * 0x100;
            numByte = 0;
        } else
        {
            arr = sp->read(2);
            if (arr.length() == 2)
            {
                valueFromPort = (qint8)arr.at(0) + qint8(arr.at(1) * 0x100);
            } else
            {
                valueFromPort = (qint8)arr.at(0);
                numByte = 1;
                return;
            }
        }
        ui->labelPortValue->setText(trUtf8("%0 - 0x%1 - младший байт (%2) - старший байт (%3) ")
                                    .arg(valueFromPort).arg(valueFromPort, 4, 16, QChar('0'))
                                    .arg(valueFromPort % 0x100).arg(valueFromPort / 0x100));
        qDebug() << valueFromPort;
    }


}

void ComSlider::writeInPort(const int &value)
{
    if (sp->isOpen())
    {
        QByteArray arr;
        arr.append((char)value % 0x100);
        arr.append((char)value / 0x100);
        sp->write(arr);
        qDebug() << "write:"  << arr ;
    }
}

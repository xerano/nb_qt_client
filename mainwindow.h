#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QSerialPort>
#include <QHttpServer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class SerialSettingsDialog;
class ReceiverThread;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void readData();
    void handleSerialError(QSerialPort::SerialPortError error);

    void volumeChanged(float value);
    void volumeDialValueChanged(int value);

    void pushButtonSelect1Pressed();
    void pushButtonSelect2Pressed();
    void pushButtonSelect3Pressed();
    void pushButtonSelect4Pressed();
    void pushButtonSelect5Pressed();

    void pushButtonPlay1Pressed();
    void pushButtonPlay2Pressed();
    void pushButtonPlay3Pressed();
    void pushButtonPlay4Pressed();
    void pushButtonPlay5Pressed();

private:
    Ui::MainWindow *ui;
    QMediaPlayer *m_mediaPlayer;
    QAudioOutput *m_audioOutput;
    SerialSettingsDialog *m_serialSettingsDialog;
    QSerialPort *m_serial = nullptr;
    QHttpServer *m_apiServer = nullptr;
    void playSong(int pos);
    void readSettings();
    void writeSettings();
    void openSerialSettings();
    void setVolume(int volume);
    void closeEvent(QCloseEvent *event);
    long map(long x, long in_min, long in_max, long out_min, long out_max)
    {
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }

};
#endif // MAINWINDOW_H

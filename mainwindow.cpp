#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "serialsettingsdialog.h"

#include <QSerialPortInfo>
#include <QFileDialog>
#include <QDir>
#include <QSettings>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_serialSettingsDialog(new SerialSettingsDialog),
    m_mediaPlayer(new QMediaPlayer),
    m_serial(new QSerialPort(this))
{
    ui->setupUi(this);

    connect(m_mediaPlayer, &QMediaPlayer::volumeChanged, this, &MainWindow::volumeChanged);
    connect(ui->actionSettings, &QAction::triggered, m_serialSettingsDialog, &SerialSettingsDialog::show);

    connect(m_serial, &QSerialPort::readyRead, this, &MainWindow::readData);

    connect(ui->pushButtonPlay1, &QAbstractButton::clicked, this, &MainWindow::pushButtonPlay1Pressed);
    connect(ui->pushButtonPlay2, &QAbstractButton::clicked, this, &MainWindow::pushButtonPlay2Pressed);
    connect(ui->pushButtonPlay3, &QAbstractButton::clicked, this, &MainWindow::pushButtonPlay3Pressed);
    connect(ui->pushButtonPlay4, &QAbstractButton::clicked, this, &MainWindow::pushButtonPlay4Pressed);
    connect(ui->pushButtonPlay5, &QAbstractButton::clicked, this, &MainWindow::pushButtonPlay5Pressed);

    connect(ui->pushButtonSelect1, &QAbstractButton::clicked, this, &MainWindow::pushButtonSelect1Pressed);
    connect(ui->pushButtonSelect2, &QAbstractButton::clicked, this, &MainWindow::pushButtonSelect2Pressed);
    connect(ui->pushButtonSelect3, &QAbstractButton::clicked, this, &MainWindow::pushButtonSelect3Pressed);
    connect(ui->pushButtonSelect4, &QAbstractButton::clicked, this, &MainWindow::pushButtonSelect4Pressed);
    connect(ui->pushButtonSelect5, &QAbstractButton::clicked, this, &MainWindow::pushButtonSelect5Pressed);

    connect(ui->dial, &QAbstractSlider::valueChanged, this, &MainWindow::volumeDialValueChanged);

    readSettings();

    SerialSettingsDialog::Settings serialSettings = m_serialSettingsDialog->settings();
    qDebug() << "loaded serial settings port: " << serialSettings.name << " baud rate: " << serialSettings.baudRate;

    m_serial->setBaudRate(serialSettings.baudRate);
    m_serial->setPortName(serialSettings.name);

    if(m_serial->open(QIODevice::ReadWrite)){
        ui->statusbar->showMessage(QString("Arduino %1[%2] connected").arg(serialSettings.name).arg(serialSettings.baudRate));
    } else {
        ui->statusbar->showMessage(QString("failed to connect arduino %1[%2]").arg(serialSettings.name).arg(serialSettings.baudRate));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::writeSettings() {
    QSettings settings("SV48Reichwalde", "Nippelboard");
    settings.beginGroup("Buttons");
    settings.setValue("button0", ui->lineEdit_1->text());
    settings.setValue("button1", ui->lineEdit_2->text());
    settings.setValue("button2", ui->lineEdit_3->text());
    settings.setValue("button3", ui->lineEdit_4->text());
    settings.setValue("button4", ui->lineEdit_5->text());
    settings.endGroup();
}

void MainWindow::readSettings() {
    QSettings settings("SV48Reichwalde", "Nippelboard");
    settings.beginGroup("Buttons");
    ui->lineEdit_1->setText(settings.value("button0", QString()).toString());
    ui->lineEdit_2->setText(settings.value("button1", QString()).toString());
    ui->lineEdit_3->setText(settings.value("button2", QString()).toString());
    ui->lineEdit_4->setText(settings.value("button3", QString()).toString());
    ui->lineEdit_5->setText(settings.value("button4", QString()).toString());
    settings.endGroup();
}

void MainWindow::readData() {

    const QByteArray data = m_serial->readAll();
    QString dataString = QString(data);
    qDebug() << "Received: " << dataString;
    auto requests = dataString.split("\r\n");

    for(const QString &requestString : std::as_const(requests)) {
        QStringList parts = requestString.split(";");
        if(parts.size() == 2) {
            int songId = parts.at(0).toInt();
            int volume = map(parts.at(1).toInt(), 0, 255, 0, 100);
            switch(songId) {
            case 0:
                m_mediaPlayer->setVolume(volume);
                break;
            case 1:
                playSong(0);
                m_mediaPlayer->setVolume(volume);
                break;
            case 2:
                playSong(1);
                m_mediaPlayer->setVolume(volume);
                break;
            case 4:
                playSong(2);
                m_mediaPlayer->setVolume(volume);
                break;
            case 8:
                playSong(3);
                m_mediaPlayer->setVolume(volume);
                break;
            case 16:
                playSong(4);
                m_mediaPlayer->setVolume(volume);
                break;
            }
        }
    }

}

void MainWindow::handleSerialError(QSerialPort::SerialPortError error) {

}

void MainWindow::volumeChanged(int value)
{
    ui->dial->setValue(value);
}

void MainWindow::playSong(int pos) {
    switch(pos) {
    case 0:
        if(!ui->lineEdit_1->text().isEmpty()){
            m_mediaPlayer->setMedia(QUrl::fromLocalFile(ui->lineEdit_1->text()));
            m_mediaPlayer->play();

            ui->pushButtonPlay1->setChecked(true);
            ui->pushButtonPlay2->setChecked(false);
            ui->pushButtonPlay3->setChecked(false);
            ui->pushButtonPlay4->setChecked(false);
            ui->pushButtonPlay5->setChecked(false);
        }
        break;
    case 1:
        if(!ui->lineEdit_2->text().isEmpty()){
            m_mediaPlayer->setMedia(QUrl::fromLocalFile(ui->lineEdit_2->text()));
            m_mediaPlayer->play();

            ui->pushButtonPlay1->setChecked(false);
            ui->pushButtonPlay2->setChecked(true);
            ui->pushButtonPlay3->setChecked(false);
            ui->pushButtonPlay4->setChecked(false);
            ui->pushButtonPlay5->setChecked(false);
        }
        break;
    case 2:
        if(!ui->lineEdit_3->text().isEmpty()){
            m_mediaPlayer->setMedia(QUrl::fromLocalFile(ui->lineEdit_3->text()));
            m_mediaPlayer->play();

            ui->pushButtonPlay1->setChecked(false);
            ui->pushButtonPlay2->setChecked(false);
            ui->pushButtonPlay3->setChecked(true);
            ui->pushButtonPlay4->setChecked(false);
            ui->pushButtonPlay5->setChecked(false);
        }
        break;
    case 3:
        if(!ui->lineEdit_4->text().isEmpty()){
            m_mediaPlayer->setMedia(QUrl::fromLocalFile(ui->lineEdit_4->text()));
            m_mediaPlayer->play();

            ui->pushButtonPlay1->setChecked(false);
            ui->pushButtonPlay2->setChecked(false);
            ui->pushButtonPlay3->setChecked(false);
            ui->pushButtonPlay4->setChecked(true);
            ui->pushButtonPlay5->setChecked(false);
        }
        break;
    case 4:
        if(!ui->lineEdit_5->text().isEmpty()){
            m_mediaPlayer->setMedia(QUrl::fromLocalFile(ui->lineEdit_5->text()));
            m_mediaPlayer->play();

            ui->pushButtonPlay1->setChecked(false);
            ui->pushButtonPlay2->setChecked(false);
            ui->pushButtonPlay3->setChecked(false);
            ui->pushButtonPlay4->setChecked(false);
            ui->pushButtonPlay5->setChecked(true);
        }
        break;
    }
}

void MainWindow::openSerialSettings() {

}

void MainWindow::volumeDialValueChanged(int value)
{
    m_mediaPlayer->setVolume(value);
}


void MainWindow::pushButtonSelect1Pressed()
{
    ui->lineEdit_1->setText(
            QFileDialog::getOpenFileName(this, tr("Choose MP3 file for button"), QDir::homePath(), tr("MP3 (*.mp3)"))
        );
    writeSettings();
}

void MainWindow::pushButtonSelect2Pressed()
{
    ui->lineEdit_2->setText(
        QFileDialog::getOpenFileName(this, tr("Choose MP3 file for button"), QDir::homePath(), tr("MP3 (*.mp3)"))
        );
    writeSettings();
}

void MainWindow::pushButtonSelect3Pressed()
{
    ui->lineEdit_3->setText(
        QFileDialog::getOpenFileName(this, tr("Choose MP3 file for button"), QDir::homePath(), tr("MP3 (*.mp3)"))
        );
    writeSettings();
}

void MainWindow::pushButtonSelect4Pressed()
{
    ui->lineEdit_4->setText(
        QFileDialog::getOpenFileName(this, tr("Choose MP3 file for button"), QDir::homePath(), tr("MP3 (*.mp3)"))
        );
    writeSettings();
}

void MainWindow::pushButtonSelect5Pressed()
{
    ui->lineEdit_5->setText(
        QFileDialog::getOpenFileName(this, tr("Choose MP3 file for button"), QDir::homePath(), tr("MP3 (*.mp3)"))
        );
    writeSettings();
}



void MainWindow::pushButtonPlay1Pressed()
{
    playSong(0);
}

void MainWindow::pushButtonPlay2Pressed()
{
    playSong(1);
}

void MainWindow::pushButtonPlay3Pressed()
{
    playSong(2);
}

void MainWindow::pushButtonPlay4Pressed()
{
    playSong(3);
}

void MainWindow::pushButtonPlay5Pressed()
{
    playSong(4);
}

void MainWindow::closeEvent(QCloseEvent *event) {
    qDebug() << "CloseEvent";
    writeSettings();
}

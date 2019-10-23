#include "mainwindow.h"
#include <QLabel>
#include <QApplication>
#include <QProcess>
#include <QDebug>
#include <QFile>
#include <QMessageBox>

static QProcess *defaultAudio;
int main(int argc, char *argv[])
{

    QString audioLogFile = "default-audio-log.txt";

    QApplication a(argc, argv);


    QString program = "/bin/sh"; //start a shell
    QStringList arguments;

    QString getDefaultAudio = "pacmd list-sinks | grep -A1 \"* index:\" | grep \"name\" | awk '{print $2;}'";
    arguments << "-c";
    arguments.append(getDefaultAudio);

    defaultAudio = new QProcess;

    defaultAudio->setProgram(program);
    defaultAudio->setArguments(arguments);

    defaultAudio->setStandardOutputFile(audioLogFile);
    defaultAudio->setStandardErrorFile("default-audio-error-log.txt");

    defaultAudio->start();
    defaultAudio->waitForFinished();

    QString defaultAudioSink = defaultAudio->readAllStandardOutput();
    //QString defaultAudioSinkErr = defaultAudio->readAllStandardError();

    QString audiologfilePath = QCoreApplication::applicationDirPath() + audioLogFile;

    QFile file(audiologfilePath);

    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(nullptr, "error", file.errorString());
    }

    QTextStream in(&file);

    while(!in.atEnd()) {
        QString line = in.readLine();
         qInfo( "%s", line.toStdString().c_str() );
        //MainWindow::setDefaultAudioSink(line);
    }

    file.close();


    qDebug() << defaultAudioSink;
    qInfo( "%s", defaultAudioSink.toStdString().c_str() );
    //qInfo( "%s", defaultAudioSinkErr.toStdString().c_str() );

    //setDefaultAudioSink(QString defaultAudioSink)

    defaultAudio->terminate();



    MainWindow w;
    w.setWindowTitle("Pulse Audio Monitor Recorder");

    w.show();
    return a.exec();
}

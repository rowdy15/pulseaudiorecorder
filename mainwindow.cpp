#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcess>
#include <QString>
#include <QDir>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
//#include <QProcessEnvironment>

#include<QDebug>

static QProcess *process;
static QString folder;
static QProcess *defaultAudio;

QString MainWindow::mDefaultAudioSink;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//TODO
// disable recording button when pressed so only one process happens at any one time
void MainWindow::on_pushButton_clicked()
{
    ui->pushButton->setDisabled(true);
    MainWindow::setDefaultAudioSink();

    ui->errorLabel->setText("");
    QString mp3file = ui->fileNameTextEdit->toPlainText();

    QString program = "/bin/sh"; //start a shell
    QStringList arguments;

    QString cmd = "parec -d ";
    cmd.append(mDefaultAudioSink);
    cmd.append(".monitor | lame -r -b");
    QString bitRate = QString::number(ui->spinBox->value());
    cmd.append(bitRate);
    cmd.append(" - ");
    if(folder.isNull() || folder.isEmpty()){
        qInfo( "folder is not null" );
        cmd.append("~/");
    } else {
         qInfo( "folder is not null" );
        if(!QDir(folder).exists()){
            qInfo( "making new path" );
            QDir().mkdir(folder);
        }

        if (!QDir(folder).exists()){
            ui->errorLabel->setText("The folder you suggested isn't well formed so the file has been put in your home folder");
            cmd.append("~/");
        } else{
            cmd.append(folder);
            cmd.append("/");
        }

    }

    if(ui->fileNameTextEdit->toPlainText().isNull() || ui->fileNameTextEdit->toPlainText().isEmpty()) {
        cmd.append("audio-stream.mp3");
    } else {
        cmd.append(ui->fileNameTextEdit->toPlainText());
        cmd.append(".mp3");
    }

    qInfo("%s", cmd.toStdString().c_str());

    arguments << "-c";
    arguments.append(cmd);

    //arguments << "-c" << "parecord -d combined.monitor --file-format=oga test.oga";
    process = new QProcess;

    process->setProgram(program);
    process->setArguments(arguments);

    process->setStandardOutputFile("log.txt");
    process->setStandardErrorFile("error-log.txt");


    process->startDetached();
    qInfo( "recording process has started" );
    ui->label->setText("You are now recording!");

    //process->waitForFinished(10000);

    //process.close();

}

void MainWindow::on_pushButton_2_clicked()
{
    qInfo( "terminate request has been sent to recording process" );
    system("killall -9 parec");
    process->kill();
    ui->label->setText("You have stopped recording!");

    ui->pushButton->setEnabled(true);
}

void MainWindow::on_plainTextEdit_textChanged()
{
    folder = ui->plainTextEdit->toPlainText();
}

void MainWindow::setDefaultAudioSink()
{
    QString audioLogFile = "default-audio-log.txt";
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


    //QString defaultAudioSinkErr = defaultAudio->readAllStandardError();

    QString audiologfilePath = QCoreApplication::applicationDirPath() + "/" + audioLogFile;

    QFile file(audiologfilePath);

    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(nullptr, "error", file.errorString());
    }

    QTextStream in(&file);

    while(!in.atEnd()) {
        QString defaultAudioSink;
        QString line = in.readLine();
        //qInfo( "The default audio output is: %s", line.toStdString().c_str() );
        defaultAudioSink =  line.remove(0,1);
        defaultAudioSink =  defaultAudioSink.remove(defaultAudioSink.size()-1,1);
        mDefaultAudioSink = defaultAudioSink;
    }

    file.close();
    defaultAudio->terminate();
}

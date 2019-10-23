#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcess>
#include <QString>
#include <QDir>
//#include <QProcessEnvironment>

#include<QDebug>

static QProcess *process;
static QString folder;

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

//    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
//    env.insert("TMPDIR", "home/sam/audio"); // Add an environment variable
//    process->setProcessEnvironment(env);

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
}

void MainWindow::on_plainTextEdit_textChanged()
{
    folder = ui->plainTextEdit->toPlainText();
}

void MainWindow::setDefaultAudioSink(QString defaultAudioSink)
{
    defaultAudioSink =  defaultAudioSink.left(-1);
    defaultAudioSink =  defaultAudioSink.right(-1);
    mDefaultAudioSink = defaultAudioSink;
}

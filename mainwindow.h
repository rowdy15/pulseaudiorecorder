#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setDefaultAudioSink(QString defaultAudioSink);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_plainTextEdit_textChanged();





private:
    Ui::MainWindow *ui;
    QString mDefaultAudioSink;
};
#endif // MAINWINDOW_H

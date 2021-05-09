#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QWidget>
#include <QDebug>
#include <QMessageBox>
#include <QTime>
#include <Simulator.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Codes* codes;
    Sim* sim;
    QList<QPair<int, QColor>> highlights;
    int debug_mode = 0;
    void Sleep(int);
protected:
    bool eventFilter(QObject *target, QEvent *event);//事件过滤器
    void uirender();
private slots:
    void Myprint();
    void on_send_clicked();
    void on_load_clicked();
    void on_run_clicked();
    void on_clear_clicked();
    void on_step_clicked();


private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

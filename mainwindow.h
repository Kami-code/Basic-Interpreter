#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QWidget>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    bool eventFilter(QObject *target, QEvent *event);//事件过滤器
    void uirender();
private slots:
    void Myprint();
    void on_send_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

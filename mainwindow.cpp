#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "algorithm.h"
#include <vector>
using namespace std;

extern vector<code_class> codes;

void MainWindow::Myprint() {
    qDebug() << ui->textEdit->toPlainText() << Qt::endl;
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->textEdit->installEventFilter(this);
    connect(ui->loadButton, &QPushButton::released, this, &MainWindow::Myprint);
}

bool MainWindow::eventFilter(QObject *target, QEvent *event)
{
    if(target == ui->textEdit)
    {
        if(event->type() == QEvent::KeyPress)//回车键
        {
             QKeyEvent *k = static_cast<QKeyEvent *>(event);
             if(k->key() == Qt::Key_Return)
             {
                 on_send_clicked();
                 return true;
             }
        }
    }
    return QWidget::eventFilter(target,event);
}

void MainWindow::uirender() {
    ui->codeBroswer->setText("");
    for (auto i = codes.begin(); i != codes.end(); ++i) {
        QString q_code_string = QString::fromStdString((*i).code_string);
        ui->codeBroswer->append(q_code_string);
        qDebug() << "fucked!" << q_code_string << Qt::endl;
    }
    return;
}

void MainWindow::on_send_clicked()
{
    QString msg = ui->textEdit->toPlainText();
    if (judge(msg.toStdString()) == false) return;
    ui->textEdit->clear();
    ui->textEdit->setFocus();
    uirender();
    ui->grammarBrowser->append(msg);
    ui->resultBrowser->append(msg);
}

MainWindow::~MainWindow()
{
    delete ui;
}


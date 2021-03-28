#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "algorithm.h"
#include <vector>
#include <QApplication>
#include <QFileDialog>
#include <QDebug>
#include <QString>
#include <queue>
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
    connect(ui->loadButton, &QPushButton::released, this, &MainWindow::on_load_clicked);
    connect(ui->runButton, &QPushButton::released, this, &MainWindow::on_run_clicked);
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
}

extern void execute_codes(vector<code_class>&, queue<string>&, queue<string>&);

void MainWindow::on_run_clicked()
{
    queue<string>result_strings, gammer_strings;
    execute_codes(codes, result_strings, gammer_strings);
    ui->resultBrowser->clear();
    while (result_strings.empty() != true) {
        ui->resultBrowser->append(QString::fromStdString(result_strings.front()));
        result_strings.pop();
    }
    ui->grammarBrowser->clear();
    for (auto j = codes.begin(); j != codes.end(); ++j) { //此处O(N)查找可以降低到O(logN)
        ui->grammarBrowser->append(QString::fromStdString((*j).grammer));
    }
//    while (gammer_strings.empty() != true) {
//        ui->grammarBrowser->append(QString::fromStdString(gammer_strings.front()));
//        gammer_strings.pop();
//    }
}

void MainWindow::on_load_clicked()
{

    QString fileName;
    fileName = QFileDialog::getOpenFileName(this,
        tr("Open Codes"), "", tr("TXT Files (*.txt)"));

    if (!fileName.isNull())//fileName即是选择的文件名
    {
        QFile file(fileName);
        if (!file.open(QFile::ReadOnly|QFile::Text)) {
            return;
        }
        QTextStream in(&file);

        while(1) {
            QString a = "";
            a = in.readLine();
            judge(a.toStdString());
            if (a == "") break;
            ui->codeBroswer->append(a);
        }


        update();
        return;
    }
    else { //点取消
        return;
    }


}

MainWindow::~MainWindow()
{
    delete ui;
}


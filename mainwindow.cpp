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

extern save_class saver;
queue<string>result_strings, gammer_strings;

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
    connect(ui->clearButton, &QPushButton::released, this, &MainWindow::on_clear_clicked);
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
    for (auto i = saver.codes.begin(); i != saver.codes.end(); ++i) {
        QString q_code_string = QString::fromStdString((*i).code_string);
        ui->codeBroswer->append(q_code_string);
    }
    return;
}

extern int judge(string, int&);
extern void execute_codes(vector<code_class>&);

void MainWindow::on_send_clicked()
{
    QString msg = ui->textEdit->toPlainText();
    int res = 0;
    int judge_result_mode = judge(msg.toStdString(), res);
    if (judge_result_mode == 0) return; //判断是否满足序号 命令的情况
    else if (judge_result_mode == 2) { //PRINT的情况
        ui->resultBrowser->append(QString::fromStdString(to_string(res)));
        ui->textEdit->clear();
        ui->textEdit->setFocus();
    }
    else if (judge_result_mode == 1 || judge_result_mode == 3 || judge_result_mode == 5){ //普通情况
        if (judge_result_mode == 5) ui->resultBrowser->append("?");
        ui->textEdit->clear();
        ui->textEdit->setFocus();
        uirender();
    }
    else if (judge_result_mode == 4) { //纯数字且Input的情况
        qDebug() << "这是一个INPUT情况" << endl;
        ui->textEdit->clear();
        ui->textEdit->setFocus();
        QTextCursor tc = ui->resultBrowser->textCursor();
        tc.movePosition(QTextCursor::End);
        tc.insertText(" " + msg);
        if (saver.status == 2) {
            saver.status = 3;
            on_run_clicked();
        }
    }
}


void MainWindow::on_run_clicked()
{

    execute_codes(saver.codes);
    //打印执行结果
    qDebug() << "队列是否为空? " << endl;
    while (result_strings.empty() != true) {
        ui->resultBrowser->append(QString::fromStdString(result_strings.front()));
        qDebug() << "开始打印结果" << QString::fromStdString(result_strings.front()) << endl;
        result_strings.pop();
    }
    if (saver.status == 2) {
        ui->resultBrowser->append("?");
    }
    ui->grammarBrowser->clear();
    //打印表达式树
    for (auto j = saver.codes.begin(); j != saver.codes.end(); ++j) { //此处O(N)查找可以降低到O(logN)
        if ((*j).grammer == "") continue;
        ui->grammarBrowser->append(QString::fromStdString((*j).grammer));
    }
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
            int b;
            judge(a.toStdString(), b);
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


void MainWindow::on_clear_clicked()
{
    saver.reinit();
    while(result_strings.empty() != true) {
        result_strings.pop();
    }
    while(gammer_strings.empty() != true) {
        gammer_strings.pop();
    }
    ui->grammarBrowser->clear();
    ui->codeBroswer->clear();
    ui->resultBrowser->clear();
}

MainWindow::~MainWindow()
{
    delete ui;
}


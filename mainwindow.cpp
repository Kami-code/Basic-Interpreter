#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ExpressionTree.h"
#include <vector>
#include <QApplication>
#include <QFileDialog>
#include <QDebug>
#include <QString>
#include <queue>
using namespace std;

queue<string>result_strings, gammer_strings;

void MainWindow::Myprint() {
    qDebug() << ui->textEdit->toPlainText() << Qt::endl;
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    codes = new Codes();
    sim = new Sim(codes);
    ui->setupUi(this);
    ui->textEdit->installEventFilter(this);
    connect(ui->loadButton, &QPushButton::released, this, &MainWindow::on_load_clicked);
    connect(ui->runButton, &QPushButton::released, this, &MainWindow::on_run_clicked);
    connect(ui->clearButton, &QPushButton::released, this, &MainWindow::on_clear_clicked);
    connect(ui->stepButton, &QPushButton::released, this, &MainWindow::on_step_clicked);
    highlights = {
            {0, QColor(100, 255, 100)},
            {145, QColor(255, 100, 100)},
            {180, QColor(255, 100, 100)}
        };
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
    for (auto i = codes->getCodesVec()->begin(); i != codes->getCodesVec()->end(); ++i) {
        QString q_code_string = QString::fromStdString((*i).getCodeString());
        ui->codeBroswer->append(q_code_string);
    }

    QTextBrowser *code = ui->codeBroswer;
    QTextCursor cursor(code->document());
    // 用于维护的所有高亮的链表
    QList<QTextEdit::ExtraSelection> extras;
    // 配置高亮，并加入到 extras 中
    for (auto &line : highlights) {
        QTextEdit::ExtraSelection h;
        h.cursor = cursor;
        // 下面这些的功能，请大家自行查看文档
        h.cursor.setPosition(line.first);
        h.cursor.movePosition(QTextCursor::StartOfLine);
        h.cursor.movePosition(QTextCursor::EndOfLine);
        h.format.setProperty(QTextFormat::FullWidthSelection, true);
        h.format.setBackground(line.second);
        extras.append(h);
    }
    // 应用高亮效果
    code->setExtraSelections(extras);

    ui->variableBrowser->setText(QString::fromStdString(sim->showVariables()));

    return;
}

void MainWindow::on_send_clicked()
{
    QString msg = ui->textEdit->toPlainText();
}


void MainWindow::on_run_clicked()
{
}

void MainWindow::on_step_clicked()
{
    int return_status = 0;
    if (debug_mode == 0) {
        sim->refreshCodes(codes);
        //得到即将执行的下一行代码
        return_status = codes->getHightlightByIndex(0);
        if (return_status == -1) {
            debug_mode = 0;
            return;
        }
        else {
            debug_mode = return_status;
            ui->clearButton->setEnabled(false);
            ui->loadButton->setEnabled(false);
        }

    }
    else {
        sim->singleStepSim(return_status);
        cout << "return status = " << return_status << endl;
        if (return_status == SAOK) {
            int index = codes->searchByPC(sim->get_PC());
            cout << "index = " << index << endl;
            cout << "getHightlightByIndex = " << codes->getHightlightByIndex(index) << endl;
            debug_mode = codes->getHightlightByIndex(index);
            //得到即将执行的下一行代码
        }
//        else if (return_status == SINS) {
//            QMessageBox::critical(NULL, "程序出错", "该语句有错误", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
//        }
//        else if (return_status == SFIN) {
//            QMessageBox::about(NULL, "程序结束", "被调试的程序正常结束");
//        }
        highlights = {
                {debug_mode, QColor(100, 255, 100)}
            };
    }

    uirender();
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
            if (a == "") break;
            codes->uniquePush(Code(a.toStdString()));
        }
        update();
        uirender();
        return;
    }
    else { //点取消
        return;
    }
}


void MainWindow::on_clear_clicked()
{
    while(result_strings.empty() != true) {
        result_strings.pop();
    }
    while(gammer_strings.empty() != true) {
        gammer_strings.pop();
    }
    ui->grammarBrowser->clear();
    ui->codeBroswer->clear();
    ui->resultBrowser->clear();
    ui->variableBrowser->clear();
}

MainWindow::~MainWindow()
{
    delete ui;
}


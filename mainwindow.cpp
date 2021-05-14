#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "expressiontree.h"
#include <vector>
#include <QApplication>
#include <QFileDialog>
#include <QDebug>
#include <QString>
#include <queue>
using namespace std;


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
    highlights = {};
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
        ui->codeBroswer->setTextCursor(h.cursor);
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
    string exp = (ui->textEdit->toPlainText()).toStdString();
    if (sim->getStatus() == SHLT) { //INPUT的情况
        int return_status = 0;
        map<string, int> &variables = (*sim->getVariables());
        map<string, string> &str_variables = (*sim->getStrVariables());
        Code code = sim->getLatestCode();
        string variable = code.getVariable();
        if (code.getCodeType() == "INPUT") {
            try {
                parseExpression(exp, return_status, variables, 1);
                variables[code.getVariable()] = return_status;
                ui->resultBrowser->append(QString::fromStdString(to_string(return_status)));
                auto iter=str_variables.find(variable);
                if (iter != str_variables.end()) str_variables.erase(iter);
                sim->setStatus(SAOK);
                on_step_clicked();
                return;
            }  catch (...) {
                QMessageBox::critical(NULL, "输入出错", "请输入一个合法表达式！", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            }
        }
        else if (code.getCodeType() == "INPUTS") {
            try {
                string str_variable;
                parse_str_variable(exp, return_status, str_variable);
                if (return_status == -1) {
                    QMessageBox::critical(NULL, "输入出错", "请输入一个合法字符串！", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
                    return;
                }
                else {
                    str_variables[variable] = str_variable;
                    ui->resultBrowser->append(QString::fromStdString(str_variable));
                    auto iter=variables.find(variable);
                    if (iter != variables.end()) variables.erase(iter);
                    sim->setStatus(SAOK);
                    on_step_clicked();
                    return;
                }
            }  catch (...) {
                QMessageBox::critical(NULL, "输入出错", "该语句有错误", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            }
        }
    }
    else {
        Code code_tmp(exp);
        int code_number = code_tmp.getCodeNumber();
        string code_type = code_tmp.getCodeType();
        if (code_number == 0) {
            if (code_type == "LET") {
                int exp_val;
                string exp1 = code_tmp.getExp1();
                map<string, int> &variables = *sim->getVariables();
                map<string, string> &str_variables = *sim->getStrVariables();
                try {
                    parseExpression(exp1, exp_val, variables, 1);
                    variables[code_tmp.getVariable()] = exp_val;
                    auto iter=str_variables.find(code_tmp.getVariable());
                    if (iter != str_variables.end()) str_variables.erase(iter);
                }  catch (...) {
                    string str_variable;
                    int return_status = 0;
                    parse_str_variable(exp1, return_status, str_variable);
                    if (return_status == -1) {
                        QMessageBox::critical(NULL, "程序出错", "LET语句不合法！", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
                        return;
                    }
                    else {
                        str_variables[code_tmp.getVariable()] = str_variable;
                        auto iter=variables.find(code_tmp.getVariable());
                        if (iter != variables.end()) variables.erase(iter);
                    }
                }
            }
            else if (code_type == "PRINT") {
                int exp_val;
                try {
                    parseExpression(code_tmp.getExp1(), exp_val, *sim->getVariables(), 1);
                    ui->resultBrowser->append(QString::fromStdString(to_string(exp_val)));
                }  catch (...) {
                    QMessageBox::critical(NULL, "程序出错", "PRINT语句不合法！", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
                    return;
                }
            }
        }
        else {
            codes->uniquePush(code_tmp);
        }

        uirender();
    }
}

void MainWindow::Sleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
void MainWindow::on_run_clicked()
{
    on_step_clicked();
    Sleep(200);
    while(debug_mode) {
        on_step_clicked();
        Sleep(200);
    }
}

void MainWindow::on_step_clicked()
{
    int return_status = 0;
    if (debug_mode == 0) {
        sim->getVariables()->clear();
        sim->getStrVariables()->clear();
        sim->refreshCodes(codes);
        //得到即将执行的下一行代码
        if (codes->getCodesVec()->size() == 0) {
            debug_mode = 0;
        }
        else {
            for (size_t i = 0; i < codes->getCodesVec()->size(); ++i) {
                if ((*codes->getCodesVec())[i].getGrammer() == "invaild") {
                    highlights.append({codes->getHightlightByIndex(i), QColor(255, 100, 100)});
                }
            }
            ui->grammarBrowser->clear();
            ui->grammarBrowser->append(QString::fromStdString((*codes->getCodesVec())[0].getGrammer()));
            debug_mode = codes->getHightlightByIndex(0);
            highlights.append({0, QColor(100, 255, 100)});
            ui->clearButton->setEnabled(false);
            ui->loadButton->setEnabled(false);
        }
        uirender();
        return;
    }
    else {
        status old_status = sim->getStatus();
        sim->singleStepSim(return_status);
        cout << "return status = " << return_status << endl;
        if (return_status == SAOK || return_status == SHLT) {
            if (old_status == SAOK && return_status == SHLT) {
                ui->resultBrowser->append(QString::fromStdString("  ?"));
            }

            int index = codes->searchByPC(sim->get_PC());
            if (index == -1) {
                QMessageBox::critical(NULL, "程序出错", "没有找到终止的END语句", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
                debug_mode = 0;
                highlights.clear();
                ui->clearButton->setEnabled(true);
                ui->loadButton->setEnabled(true);
                uirender();
                return;
            }
            ui->grammarBrowser->clear();
            ui->grammarBrowser->append(QString::fromStdString((*codes->getCodesVec())[index].getGrammer()));
            if (sim->getLatestCode().getCodeType() == "PRINT" || sim->getLatestCode().getCodeType() == "PRINTF") {
                ui->resultBrowser->append(QString::fromStdString(sim->getLatestOut()));
            }
            debug_mode = codes->getHightlightByIndex(index);
            if (highlights.empty() == false) highlights.last() = {debug_mode, QColor(100, 255, 100)};
            else highlights.append({debug_mode, QColor(100, 255, 100)});

            uirender();
            return;
            //得到即将执行的下一行代码
        }
        else if (return_status == SINS) {
            QMessageBox::critical(NULL, "程序出错", "该语句有错误", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            debug_mode = 0;
            highlights.clear();
            ui->clearButton->setEnabled(true);
            ui->loadButton->setEnabled(true);
            uirender();
            return;
        }
        else if (return_status == SFIN) {
            QMessageBox::about(NULL, "程序结束", "被调试的程序正常结束");
            debug_mode = 0;
            highlights.clear();
            ui->clearButton->setEnabled(true);
            ui->loadButton->setEnabled(true);
            uirender();
            return;
        }

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
    ui->grammarBrowser->clear();
    ui->codeBroswer->clear();
    ui->resultBrowser->clear();
    ui->variableBrowser->clear();
    delete codes;
    delete sim;
    codes = new Codes();
    sim = new Sim(codes);
}

MainWindow::~MainWindow()
{
    delete ui;
}


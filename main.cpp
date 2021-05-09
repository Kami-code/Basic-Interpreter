#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}


//#include "utils.h"
//#include "Code.h"
//#include "Simulator.h"
//#include <iostream>
//using namespace std;

//int main() {

//    Code bbbb("100 REM");
//    bbbb.createGrammerTree();

//    Code cccc("11 LET m=p+q*t");
//    cccc.createGrammerTree();

//    Code d("12 PRINT p+q*t");
//    d.createGrammerTree();

//    Codes codes;
//    codes.uniquePush(Code("100 REM"));
//    codes.uniquePush(Code("110 LET max = 10000"));
//    codes.uniquePush(Code("120 LET n1 = 0"));
//    codes.uniquePush(Code("130 LET n2 = 1"));
//    codes.uniquePush(Code("140 IF n1 > max THEN 190"));
//    codes.uniquePush(Code("145 PRINT n1"));
//    codes.uniquePush(Code("150 LET n3 = n1+n2"));
//    codes.uniquePush(Code("160 LET n1 = n2"));
//    codes.uniquePush(Code("170 LET n2 = n3"));
//    codes.uniquePush(Code("180 GOTO 140"));
//    codes.uniquePush(Code("190 END"));

//    codes.showCodes();

//    Sim sim(&codes);
//    int return_status = 0;
//    int cnt = 0;
//    while(return_status == 0) {
//        sim.singleStepSim(return_status);
//        cnt++;
//    }
//    return 0;
//}

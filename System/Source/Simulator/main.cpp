#include "mainwindow.h"
#include "DB.h"

#include <QApplication>
//#include <sys/ptrace.h>

int main(int argc, char *argv[])
{
  try {
//  ptrace(PTRACE_TRACEME, getpid(), 0, 0);
  QApplication a(argc, argv);
  DB db;
  MainWindow w;
  w.show();

  return a.exec();
  } catch (...)
  {
    KKK();
  }

}

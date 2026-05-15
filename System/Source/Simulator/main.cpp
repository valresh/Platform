#include "mainwindow.h"
#include "DB.h"

#include <QApplication>

int main(int argc, char *argv[])
{
  try {
//  ptrace(PTRACE_TRACEME, getpid(), 0, 0);
//  DB::Test( );
//  uint customStackSize = 4 * 1024 * 1024;
//  setStackSize(customStackSize);
  DB::Read( );
  QApplication a(argc, argv);
  MainWindow w;
  w.show();

  return a.exec();
  } catch (...)
  {
    KKK();
  }

}

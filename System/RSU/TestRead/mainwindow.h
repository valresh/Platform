#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <CommProc.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
public slots:
    void OutMsg( const char * Title, const char * Txt,
                 DWORD flags,
                 DWORD def,
                int * Res );

    void OutTxt( const char * Txt );

};
#endif // MAINWINDOW_H

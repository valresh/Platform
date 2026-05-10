#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "/home/valresh/Platform/RSU_F/HoneyWell5xx/DCUs/CnfReader/CnfReader.h"

#include <QMessageBox>

KCnfReader reader;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    reader.ReadCnf( "/home/valresh/Platform/projects/319_VSB_KF/SM/ESD_101_1/ESD_101_1.0006", IBlock::emrSymbolAttrs | IBlock::emrConVertexs | IBlock::emrParameters );

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OutMsg( const char * Title, const char * Txt,
                        DWORD flags,
                        DWORD def,
                        int * Res )
{
    *Res = QMessageBox::critical( NULL, Title, Txt, (QMessageBox::StandardButtons)flags, (QMessageBox::StandardButton)def );
}


void MainWindow::OutTxt( const char * Txt )
{
   qDebug( Txt );
}


// FILE * l_fopen ( const char * filename,
//               const char * modes )
// {
//     return fopen ( filename, modes );
// }

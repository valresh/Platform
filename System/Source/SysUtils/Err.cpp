#include <iostream>
#include <chrono>
#include "Err.h"
#include "BaseModel.h"
#include <QMessageBox>
#include <QCoreApplication>
#include <QGuiApplication>
#include <QClipboard>
#include "../Simulator/mainwindow.h"

extern char LOG_PATH[MAX_PATH];
//char CSyntErr::ErrMsg[1024];
tSysMsg pSysMsg;
class MainWindow * pMainWindow = NULL;
PosErrInFile PosErr;
int SysErrors;

int iKKK = 0;
void KKK()
{
    iKKK++;
}


SendMsg Sender;
////////////////////////////////////////////////////////////
void SysAssert( char * File, int Line, int* pNumbAssert )
{
    if ( (*pNumbAssert)++ < 5 )
    {
        FILE * F;
        time_t T = time(NULL);
        struct tm* Tm = localtime(&T );
        char Time[256];
        strftime( Time, 255, "%d.%m %H:%M:%S", Tm );
        if ( (F = fopen (LOG_PATH, "at" )) != 0 )
        {
            fprintf ( F, "%s ASS в '%s' : File '%s' Line %d\n", Time, CURR_PROJECT, File, Line );
            fclose ( F );
        }
        char Msg[1024];
        snprintf ( Msg, 1024, "%s ASS в '%s' : File '%s' Line %d", Time, CURR_PROJECT, File, Line );
        int Res = 0;
        Sender.SysOutMsg( "Ошибка", Msg, (DWORD)(QMessageBox::Abort|QMessageBox::Ignore),(DWORD)QMessageBox::Ignore, &Res );
        if ( Res == QMessageBox::Abort)
            if ( Res == QMessageBox::Abort )
            {
                QCoreApplication::exit(0);
            }
    }
// #ifdef QT_DEBUG
//     BREAK
// #endif
}

static char Msg[1024];
void MsgErr ( void *rezerv, LPCSTR Fmt, ... )
{
    va_list arg;
    va_start(arg,Fmt);
    vsnprintf ( Msg, sizeof(Msg), Fmt, arg );
    va_end(arg);
    int Res = 0;
    SysErrors++;
    Sender.SysOutMsg( "Ошибка", Msg, (DWORD)(QMessageBox::Abort|QMessageBox::Ignore),(DWORD)QMessageBox::Ignore, &Res );
    if ( Res == QMessageBox::Abort )
    {
        QCoreApplication::exit(0);
    }
}

void LogMsg( char * Fmt, ... )
{

}

int Record( char * Fmt, ... )
{
    return 0;
}

void SysMsgErr ( LPCSTR Fmt, ... )
{
    va_list arg;
    va_start(arg,Fmt);
    vsnprintf ( Msg, sizeof(Msg), Fmt, arg );
    if ( Msg[0] != '#' )
        SysErrors++;
    Sender.SysOutTxt( Msg );
    va_end(arg);
}

void NameToClp( char * Txt )
{
    QClipboard * clp = QGuiApplication::clipboard();
    clp->clear();
    clp->setText( Txt );
}

void ModelMsg(IBaseModel* pModel, LPCSTR Fmt, ...)
{
    char* ObjName = NULL;
    if (pModel)
    {
        ObjName = pModel->ObjName;
        NameToClp(ObjName);
    }
    va_list arg;
    va_start(arg, Fmt);
    vsnprintf(Msg, sizeof(Msg), Fmt, arg);
    va_end(arg);
    if ( Msg[0] != '#' )
        SysErrors++;
    Sender.SysOutTxt( Msg );
}

void InitErr(MainWindow * _pMainWindow )
{
    pMainWindow = _pMainWindow;
    Sender.Init();
    pSysMsg = SysMsgErr;
}

SendMsg::SendMsg():QObject(nullptr)
{

}

void SendMsg::Init()
{
    connect (this, &SendMsg::OutTxt, pMainWindow, &MainWindow::OutTxt, Qt::DirectConnection );// Qt::QueuedConnection );
    connect (this, &SendMsg::OutMsg, pMainWindow, &MainWindow::OutMsg, Qt::BlockingQueuedConnection );
}

void SendMsg::SysOutTxt(const char *Txt)
{
    emit OutTxt( Txt );
}

void SendMsg::SysOutMsg(const char *Title, const char *Txt, DWORD flags, DWORD def, int *Res)
{
    emit OutMsg( Title, Txt, (DWORD)flags, (DWORD)def, Res );
}

C_BP * C_BP::pFirst = NULL;
C_BP * C_BP::pLast = NULL;
C_BP::C_BP()
{
    memset ( this, 0, sizeof ( C_BP ));
    if ( pFirst == NULL )
        pFirst = this;
    if ( pLast )
        pLast->pNext = this;
    pLast = this;
    pNext = NULL;
};

static DWORD ClassNumb = 1;

void C_BP::SetID( DWORD & ClassID )
{
    if ( ID )
    {
        ClassID = ID;
        return;
    }
    // ID = 0
    if ( ClassID )
    {
        ID = ClassID;
        return;
    }
    // ID = ClassID
    ID = ClassID = ClassNumb++;
}

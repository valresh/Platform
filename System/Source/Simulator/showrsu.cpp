#include "showrsu.h"
#include "ui_showrsu.h"
#include "Err.h"
#include "Param.h"

ShowRSU::ShowRSU(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ShowRSU)
{
    WasInit = false;
    ui->setupUi(this);
    connect( ui->ListObj, &ObjListTab::ShowObject, this, &ShowRSU::ShowObject );
    pConn_Info = NULL;
}


void ShowRSU::ShowObject ( LPCSTR ObjName, LPCSTR Model, void * pBase )
{
   GetObjParams( pBase, Model, &ui->List_P->Model.Params, &ui->List_W->Model.Params );
   Refresh();
}

void ShowRSU::Go()
{
    show();
}

void ShowRSU::Refresh()
{
    ui->ListObj->Refresh();
    ui->List_W->Refresh();
    ui->List_P->Refresh();
}

CMem<RSU_Obj, 1024, 1024> * pRSU = NULL;

int CompRSU_Pnt ( const void * p1, const void * p2 )
    {
    int n1 = *(int*)p1;
    int n2 = *(int*)p2;
    RSU_Obj * pO1 = pRSU->Get( n1 );
    RSU_Obj * pO2 = pRSU->Get( n2 );
    return strcmp ( pO1->ObjName, pO2->ObjName );
    }

 int TestRSU_Pnt ( const void * p1, const void * p2 )
    {
        const char * s1 = (const char *)p1;
        int n2 = *(int*)p2;
        RSU_Obj * pO2 = pRSU->Get( n2 );
        return strcmp ( s1, pO2->ObjName );
    }

void ShowRSU::SetRSURef( )
    {
    if ( pConn_Info == NULL )
        return;
    pRSU = &ui->ListObj->Model.RSU_Pnt;
    int L = pRSU->L;
    if ( L <= 0 )
        return;
    int * O = NewArr ( int, L );
    for ( int n = 0; n < L; n++ )
        O[n] = n;
    std::qsort ( O,  L, sizeof(int), CompRSU_Pnt );
    for ( int n = 0; n < pConn_Info->L; n++ )
    {
        ConnInfo * pC = pConn_Info->Get ( n );
        char Name[1024];
        strcpy ( Name,pC->RSU);
        int L = strlen ( Name )-1;
        while ( L >= 0 )
        {
            if ( Name[L] == '.' )
            {
                Name[L] = 0;
                break;
            }
            L--;
        }
        int * pN = (int*)std::bsearch ( Name, O, L, sizeof(int), TestRSU_Pnt );
        if ( pN )
        {
            int n = *pN;
            RSU_Obj * pO = pRSU->Get( n );
            pO->Ref = pC->Model;
        }
    }
    //
}

 void ShowRSU::Init()
   {
    if (WasInit)
    {
    ObjListModel::NoRefresh = true;
    List_W_Model::NoRefresh = true;
    List_P_Model::NoRefresh = true;
    return;
    }
    WasInit = true;
    const char * Files[16];
    int kFiles = 0;
    const char * Models[256];
    int kModels = 0;
    GetRSUFilesList  ( &kFiles, Files );
    GetRSUModelsList  ( &kModels,Models );
    QComboBox * File = ui->File;
    File->addItem( "*" );
    for ( int n = 0; n < kFiles; n++ )
        File->addItem( Files[n] );
    QComboBox *TypeObj =  ui->TypeObj;
    TypeObj->addItem( "*" );
//    for ( int n = 0; n < kModels; n++ )
//        TypeObj->addItem( Models[n] );
    TypeObj->addItem( "AICHANNEL" );
    TypeObj->addItem( "AOCHANNEL" );
    TypeObj->addItem( "PID" );
}

ShowRSU::~ShowRSU()
{
    delete ui;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ObjListModel::NoRefresh = true;
#include <QMouseEvent>

void ObjListTab::mousePressEvent(QMouseEvent* event)
{
    QTableView::mousePressEvent(event); //
    if ( event->button() == Qt::LeftButton)
        {
 //       QPoint viewportPos = viewport()->mapFromParent(event->pos());
 //       QModelIndex index = indexAt(viewportPos);
        QModelIndexList Ind = selectedIndexes();
        if ( Ind.empty())
            return;
        QModelIndex I =  Ind[0];
        int r = I.row();
        if ( r < 0 || r >= Model.RSU_Pnt.L )
            return;
        RSU_Obj * pObj = Model.RSU_Pnt.Get(r);
        emit ShowObject ( pObj->ObjName.Str, pObj->Model, pObj->pBase );
        }
    //   void show(CParams * pParams, int kParams);
};

ObjListModel::ObjListModel(QObject *parent)
{
//     IsData = false;
//     timer = new QTimer(this);
//     timer->setInterval(1000);
//     connect(timer, &QTimer::timeout , this, &ObjListModel::timerHit);
//     timer->start();
 }

void ObjListModel::Refresh()
{
    QModelIndex item_idx_s = index(0,0);
    QModelIndex item_idx_e = index(RSU_Pnt.L-1,1);
    emit dataChanged(item_idx_s ,item_idx_e );
}

// void ObjListModel::timerHit()
// {
// //    QModelIndex topLeft = createIndex(0,0);
//  //   QModelIndex bottomRight = createIndex(kParams_old-1,1);
// }

int ObjListModel::rowCount(const QModelIndex &parent) const
{
    return 100;//kParams;
}

int ObjListModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}

QVariant ObjListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return QString("RSU");
        case 1:
            return QString("Тип");
        }
    }
    return QVariant();
}

Qt::ItemFlags ObjListModel::flags(const QModelIndex &index) const
{
    int r = index.row();
    int c = index.column();
    return QAbstractTableModel::flags(index);
};

bool ObjListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int r = index.row();
    return false;
}

QVariant ObjListModel::data(const QModelIndex &index, int role) const
{
    if ( NoRefresh )
        return QVariant();
    int r = index.row();
    if ( r >= RSU_Pnt.L )
        return QVariant();
    int c = index.column();
    switch (role)
    {
    case Qt::DisplayRole:
    {
        RSU_Obj * pObj =  RSU_Pnt.Get( r );
        if ( c == 0 )
        {
       return QString( pObj->ObjName );
        }
     if ( pObj->Ref )
        return QString ( pObj->Ref );
     break;
    }
    case Qt::EditRole:
    break;
    case Qt::FontRole:
     break;
    case Qt::ForegroundRole:
    break;
    case Qt::BackgroundRole:
    break;
    case Qt::TextAlignmentRole:
    break;
    case Qt::CheckStateRole:
    break;
    case Qt::UserRole:
    {
//        return QVariant( Var );
    }
    break;
    }
    return QVariant();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool List_W_Model::NoRefresh = true;

List_W_Model::List_W_Model(QObject *parent)
{
    IsData = false;
    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, &QTimer::timeout , this, &List_W_Model::timerHit);
    timer->start();
    NoRefresh = false;
}
void List_W_Model::Refresh()
{
    QModelIndex item_idx_s = index(0,0);
    QModelIndex item_idx_e = index(Params.L-1,1);
    emit dataChanged(item_idx_s ,item_idx_e );
}

void List_W_Model::timerHit()
{
    if ( NoRefresh )
        return;
    // we identify the top left cell
    QModelIndex topLeft = createIndex(0,0);
    QModelIndex bottomRight = createIndex(Params.L-1,1);
    emit dataChanged(topLeft, bottomRight, {Qt::DisplayRole});
}

int List_W_Model::rowCount(const QModelIndex &parent) const
{
    return 100;//kParams;
}

int List_W_Model::columnCount(const QModelIndex &parent) const
{
    return 2;
}

QVariant List_W_Model::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return QString("Параметр");
        case 1:
            return QString("Значение");
        }
    }
    return QVariant();
}

Qt::ItemFlags List_W_Model::flags(const QModelIndex &index) const
{
    return QAbstractTableModel::flags(index);
};

bool List_W_Model::setData(const QModelIndex &index, const QVariant &value, int role)
{
    // int r = index.row();
    // if ( r >= Params.L )
    //     return false;
    // int c = index.column();
    // if ( c != 1 )
    //     return false;
    // if (role == Qt::EditRole)
    //     {
    //     QString Txt = value.toString();
    //     QParams * pP = Params.Get( r );
    //     pP->SetValue ( (char*)Txt.toStdString().c_str());
    //     NoRefresh = false;
    //     return true;
    //     }
    // if (role == Qt::CheckStateRole)
    // {
    //     QParams * pP = Params.Get( r );
    //     if ( pP->Type == 'B' )
    //     {
    //         if ( value.toInt() == Qt::Checked )
    //             *(bool*)pP->Addr = true;
    //         else
    //             *(bool*)pP->Addr = false;
    //         return true;
    //     }
    //     // Update your internal data structure with the new state
    // }
    return false;
}

QVariant List_W_Model::data(const QModelIndex &index, int role) const
    {
    int r = index.row();
    if ( r >= Params.L )
        return QVariant();
    int c = index.column();
    QParams * pP = Params.Get( r );
    switch (role)
    {
    case Qt::DisplayRole:
        if ( c == 0 )
        {
            char * N = pP->ParamName;
            if ( N[0] == '#' )
                return QString( N + 1 );
            else
                return QString( N );
        }
        char Txt[256];
        pP->Out(Txt);
        return QString ( Txt );
    case Qt::EditRole:
        NoRefresh = true;
    break;
        //   return QTime::currentTime().toString();
    case Qt::FontRole:
        // if (r == 0 && c == 0)
        //   { // change font only for cell(0,0)
        //   QFont boldFont;
        //   boldFont.setBold(true);
        //   return boldFont;
        // }
        break;
    case Qt::ForegroundRole:
    {
      if ( c == 0 && pP->ParamName[0] == '#' )
        {
            return QColor( 255, 0, 0 );
        }
    }
    break;
    case Qt::BackgroundRole:
        // if (r == 1 && c == 1)  // change background only for cell(1,2)
        //   return QBrush(Qt::red);
        break;
    case Qt::TextAlignmentRole:
        //      if (r == 1 && c == 1) // change text alignment only for cell(1,1)
        //        return int(Qt::AlignRight | Qt::AlignVCenter);
        break;
    case Qt::CheckStateRole:
        if ( c == 1 && pP->Type == 'B' )
        {
            bool V = *(bool*)pP->Addr;
            if ( V )
                return Qt::Checked;
            else
                return Qt::Unchecked;
        }
        break;
    case Qt::UserRole:
    {
//        qlonglong Var = (qlonglong)&pParams[r];
//        return QVariant( Var );
    }
    break;
    }
    return QVariant();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool List_P_Model::NoRefresh = true;

List_P_Model::List_P_Model(QObject *parent)
{
    IsData = false;
    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, &QTimer::timeout , this, &List_P_Model::timerHit);
    timer->start();
    NoRefresh = false;
}
void List_P_Model::Refresh()
{
    QModelIndex item_idx_s = index(0,0);
    QModelIndex item_idx_e = index(Params.L-1,1);
    emit dataChanged(item_idx_s ,item_idx_e );
}

void List_P_Model::timerHit()
{
    if ( !IsData )
        return;
    if ( NoRefresh )
        return;
    // we identify the top left cell
    QModelIndex topLeft = createIndex(0,0);
    QModelIndex bottomRight = createIndex(Params.L-1,1);
    emit dataChanged(topLeft, bottomRight, {Qt::DisplayRole});
}

int List_P_Model::rowCount(const QModelIndex &parent) const
{
    return 100;//kParams;
}

int List_P_Model::columnCount(const QModelIndex &parent) const
{
    return 2;
}

QVariant List_P_Model::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return QString("Параметр");
        case 1:
            return QString("Значение");
        }
    }
    return QVariant();
}

Qt::ItemFlags List_P_Model::flags(const QModelIndex &index) const
    {
    int r = index.row();
    int c = index.column();
    if ( r < Params.L && c == 1 )
    {
        QParams * pP = Params.Get( r );
        if ( pP->Type == 'B' )
            return QAbstractTableModel::flags(index) | Qt::ItemIsUserCheckable;
        if ( pP->ParamName[0] != '#' )
            return QAbstractTableModel::flags(index) | Qt::ItemIsEditable; // Разрешить редактирование
    }
    return QAbstractTableModel::flags(index);
};

bool List_P_Model::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int r = index.row();
    if ( r >= Params.L )
        return false;
    int c = index.column();
    if ( c != 1 )
        return false;
    QParams * pP = Params.Get( r );
    if (role == Qt::EditRole)
    {
        QString Txt = value.toString();
        pP->SetValue ( (char*)Txt.toStdString().c_str());
        NoRefresh = false;
        return true;
    }
    if (role == Qt::CheckStateRole)
    {
        if ( pP->Type == 'B' )
        {
            if ( value.toInt() == Qt::Checked )
                *(bool*)pP->Addr = true;
            else
                *(bool*)pP->Addr = false;
            return true;
        }
        // Update your internal data structure with the new state
    }
    return false;
}

QVariant List_P_Model::data(const QModelIndex &index, int role) const
{
    int r = index.row();
    if ( r >= Params.L )
        return QVariant();
    int c = index.column();
    QParams * pP = Params.Get( r );
    switch (role)
    {
    case Qt::DisplayRole:
        if ( c == 0 )
        {
            char * N = pP->ParamName;
            if ( N[0] == '#' )
                return QString( N + 1 );
            else
                return QString( N );
        }
        char Txt[256];
        pP->Out(Txt);
        return QString ( Txt );
    case Qt::EditRole:
        NoRefresh = true;
        if ( c == 1 )
        {
            KKK();
            //     char Txt[256];
            //     pParams[r].Out(Txt);
            //     return QString ( Txt );
        }
        break;
        //   return QTime::currentTime().toString();
    case Qt::FontRole:
        // if (r == 0 && c == 0)
        //   { // change font only for cell(0,0)
        //   QFont boldFont;
        //   boldFont.setBold(true);
        //   return boldFont;
        // }
        break;
    case Qt::ForegroundRole:
    {
        if ( c == 0 && pP->ParamName[0] == '#' )
        {
            return QColor( 255, 0, 0 );
        }
    }
    break;
    case Qt::BackgroundRole:
        // if (r == 1 && c == 1)  // change background only for cell(1,2)
        //   return QBrush(Qt::red);
        break;
    case Qt::TextAlignmentRole:
        //      if (r == 1 && c == 1) // change text alignment only for cell(1,1)
        //        return int(Qt::AlignRight | Qt::AlignVCenter);
        break;
    case Qt::CheckStateRole:
        if ( c == 1 && pP->Type == 'B' )
        {
            bool V = *(bool*)pP->Addr;
            if ( V )
                return Qt::Checked;
            else
                return Qt::Unchecked;
        }
        break;
    case Qt::UserRole:
    {
//        qlonglong Var = (qlonglong)&pParams[r];
//        return QVariant( Var );
    }
    break;
    }
    return QVariant();
}


void ShowRSU::on_Find_clicked()
    {
//    CMem<RSU_Obj, 1024, 1024> RSU_Pnt;
    QString  Filtr = ui->Filtr->currentText();
    QString  File = ui->File->currentText();
    QString  TypeObj = ui->TypeObj->currentText();
    GetRSUPnt ( STR(Filtr), STR(File), STR(TypeObj), &ui->ListObj->Model.RSU_Pnt );
    SetRSURef( );
    ObjListModel::NoRefresh = false;
//    List_W_Model::NoRefresh = true;
//    List_P_Model::NoRefresh = true;
    Refresh();
    }


#include "findobj.h"
#include "ui_findobj.h"
#include "BaseModel.h"
#include "Sheme/showparams.h"

FindObj::FindObj(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FindObj)
{
    ui->setupUi(this);
    QHeaderView * headerV = ui->Tab->verticalHeader();
    headerV->setVisible(false);
    QHeaderView * headerH = ui->Tab->horizontalHeader();
    headerH->setStretchLastSection(true);
    ui->Tab->setModel( &Model );
}

FindObj::~FindObj()
{
    delete ui;
}

void FindObj::on_Find_clicked()
{
    char Txt[1024];
    strcpy ( Txt,  STR(ui->Filtr->currentText()));
    SetList (Txt );
    repaint();
}

void FindObj::on_OnSheme_checkStateChanged(const Qt::CheckState &arg1)
{
    on_Find_clicked();
}


void FindObj::on_Modeles_checkStateChanged(const Qt::CheckState &arg1)
{
    on_Find_clicked();
}


void FindObj::on_OK_clicked()
{
    QModelIndexList selected = ui->Tab->selectionModel()->selectedIndexes();
    for (const QModelIndex &index : selected)
       {
       int r = index.row();
       int c = index.column();
       FindData * pFD = Model.List.Get( r );
       if ( c == 2 )
       {
        char Shema[256];
        strcpy (Shema, pFD->Shema );
        char * P = strchr ( Shema, ';');
        if ( P )
            *P = 0;
       emit ShowSheme  ( Shema,  pFD->pObj->ObjName.Str  );
       }
       else
       {
        ShowParams * pParams = new ShowParams( this, pFD->pObj );
        pParams->setWindowTitle( (char*)pFD->pObj->ObjName.Str );
        pParams->show();
       }
    }
   accept();
}

void FindObj::SetList ( char *szFilter)
    {
    Model.List.L = 0;
    int N = 0;
    int lF = strlen ( szFilter );
    char File[256];
    File[0] = 0;
    bool Modeles = ui->Modeles->checkState() == Qt::Checked;
    bool Sheme = ui->OnSheme->checkState() == Qt::Checked;
    for ( int nObj = 0; nObj < IBaseModel::kObjects; nObj++ )
        {
        IBaseModel * pObj = IBaseModel::AllObjects[nObj];
        bool Res = false;
        if ( lF > 0 )
            {
            char * pName = pObj->ObjName;
            if ( Modeles )
                pName = pObj->Model;
            else
                pName = pObj->ObjName;
            char * pModelName = pObj->Model;
            Res = FiltrName( szFilter, pName );
            }
        if ( !Res )
            continue;
        while ( 1 )
            {
            char * P = (char*)pObj->Shema.Str;
            if ( Sheme && P == NULL )
                break;
            if ( Model.List.L < 1025 )
                {
                FindData & W = Model.List.push_back();
                W.pObj = pObj;
                W.Shema = P;
                }
            if ( P == NULL )
                break;
            char * E = strchr( P, ';' );
            if ( E == NULL )
                break;
            P = E + 1;
            E = strchr( P, ';' );
            if ( E == NULL )
                break;
            }
        }
    }

bool CompName ( int Level, int lN, const char * Name, int lS, char * S )
    {
    if ( Level > 50 )
        return true;
    if ( lN == 0 && lS == 0 )
        return true;
    if ( lS == 1 && S[0] == '*' )
        return true;
    if ( lN > 0 && lS == 0 )
        return false;
    if ( S[0] == '*' )
        {
        for ( int n = 0; n < lN; n++ )
            {
            if ( CompName ( Level + 1, lN - n, Name + n, lS - 1, S + 1 ))
                return true;
            }
        return false;
        }
    if ( S[0] == '?' )
        {
        return CompName ( Level + 1, lN - 1, Name + 1, lS - 1, S + 1 );
        }
//    if ( ToBigEng( S[0] ) != ToBigEng( Name[0] ))
//        return false;
     if (  S[0]  != Name[0])
        return false;
    return CompName ( Level + 1, lN - 1, Name + 1, lS - 1, S + 1 );
    }

bool FindObj::FiltrName( char * szFilter, char * Name )
    {
    int lN = strlen ( Name );
    bool Res = false;
    char * P =  szFilter;
    while ( 1 )
        {
        char * Pn = strchr ( P, ';' );
        if ( Pn )
            *Pn = 0;
        if ( CompName ( 1, lN, Name, lstrlen( P ), P ) )
            {
            if ( Pn )
                *Pn = ';';
            Res = true;
            break;
            }
        if ( Pn )
            {
            *Pn = ';';
            P = Pn + 1;
            }
        else
            break;
        }
    return Res;
    }

 bool FindObjModel::Add ( IBaseModel* pObj )
    {
     if ( List.L >= 1024)
            return false;
     FindData & FD = List.push_back();

    }

   FindObjModel::FindObjModel(QObject *parent)
    {
    }

    int FindObjModel::rowCount(const QModelIndex &parent) const
    {
        return 100;//kVars;
    }

    int FindObjModel::columnCount(const QModelIndex &parent) const
    {
        return 3;
    }

    QVariant FindObjModel::headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
            switch (section) {
            case 0:
                return QString("Объект");
            case 1:
                return QString("Модель");
            case 2:
                return QString("Схема");
            }
        }
        return QVariant();
    }

    Qt::ItemFlags FindObjModel::flags(const QModelIndex &index) const
    {
        int r = index.row();
        int c = index.column();
        if ( c == 1 )
            return QAbstractTableModel::flags(index)|Qt::ItemIsUserCheckable;
        return QAbstractTableModel::flags(index);
    };

    bool FindObjModel::setData(const QModelIndex &index, const QVariant &value, int role)
    {
        // int r = index.row();
        // if ( r >= List.L )
        //     return false;
        // int c = index.column();
        // if ( c != 1 )
        //     return false;
        // // if (role == Qt::EditRole)
        // //   {
        // //   QString Txt = value.toString();
        // //   pParams[r].SetValue ( (char*)Txt.toStdString().c_str());
        // //   NoRefresh = false;
        // //   return true;
        // //   }
        // // Update your internal data structure with the new state
        // return false;
    }

    QVariant FindObjModel::data(const QModelIndex &index, int role) const
    {
        int r = index.row();
        if ( r >= List.L )
            return QVariant();
        int c = index.column();
        switch (role)
        {
        case Qt::DisplayRole:
        {
            if ( c == 0 )
              {
               FindData * FD = List.Get(r);
               return QString( FD->pObj->ObjName.Str );
              }
            if ( c == 1 )
                {
                FindData * FD = List.Get(r);
                return QString( FD->pObj->Model.Str );
                }
            char Txt[256];
            FindData * FD = List.Get(r);
            if ( FD->Shema)
                strcpy ( Txt, FD->Shema );
            char * P = strchr ( Txt, ';');
            if ( P )
                *P = 0;
            return QString ( Txt );
            break;
        }
        case Qt::EditRole:
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
        }
        break;
        case Qt::BackgroundRole:
         break;
        case Qt::TextAlignmentRole:
            //      if (r == 1 && c == 1) // change text alignment only for cell(1,1)
            //        return int(Qt::AlignRight | Qt::AlignVCenter);
            break;
        case Qt::CheckStateRole:
        break;
        case Qt::UserRole:
        break;
        }
        return QVariant();
    }


/*
void CDialFindObj::SaveStr()
{
    int K = (int)m_cComboMask.SendMessage( CB_GETCOUNT, 0, 0 );
    char szPath[_MAX_PATH];
    sprintf_s ( szPath, _MAX_PATH, "%sFindObj.txt", GetDumpPath());
    TakeOffReadOnly( szPath );
    FILE * F; fopen_s ( &F, szPath, "wt" );
    if ( F == NULL )
        return ;
    for ( int n = K-1; n >= 0; n-- )
    {
        m_cComboMask.SendMessage( CB_GETLBTEXT, n, (LPARAM)szPath );
        fprintf ( F, "%s\n", szPath );
    }
    fclose ( F );
}
void CDialFindObj::ReadStr()
{
    char szPath[_MAX_PATH];
    sprintf_s ( szPath, _MAX_PATH, "%sFindObj.txt", GetDumpPath());
    FILE * F; fopen_s ( &F, szPath, "rt" );
    if ( F == NULL )
        return ;
    while ( fgets ( szPath, 1023, F ))
    {
        int L = (int)strlen ( szPath );
        if ( L < 2 )
            continue;
        szPath[L-1] = 0;
        COMBOBOXEXITEM Item;
        memset ( &Item, 0, sizeof ( Item ));
        Item.pszText = szPath;
        Item.mask = CBEIF_TEXT;
        m_cComboMask.SendMessage( CBEM_INSERTITEM, 0, (LPARAM)&Item );
    }
    fclose ( F );
}
void CDialFindObj::SetList (char *szFilter)
{
    int N = 0;
    int lF = lstrlen ( szFilter );
    char File[256];
    File[0] = 0;
    if ( m_bShowConnectedObj )
    {
        m_bShowConnectedObj = FALSE;
        if ( pGetFirstConnection == NULL )
            return;
        if ( pGetNextConnection == NULL )
            return;
        (*pGetFirstConnection)();
        void * pData = NULL;
        char Src[512];
        char Dest[512];
        Dest[0] = ' ';
        Dest[1] = '•';
        while ( (*pGetNextConnection)( &pData, Src, Dest ))
        {
            if ( lstrcmp ( szObjTxtFiltr, Src ) == 0 )
            {
                char * P = strchr ( Dest, '|' );
                *P = 0;
                if ( !m_cObject.Compare (Dest + 2) )
                //if ( lstrcmp ( Dest + 2, Obj ) == 0 )
                    continue;
                m_cListCtrl.AddItem (Dest, P + 1, 0, (DWORD_PTR)pData);
            }
        }
        return;
    }
    for ( int nObj = 0; nObj < IBaseModel::kObjects; nObj++ )
    {
        IBaseModel * pObj = IBaseModel::AllObjects[nObj];
        bool Res = false;
        if ( lF > 0 )
        {
            char * pName = pObj->ObjName;
            char * pModelName = pObj->Model;
            if ( pGetIBaseModelParams )
                (*pGetIBaseModelParams)( pObj, &pName, &pModelName );
            Res = FiltrName( szFilter, pName );
        }
        if ( !Res )
            continue;
        int nImage = 0;//!m_bExtended ? 0 : (pObj->TypeObj == IBaseModel::Y_Obj) ? 2 : (pObj->TypeObj == IBaseModel::RSU_Obj) ? 1 : 0;
        int nASCIndex = (nImage == 0) ? -1 : 0;
        //if ( pObj->TypeObj == IBaseModel::Y_Obj )
        //{
        //	CY_Base *pYBase = (CY_Base *)pObj;
        //	if ( pYBase->controllerType == CY_Base::ectPAZ && m_nPAZIndex >= 0 )
        //		nASCIndex = m_nPAZIndex;
        //}
        m_cListCtrl.AddItem (pObj->ObjName, pObj->Model, nImage, (DWORD_PTR)nObj, nASCIndex);
    }
    // Перебор по моделям RSU
#if 1
    for( int i=0; i<_countof(s_lstAcsFindFunc); ++i )
    {
        if( !s_lstAcsFindFunc[i].pGetF )
            break;
        if( !(s_lstAcsFindFunc[i].pGetF() ))
            continue;
        int nASCIndex = i;
        void * pData = NULL;
        TCHAR szName[512] = { 0 };
        TCHAR szInfo[512] = { 0 };
        DWORD dwType = 0;
        while ( s_lstAcsFindFunc[i].pGetN( dwType, &pData, szName, szInfo ))
        {
            bool Res = FiltrName( szFilter, szName );
            if( Res )
            {
                m_cListCtrl.AddItem (szName, szInfo, 1, (DWORD_PTR)pData, nASCIndex);
            }
            pData = NULL;
        }
    }
#endif
}

*/



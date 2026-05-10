
#include "stdafx.h"
#define DLL_Chem
#include "Chem.h"
#include "math.h"
#include "Err.h"
#include "Data.h"
#include "Prop.h"
#include "SysDataTypes.h"
#include "Connect.h"
#include "Split.h"
#include "ChemBase.h"

#ifdef LINUX
#include "WinFiles.h"
#endif

//#define OLD
CGas     ** pGas = NULL;
CSolid   ** pSolid = NULL;
CVirtual ** pVirtual = NULL;

struct IGamma * pGamma[MAX_GAMMA];

char NameGasComp[K_GAS][64];
char * ListGasComp;
double MwGasComp[K_GAS];
int ID_GasComp[K_GAS];
int N_GasComp[K_GAS];
int AltID_GasComp[K_GAS];

int LAST_GAS = -1;
int LAST_SOLID = -1;
int LAST_VIRT = -1;

int G_CH4 = -1;
int G_N2 = -1;
int G_O2 = -1;
int G_H2O = -1;
int G_OIL = -1;
int G_CO = -1;
int G_CO2 = -1;
int G_H2 = -1;
int G_H2S = -1;
int G_C6N = -1;
int G_C2H6 = -1;
int G_C3H8 = -1;
int G_C4H10N = -1;
int G_C4H10I = -1;
int G_C5H12N = -1;
int G_C5H12I = -1;
int G_C6H14N = -1;
int G_C7H16N = -1;
int G_C8H18N = -1;
int G_C9H20N = -1;
int G_C10H22N = -1;
int G_HE = -1;
int G_AR = -1;
int G_NH3 = -1;
int G_MDEA = -1;
int G_K2CO3 = -1;
int G_CH3OH = -1;
int G_METHANOL = -1;

int G_СуммаS = -1;
int G_Меркаптаны = -1;
int G_Сульфиды = -1;
int G_Дисульфиды = -1;
int G_Тиофены = -1;
int G_Олефины = -1;

int G_ИПБ = -1; 
int G_COKE = -1;

void CChem::InitCompID( )
{
    CNoErrGasID NoErr;
#define COMP(ID,Name) ID = GetGasID( Name );
    //CCompID CompID[] = {
  COMP(G_CH4,"METHANE")
    COMP(G_N2, "NITROGEN")
    COMP(G_O2, "OXYGEN")
    COMP(G_H2O, "WATER")
    COMP(G_CO, "CARBON MONOXIDE")
    COMP(G_CO2, "CARBON DIOXIDE")
    COMP(G_H2, "HYDROGEN")
    COMP(G_H2S, "HYDROGEN SULFIDE")
    COMP(G_C6N, "n-HEXANE")
    COMP(G_MDEA, "DIETHANOLAMINE" )
    COMP(G_C2H6, "ETHANE")
    COMP(G_C3H8, "PROPANE")
    COMP(G_C4H10N, "n-BUTANE")
    COMP(G_C4H10I, "ISOBUTANE")
    COMP(G_C5H12N, "n-PENTANE")
    COMP(G_C5H12I, "ISOPENTANE")
    COMP(G_C6H14N, "n-HEXANE")
    COMP(G_C7H16N, "n-HEPTANE")
    COMP(G_C8H18N, "n-OCTANE")
    COMP(G_C9H20N, "n-NONANE")
    COMP(G_C10H22N, "n-DECANE")
    COMP(G_HE, "He")
    COMP(G_AR, "ARGON")
    COMP(G_NH3, "AMMONIA")
    COMP(G_CH3OH, "METHANOL")
    COMP(G_METHANOL, "METHANOL")
    //
#undef COMP
#define COMP(ID,Name) ID = GetVirtID( Name );
    COMP(G_Меркаптаны, "Меркаптаны")
    COMP(G_Сульфиды, "Сульфиды")
    COMP(G_Дисульфиды, "Дисульфиды")
    COMP(G_Тиофены, "Тиофены")
    COMP(G_Олефины, "Олефины")
#undef COMP
#define COMP(ID,Name) ID = GetSolidID( Name );
    COMP(G_ИПБ,"CUMENE")
    COMP(G_COKE,"COKE")
}


CGas::CGas( )
{
    Mw = 0.;
    Tb = 0.;;
    Tcr = Pcr = Vcr = Rocr = Zcr = Omega = 0.;
    CLEAR(Name);    //Химическое название
    CLEAR(Alt_Name);
    CLEAR(Formula)
    Numb = -1;
    k_C = k_H = k_O = 0.;
    AsWater = false;
    Function = 0;
    pApprCp = NULL;
}

void SetChemAdditions();

CChem::CChem( char * Name ) : IBaseModel ( Name )
    {
    ModelFlags = Flag_NoAll;
    Model = "Химия";
    UseNH3_H2O = false;
    GammaN2 = 1.;
    WasInit = false;
    SetChemAdditions();
    }

CChem_W::CChem_W()
{
    memset ( this, 0, sizeof ( CChem_W ));
}

int CompNumbGasComp ( const void * p1, const void * p2 );
int AltCompGasComp ( const void * p1, const void * p2 );

void CalcISO();

//#include <share.h>



struct CSpecData
{
    char Params[256];
};

extern CReal * pWater_NIST;
enum eInputState { eGasData = 1, eSolidData, eVirtualData };
int CChem::Init( int )
{
    char Path[1024];
    sprintf_s( Path, 1024, "%sDATA\\COMPOSITION\\Components.dat", PROJECT_ROOT );
    _HANDLE hFile = CreateFile ( Path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_ARCHIVE, NULL );
    ASS( hFile != INVALID_HANDLE_VALUE );
    if ( hFile == INVALID_HANDLE_VALUE )
        return 1;
    pGas   = NewArr(CGas*,K_GAS);
    pSolid = NewArr(CSolid*,K_SOLID);
    pVirtual = NewArr(CVirtual*,K_VIRTUAL);
    int nGas = 0;
    int nSolid = 0;
    int nVirt = 0;
    eInputState InputState = eGasData;
    CSplit<8,64> Split;
    int kErr = 0;
    //  int Line = 0;
    while ( 1 )
    {
        //    Line++;
        //    if ( Line > 11 )
        //      KKK();
        DWORD Lr;
        DWORD Key = 0;
        ASS( ReadFile( hFile, &Key, 4, &Lr, 0 ));
        if ( Key == 0x99999999 )
            break;
        switch ( Key )
        {
        case 0x11111111:
        {
            ASS( InputState == eGasData)
            CReal * pReal = new CReal();
            if ( pReal->ReadData( hFile ))
            {
                ModMsg ( "Ошибка чтения Real компоненты" );
                kErr++;
                break;
            }
            //if ( lstrcmp ( pReal->CGas::Name, "WATER" ) == 0 )
            //  {
            //  *pWater_NIST = *pReal;
            //  pReal = pWater_NIST;
            //  }
            pReal->Init();
            pReal->Numb = nGas;
            ASS(nGas<K_GAS)
            pGas[nGas++] = pReal;
        }
        break;
        case 0x22222222:
        {
            /// Временно
            ///
            //        void CreateAllPseudo( int & nGas );
            //        CreateAllPseudo( nGas );
            ASS( InputState == eGasData)
            CPseudo * pPseudo = new CPseudo();
            if ( pPseudo->ReadData( hFile ))
            {
                ModMsg ( "Ошибка чтения Pseudo компоненты" );
                kErr++;
                break;
            }
            pPseudo->Numb = nGas;
            ASS(nGas<K_GAS)
            pGas[nGas++] = pPseudo;
        }
        break;
        case 0x33333333:
        {
            ASS( InputState == eSolidData)
            CSolid * pS = new CSolid();
            if ( pS->ReadData( hFile ))
            {
                ModMsg ( "Ошибка чтения Solid компоненты" );
                kErr++;
                break;
            }
            pS->Init();
            ASS(nSolid<K_SOLID)
            pSolid[nSolid++] = pS;
        }
        break;
        case 0x44444444:
        {
            // может быть чисто Special, а может на основе Real
            DWORD Key = 0x01010101;
            ReadFile( hFile, &Key, sizeof ( Key ), &Lr, 0 );
            if ( Key == 0x11111111 )
            {
                ASS( InputState == eGasData)
                //        На основе Real
                char Param[256];
                ReadFile( hFile, Param, 256, &Lr, 0 );
#ifdef LINUX
                ToUTF8(Param,256);
#endif
                Split.Split( Param );
                // Special	0)-34250;1)-Вода аппр.;2)Вода;3)ChemSpec
                char * pChr = strchr(Split.Cols[3], '/');
                char * DllPath = NULL;
                char * DllName = NULL;
                if( pChr )
                {
                    *pChr = 0;
                    pChr++;
                    DllName = pChr;
                    DllPath = Split.Cols[3];
                }
                else
                {
                    pChr = strchr(Split.Cols[3], '\\');
                    if( pChr )
                    {
                        *pChr = 0;
                        pChr++;
                        DllName = pChr;
                        DllPath = Split.Cols[3];
                    }
                    else
                        DllName = Split.Cols[3];
                }
                _HANDLE hLib = Load_Dll ( DllName, DllPath );
                if ( hLib == NULL )
                {
                    ModMsg ( "Ошибка загрузки DLL '%s/%s'", DllPath, DllName );
                    kErr++;
                    break;
                }
                typedef
                    CGas * (*tGetGasComp) ( const char * CompName );
                typedef
                    CReal * (*tGetRealComp) ( const char * CompName );
                tGetGasComp pGetGasComp = (tGetGasComp)GetProcAddress( (HMODULE)hLib, "GetGasComp" );
                tGetRealComp pGetRealComp = (tGetRealComp)GetProcAddress( (HMODULE)hLib, "GetRealComp" );
                if ( pGetGasComp == NULL && pGetRealComp == NULL )
                {
                    ModMsg ( "DLL '%s' не поддерживает загрузку компонент", Split.Cols[3] );
                    kErr++;
                    break;
                }
                CGas * pGasComp = NULL;
                if ( pGetGasComp )
                {
                    pGasComp = (*pGetGasComp) ( Split.Cols[1] );
                    if ( pGasComp == NULL )
                        pGasComp = (*pGetGasComp) ( Split.Cols[2] );
                }
                CReal * pRealComp = NULL;
                if ( pGasComp == NULL && pGetRealComp )
                {
                    pRealComp = (*pGetRealComp) ( Split.Cols[1] );
                    if ( pRealComp == NULL )
                        pRealComp = (*pGetRealComp) ( Split.Cols[2] );
                }
                if ( pGasComp == NULL && pRealComp == NULL )
                {
                    ModMsg ( "DLL '%s' не поддерживает компоненту '%s'", Split.Cols[3], Split.Cols[1] );
                    kErr++;
                    break;
                }
                if ( pGasComp )
                {
                    strcpy_s ( pGasComp->Alt_Name, 64, Split.Cols[2] );
                    pGasComp->ReadData ( hFile );
                    pGasComp->Init ( );
                    pGasComp->Numb = nGas;
                    lstrcpy ( pGasComp->CGas::Name, Split.Cols[2] );
                    lstrcpy ( pGasComp->CGas::Alt_Name, Split.Cols[2] );
                    ASS(nGas<K_GAS)
                    pGas[nGas++] = pGasComp;
                }
                if ( pRealComp )
                {
                    strcpy_s ( pRealComp->Alt_Name, 64, Split.Cols[2] );
                    pRealComp->ReadData ( hFile );
                    pRealComp->Numb = nGas;
                    lstrcpy ( pRealComp->CGas::Name, Split.Cols[1] );
                    lstrcpy ( pRealComp->CGas::Alt_Name, Split.Cols[2] );
                    pRealComp->Init ( );
                    ASS(nGas<K_GAS)
                    pGas[nGas++] = pRealComp;
                }
                break;
            }
            ////////////////////////////////////////////////////
            ASS(Key == 0x00000000 )
            CSpecData Data;
            ReadFile( hFile, &Data, sizeof ( Data ), &Lr, 0 );
            Split.Split( Data.Params );
            // 0);1)Вода аппр.;2)Вода;3)ChemSpec
            //
            char* pChr = strchr(Split.Cols[3], '/');
            if(pChr)
            {
                *pChr = 0;
                pChr++;
            }
            else
            {
                pChr = strchr(Split.Cols[3], '\\');
                if(pChr)
                {
                    *pChr = 0;
                    pChr++;
                }
            }
#ifndef LINUX
            _HANDLE hLib = Load_Dll ( pChr, Split.Cols[3] );
            if ( hLib == NULL )
            {
                ModMsg ( "Ошибка загрузки DLL '%s'", Split.Cols[3] );
                kErr++;
                break;
            }
            if ( InputState == eGasData )
            {
                /// специальная газовая компонента
                typedef
                    CGas * (*tGetGasComp) ( const char * CompName );
                tGetGasComp pGetGasComp = (tGetGasComp)GetProcAddress( (HMODULE)hLib, "GetGasComp" );
                if ( pGetGasComp == NULL )
                {
                    ModMsg ( "DLL '%s' не поддерживает загрузку компонент", Split.Cols[3] );
                    kErr++;
                    break;
                }
                CGas * pComp = (*pGetGasComp) ( Split.Cols[1] );
                if ( pComp == NULL )
                {
                    ModMsg ( "Компонента '%s' не найдена в '%s.dll'", Split.Cols[1], Split.Cols[3] );
                    kErr++;
                    break;
                }
                strcpy_s ( pComp->Alt_Name, 64, Split.Cols[2] );
                pComp->Numb = nGas;
                pComp->Init();
                ASS(nGas<K_GAS)
                pGas[nGas++] = pComp;
                break;
            }
            if ( InputState == eSolidData )
            {
                // специальная твердая компонента
                typedef
                    CSolid * (*tGetSolidComp) ( const char * CompName );
                tGetSolidComp pGetSolidComp = (tGetSolidComp)GetProcAddress( (HMODULE)hLib, "GetSolidComp" );
                if ( pGetSolidComp == NULL )
                {
                    ModMsg ( "DLL '%s' не поддерживает загрузку компонент", Split.Cols[3] );
                    kErr++;
                    break;
                }
                CSolid * pComp = (*pGetSolidComp) ( Split.Cols[1] );
                ASS(nSolid<K_SOLID)
                pSolid[nSolid++] = pComp;
                break;
            }
            if ( InputState == eVirtualData )
            {
                // специальная виртуальная компонента
                typedef
                    CVirtual * (*tGetVirtualComp) ( const char * CompName );
                tGetVirtualComp pGetVirtualComp = (tGetVirtualComp)GetProcAddress( (HMODULE)hLib, "GetVirtualComp" );
                if ( pGetVirtualComp == NULL )
                {
                    ModMsg ( "DLL '%s' не поддерживает загрузку компонент", Split.Cols[3] );
                    kErr++;
                    break;
                }
                CVirtual * pComp = (*pGetVirtualComp) ( Split.Cols[1] );
                ASS(nVirt<K_VIRTUAL)
                pVirtual[nVirt++] = pComp;
                break;
            }
#endif
        }
        break;
        case 0x55555555:
        {
            // Переключение типов на несмешиваемые
            InputState = eSolidData;
        }
        break;
        case 0x66666666:
        {
            // Переключение типов на детализация
            InputState = eVirtualData;
        }
        break;
        default:
            ModMsg ( "Ошибка чтения файла компонентов");
            kErr++;
            return 1;
        }
    }
    if ( kErr > 0 )
        return kErr;
    LAST_GAS = nGas;
    LAST_SOLID = nSolid;
    LAST_VIRT = nVirt;
    CloseHandle( hFile );
    InitCompID( );
    void ReadApprC();
    ReadApprC();
    void PrtCp();
    PrtCp();
    ////////////
    //
    //=======================================================
    CNoErrGasID NoErr;
    ////////////////////////////////
    ASS(G_N2>=0)
    ASS(G_O2>=0)
    ASS(G_H2O>=0)
    if ( G_H2O > 0 )
        pGas[G_H2O]->AsWater  = true;
    if ( G_MDEA > 0 )
        pGas[G_MDEA]->AsWater  = true;
    if ( G_METHANOL > 0 )
        pGas[G_METHANOL]->AsWater  = true;
    pGas[G_N2]->GlobalGamma = GammaN2;
    CalcChem_H = false;
    //////////////////////////////////////////////////////////////
    SetS_G( G_AR, 155. );
    SetS_G( G_CO, 197.5 );
    SetS_G( G_CO2, 213.7 );
    SetS_G( G_CH4, 186.19 );
    SetS_G( G_C2H6, 229.1 );
    SetS_G( G_C3H8, 270.1 );
    SetS_G( G_H2, 130.52 );
    SetS_G( G_H2O, 188.74 );
    SetS_G( G_N2, 191.5 );
    SetS_G( G_NH3, 192.5 );
    SetS_G( G_O2, 205.03 );
    SetS_G( G_H2S, 205.64 );
    //////////////////////////////////////////////////////////////
    SetS_G( G_MDEA, 200. );
    SetS_G( G_METHANOL, 160.7 );
    //////////////////////////////////////////////////////////////

    static bool PrtComponents = false;
    void * Prt = NULL;
    if ( PrtComponents )
    {
        Prt = Fopen ( "Компоненты.csv", "wt" );
        Fprintf ( Prt, "Имя;Имя;Формула;Ткипения;\n" );
    }
    //
    for ( int n = 0; n < LAST_GAS; n++ )
    {
        if ( PrtComponents )
        {
            Fprintf( Prt, "%s;%s;%s;%3.1lf;\n", pGas[n]->Name, pGas[n]->Alt_Name, pGas[n]->Formula, pGas[n]->Tb );
            //      pGas[n]->Print ( Prt );
        }
        lstrcpy( NameGasComp[n], pGas[n]->Name );
        MwGasComp[n] = pGas[n]->Mw;
        ID_GasComp[n] = n;
        AltID_GasComp[n] = n;
    }
    if ( PrtComponents )
        Fclose( Prt );
    PrtComponents = false;
    //
    int K = LAST_GAS;
    qsort( ID_GasComp, LAST_GAS, sizeof(int), CompNumbGasComp );
    //
    void SetCompList ( char * PreTxt );
    SetCompList ( "Сумма всех" );
    // Чтение альтернативных имен
    sprintf_s( Path, 1024, "%sDATA\\COMPOSITION\\Синонимы.csv", PROJECT_ROOT );
    FILE * F = fopen ( Path, "rt" );;
    if ( F )
    {
        char Str[256];
        int kLine = 0;
        while ( fgets ( Str, 255, F ))
        {
            kLine++;
            int L = lstrlen( Str ) - 1;
            if ( L >= 0 )
                Str[L] = 0;
            char * Name = Str;
            char * Alt = strchr ( Name, ';' );
            if ( Alt == NULL )
                continue;
            *Alt = 0;
            Alt++;
            char * End = strchr ( Alt, ';' );
            if ( End )
                *End = 0;
            //
            int N = FindGasComp ( Name );
            if ( N >= 0 )
                lstrcpy ( pGas[N]->Alt_Name, Alt );
            else
                KKK();
        }
        fclose ( F );
    }
    qsort( AltID_GasComp, LAST_GAS, sizeof(int), AltCompGasComp );
    ///////////////////////////////
    //  void TestAllPseudo(  );
    //  TestAllPseudo(  );
    //  void TestVis();
    //  TestVis();
    //  void TestPseudo ( );
    //  TestPseudo( );
    return 0;
}

int CChem::Step0()
{
    if ( UseNH3_H2O )
        pGamma[0] =  GetPropInt<IGamma*>( "Gamma", "Аммиак+Вода" );
    else
        pGamma[0] = NULL;
    return 0;
}

int CChem::Step1()
{
    return 0;
}

//IChemParams * IChemParams::pFirsrtChemParam = NULL;

int CChem::GetParams( char * StrName )
{
#include "IO_Parms.h"
    TAB("Gamma",1)
    Char<128>Name;
    for ( int c = 0; c < LAST_GAS; c++ )
    {
        int N = ID_GasComp[c];
        Name.Prt ( "Gamma '%s'", pGas[N]->Name );
        PARM ( pGas[N]->GlobalGamma, Name );
    }
    ETAB
        TAB("Дополнения",1)
        PARM ( UseNH3_H2O, "Аммиак+Вода" );
    PARM ( CReal::Max_Pz, "Максимальное давление" );
    PARM ( GammaN2, "Gamma N2" );
    ETAB
        //  if ( IChemParams::pFirsrtChemParam )
        //    IChemParams::pFirsrtChemParam->GetParams( this );
        return 0;
}

int CChem::UpdateParam( struct CParams & Param )
{
    if ( Param.Addr == &UseNH3_H2O )
    {
        if ( UseNH3_H2O )
            pGamma[0] =  GetPropInt<IGamma*>( "Gamma", "Аммиак+Вода" );
        else
            pGamma[0] = NULL;
    }
    if ( Param.Addr == &GammaN2 )
    {
        if ( G_N2 >= 0 )
            pGas[G_N2]->GlobalGamma = GammaN2;
    }
    //  if ( IChemParams::pFirsrtChemParam )
    //    IChemParams::pFirsrtChemParam->UpdateParam( Param );
    return 0;
}

int CChem::SaveState( )
{
    S_CLASS("W",CChem_W)
    return 0;
}

int CChem::RestoreState( char * StrName )
{
    R_CLASS("W",CChem_W)
    return 1;
}


int CompNumbGasComp ( const void * p1, const void * p2 )
{
    int n1 = *(int*)p1;
    int n2 = *(int*)p2;
    return lstrcmp ( pGas[n1]->Name, pGas[n2]->Name );
}

int AltCompGasComp ( const void * p1, const void * p2 )
{
    int n1 = *(int*)p1;
    int n2 = *(int*)p2;
    return lstrcmp ( pGas[n1]->Alt_Name, pGas[n2]->Alt_Name );
}

int CompNumbNameGasComp ( const void * p1, const void * p2 )
{
    char * n1 = (char*)p1;
    int n2 = *(int*)p2;
    return lstrcmp ( n1, NameGasComp[n2] );
}

int AltCompNumbNameGasComp ( const void * p1, const void * p2 )
{
    char * n1 = (char*)p1;
    int n2 = *(int*)p2;
    return lstrcmp ( n1, pGas[n2]->Alt_Name );
}

int FindGasComp ( char * Name )
{
    int L = LAST_GAS;
    int * Res = (int*)bsearch( Name, ID_GasComp, LAST_GAS, sizeof(int), CompNumbNameGasComp );
    if ( Res == NULL )
        return -1;
    return *Res;
}

int AltFindGasComp ( char * AltName )
{
    int * Res = (int*)bsearch( AltName, AltID_GasComp, LAST_GAS, sizeof(int), AltCompNumbNameGasComp );
    if ( Res == NULL )
        return -1;
    return *Res;
}

bool MustGasID = false;

bool CNoErrGasID::NoErr = true;

int GetGasID ( char * FullName )
{
    char AltName[256];
    lstrcpy ( AltName, FullName );
    char * Name = AltName;
    while ( 1 )
    {
        char * Alt = strchr ( Name, '|' );
        if ( Alt )
            *Alt = 0;
        for ( int n = 0; n < LAST_GAS; n++ )
        {
            if ( strcmp ( pGas[n]->Name, Name ) == 0 )
                return n;
            if ( strcmp ( pGas[n]->Alt_Name, Name ) == 0 )
                return n;
        }
        if ( Alt == NULL )
            break;
        Name = Alt + 1;
    }
    if ( !CNoErrGasID::NoErr )
    {
        ASS(0)
    }
    return -1;
}

int GetSolidID ( char * FullName )
{
    char AltName[256];
    lstrcpy ( AltName, FullName );
    char * Name = AltName;
    while ( 1 )
    {
        char * Alt = strchr ( Name, '|' );
        if ( Alt )
            *Alt = 0;
        for ( int n = 0; n < LAST_SOLID; n++ )
        {
            if ( strcmp ( pSolid[n]->Name, Name ) == 0 )
                return n;
        }
        if ( Alt == NULL )
            break;
        Name = Alt + 1;
    }
    if ( !CNoErrGasID::NoErr )
    {
        ASS(0)
    }
    return -1;
}

int GetVirtID ( char * FullName )
{
    char AltName[256];
    lstrcpy ( AltName, FullName );
    char * Name = AltName;
    while ( 1 )
    {
        char * Alt = strchr ( Name, '|' );
        if ( Alt )
            *Alt = 0;
        for ( int n = 0; n < LAST_VIRT; n++ )
        {
            if ( strcmp ( pVirtual[n]->Name, Name ) == 0 )
                return n;
        }
        if ( Alt == NULL )
            break;
        Name = Alt + 1;
    }
    if ( !CNoErrGasID::NoErr )
    {
        ASS(0)
    }
    return -1;
}

// void Test()
//   {
//   CChem Chem ( "Химия" );
//   Chem.Init( 1 );
//   Chem.InitCompID( );
//   double T1 = 20;
//   double T2 = 100;
//   double Nu_gas1 = pGas[G_O2]->Vis_gas( 1., T1 ) * 1e8;
//   double Nu_gas2 = pGas[G_O2]->Vis_gas( 1., T2 ) * 1e8;
//   double Nu_liq1 = pGas[G_H2O]->Vis_liq( T1 );
//   double Nu_liq2 = pGas[G_H2O]->Vis_liq( T2 );
//   KKK();
//   //  Mix.IsComp[0] = true;
//   //  Mix.IsComp[1] = true;
//   //  Mix.Z[0] = 0.5;
//   //  Mix.Z[1] = 0.5;
//   ////  Mix.P = 1e3 * Pascal;
//   ////  Mix.T = 300. + TK;
//   //  double Pres = 1.;
//   //  Mix.Pz ( 300., Pres );
//   //  KKK();
//   }

void ReadApprC()
{
    char Path[1024];
    sprintf_s( Path, 1024, "%sData\\APPR\\ApprCp.csv", PROJECT_ROOT );
    CSplit<128,32,1024> Data;
    if ( !Data.Open( Path, false ))
        return;
    while ( Data.Next())
    {
        // Cols[0] - Имя
        CNoErrGasID::NoErr = true;
        int ID = GetGasID( Data.Cols[0] );
        if ( ID < 0 )
            continue;
        int Type = atoi ( Data.Cols[1] );
        if ( Type == 1 ) // Пока тип = 1 Tmin,Tmax,Step,Cp...
        {
            double Tmin = atof ( Data.Cols[2] );
            double Tmax = atof ( Data.Cols[3] );
            double Step = atof ( Data.Cols[4] );
            if ( Step < 1. )
            {
                ModMsg ( "Ошибка 1 при обработке аппроксимации '%s'", Data.Cols[0] );
                continue;
            }
            int kCp = (int)((Tmax-Tmin)/Step)+1;
            if ( kCp > 128 )
            {
                ModMsg ( "Ошибка 2 при обработке аппроксимации '%s'", Data.Cols[0] );
                continue;
            }
            if ( Data.kCol < kCp + 5 )
            {
                ModMsg ( "Ошибка 3 при обработке аппроксимации '%s'", Data.Cols[0] );
                continue;
            }
            double Cp[128];
            for ( int n = 0; n < kCp; n++ )
                Cp[n] = atof ( Data.Cols[n+5] );
            CLinApprCp * pAppr = new CLinApprCp;
            if ( !pAppr->Init ( Tmin, Tmax, Step, Cp ))
            {
                ModMsg ( "Ошибка 4 при обработке аппроксимации '%s'", Data.Cols[0] );
                continue;
            }
            pGas[ID]->pApprCp = pAppr;
            continue;
        }
        ModMsg ( "Ошибка 5 при обработке аппроксимации '%s'", Data.Cols[0] );
    }
    Data.Close( );
}
///////////////////////////////////////////////////////////////////////////////////////
void PrtCp()
{
    //   return;
    //   void * F = Fopen ( "Cp.csv", "wt" );
    //   Fprintf ( F, "T;" );
    //   for ( int c = 0; c < LAST_GAS; c++ )
    //     {
    //     Fprintf ( F, "%s;", pGas[c]->Name );
    //     }
    //   Fprintf ( F, "\n" );
    //   for ( double T = 0.; T <= 300.; T += 10 )
    //     {
    //     Fprintf ( F, "%5.0lf;", T );
    //     for ( int c = 0; c < LAST_GAS; c++ )
    //       {
    //       double С = pGas[c]->C_IG ( T );
    //       Fprintf ( F, "%5.3lf;", С );
    //       }
    //     Fprintf ( F, "\n" );
    //     }
    //   Fclose ( F );
    // //............
    //   F = Fopen ( "H.csv", "wt" );
    //   Fprintf ( F, "T;" );
    //   for ( int c = 0; c < LAST_GAS; c++ )
    //     {
    //     Fprintf ( F, "%s;", pGas[c]->Name );
    //     }
    //   Fprintf ( F, "\n" );
    //   for ( double T = 0.; T <= 1500.; T += 10 )
    //     {
    //     Fprintf ( F, "%5.0lf;", T );
    //     for ( int c = 0; c < LAST_GAS; c++ )
    //       {
    //       double H = pGas[c]->H_IG ( T );
    //       Fprintf ( F, "%5.3lf;", H );
    //       }
    //     Fprintf ( F, "\n" );
    //     }
    //   Fclose ( F );
    /**/
}

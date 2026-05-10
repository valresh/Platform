#include "stdafx.h"
#include "Y_Data.h"
#include "StdDef.h"
#include "Y_Draw.h"
#include <ConnectY.h>
#include "ReadParms.h"
#include "GetObj.h"
#include <vector>
#include <macros/StrHelps.h>

FILE * fopen_err ( char * File, char * Mode );


CY_LC64_W::CY_LC64_W()
	{
	memset ( this, 0, sizeof ( CY_LC64_W ));
	}

CY_LC64_E_Base::CY_LC64_E_Base() : CY_Base()
{
	hWnd = NULL;
	DX_Cond_S = 0;
	DY_Cond_S = 0;
	DX_Cond_L = 0;
	DY_Cond_L = 0;
	kObj = 0;
	kISS = 0;
	kCOS = 0;
	StopOnChange = false;
  m_timeHolder = 0;
//
	kDATA = 0;
	kMTRX = 0;
	kMTRX_CONN = 0;
	kMTRX_PNT = 0;
  VarCond = NULL;
  VarAct = NULL;
	Node = NULL;
  Blk = NULL;
	Timer = NULL;
  RefName = NULL;
	OrdNode = NULL;
	OrdISS = NULL;
	OrdCOS = NULL;
	IO = NULL;
  FT = NULL;
  Pnt = NULL;
  execStartupAtInitialColdStart_Restart = true;
  pSTs64 = pPTs64 = NULL;
  m_LastWorkStep = 0;
}

#include <SR_DCU.h>

bool CY_LC64_E_Base::SaveParms( HANDLE hFile )
{
  DWORD Lw;
  WRT_KEY(eYKeyLC64begin)
  WRT_VAR(kDATA)
  WRT_VAR(kMTRX)
  WRT_VAR(kMTRX_CONN)
  WRT_VAR(kMTRX_PNT)
  if( kDATA )
  {
    WRT_CLASS(BYTE,kDATA,Timer)
	WRT_CLASS(SBlkLC64,kDATA,Blk)
	WRT_CLASS(C_LC64_Node,kDATA,Node)	
	WRT_CLASS(charMx32,kDATA,RefName)
  }
////////////////////////////
  if( kMTRX_CONN )
  {
    WRT_CLASS(C_LC64_FT,kMTRX_CONN,FT)
  }
  if( kMTRX_PNT )
  {
    WRT_CLASS(C_LC64_Pnt,kMTRX_PNT,Pnt)
  }
////////////////////////////
  WRT_KEY(eYKeyLC64end)
  return false;
}

bool CY_LC64_E_Base::RestParms( HANDLE hFile )
{
  DWORD Lr;
  READ_KEY(eYKeyLC64begin, false)
  READ_VAR(kDATA)
  READ_VAR(kMTRX)
  READ_VAR(kMTRX_CONN)
  READ_VAR(kMTRX_PNT)  
  if( kDATA )
  {
    Timer = MemAlloc<BYTE>(kDATA);
    RefName = MemAlloc<charMx32>(kDATA);
    READ_CLASS(BYTE,kDATA,Timer)
    READ_CLASS(SBlkLC64,kDATA,Blk)
    READ_CLASS(C_LC64_Node,kDATA,Node)	
    READ_CLASS(charMx32,kDATA,RefName)
  }
////////////////////////////
  if( kMTRX_CONN )
  {
    READ_CLASS(C_LC64_FT,kMTRX_CONN,FT)
  }
  if( kMTRX_PNT )
  {
    READ_CLASS(C_LC64_Pnt,kMTRX_PNT,Pnt)
  }
////////////////////////////
  READ_KEY(eYKeyLC64end, false)
  if( kDATA )
  {
    VarCond = new tCond_[kDATA];
    VarAct = new tAction_[kDATA];
    OrdNode = MemAlloc<int>(kDATA);
    OrdISS = MemAlloc<int>(kDATA);
    OrdCOS = MemAlloc<int>(kDATA);
    IO = MemAlloc<tIO_>(kDATA);
  }
  return false;
}

static int s_nLastAssignedTimerN = 0;

bool CY_LC64_E_Base::ReadParm( char * ParmName, char * ParmValue, BYTE * pData )
{
  if ( strcmp ( ParmName, "MTRX" ))
    return CY_Base::ReadParm( ParmName, ParmValue, pData );

  if ( kMTRX == 0 )
  {
    ReadData( pData );
    s_nLastAssignedTimerN = 0;
  }
  char Fld[40][64] = { 0 };                 
// 0    1         2                 3
//1:@LOC:C14,10001::,HS5005ARA.PV.ON:@FROM:@TO:1,G14,1
//1:@LOC:O09,10702::,PIS10129A.ALRM.LL:,PIS10129A.AOFS.AOF:@FROM:@TO:1,Q08,1:2,R09,20;
  int K = SetFld_X( ParmValue, ':', Fld, false, false, false );
 	ASS(kMTRX < kDATA )
	char * P = strchr ( Fld[2], ',' );
	ASS( P );
	*P = 0;
  P++;
	BYTE Col = Fld[2][0] - 'A';
	BYTE Row = atoi ( &Fld[2][1] ) - 1;
	Node[kMTRX].x = Col;
	Node[kMTRX].y = Row;
// Тип
	const int Tp = atoi ( P );
	ASS(Blk[kMTRX].Tp == Tp)
	if( Tp == 43111 || Tp == 43011 /*OND*/
    || Tp == 44011 || Tp == 44111/*OFFD*/ )
  {
    char ppp[8][64] = { 0 };
    int N = SetFld_X( P, ',', ppp, false, false, true );
    ASS( N<5 );
    ASSD( !strncmp(ppp[1],"sec.", 4) );
		int T = atoi( ppp[2] );
    if( !T )
    {
      ASS( s_nLastAssignedTimerN );
      T = s_nLastAssignedTimerN+1;
    }
    ASS(T);
		Timer[kMTRX] = T;
    s_nLastAssignedTimerN = __max( s_nLastAssignedTimerN, T);
    if( ppp[3][0] )
    {
      if( ppp[3][0]>='0'&&ppp[3][0]<='9')
        pSTs64[T] = atof( ppp[3] );
      else
        pSTs64[T] = 1;
    }
    KKK();
	}
	else
		Timer[kMTRX] = 0;
	if ( Fld[3][0] )
		strcpy_s ( RefName[kMTRX][0], Fld[3]	);
	else
  {
    int n = 0;
    for( int i=4; i<_countof(Fld)-4; ++i, n++ )
    {
      if( '@'==Fld[i][0] )
        break;
      ASS( n<_countof(RefName[kMTRX]) );
      char * Pz = strrchr ( Fld[i], ',' );
      ASSD( Pz );
		  strcpy_s ( RefName[kMTRX][n], Pz + 1	);
    }
    if( RefName[kMTRX][5][0] )
    {
      ASSD(eBLK_Action1Output6==Tp || eBLK_Condition1Input6==Tp );
    }
    else if( RefName[kMTRX][4][0] )
    {
      ASSD( eBLK_Condition1Input5==Tp || eBLK_Action1Output5==Tp );
    }
    else if( RefName[kMTRX][3][0] )
    {
      ASSD(eBLK_Action1Output4==Tp || eBLK_Condition1Input4==Tp );
    }
    else if( RefName[kMTRX][2][0] )
    {
      ASSD(eBLK_Action1Output3==Tp || eBLK_Condition1Input3==Tp );
    }
    else if( RefName[kMTRX][1][0] )
    {
      ASSD(eBLK_Action1Output2==Tp || eBLK_Condition1Input2==Tp || eBLK_Action3Output2==Tp || eBLK_Condition3Input2==Tp );
    }
    else if( RefName[kMTRX][0][0] )
    {
      ASSD(eBLK_Action1Output1==Tp || eBLK_Condition1Input1==Tp 
        || eBLK_Condition2==Tp || eBLK_Condition2Input1==Tp
        || eBLK_Action2==Tp || eBLK_Action2Output1==Tp
        || eBLK_Action3Output1==Tp || eBLK_Condition3Input1==Tp
        || eBLK_Action5Output1==Tp || eBLK_Action4Output1==Tp || eBLK_Action6Output1
        );
    }
	}
	kMTRX++;
	ASS ( kMTRX <= kDATA )
  return true;
  }

void CY_LC64_E_Base::ReadData( BYTE * pData )
{
  SYokogawa200* hdr = (SYokogawa200*)(char*)pData;
  SYokogawaID*  pGBLK = FindYokoID( hdr, "GBLK" );
  ASS( pGBLK )
  if( !pGBLK )
    return;

  kDATA = pGBLK->dwCount;
  ASS ( kDATA > 0 )
//............................................................
	Node = new C_LC64_Node[kDATA];
  Blk = new SBlkLC64[kDATA];
	Timer = MemAlloc<BYTE>(kDATA);
  RefName = MemAlloc<charMx32>(kDATA);
  VarCond = new tCond_[kDATA];
  VarAct = new tAction_[kDATA];
	OrdNode = MemAlloc<int>(kDATA);
	OrdISS = MemAlloc<int>(kDATA);
	OrdCOS = MemAlloc<int>(kDATA);
	IO = MemAlloc<tIO_>(kDATA);
  //............................................................

	for ( DWORD n = 0; n < kDATA; n++ )
	{
    SGBLK_LC64_HDR *p = (SGBLK_LC64_HDR*)pGBLK->Item( pData, n );
		ASS( p->Numb == n + 1 );
    MoveMemory( &Blk[n], p, sizeof(Blk[n]) );
    Blk[n].dHeight = Blk[n].dWidth = 0;
    if( hdr->version.majorVer == 0x4 )
    {
      if( eBLK_Condition2==p->Tp )
      {
        Blk[n].y -= 97;
        Blk[n].dWidth = 47;
      }
    }
    if( hdr->version.majorVer == 0x6 )
    {
      if( eBLK_Condition2==p->Tp || eBLK_Condition3Input1==p->Tp )
      {
        Blk[n].dWidth = -7;
      }
    }
	}
//GCNC - связи
  SYokogawaID*  pGCNC = FindYokoID( hdr, "GCNC" );
  ASS( pGCNC )
  if( !pGCNC )
    return;

	kMTRX_CONN = pGCNC->dwCount;
  ASS( kMTRX_CONN > 0 )
  //
  FT = new C_LC64_FT[kMTRX_CONN];
	for ( DWORD n = 0; n < kMTRX_CONN; n++ )
	{
    SGCNC_LC64 *p = (SGCNC_LC64*)pGCNC->Item( pData, n );
		FT[n].o_f = p->ObjFrom;
		FT[n].n_f = p->NodeFrom;
		FT[n].o_t = p->ObjTo;
		FT[n].n_t = p->NodeTo;
	}
// точки линий рисунка
//GCOD
  SYokogawaID*  pGCOD = FindYokoID( hdr, "GCOD" );
  ASS( pGCOD )
  if( !pGCOD )
    return;

	kMTRX_PNT = pGCOD->dwCount;
  ASS( kMTRX_PNT > 0 )
  Pnt = new C_LC64_Pnt[kMTRX_PNT];
	for ( DWORD n = 0; n < kMTRX_PNT; n++ )
	{
    SGCOD_LC64 *p = (SGCOD_LC64*)pGCOD->Item( pData, n );
    Pnt[n].x = p->x;
		Pnt[n].y = p->y;
	}
}

static C_LC64_Node * pNodes;
int CompISS(const void * p1, const void *p2 )
	{
	int i1 = *(int*)p1;
	int i2 = *(int*)p2;
	C_LC64_Node & n1 = pNodes[i1];
	C_LC64_Node & n2 = pNodes[i2];
	if ( n1.y < n2.y )
		return -1;
	if ( n1.y > n2.y )
		return 1;
	if ( n1.x < n2.x )
		return -1;
	if ( n1.x > n2.x )
		return -1;
	return 0;
	}

int CompCOS(const void * p1, const void *p2 )
	{
	int i1 = *(int*)p1;
	int i2 = *(int*)p2;
	C_LC64_Node & n1 = pNodes[i1];
	C_LC64_Node & n2 = pNodes[i2];
	if ( n1.x < n2.x )
		return -1;
	if ( n1.x > n2.x )
		return 1;
	if ( n1.y < n2.y )
		return -1;
	if ( n1.y > n2.y )
		return 1;
	return 0;
	}

int CY_LC64_E_Base::Prepare1()
{
	for ( DWORD n = 0; n < kMTRX; n++ )
	{
    Node[n].Time = 0;
    for( int i=0; i<maxInputOutput; ++i )
    {
      if( IO[n][i] < 0 && VarCond[n][i].pVar && VarCond[n][i].IsMethod() )
      {
        VarCond[n][i].Cond();
      }
    }
  }
	kObj = kMTRX;
//
	kISS = 0;
	kCOS = 0;
	for ( DWORD n = 0; n < kMTRX; n++ )
	{
		if ( Blk[n].Tp == eBLK_Action1Output2 
      || Blk[n].Tp == eBLK_Action1Output1 
      || Blk[n].Tp == eBLK_Action1Output3 
      || Blk[n].Tp == eBLK_Action1Output4
      || Blk[n].Tp == eBLK_Action1Output5
      || Blk[n].Tp == eBLK_Action1Output6
      || Blk[n].Tp == eBLK_Action2
      || Blk[n].Tp == eBLK_Action2Output1
      || Blk[n].Tp == eBLK_Action3Output1
      || Blk[n].Tp == eBLK_Action3Output2
      || Blk[n].Tp == eBLK_Action4Output1
      || Blk[n].Tp == eBLK_Action5Output1
      || Blk[n].Tp == eBLK_Action6Output1 )
			continue;//Aсt
		if ( Blk[n].Tp == eBLK_Condition1Input1 
      || Blk[n].Tp == eBLK_Condition1Input2
      || Blk[n].Tp == eBLK_Condition1Input3
      || Blk[n].Tp == eBLK_Condition1Input4
      || Blk[n].Tp == eBLK_Condition1Input5
      || Blk[n].Tp == eBLK_Condition1Input6
      || Blk[n].Tp == eBLK_Condition2
      || Blk[n].Tp == eBLK_Condition2Input1
      || Blk[n].Tp == eBLK_Condition3Input1
      || Blk[n].Tp == eBLK_Condition3Input2
      )
		{//Cond
			OrdISS[kISS++] = n;
			continue;
		}
		OrdCOS[kCOS++] = n;
	}
	pNodes = Node;
	if ( kISS > 1 )
		qsort( OrdISS, kISS, 4, CompISS );
	if ( kCOS > 1 )
		qsort( OrdCOS, kCOS, 4, CompCOS );
	return 0;
}

bool CY_LC64_E_Base::CreatePSMGraf( struct CGrafParm ** GP )
	{
	if ( pCreateLCWnd )
		(*pCreateLCWnd) ( Name, hWnd, this );	
	return true;
	}

typedef std::vector<SCond_ST16_LC64_Consts> tForConstCondCont_;
static tForConstCondCont_ s_bufferForConstCond;

int CY_LC64_E_Base::SaveState( )
{
  DWORD Key = emkcStruct;
  if( !pYokoStateSer->SimpleWrite( &Key, sizeof( Key )) ) return -1;
  if( !pYokoStateSer->WriteStr( "Nodes" ) ) return -2;
  if( (sizeof(*Node)*kMTRX) != pYokoStateSer->WriteSized( Node, sizeof(*Node)*kMTRX) ) 
    return -3;

  if( kDATA )
  {
    s_bufferForConstCond.resize( kDATA );
    DWORD i = 0;
    for( tForConstCondCont_::iterator it=s_bufferForConstCond.begin(), end(s_bufferForConstCond.end()); it!=end; ++it, ++i )
    {
      tForConstCondCont_::value_type &c = *it;
      c.Const_No = VarCond[i][0].Const_No;
      c.Const_Yes = VarCond[i][0].Const_Yes;
    }
    if( !pYokoStateSer->SimpleWrite( &Key, sizeof( Key )) ) return -4;
    if( !pYokoStateSer->WriteStr( "Consts" ) ) return -5;
    if( sizeof(tForConstCondCont_::value_type)*s_bufferForConstCond.size()!=pYokoStateSer->WriteSized( &s_bufferForConstCond[0], sizeof(tForConstCondCont_::value_type)*(DWORD)s_bufferForConstCond.size() ) )
      return -6;
  }
	return 0;
}

static std::vector<BYTE> s_bufferForNodes;

int CY_LC64_E_Base::RestoreState ( char * StrName )
{
  execStartupAtInitialColdStart_Restart = false;
	if ( CY_Base::RestoreState ( StrName ) == 0 )
		return 0;
  if( !strcmp(StrName,"Nodes") )
  {
    DWORD inMem = pYokoStateSer->BytesCanRead();
    if( kMTRX )
    {
      s_bufferForNodes.resize( sizeof(*Node)*kMTRX );
      if( s_bufferForNodes.size()==pYokoStateSer->BytesCanRead() )
      {
        pYokoStateSer->SimpleRead( &s_bufferForNodes[0], (DWORD)s_bufferForNodes.size() );
        C_LC64_Node *pSrc = (C_LC64_Node *)&s_bufferForNodes[0];
        for ( DWORD i = 0; i<kMTRX; i++ )
        {
          Node[i].State = pSrc[i].State;
          Node[i].State2 = pSrc[i].State2;
          Node[i].State3 = pSrc[i].State3;
          Node[i].State4 = pSrc[i].State4;
          Node[i].State5 = pSrc[i].State5;
          Node[i].State6 = pSrc[i].State6;
          Node[i].old_State = pSrc[i].old_State;
          Node[i].old_State2 = pSrc[i].old_State2;
          Node[i].old_State3 = pSrc[i].old_State3;
          Node[i].old_State4 = pSrc[i].old_State4;
          Node[i].old_State5 = pSrc[i].old_State5;
          Node[i].old_State6 = pSrc[i].old_State6;
          Node[i].Time = pSrc[i].Time;
        }
      }
      else
      {
        pYokoStateSer->Shift( pYokoStateSer->BytesCanRead() );
      }
    }
    else
    {
      pYokoStateSer->Shift( pYokoStateSer->BytesCanRead() );
    }
    return 0;
  }
  if( !strcmp(StrName,"Consts") )
	{
    size_t inRS = pYokoStateSer->BytesCanRead();
    if( kDATA )
    {
      s_bufferForConstCond.clear();
      s_bufferForConstCond.resize( kDATA );
      if( (sizeof(tForConstCondCont_::value_type)*s_bufferForConstCond.size())==pYokoStateSer->BytesCanRead() )
      {
        pYokoStateSer->SimpleRead( &s_bufferForConstCond[0], sizeof(tForConstCondCont_::value_type)*(DWORD)s_bufferForConstCond.size() );//ASS(pSRFile->RStruct( sizeof(tForConstCondCont_::value_type)*s_bufferForConstCond.size(), &s_bufferForConstCond[0]) );
        DWORD i = 0;
        for( tForConstCondCont_::iterator it=s_bufferForConstCond.begin(), end(s_bufferForConstCond.end()); it!=end; ++it, ++i )
        {
          tForConstCondCont_::value_type &c = *it;
          VarCond[i][0].Const_No = c.Const_No;
          VarCond[i][0].Const_Yes = c.Const_Yes;
        }
      }
      else
        pYokoStateSer->Shift( pYokoStateSer->BytesCanRead() );
    }
    else
      pYokoStateSer->Shift( pYokoStateSer->BytesCanRead() );
    return 0;
  }
	return 1;
}

int CY_LC64_E_Base::StateSave( IYokoStateSer* pStateSer )
{
  __super::StateSave( pStateSer );
  if( !pStateSer->SimpleWrite( eKeyLocalStruct ) ) return -1;
  if( !pStateSer->WriteStr( "Nodes" ) ) return -2;
  if( (sizeof(*Node)*kMTRX)!=pStateSer->WriteSized( Node, sizeof(*Node)*kMTRX) ) 
    return -3;

  if( kDATA )
  {
    s_bufferForConstCond.resize( kDATA );
    DWORD i = 0;
    for( tForConstCondCont_::iterator it=s_bufferForConstCond.begin(), end(s_bufferForConstCond.end()); it!=end; ++it, ++i )
    {
      tForConstCondCont_::value_type &c = *it;
      c.Const_No = VarCond[i][0].Const_No;
      c.Const_Yes = VarCond[i][0].Const_Yes;
    }
    if( !pStateSer->SimpleWrite( eKeyLocalStruct ) ) return -4;
    if( !pStateSer->WriteStr( "Consts" ) ) return -5;
    if( sizeof(tForConstCondCont_::value_type)*s_bufferForConstCond.size()!=pStateSer->WriteSized( &s_bufferForConstCond[0], sizeof(tForConstCondCont_::value_type)*(DWORD)s_bufferForConstCond.size() ) )
      return -6;
  }
  return 0;
}

int CY_LC64_E_Base::StateRestore( IYokoStateSer* pStateSer )
{
  execStartupAtInitialColdStart_Restart = false;
  __super::StateRestore( pStateSer );
  EStateKeys key = sKeyNull;
  char buf[64] = { 0 };
  DWORD len = 0;

  for( int j=0; j<2; ++j )
  {
    pStateSer->SimpleRead( key );
    if( eKeyLocalStruct==key )
    {}
    else if( eKeyObject==key )
    {
      pStateSer->Shift( -LONG(sizeof(key)) );
      break;
    }
    else
    {
      ASS(0);
    }
    pStateSer->ReadLenStr( buf, _countof(buf) );
    pStateSer->SimpleRead( len );
    pStateSer->SetReadyBytes( len );
    CY_LC64_E_Base::RestoreState( buf );
  }
  pStateSer->SetReadyBytes( 0 );

  

  return 0;
}

int CY_LC64_E_Base::GetParams( KYBridge2SysParam &params )
{
#include "IO_Parms.h"
	MENU("Base")
	CY_Base::GetParams( params );
  PARM( m_LastWorkStep, "#Шаг Срабатывания")
  if( Name[0] )
  {
    SYParam & __P = params.Add( Name, "#Логика" );
    __P.pCallExternWindowHandler = CallDrawLC64Handler;
    __P.domain = pDR->domain;
  }
	E_MENU("Base")
	return 0;
}

bool CY_LC64_E_Base::SetLimits( )
  {
  return true;
  }

bool CY_LC64_E_Base::Draw ( CDrawData * pDD, int x, int y )
	{
	hWnd = pDD -> hWnd;
	return true;
	}


//static C_LC64_Node * pNodes;
int Comp_C_LC64_Node( const void * p1, const void  * p2 )
	{
	int i1 = *(int*)p1;
	int i2 = *(int*)p2;
	C_LC64_Node & n1 = pNodes[i1];
	C_LC64_Node & n2 = pNodes[i2];
	// 1..14
	if ( n1.x < n2.x )
		return -1;
	if ( n1.x > n2.x )
		return 1;
	if ( n1.y < n2.y )
		return -1;
	if ( n1.y > n2.y )
		return 1;
	return 0;
	}

int CY_LC64_E_Base::FindNode ( int x, int y )
	{
	for ( DWORD n = 0; n < kMTRX; n++ )
		{
		C_LC64_Node & N = Node[n];
		if ( N.x == x && N.y == y	)
			return n;
		}
	return -1;
	}

struct CTypeNumb
{
	int Numb;
	char * szType;
  C_LC64_Node::eNodeTypeLogicOperation nt;
};

static const CTypeNumb TypeNumb[] = {
  41001, "OR",    C_LC64_Node::ent_OR,//OR-Left-5
  41011, "OR",    C_LC64_Node::ent_OR,//OR-Up-5
  41031, "OR",    C_LC64_Node::ent_OR,//OR-Down-5
	41041, "OR",    C_LC64_Node::ent_OR,
  41051, "OR",    C_LC64_Node::ent_OR,//OR-Left-19
  41061, "OR",    C_LC64_Node::ent_OR,//OR-Left-31
	45011, "TON",	  C_LC64_Node::ent_TON,
	46011, "TOFF",  C_LC64_Node::ent_TOFF,
	42121, "NOT",   C_LC64_Node::ent_NOT,
	42021, "NOT",	  C_LC64_Node::ent_NOT,
	40001, "AND",	  C_LC64_Node::ent_AND,//AND-Left-13
  40031, "AND",		C_LC64_Node::ent_AND,
  40041, "AND",	  C_LC64_Node::ent_AND,//AND-Left-13
  40051, "AND",	  C_LC64_Node::ent_AND,
  40061, "AND",		C_LC64_Node::ent_AND,
	44011, "OFFD",  C_LC64_Node::ent_OFFD,//+Comment
  44111, "OFFD",  C_LC64_Node::ent_OFFD,
	43011, "OND",	  C_LC64_Node::ent_OND,
  43111, "OND",	  C_LC64_Node::ent_OND,//+Comment
  48021, "SRS1-S",C_LC64_Node::ent_SRS1_S,
  48122, "SRS2-S",C_LC64_Node::ent_SRS2_S,
  50021, "SRS1-R",C_LC64_Node::ent_SRS1_R,
  50122, "SRS2-R",C_LC64_Node::ent_SRS2_R,
  51021, "GT",	  C_LC64_Node::ent_GT,
  52021, "GE",	  C_LC64_Node::ent_GE,
  53021, "EQ",	  C_LC64_Node::ent_EQ,
  47021, "W.O",   C_LC64_Node::ent_WOUT,
  47121, "W.O",   C_LC64_Node::ent_WOUT_Up,
};

void GetST16_LC64CondFunc( char * Cond, CCond_ST16_LC64 * pVC, CY_Base *pObj );
void GetLC64ActFunc ( char * Act, CAct_LC64 * pVA, CY_Base *pObj );
void CorrectLC64ActFunc( char * Act, CAct_LC64 * pVA, CY_Base *pObj );

void CY_LC64_E_Base::Prepare()
{
// Получение связей
	for ( DWORD n = 0; n < kMTRX; n++ )
	{
    const char sep = '.';
		ZeroMemory( IO[n], sizeof(IO[n]) );
    if ( Blk[n].Tp == eBLK_Action1Output2 
      || Blk[n].Tp == eBLK_Action1Output1 
      || Blk[n].Tp == eBLK_Action1Output3 
      || Blk[n].Tp == eBLK_Action1Output4
      || Blk[n].Tp == eBLK_Action1Output5
      || Blk[n].Tp == eBLK_Action1Output6
      || Blk[n].Tp == eBLK_Action2
      || Blk[n].Tp == eBLK_Action2Output1
      || Blk[n].Tp == eBLK_Action3Output1
      || Blk[n].Tp == eBLK_Action3Output2
      || Blk[n].Tp == eBLK_Action4Output1
      || Blk[n].Tp == eBLK_Action5Output1
      || Blk[n].Tp == eBLK_Action6Output1
      )
		{//action
      for( int i=0; i<maxInputOutput; i++ )
      {
			  char * A = RefName[n][i];
        if( !*A )
          break;
        CY_Base * pObj = NULL;

        char * P = strchr ( A, sep );

        TStringTerminator stP( P );
        if( P )
        {
          pObj = GetObj ( A, this );
          *P = sep;
        }
        else
          pObj = this;

        //VarAct[n][i].pObj = pObj;
        if( pObj == NULL )
        {
          VarAct[n][i].Name = RefName[n][i];
          VarAct[n][i].pVar = NULL;
          VarAct[n][i].VarType = ' ';
          continue;
        }
        int nBranch = 0;
        if( P )
        {
          *P = sep;
          P++;
        }
        else
        {
          P = A;
          nBranch = 2;
          static char tmp[12];
          sprintf_s( tmp ,"_%s", P );
          P = tmp;
        }
        char * Q = NULL;
        if( !nBranch )
          Q = strchr( P, sep );

        TStringTerminator stQ( Q );
        void * pVar = NULL;
        void * pTuneVar = NULL; 
        char Type = ' ';
        USHORT varSize = 0;
        CY_Base::eVarType Work = CY_Base::evtP;
        CAlarmBase::tFlagUse_ *pFlagUse = NULL;
        bool bACT = strcmp( P, "ACT" ) ? false : true;
        bool r = pObj->GetVar( P, &pVar, &pTuneVar, &Type, &Work, &pFlagUse, &varSize );
        //ASSD( r );
        if( pObj->pClassBase )
        {
          CAlarmBase *pAB = reinterpret_cast<CAlarmBase*>(pObj->pClassBase);
          pAB->FlagUse |= CAlarmBase::fuYConnect;
        }
        else if( pFlagUse )
          *pFlagUse |= CAlarmBase::fuYConnect;
			  IO[n][i] = 1;
        if( Q )
        {
          *Q = sep;
          Q++;
        }
        VarAct[n][i].Name = RefName[n][i];
        VarAct[n][i].pVar = pVar;
        VarAct[n][i].VarType = Type;
        VarAct[n][i].P_Type = false;

        if( !nBranch )
        {
          GetLC64ActFunc ( P, &VarAct[n][i], pObj );
          CorrectLC64ActFunc( P, &VarAct[n][i], pObj );
        }
        if( Q && (strcmp ( Q, "L" ) == 0 || strcmp ( Q, "H" ) == 0) )
          VarAct[n][i].L_Type = true;
        /*else
          VarAct[n][i].L_Type = false;*/

        if( bACT )
        {
          if( id_BDSET_1L==pObj->Y_Type || id_BDSET_1C==pObj->Y_Type || id_BDSET_2L==pObj->Y_Type || id_BDSET_2C==pObj->Y_Type )
          {
            ASS( 'I'==Type );
            int shift = atoi( Q ) - 1;
            if( shift>=0 && shift < varSize/sizeof(int) )
            {
              VarAct[n][i].pVar = (BYTE*)pVar + shift * sizeof(int);
              VarAct[n][i].pObj = pObj;
            }
            VarAct[n][i].L_Type = true;
          }
        }
        else if( false==VarAct[n][i].L_Type )
        {
          if( id_MC_2==pObj->Y_Type || id_MC_2E==pObj->Y_Type || id_MC_3==pObj->Y_Type || id_MC_3E==pObj->Y_Type )
          {
            VarAct[n][i].pVar = (BYTE*)pVar;
            VarAct[n][i].pObj = pObj;
          }
        }

        CY_Base::ELActLC64 lTest = pObj->IsLActionInLC64( P );
        switch( lTest )
        {
        case elHZ:
          break;
        case elyesL:
          VarAct[n][i].pObj = pObj;
          break;
        case elnoL:
          ASSD(0);
          break;
        }

        if( Q && !strcmp ( Q, "P" ) )
        {
          ASSD(!strcmp(P,"PV.P"));//"разобраться"
          VarAct[n][i].P_Type = true;
        }
      }
    }
		if(  Blk[n].Tp == eBLK_Condition1Input1 
      || Blk[n].Tp == eBLK_Condition2 
      || Blk[n].Tp == eBLK_Condition2Input1
      || Blk[n].Tp == eBLK_Condition1Input2
      || Blk[n].Tp == eBLK_Condition1Input3
      || Blk[n].Tp == eBLK_Condition1Input4
      || Blk[n].Tp == eBLK_Condition1Input5
      || Blk[n].Tp == eBLK_Condition1Input6
      || Blk[n].Tp == eBLK_Condition3Input1
      || Blk[n].Tp == eBLK_Condition3Input2
      )
		{//condition
      for( int i=0; i<maxInputOutput; i++ )
      {
        char * A = RefName[n][i];
        if( !*A )
          break;
        char * P = strchr ( A, sep );
        CY_Base * pObj = NULL;
        TStringTerminator stP( P );
        if( P )
        {
          pObj = GetObj ( A, this );
          if( !pObj )
            continue;
        }
        else
          pObj = this;
        int nBranch = 0;
        if( P )
        {
          *P = sep;
          P++;
        }
        else
        {
          P = A;
          nBranch = 2;
        }
        char sep2 = sep;
        char * Q = strchr ( P, sep2 );
        if( !nBranch && !Q )
        {
          sep2 = '=';
          Q = strchr ( P, sep2 );
          ASS( Q );
          if( !Q )
            continue;
        }
        TStringTerminator stQ( Q );
        void * pVar = NULL;
        void * pTuneVar = NULL; 
        char Type = ' ';
        CY_Base::eVarType Work = CY_Base::evtP;
        if ( pObj )
        {
          bool r = pObj->GetVar( P, &pVar, &pTuneVar, &Type, &Work );
          if( !r )
          {
            static char tmp[12];
            sprintf_s( tmp ,"_%s", P );
            P = tmp;
            r = pObj->GetVar( tmp, &pVar, &pTuneVar, &Type, &Work );
          }
          //ASSD( r );
          if( !r )
            continue;
        }
        IO[n][i] = -1;
        if( Q )
          *Q = sep2;
        VarCond[n][i].Name = RefName[n][i];
        VarCond[n][i].pVar = pVar;
        VarCond[n][i].VarType = Type;
        if( !nBranch )
          GetST16_LC64CondFunc( P, &VarCond[n][i], pObj );
      }//for [][]
		}
	}
	pNodes = Node;
	for ( DWORD n = 0; n < kMTRX; n++ )
	{
		OrdNode[n] = n;
    memset( Node[n].In, -1, sizeof(Node[n].In) );
		Node[n].kIn = 0;
		Node[n].old_State = 0xFF;
		Node[n].State = 0;
		int Numb = Blk[n].Tp;
		if( Numb >= 40000 && Numb <= 55000 )
		{
      C_LC64_Node::eNodeTypeLogicOperation nT = C_LC64_Node::ent_undefined;
			for ( int t = 0; t < _countof(TypeNumb); t++ )
		  {
			  if ( Numb == TypeNumb[t].Numb )
				{
					nT = TypeNumb[t].nt;
					break;
				}
			}
      ASS( nT > C_LC64_Node::ent_empty )
			Node[n].Type = nT;
	  }
		else
		{
      C_LC64_Node::eNodeTypeLogicOperation nT = C_LC64_Node::ent_empty;
      if( eBLK_Condition1Input1==Numb )
        nT = C_LC64_Node::ent_Input1;
      else if( eBLK_Condition3Input1==Numb )
        nT = C_LC64_Node::ent_Input1;
      else if( eBLK_Condition2Input1==Numb )
        nT = C_LC64_Node::ent_Input1;
      else if( eBLK_Condition1Input2==Numb )
        nT = C_LC64_Node::ent_Input2;
      else if( eBLK_Condition3Input2==Numb )
        nT = C_LC64_Node::ent_Input2;
      else if( eBLK_Condition1Input3==Numb )
        nT = C_LC64_Node::ent_Input3;
      else if( eBLK_Condition1Input4==Numb )
        nT = C_LC64_Node::ent_Input4;
      else if( eBLK_Condition1Input5==Numb )
        nT = C_LC64_Node::ent_Input5;
      else if( eBLK_Condition1Input6==Numb )
        nT = C_LC64_Node::ent_Input6;
      else if( eBLK_Action1Output1==Numb )
        nT = C_LC64_Node::ent_Output1;
      else if( eBLK_Action3Output1==Numb )
        nT = C_LC64_Node::ent_Output1;
      else if( eBLK_Action1Output2==Numb )
        nT = C_LC64_Node::ent_Output2;
      else if( eBLK_Action3Output2==Numb )
        nT = C_LC64_Node::ent_Output2;
      else if( eBLK_Action4Output1==Numb )
        nT = C_LC64_Node::ent_Output1;
      else if( eBLK_Action5Output1==Numb )
        nT = C_LC64_Node::ent_Output1;
      else if( eBLK_Action6Output1==Numb )
        nT = C_LC64_Node::ent_Output1;
      else if( eBLK_Action1Output3==Numb )
        nT = C_LC64_Node::ent_Output3;
      else if( eBLK_Action1Output4==Numb )
        nT = C_LC64_Node::ent_Output4;
      else if( eBLK_Action1Output5==Numb )
        nT = C_LC64_Node::ent_Output5;
      else if( eBLK_Action1Output6==Numb )
        nT = C_LC64_Node::ent_Output6;
      else if( eBLK_Action2Output1==Numb )
        nT = C_LC64_Node::ent_Output1;
      Node[n].Type = nT;
		}
	}
  if( kMTRX )
	  qsort( OrdNode, kMTRX, sizeof ( int ), Comp_C_LC64_Node );
// Расстановка входов
	for ( DWORD c = 0; c < kMTRX_CONN; c++ )
	{
		int oF = FT[c].o_f - 1;
		int oT = FT[c].o_t - 1;
		ASS(oT>=0)
//
		C_LC64_OP * pN = (C_LC64_OP*)&Node[oT];
		pN->AddIn( oF, FT[c].n_t, FT[c].n_f );
    KKK();
//
	}
}

void CY_LC64_E_Base::Logic( double dt )
{
  m_LastWorkStep = CY_Base::s_nCalcSteps;
	SET_HBP
		{
  	KKK();
		}
	C_LC64_OP::Nodes = Node;
	for ( DWORD n = 0; n < kMTRX; n++ )
		{
		Node[n].x = Node[n].State;
		}
// Все условия
	for ( DWORD n = 0; n < kMTRX; n++ )
	{
    for( int i=0; i<maxInputOutput; ++i )
    {
      if( IO[n][i] < 0 )
		  {// Условие
        if( VarCond[n][i].pVar && VarCond[n][i].IsMethod() )
			  {
          VarCond[n][i].Cond();
          BYTE st = VarCond[n][i].Yes ? 1 : 0;
          switch( i )
          {
          case 0:
            Node[n].State = st;
            break;
          case 1:
            Node[n].State2 = st;
            break;
          case 2:
            Node[n].State3 = st;
            break;
          case 3:
            Node[n].State4 = st;
            break;
          case 4:
            Node[n].State5 = st;
            break;
          case 5:
            Node[n].State6 = st;
            break;
          }
			  }
		  }
  // Сброс 'L'
		  if ( IO[n][i] > 0 )
		  {
			  if ( VarAct[n][i].pVar )
         {
          VarAct[n][i].Active = false;
          if ( VarAct[n][i].VarType == 'I' )
          {
            if( VarAct[n][i].pAI )
            {
              int val = *(int*)VarAct[n][i].pVar;
              VarAct[n][i].State = val==VarAct[n][i].Iparam ? 1 : 0;
            }
            else
              VarAct[n][i].State = *(int*)VarAct[n][i].pVar;
          }
          else if ( VarAct[n][i].VarType == 'W' )
          {
            if( VarAct[n][i].pAB )
            {
              BYTE val = *(BYTE*)VarAct[n][i].pVar;
              VarAct[n][i].State = val==VarAct[n][i].Iparam ? 1 : 0;
            }
            else
              VarAct[n][i].State = *(BYTE*)VarAct[n][i].pVar;
          }
          else
            VarAct[n][i].State = -1;
        }
      }
    }//maxInputOutput
	}
//
	for ( DWORD n = 0; n < kMTRX; n++ )
	{
		int N = OrdNode[n];
		if( IO[N][0] != 1 && IO[N][0] != -1 )
			ZeroMemory( IO[N], sizeof(IO[N]) );
		if( IO[N][0] < 0 )
			continue;
		if( IO[N][0] > 0 )
		{// Action
			C_LC64_Node & node = Node[N];
			C_LC64_OP * pOP = (C_LC64_OP*)&Node[N];
      pOP->Set();
      BYTE States[] = { pOP->State, pOP->State2, pOP->State3, pOP->State4, pOP->State5, pOP->State6 } ;
      for( int i=0; i<_countof(States); ++i )
      {
        if( !VarAct[N][i].pVar || !VarAct[N][i].IsMethod() )
          continue;
        if( VarAct[N][i].P_Type )
          VarAct[N][i].Set_P( node.old_State != node.State, States[i] ? true : false );
        else
        {
          if ( VarAct[N][i].L_Type )
            VarAct[N][i].SET_0();
          VarAct[N][i].Act( States[i] );
        }
      }
////////////////////////////////////////////////////
			if ( node.old_State != node.State )
			{
				node.old_State = node.State;
				if( node.State == 1 )//Yes
				{
				}
				else//сигнал пропал
				{
				}
			}
			continue;
		}
    // Логика
		C_LC64_OP * pNode = (C_LC64_OP*)&Node[N];
		switch ( Node[N].Type )
		{
    default:
      ASS(0);
    case C_LC64_Node::ent_Input1:
    case C_LC64_Node::ent_Input2:
    case C_LC64_Node::ent_Input3:
    case C_LC64_Node::ent_Input4:
    case C_LC64_Node::ent_Input5:
    case C_LC64_Node::ent_Input6:
    case C_LC64_Node::ent_Output1:
    case C_LC64_Node::ent_Output2:
    case C_LC64_Node::ent_Output3:
    case C_LC64_Node::ent_Output4:
    case C_LC64_Node::ent_Output5:
    case C_LC64_Node::ent_Output6:
    case C_LC64_Node::ent_empty:
      break;
		case C_LC64_Node::ent_OR:
			pNode->OR();
			break;
		case C_LC64_Node::ent_TON:
			pNode->TON();
		  break;
		case C_LC64_Node::ent_TOFF:
			pNode->TOFF();
		  break;
		case C_LC64_Node::ent_NOT:
			pNode->NOT();
			break;
    case C_LC64_Node::ent_AND:
			pNode->AND();
			break;
		case C_LC64_Node::ent_OFFD:
			{
			  int n = Timer[N] - 1;
        if( n < 0 )
          break;
			  pNode->OFFD( dt, pSTs64[n], pPTs64[n] );
			}
		  break;
		case C_LC64_Node::ent_OND:
			{
			  int n = Timer[N] - 1;
        if( n < 0 )
          break;
			  pNode->OND( dt, pSTs64[n], pPTs64[n] );
			}
		  break;
		case C_LC64_Node::ent_SRS1_S:
			pNode->SRS1_S();
			break;
		case C_LC64_Node::ent_SRS2_S:
			pNode->SRS2_S();
			break;
		case C_LC64_Node::ent_SRS1_R:
			pNode->SRS1_R();
			break;
		case C_LC64_Node::ent_SRS2_R:
			pNode->SRS2_R();
			break;
		case C_LC64_Node::ent_GT:
			pNode->GT();
			break;
		case C_LC64_Node::ent_GE:
			pNode->GE();
			break;
		case C_LC64_Node::ent_EQ:
			pNode->EQ();
			break;
    case C_LC64_Node::ent_WOUT:
      pNode->WOUT();
      break;
    case C_LC64_Node::ent_WOUT_Up:
      pNode->WOUT_Up();
      break;
		}
	}

	if ( StopOnChange )
		{
		for ( DWORD n = 0; n < kMTRX; n++ )
			{
			if ( Node[n].x != Node[n].State )
				{
// Изменение
				MessageBox ( NULL, "Изменение", "", MB_ICONSTOP|MB_SYSTEMMODAL);
				//StopModel = true;
				break;
				}
			}
		}
}

C_LC64_Node * C_LC64_OP::Nodes = NULL;

BYTE C_LC64_Node::GetState( BYTE sn )
{
  switch( Type )
  {
  case C_LC64_Node::ent_Input6:
    if( 5==sn )
      return State6;
  case C_LC64_Node::ent_Input5:
    if( 4==sn )
      return State5;
  case C_LC64_Node::ent_Input4:
    if( 3==sn )
      return State4;
  case C_LC64_Node::ent_Input3:
    if( 2==sn )
      return State3;
  case C_LC64_Node::ent_Input2:
    if( 1==sn )
      return State2;
    if( !sn )
      return State;
    ASSD(0);
    break;
  case C_LC64_Node::ent_SRS2_S:
  case C_LC64_Node::ent_SRS2_R:
    if( !sn )
      return State;
    if( 1==sn )
      return State2;
    ASSD(0);
  }
  return State;
}

void C_LC64_OP::AddIn ( SHORT N, SHORT Np, SHORT stateN )
{
  --stateN;
  if( Type >= C_LC64_Node::ent_SRS1_S && Type <= C_LC64_Node::ent_WOUT_Up
   || Type==C_LC64_Node::ent_Output2
   || Type==C_LC64_Node::ent_Output3
   || Type==C_LC64_Node::ent_Output4
   || Type==C_LC64_Node::ent_Output5
   || Type==C_LC64_Node::ent_Output6
   )
  {
		Np--;
    ASS( Np < _countof(In) )
    if ( In[Np].index>=0 )
			return;
		In[Np].index = N;
    In[Np].stateNum = stateN;
    ASS( In[Np].stateNum < 16 );
		kIn = 2;
    if( Type==C_LC64_Node::ent_Output3 )
      kIn = 3;
    if( Type==C_LC64_Node::ent_Output4 )
      kIn = 4;
    if( Type==C_LC64_Node::ent_Output5 )
      kIn = 5;
    if( Type==C_LC64_Node::ent_Output6 )
      kIn = 6;
		return;
	}
	for ( int n = 0; n < kIn; n++ )
	{
		if ( In[n].index == N && In[n].stateNum==stateN )
			return;
	}
	ASS( kIn < _countof(In) )
  In[kIn].stateNum = stateN;
  ASS( In[kIn].stateNum < 16 );
	In[kIn++].index = N;
}

void C_LC64_OP::Set()
{
	ASS( kIn );
  if( 1<=kIn )
  {
    int N = In[0].index;
	  State = Nodes[N].GetState( In[0].stateNum );
  }
  if( 2<=kIn )
  {
    int N = In[1].index;
    State2 = Nodes[N].GetState( In[1].stateNum );
  }
  if( 3<=kIn )
  {
    int N = In[2].index;
    State3 = Nodes[N].GetState( In[2].stateNum );
  }
  if( 4<=kIn )
  {
    int N = In[3].index;
    State4 = Nodes[N].GetState( In[3].stateNum );
  }
  if( 5<=kIn )
  {
    int N = In[4].index;
    State5 = Nodes[N].GetState( In[4].stateNum );
  }
  if( 6<=kIn )
  {
    int N = In[5].index;
    State6 = Nodes[N].GetState( In[5].stateNum );
  }
}

void C_LC64_OP::OR()
	{
	State = 0;
	for ( int n = 0; n < kIn; n++ )
		{
    int N = In[n].index;
		if ( Nodes[N].GetState(In[n].stateNum) )
			{
			State = 1;
			break;
			}
		}
	}

void C_LC64_OP::TON()
	{
	ASS( kIn == 1 )
	/*if ( Time > 0 )
		{
		Time--;
		return;
		}*/
	State = 0;
//
	int N = In[0].index;
  BYTE S = Nodes[N].GetState(In[0].stateNum);
	if ( S == 0 )
		{
		old_State = 0;
		return;
		}
	if ( S && old_State == 0 )
		{
		old_State = 1;
		Time = 1;
		State = 1;
		return;
		}
	}

void C_LC64_OP::TOFF()
	{
	ASS( kIn == 1 )
	/*if ( Time > 0 )
		{
		Time--;
		return;
		}*/
	State = 0;
//
  int N = In[0].index;
  ASS(N>=0);
  BYTE S = Nodes[N].GetState(In[0].stateNum);
	if ( S == 1 )
		{
		old_State = 0;
		return;
		}
	if ( S == 0 && old_State == 0 )
		{
		old_State = 1;
		Time = 1;
		State = 1;
		return;
		}
	}

void C_LC64_OP::NOT()
	{
	ASS(kIn == 1)
	int N = In[0].index;
  if ( Nodes[N].GetState(In[0].stateNum) )
		State = 0;
	else
		State = 1;
	}


void C_LC64_OP::AND()
	{
	State = 1;
	for ( int n = 0; n < kIn; n++ )
		{
		int N = In[n].index;
    if ( Nodes[N].GetState(In[n].stateNum) == 0 )
			{
			State = 0;
			break;
			}
		}
	}

void C_LC64_OP::OFFD( double dt, double & ST, double & PT )
{
	ASS(kIn == 1)
	int nS = In[0].index;
  BYTE IN = Nodes[nS].GetState(In[0].stateNum);
	if ( IN )
	{
		State = 1;
		PT = 0.;
		return;
	}
  if( !State )
    return;
	if( PT < ST )
	{
		PT += dt;
		State = 1;
		return;
	}
	PT = ST;
	State = 0;
}

void C_LC64_OP::OND( double dt, double & ST, double & PT )
{
	ASS(kIn == 1)
	int nS = In[0].index;
  BYTE IN = Nodes[nS].GetState(In[0].stateNum);
	if ( IN == 0 )
	{
		State = 0;
		PT = 0.;
		return;
	}
  if( State )
    return;
  if ( ST < 1. )
    ST = 1.;
	if( PT < ST )
	{
		PT += dt;
		State = 0;
		return;
	}
	PT = ST;
	State = 1;
}

void C_LC64_OP::SRS1_S()
	{
// S - 0
// R - 1
	ASS(kIn == 2)
	int nS = In[0].index;
	int nR = In[1].index;
  BYTE S = Nodes[nS].GetState(In[0].stateNum);
	BYTE R = Nodes[nR].GetState(In[1].stateNum);
	if ( S == 0 && R == 0 )
		return;
	if ( S == 1 )
		{
		State = 1;
		return;
		}
	State = 0;
	}

void C_LC64_OP::SRS2_S()
{
  // S - 0
  // R - 1
  ASS(kIn == 2)
  int nS = In[0].index;
  int nR = In[1].index;
  BYTE S = Nodes[nS].GetState(In[0].stateNum);
  BYTE R = Nodes[nR].GetState(In[1].stateNum);
  if ( S == 0 && R == 0 )
    return;
  if ( S == 1 )
  {
    State = 1;
    State2 = 0;
    return;
  }
  State = 0;
  State2 = 1;
}

void C_LC64_OP::SRS1_R()
	{
// S - 0
// R - 1
	ASS(kIn == 2)
	int nS = In[0].index;
	int nR = In[1].index;
  BYTE S = Nodes[nS].GetState(In[0].stateNum);
	BYTE R = Nodes[nR].GetState(In[1].stateNum);
	if ( S == 0 && R == 0 )
		return;
	if ( R == 1 )
		{
		State = 0;
		return;
		}
	State = 1;
	}

void C_LC64_OP::SRS2_R()
{
  // S - 0
  // R - 1
  ASS(kIn == 2)
  int nS = In[0].index;
  int nR = In[1].index;
  BYTE S = Nodes[nS].GetState(In[0].stateNum);
  BYTE R = Nodes[nR].GetState(In[1].stateNum);
  if ( S == 0 && R == 0 )
    return;
  if ( R == 1 )
    {
    State = 0;
    State2 = 1;
    return;
    }
  State = 1;
  State2 = 0;
}

void C_LC64_OP::GT()
	{
	ASS(kIn == 2)
	int n1 = In[0].index;
	int n2 = In[1].index;
  BYTE S1 = Nodes[n1].GetState(In[0].stateNum);
	BYTE S2 = Nodes[n2].GetState(In[1].stateNum);
	if ( S1 > S2 )
		State = 1;
	else
		State = 0;
	}

void C_LC64_OP::GE()
	{
	ASS(kIn == 2)
	int n1 = In[0].index;
	int n2 = In[1].index;
  BYTE S1 = Nodes[n1].GetState(In[0].stateNum);
	BYTE S2 = Nodes[n2].GetState(In[1].stateNum);
	if ( S1 >= S2 )
		State = 1;
	else
		State = 0;
	}

void C_LC64_OP::EQ()
	{
	ASS(kIn == 2)
	int n1 = In[0].index;
	int n2 = In[1].index;
  BYTE S1 = Nodes[n1].GetState(In[0].stateNum);
  BYTE S2 = Nodes[n2].GetState(In[1].stateNum);
	if ( S1 == S2 )
		State = 1;
	else
		State = 0;
	}

void C_LC64_OP::WOUT()
{
  ASS(kIn == 2)
  int n1 = In[0].index;
  int n2 = In[1].index;
  BYTE S1 = Nodes[n1].GetState(In[0].stateNum);
  BYTE S2 = Nodes[n2].GetState(In[1].stateNum);
  if( S2 )
    State = 0;
  else
    State = S1;
}

void C_LC64_OP::WOUT_Up()
{
  ASS(kIn == 2)
  int n1 = In[0].index;
  int n2 = In[1].index;
  BYTE S1 = Nodes[n1].GetState(In[0].stateNum);
  BYTE S2 = Nodes[n2].GetState(In[1].stateNum);
  if( S2 )
    State = 0;
  else
    State = S1;
}

void CY_LC64_E_Base::OutOG( int N )
{
//
	char OG[128];
	strcpy_s ( OG, 128, pDR->FCS );
	char * P = strchr ( OG, '/' );
	if ( P )
		*P = 0;
	if ( pSendOG )
		(*pSendOG) ( OG, N );
	return;
}

bool CY_LC64_E_Base::EnumUsedObj( DWORD &pos, LPCSTR *ppName, CY_Base **ppObj )
{
  if( !ppName )
    return false;
  if( ppObj )
    *ppObj = NULL;
  *ppName = NULL;

  WORD con = HIWORD( pos );
  WORD act = LOWORD( pos );
  DWORD S = 0;
  for ( DWORD n = 0; n < kDATA; ++n, ++S )
	{
    for( int i=0; i<maxInputOutput; ++i, ++S )
    {
      if( S<con )
        continue;
      //if( IO[n][i] < 0 )
		  {// Условие
        if( !VarCond[n][i].pVar )
          continue;
        *ppName = VarCond[n][i].Name;
        ++S;
        ASS( S < USHRT_MAX );
        con = S;
        pos = MAKELONG( act, con);
        return true;
      }
    }
  }
  S = 0;
  for ( DWORD n = 0; n < kDATA; ++n, ++S )
  {
    for( int i=0; i<maxInputOutput; ++i, ++S )
    {
      if( S<act )
        continue;
      //if( IO[n][i] > 0 )
      {
        if( !VarAct[n][i].pVar )
          continue;
        *ppName = VarAct[n][i].Name;
        ++S;
        ASS( S < USHRT_MAX );
        act = S;
        pos = MAKELONG( act, con);
        return true;
      }
    }
  }
  return false;
}

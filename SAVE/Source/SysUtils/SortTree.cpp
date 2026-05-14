#include "CommProc.h"


CSortTree * From( int a, CSortTree * N )
  {
  if ( a < 0 )
    return N->pL;
  return N->pR;
  }

void To( int a, CSortTree * N, CSortTree * Ref )
  {
  if ( a < 0 )
    N->pL = Ref;
  else
    N->pR = Ref;
  }

// Кнут 286
CSortTree * Find( CSortTreeInfo * Info, void * K, bool AddNoFound, bool *pbAdded )
  {
  CSortTree * T;//Отец S
  CSortTree * S;//Указывает на место, где может потребоваться баланс
  CSortTree * P;//Двигается вниз по дереву
  T = Info->Root;
  if( !T )
    return NULL;
  S = P = T->pR;
  if ( P == NULL )
    {
    if ( !AddNoFound )
      return NULL;
    P = (*Info->pNew)( K, Info->Type );
	  if ( pbAdded )
		  *pbAdded = true;
    T->pR = P;
    Info->Add( P );
    return P;
    }
A2:
  int Comp = (*Info->pCompKey)( K, P->Key, Info->Type );
  if ( Comp == 0 )
    return P;
  CSortTree * Q;
  if ( Comp < 0 )
    {
    // A3
    Q = P->pL;
    if ( Q == NULL )
      {
      if ( !AddNoFound )
        return NULL;
      Q = (*Info->pNew)( K, Info->Type );
  		if ( pbAdded )
			  *pbAdded = true;
      P->pL = Q;
      Info->Add( Q );
      goto A5;
      }
    }
  else
    {
    // A4
    Q = P->pR;
    if ( Q == NULL )
      {
      if ( !AddNoFound )
        return NULL;
      Q = (*Info->pNew)( K, Info->Type );
		  if ( pbAdded )
			  *pbAdded = true;
      P->pR = Q;
      Info->Add( Q );
      goto A5;
      }
    }
  if ( Q->Balance )
    {
    T = P;
    S = Q;
    }
  P = Q;
  goto A2;
A5:
  //Q -> Key = K;
  //Q -> pL = NULL;
  //Q -> pR = NULL;
  //Q -> Balance = 0; 
  //A6:
  Comp = (*Info->pCompKey)( K, S->Key, Info->Type );
  int a;
  CSortTree * R;
  if ( Comp < 0 )
    {
    // От S до Q Balance = -1
    P = S->pL;
    R = P;
    a = -1;
    }
  else
    {
    // От S до Q Balance = 1
    P = S->pR;
    R = P;
    a = 1;
    }
  while ( P && P != Q )
    {
    Comp = (*Info->pCompKey)( K, P->Key, Info->Type );
    if ( Comp < 0 )
      {
      P->Balance = -1;
      P = P->pL;
      }
    else
      {
      P->Balance = 1;
      P = P->pR;
      }
    }
  //A7:
  if ( S->Balance == 0 )
    {
    S->Balance = a;
    return Q;
    }
  if ( S->Balance == -a )
    {
    S->Balance = 0;
    return Q;
    }
  //
  if ( R->Balance == a )
    goto A8;
  if ( R->Balance == -a )
    goto A9;
  ASS(0)
A8:
  P = R;
  To(a,S,From(-a,R));
  To(-a,R,S);
  R->Balance = 0;
  S->Balance = 0;
  goto A10;
A9:
  P = From(-a,R);
  To(-a,R,From(a,P));
  To(a,P,R);
  To(a,S,From(-a,P));
  To(-a,P,S);
  if ( P->Balance == 0 )
    {
    S->Balance = 0;
    R->Balance = 0;
    }
  else
    {
    if ( P->Balance == a )
      {
      S->Balance = -a;
      R->Balance = 0;
      }
    else
      {
      S->Balance = 0;
      R->Balance = a;
      }
    P->Balance = 0;
    }
A10:
  if ( S == T -> pR )
    T -> pR = P;
  else
    T -> pL = P;
  return Q;
  }

bool Add( CSortTreeInfo * Info, void * K, CSortTree * pNewItem )
  {
  ASS(pNewItem && pNewItem->Key)
  CSortTree * T;//Отец S
  CSortTree * S;//Указывает на место, где может потребоваться баланс
  CSortTree * P;//Двигается вниз по дереву
  T = Info->Root;
  S = P = T->pR;
  if ( P == NULL )
    {
    P = pNewItem;
    T->pR = P;
    Info->Add( pNewItem );
    return true;
    }
A2:
  int Comp = (*Info->pCompKey)( K, P->Key, Info->Type );
  if ( Comp == 0 )
    return false;
  CSortTree * Q;
  if ( Comp < 0 )
    {
    // A3
    Q = P->pL;
    if ( Q == NULL )
      {
      Q = pNewItem;
      P->pL = Q;
      goto A5;
      }
    }
  else
    {
    // A4
    Q = P->pR;
    if ( Q == NULL )
      {
      Q = pNewItem;
      P->pR = Q;
      goto A5;
      }
    }
  if ( Q->Balance )
    {
    T = P;
    S = Q;
    }
  P = Q;
  goto A2;
A5:
  //Q -> Key = K;
  //Q -> pL = NULL;
  //Q -> pR = NULL;
  //Q -> Balance = 0; 
  //A6:
  Comp = (*Info->pCompKey)( K, S->Key, Info->Type );
  int a;
  CSortTree * R;
  if ( Comp < 0 )
    {
    // От S до Q Balance = -1
    P = S->pL;
    R = P;
    a = -1;
    }
  else
    {
    // От S до Q Balance = 1
    P = S->pR;
    R = P;
    a = 1;
    }
  while ( P != Q )
    {
    Comp = (*Info->pCompKey)( K, P->Key, Info->Type );
    if ( Comp < 0 )
      {
      P->Balance = -1;
      P = P->pL;
      }
    else
      {
      P->Balance = 1;
      P = P->pR;
      }
    }
  //A7:
  if ( S->Balance == 0 )
    {
    S->Balance = a;
    Info->Add( pNewItem );
    return true;
    }
  if ( S->Balance == -a )
    {
    S->Balance = 0;
    Info->Add( pNewItem );
    return true;
    }
  //
  if ( R->Balance == a )
    goto A8;
  if ( R->Balance == -a )
    goto A9;
  ASS(0)
A8:
  P = R;
  To(a,S,From(-a,R));
  To(-a,R,S);
  R->Balance = 0;
  S->Balance = 0;
  goto A10;
A9:
  P = From(-a,R);
  To(-a,R,From(a,P));
  To(a,P,R);
  To(a,S,From(-a,P));
  To(-a,P,S);
  if ( P->Balance == 0 )
    {
    S->Balance = 0;
    R->Balance = 0;
    }
  else
    {
    if ( P->Balance == a )
      {
      S->Balance = -a;
      R->Balance = 0;
      }
    else
      {
      S->Balance = 0;
      R->Balance = a;
      }
    P->Balance = 0;
    }
A10:
  if ( S == T -> pR )
    T -> pR = P;
  else
    T -> pL = P;
  Info->Add( pNewItem );
  return true;
  }

enum eState { Вошел, ПошелНалево, ВыдалСебя, ПошелНаправо, КореньВошел, КореньНалево, КореньНаправо, Конец };
#define MAX_STACK 128
static CSortTree * Stack[MAX_STACK];
static eState State[MAX_STACK]; // 0 - вошел
static int kStack;

CSortTree * GetFirst( CSortTreeInfo * Info )
  {
  kStack = 0;
  State[kStack] = КореньВошел;
  Stack[kStack++] = Info->Root;
  return GetNext( );
  }

CSortTree * GetNext( )
  {
Start:
  eState S = State[kStack-1];
  CSortTree * pNode = Stack[kStack-1];
  if ( pNode == NULL )
    return NULL;
  switch( S )
    {
    case Вошел:
      {
      if ( pNode->pL )
        {
        State[kStack-1] = ПошелНалево;
        State[kStack] = Вошел;
        Stack[kStack++] = pNode->pL;
        goto Start;
        }
      State[kStack-1] = ВыдалСебя;
      return pNode;
      }
    break;
    case КореньВошел:
      {
      if ( pNode->pR )
        {
        State[kStack-1] = КореньНаправо;
        State[kStack] = Вошел;
        Stack[kStack++] = pNode->pR;
        goto Start;
        }
      State[kStack-1] = Конец;
      return NULL;
      }
    break;
    case ПошелНалево:
      {
      State[kStack-1] = ВыдалСебя;
      return pNode;
      }
    break;
    case ВыдалСебя:
      {
      if ( pNode->pR )
        {
        State[kStack-1] = ПошелНаправо;
        State[kStack] = Вошел;
        Stack[kStack++] = pNode->pR;
        goto Start;
        }
      kStack--;
      goto Start;
      }
    break;
    case ПошелНаправо:
      {
      kStack--;
      goto Start;
      }
    break;
    case КореньНаправо:
      {
      State[kStack] = Конец;
      return NULL;
      }
    break;
    case Конец:
      {
      return NULL;
      }
    break;
    }
  return NULL;
  }


CSortTree * CSortTreeInfo::GetFirst(  )
  {
  return FirstInGroup;
  }

CSortTree * CSortTreeInfo::GetNext( CSortTree * Prev )
  {
  if ( Prev )
    return Prev->pNext_Group_Pnt;
  return NULL;
  }

void CSortTreeInfo::Add( CSortTree * pItem )
  {
  if ( pItem == NULL )
    return;
  if ( FirstInGroup == NULL )
    FirstInGroup = pItem;
  if ( LastInGroup )
    LastInGroup->pNext_Group_Pnt = pItem;
  LastInGroup = pItem;
  pItem->pNext_Group_Pnt = NULL;
  }


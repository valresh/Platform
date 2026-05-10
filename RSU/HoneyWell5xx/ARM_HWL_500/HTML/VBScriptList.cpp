#include "StdAfx.h"
#include "HTML.h"
#include "VBScript.h"
//
#include <Profile.h>


inline bool IsOper2( SToken& n )
  {
  return  OPER_EQ == n.dw || n.dw ==  OPER_MEQ || n.dw ==  OPER_BEQ || n.dw ==  OPER_MEN || n.dw ==  OPER_BOL;
  }

UINT CVBScript::VBRecursiveIf( SToken* token, int nCount )
  {
  UINT nCountOld = mComm.Count();
  UINT nComm = -1;
  UINT nThen = -1;
  DWORD dwIF =  0;
  int N;
  int n = 0;
  while ( n < nCount )
    {
    switch ( token[n].dw )
      {
      case KEY_IF:
        if ( dwIF==KEY_THEN || (Version>=VSB_KF && dwIF==KEY_ELSE))
          {
          N = VBRecursiveIf( token+n, nCount-n );
          }
        else
          {
          N = 1;
          if ( AS(dwIF==0 || dwIF == KEY_THENIF || dwIF==KEY_ELSE) )  //KEY_ELSE проверить
             return -1;
          dwIF = KEY_IF;
          }
        break;
      case KEY_ELSEIF:
        if ( nThen != -1 ) nThen = -1, dwIF = KEY_THEN;
        if ( AS(dwIF == KEY_THEN) ) 
        //if ( (dwIF != KEY_THEN) && (dwIF != KEY_THENIF) ) 
            return -1;
        KeyIF( KEY_THEN );
        N = 1;
        dwIF = KEY_ELSEIF;
        if ( As(nComm) ) return -1;
        {
        //SCommand& c = mComm.Obj( nComm );
        //c.nArg1 = mComm.Count()-1;
        //nComm = -1;
        }
        break;
      case KEY_THEN:
        N = 1;//KeyIF( KEY_THEN );
        if ( AS(dwIF == KEY_IF||dwIF == KEY_ELSEIF) ) return  -1;
        dwIF = KEY_THEN;
        break;
      case KEY_THENIF:
        N = 1;
        if ( AS(dwIF == KEY_IF||dwIF == KEY_ELSEIF) ) return  -1;
        dwIF = KEY_THENIF;
        break;
      case KEY_ELSE:
        if ( nThen != -1 ) nThen = -1, dwIF = KEY_THEN;
        if ( AS(dwIF == KEY_THEN||dwIF == KEY_THENIF) ) 
            return -1;
        KeyIF( KEY_THEN );
        N = KeyIF( KEY_ELSE );
        dwIF = KEY_ELSE;
        if ( As(nComm) ) return -1;
        {
        SCommand& c = mComm.Obj( nComm );
        c.nArg1 = mComm.Count()-1;
        nComm = -1;
        }
        break;
      case KEY_END:
        if ( nThen != -1 ) nThen = -1, dwIF = KEY_THEN;
        N = KeyEND( token+n, nCount-n );
        if ( AS(dwIF == KEY_ELSE||dwIF == KEY_THEN||dwIF == KEY_THENIF) ) return -1;
        dwIF = 0;
        if ( nComm != -1 )
          {
          SCommand& c = mComm.Obj( nComm );
          c.nArg1 = mComm.Count()-1;
          nComm = -1;
          }
        // Собираем все Then и ставим им переход сюда
          {
          UINT nCountNew = mComm.Count();
          for ( UINT x = nCountOld; x < nCountNew; x++ )
            {
            SCommand& c = mComm.Obj( x );
            if ( c.nOper != KEY_THEN ) continue;
            // Уже поставили во внутренней рекурсии
            if ( c.nArg1 != -1 ) continue;
            c.nArg1 = nCountNew-1;
            }
          }
        return n+N;
        break;
      case KEY_DIM:
      case KEY_CALL:
      case KEY_ARRAY:
        N = 1;
        break;
      case KEY_SUB:
        //ASSERT(0);
        break;
      case KEY_EXITSUB:
        {
        SCommand a;
        a.nOper = token[n].dw;//Операция
        UINT nAdd = AddComm( a );
        if ( nAdd < 0 ) 
            return -1;
        N = 1;
        }
        break;
      default:
        if ( dwIF == KEY_IF || dwIF == KEY_ELSEIF )
          {
          N = KeyLogic( token+n, nCount-n, dwIF );
          bool bFound = false;
          if ( nComm != -1 )
            {
            SCommand& c = mComm.Obj( nComm );            
            if(Version != LG35_8_KF)
              {    
               int ii = mComm.Count()-1;            
               for(; ii>0; ii--)
                {
                    SCommand& c2 = mComm.Obj( ii );
                    if(c2.nOper == KEY_THEN)
                      {
                        bFound = true;
                        c.nArg1 = ii+1;
                        break;
                      }
                    if(c2.nOper == KEY_IF)
                      {
                        //не нашли - чтобы не зациклилось RUN
                        break;
                      }
                }
              }
            if(!bFound) //не должно бы быть
            {//как было
            c.nArg1 = mComm.Count()-1;
            SCommand& c2 = mComm.Obj( c.nArg1 );
            if (c2.nOper == KEY_ELSEIF) //пока не понял отчего тут номер операции больше
               c.nArg1--;
            }
            nComm = -1;
            }
          nComm = mComm.Count()-1;
          }
        else
          {
          N = KeyOther( token+n, nCount-n );
          }
        break;
      }
    if ( N <= 0 )
      {
      ASSERT(0);
      return -1;
      }
    n += N;
    }
  return n;
  }
//
UINT CVBScript::VBScriptList( SToken* token, int nCount, bool bLogic )
  {
  DWORD dwIF = 0;
  int N;
  int n = 0;
  while ( n < nCount )
    {
    switch ( token[n].dw )
      {
      case KEY_IF:
        N = VBRecursiveIf( token+n, nCount-n );
        break;
      case KEY_ELSEIF: case KEY_THEN: case KEY_THENIF: case KEY_ELSE: case KEY_END: case KEY_SUB:
        ASSERT(0);
        break;
      case KEY_CALL:
        N = 1;
        break;
      case KEY_DIM:
      case KEY_ARRAY:
        N = 1;
        break;
      case KEY_EXITSUB:
        {
        SCommand a;
        a.nOper = token[n].dw;//Операция
        UINT nAdd = AddComm( a );
        if ( nAdd < 0 ) 
            return -1;
        N = 1;
        }
        break;
      default:
        if ( bLogic )
          N = KeyArgus( token+n, nCount-n );
        else
          N = KeyOther( token+n, nCount-n );
        break;
      }
    if ( N <= 0 )
      {
      ASSERT(0);
      return -1;
      }
    n += N;
    }
  return n;
  }
// Аргументы функций
bool CVBScript::VBArgumentis( SToken* token, int nCount )
  {
  DWORD dwIF = 0;
  int N;
  int n = 0;
  while ( n < nCount )
    {
    switch ( token[n].dw )
      {
      case KEY_IF:
      case KEY_ELSEIF:
      case KEY_THEN:
      case KEY_ELSE:
      case KEY_END:
        ASSERT(0);
        return false;
      default:
        N = KeyArgus( token+n, nCount-n );
        break;
      }
    if ( N <= 0 )
      {
      ASSERT(0);
      return false;
      }
    n += N;
    }
  return true;
  }
//
// Типы if
// if then end if
// if then else end if
// if then elseif ... else  end if
int CVBScript::KeyIF( UINT n )
  {
  SCommand a;
  a.nOper = n;//Операция
  UINT nComm = AddComm( a );
  if ( As( nComm ) ) return -1;
  return 1;
  }
//
int CVBScript::KeyEND( SToken* token, int nCount )
  {
  if ( AS( nCount > 1 ) ) return -1;
  SCommand a;
  a.nOper = token[0].dw;//Операция
  UINT nComm = AddComm( a );
  if ( As( nComm ) ) return -1;
  if ( AS( token[1].dw == KEY_IF ) ) return -1;
  return 2;
  }
//
// Слева может стоять
bool IsLeftS( SToken& n )
  {
  bool bVar = false;
  if(Version!=LG35_8_KF) //пока так чтоб не попортить 140
    bVar = IsVar(n);
  return n.dw == 0 || IsValue(n) || IsParam(n) || IsWnd(n) || bVar;
  }
//
// Справа может стоять
bool IsRight( SToken& n )
  {
  return n.dw == 0 || IsValue(n) || IsParam(n) || IsConst(n) || IsFunc(n) || IsWnd(n) || n.dw == OPER_NOT ||
         ( VAR_INTEGER <= n.dw && n.dw <= VAR_STRING );
  }
//
int CVBScript::FindEnd( SToken* token, int nCount )
  {
  int nEnd = 1;
  for ( ; nEnd < nCount-1; nEnd+=2 )
    {
    if ( !IsOper(token[nEnd]) )
      break;
    bool bRight = IsRight( token[nEnd+1] );
    if ( AS(bRight) ) 
       return -1;
    if ( token[nEnd+1].dw==OPER_NOT )
       nEnd++;
    }
  if ( As(nEnd) ) return -1;
  return nEnd;
  }
//
int CVBScript::KeyOther( SToken* token, int nCount )
  {
  bool bLeft = IsLeftS(token[0]);
  if ( AS(bLeft) )
  {
      if(IsSub(token[0]))
      {
         int nEnd = FindEnd( token, nCount );
         if ( nEnd == -1 ) 
             return -1;
         UINT nRes = AddArgument(token[0],false);
         if ( As(nRes) ) 
            return -1;
         return nEnd;
      }
      else
      if(token[0].dw==KEY_FOR)
        {
            int NN = KeyOther( &token[1], nCount-1 );
            if(NN==-1)
              return -1;
            return NN+1;
        }
      else
      if(token[0].dw==KEY_TO)
        {
         UINT nRes = AddArgument(token[0], false);
         if (nRes!=-1) 
            return 1;
        }
      else
      if(token[0].dw==KEY_NEXT)
        {
         //поиск последнего KEY_TO в командах
         int nNEXTCount =0;
         for(int ii = mComm.Count()-1; ii>0; ii--)
           {
              SCommand& cTo = mComm.Obj( ii );
              if(cTo.nOper == KEY_NEXT)
                ++nNEXTCount;  //NEXT от вложенного цикла
              else
              if(cTo.nOper == KEY_TO)
                {
                 if(nNEXTCount>0)
                   { 
                    --nNEXTCount;  //TO от вложенного цикла
                    continue;
                   }
                 SCommand cm;
                 cm.nOper = KEY_NEXT;//Операция
                 cm.nRes;
                 char buf[32];
                 _itoa_s(ii, buf, sizeof(buf)-1, 10); 
                 bool b = Arguments( buf, cm.nRes, false );
                 if(cm.nRes != -1)
                   {
                    cm.nArg1 = cTo.nArg2;  //индекс цикла
                    AddComm( cm );
                    //
                    const char* an1 = mGVAR.Name( cTo.nArg1 );
                    GVAR& v = mGVAR.Obj( cTo.nArg1 );
                    if( (v.nType&0xf) == TYPE_INT)
                      {
                      int* a1 = (int*)mGVAR.Chars(cTo.nArg1);
                      if(a1)
                        {
                        *a1 = mComm.Count();
                        return 1;
                        }
                     }
                   }
                 break;
                }
           }
        }
      else
      if(token[0].dw==FUNC_CLOSE_POPUP)
        return 1;
      else if(token[0].dw==FUNC_DATAVALUE && token[1].dw == OPER_EQ)
      {
         int nEnd = FindEnd( token, nCount );
         if ( nEnd == -1 || nEnd != 3) 
             return -1;
         token[0].dw = FUNC_SET_DATAVALUE;
         SCommand a;
         a.nOper = FUNC_SET_DATAVALUE;
         a.nRes = AddTemp( TYPE_INT );
         if(Arguments( token[0].txt, a.nArg1, true ))
         if(Arguments( token[2].txt, a.nArg2, true ))
         {
           UINT nComm = AddComm( a );
           return nEnd;
         }
      }
      return -1;
  }
  //
  if ( token[1].dw != OPER_EQ ) 
  {
      if( token[1].dw == OPER_IS)
      {
          AAA();
      }
      return -1;
  }
  //
  int nEnd = FindEnd( token, nCount );
  if ( nEnd == -1 ) return -1;
  //
  UINT nComm;
  UINT nRes = AddArgument(token[2],false);
  // nEnd == 3 Простое присваивание a = b
  // Более сложное выражение a = b ... + c
  if ( nEnd != 3 )
    {
    nComm = AddCommand( token[3].dw, nRes, token[4], &nRes, false );
    if ( As(nComm) ) return -1;
    for ( int n = 5; n < nEnd; n+=2 )
      {
      nComm = AddCommand( token[n].dw, nRes, token[n+1], &nRes, false );
      if ( As(nComm) ) return -1;
      }
    }
  nComm = AddCommand( token[1].dw, nRes, token[0], NULL, false );
  if ( As(nComm) ) return -1;
  return nEnd;
  }
//
UINT CVBScript::KeyArgusRLog( SToken* token, int nCount, UINT nRes1 )
{
	if(nCount <= 0)
		return nRes1;

	switch(token[0].dw)
	{
		case OPER_EQ: case OPER_MEN: case OPER_MEQ: case OPER_EQ_EQ:
		case OPER_BOL: case OPER_NEQ :case OPER_BEQ: 
		case OPER_MOD: case OPER_SUB: case OPER_ADD: case OPER_DIV: 
		case OPER_MUL:
			{
				if ( token[0].dw == OPER_EQ ) token[0].dw = OPER_EQ_EQ;
				UINT nRes2 = AddArgument(token[1], true);
				if( nRes2 == -1)
					return -1;

				SCommand a;
				a.nOper = token[0].dw;
				a.nRes = AddTemp( TYPE_BOOL );
				a.nArg1 = nRes1;
				a.nArg2 = nRes2;
				AddComm( a );

				return KeyArgusRLog(token+2, nCount-2, a.nRes);
			}
			break;
		 case OPER_OR : case OPER_AND:
		 {
			UINT nRes2 = KeyArgusRLog(token+1, nCount-1, -1);
			if( nRes2 == -1)
				return nRes1;

			SCommand a;
			a.nOper = token[0].dw;
			a.nRes = AddTemp( TYPE_BOOL );
			a.nArg1 = nRes1;
			a.nArg2 = nRes2;
			AddComm( a );
			return a.nRes;
		 }
		 break;
	}

   UINT nRes2 = AddArgument(token[0], true);
   if(nRes2<0)
     return -1;

   return KeyArgusRLog(token+1, nCount-1, nRes2);
}

int CVBScript::KeyArgusLog( SToken* token, int nCount )
{
	for(int i = 1; i < nCount; i += 4)
		switch(token[i].dw)
		{
		case OPER_EQ: case OPER_MEN : case OPER_MEQ:
		case OPER_BOL:case OPER_NEQ : case OPER_BEQ: 
          if ( token[i].dw == OPER_EQ ) token[i].dw = OPER_EQ_EQ;
		  break;
		default:
		  return -1;
		}

	for(int i = 3; i < nCount; i += 4)
      if( ! (token[i].dw==OPER_OR || token[i].dw==OPER_AND)) 
		  return -1;

       UINT nRes1 = AddArgument(token[0], true);
       if(nRes1<0)
         return -1;
       UINT nComm1 = AddCommand( token[1].dw, nRes1, token[2], &nRes1, true );
       if(nComm1<0)
         return -1;

	    UINT tmpRes = -1;
		for(int i = 4; i < nCount; i += 4)
		{
		   UINT nRes2 = AddArgument(token[i], true);
		   if(nRes2<0)
			 return -1;
		   UINT nComm2 = AddCommand( token[i+1].dw, nRes2, token[i+2], &nRes2, true );
		   if(nComm2<0)
			 return -1;

		   SCommand a;
		   a.nOper = token[i-1].dw;
		   a.nRes = AddTemp( TYPE_BOOL );
		   a.nArg1 = tmpRes != -1 ? tmpRes : nRes1;
		   a.nArg2 = nRes2;
		   AddComm( a );
		   
		   tmpRes = a.nRes;
		}
       return nCount;
}

int CVBScript::KeyArgus( SToken* token, int nCount )
  {
  //
  bool bLeft = IsRight(token[0]);
  if ( AS(bLeft) ) 
      return -1;
  //
  if ( token[1].dw == KEY_THEN || token[1].dw == KEY_THENIF || token[1].dw == KEY_ELSEIF )
    {
    UINT nRes = -1;
    char nul[32] = "0";
    bool b = Arguments( nul, nRes, true );
    if ( !b ) return -1;
    if ( As(nRes) ) return -1;
    UINT nComm = AddCommand( OPER_NEQ, nRes, token[0], &nRes, true );
    if ( As(nComm) ) return -1;
    return 1;
    }
  else
  if ( token[0].dw == OPER_NOT && nCount >= 2 )
    {
    SCommand aa;
    aa.nOper = token[0].dw;//Операция
    aa.nArg1 = AddArgument(token[1], true);
    aa.nRes = AddTemp( TYPE_BOOL );
    UINT nComm = AddComm( aa );
    if ( nComm < 0 ) 
        return -1;

    int nn=2;
    UINT nRes = aa.nRes;
    while( IsOper(token[nn]) )
      {
         UINT nComm;
         if(token[nn+1].dw == OPER_NOT)
           {
           int NN = KeyArgus( &token[nn+1], nCount-nn-1);
           if(NN<0) 
             return -1;
           nComm = AddCommand( token[nn].dw, nRes, token[nn+1], &nRes, true );
           if(nComm<0) 
             return -1;
           nn = 1+NN;
           }
         else nComm = AddCommand( token[nn].dw, nRes, token[nn+1], &nRes, true );
         if ( As(nComm) ) 
            return -1;
         nn+=2;
      }
    return nn; 
    }
  //
  if ( AS( IsOper(token[1]) ) ) 
      return -1;
  //
  int nEnd = FindEnd( token, nCount );
  if ( As(nEnd) ) return -1;
  //
  if(Version == T1163_ONPZ)
	  //if(KeyArgusRLog( token, nEnd, -1 ) != -1 )
	  if(KeyArgusLog( token, nEnd ) != -1 )
		  return nEnd;

  if( nEnd==3 && token[1].dw == OPER_EQ && token[2].dw == FUNC_GETELEMENTBYID)
    {//исуственная вставка получения объекта для getElementById() в IsStd
       SCommand a;
       a.nOper = token[1].dw;
       a.nRes = AddArgument(token[0],false);
       a.nArg1 = AddArgument(token[2],false);
       AddComm( a );
       //
       int nEnd2 = FindEnd( &token[3], nCount-3 );
       if ( As(nEnd2) ) 
           return -1;
       nEnd +=  nEnd2;
       //
       if(nEnd==4)
       { //cstr(document.getElementById("alpha" & i).value)
         SCommand a0;
         a0.nOper = OPER_EQ;//Операция
         a0.nArg1 = AddArgument(token[3],true);
         a0.nRes = AddTemp( TYPE_STRING );
         UINT nComm = AddComm( a0 );
         if ( As(nComm ) ) 
             return -1;
         return nEnd;
       }
       //
       UINT nComm;
       UINT nRes = AddArgument(token[3],true);
       if ( As(nRes) ) 
           return -1;
       for ( int n = 4; n < nEnd; n+=2 )
         {
         if ( token[n].dw == OPER_EQ )
           token[n].dw = OPER_EQ_EQ;
         nComm = AddCommand( token[n].dw, nRes, token[n+1], &nRes, true );
         if(token[n+1].dw == OPER_NOT)
           n++;
         if ( As(nComm) ) 
             return -1;
         }
       return nEnd;
    }
  else
  if( nEnd==5 && (IsFunc(token[0]) && IsOper(token[1]) && IsFunc(token[2]) && IsOper(token[3]) && IsVar( token[4])
                  || (IsOper2(token[1]) && token[3].dw==OPER_AND && token[1].dw != token[3].dw) ) 
    )
  { //IsNumeric(nNumericFormat) and instr(nNumericFormat,".")=0
       if ( token[1].dw == OPER_EQ )
          token[1].dw = OPER_EQ_EQ;
       if ( token[3].dw == OPER_EQ )
          token[3].dw = OPER_EQ_EQ;
       UINT nRes1 = AddArgument(token[0],true);
       UINT nRes2 = AddArgument(token[2],true);
       UINT nRes3 = AddArgument(token[4], true);
       if( token[1].dw == OPER_EQ_EQ && (token[3].dw == OPER_ADD || token[3].dw == OPER_SUB) )
       {//if instr(ucase(sDsp),"_G")= len(sDSP)-1 then 
        SCommand a;
        a.nOper = token[3].dw;
        a.nRes = AddTemp( TYPE_INT );
        a.nArg1 = nRes2;
        a.nArg2 = nRes3;
        AddComm( a );
        SCommand b;
        b.nOper = token[1].dw;
        b.nRes = AddTemp( TYPE_BOOL );
        b.nArg1 = nRes1;
        b.nArg2 = a.nRes;
        AddComm( b );
        return nEnd;
       }

       SCommand a;
       a.nOper = token[1].dw;
       a.nRes = AddTemp( TYPE_BOOL );
       a.nArg1 = nRes1;
       a.nArg2 = nRes2;
       AddComm( a );
       SCommand b;
       b.nOper = token[3].dw;
       b.nRes = AddTemp( TYPE_BOOL );
       b.nArg1 = a.nRes;
       b.nArg2 = nRes3;
       AddComm( b ); 
       return nEnd;
  }
  else
  if( nEnd==7 
      && (token[1].dw==OPER_EQ ||token[1].dw==OPER_MEN ||token[1].dw==OPER_BOL ||token[1].dw==OPER_BOL ||token[1].dw==OPER_NEQ ||token[1].dw==OPER_BEQ ||token[1].dw==OPER_MEQ)
      && (token[3].dw==OPER_OR || token[3].dw==OPER_AND) 
      && (token[5].dw==OPER_EQ ||token[5].dw==OPER_MEN ||token[5].dw==OPER_BOL ||token[5].dw==OPER_BOL ||token[5].dw==OPER_NEQ ||token[5].dw==OPER_BEQ ||token[5].dw==OPER_MEQ)
    )
   { //if me.DataValue("IS2120_FL.2120_ST.PV")=5 or me.DataValue("IS2120_FL.2120_ST.PV")=6 then 
       if ( token[1].dw == OPER_EQ )
          token[1].dw = OPER_EQ_EQ;
       if ( token[5].dw == OPER_EQ )
          token[5].dw = OPER_EQ_EQ;
       UINT nRes1 = AddArgument(token[0], true);
       if(nRes1<0)
         return -1;
       UINT nComm1 = AddCommand( token[1].dw, nRes1, token[2], &nRes1, true );
       if(nComm1<0)
         return -1;
       UINT nRes2 = AddArgument(token[4], true);
       if(nRes2<0)
         return -1;
       UINT nComm2 = AddCommand( token[5].dw, nRes2, token[6], &nRes2, true );
       if(nComm2<0)
         return -1;
       SCommand a;
       a.nOper = token[3].dw;
       a.nRes = AddTemp( TYPE_BOOL );
       a.nArg1 = nRes1;
       a.nArg2 = nRes2;
       AddComm( a );
       return nEnd;
   } 
  else
  if( nEnd==11 
      && (token[1].dw==OPER_EQ ||token[1].dw==OPER_MEN ||token[1].dw==OPER_BOL ||token[1].dw==OPER_BOL ||token[1].dw==OPER_NEQ ||token[1].dw==OPER_BEQ ||token[1].dw==OPER_MEQ)
      && (token[3].dw==OPER_OR || token[3].dw==OPER_AND) 
      && (token[5].dw==OPER_EQ ||token[5].dw==OPER_MEN ||token[5].dw==OPER_BOL ||token[5].dw==OPER_BOL ||token[5].dw==OPER_NEQ ||token[5].dw==OPER_BEQ ||token[5].dw==OPER_MEQ)      
      && (token[7].dw==OPER_OR || token[7].dw==OPER_AND) 
      && (token[9].dw==OPER_EQ ||token[9].dw==OPER_MEN ||token[9].dw==OPER_BOL ||token[9].dw==OPER_BOL ||token[9].dw==OPER_NEQ ||token[9].dw==OPER_BEQ ||token[9].dw==OPER_MEQ)
    )
  {//if DBL = 0 and FLT = 1 and ALM = 0
       if ( token[1].dw == OPER_EQ )
          token[1].dw = OPER_EQ_EQ;
       if ( token[5].dw == OPER_EQ )
          token[5].dw = OPER_EQ_EQ;
       if ( token[9].dw == OPER_EQ )
          token[9].dw = OPER_EQ_EQ;
       UINT nRes1 = AddArgument(token[0], true);
       if(nRes1<0)
         return -1;
       UINT nComm1 = AddCommand( token[1].dw, nRes1, token[2], &nRes1, true );
       if(nComm1<0)
         return -1;
       UINT nRes2 = AddArgument(token[4], true);
       if(nRes2<0)
         return -1;
       UINT nComm2 = AddCommand( token[5].dw, nRes2, token[6], &nRes2, true );
       if(nComm2<0)
         return -1;

       UINT nRes3 = -1;
       if( token[9].dw == OPER_EQ_EQ && nCount > 13 && token[10].dw == FUNC_GETELEMENTBYID && token[11].dw == VALUE_OBJECT)
       { //if (PV = "-9999" or PV = "----" or document.getElementById("alpha" & i).style.visibility = "hidden") then
         UINT nRes_3 = AddArgument(token[8], true); //document.getElementById("alpha" & i)
         if(nRes3<0)
           return -1;
         UINT nRes_3_ = -1;
         UINT nComm_3 = AddCommand( OPER_EQ, nRes_3, token[10], &nRes_3_, true ); //tmp = document.getElementById("alpha" & i)
         if(nComm_3<0)
           return -1;
         UINT nRes4 = AddArgument(token[11], true);  //tmp.style.visibility
         if ( token[12].dw == OPER_EQ )
           token[12].dw = OPER_EQ_EQ;
         UINT nComm3 = AddCommand( token[12].dw, nRes4, token[13], &nRes3, true ); //tmp.style.visibility = "hidden"
         if(nComm3<0)
          return -1;
         nEnd += 3; 
       }
       else
       {
       nRes3 = AddArgument(token[8], true);
       if(nRes3<0)
         return -1;
       UINT nComm3 = AddCommand( token[9].dw, nRes3, token[10], &nRes3, true );
       if(nComm3<0)
         return -1;
       }
       SCommand a;
       a.nOper = token[3].dw;
       a.nRes = AddTemp( TYPE_BOOL );
       a.nArg1 = nRes1;
       a.nArg2 = nRes2;
       AddComm( a );   //DBL = 0 and FLT = 1
       SCommand b;
       b.nOper = token[7].dw;
       b.nRes = AddTemp( TYPE_BOOL );
       b.nArg1 = a.nRes;
       b.nArg2 = nRes3;
       AddComm( b );   //DBL = 0 and FLT = 1 and ALM = 0
       return nEnd;
  }
  else
  if( nEnd==19 
      && (token[1].dw==OPER_EQ ||token[1].dw==OPER_MEN ||token[1].dw==OPER_BOL ||token[1].dw==OPER_BOL ||token[1].dw==OPER_NEQ ||token[1].dw==OPER_BEQ ||token[1].dw==OPER_MEQ)
      && (token[3].dw==OPER_OR || token[3].dw==OPER_AND) 
      && (token[5].dw==OPER_EQ ||token[5].dw==OPER_MEN ||token[5].dw==OPER_BOL ||token[5].dw==OPER_BOL ||token[5].dw==OPER_NEQ ||token[5].dw==OPER_BEQ ||token[5].dw==OPER_MEQ)      
      && (token[7].dw==OPER_OR || token[7].dw==OPER_AND) 
      && (token[9].dw==OPER_EQ ||token[9].dw==OPER_MEN ||token[9].dw==OPER_BOL ||token[9].dw==OPER_BOL ||token[9].dw==OPER_NEQ ||token[9].dw==OPER_BEQ ||token[9].dw==OPER_MEQ)
      && (token[11].dw==OPER_OR || token[11].dw==OPER_AND) 
      && (token[13].dw==OPER_EQ ||token[13].dw==OPER_MEN ||token[13].dw==OPER_BOL ||token[13].dw==OPER_BOL ||token[13].dw==OPER_NEQ ||token[13].dw==OPER_BEQ ||token[13].dw==OPER_MEQ)
      && (token[15].dw==OPER_OR || token[15].dw==OPER_AND) 
      && (token[17].dw==OPER_EQ ||token[17].dw==OPER_MEN ||token[17].dw==OPER_BOL ||token[17].dw==OPER_BOL ||token[17].dw==OPER_NEQ ||token[17].dw==OPER_BEQ ||token[17].dw==OPER_MEQ)
    )
  {//if	ai1 > trip and ai2 > trip and ai3 > trip and di1 = 1 and di2 = 1 then 
       if ( token[1].dw == OPER_EQ )
          token[1].dw = OPER_EQ_EQ;
       if ( token[5].dw == OPER_EQ )
          token[5].dw = OPER_EQ_EQ;
       if ( token[9].dw == OPER_EQ )
          token[9].dw = OPER_EQ_EQ;
       if ( token[13].dw == OPER_EQ )
          token[13].dw = OPER_EQ_EQ;
       if ( token[17].dw == OPER_EQ )
          token[17].dw = OPER_EQ_EQ;

       //ai1 > trip
       UINT nRes1 = AddArgument(token[0], true);
       if(nRes1<0)
         return -1;
       UINT nComm1 = AddCommand( token[1].dw, nRes1, token[2], &nRes1, true );
       if(nComm1<0)
         return -1;
       //ai2 > trip
       UINT nRes2 = AddArgument(token[4], true);
       if(nRes2<0)
         return -1;
       UINT nComm2 = AddCommand( token[5].dw, nRes2, token[6], &nRes2, true );
       if(nComm2<0)
         return -1;
       //ai3 > trip
       UINT nRes3 = AddArgument(token[8], true);
       if(nRes3<0)
         return -1;
       UINT nComm3 = AddCommand( token[9].dw, nRes3, token[10], &nRes3, true );
       if(nComm3<0)
         return -1;
       //di1 = 1
       UINT nRes4 = AddArgument(token[12], true);
       if(nRes4<0)
         return -1;
       UINT nComm4 = AddCommand( token[13].dw, nRes3, token[14], &nRes3, true );
       if(nComm4<0)
         return -1;
       //di2 = 1
       UINT nRes5 = AddArgument(token[16], true);
       if(nRes5<0)
         return -1;
       UINT nComm5 = AddCommand( token[17].dw, nRes3, token[18], &nRes3, true );
       if(nComm5<0)
         return -1;

       SCommand a;
       a.nOper = token[3].dw;
       a.nRes = AddTemp( TYPE_BOOL );
       a.nArg1 = nRes1;
       a.nArg2 = nRes2;
       AddComm( a );   //ai1 > trip and ai2 > trip and ai3 > trip

       SCommand b;
       b.nOper = token[7].dw;
       b.nRes = AddTemp( TYPE_BOOL );
       b.nArg1 = a.nRes;
       b.nArg2 = nRes3;
       AddComm( b );   //ai1 > trip and ai2 > trip and ai3 > trip

       SCommand c;
       c.nOper = token[11].dw;
       c.nRes = AddTemp( TYPE_BOOL );
       c.nArg1 = b.nRes;
       c.nArg2 = nRes4;
       AddComm( c );   //ai1 > trip and ai2 > trip and ai3 > trip and di1 = 1

       SCommand d;
       d.nOper = token[15].dw;
       d.nRes = AddTemp( TYPE_BOOL );
       d.nArg1 = c.nRes;
       d.nArg2 = nRes4;
       AddComm( d );   //ai1 > trip and ai2 > trip and ai3 > trip and di1 = 1 and di2 = 1
       return nEnd;
  }
  //
  UINT nComm;
  UINT nRes = AddArgument(token[0],true);
  if ( As(nRes) ) 
      return -1;

  if( nEnd == 5 && token[1].dw==OPER_AND && IsCompareOper(token[3]) )
  { // if (me.DataValue("SASH_101.DO.PV") = 0 or me.DataValue("SAS_101.DO.PV") = 0) and me.DataValue("101J_5.DB_101J_1.PV") = 0 then
     SCommand a;
     a.nRes = AddTemp( TYPE_BOOL );
     a.nOper = token[3].dw;
     a.nArg1 = AddArgument(token[2],true);
     a.nArg2 = AddArgument(token[4],true);
     AddComm( a ); 

     SCommand b;
     b.nRes = AddTemp( TYPE_BOOL );
     b.nOper = token[1].dw;
     b.nArg1 = nRes;
     b.nArg2 = a.nRes;
     AddComm( b );
     return nEnd;
  }

  for ( int n = 1; n < nEnd; n+=2 )
    {
    if ( token[n].dw == OPER_EQ )
      token[n].dw = OPER_EQ_EQ;
    nComm = AddCommand( token[n].dw, nRes, token[n+1], &nRes, true );
    if(token[n+1].dw == OPER_NOT)
      n++;
    if ( As(nComm) ) return -1;
    }
  return nEnd;
  }
//
int CVBScript::KeyLogic( SToken* token, int nCount, DWORD dwIF )
  {
  int N = KeyArgus( token, nCount );
  if ( N < 0 ) return N;
  //
  SCommand& last = CommLast();
  GVAR& v = mGVAR.Obj(last.nRes);
  //
  if ( last.nOper == OPER_EQ )
    last.nOper = OPER_EQ_EQ;
  //
  if ( v.nType != TYPE_BOOL )
    {
    v.nType = TYPE_BOOL;
    v.nSize = sizeof(bool);
    }
  const char* name = mGVAR.Name(last.nRes);
  // В конце ставим условие
  SCommand a;
  a.nOper = dwIF;//Операция
  a.nRes  = last.nRes;//Результат
  a.nArg1 = -1;//Куда идти при true
  a.nArg2 = -1;//Куда идти при false
  UINT nComm = AddComm( a );
  if ( As(nComm) ) return -1;
  return N;
  }
//

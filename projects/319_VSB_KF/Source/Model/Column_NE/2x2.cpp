#include "stdafx.h"
#include "2x2.h"
#include "math.h"
#include "CommProc.h"

bool Inv ( M22 & Res, M22 & M )
  {
  double det = M.a00 * M.a11 - M.a10 * M.a01;
  if ( fabs ( det ) < 1e-10 )
    return false;
  det = 1. / det;
  M22 R;
  R.a00 = M.a11 * det;
  R.a11 = M.a00 * det;
  R.a01 = -M.a01 * det;
  R.a10 = -M.a10 * det;
  memmove ( &Res, &R, sizeof ( M22 ));
  return true;
  }

void Mult ( M22 & Res, M22 & M1, M22 & M2 )
  {
  M22 R;
  R.a00 = M1.a00 * M2.a00 + M1.a01 * M2.a10;
  R.a10 = M1.a10 * M2.a00 + M1.a11 * M2.a10;
  R.a01 = M1.a00 * M2.a01 + M1.a01 * M2.a11;
  R.a11 = M1.a10 * M2.a01 + M1.a11 * M2.a11;
  memmove ( &Res, &R, sizeof ( M22 ));
  }

void Add ( M22 & Res, M22 & M1, M22 & M2 )
  {
  Res.a00 = M1.a00 + M2.a00;
  Res.a01 = M1.a01 + M2.a01;
  Res.a10 = M1.a10 + M2.a10;
  Res.a11 = M1.a11 + M2.a11;
  }

void Sub ( M22 & Res, M22 & M1, M22 & M2 )
  {
  Res.a00 = M1.a00 - M2.a00;
  Res.a01 = M1.a01 - M2.a01;
  Res.a10 = M1.a10 - M2.a10;
  Res.a11 = M1.a11 - M2.a11;
  }

void Sgn ( M22 & Res, M22 & M )
  {
  Res.a00 =  -M.a00;
  Res.a01 =  -M.a01;
  Res.a10 =  -M.a10;
  Res.a11 =  -M.a11;
  }

void Mult ( M2 & Res, M22 & M, M2 & V )
  {
  M2 R;
  R.a0 = M.a00 * V.a0 + M.a01 * V.a1;
  R.a1 = M.a10 * V.a0 + M.a11 * V.a1;
  memmove ( &Res, &R, sizeof ( M2 ));
  }

void Add ( M2 & Res, M2 & M1, M2 & M2 )
  {
  Res.a0 = M1.a0 + M2.a0;
  Res.a1 = M1.a1 + M2.a1;
  }

void Sub ( M2 & Res, M2 & M1, M2 & M2 )
  {
  Res.a0 = M1.a0 - M2.a0;
  Res.a1 = M1.a1 - M2.a1;
  }

void Sgn ( M2 & Res, M2 & M )
  {
  Res.a0 =  -M.a0;
  Res.a1 =  -M.a1;
  }

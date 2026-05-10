#pragma once


//typedef 
//double M22[2][2];

//typedef 
//double M2[2];

struct M22
  {
  double a00, a01, a10, a11;
  };

struct M2
  {
  double a0, a1;
  };

bool Inv ( M22 & Res, M22 & M );
void Mult ( M22 & Res, M22 & M1, M22 & M2 );
void Add ( M22 & Res, M22 & M1, M22 & M2 );
void Sub ( M22 & Res, M22 & M1, M22 & M2 );
void Sgn ( M22 & Res, M22 & M );

void Mult ( M2 & Res, M22 & M1, M2 & M2 );
void Add ( M2 & Res, M2 & M1, M2 & M2 );
void Sub ( M2 & Res, M2 & M1, M2 & M2 );
void Sgn ( M2 & Res, M2 & M );

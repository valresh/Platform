#include "stdafx.h"
#include "CommProc.h"
#include "Err.h"

bool SolveMatr ( int N, double M_comm[], double V[], double X[] );
typedef 
void (*appr) ( int & N, int n, double &F, double f[] ); 

#define M_comm(r,c) Matr[(r)*N+(c)]
#define V(r) Vect[r]

void Appr ( appr fappr, int kPnt, double X[], double & MaxErr, double & Err, int & nMax  )
	{
  double Matr[10000];
  double Vect[100];
	double f[100];
	memset ( Matr, 0, sizeof ( Matr ));
	memset ( Vect, 0, sizeof ( Vect ));
	memset ( f, 0, sizeof ( f ));
	int N;
	for ( int n = 0; n < kPnt; n++ )
		{
		double F;
		(*fappr) ( N, n, F, f ); 
		for ( int r = 0; r < N; r++ )
			{
			for ( int c = 0; c < N; c++ )
				{
				M_comm(r,c) += f[r] * f[c];
				}
			V(r) += F * f[r];
			}
		}
	ASS(SolveMatr ( N, Matr, Vect, X ));
	MaxErr = 0.;
	nMax = -1;
	Err = 0.;
	for ( int n = 0; n < kPnt; n++ )
		{
		double F;
		(*fappr) ( N, n, F, f );
		double S = -F;
		for ( int c = 0; c < N; c++ )
			{
			S += X[c] * f[c];
			}
		Err += S * S;
		if ( fabs ( S ) > MaxErr )
			{
			MaxErr = fabs ( S );
			nMax = n;
			}
		}
	Err /= kPnt;
	Err = sqrt ( Err );
	}

#define A(i,j) *(M_comm+(i)*N+(j))
#define B(i,j) *(W+(i)*N+(j))
#define v(i) *(V+(i))
#define x(i) *(X+(i))
#define y(i) *(Y+(i))


bool SolveMatr ( int N, double M_comm[], double V[], double X[] )
	{
	double W[1000];
	double Y[1000];
	for ( int k = 0; k < N; k++ )
		{
		double S = A(k,k);
		for ( int j = 0; j < k; j++ )
			S -= B(k,j) * B(k,j);
		if ( S <= 0. )
			return false;
		double Bkk = sqrt ( S );
		B(k,k) = Bkk;
		for ( int i = k + 1; i < N; i++ )
			{
			S = A(i,k);
			for ( int j = 0; j < k; j++ )
				S -= B(i,j) * B(k,j);
			B(i,k) = S / Bkk;
			}
		}
//
	for ( int k = 0; k < N; k++ )
		{
		double S = v(k);
		for ( int j = 0; j < k; j++ )
			S -= B(k,j) * y(j);
		y(k) = S / B(k,k);
		}
	for ( int k = N-1; k >= 0; k-- )
		{
		double S = y(k);
		for ( int j = k+1; j < N; j++ )
			S -= B(j,k) * x(j);
		x(k) = S / B(k,k);
		}
	return true;
	}



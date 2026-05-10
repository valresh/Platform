#pragma once

#define MATR_P(P) \
    {\
    double * Eq = A;\
	  Eq[P_0] = 1.;\
	  Eq[F_0] = 0.;\
    B[0] = P;\
    return 0;\
    }

#define MATR_F(F) \
    {\
    double * Eq = A;\
	  Eq[F_0] = 1.;\
    B[0] = F;\
    return 0;\
    }

#define MATR_OMEGA(Omega) \
    {\
    double * Eq = A;\
	  Eq[P_0] = Omega;\
	  Eq[P_1] = -Omega;\
	  Eq[F_0] = -1.;\
    Eq += 4;\
    Eq[F_0] = -1.;\
    Eq[F_1] = 1.;\
    return 0;\
    }

#define MATR_IN(P,Omega) \
    {\
    double * Eq = A;\
	  Eq[P_0] = (Omega);\
	  Eq[F_0] = -1.;\
    B[0] = (Omega)*(P);\
    return 0;\
    }

#define MATR_OUT(P,Omega) \
    {\
    double * Eq = A;\
	  Eq[P_0] = (Omega);\
	  Eq[F_0] = 1.;\
    B[0] = (Omega)*(P);\
    return 0;\
    }


typedef double Row_2[2];
typedef double Row_3[3];
typedef double Row_4[4];

#define P_0 0
#define F_0 1
#define P_1 2
#define F_1 3
#define P_2 4
#define F_2 5
#define P_3 6
#define F_3 7
#define P_4 8
#define F_4 9


#pragma once
#ifndef FULL_LIB_H
#define FULL_LIB_H
#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <math.h>
#include <vector>
#include <time.h>
#include <fstream>

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define ABS(x) abs(x)
#define FLIP(i) (-(i)-2)
#define UNFLIP(i) (((i) < 0) ? FLIP(i) : (i))
#define MARKED(w, j) (w[j] < 0)
#define MARK(w, j) \
	{ w[j] = FLIP(w[j]); }
#define BLK_SIZE 500               /* minimum size is 1 */
#if __cplusplus <= 201103L
int isinf(double x);
int isnan(double x);
#endif


struct SparseMatrix /* matrix in compressed-column form */
{
	int nzmax; /* maximum number of entries */
	int m;     /* number of rows */
	int n;     /* number of columns */
	int* p;    /* column pointers (size n+1) */
	int* i;    /* row indices, size nzmax */
	double* x; /* numerical values, size nzmax */
	bool isOff;
	int off_nnz;
	int off_n;
	SparseMatrix(int offN, int offNZ) { p = NULL; i = NULL; x = NULL; n = 0; isOff = true; off_nnz = offNZ; off_n = offN; };
	SparseMatrix(int M, int N, int NNZ, int n_blk = 0);
	~SparseMatrix();
	void ZeroInit(int M, int N, int NNZ);
	void PrintMatrToFile(char * filepth1, char * filepth2, double* brhs);
};

struct SymbFactor /* symbolic LU */
{
	int* q;        /* fill-reducing column permutation for LU*/
	double lnz;    /* # entries in L for LU */
	double unz;    /* # entries in U for LU */
	int* pinv;     /* inverse row perm. for QR, fill red. perm for Chol */
	int* parent;   /* elimination tree for Cholesky and QR */
	int* cp;       /* column pointers for Cholesky, row counts for QR */
	int* leftmost; /* leftmost[i] = min(find(A(i,:))), for QR */
	int m2;        /* # of rows for QR, after adding fictitious rows */
	bool QR;
	SymbFactor(int NNZ, bool qr = false);
	~SymbFactor();
	void Set_q(int* perm, int N);
};

class dmPermutation;
struct NumbFactor		/* numeric  LU */
{
	SparseMatrix* L;  /* L for LU and Cholesky, V for QR */
	SparseMatrix* U;  /* U for LU, r for QR, not used for Cholesky */
	int* pinv;        /* partial pivoting for LU */
	double* B;       /* beta [0..n-1] for QR */
	bool QR;
	bool isBlock;
	int* snz;
	int nb;
	NumbFactor() {}
	NumbFactor(int M, int N, int lnz, int unz, bool qr = false);
	~NumbFactor();
	void ZeroInit(int M, int N, int lnz, int unz, bool qr = false);
	void SetOffset(dmPermutation* DM);
};
SparseMatrix* offsetA(SparseMatrix* A, int* off);
NumbFactor* offsetN(NumbFactor* N, int* off);
struct dm_perm_res  /* DM permutation results */
{
	int* p;        /* size m, row permutation */
	int* q;        /* size n, column permutation */
	int* r;        /* size nb+1, block k is rows r[k] to r[k+1]-1 in A(p,q) */
	int* s;        /* size nb+1, block k is cols s[k] to s[k+1]-1 in A(p,q) */
	int nb;        /* # of blocks in fine dmperm decomposition */
	int rr[5];     /* coarse row decomposition */
	int cc[5];     /* coarse column decomposition */
	int n;
	int m;
	dm_perm_res(int ncol, int nrow);
	void clear();
	~dm_perm_res();
};


struct utils
{
	void lsolve(SparseMatrix* L, double* x);
	void usolve(SparseMatrix* U, double* x);
	void ipvec(const int* p, const double* b, double* x, int n);
	int dfs(int j, SparseMatrix* G, int top, int* xi, int* pstack, const int* pinv);
	void randperm(int n, int seed, int* p);
	double cumsum(int* p, int* c, int n);
	int happly(const SparseMatrix* V, int i, double beta, double* x);
	int reach(SparseMatrix* G, const SparseMatrix* B, int k, int* xi, const int* pinv, int n_row = 0);
	int spsolve(SparseMatrix* G, const SparseMatrix* B, int k, int* xi, double* x, const int* pinv, int lo, int n_row = 0);
	int leaf(int i, int j, const int* first, int* maxfirst, int* prevleaf, int* ancestor, int* jleaf);
	int tdfs(int j, int k, int* head, const int* next, int* post, int* stack);
	int scatter(const SparseMatrix* A, int j, double beta, int* w, double* x, int mark, SparseMatrix* C, int nz);
	double house(double* x, double* beta, int n);
	void augment(int k, const SparseMatrix* A, int* jmatch, int* cheap, int* w, int* js, int* is, int* ps);
	int bfs(const SparseMatrix* A, int n, int* wi, int* wj, int* queue, const int* imatch, const int* jmatch, int mark, int* Work);
	void unmatched(int m, const int* wi, int* p, int* rr, int set);
	void matched(int n, const int* wj, const int* imatch, int* p, int* q, int* cc, int* rr, int set, int mark);
	void ipv_rows(const int* p, const int* Ai, int* Ai_new, int n);

	SparseMatrix* permute(const SparseMatrix* A, const int* pinv, const int* q, int values);
	SparseMatrix* transpose(const SparseMatrix* A, int* work);
	SparseMatrix* plus(const SparseMatrix* A, const SparseMatrix* B, double alpha, double beta);
	SparseMatrix* multiply(const SparseMatrix* A, const SparseMatrix* B);
};


class dmPermutation : public utils
{
	int* jimatch;
	int* w;
	int* q;
	int* xi;
	int* P;
	int* R;
	int* Work;
	double* dWork;
	dm_perm_res* Dscc;
	SparseMatrix* C_perm;
	SparseMatrix* C_ones;
	SparseMatrix* Csum;
	SparseMatrix* CT;

public:
	int N_Blk;
	dm_perm_res* D;
	SparseMatrix* C2;
	int* pinv;
	int* comcon;
	int* comcon_NNZ;

	dmPermutation(int nrow, int ncol);
	~dmPermutation();

	void tr(const SparseMatrix* A, int* work);
	void add(const SparseMatrix* A, const SparseMatrix* B, int* Work, double* dWork);
	void spones(const SparseMatrix* A);
	void perm(const SparseMatrix* A, const int* pinv, const int* q);
	void strong_con_comp(SparseMatrix* A);
	int f_keep(SparseMatrix* A, int (*fkeep) (int, int, double, void*), void* other);
	void p_inv(int const* p, int n);
	void maxtrans(const SparseMatrix* A, int seed);
	void Run(const SparseMatrix* A, int seed = 1);
	void sccomp(SparseMatrix* A, int* xi);
};

struct Col_struct
{
	int start;  /* index for A of first row in this column, or DEAD */
	/* if column is dead */
	int length; /* number of rows in this column */
	union
	{
		int thickness; /* number of original columns represented by this */
		/* col, if the column is alive */
		int parent;    /* parent in parent tree super-column structure, if */
		/* the column is dead */
	} shared1;
	union
	{
		int score; /* the score used to maintain heap, if col is alive */
		int order; /* pivot ordering of this column, if col is dead */
	} shared2;
	union
	{
		int headhash; /* head of a hash bucket, if col is at the head of */
		/* a degree list */
		int hash;     /* hash value, if col is not in a degree list */
		int prev;     /* previous column in degree list, if col is in a */
		/* degree list (but not at the head of a degree list) */
	} shared3;
	union
	{
		int degree_next; /* next column, if col is in a degree list */
		int hash_next;   /* next column, if col is in a hash list */
	} shared4;
};

struct Row_struct
{
	int start;  /* index for A of first col in this row */
	int length; /* number of principal columns in this row */
	union
	{
		int degree; /* number of principal & non-principal columns in row */
		int p;      /* used as a row pointer in init_rows_cols () */
	} shared1;
	union
	{
		int mark;         /* for computing set differences and marking dead rows*/
		int first_column; /* first column in row (used in garbage collection) */
	} shared2;
};

class ReOdering
{
	int n_col;
	int n_row;
	int Alen;
	int knobs[3];
	size_t Col_size;
	size_t Row_size;
	size_t need;
	Row_struct* Row;            /* pointer into A of Row [0..n_row] array */
	Col_struct* Col;            /* pointer into A of Col [0..n_col] array */
	int* A;
public:
	ReOdering(int N, int M, int NNZ, int* row_ind, int offset = 0);
	~ReOdering();
	void init_rows_cols(int p[]);
	void order_children(int p[]);
	void detect_super_cols(int head[], int row_start, int row_length);
	int clear_mark(int tag_mark, int max_mark);
	int garbage_collection(int* pfree);
	int find_ordering(int head[], int n_col2, int max_deg, int pfree, int aggressive);
	void init_scoring(int head[], int* p_n_row2, int* p_n_col2, int* p_max_deg);
	void Run(int* p);
	void PrintNewOrder(int p[]);
};



class Solver : public utils
{
	bool isZero;
	SparseMatrix* A_for_DM;
public:
	ReOdering* ReO;
	SparseMatrix* matrA;
	SymbFactor* S;
	NumbFactor* N;
	SparseMatrix* C;
	dmPermutation* DM;

	std::vector<SparseMatrix*> At;
	std::vector<NumbFactor*> Nt;

	int mInt;
	int nInt;
	int nnzMaybe;
	int* W;

	int* Ti_n;
	int* colOrder;
	int *compressPosition;

	bool isLU;
	bool isQR;
	bool is_A;
	bool isAdditional;

	int NNZ;


	Solver();
	Solver(int nrow, int ncol);
	~Solver();

	int compress(int NNZ, const int Ti[], const int Tj[], const double Tx[], SparseMatrix* matrA);
	void updateValues(const double Tx[], SparseMatrix* matrA);
	void ZeroStep(int nnzv);
	void fill_A(const int Ti[], const int Tj[], const double Tx[]);
	void fill_A_2(const int Ti[], const int Tj[], const double Tx[]);
	int LU(NumbFactor* N, const SparseMatrix* A, const SymbFactor* S, double tol, double* x, int* xi);
	int LU_2(NumbFactor* N, const SparseMatrix* A, const int* q, double tol, double* x, int* xi, int n_row);
	void solve_from_LU(NumbFactor* N, SymbFactor* S, double* b, int n, double* x);
	void solve_from_LU_2(NumbFactor* N, int* q, double* b, int n, double* x);
	void clearOffsets(int n);

};

class QRsolver : public Solver
{
	int* w;
	int* pst;
	bool isS;
public:
	QRsolver(int N, int M, int* Q, SparseMatrix* A);
	QRsolver(int N, int M, SparseMatrix* A);
	QRsolver(int N, int M);
	~QRsolver();
	void etree(const SparseMatrix* A, int ata);
	void post(int n);
	int vcount(const SparseMatrix* A);
	//SparseMatrix* tr(const SparseMatrix* A, int values);
	static void init_ata(SparseMatrix* AT, const int* post, int* w, int** head, int** next);
	void counts(const SparseMatrix* A, const int* parent, const int* post, int ata);
	void sqr(int order, const SparseMatrix* A);
	void qr(const SparseMatrix* A);
	void solve_from_qr(double* b, int m, int n);
	void Run_from_lu(SparseMatrix* A, double* b);
	void Run(double* b);
	void solve(int* Q, SparseMatrix* A, double* b, int off_n = 0, int off_nnz = 0);
	void solve_2(int* Q, SparseMatrix* A, double* b, SymbFactor* S_extern);

};

class LUsolver : public Solver
{
	double* b_data;
	double* b_perm;

public:
	QRsolver* QR;
	double* x_tmp, * x;
	int* xi;
	bool isPer;

	LUsolver(int Nrow);
	LUsolver(int Nrow, int nnzv, const int Ti[], const int Tj[], const double Tx[]);
	~LUsolver();
	void Run(double* b_tmp, double *Xres);
	void Run_blk(double* b_tmp, double *Xres);
	void prep_blk(int nnzv, const int Ti[], const int Tj[], const double Tx[], double* b_tmp, double *Xres);
	void solve(bool flag, int nnzv, const int Ti[], const int Tj[], const double Tx[], double* b_tmp, double *Xres);
	void solve_blk(int nnzv, const int Ti[], const int Tj[], const double Tx[], double* b_tmp, double *Xres);
	void light_solve_blk(const double Tx[], double* b_tmp, double *Xres);
};


void PrintToFile(int nrow, int nnz, const int Ti[], const int Tj[], const double Tx[], const double *b);


#endif 
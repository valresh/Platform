#pragma once
#include "Full_Lib.h"
#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <math.h>
#include <vector>
#include <float.h>
#include "omp.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <functional>
#include <omp.h>

#define __max(a,b)  (((a) > (b)) ? (a) : (b))
#define __min(a,b)  (((a) < (b)) ? (a) : (b))

#if __cplusplus <= 201103L
int isinf(double x)
{
    union { size_t u; double f; } ieee754;
    ieee754.f = x;
    return ((unsigned)(ieee754.u >> 32) & 0x7fffffff) == 0x7ff00000 &&
        ((unsigned)ieee754.u == 0);
}
int isnan(double x)
{
    union { size_t u; double f; } ieee754;
    ieee754.f = x;
    return ((unsigned)(ieee754.u >> 32) & 0x7fffffff) +
        ((unsigned)ieee754.u != 0) > 0x7ff00000;
}
#endif


SparseMatrix::SparseMatrix(int M, int N, int NNZ, int n_blk)
{
    this->m = M; /* define dimensions and nzmax */
    this->n = N;
    this->nzmax = NNZ;
    this->p = new int[N + n_blk + 1];
    this->i = new int[NNZ];
    this->x = new double[NNZ];
    isOff = false;
    off_nnz = 0;
    off_n = 0;
}


SparseMatrix::~SparseMatrix()
{
    if (!isOff)
    {
        delete[] this->p;
        delete[] this->i;
        delete[] this->x;
    }
}
void SparseMatrix::ZeroInit(int M, int N, int NNZ)
{
    this->m = M; /* define dimensions and nzmax */
    this->n = N;
    this->nzmax = NNZ;
}
#include <fstream>
void SparseMatrix::PrintMatrToFile(char * filepth1, char * filepth2, double * brhs)
{
    int* Ai, * Ap;
    double* Ax;
    Ai = this->i;
    Ax = this->x;
    Ap = this->p;
    std::ofstream fout_A(filepth1);
    std::ofstream fout_b(filepth2);
    for (int i = 0; i < n; ++i)
        for (int v = Ap[i]; v < Ap[i + 1]; ++v)
            fout_A << i << " " << Ai[v] << " " << Ax[v] << std::endl;

    fout_b << n << "\n" << nzmax << "\n";
    for (int i = 0; i < n; ++i)
        fout_b << brhs[i] << "\n";

    fout_A.close();
    fout_b.close();
}

SymbFactor::SymbFactor(int NNZ, bool qr)
{
    this->q = new int[NNZ];
    QR = qr;
}

SymbFactor::~SymbFactor()
{
    delete[] this->q;
    if (QR)
    {
        delete[] this->pinv;
        delete[] this->leftmost;
        delete[] this->cp;
        delete[] this->parent;
    }
}

void SymbFactor::Set_q(int* perm, int N)	{memcpy(this->q, perm, N * sizeof(int));}

NumbFactor::NumbFactor(int M, int N, int lnz, int unz, bool qr)
{
    this->L = new SparseMatrix(M, N, lnz, N / 3);
    this->U = new SparseMatrix(M, N, unz, N / 3);
    this->pinv = new int[N + 1];
    int nbMaybe = N / 3;

    QR = qr;
    isBlock = false;
    if (QR)
        this->B = new double[N];
    if (!QR)
    {

        snz = new int[nbMaybe + 1];
    }
}

NumbFactor::~NumbFactor()
{
    if (!this->L->isOff)
    {
        delete[] this->pinv;
        if (QR)
            delete[] this->B;
        if (isBlock)
            delete[] this->snz;
    }
    delete this->L;
    delete this->U;
}
void NumbFactor::SetOffset(dmPermutation* DM)
{
    nb = DM->N_Blk;
    int offrow = 0;
    int offzeros = 0;
    int  nk;
    int N = this->L->n;
    size_t tmp = 0;
    isBlock = true;
    QR = false;
    int nbMaybe = N / 3;


    memset(snz, 0, (nb + 1) * sizeof(int));

    snz[0] = 0;
    for (int i = 0; i < nb; ++i)
    {
        nk = DM->comcon[i + 1] - DM->comcon[i];
        int nnzMaybe = (DM->comcon_NNZ[i + 1] - DM->comcon_NNZ[i]) * 5;
        snz[i + 1] = snz[i] + __min(nnzMaybe, nk * nk / 2);
    }
}

void NumbFactor::ZeroInit(int M, int N, int lnz, int unz, bool qr)
{
    this->L->ZeroInit(M, N, lnz);
    this->U->ZeroInit(M, N, unz);
    QR = qr;
}


dm_perm_res::dm_perm_res(int ncol, int nrow)
{
    n = nrow;
    m = ncol;
    p = new int[m];
    r = new int[m + 6];
    q = new int[n];
    s = new int[n + 6];
    this->clear();
}

void dm_perm_res::clear()
{
    std::memset(p, 0, m * sizeof(int));
    std::memset(r, 0, (m + 6) * sizeof(int));
    std::memset(q, 0, n * sizeof(int));
    std::memset(s, 0, (n + 6) * sizeof(int));
    std::memset(rr, 0, 5 * sizeof(int));
    std::memset(cc, 0, 5 * sizeof(int));
}

dm_perm_res::~dm_perm_res()
{
    delete[] p;
    delete[] r;
    delete[] q;
    delete[] s;
}

static int rprune(int i, int j, double aij, void* other)
{
    int* rr = (int*)other;
    return (i >= rr[1] && i < rr[2]);
}


void utils::lsolve(SparseMatrix* L, double* x)
{
    int p, j, n, * Lp, * Li;
    double* Lx;
    n = L->n;
    Lp = L->p;
    Li = L->i;
    Lx = L->x;
    for (j = 0; j < n; j++)
    {
        x[j] = (x[j] / Lx[Lp[j]]);
        for (p = Lp[j] + 1; p < Lp[j + 1]; p++)
            x[Li[p]] = (x[Li[p]] - Lx[p] * x[j]);       
     }
    
}

void utils::usolve(SparseMatrix* U, double* x)
{
    int p, j, n, * Up, * Ui;
    double* Ux;
    n = U->n;
    Up = U->p;
    Ui = U->i;
    Ux = U->x;
    for (j = n - 1; j >= 0; j--)
    {
        x[j] = (x[j] / Ux[Up[j + 1] - 1]);
        for (p = Up[j]; p < Up[j + 1] - 1; p++)
            x[Ui[p]] = (x[Ui[p]] - Ux[p] * x[j]);
    }
    
}

void utils::ipvec(const int* p, const double* b, double* x, int n)
{
    for (int k = 0; k < n; k++)
        x[p ? p[k] : k] = b[k];
}

int utils::dfs(int j, SparseMatrix* G, int top, int* xi, int* pstack, const int* pinv)
{
    int i, p, p2, done, jnew, head = 0, * Gp, * Gi;
    Gp = G->p;
    Gi = G->i;
    xi[0] = j; /* initialize the recursion stack */
    while (head >= 0)
    {
        j = xi[head]; /* get j from the top of the recursion stack */
        jnew = pinv ? (pinv[j]) : j;
        if (!MARKED(Gp, j))
        {
            MARK(Gp, j); /* mark node j as visited */
            pstack[head] = (jnew < 0) ? 0 : UNFLIP(Gp[jnew]);
        }
        done = 1; /* node j done if no unvisited neighbors */
        p2 = (jnew < 0) ? 0 : UNFLIP(Gp[jnew + 1]);

        for (p = pstack[head]; p < p2; p++)
        {              /* examine all neighbors of j */
            i = Gi[p]; /* consider neighbor node i */
            if (MARKED(Gp, i)) /* skip visited node i */
                continue;

            pstack[head] = p; /* pause depth-first search of node j */
            xi[++head] = i;   /* start dfs at node i */
            done = 0;         /* node j is not done */
            break;            /* break, to start dfs (i) */
        }

        if (done)
        {                  /* depth-first search at node j is done */
            head--;        /* remove j from the recursion stack */
            xi[--top] = j; /* and place in the output stack */
        }
    }
    return (top);
}

void utils::randperm(int n, int seed, int* p)
{
    int k, j, t;
    if (seed == 0)
        return;      /* return p = NULL (identity) */
    if (!p)
        return;             /* out of memory */
	std::memset(p, 0, n * sizeof(int));
    for (k = 0; k < n; k++) p[k] = n - k - 1;
    if (seed == -1)
        return;        /* return reverse permutation */
    srand(seed);                      /* get new random number seed */
    for (k = 0; k < n; k++)
    {
        j = k + (rand() % (n - k));    /* j = rand integer in range k to n-1 */
        t = p[j];                     /* swap p[k] and p[j] */
        p[j] = p[k];
        p[k] = t;
    }
    return;
}

double utils::cumsum(int* p, int* c, int n)
{
    int i, nz = 0;
    double nz2 = 0;
    if (!p || !c)
        return (-1);
    /* check inputs */
    for (i = 0; i < n; i++)
    {
        p[i] = nz;
        nz += c[i];
        nz2 += c[i]; /* also in double to avoid int overflow */
        c[i] = p[i]; /* also copy p[0..n-1] back into c[0..n-1]*/
    }
    p[n] = nz;
    return (nz2); /* return sum (c [0..n-1]) */
}

SparseMatrix* utils::transpose(const SparseMatrix* A, int* work)
{
    int p, q, j, * Cp, * Ci, n, m, * Ap, * Ai;
    double* Cx, * Ax;
    SparseMatrix* C;

    m = A->m;
    n = A->n;
    Ap = A->p;
    Ai = A->i;
    Ax = A->x;
    C = new SparseMatrix(m, n, A->nzmax);
    std::memset(work, 0, sizeof(int) * n);

    Cp = C->p;
    Ci = C->i;
    Cx = C->x;
    for (p = 0; p < Ap[n]; p++) work[Ai[p]]++;
    /* row counts */
    cumsum(Cp, work, m); /* row pointers */
    for (j = 0; j < n; j++)
        for (p = Ap[j]; p < Ap[j + 1]; p++)
        {
            Ci[q = work[Ai[p]]++] = j; /* place A(i,j) as entry C(j,i) */
            if (Cx) Cx[q] = Ax[p];
        }

    return C;
}


int utils::happly(const SparseMatrix* V, int i, double beta, double* x)
{
    int p, * Vp, * Vi;
    double* Vx, tau = 0;

    Vp = V->p; Vi = V->i; Vx = V->x;
    for (p = Vp[i]; p < Vp[i + 1]; p++)   /* tau = v'*x */
        tau += (Vx[p]) * x[Vi[p]];
    
    tau *= beta;                           /* tau = beta*(v'*x) */
    for (p = Vp[i]; p < Vp[i + 1]; p++)   /* x = x - v*tau */
        x[Vi[p]] -= Vx[p] * tau;

    return (1);
}

int utils::reach(SparseMatrix* G, const SparseMatrix* B, int k, int* xi, const int* pinv, int n_row)
{
    int p, n, top, * Bp, * Bi, * Gp;
    n = G->n;
    Bp = B->p;
    Bi = B->i;
    Gp = G->p;
    top = n;
    int start = Bp[k] - n_row;
    int end = Bp[k + 1] - n_row;
    for (p = start; p < end; p++)
        if (!MARKED(Gp, Bi[p])) /* start a dfs at unmarked node i */
            top = dfs(Bi[p], G, top, xi, xi + n, pinv);

    for (p = top; p < n; p++)    /* restore G */
        MARK(Gp, xi[p]);

    return (top);
}

int utils::spsolve(SparseMatrix* G, const SparseMatrix* B, int k, int* xi, double* x, const int* pinv, int lo, int n_row)
{
    int j, J, p, q, px, top, n, * Gp, * Gi, * Bp, * Bi;
    double* Gx, * Bx;
    int off_nnz = 0;
    int off_n = 0;


    Gp = G->p;
    Gi = G->i;
    Gx = G->x;
    n = G->n;
    Bp = B->p;
    Bi = B->i;
    Bx = B->x;
    top = reach(G, B, k, xi, pinv, n_row); /* xi[top..n-1]=Reach(B(:,k)) */
    for (p = top; p < n; p++) x[xi[p]] = (0);

    /* clear x */
    int start = Bp[k] - n_row;
    int end = Bp[k + 1] - n_row;
    for (p = start; p < end; p++) x[Bi[p]] = Bx[p];

    /* scatter B */
    for (px = top; px < n; px++)
    {
        j = xi[px];               /* x(j) is nonzero */
        J = pinv ? (pinv[j]) : j; /* j maps to col J of G */
        if (J < 0) /* column J is empty */
            continue;
                    
        x[j] = x[j] / Gx[Gp[J]]; /* x(j) /= G(j,j) */
        p = Gp[J] + 1;                       /* lo: L(j,j) 1st entry */
        q = Gp[J + 1];               /* up: U(j,j) last entry */
        for (; p < q; p++)
			x[Gi[p]] = (x[Gi[p]] - Gx[p] * x[j]); /* x(i) -= G(i,j) * x(j) */   
    }
    return (top); /* return top of stack */
}

int utils::leaf(int i, int j, const int* first, int* maxfirst, int* prevleaf, int* ancestor, int* jleaf)
{
    int q, s, sparent, jprev;
    if (!first || !maxfirst || !prevleaf || !ancestor || !jleaf) 
        return (-1);

    *jleaf = 0;
    if (i <= j || first[j] <= maxfirst[i]) /* j not a leaf */
        return (-1);

    maxfirst[i] = first[j]; /* update max first[j] seen so far */
    jprev = prevleaf[i];    /* jprev = previous leaf of ith subtree */
    prevleaf[i] = j;
    *jleaf = (jprev == -1) ? 1 : 2; /* j is first or subsequent leaf */
    if (*jleaf == 1) 
        return (i);

	/* if 1st leaf, q = root of ith subtree */
    for (q = jprev; q != ancestor[q]; q = ancestor[q])
;
    for (s = jprev; s != q; s = sparent) 
	{
        sparent = ancestor[s]; /* path compression */
        ancestor[s] = q;
    }
    return (q); /* q = least common ancester (jprev,j) */
}

int utils::tdfs(int j, int k, int* head, const int* next, int* post, int* stack)
{
    int i, p, top = 0;
    /* check inputs */
    stack[0] = j;       /* place j on the stack */
    while (top >= 0)  /* while (stack is not empty) */
	{  
        p = stack[top]; /* p = top of stack */
        i = head[p];    /* i = youngest child of p */
        if (i == -1) 
		{
            top--;         /* p has no unordered children left */
            post[k++] = p; /* node p is the kth postordered node */
        }
        else 
		{
            head[p] = next[i]; /* remove i from children of p */
            stack[++top] = i;  /* start dfs on child node i */
        }
    }
    return (k);
}

int utils::scatter(const SparseMatrix* A, int j, double beta, int* w, double* x, int mark, SparseMatrix* C, int nz)
{
    int i, p, * Ap, * Ai, * Ci;
    double* Ax;

    Ap = A->p; Ai = A->i; Ax = A->x; Ci = C->i;
    for (p = Ap[j]; p < Ap[j + 1]; p++)
    {
        i = Ai[p];                            /* A(i,j) is nonzero */
        if (w[i] < mark)
        {
            w[i] = mark;                      /* i is new entry in column j */
            Ci[nz++] = i;                     /* add i to pattern of C(:,j) */
            if (x) x[i] = beta * Ax[p];      /* x(i) = beta*A(i,j) */
        }
        else if (x) x[i] += beta * Ax[p];    /* i exists in C(:,j) already */
    }
    return (nz);
}

double utils::house(double* x, double* beta, int n)
{
    double s = 0;
    int i;
    if (!x || !beta) return (-1);          /* check inputs */
    /* s = norm(x) */
    for (i = 0; i < n; i++) s += x[i] * (x[i]);
    s = sqrt(s);
    if (s == 0)
    {
        (*beta) = 0;
        x[0] = 1;
    }
    else
    {
        /* s = sign(x[0]) * norm (x) ; */
        if (x[0] != 0)
            s *= x[0] / fabs(x[0]);

        x[0] += s;
        (*beta) = 1. / ((s)*x[0]);
    }
    return (-s);
}

void utils::augment(int k, const SparseMatrix* A, int* jmatch, int* cheap, int* w, int* js, int* is, int* ps)
{
    int found = 0, p, i = -1, * Ap = A->p, * Ai = A->i, head = 0, j;
    js[0] = k;                        /* start with just node k in jstack */
    while (head >= 0)
    {
        /* --- Start (or continue) depth-first-search at node j ------------- */
        j = js[head];                 /* get j from top of jstack */
        if (w[j] != k)                 /* 1st time j visited for kth path */
        {
            w[j] = k;                 /* mark j as visited for kth path */
            for (p = cheap[j]; p < Ap[j + 1] && !found; p++)
            {
                i = Ai[p];            /* try a cheap assignment (i,j) */
                found = (jmatch[i] == -1);
            }
            cheap[j] = p;             /* start here next time j is traversed*/
            if (found)
            {
                is[head] = i;         /* column j matched with row i */
                break;                 /* end of augmenting path */
            }
            ps[head] = Ap[j];        /* no cheap match: start dfs for j */
        }
        /* --- Depth-first-search of neighbors of j ------------------------- */
        for (p = ps[head]; p < Ap[j + 1]; p++)
        {
            i = Ai[p];                /* consider row i */
            if (w[jmatch[i]] == k) continue; /* skip jmatch [i] if marked */
            ps[head] = p + 1;         /* pause dfs of node j */
            is[head] = i;             /* i will be matched with j if found */
            js[++head] = jmatch[i];  /* start dfs at column jmatch [i] */
            break;
        }
        if (p == Ap[j + 1]) head--;     /* node j is done; pop from stack */
    }                                   /* augment the match if path found: */
    if (found) for (p = head; p >= 0; p--) jmatch[is[p]] = js[p];
}

int utils::bfs(const SparseMatrix* A, int n, int* wi, int* wj, int* queue, const int* imatch, const int* jmatch, int mark, int* Work)
{
    int* Ap, * Ai, head = 0, tail = 0, j, i, p, j2;
    SparseMatrix* C;
    for (j = 0; j < n; j++)           /* place all unmatched nodes in queue */
    {
        if (imatch[j] >= 0) continue; /* skip j if matched */
        wj[j] = 0;                    /* j in set C0 (R0 if transpose) */
        queue[tail++] = j;            /* place unmatched col j in queue */
    }
    if (tail == 0) return (1);         /* quick return if no unmatched nodes */
    C = (mark == 1) ? ((SparseMatrix*)A) : transpose(A, Work);
    if (!C) return (0);                /* bfs of C=A' to find R3,C3 from R0 */
    Ap = C->p; Ai = C->i;
    while (head < tail)                 /* while queue is not empty */
    {
        j = queue[head++];            /* get the head of the queue */
        for (p = Ap[j]; p < Ap[j + 1]; p++)
        {
            i = Ai[p];
            if (wi[i] >= 0) continue; /* skip if i is marked */
            wi[i] = mark;             /* i in set R1 (C3 if transpose) */
            j2 = jmatch[i];           /* traverse alternating path to j2 */
            if (wj[j2] >= 0) continue;/* skip j2 if it is marked */
            wj[j2] = mark;            /* j2 in set C1 (R3 if transpose) */
            queue[tail++] = j2;       /* add j2 to queue */
        }
    }
    if (mark != 1)
        delete C;      /* free A' if it was created */
    return (1);
}

void utils::unmatched(int m, const int* wi, int* p, int* rr, int set)
{
    int i, kr = rr[set];
    for (i = 0; i < m; i++) if (wi[i] == 0) p[kr++] = i;
    rr[set + 1] = kr;
}

void utils::matched(int n, const int* wj, const int* imatch, int* p, int* q,
    int* cc, int* rr, int set, int mark)
{
    int kc = cc[set], j;
    int kr = rr[set - 1];
    for (j = 0; j < n; j++)
    {
        if (wj[j] != mark) continue;      /* skip if j is not in C set */
        p[kr++] = imatch[j];
        q[kc++] = j;
    }
    cc[set + 1] = kc;
    rr[set] = kr;
}



SparseMatrix* utils::plus(const SparseMatrix* A, const SparseMatrix* B, double alpha = 1, double beta = 1)
{
    int p, j, nz = 0, anz, * Cp, * Ci, * Bp, m, n, bnz, * w, values;
    double* x, * Bx, * Cx;
    SparseMatrix* C;


    m = A->m; anz = A->p[A->n];
    n = B->n; Bp = B->p; Bx = B->x; bnz = Bp[n];
    w = new int[m];
    values = (A->x != NULL) && (Bx != NULL);
    x = values ? (new double[m]) : NULL;    /* get workspace */
    C = new SparseMatrix(m, n, anz + bnz);// cs_spalloc (m, n, anz + bnz, values, 0) ;           /* allocate result*/
    memset(w, 0, m * sizeof(int));
    memset(x, 0, m * sizeof(double));

    Cp = C->p; Ci = C->i; Cx = C->x;
    for (j = 0; j < n; j++)
    {
        Cp[j] = nz;                   /* column j of C starts here */
        nz = scatter(A, j, alpha, w, x, j + 1, C, nz);   /* alpha*A(:,j)*/
        nz = scatter(B, j, beta, w, x, j + 1, C, nz);    /* beta*B(:,j) */
        if (values) for (p = Cp[j]; p < nz; p++) Cx[p] = x[Ci[p]];
    }
    Cp[n] = nz;                       /* finalize the last column of C */

    delete[] w;
    delete[] x;
    return C;
}

SparseMatrix* utils::permute(const SparseMatrix* A, const int* pinv, const int* q, int values)
{
    int t, j, k, nz = 0, m, n, * Ap, * Ai, * Cp, * Ci;
    double* Cx, * Ax;
    SparseMatrix* C;
    int start, end;
    int off_nnz = 0;
    int off_n = 0;
    if (A->isOff)
    {
        off_nnz = A->off_nnz;
        off_n = A->off_n;
    }

    m = A->m;
    n = A->n;
    Ap = A->p;
    Ai = A->i;
    Ax = A->x;
    C = new SparseMatrix(A->m, A->n, A->nzmax);

    Cp = C->p;
    Ci = C->i;
    Cx = C->x; 

    for (k = 0; k < n; k++) 
    {
        Cp[k] = nz; /* column k of C is column q[k] of A */
        j = q ? (q[k]) : k;
        start = Ap[j] - off_nnz;
        end = Ap[j + 1] - off_nnz;
        for (t = start; t < end; t++) 
		{
            if (Cx)  Cx[nz] = Ax[t];/* row i of A is row pinv[i] of C */
            Ci[nz++] = pinv ? (pinv[Ai[t]]) : Ai[t];
        }
    }
    Cp[n] = nz; /* finalize the last column of C */
    return C;
}

dmPermutation::dmPermutation(int nrow, int ncol)
{
    D    = new dm_perm_res(ncol, nrow);
    Dscc = new dm_perm_res(ncol, 0);
    C_perm = new SparseMatrix(ncol, nrow, nrow * 20);
    C_ones = new SparseMatrix(ncol, nrow, nrow * 20);
    Csum = new SparseMatrix(ncol, nrow, nrow * 40);
    CT = new SparseMatrix(ncol, nrow, nrow * 20);

    jimatch = new int[nrow + ncol];
    q = new int[nrow];
    pinv = new int[nrow];
    xi = new int[2 * nrow + 1];
    P = new int[nrow];
    R = new int[nrow + 1];
    Work  = new int[30 * nrow];
    dWork = new double[30 * nrow];
    comcon = new int[nrow / 3];
    comcon_NNZ = new int[nrow / 3];

}
dmPermutation::~dmPermutation()
{
    delete[] pinv;
    delete[] jimatch;
    //delete[] w;
    delete[] q;
    delete[] xi;
    delete[]P;
    delete[]R;
    delete[]Work;
    delete D;
    delete Dscc;
    delete C_perm;
    delete C_ones;
    delete Csum;
    delete CT;
    delete[] dWork;
}

void dmPermutation::tr(const SparseMatrix* A, int* work)
{
    int p, q, j, * Cp, * Ci, n, m, * Ap, * Ai;
    double* Cx, * Ax;
    SparseMatrix* C;

    m = A->m;
    n = A->n;
    Ap = A->p;
    Ai = A->i;
    Ax = A->x;

    C = CT;
    std::memset(work, 0, sizeof(int) * n);

    Cp = C->p;
    Ci = C->i;
    Cx = C->x;
    for (p = 0; p < Ap[n]; p++) work[Ai[p]]++; /* row counts */
    
    cumsum(Cp, work, m); /* row pointers */
    for (j = 0; j < n; j++)
        for (p = Ap[j]; p < Ap[j + 1]; p++)
        {
            Ci[q = work[Ai[p]]++] = j; /* place A(i,j) as entry C(j,i) */
            if (Cx) Cx[q] = Ax[p];
        }

    return;
}

void dmPermutation::add(const SparseMatrix* A, const SparseMatrix* B, int *Work, double *dWork)
{
    int p, j, nz = 0, anz, * Cp, * Ci, * Bp, m, n, bnz, * w, values;
    double* x, * Bx, * Cx;
    SparseMatrix* C;
    double alpha = 1;
    double beta = 1;

    m = A->m; anz = A->p[A->n];
    n = B->n; Bp = B->p; Bx = B->x; bnz = Bp[n];
    w = Work;//w = new int[m];
    values = (A->x != NULL) && (Bx != NULL);
    x = dWork;

    C = Csum;
    
    memset(w, 0, m * sizeof(int));
    memset(x, 0, m * sizeof(double));

    Cp = C->p; Ci = C->i; Cx = C->x;
    for (j = 0; j < n; j++)
    {
        Cp[j] = nz;                   /* column j of C starts here */
        nz = scatter(A, j, alpha, w, x, j + 1, C, nz);   /* alpha*A(:,j)*/
        nz = scatter(B, j, beta, w, x, j + 1, C, nz);    /* beta*B(:,j) */
        if (values) for (p = Cp[j]; p < nz; p++) Cx[p] = x[Ci[p]];
    }
    Cp[n] = nz;                       /* finalize the last column of C */

    
    return;
}

void dmPermutation::spones(const SparseMatrix* A)
{
    SparseMatrix* res;
    res = C_ones;
    memcpy(res->i, A->i, A->nzmax * sizeof(int));
    memcpy(res->p, A->p, (A->n + 1) * sizeof(int));
    for (int i = 0; i < A->nzmax; ++i) res->x[i] = 1;

    return;
}

void dmPermutation::perm(const SparseMatrix* A, const int* pinv, const int* q)
{
    int t, j, k, nz = 0, m, n, * Ap, * Ai, * Cp, * Ci;
    double* Cx, * Ax;
    SparseMatrix* C;
    
    m = A->m;
    n = A->n;
    Ap = A->p;
    Ai = A->i;
    Ax = A->x;

    C_perm->ZeroInit(m, n, A->nzmax);
    C = C_perm;
    Cp = C->p;
    Ci = C->i;
    Cx = C->x;
    for (k = 0; k < n; k++)
    {
        Cp[k] = nz; /* column k of C is column q[k] of A */
        j = q ? (q[k]) : k;
        for (t = Ap[j]; t < Ap[j + 1]; t++) 
		{
            if (Cx)  Cx[nz] = Ax[t]; /* row i of A is row pinv[i] of C */
            Ci[nz++] = pinv ? (pinv[Ai[t]]) : Ai[t];
        }
    }
    Cp[n] = nz; /* finalize the last column of C */
}

void dmPermutation::strong_con_comp(SparseMatrix* A)     /* matrix A temporarily modified, then restored */
{
    int n, i, k, b, nb = 0, top, * pstack, * p, * r, * Ap, * ATp, * rcopy, * Blk;
    SparseMatrix* AT;
    dm_perm_res* D;

    n = A->n; Ap = A->p;
    Dscc->clear();
    D = Dscc;
    AT = transpose(A, Work);                      /* AT = A' */
    
    Blk = xi; rcopy = pstack = xi + n;
    p = D->p; r = D->r; ATp = AT->p;
    top = n;
    for (i = 0; i < n; i++)   /* first dfs(A) to find finish times (xi) */
    {
        if (!MARKED(Ap, i)) top = dfs(i, A, top, xi, pstack, NULL);
    }
    for (i = 0; i < n; i++) MARK(Ap, i); /* restore A; unmark all nodes*/
    top = n;
    nb = n;
    for (k = 0; k < n; k++)   /* dfs(A') to find strongly connnected comp */
    {
        i = xi[k];            /* get i in reverse order of finish times */
        if (MARKED(ATp, i)) continue;  /* skip node i if already ordered */
        r[nb--] = top;        /* node i is the start of a component in p */
        top = dfs(i, AT, top, p, pstack, NULL);
    }
    r[nb] = 0;                /* first block starts at zero; shift r up */
    for (k = nb; k <= n; k++) r[k - nb] = r[k];
    D->nb = nb = n - nb;         /* nb = # of strongly connected components */
    for (b = 0; b < nb; b++)  /* sort each block in natural order */
    {
        for (k = r[b]; k < r[b + 1]; k++) Blk[p[k]] = b;
    }
    for (b = 0; b <= nb; b++) rcopy[b] = r[b];
    for (i = 0; i < n; i++) p[rcopy[Blk[i]]++] = i;

    delete AT;
    
    return;
}

int dmPermutation::f_keep(SparseMatrix* A, int (*fkeep) (int, int, double, void*), void* other)
{
    int j, p, nz = 0, n, * Ap, * Ai;
    double* Ax;
    if (!fkeep) return (-1);    /* check inputs */
    n = A->n; Ap = A->p; Ai = A->i; Ax = A->x;
    for (j = 0; j < n; j++)
    {
        p = Ap[j];                        /* get current location of col j */
        Ap[j] = nz;                       /* record new location of col j */
        for (; p < Ap[j + 1]; p++)
        {
            if (fkeep(Ai[p], j, Ax ? Ax[p] : 1, other))
            {
                if (Ax) Ax[nz] = Ax[p];  /* keep A(i,j) */
                Ai[nz++] = Ai[p];
            }
        }
    }
    Ap[n] = nz;                           /* finalize A */
    return (nz);
}

void dmPermutation::p_inv(int const* p, int n)
{
    int k;
    if (!p)
        return;                     /* p = NULL denotes identity */
    
    if (!pinv)
        return;                  /* out of memory */
    for (k = 0; k < n; k++) pinv[p[k]] = k;/* invert the permutation */
    return;                             /* return result */
}

void dmPermutation::maxtrans(const SparseMatrix* A, int seed)  /*[jmatch [0..m-1]; imatch [0..n-1]]*/
{
    int i, j, k, n, m, p, n2 = 0, m2 = 0, * Ap, * cheap, * js, * is,
        * ps, * Ai, * Cp, * jmatch, * imatch;
    SparseMatrix* C;

    n = A->n; m = A->m; Ap = A->p; Ai = A->i;
    w = jimatch;// = new int[n + m] ;   /* allocate result */
    if (!jimatch)
        return;
    for (k = 0, j = 0; j < n; j++)    /* count nonempty rows and columns */
    {
        n2 += (Ap[j] < Ap[j + 1]);
        for (p = Ap[j]; p < Ap[j + 1]; p++)
        {
            w[Ai[p]] = 1;
            k += (j == Ai[p]);        /* count entries already on diagonal */
        }
    }
    if (k == MIN(m, n))              /* quick return if diagonal zero-free */
    {
        jmatch = jimatch; imatch = jimatch + m;
        for (i = 0; i < k; i++) jmatch[i] = i;
        for (; i < m; i++) jmatch[i] = -1;
        for (j = 0; j < k; j++) imatch[j] = j;
        for (; j < n; j++) imatch[j] = -1;
        return;
    }
    for (i = 0; i < m; i++) m2 += w[i];
    C = (m2 < n2) ? transpose(A, Work) : ((SparseMatrix*)A); /* transpose if needed */
    if (!C)
    {
        if (m2 < n2)
            delete C;
        return;
    }
    n = C->n; m = C->m; Cp = C->p;
    jmatch = (m2 < n2) ? jimatch + n : jimatch;
    imatch = (m2 < n2) ? jimatch : jimatch + m;
    w = Work;

    if (!w)
    {
        if (m2 < n2)
            delete C;
        return;
    }
    cheap = w + n; js = w + 2 * n; is = w + 3 * n; ps = w + 4 * n;
    for (j = 0; j < n; j++) cheap[j] = Cp[j];  /* for cheap assignment */
    for (j = 0; j < n; j++) w[j] = -1;          /* all columns unflagged */
    for (i = 0; i < m; i++) jmatch[i] = -1;     /* nothing matched yet */


    randperm(n, seed, q);                     /* q = random permutation */
    for (k = 0; k < n; k++)   /* augment, starting at column q[k] */
        augment(q ? q[k] : k, C, jmatch, cheap, w, js, is, ps);

    for (j = 0; j < n; j++) imatch[j] = -1;     /* find row match */
    for (i = 0; i < m; i++) if (jmatch[i] >= 0) imatch[jmatch[i]] = i;

    if (m2 < n2)
        delete C;
    return;

}

void dmPermutation::Run(const SparseMatrix* A, int seed)
{
    int m, n, i, j, k, cnz, nc, * jmatch, * imatch, * wi, * wj, * Cp, * Ci,
        * ps, * rs, nb1, nb2, * p, * q, * cc, * rr, * r, * s, ok;
    SparseMatrix* C;
    dm_perm_res* scc;

    /* --- Maximum matching ------------------------------------------------- */
    m = A->m; n = A->n;
    p = D->p; q = D->q; r = D->r; s = D->s; cc = D->cc; rr = D->rr;
    this->maxtrans(A, seed);            /* max transversal */
    jmatch = this->jimatch;
    imatch = jmatch + m;                       /* imatch = inverse of jmatch */
    if (!jmatch)
        return;

    /* --- Coarse decomposition --------------------------------------------- */
    wi = r; wj = s;                           /* use r and s as workspace */
    for (j = 0; j < n; j++) wj[j] = -1;     /* unmark all cols for bfs */
    for (i = 0; i < m; i++) wi[i] = -1;     /* unmark all rows for bfs */
    bfs(A, n, wi, wj, q, imatch, jmatch, 1, Work);       /* find C1, R1 from C0*/
    ok = bfs(A, m, wj, wi, p, jmatch, imatch, 3, Work);  /* find R3, C3 from R0*/
    if (!ok)
        return;

    unmatched(n, wj, q, cc, 0);                    /* unmatched set C0 */
    matched(n, wj, imatch, p, q, cc, rr, 1, 1);    /* set R1 and C1 */
    matched(n, wj, imatch, p, q, cc, rr, 2, -1);   /* set R2 and C2 */
    matched(n, wj, imatch, p, q, cc, rr, 3, 3);    /* set R3 and C3 */
    unmatched(m, wi, p, rr, 3);                    /* unmatched set R0 */
    
    /* --- Fine decomposition ----------------------------------------------- */
    p_inv(p, m);         /* pinv=p' */
    if (!pinv)
        return;

    perm(A, pinv, q);/* C=A(p,q) (it will hold A(R2,C2)) */

    C = C_perm;
    if (!C)
        return;
    Cp = C->p;
    nc = cc[3] - cc[2];          /* delete cols C0, C1, and C3 from C */
    if (cc[2] > 0) for (j = cc[2]; j <= cc[3]; j++) Cp[j - cc[2]] = Cp[j];
    C->n = nc;
    if (rr[2] - rr[1] < m)        /* delete rows R0, R1, and R3 from C */
    {
        this->f_keep(C, rprune, rr);
        cnz = Cp[nc];
        Ci = C->i;
        if (rr[1] > 0) for (k = 0; k < cnz; k++) Ci[k] -= rr[1];
    }
    C->m = nc;
    this->strong_con_comp(C);              /* find strongly connected components of C*/
    scc = Dscc;
    if (!scc)
    {
        delete C;
        return;
    }
    /* --- Combine coarse and fine decompositions --------------------------- */
    ps = scc->p;                   /* C(ps,ps) is the permuted matrix */
    rs = scc->r;                   /* kth block is rs[k]..rs[k+1]-1 */
    nb1 = scc->nb;                /* # of blocks of A(R2,C2) */
    for (k = 0; k < nc; k++) wj[k] = q[ps[k] + cc[2]];
    for (k = 0; k < nc; k++) q[k + cc[2]] = wj[k];
    for (k = 0; k < nc; k++) wi[k] = p[ps[k] + rr[1]];
    for (k = 0; k < nc; k++) p[k + rr[1]] = wi[k];
    nb2 = 0;                       /* create the fine block partitions */
    r[0] = s[0] = 0;
    if (cc[2] > 0) nb2++;         /* leading coarse block A (R1, [C0 C1]) */
    for (k = 0; k < nb1; k++)     /* coarse block A (R2,C2) */
    {
        r[nb2] = rs[k] + rr[1]; /* A (R2,C2) splits into nb1 fine blocks */
        s[nb2] = rs[k] + cc[2];
        nb2++;
    }
    if (rr[2] < m)
    {
        r[nb2] = rr[2];          /* trailing coarse block A ([R3 R0], C3) */
        s[nb2] = cc[3];
        nb2++;
    }
    r[nb2] = m;
    s[nb2] = n;
    D->nb = nb2;

    memset(pinv, 0, n * sizeof(int));
    p_inv(p, m);
    for (int i = 0; i < n; ++i) q[i] = i;

    perm(A, pinv, q);
    C2 = C_perm;

    spones(C2);
    tr(C_ones, Work);
    add(C_ones, CT, Work, dWork);

    memset(comcon_NNZ, 0, sizeof(int) * (A->n / 3));
    memset(comcon, 0, sizeof(int) * (A->n / 3));   

    sccomp(Csum, Work);
       
    for (int i = 0; i <= N_Blk; ++i) comcon_NNZ[i] = (C2->p[comcon[i]]);


    return;
}

void dmPermutation::sccomp(SparseMatrix* A, int* xi)
{
    int n, i, nb = 0, top, * pstack, * Ap, * rcopy, * Ai;
    int pre_pos = 0;
    N_Blk = 0;    

    n = A->n;
    Ap = A->p;
    Ai = A->i;
    memset(xi, 0, (2 * n + 1) * sizeof(int));
    rcopy = pstack = xi + n;
    comcon[N_Blk] = 0;
    ++N_Blk;
    top = n;
    nb = 0;
    for (i = 0; i < n; ++i)   /* first dfs(A) to find finish times (xi) */
        if (!MARKED(Ap, i))
        {
            if (abs((i + 1) - pre_pos) > BLK_SIZE)
            {
                comcon[N_Blk] = i;
                ++N_Blk;
                pre_pos = i + 1;
            }
            top = dfs(i, A, top, xi, pstack, NULL);
            ++nb;
        }

    for (i = 0; i < n; i++) MARK(Ap, i); /* restore A; unmark all nodes*/
    comcon[N_Blk] = n;
}

Solver::Solver()
{
    mInt = nInt = 0;
    isAdditional = false;
    isZero = true;
}

Solver::Solver(int nrow, int ncol)
{
    mInt = ncol;
    nInt = nrow;
    nnzMaybe = nInt * 20;
    W = new int[nInt];
    isZero = true;
    A_for_DM = new SparseMatrix(nInt, nnzMaybe, nnzMaybe);
    A_for_DM->n = nInt;
}
Solver::~Solver()
{
    if (!isAdditional)
    {
        delete matrA;        
        delete[] W;
        delete A_for_DM;
    }
    if (!isZero)
    {
    }
}


int Solver::LU(NumbFactor* N, const SparseMatrix* A, const SymbFactor* S, double tol, double* x, int* xi)
{
    SparseMatrix* L, * U;
    double pivot, * Lx, * Ux;
    double a, t;
    int* Lp, * Li, * Up, * Ui, * pinv, * q, n, ipiv, k, top, p, i, col, lnz, unz;
    
	n = A->n;
    q = S->q;
    lnz = (int)S->lnz;
    unz = (int)S->unz;

    L = N->L;
    U = N->U;
    pinv = N->pinv;

    Lp = L->p;
    Up = U->p;
    std::memset(x, 0, sizeof(double) * (n));

    for (i = 0; i < n; i++) pinv[i] = -1;
    
    std::memset(Lp, 0, sizeof(int) * (n + 1));
    lnz = unz = 0;

    for (k = 0; k < n; k++)
    { /* compute L(:,k) and U(:,k) */
        /* --- Triangular solve --------------------------------------------- */
        Lp[k] = lnz; /* L(:,k) starts here */
        Up[k] = unz; /* U(:,k) starts here */


        Li = L->i;
        Lx = L->x;
        Ui = U->i;
        Ux = U->x;
        col = q ? (q[k]) : k;
        top = this->spsolve(L, A, col, xi, x, pinv, 1); /* x = L\A(:,col) */
        /* --- Find pivot --------------------------------------------------- */
        ipiv = -1;
        a = -1;
        for (p = top; p < n; p++)
        {
            i = xi[p]; /* x(i) is nonzero */
            if (pinv[i] < 0)
            { /* row i is not yet pivotal */
                if ((t = ABS(x[i])) > a)
                {
                    a = t; /* largest pivot candidate so far */
                    ipiv = i;
                }
            }
            else
            { /* x(i) is the entry U(pinv[i],k) */
                Ui[unz] = pinv[i];
                Ux[unz++] = x[i];
            }
        }

        if (ipiv == -1 || a <= 0)
            return -1; //(cs_ndone(N, NULL, xi, x, 0));

        /* tol=1 for  partial pivoting; tol<1 gives preference to diagonal */
        if (pinv[col] < 0 && ABS(x[col]) >= a * tol)
            ipiv = col;

        /* --- Divide by pivot ---------------------------------------------- */
        pivot = x[ipiv]; /* the chosen pivot */
        Ui[unz] = k;     /* last entry in U(:,k) is U(k,k) */
        Ux[unz++] = pivot;
        pinv[ipiv] = k; /* ipiv is the kth pivot row */
        Li[lnz] = ipiv; /* first entry in L(:,k) is L(k,k) = 1 */
        Lx[lnz++] = (1);
        for (p = top; p < n; p++)
        { /* L(k+1:n,k) = x / pivot */
            i = xi[p];
            if (pinv[i] < 0)
            {                               /* x(i) is an entry in L(:,k) */
                Li[lnz] = i;                /* save unpermuted row in L */
                Lx[lnz++] = (x[i] / pivot); /* scale pivot column */
            }
            x[i] = (0); /* x [0..n-1] = 0 for next k */
        }
    }
    
    /* --- Finalize L and U ------------------------------------------------- */
    Lp[n] = lnz;
    Up[n] = unz;
    Li = L->i; /* fix row indices of L for final pinv */

    for (p = 0; p < lnz; p++)   Li[p] = pinv[Li[p]];
    
    return 0; // N; /* success */
}


int Solver::LU_2(NumbFactor* N, const SparseMatrix* A, const int* q, double tol, double* x, int* xi, int n_row)
{
    SparseMatrix* L, * U;
    double pivot, * Lx, * Ux;
    double a, t;
    int* Lp, * Li, * Up, * Ui, * pinv, n, ipiv, k, top, p, i, col, lnz, unz;
    
    L = N->L;
    n = L->n;
    U = N->U;
    pinv = N->pinv;

    Lp = L->p;
    Up = U->p;
    std::memset(x, 0, sizeof(double) * (n));

    for (i = 0; i < n; i++) pinv[i] = -1;
    
    std::memset(Lp, 0, sizeof(int) * (n + 1));

    lnz = unz = 0;
    for (k = 0; k < n; k++) /* compute L(:,k) and U(:,k) */
    { 
        /* --- Triangular solve --------------------------------------------- */
        Lp[k] = lnz; /* L(:,k) starts here */
        Up[k] = unz; /* U(:,k) starts here */
        Li = L->i;
        Lx = L->x;
        Ui = U->i;
        Ux = U->x;
        col = q ? (q[k]) : k;
        top = this->spsolve(L, A, col, xi, x, pinv, 1, n_row); /* x = L\A(:,col) */
        /* --- Find pivot --------------------------------------------------- */
        ipiv = -1;
        a = -1;
        for (p = top; p < n; p++)
        {
            i = xi[p]; /* x(i) is nonzero */
            if (pinv[i] < 0)
            { /* row i is not yet pivotal */
                if ((t = ABS(x[i])) > a)
                {
                    a = t; /* largest pivot candidate so far */
                    ipiv = i;
                }
            }
            else
            { /* x(i) is the entry U(pinv[i],k) */
                Ui[unz] = pinv[i];
                Ux[unz++] = x[i];
            }
        }
        
        if (ipiv == -1 || a <= 0)
            return -1; //(cs_ndone(N, NULL, xi, x, 0));

        /* tol=1 for  partial pivoting; tol<1 gives preference to diagonal */
        if (pinv[col] < 0 && ABS(x[col]) >= a * tol)
            ipiv = col;

        /* --- Divide by pivot ---------------------------------------------- */
        pivot = x[ipiv]; /* the chosen pivot */
        Ui[unz] = k;     /* last entry in U(:,k) is U(k,k) */
        Ux[unz++] = pivot;
        pinv[ipiv] = k; /* ipiv is the kth pivot row */
        Li[lnz] = ipiv; /* first entry in L(:,k) is L(k,k) = 1 */
        Lx[lnz++] = (1);
        for (p = top; p < n; p++)
        { /* L(k+1:n,k) = x / pivot */
            i = xi[p];
            if (pinv[i] < 0)
            {                               /* x(i) is an entry in L(:,k) */
                Li[lnz] = i;                /* save unpermuted row in L */
                Lx[lnz++] = (x[i] / pivot); /* scale pivot column */
            }
            x[i] = (0); /* x [0..n-1] = 0 for next k */
        }
    }
    
    /* --- Finalize L and U ------------------------------------------------- */
    Lp[n] = lnz;
    Up[n] = unz;
    Li = L->i; /* fix row indices of L for final pinv */

    for (p = 0; p < lnz; p++)   Li[p] = pinv[Li[p]];
    

    return 0; /* success */
}


void Solver::solve_from_LU(NumbFactor* N, SymbFactor* S, double* b, int n, double* x)
{
    this->ipvec(N->pinv, b, x, n); /* x = b(p) */
    this->lsolve(N->L, x);         /* x = L\x */
    this->usolve(N->U, x);         /* x = U\x */
    this->ipvec(S->q, x, b, n);    /* b(q) = x */
}

void Solver::solve_from_LU_2(NumbFactor* N, int* q, double* b, int n, double* x)
{
    
    this->ipvec(N->pinv, b, x, n); /* x = b(p) */
    this->lsolve(N->L, x);         /* x = L\x */
    this->usolve(N->U, x);         /* x = U\x */
    this->ipvec(q, x, b, n);    /* b(q) = x */
}


ReOdering::ReOdering(int N, int M, int NNZ, int* row_ind, int offset)
{
    n_col = M;
    n_row = N;
    Alen = 2 * NNZ + 8 * (M + 1) + 6 * (N + 1) + M;
    A = new int[Alen];
    std::memset(A, 0, Alen * sizeof(int));
    for (int i = 0; i < NNZ; ++i)
        A[i] = row_ind[i] - offset;

    knobs[0] = 10;
    knobs[1] = 10;
    knobs[2] = 1;
    Col_size = ((n_col + 1) * sizeof(Col_struct)) / sizeof(int); /* size of Col array of structs */
    Row_size = ((n_row + 1) * sizeof(Row_struct)) / sizeof(int); /* size of Row array of structs */
    need = 2 * NNZ + n_col + Col_size + Row_size;
    Alen -= (int)(Col_size + Row_size);
    Col = (Col_struct*)&A[Alen];
    Row = (Row_struct*)&A[Alen + Col_size];
}
ReOdering::~ReOdering()
{
    delete[] A;
}
void ReOdering::init_rows_cols(int p[])
{
    /* === Local variables ================================================== */

    int col;      /* a column index */
    int row;      /* a row index */
    int* cp;      /* a column pointer */
    int* cp_end;  /* a pointer to the end of a column */
    int* rp;      /* a row pointer */
    int* rp_end;  /* a pointer to the end of a row */
    int last_row; /* previous row */
    int Status = 0;

    /* === Initialize columns, and check column pointers ==================== */

    for (col = 0; col < n_col; col++)
    {
        Col[col].start = p[col];
        Col[col].length = p[col + 1] - p[col];

        Col[col].shared1.thickness = 1;
        Col[col].shared2.score = 0;
        Col[col].shared3.prev = -1;
        Col[col].shared4.degree_next = -1;
    }

    /* p [0..n_col] no longer needed, used as "head" in subsequent routines */

    /* === Scan columns, compute row degrees, and check row indices ========= */

    //stats[COLAMD_INFO3] = 0; /* number of duplicate or unsorted row indices*/

    for (row = 0; row < n_row; row++)
    {
        Row[row].length = 0;
        Row[row].shared2.mark = -1;
    }

    for (col = 0; col < n_col; col++)
    {
        last_row = -1;

        cp = &A[p[col]];
        cp_end = &A[p[col + 1]];

        while (cp < cp_end)
        {
            row = *cp++;

            /* make sure row indices within range */

            if (row <= last_row || Row[row].shared2.mark == col)
            {
                /* row index are unsorted or repeated (or both), thus col */
                /* is jumbled.  This is a notice, not an error condition. */
                Status = 1;

            }

            if (Row[row].shared2.mark != col)
            {
                Row[row].length++;
            }
            else
            {
                /* this is a repeated entry in the column, */
                /* it will be removed */
                Col[col].length--;
            }

            /* mark the row as having been seen in this column */
            Row[row].shared2.mark = col;

            last_row = row;
        }
    }

    /* === Compute row pointers ============================================= */

    /* row form of the matrix starts directly after the column */
    /* form of matrix in A */
    Row[0].start = p[n_col];
    Row[0].shared1.p = Row[0].start;
    Row[0].shared2.mark = -1;
    for (row = 1; row < n_row; row++)
    {
        Row[row].start = Row[row - 1].start + Row[row - 1].length;
        Row[row].shared1.p = Row[row].start;
        Row[row].shared2.mark = -1;
    }

    /* === Create row form ================================================== */

    if (Status == 1)
    {
        /* if cols jumbled, watch for repeated row indices */
        for (col = 0; col < n_col; col++)
        {
            cp = &A[p[col]];
            cp_end = &A[p[col + 1]];
            while (cp < cp_end)
            {
                row = *cp++;
                if (Row[row].shared2.mark != col)
                {
                    A[(Row[row].shared1.p)++] = col;
                    Row[row].shared2.mark = col;
                }
            }
        }
    }
    else
    {
        /* if cols not jumbled, we don't need the mark (this is faster) */
        for (col = 0; col < n_col; col++)
        {
            cp = &A[p[col]];
            cp_end = &A[p[col + 1]];
            while (cp < cp_end)
            {
                A[(Row[*cp++].shared1.p)++] = col;
            }
        }
    }

    /* === Clear the row marks and set row degrees ========================== */

    for (row = 0; row < n_row; row++)
    {
        Row[row].shared2.mark = 0;
        Row[row].shared1.degree = Row[row].length;
    }

    /* === See if we need to re-create columns ============================== */

    if (Status == 1)
    {
        /* === Compute col pointers ========================================= */

        /* col form of the matrix starts at A [0]. */
        /* Note, we may have a gap between the col form and the row */
        /* form if there were duplicate entries, if so, it will be */
        /* removed upon the first garbage collection */
        Col[0].start = 0;
        p[0] = Col[0].start;
        for (col = 1; col < n_col; col++)
        {
            /* note that the lengths here are for pruned columns, i.e. */
            /* no duplicate row indices will exist for these columns */
            Col[col].start = Col[col - 1].start + Col[col - 1].length;
            p[col] = Col[col].start;
        }

        /* === Re-create col form =========================================== */

        for (row = 0; row < n_row; row++)
        {
            rp = &A[Row[row].start];
            rp_end = rp + Row[row].length;
            while (rp < rp_end)
            {
                A[(p[*rp++])++] = row;
            }
        }
    }

    /* === Done.  Matrix is not (or no longer) jumbled ====================== */
}
void ReOdering::order_children(int p[])
{
    /* === Local variables ================================================== */

    int i;      /* loop counter for all columns */
    int c;      /* column index */
    int parent; /* index of column's parent */
    int order;  /* column's order */

    /* === Order each non-principal column ================================== */

    for (i = 0; i < n_col; i++)
    {
        /* find an un-ordered non-principal column */

        if (!(Col[i].start == -1) && Col[i].shared2.order == -1)
        {
            parent = i;
            /* once found, find its principal parent */
            do
            {
                parent = Col[parent].shared1.parent;
            } while (!(Col[parent].start == -1));

            /* now, order all un-ordered non-principal columns along path */
            /* to this parent.  collapse tree at the same time */
            c = i;
            /* get order of parent */
            order = Col[parent].shared2.order;

            do
            {

                /* order this column */
                Col[c].shared2.order = order++;
                /* collaps tree */
                Col[c].shared1.parent = parent;

                /* get immediate parent of this column */
                c = Col[c].shared1.parent;

                /* continue until we hit an ordered column.  There are */
                /* guarranteed not to be anymore unordered columns */
                /* above an ordered column */
            } while (Col[c].shared2.order == -1);

            /* re-order the super_col parent to largest order for this group */
            Col[parent].shared2.order = order;
        }
    }

    /* === Generate the permutation ========================================= */

    for (c = 0; c < n_col; c++)
    {
        p[Col[c].shared2.order] = c;
    }
}
void ReOdering::detect_super_cols(  /*int A[],*/ int head[], int row_start, int row_length)
{
    /* === Local variables ================================================== */

    int hash;        /* hash value for a column */
    int* rp;         /* pointer to a row */
    int c;           /* a column index */
    int super_c;     /* column index of the column to absorb into */
    int* cp1;        /* column pointer for column super_c */
    int* cp2;        /* column pointer for column c */
    int length;      /* length of column super_c */
    int prev_c;      /* column preceding c in hash bucket */
    int i;           /* loop counter */
    int* rp_end;     /* pointer to the end of the row */
    int col;         /* a column index in the row to check */
    int head_column; /* first column in hash bucket or degree list */
    int first_col;   /* first column in hash bucket */

    /* === Consider each column in the row ================================== */

    rp = &A[row_start];
    rp_end = rp + row_length;
    while (rp < rp_end)
    {
        col = *rp++;
        if (Col[col].start < 0)
        {
            continue;
        }

        /* get hash number for this column */
        hash = Col[col].shared3.hash;

        /* === Get the first column in this hash bucket ===================== */
        head_column = head[hash];
        if (head_column > -1)
        {
            first_col = Col[head_column].shared3.headhash;
        }
        else
        {
            first_col = -(head_column + 2);
        }

        /* === Consider each column in the hash bucket ====================== */
        for (super_c = first_col; super_c != -1;
            super_c = Col[super_c].shared4.hash_next)
        {
            length = Col[super_c].length;

            /* prev_c is the column preceding column c in the hash bucket */
            prev_c = super_c;

            /* === Compare super_c with all columns after it ================ */

            for (c = Col[super_c].shared4.hash_next;
                c != -1; c = Col[c].shared4.hash_next)
            {
                /* not identical if lengths or scores are different */
                if (Col[c].length != length ||
                    Col[c].shared2.score != Col[super_c].shared2.score)
                {
                    prev_c = c;
                    continue;
                }

                /* compare the two columns */
                cp1 = &A[Col[super_c].start];
                cp2 = &A[Col[c].start];

                for (i = 0; i < length; i++)
                {
                    /* the columns are "clean" (no dead rows) */
                    /* row indices will same order for both supercols, */
                    /* no gather scatter nessasary */
                    if (*cp1++ != *cp2++)
                    {
                        break;
                    }
                }

                /* the two columns are different if the for-loop "broke" */
                if (i != length)
                {
                    prev_c = c;
                    continue;
                }

                /* === Got it!  two columns are identical =================== */
                Col[super_c].shared1.thickness += Col[c].shared1.thickness;
                Col[c].shared1.parent = super_c;
                Col[c].start = -2;
                /* order c later, in order_children() */
                Col[c].shared2.order = -1;
                /* remove c from hash bucket */
                Col[prev_c].shared4.hash_next = Col[c].shared4.hash_next;
            }
        }

        /* === Empty this hash bucket ======================================= */

        if (head_column > -1)
        {
            /* corresponding degree list "hash" is not empty */
            Col[head_column].shared3.headhash = -1;
        }
        else
        {
            /* corresponding degree list "hash" is empty */
            head[hash] = -1;
        }
    }
}
int ReOdering::garbage_collection( /*int A[],*/ int* pfree)
{
    /* === Local variables ================================================== */

    int* psrc;  /* source pointer */
    int* pdest; /* destination pointer */
    int j;      /* counter */
    int r;      /* a row index */
    int c;      /* a column index */
    int length; /* length of a row or column */

    /* === Defragment the columns =========================================== */

    pdest = &A[0];
    for (c = 0; c < n_col; c++)
    {
        if (Col[c].start >= 0)
        {
            psrc = &A[Col[c].start];

            /* move and compact the column */
            Col[c].start = (int)(pdest - &A[0]);
            length = Col[c].length;
            for (j = 0; j < length; j++)
            {
                r = *psrc++;
                if (Row[r].shared2.mark >= 0)
                {
                    *pdest++ = r;
                }
            }
            Col[c].length = (int)(pdest - &A[Col[c].start]);
        }
    }

    /* === Prepare to defragment the rows =================================== */

    for (r = 0; r < n_row; r++)
    {
        if ((Row[r].shared2.mark < 0) || (Row[r].length == 0))
        {
            /* This row is already dead, or is of zero length.  Cannot compact
             * a row of zero length, so kill it.  NOTE: in the current version,
             * there are no zero-length live rows.  Kill the row (for the first
             * time, or again) just to be safe. */
            Row[r].shared2.mark = -1;
        }
        else
        {
            /* save first column index in Row [r].shared2.first_column */
            psrc = &A[Row[r].start];
            Row[r].shared2.first_column = *psrc;

            /* flag the start of the row with the one's complement of row */
            *psrc = (-(r)-1);
        }
    }

    /* === Defragment the rows ============================================== */

    psrc = pdest;
    while (psrc < pfree)
    {
        /* find a negative number ... the start of a row */
        if (*psrc++ < 0)
        {
            psrc--;
            /* get the row index */
            r = (-(*psrc) - 1);

            /* restore first column index */
            *psrc = Row[r].shared2.first_column;

            /* move and compact the row */

            Row[r].start = (int)(pdest - &A[0]);
            length = Row[r].length;
            for (j = 0; j < length; j++)
            {
                c = *psrc++;
                if (Col[c].start >= 0)
                {
                    *pdest++ = c;
                }
            }
            Row[r].length = (int)(pdest - &A[Row[r].start]);
        }
    }
    /* ensure we found all the rows */
    /* === Return the new value of pfree ==================================== */

    return ((int)(pdest - &A[0]));
}
int ReOdering::clear_mark(int tag_mark, int max_mark)
{
    int r;
    if (tag_mark <= 0 || tag_mark >= max_mark)
    {
        for (r = 0; r < n_row; r++)
        {
            if (Row[r].shared2.mark >= 0)
                Row[r].shared2.mark = 0;
        }
        tag_mark = 1;
    }
    return (tag_mark);
}
int ReOdering::find_ordering( /*int A[],*/ int head[], int n_col2, int max_deg, int pfree, int aggressive)
{
    /* === Local variables ================================================== */

    int k;                   /* current pivot ordering step */
    int pivot_col;           /* current pivot column */
    int* cp;                 /* a column pointer */
    int* rp;                 /* a row pointer */
    int pivot_row;           /* current pivot row */
    int* new_cp;             /* modified column pointer */
    int* new_rp;             /* modified row pointer */
    int pivot_row_start;     /* pointer to start of pivot row */
    int pivot_row_degree;    /* number of columns in pivot row */
    int pivot_row_length;    /* number of supercolumns in pivot row */
    int pivot_col_score;     /* score of pivot column */
    int needed_memory;       /* free space needed for pivot row */
    int* cp_end;             /* pointer to the end of a column */
    int* rp_end;             /* pointer to the end of a row */
    int row;                 /* a row index */
    int col;                 /* a column index */
    int max_score;           /* maximum possible score */
    int cur_score;           /* score of current column */
    unsigned int hash;       /* hash value for supernode detection */
    int head_column;         /* head of hash bucket */
    int first_col;           /* first column in hash bucket */
    int tag_mark;            /* marker value for mark array */
    int row_mark;            /* Row [row].shared2.mark */
    int set_difference;      /* set difference size of row with pivot row */
    int min_score;           /* smallest column score */
    int col_thickness;       /* "thickness" (no. of columns in a supercol) */
    int max_mark;            /* maximum value of tag_mark */
    int pivot_col_thickness; /* number of columns represented by pivot col */
    int prev_col;            /* Used by Dlist operations. */
    int next_col;            /* Used by Dlist operations. */
    int ngarbage;            /* number of garbage collections performed */
#define INT_MAX       2147483647    /* maximum (signed) int value */
    /* === Initialization and clear mark ==================================== */
    max_mark = INT_MAX - n_col; /* int_MAX defined in <limits.h> */
    tag_mark = this->clear_mark(0, max_mark);
    min_score = 0;
    ngarbage = 0;

    /* === Order the columns ================================================ */

    for (k = 0; k < n_col2; /* 'k' is incremented below */)
    {

        /* === Select pivot column, and order it ============================ */
        /* make sure degree list isn't empty */
        /* get pivot column from head of minimum degree list */
        while (head[min_score] == -1 && min_score < n_col)
        {
            min_score++;
        }
        pivot_col = head[min_score];

        next_col = Col[pivot_col].shared4.degree_next;
        head[min_score] = next_col;
        if (next_col != -1)
        {
            Col[next_col].shared3.prev = -1;
        }

        /* remember score for defrag check */
        pivot_col_score = Col[pivot_col].shared2.score;

        /* the pivot column is the kth column in the pivot order */
        Col[pivot_col].shared2.order = k;

        /* increment order count by column thickness */
        pivot_col_thickness = Col[pivot_col].shared1.thickness;
        k += pivot_col_thickness;

        /* === Garbage_collection, if necessary ============================= */

        needed_memory = __min(pivot_col_score, n_col - k);
        if (pfree + needed_memory >= Alen)
        {
            pfree = this->garbage_collection(&A[pfree]);
            ngarbage++;
            /* after garbage collection we will have enough */

            /* garbage collection has wiped out the Row[].shared2.mark array */
            tag_mark = this->clear_mark(0, max_mark);
        }

        /* === Compute pivot row pattern ==================================== */

        /* get starting location for this new merged row */
        pivot_row_start = pfree;

        /* initialize new row counts to zero */
        pivot_row_degree = 0;

        /* tag pivot column as having been visited so it isn't included */
        /* in merged pivot row */
        Col[pivot_col].shared1.thickness = -pivot_col_thickness;

        /* pivot row is the union of all rows in the pivot column pattern */
        cp = &A[Col[pivot_col].start];
        cp_end = cp + Col[pivot_col].length;
        while (cp < cp_end)
        {
            /* get a row */
            row = *cp++;

            /* skip if row is dead */
            if (Row[row].shared2.mark >= 0)
            {
                rp = &A[Row[row].start];
                rp_end = rp + Row[row].length;
                while (rp < rp_end)
                {
                    /* get a column */
                    col = *rp++;
                    /* add the column, if alive and untagged */
                    col_thickness = Col[col].shared1.thickness;
                    if (col_thickness > 0 && (Col[col].start >= 0))
                    {
                        /* tag column in pivot row */
                        Col[col].shared1.thickness = -col_thickness;
                        /* place column in pivot row */
                        A[pfree++] = col;
                        pivot_row_degree += col_thickness;
                    }
                }
            }
        }

        /* clear tag on pivot column */
        Col[pivot_col].shared1.thickness = pivot_col_thickness;
        max_deg = __max(max_deg, pivot_row_degree);

        /* === Kill all rows used to construct pivot row ==================== */

        /* also kill pivot row, temporarily */
        cp = &A[Col[pivot_col].start];
        cp_end = cp + Col[pivot_col].length;
        while (cp < cp_end)
        {
            /* may be killing an already dead row */
            row = *cp++;
            Row[row].shared2.mark = -1;
        }

        /* === Select a row index to use as the new pivot row =============== */

        pivot_row_length = pfree - pivot_row_start;
        if (pivot_row_length > 0)
        {
            /* pick the "pivot" row arbitrarily (first row in col) */
            pivot_row = A[Col[pivot_col].start];
        }
        else
        {
            /* there is no pivot row, since it is of zero length */
            pivot_row = -1;
        }

        /* === Approximate degree computation =============================== */

        /* Here begins the computation of the approximate degree.  The column */
        /* score is the sum of the pivot row "length", plus the size of the */
        /* set differences of each row in the column minus the pattern of the */
        /* pivot row itself.  The column ("thickness") itself is also */
        /* excluded from the column score (we thus use an approximate */
        /* external degree). */

        /* The time taken by the following code (compute set differences, and */
        /* add them up) is proportional to the size of the data structure */
        /* being scanned - that is, the sum of the sizes of each column in */
        /* the pivot row.  Thus, the amortized time to compute a column score */
        /* is proportional to the size of that column (where size, in this */
        /* context, is the column "length", or the number of row indices */
        /* in that column).  The number of row indices in a column is */
        /* monotonically non-decreasing, from the length of the original */
        /* column on input to colamd. */

        /* === Compute set differences ====================================== */

        /* pivot row is currently dead - it will be revived later. */

        /* for each column in pivot row */
        rp = &A[pivot_row_start];
        rp_end = rp + pivot_row_length;
        while (rp < rp_end)
        {
            col = *rp++;

            /* clear tags used to construct pivot row pattern */
            col_thickness = -Col[col].shared1.thickness;

            Col[col].shared1.thickness = col_thickness;

            /* === Remove column from degree list =========================== */

            cur_score = Col[col].shared2.score;
            prev_col = Col[col].shared3.prev;
            next_col = Col[col].shared4.degree_next;

            if (prev_col == -1)
            {
                head[cur_score] = next_col;
            }
            else
            {
                Col[prev_col].shared4.degree_next = next_col;
            }
            if (next_col != -1)
            {
                Col[next_col].shared3.prev = prev_col;
            }

            /* === Scan the column ========================================== */

            cp = &A[Col[col].start];
            cp_end = cp + Col[col].length;
            while (cp < cp_end)
            {
                /* get a row */
                row = *cp++;
                row_mark = Row[row].shared2.mark;
                /* skip if dead */
                if (row_mark < 0)
                {
                    continue;
                }

                set_difference = row_mark - tag_mark;
                /* check if the row has been seen yet */
                if (set_difference < 0)
                {
                    set_difference = Row[row].shared1.degree;
                }
                /* subtract column thickness from this row's set difference */
                set_difference -= col_thickness;

                /* absorb this row if the set difference becomes zero */
                if (set_difference == 0 && aggressive)
                {
                    Row[row].shared2.mark = -1;
                }
                else
                {
                    /* save the new mark */
                    Row[row].shared2.mark = set_difference + tag_mark;
                }
            }
        }

        /* === Add up set differences for each column ======================= */

        /* for each column in pivot row */
        rp = &A[pivot_row_start];
        rp_end = rp + pivot_row_length;
        while (rp < rp_end)
        {
            /* get a column */
            col = *rp++;

            hash = 0;
            cur_score = 0;
            cp = &A[Col[col].start];
            /* compact the column */
            new_cp = cp;
            cp_end = cp + Col[col].length;

            while (cp < cp_end)
            {
                /* get a row */
                row = *cp++;

                row_mark = Row[row].shared2.mark;
                /* skip if dead */
                if (row_mark < 0)
                {
                    continue;
                }

                /* compact the column */
                *new_cp++ = row;
                /* compute hash function */
                hash += row;
                /* add set difference */
                cur_score += row_mark - tag_mark;
                /* integer overflow... */
                cur_score = __min(cur_score, n_col);
            }

            /* recompute the column's length */
            Col[col].length = (int)(new_cp - &A[Col[col].start]);

            /* === Further mass elimination ================================= */

            if (Col[col].length == 0)
            {
                /* nothing left but the pivot row in this column */
                Col[col].start = -1;
                pivot_row_degree -= Col[col].shared1.thickness;
                /* order it */
                Col[col].shared2.order = k;
                /* increment order count by column thickness */
                k += Col[col].shared1.thickness;
            }
            else
            {
                /* === Prepare for supercolumn detection ==================== */
                /* save score so far */
                Col[col].shared2.score = cur_score;

                /* add column to hash table, for supercolumn detection */
                hash %= n_col + 1;

                head_column = head[hash];
                if (head_column > -1)
                {
                    /* degree list "hash" is non-empty, use prev (shared3) of */
                    /* first column in degree list as head of hash bucket */
                    first_col = Col[head_column].shared3.headhash;
                    Col[head_column].shared3.headhash = col;
                }
                else
                {
                    /* degree list "hash" is empty, use head as hash bucket */
                    first_col = -(head_column + 2);
                    head[hash] = -(col + 2);
                }
                Col[col].shared4.hash_next = first_col;

                /* save hash function in Col [col].shared3.hash */
                Col[col].shared3.hash = (int)hash;
            }
        }

        /* The approximate external column degree is now computed.  */

        /* === Supercolumn detection ======================================== */

        this->detect_super_cols(head, pivot_row_start, pivot_row_length);

        /* === Kill the pivotal column ====================================== */

        Col[pivot_col].start = -1;

        /* === Clear mark =================================================== */

        tag_mark = this->clear_mark(tag_mark + max_deg + 1, max_mark);

        /* === Finalize the new pivot row, and column scores ================ */
        /* for each column in pivot row */
        rp = &A[pivot_row_start];
        /* compact the pivot row */
        new_rp = rp;
        rp_end = rp + pivot_row_length;
        while (rp < rp_end)
        {
            col = *rp++;
            /* skip dead columns */
            if (Col[col].start < 0)
            {
                continue;
            }
            *new_rp++ = col;
            /* add new pivot row to column */
            A[Col[col].start + (Col[col].length++)] = pivot_row;

            /* retrieve score so far and add on pivot row's degree. */
            /* (we wait until here for this in case the pivot */
            /* row's degree was reduced due to mass elimination). */
            cur_score = Col[col].shared2.score + pivot_row_degree;

            /* calculate the max possible score as the number of */
            /* external columns minus the 'k' value minus the */
            /* columns thickness */
            max_score = n_col - k - Col[col].shared1.thickness;

            /* make the score the external degree of the union-of-rows */
            cur_score -= Col[col].shared1.thickness;

            /* make sure score is less or equal than the max score */
            cur_score = __min(cur_score, max_score);

            /* store updated score */
            Col[col].shared2.score = cur_score;

            /* === Place column back in degree list ========================= */
            next_col = head[cur_score];
            Col[col].shared4.degree_next = next_col;
            Col[col].shared3.prev = -1;
            if (next_col != -1)
            {
                Col[next_col].shared3.prev = col;
            }
            head[cur_score] = col;

            /* see if this score is less than current min */
            min_score = __min(min_score, cur_score);
        }

        /* === Resurrect the new pivot row ================================== */

        if (pivot_row_degree > 0)
        {
            /* update pivot row length to reflect any cols that were killed */
            /* during super-col detection and mass elimination */
            Row[pivot_row].start = pivot_row_start;
            Row[pivot_row].length = (int)(new_rp - &A[pivot_row_start]);
            Row[pivot_row].shared1.degree = pivot_row_degree;
            Row[pivot_row].shared2.mark = 0;
            /* pivot row is no longer dead */
        }
    }

    /* === All principal columns have now been ordered ====================== */

    return (ngarbage);
}
void ReOdering::init_scoring( /*int A[],*/ int head[], int* p_n_row2, int* p_n_col2, int* p_max_deg)
{
    int c;               /* a column index */
    int r, row;          /* a row index */
    int* cp;             /* a column pointer */
    int deg;             /* degree of a row or column */
    int* cp_end;         /* a pointer to the end of a column */
    int* new_cp;         /* new column pointer */
    int col_length;      /* length of pruned column */
    int score;           /* current column score */
    int n_col2;          /* number of non-dense, non-empty columns */
    int n_row2;          /* number of non-dense, non-empty rows */
    int dense_row_count; /* remove rows with more entries than this */
    int dense_col_count; /* remove cols with more entries than this */
    int min_score;       /* smallest column score */
    int max_deg;         /* maximum row degree */
    int next_col;        /* Used to add to degree list.*/

    /* === Extract knobs ==================================================== */

    /* Note: if knobs contains a NaN, this is undefined: */

    dense_row_count = (int)(__max(16.0, 10 * sqrt((double)n_col)));

    dense_col_count = (int)(__max(16.0, 10 * sqrt((double)n_col)));

    max_deg = 0;
    n_col2 = n_col;
    n_row2 = n_row;

    /* === Kill empty columns =============================================== */

    /* Put the empty columns at the end in their natural order, so that LU */
    /* factorization can proceed as far as possible. */
    for (c = n_col - 1; c >= 0; c--)
    {
        deg = Col[c].length;
        if (deg == 0)
        {
            /* this is a empty column, kill and order it last */
            Col[c].shared2.order = --n_col2;
            Col[c].start = -1;
        }
    }

    /* === Kill dense columns =============================================== */

    /* Put the dense columns at the end, in their natural order */
    for (c = n_col - 1; c >= 0; c--)
    {
        /* skip any dead columns */
        if (Col[c].start < 0)
        {
            continue;
        }
        deg = Col[c].length;
        if (deg > dense_col_count)
        {
            /* this is a dense column, kill and order it last */
            Col[c].shared2.order = --n_col2;
            /* decrement the row degrees */
            cp = &A[Col[c].start];
            cp_end = cp + Col[c].length;
            while (cp < cp_end)
            {
                Row[*cp++].shared1.degree--;
            }
            Col[c].start = -1;
        }
    }

    /* === Kill dense and empty rows ======================================== */

    for (r = 0; r < n_row; r++)
    {
        deg = Row[r].shared1.degree;
        if (deg > dense_row_count || deg == 0)
        {
            /* kill a dense or empty row */
            Row[r].shared2.mark = -1;
            --n_row2;
        }
        else
        {
            /* keep track of max degree of remaining rows */
            max_deg = __max(max_deg, deg);
        }
    }

    /* === Compute initial column scores ==================================== */

    /* At this point the row degrees are accurate.  They reflect the number */
    /* of "live" (non-dense) columns in each row.  No empty rows exist. */
    /* Some "live" columns may contain only dead rows, however.  These are */
    /* pruned in the code below. */

    /* now find the initial matlab score for each column */
    for (c = n_col - 1; c >= 0; c--)
    {
        /* skip dead column */
        if (Col[c].start < 0)
        {
            continue;
        }
        score = 0;
        cp = &A[Col[c].start];
        new_cp = cp;
        cp_end = cp + Col[c].length;
        while (cp < cp_end)
        {
            /* get a row */
            row = *cp++;
            /* skip if dead */
            if ((Row[row].shared2.mark < 0))
            {
                continue;
            }
            /* compact the column */
            *new_cp++ = row;
            /* add row's external degree */
            score += Row[row].shared1.degree - 1;
            /* guard against integer overflow */
            score = __min(score, n_col);
        }
        /* determine pruned column length */
        col_length = (int)(new_cp - &A[Col[c].start]);
        if (col_length == 0)
        {
            /* a newly-made null column (all rows in this col are "dense" */
            /* and have already been killed) */

            Col[c].shared2.order = --n_col2;
            Col[c].start = -1;
        }
        else
        {
            /* set column length and set score */

            Col[c].length = col_length;
            Col[c].shared2.score = score;
        }
    }

    /* At this point, all empty rows and columns are dead.  All live columns */
    /* are "clean" (containing no dead rows) and simplicial (no supercolumns */
    /* yet).  Rows may contain dead columns, but all live rows contain at */
    /* least one live column. */

    /* === Initialize degree lists ========================================== */

    /* clear the hash buckets */
    for (c = 0; c <= n_col; c++)
    {
        head[c] = -1;
    }
    min_score = n_col;
    /* place in reverse order, so low column indices are at the front */
    /* of the lists.  This is to encourage natural tie-breaking */
    for (c = n_col - 1; c >= 0; c--)
    {
        /* only add principal columns to degree lists */
        if (Col[c].start >= 0)
        {
            /* === Add columns score to DList =============================== */

            score = Col[c].shared2.score;

            /* now add this column to dList at proper score location */
            next_col = head[score];
            Col[c].shared3.prev = -1;
            Col[c].shared4.degree_next = next_col;

            /* if there already was a column with the same score, set its */
            /* previous pointer to this new column */
            if (next_col != -1)
            {
                Col[next_col].shared3.prev = c;
            }
            head[score] = c;

            /* see if this score is less than current min */
            min_score = __min(min_score, score);
        }
    }
    /* === Return number of remaining columns, and max row degree =========== */

    *p_n_col2 = n_col2;
    *p_n_row2 = n_row2;
    *p_max_deg = max_deg;
}
void ReOdering::Run(int* p)
{
    int nnz;                    /* nonzeros in A */

    /* minimum required length of A */

    int n_col2;                 /* number of non-dense, non-empty columns */
    int n_row2;                 /* number of non-dense, non-empty rows */
    int ngarbage;               /* number of garbage collections performed */
    int max_deg;                /* maximum row degree */
    int aggressive;             /* do aggressive absorption */
    int ok;


    aggressive = (knobs[2] != 0);
    nnz = p[n_col];
    ok = 1;

    /* === Initialize scores, kill dense rows/columns ======================= */

    this->init_rows_cols(p);
    this->init_scoring(p, &n_row2, &n_col2, &max_deg);

    /* === Order the supercolumns =========================================== */

    ngarbage = this->find_ordering(p, n_col2, max_deg, 2 * nnz, aggressive);

    /* === Order the non-principal columns ================================== */

    this->order_children(p);
}
void ReOdering::PrintNewOrder(int p[])
{
    std::cout << "\n\n\n";
    for (int i = 0; i < n_row; ++i)
        std::cout << p[i] << std::endl;
}


QRsolver::QRsolver(int N, int M, int* Q, SparseMatrix* A) : Solver(N, M)
{
    nInt = N;
    mInt = M;
    w = new int[5 * nInt + mInt + 1];
    isAdditional = true;
    isLU = true;
    isQR = false;
    S = new SymbFactor(N * M, true);
    S->Set_q(Q, nInt);
    pst = new int[nInt];
    

}

QRsolver::QRsolver(int N, int M, SparseMatrix* A) : Solver(0, 0)
{
    nInt = N;
    mInt = M;
    w = new int[5 * nInt + mInt + 1];
    isAdditional = true;
    isLU = true;
    isQR = false;
    isS = false;
    pst = new int[nInt];
    
}

QRsolver::QRsolver(int N, int M) : Solver(N, M)
{
    nInt = N;
    mInt = M;
    w = new int[5 * nInt + mInt + 1];
    NNZ = 0;
    isAdditional = false;
    isLU = false;
    isQR = true;
    pst = new int[nInt];
    
}
QRsolver::~QRsolver()
{
    delete[] w;
    delete S;
    delete C;
    delete N;
    
    delete[] pst;
}

void QRsolver::etree(const SparseMatrix* A, int ata)
{
    int i, k, p, m, n, inext, * Ap, * Ai, * parent, * ancestor, * prev;
    int start, end;
    int off_nnz = 0;
    int off_n = 0;
    if (A->isOff)
    {
        off_nnz = A->off_nnz;
        off_n = A->off_n;
    }

    m = A->m;
    n = A->n;
    Ap = A->p;
    Ai = A->i;
    parent = this->S->parent = new int[n];            /* allocate result */

    std::memset(this->w, 0, sizeof(int) * (n + m));
    ancestor = w;
    prev = w + n;
    if (ata)
        for (i = 0; i < m; i++) 
            prev[i] = -1;

    for (k = 0; k < n; k++) 
    {
        parent[k] = -1;   /* node k has no parent yet */
        ancestor[k] = -1; /* nor does k have an ancestor */
        start = Ap[k] - off_nnz;
        end = Ap[k + 1] - off_nnz;
        for (p = start; p < end; p++) 
		{
            i = ata ? (prev[Ai[p]]) : (Ai[p]);
            for (; i != -1 && i < k; i = inext) /* traverse from i to k */
			{ 
                inext = ancestor[i];              /* inext = ancestor of i */
                ancestor[i] = k;                  /* path compression */
                if (inext == -1)				  /* no anc., parent is k */
                    parent[i] = k;

            }
            if (ata) 
                prev[Ai[p]] = k;

        }
    }

}

void QRsolver::post(int n)
{
    int j, k = 0, * head, * next, * stack;
    std::memset(this->w, 0, sizeof(int) * 3 * n);

    head = this->w;
    next = this->w + n;
    stack = this->w + 2 * n;
    for (j = 0; j < n; j++) /* empty linked lists */
        head[j] = -1;

    for (j = n - 1; j >= 0; j--) /* traverse nodes in reverse order*/
	{
        if (this->S->parent[j] == -1)  /* j is a root */
            continue;

        next[j] = head[this->S->parent[j]]; /* add j to list of its parent */
        head[this->S->parent[j]] = j;
    }
    for (j = 0; j < n; j++)
    {
        if (this->S->parent[j] != -1)/* skip j if it is not a root */
            continue;
        k = tdfs(j, k, head, next, pst, stack);
    }
}

int QRsolver::vcount(const SparseMatrix* A)
{
    int i, k, p, pa, n = A->n, m = A->m, * Ap = A->p, * Ai = A->i, * next, * head, * tail, * nque,
        * pinv, * leftmost, * parent = S->parent;
    S->pinv = pinv = new int[m + n];     /* allocate pinv, */
    S->leftmost = leftmost = new int[m];

    std::memset(this->w, 0, sizeof(int) * (m + 3 * n));
    next = w;
    head = w + m;
    tail = w + m + n;
    nque = w + m + 2 * n;
    for (k = 0; k < n; k++) 
        head[k] = -1;

    for (k = 0; k < n; k++) /* queue k is empty */
        tail[k] = -1;

    memset(nque, 0, n * sizeof(int));
	
    for (i = 0; i < m; i++) 
        leftmost[i] = -1;

    for (k = n - 1; k >= 0; k--) 
        for (p = Ap[k]; p < Ap[k + 1]; p++) 
            leftmost[Ai[p]] = k; /* leftmost[i] = min(find(A(i,:)))*/

    for (i = m - 1; i >= 0; i--)   /* scan rows in reverse order */
	{ 
        pinv[i] = -1;              /* row i is not yet ordered */
        k = leftmost[i];
        if (k == -1) /* row i is empty */
            continue;

        if (nque[k]++ == 0) /* first row in queue k */
            tail[k] = i;

        next[i] = head[k]; /* put i at head of queue k */
        head[k] = i;
    }
    S->lnz = 0;
    S->m2 = m;
    for (k = 0; k < n; k++) 
	{						  /* find row permutation and nnz(V)*/
        i = head[k];          /* remove row i from queue k */
        S->lnz++;             /* count V(k,k) as nonzero */
        if (i < 0) /* add a fictitious row */
            i = S->m2++;

        pinv[i] = k;		/* associate row i with V(:,k) */
        if (--nque[k] <= 0)  /* skip if V(k+1:m,k) is empty */
            continue;
                
        S->lnz += nque[k];            /* nque [k] is nnz (V(k+1:m,k)) */
        if ((pa = parent[k]) != -1) /* move all rows to parent of k */
		{ 
            if (nque[pa] == 0) 
                tail[pa] = tail[k];

            next[tail[k]] = head[pa];
            head[pa] = next[i];
            nque[pa] += nque[k];
        }
    }
    for (i = 0; i < m; i++)
        if (pinv[i] < 0) 
            pinv[i] = k++;


    return (1);
}




#define HEAD(k, j) (ata ? head[k] : j)
#define NEXT(J) (ata ? next[J] : -1)
void QRsolver::init_ata(SparseMatrix* AT, const int* post, int* w, int** head, int** next)
{
    int i, k, p, m = AT->n, n = AT->m, * ATp = AT->p, * ATi = AT->i;
    *head = w + 4 * n, * next = w + 5 * n + 1;
    for (k = 0; k < n; k++) /* invert post */
        w[post[k]] = k;

    for (i = 0; i < m; i++) 
	{
        for (k = n, p = ATp[i]; p < ATp[i + 1]; p++) 		
            k = MIN(k, w[ATi[p]]);

        (*next)[i] = (*head)[k]; /* place row i in linked list k */
        (*head)[k] = i;
    }
}
void QRsolver::counts(const SparseMatrix* A, const int* parent, const int* post, int ata)
{
    int i, j, k, n, m, J, s, p, q, jleaf, * ATp, * ATi, * maxfirst, * prevleaf, * ancestor,
        * head = NULL, * next = NULL, * first, * delta;
    SparseMatrix* AT;

    m = A->m;
    n = A->n;
    s = 4 * n + (ata ? (n + m + 1) : 0);
    delta = this->S->cp = new int[n];

    AT = transpose(A, this->w);                         /* AT = A' */
    std::memset(this->w, 0, sizeof(int) * (s));

    ancestor = w;
    maxfirst = w + n;
    prevleaf = w + 2 * n;
    first = w + 3 * n;
    for (k = 0; k < s; k++)/* clear workspace w [0..s-1] */ 
        w[k] = -1;

    for (k = 0; k < n; k++) /* find first [j] */
	{ 
        j = post[k];
        delta[j] = (first[j] == -1) ? 1 : 0; /* delta[j]=1 if j is a leaf */
        for (; j != -1 && first[j] == -1; j = parent[j]) {
            first[j] = k;
        }
    }
    ATp = AT->p;
    ATi = AT->i;
    if (ata) 
        init_ata(AT, post, w, &head, &next);

    for (i = 0; i < n; i++) /* each node in its own set */
        ancestor[i] = i;

    for (k = 0; k < n; k++) 
	{
        j = post[k]; /* j is the kth node in postordered etree */
        if (parent[j] != -1) 
            delta[parent[j]]--;		/* j is not a root */
                                                    
        for (J = HEAD(k, j); J != -1; J = NEXT(J))  /* J=j for LL'=A case */
            for (p = ATp[J]; p < ATp[J + 1]; p++) 
			{
                i = ATi[p];
                q = leaf(i, j, first, maxfirst, prevleaf, ancestor, &jleaf);
                if (jleaf >= 1) /* A(i,j) is in skeleton */
                    delta[j]++;

                if (jleaf == 2) /* account for overlap in q */
                    delta[q]--;
            }

        if (parent[j] != -1) 
            ancestor[j] = parent[j];
        
    }
    for (j = 0; j < n; j++) /* sum up delta's of each child */
		if (parent[j] != -1) 
            this->S->cp[parent[j]] += this->S->cp[j];

    delete AT;
    
}
void QRsolver::sqr(int order, const SparseMatrix* A)
{
    int n, k, ok = 1;

    n = A->n;
    C = this->permute(A, NULL, S->q, 0);
    this->etree(C, 1); /* etree of C'*C, where C=A(:,q) */
    this->post(n);

    this->counts(C, S->parent, pst, 1);//S->cp = cs_counts(C, S->parent, pst, 1); /* col counts chol(C'*C) */
    ok = C && S->parent && S->cp && vcount(C);
    if (ok)
        for (S->unz = 0, k = 0; k < n; k++)
            S->unz += S->cp[k];
}


void QRsolver::qr(const SparseMatrix* A)
{
    double* Rx, * Vx, * Ax, * x;
    double* Beta;
    int i, k, p, n, vnz, p1, top, m2, len, col, rnz, * s, * leftmost, * Ap, * Ai, * parent, * Rp,
        * Ri, * Vp, * Vi, * pinv, * q;
    SparseMatrix* R, * V;
    int start, end;
    int off_nnz = 0;
    int off_n = 0;
    if (A->isOff)
    {
        off_nnz = A->off_nnz;
        off_n = A->off_n;
    }
    n = A->n;
    Ap = A->p;
    Ai = A->i;
    Ax = A->x;
    q = S->q;
    parent = S->parent;
    pinv = S->pinv;
    m2 = S->m2;
    vnz = (int)S->lnz;
    rnz = (int)S->unz;
    leftmost = (int*)S->leftmost;
    
    std::memset(this->w, 0, sizeof(int) * (m2 + n));
    x = new double[m2];

    s = w + m2; /* s is size n */
    for (k = 0; k < m2; k++) {
        x[k] = 0;
    }
    N = new NumbFactor(m2, n, vnz, rnz, true);                                           /* clear workspace x */
    V = N->L;
    R = N->U;
    Beta = N->B;

    Rp = R->p;
    Ri = R->i;
    Rx = R->x;
    Vp = V->p;
    Vi = V->i;
    Vx = V->x;
    for (i = 0; i < m2; i++) {
        w[i] = -1;
    } /* clear w, to mark nodes */
    rnz = 0;
    vnz = 0;
    for (k = 0; k < n; k++) { /* compute V and R */
        Rp[k] = rnz;          /* R(:,k) starts here */
        Vp[k] = p1 = vnz;     /* V(:,k) starts here */
        w[k] = k;             /* add V(k,k) to pattern of V */
        Vi[vnz++] = k;
        top = n;
        col = q ? q[k] : k;
        start = Ap[col] - off_nnz;
        end = Ap[col + 1] - off_nnz;
        for (p = start; p < end; p++) {     /* find R(:,k) pattern */
            i = leftmost[Ai[p]];                      /* i = min(find(A(i,q))) */
            for (len = 0; w[i] != k; i = parent[i]) { /* traverse up to k */
                s[len++] = i;
                w[i] = k;
            }
            while (len > 0) {
                s[--top] = s[--len];
            }                        /* push path on stack */
            i = pinv[Ai[p]];         /* i = permuted row of A(:,col) */
            x[i] = Ax[p];            /* x (i) = A(:,col) */
            if (i > k && w[i] < k) { /* pattern of V(:,k) = x (k+1:m) */
                Vi[vnz++] = i;       /* add i to pattern of V(:,k) */
                w[i] = k;
            }
        }
        for (p = top; p < n; p++)
        {      /* for each i in pattern of R(:,k) */
            i = s[p];                    /* R(i,k) is nonzero */
            happly(V, i, Beta[i], x); /* apply (V(i),Beta(i)) to x */
            Ri[rnz] = i;                 /* R(i,k) = x(i) */
            Rx[rnz++] = x[i];
            x[i] = 0;
            if (parent[i] == k) {
                vnz = scatter(V, i, 0, w, NULL, k, V, vnz);
            }
        }
        for (p = p1; p < vnz; p++) { /* gather V(:,k) = x */
            Vx[p] = x[Vi[p]];
            x[Vi[p]] = 0;
        }
        Ri[rnz] = k;                                       /* R(k,k) = norm (x) */
        Rx[rnz++] = house(Vx + p1, Beta + k, vnz - p1); /* [v,beta]=house(x) */
    }
    Rp[n] = rnz;                         /* finalize R */
    Vp[n] = vnz;                         /* finalize V */

    delete[] x;

}



void QRsolver::solve_from_qr(double* b, int m, int n)
{
    double* x = new double[S->m2];
    //std::memset(x, 0, S->m2 * sizeof(double));
    if (m >= n)
    {
        ipvec(S->pinv, b, x, m); /* x(0:m-1) = b(p(0:m-1) */
        int k;
        for (k = 0; k < n; k++) /* apply Householder refl. to x */
        {
            happly(N->L, k, N->B[k], x);
        }
        usolve(N->U, x);      /* x = R\x */
        ipvec(S->q, x, b, n); /* b(q(0:n-1)) = x(0:n-1) */

    }
    // else 
    // {
    //     pvec(S->q, b, x, m); /* x(q(0:m-1)) = b(0:m-1) */
    //     utsolve(N->U, x);    /* x = R'\x */
    //     int k;
    //     for (k = m - 1; k >= 0; k--) /* apply Householder refl. to x */
    //     {
    //         happly(N->L, k, N->B[k], x);
    //     }
    //     pvec(S->pinv, x, b, n); /* b(0:n-1) = x(p(0:n-1)) */
    // }
    delete[] x;
}

void QRsolver::Run_from_lu(SparseMatrix* A, double* b)
{
    this->sqr(2, A);
    this->qr(A);
 
    this->solve_from_qr(b, nInt, mInt);

    for (int i = 0; i < nInt; ++i)
    {
        if (isnan(b[i]) || isinf(b[i]))
            b[i] = 0;
    }

}

void QRsolver::solve(int* Q, SparseMatrix* A, double* b, int off_n, int off_nnz)
{
    if (!isS)
    {
        S = new SymbFactor(nInt * mInt, true);
        isS = true;
    }
    S->Set_q(Q, nInt);
    this->sqr(2, A);
    this->qr(A);
    
    this->solve_from_qr(b, nInt, mInt);

    for (int i = 0; i < nInt; ++i)
        if (isnan(b[i]) || isinf(b[i]))
            b[i] = 0;


}

void QRsolver::solve_2(int* Q, SparseMatrix* A, double* b, SymbFactor* S_extern)
{   
	int n = A->n;
	int m = A->m;
    S = S_extern;
    S->Set_q(Q, nInt);
    this->sqr(2, A);
    this->qr(A);
   

    this->solve_from_qr(b, n, m);


    for (int i = 0; i < n; ++i)
    {
        if (isnan(b[i]) || isinf(b[i]))
            b[i] = 0;
    }
    delete this->N;
    delete this->C;
}



void QRsolver::Run(double* b)
{
    this->sqr(2, this->matrA);
    this->qr(this->matrA);


    this->solve_from_qr(b, nInt, mInt);

    for (int i = 0; i < nInt; ++i)
        if (isnan(b[i]) || isinf(b[i]))
            b[i] = 0;
}

void Solver::ZeroStep(int nnzv)
{
    NNZ = nnzv;
    matrA->ZeroInit(mInt, nInt, NNZ);
    is_A = true;

    if (isLU)
    {
        int snz = 10 * NNZ + nInt;
        S->unz = snz;
        S->lnz = snz;
        N->ZeroInit(mInt, nInt, snz, snz);
    }

    if (isQR)
        S = new SymbFactor(mInt * mInt, true);
}


void Solver::fill_A(const int Ti[], const int Tj[], const double Tx[])
{
    int  status = compress(NNZ, Ti, Tj, Tx, this->matrA);
    if (is_A)
    {
        for (int j = 0; j <= nInt; ++j) S->q[j] = matrA->p[j];
        ReO = new ReOdering(nInt, mInt, NNZ, matrA->i);
        ReO->Run(S->q);
        is_A = false;
        delete ReO;
    }
}


void utils::ipv_rows(const int* p, const int* Ai, int* Ai_new, int n)
{
    for (int k = 0; k < n; k++)
        Ai_new[k] = p ? p[Ai[k]] : Ai[k];
}


void Solver::clearOffsets(int n)
{
    if (isZero)
    {
        isZero = false;
        return;
    }

    for (int K = 0; K < n; ++K)
    {
        delete Nt[K];
        delete At[K];
    }

}

void Solver::fill_A_2(const int Ti[], const int Tj[], const double Tx[])
{
	int  status = compress(this->NNZ, Ti, Tj, Tx, this->A_for_DM);

	clearOffsets(this->DM->N_Blk);
	this->A_for_DM->nzmax = this->NNZ;
	this->DM->Run(this->A_for_DM);
	this->N->SetOffset(this->DM);

	ipv_rows(this->DM->pinv, Ti, Ti_n, this->NNZ);
	status = compress(this->NNZ, Ti_n, Tj, Tx, this->matrA);

	for (int K = 0; K < DM->N_Blk; ++K)
	{
		int k1 = DM->comcon[K];
		int k2 = DM->comcon[K + 1];
		int nk = k2 - k1;
		int n_row = nk;
		int off_n = DM->comcon[K];       // k1;
		int off_nnz = DM->comcon_NNZ[K]; // DM->comcon_NNZ[K];
		int NNZ = DM->comcon_NNZ[K + 1] - DM->comcon_NNZ[K];
		int* q, * p;
		q = colOrder + off_n + K;
		int off_setN[3] = { off_n + K, N->snz[K], off_n };
		int off_setA[2] = { off_n, off_nnz };
		Nt[K] = offsetN(N, off_setN);
		At[K] = offsetA(matrA, off_setA);
		Nt[K]->L->n = n_row;
		Nt[K]->U->n = n_row;
		ReOdering ReO(n_row, n_row, NNZ, matrA->i + off_nnz, off_n);
		p = matrA->p + off_n;
		for (int j = 0; j <= n_row; ++j) q[j] = p[j] - off_nnz;
		ReO.Run(q);
	}
	is_A = false;
}

int Solver::compress(int NNZ, const int Ti[], const int Tj[], const double Tx[], SparseMatrix* matrA)
{
    int m, n, nz, p, k, * Cp, * Ci, * w;
    double* Cx;
    SparseMatrix* C;
    C = matrA;
    
    m = matrA->m;
    n = matrA->n;
    C->nzmax = NNZ;
    nz = NNZ;
    w = W;
    std::memset(W, 0, sizeof(int) * (n));
	std::memset(compressPosition, 0, sizeof(int) * NNZ);
    
    Cp = C->p; 
    Ci = C->i; 
    Cx = C->x;
    for (k = 0; k < nz; k++) w[Tj[k]]++;           /* column counts */
    cumsum(Cp, w, n);                              /* column pointers */
    for (k = 0; k < nz; k++)
    {
        Ci[p = w[Tj[k]]++] = Ti[k];    /* A(i,j) is the pth entry in C */
        if (Cx) Cx[p] = Tx[k];
		compressPosition[k] = p;
    }
    return 0;
}

void Solver::updateValues(const double Tx[], SparseMatrix* matrA)
{
	int nz, k, * Cp, * Ci;
	double* Cx;
	SparseMatrix* C;
	C = matrA;
	Cp = C->p; 
	Ci = C->i; 
	Cx = C->x;
	C->nzmax = NNZ;
	nz = NNZ;
	for (k = 0; k < nz; k++)
		Cx[compressPosition[k]] = Tx[k];
	
}



LUsolver::LUsolver(int Nrow) : Solver(Nrow, Nrow)
{
    if (Nrow < 2)
        Nrow = 2;
    
    b_data = new double[Nrow];
    mInt = Nrow;
    nInt = Nrow;
    NNZ = 0;

    x_tmp = new double[5 * Nrow];
    x = new double[10 * Nrow];
    xi = new int[20 * Nrow];
    std::memset(x, 0, 10 * Nrow * sizeof(double));
    std::memset(xi, 0, 20 * Nrow * sizeof(int));
    b_perm = new double[nInt];
    colOrder = new int[10 * nInt];
    DM = new dmPermutation(nInt, nInt);

    matrA = new SparseMatrix(nInt, mInt, nnzMaybe);
	compressPosition = new int[nnzMaybe];
    S = new SymbFactor(nnzMaybe);
    int snz = 10 * nnzMaybe + nInt;
    N = new NumbFactor(mInt, nInt, snz, snz);
    Ti_n = new int[snz];

 
    is_A = true;
    isLU = true;
    isQR = false;
    QR = new QRsolver(nInt, nInt, this->matrA);

    Nt.resize(nInt / 3); 
    At.resize(nInt / 3); 
};

LUsolver::LUsolver(int Nrow, int nnzv, const int Ti[], const int Tj[], const double Tx[]) : Solver(Nrow, Nrow)
{
    b_data = new double[Nrow];
    mInt = Nrow;
    nInt = Nrow;
    NNZ = 0;

    x_tmp = new double[Nrow];
    x = new double[Nrow];
    xi = new int[2 * Nrow];
    
	memset(x, 0, Nrow * sizeof(double));
	memset(x_tmp, 0, Nrow * sizeof(double));

    is_A = true;
    isLU = true;
    isQR = false;
    this->ZeroStep(nnzv);
    this->fill_A(Ti, Tj, Tx);
}


LUsolver::~LUsolver()
{
    delete[] b_data;
    delete[] x_tmp;
    delete[] x;
    delete[] xi;
    delete[] b_perm;
    delete[] colOrder;
    delete[] Ti_n;
	delete[] compressPosition;
    delete S;
    delete N;
}


SparseMatrix* offsetA(SparseMatrix* A, int* off)
{
    SparseMatrix* res;
    int off_n = off[0];
    int off_nnz = off[1];
    res = new SparseMatrix(off_n, off_nnz);
    res->p = A->p + off_n;
    res->i = A->i + off_nnz;
    res->x = A->x + off_nnz;
    return res;
}

NumbFactor* offsetN(NumbFactor* N, int* off)
{
    NumbFactor* res;
    res = new NumbFactor();
    int off_nK = off[0];
    int off_nnz = off[1];
    int off_n = off[2];
    res->L = offsetA(N->L, off);
    res->U = offsetA(N->U, off);
    res->pinv = N->pinv + off_n;
    return res;
}

#include <omp.h>
void LUsolver::Run_blk(double* b_tmp, double *Xres)
{

#pragma omp single
    {
        ipvec(this->DM->pinv, b_tmp, Xres, nInt);
    }

#pragma omp for  nowait
    for (int K = 0; K < DM->N_Blk; ++K)    
    {
        int k1 = DM->comcon[K];
        int k2 = DM->comcon[K + 1];
        int nk = k2 - k1;
        

        int off_n = DM->comcon[K];
        int off_nnz = DM->comcon_NNZ[K];
        

        double* x_tm;
        int* xi_tm, *q, k;
        double* res = Xres + off_n;
        NumbFactor* Nk = Nt[K];

        q = colOrder + off_n + K;
        x_tm = x + off_n;
        xi_tm = xi + 2 * (off_n);

        if (off_n)
        {
            int* Ati;
            Ati = At[K]->i;
            int NNZ = DM->comcon_NNZ[K + 1] - DM->comcon_NNZ[K]; //DM->nz_b[K];
            for (k = NNZ; k--; ) Ati[k] = Ati[k] - off_n;
        }
        int status = this->LU_2(Nt[K], At[K], q, 1, x_tm, xi_tm, off_nnz);
        if (!status)
        {
			this->solve_from_LU_2(Nk, q, res, nk, x_tm);
        }
        else
        {
            int n_row = nk;
            At[K]->n = n_row;
            At[K]->m = n_row;
            At[K]->nzmax = NNZ;
            SymbFactor* S_qr;
            S_qr = new SymbFactor(n_row * n_row, true);
            QR->solve_2(q, At[K], res, S_qr);
            delete S_qr;
        }
    }
}

void LUsolver::Run(double* b_tmp, double *Xres)
{

    memcpy(Xres, &b_tmp[0], nInt * sizeof(double));
    
    int status = this->LU(this->N, this->matrA, this->S, 1, x, xi);
    if (status == -1)
        QR->solve(this->S->q, this->matrA, Xres);
    else
    	this->solve_from_LU_2(this->N, this->S->q, Xres, nInt, x_tmp);
    
}

void LUsolver::solve(bool flag, int nnzv, const int Ti[], const int Tj[], const double Tx[], double* b_tmp, double *Xres)
{

	if (flag)
	{
		this->ZeroStep(nnzv);
		this->fill_A(Ti, Tj, Tx);
	}
	else
		updateValues(Tx, this->matrA);
	this->Run(b_tmp, Xres);
}

void LUsolver::prep_blk(int nnzv, const int Ti[], const int Tj[], const double Tx[], double* b_tmp, double *Xres)
{
	this->ZeroStep(nnzv);   
	this->fill_A_2(Ti, Tj, Tx);
	
	ipvec(this->DM->pinv, b_tmp, Xres, nInt);
}

void LUsolver::solve_blk(int nnzv, const int Ti[], const int Tj[], const double Tx[], double* b_tmp, double *Xres)
{
		#pragma omp single
		{
			this->prep_blk(nnzv, Ti, Tj, Tx, b_tmp, Xres);
		}

		#pragma omp for  schedule(runtime)
			for (int K = 0; K < DM->N_Blk; ++K)
			{
				int k1 = DM->comcon[K];
				int k2 = DM->comcon[K + 1];
				int nk = k2 - k1;


				int off_n = DM->comcon[K];
				int off_nnz = DM->comcon_NNZ[K];


				double* x_tm;
				int* xi_tm, * q, k;
				double* res = Xres + off_n;
				NumbFactor*   Nk = Nt[K];
				SparseMatrix* Ak = At[K];

				q = colOrder + off_n + K;
				x_tm = x + off_n;
				xi_tm = xi + 2 * (off_n);

	            
				if (off_n)
				{
					int* Ati;
					Ati = Ak->i;
					int NNZ = DM->comcon_NNZ[K + 1] - DM->comcon_NNZ[K]; //DM->nz_b[K];
					for (k = NNZ; k--; ) Ati[k] = Ati[k] - off_n;
				}
				int status = this->LU_2(Nk, Ak, q, 1, x_tm, xi_tm, off_nnz);
	            
				if (!status)
					this->solve_from_LU_2(Nk, q, res, nk, x_tm);
				else
				{
					int n_row = nk;
					Ak->n = n_row;
					Ak->m = n_row;
					Ak->nzmax = NNZ;
					SymbFactor* S_qr;
					S_qr = new SymbFactor(n_row * n_row, true);
					QR->solve_2(q, Ak, res, S_qr);
					delete S_qr;
				}
			}
}

void LUsolver::light_solve_blk(const double Tx[], double* b_tmp, double *Xres)
{

#pragma omp for schedule(runtime)
	for (int K = 0; K < DM->N_Blk; ++K)
	{
		int k1 = DM->comcon[K];
		int k2 = DM->comcon[K + 1];
		int nk = k2 - k1;
		int off_n = DM->comcon[K];
		int off_nnz = DM->comcon_NNZ[K];

		double* x_tm;
		int* xi_tm, * q;
		double* res = Xres + off_n;
		NumbFactor* Nk = Nt[K];
		SparseMatrix * Ak = At[K];

		q = colOrder + off_n + K;
		x_tm = x + off_n;
		xi_tm = xi + 2 * (off_n);

		int status = this->LU_2(Nk, Ak, q, 1, x_tm, xi_tm, off_nnz);

		if (!status)
			this->solve_from_LU_2(Nk, q, res, nk, x_tm);
		else
		{
			#pragma omp critical
			{
				int n_row = nk;
				Ak->n = n_row;
				Ak->m = n_row;
				Ak->nzmax = NNZ;
				SymbFactor* S_qr;
				S_qr = new SymbFactor(n_row * n_row, true);
				QR->solve_2(q, Ak, res, S_qr);
				delete S_qr;
			}
		}
	}


}

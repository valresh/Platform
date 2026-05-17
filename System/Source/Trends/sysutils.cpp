#include "sysutils.h"
#include "CommProc.h"
#include "Err.h"
#include <qdir.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdio.h>

char PROJECT[32];
char EXE_PATH[1024];
char SYSTEM_PATH[1024];
char ROOT_PATH[1024];
char PROJECT_ROOT[1024];
char CURR_PROJECT[64];
char START_CONFIG[64];
char MODELS_PATH[1024];
char LOG_PATH[MAX_PATH];
char Err_BlkInObj[128];
char HELP_PATH[1024];
bool CompressParams;

class MainWindow * pMainWnd;
struct CExtern_Pnt * pFirst_Extern_Pnt = NULL;
struct CExtern_Pnt * pLast_Extern_Pnt = NULL;
struct IBaseControl * pFirstControl = NULL;
struct IBaseControl * pLastControl = NULL;
struct IBaseModel * pControlObject = NULL;
struct CExternalCondition * pExt = NULL;
double Press_KIP[4];
bool Electro[10];

IBaseModel * pMainHydro = NULL;
IBaseModel * pCtrlConn = NULL;
bool ShortParamsList = false;
bool Stop = false;
bool Pause = true;

void * FileTrace = NULL;
struct CSystem * pSys;
double aT_air = 1.;
double bT_air = 0.;
double T_air = 15.;
double T_min = -80;
double T_max = 2000.;
double T_std = 15.;;
BOOL g_bInstructor = FALSE;;

bool StopModel = false;
int nRunApp = 0;
bool ShowAllParams = true;
tFindSensInfo pFindSensInfo = NULL;
void * pConnectionsList = NULL;
typedef int (*tGetAllConnections)( char * ObjName, char * PntName, char & IO, char * RSU[16],  char * Desc[16] );
tGetAllConnections pGetAllConnections = NULL;


void SetPaths()
  {
  QDir DirEXE = QDir::current();
  QString EXE = DirEXE.path();
  strcpy ( EXE_PATH, EXE.toStdString().c_str() );
  strcpy ( PROJECT_ROOT, EXE_PATH );
  int L = strlen ( PROJECT_ROOT );
  while ( PROJECT_ROOT[L] != '/' )
    L--;
  PROJECT_ROOT[L] = 0;
  strcpy ( SYSTEM_PATH, PROJECT_ROOT );
  strcat ( SYSTEM_PATH, "/" );
  strcat ( PROJECT_ROOT, "/projects/" );
  strcat ( PROJECT_ROOT, PROJECT );
  strcat ( PROJECT_ROOT, "/" );
  strcat ( EXE_PATH, "/" );
  strcpy ( HELP_PATH, SYSTEM_PATH );//
  strcat ( HELP_PATH, "HELP/" );
  strcpy ( LOG_PATH, SYSTEM_PATH );//
  strcat ( LOG_PATH, "LOG/" );
  KKK();
  }

#define MAX_MEM 500000000
//QSharedMemory mem("SysMem");
//QFile file ("/home/resh/tmp/SysMem.dat");
static BYTE * pMem = NULL;
static int64_t PosMem = 0;
int MemUsed( )
 {
  return PosMem;
 }

 // void * malloc(size_t size)
 // {
 //   return NewMem( size );
 // }

 // void free ( void * ptr )
 // {

 // }

#include <QMutex>
 QMutex M;
 void * NewMem( size_t size )
  {
   // int k = size;
   // BYTE * pAddr = new BYTE[size];
   // PosMem += size;
   // memset(pAddr,0,size);
   //return pAddr;
  M.lock();
  if ( pMem == NULL )
    {
    int fd = open ( "/home/resh/Platform/DATA/mem.dat", O_RDWR );
    pMem = (BYTE*)mmap(0, MAX_MEM, PROT_READ|PROT_WRITE, MAP_PRIVATE, fd, 0 );
    close ( fd );
    PosMem = 0;
    }
//  assert(PosMem + size < MAX_MEM );
    PosMem = (( PosMem >> 4 ) + 1 ) << 4;
  if ( PosMem + size >= MAX_MEM )
    KKK();
  BYTE * Addr = pMem + PosMem;
  memset(Addr,0,size);
  PosMem += size;
  M.unlock();
  return Addr;
  }
  const double MM = MAX_MEM;
  double Use_Mem()
  {
    return (double)PosMem * 100. / MM;
  }

#include <QRandomGenerator>

double r01()
  {
    // QRandomGenerator qr;
    // double r = qr.generateDouble();
    // return r;
    return 0.5;
  }

  int CharToUTF8( char * To, int MaxTo, char * From, bool Sign )
  {
    unsigned char * F = (unsigned char *)From;
    int p = 0;
    if ( Sign )
    {
      To[p++] = 0xEF;
      To[p++] = 0xBB;
      To[p++] = 0xBF;
    }
    while ( 1 )
    {
      if ( p + 3 > MaxTo )
        break;
      int s = *F++;
      if ( s == 0 )
        break;
      if ( s < 0x80 )
      {
        To[p++] = s;
        continue;
      }
      if ( s == 0xB9 )
      {
        To[p++] = 0xE2;
        To[p++] = 0x84;
        To[p++] = 0x96;
        continue;
      }
      if ( s < 0x7FF )
      {
        if ( s >= 0xC0 )
          s += 0x410 - 0xC0;
        // требуется 2 символа
        int r2 = 0b10000000 | ( s & 0x3F );
        s = s >> 6;
        int r1 = 0b11000000 | ( s & 0x1F );
        To[p++] = r1;
        To[p++] = r2;
        continue;
      }
      assert( 0 ); // больше 2-х не будем
    }
    To[p] = 0;
    return p;
  }
 int ToUTF8( char * Str, int Maxlen )
  {
  char To[1024];
  int L = CharToUTF8( To, 1024, Str, false );
  ASS( L <= Maxlen );
  if ( L <= Maxlen )
    strcpy ( Str, To );
  else
    {
    memcpy ( Str, To, Maxlen - 1 );
    Str[Maxlen-1] = 0;
    }
  }
char * UTF( char * Str )
  {
  static char UTF[1024];
  int L = CharToUTF8( UTF, 1024, Str, false );
  return UTF;
  }

int UtfToChar( unsigned char * To, int MaxTo, char * From, bool Sign  )
  {
    if ( To == NULL )
          return 0;
    if ( From == NULL )
    {
        *To = 0;
        return 0;
    }
    unsigned char * F = (unsigned char *)From;
    unsigned char * T = (unsigned char *)To;
    //  if ( F[0] == 0xEF  && F[1] == 0xBB && F[2] == 0xBF )
    if ( Sign )
      F += 3;
    int p = 0;
    unsigned int r1;
    bool second = false;
    while ( 1 )
    {
      if ( p + 1 > MaxTo )
        break;
      int s = *F++;
      if ( s == 0 )
        break;
      if ( ( s & 0b10000000 ) == 0 )
      {
        // 1
        T[p++] = s;
        continue;
      }
      if ( second )
      {
        // продолжение
        s = s & 0b01111111;
        r1 = r1 | s;
        r1 -= 0x410 - 0xC0;
        T[p++] = r1;
        second = false;
        continue;
      }
      if ( s & 0b11000000 )
      {
        // 2
        s = s & 0b00111111;
        r1 = s;
        r1 = r1 << 6;
        second = true;
        continue;
      }
      if ( s & 0b11100000 )
      {
        // 3
        assert(0);
        continue;
      }
      if ( s & 0b11110000 )
      {
        // 4
        assert(0);
        continue;
      }
    }
    T[p] = 0;
  return p;
  }

int UtfToChar_s( unsigned char * To, int MaxTo, char * From, int L_From  )
  {
    unsigned char * F = (unsigned char *)From;
    unsigned char * T = (unsigned char *)To;
    int p = 0;
    int q = 0;
    unsigned int r1;
    bool second = false;
    while ( q < L_From )
    {
      if ( p + 1 > MaxTo )
        break;
      int s = F[q++];
      if ( s == 0 )
        break;
      if ( ( s & 0b10000000 ) == 0 )
      {
        // 1
        T[p++] = s;
        continue;
      }
      if ( second )
      {
        // продолжение
        s = s & 0b01111111;
        r1 = r1 | s;
        r1 -= 0x410 - 0xC0;
        T[p++] = r1;
        second = false;
        continue;
      }
      if ( s & 0b11000000 )
      {
        // 2
        s = s & 0b00111111;
        r1 = s;
        r1 = r1 << 6;
        second = true;
        continue;
      }
      if ( s & 0b11100000 )
      {
        // 3
        assert(0);
        continue;
      }
      if ( s & 0b11110000 )
      {
        // 4
        assert(0);
        continue;
      }
    }
    T[p] = 0;
    return p;
  }


  void Test()
  {
    //QString Src = "ййиллquyt";
    // char * pDest = NULL;
    // std::u16string Q = Src.toStdU16String();
    // size_t Res = UnicodeToUTF8( &pDest, Q, (size_t)Src.size());
  }

int FsendErr ( char * File, char * Fmt, ... )
  {
  char Txt[20000];
  QString Path = EXE_PATH;
  Path += "/Msg/Err.txt";
  FILE * F = fopen ( STR(Path), "at" );
  if ( F == NULL )
    return 0;
  va_list args;
  va_start( args, Fmt );
  int L = vsprintf( Txt, Fmt, args );
  int Lw = fprintf( F, Txt );
  fclose( F );
  return 0;
  }

BOOL
  ReadFile(
    QFile & hFile,
    LPVOID lpBuffer,
    DWORD nNumberOfBytesToRead,
    LPDWORD lpNumberOfBytesRead,
    void * lpOverlapped
    )
  {
  return hFile.read((char*)lpBuffer, nNumberOfBytesToRead );
  }

  template <int kCol, int max_size>
  int Split ( char * Str, char * Cols/*[kCol][max_size]*/ )
  {
    int L = lstrlen( Str );
    if ( Str[L-1] == 0x0d || Str[L-1] == 0x0a )
    {
      L--;
      Str[L] = 0;
    }
    int K = 0;
    memset( Cols, 0, kCol * max_size );
    char * Pnext = NULL;
    char * P = Str;
    char * Out = Cols;
    while ( K < kCol )
    {
      Pnext = strchr ( P, ';' );
      if ( Pnext == NULL )
      {
        if ( *P == 0 )
          return K + 1;
        int L = lstrlen ( P );
        if ( L >= max_size )
          P[max_size-1] = 0;
        lstrcpy ( Out, P );
        return K+1;
      }
      *Pnext = 0;
      int L = lstrlen ( P );
      if ( L >= max_size )
        P[max_size-1] = 0;
      lstrcpy ( Out, P );
      Out += max_size;
      P = Pnext + 1;
      K++;
    }
    return K;
  }

  template <int kCol, int max_size>
  int Split ( FILE * F, char * Cols /*[kCol][max_size]*/)
  {
    char Str[256];
    if ( fgets ( Str, 255, F ) == NULL )
      return -1;
    return Split<kCol,max_size>( Str, Cols );
  }
  static int NumbRec = 1;
  int kCriticalErrors = 0;
  bool WriteResume = false;
  bool LinSolv ( int N, double * A, double * B, double * X )		//Решение системы линейных уравнений методо Гаусса
  {
    if ( N == 1 )
    {
      if ( fabs ( *A ) < 1e-50 )
        return false;
      *X = *B / *A;
      return true;
    }
    int Col[100];
    int Row[100];
    int n;
    for ( n = 0; n < N; n++ )
    {
      Col[n] = n;
      Row[n] = n;
    }
  // Вперед
#define M(r,c) *(A+((r)*N+(c)))		//Переход к элементу A[r][c]
#define V(r) *(B+(r))
    for ( n = 0; n < N - 1; n++ )
    {
      // Ведущий элемент
      double Max = 0.;
      int im, jm;
      for ( int i = n; i < N; i++ )
      {
        int r = Row[i];
        for ( int j = n; j < N; j++ )
        {
          int c = Col[j];
          double D = fabs ( M(r,c) );
          if ( D > Max )
          {
            Max = D;
            im = i;
            jm = j;
          }
        }
      }
      if ( Max < 1e-50 )
        return false; // вырождение
      //
      int w = Row[n];
      Row[n] = Row[im];
      Row[im] = w;
      w = Col[n];
      Col[n] = Col[jm];
      Col[jm] = w;
      // Удаление снизу
      int rd = Row[n];
      int cd = Col[n];
      double D = 1. / M(rd,cd);
      for ( int j = n + 1; j < N; j++ )
      {
        int c = Col[j];
        M(rd,c) *= D;
      }
      V(rd) *= D;
      for ( int i = n + 1; i < N; i++ )
      {
        int r = Row[i];
        double C = M(r,cd);
        if ( C != 0. )
        {
          for ( int j = n + 1; j < N; j++ )
          {
            int c = Col[j];
            M(r,c) -= M(rd,c) * C;
          }
          V(r) -= V(rd) * C;
        }
      }
    }
    // Обратный ход
    for ( n = 0; n < N; n++ )
      X[n] = 1e50;
    int r = Row[N-1];
    int c = Col[N-1];
    double D = M(r,c);
    if ( fabs ( D ) < 1e-50 )
      return false;
    X[c] = V(r) / D;
    for ( n = N - 2; n >= 0; n-- )
    {
      int r = Row[n];
      double Sx = 0.;
      for ( int j = n + 1; j < N; j++ )
      {
        c = Col[j];
        //			int x = Row[j];
        Sx += M(r,c) * X[c];
      }
      int c = Col[n];
      X[c] = V(r) - Sx;
    }
    return true;
  }

  double Exp1 ( double Lam )
  {
    ASS(Lam >= 0.);
    if ( Lam < 1e-6 )
      return 1.;
    return ( 1. - exp ( -Lam )) / Lam;
  }

  //Функция расчета корня (1-Exp(-x))/x=Н, при 0.09<Н<0.999999
  double InvExp1 ( double E, double O )
  {
    if ( E >= 0.999999 )
      return 0.;
    if( E > 0.9995 )
    {
      double v =
        (( -3.5510669818 * E + 11.984569697 ) * E - 15.31593887 ) * E +
        6.8824361347651;
      return v;
    }
    if( E > 0.62 )
    {
      double v =
        (( -2.2976516677 * E + 7.9615691193 ) * E - 11.04556632857 ) * E +
        5.381550781;
      return v;
    }
    if( E > 0.35 )
    {
      double v =
        ((-17.54944415 * E + 35.17098164 ) * E - 27.3604038688 ) * E +
        8.67340208778;
      return v;
    }
    if( E > 0.2 )
    {
      double v =
        ((( 687.0402563452 * E - 964.2836058264 ) * E + 529.91687898) * E -
         144.086583514 ) * E + 19.20340708796;
      return v;
    }
    if( E > 0.09 )
    {
      double v =
        (((( -146164.379693 * E + 127256.7085772 ) * E -
           45238.6749683 ) * E + 8398.2385071) * E - 858.75283582) * E +
        45.8675952;
      return v;
    }
    return 1. / E;
  }

int sprintf_s ( char * To, int MaxLen, const char * Fmt, ... )
  {
  va_list arg;
  va_start(arg,Fmt);
  return vsnprintf ( To, MaxLen, Fmt, arg );
  }

int _access( const char *path, int mode )
  {
  char Path[1024];
  strcpy ( Path, path );
  char * P = Path;
  while ( 1 )
    {
    char s = *P;
    if ( s == 0 )
      break;
    if ( s == '\\')
      *P = '/';
    P++;
    }
  if ( QFile::exists( Path ))
      return 0;
  return -1;
  }

void SafeCopy( char * To, int MaxTo, const char* Name)
  {
  if ( Name == NULL)
    return;
  int L = lstrlen(Name);
  if (L < MaxTo)
    {
    strcpy( To, Name);
    return;
    }
  L = MaxTo - 3;
  memmove( To, Name, L);
  memmove( To + L, "..", 3);
  }

void Cut( char * To, int MaxTo )
  {
  if ( To == NULL)
    return;
  int L = lstrlen( To );
  if (L < MaxTo)
    {
    return;
    }
  L = MaxTo - 3;
  memmove( To + L, "..", 3);
  }

int strcmpi( const char * str1, const char * str2 )
  {
  return QString::compare(str1, str2, Qt::CaseInsensitive);
  }

int Q_DECL_EXPORT DllMain( int hModule,
             DWORD  ul_reason_for_call,
             LPVOID lpReserved	 )
  {
  return 0;
  }
  char * NodeNoNumb ( char * Node )
  {
    char * P = Node;
    while ( *P >= '0' && *P <= '9' )
      P++;
    return P;
  }

  char * ConvertNode ( char * Node )
  {
    char * NoNymb = NodeNoNumb ( Node );
    if ( strncmp ( NoNymb, "sol_", 4 ) == 0 )
      return NoNymb + 4;
    if ( strncmp ( NoNymb, "gas_", 4 ) == 0 )
      return NoNymb + 4;
    return NoNymb;
  }

  int lstrcmpi_m ( char * Variants, char * Test )
  {
    char Var[256];
    lstrcpy ( Var, Variants );
    char * B = Var;
    char * E;
    while ( 1 )
    {
      E = strchr ( B, '|' );
      if ( E )
        *E = 0;
      int Res =  lstrcmpi ( B, Test );
      if ( Res == 0 )
        return 0;
      if ( E == NULL )
        return Res;
      B = E + 1;
    }
    return false;
  }
  bool CompNodeM ( char * Node, char * Etalon )
  {
    char * Pure = NodeNoNumb ( Node );
    while ( *Etalon )
    {
      if ( *Pure != *Etalon )
        return false;
      Pure++;
      Etalon++;
    }
    return true;
  }

// #include <sched.h>
//#include <unistd.h> // Для getpid()
 void set_thread_affinity(int core_id)
    {
    // cpu_set_t mask;
    // CLEAR(mask.__bits)
    // mask.__bits[core_id] = 1;
    // __PID_T_TYPE pid = getpid();
    // int Res = sched_setaffinity( pid, 4, &mask );    // cpu_set_t cpuset;
    // cpu_set_t test;
    // Res = sched_getaffinity( pid, sizeof( pid ), &test );
    // KKK();
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core_id, &cpuset);
    pthread_t current_thread = pthread_self();
    pthread_setaffinity_np(current_thread, sizeof(cpu_set_t), &cpuset);
  }

#define TEG \
  while ( *P != ' ' )\
    P++;\
  while ( *P == ' ' )\
    P++;

#if 0
 void ProcessData::Set()
  {
   return;
   // Sprintf ( Path, "/proc/self/stat" );
   // ///proc/thread-self
   // FILE * F = fopen ( Path, "rt" );
   // fgets ( Path, 1024, F );
   // fclose ( F );
//   extern int sched_setscheduler (__pid_t __pid, int __policy,
//                                   const struct sched_param *__param) __THROW;
//   extern int sched_getscheduler (__pid_t __pid) __THROW;
//int sched_setaffinity(pid_t pid, unsigned int len, unsigned long *mask);

//int sched_getaffinity(pid_t pid, unsigned int len, unsigned long *mask);
   /* Retrieve scheduling algorithm for a particular purpose.  */
//   extern int sched_getscheduler (__pid_t __pid) __THROW;
   char Txt[1024];
   FILE * F = fopen ( "/proc/thread-self/stat", "rt" );
   fgets ( Txt, 1024, F );
   fclose ( F );
   char * P = Txt;
   for ( int n = 0; n < 12; n++ )
   {
     TEG
   }
   int64_t U = atoi ( P );
   TEG
   int64_t S = atoi ( P );
   TEG
   int64_t M = atoi ( P );
   int64_t dU = U - User;
   int64_t dS = S - Sys;
   ProcUer = (double)dU * 100. / (double)(dU+dS);
   User = U;
   Sys = S;
   Mem = M;
   KKK();
  }

  void ProcessData::SetSys()
  {
    // Sprintf ( Path, "/proc/self/stat" );
    // ///proc/thread-self
    // FILE * F = fopen ( Path, "rt" );
    // fgets ( Path, 1024, F );
    // fclose ( F );
    //   extern int sched_setscheduler (__pid_t __pid, int __policy,
    //                                   const struct sched_param *__param) __THROW;
    //   extern int sched_getscheduler (__pid_t __pid) __THROW;

    /* Retrieve scheduling algorithm for a particular purpose.  */
    //   extern int sched_getscheduler (__pid_t __pid) __THROW;
    char Txt[1024];
    FILE * F = fopen ( "/proc/self/stat", "rt" );
    fgets ( Txt, 1024, F );
    fgets ( Txt, 1024, F );
    fgets ( Txt, 1024, F );
    fclose ( F );
    char * P = Txt;
    for ( int n = 0; n < 13; n++ )
    {
      TEG
    }
    int64_t U = atoi ( P );
    TEG
      int64_t S = atoi ( P );
    TEG
      int64_t M = atoi ( P );
    int64_t dU = U - User;
    int64_t dS = S - Sys;
    ProcUer = (double)dU * 100. / (double)(dU+dS);
    User = U;
    Sys = S;
    Mem = M;
    KKK();
  }
#endif

void ProcessData::SetCPU( int n0, int n1, int n2 )
  {
    char Txt[1024];
    FILE * F = fopen ( "/proc/stat", "rt" );
    fgets ( Txt, 1024, F );
    char * P = Txt;
    int64_t St = 0;
    for ( int p = 0; p < 10; p++ )
    {
        TEG
            St += atoi( P );
    }
    int64_t dSt = St - S;
    S = St;
    // Загруженность процессоров
    int n_processors = sysconf(_SC_NPROCESSORS_ONLN);
    for ( int p = 0; p < n_processors; p++ )
    {
        if ( !fgets ( Txt, 1024, F ))
            break;
        char * P = Txt;
        int64_t user = 0;
        int64_t idle = 0;
        for ( int n = 0; n < 10; n++ )
        {
            TEG
             if ( *P == 0 )
                break;
            if ( n != 3 )
                user += atoi ( P );
            else
                idle += atoi ( P );
        }
     int64_t dU = user - User[p];
     User[p] = user;
     int64_t dI = idle - Idle[p];
     Idle[p] = idle;
     if ( dI < 0 )
        dI = 0;
     if ( dU <= 0 )
         Proc[p] = 0.;
    else
        Proc[p] = (double)dU * 100. / (double)(dU + dI);
    }
   fclose(F);
   SysProc[0] = Proc[n0];
   SysProc[1] = Proc[n1];
   SysProc[2] = Proc[n2];
// Загруженность процесса
  F = fopen ( "/proc/self/stat", "rt" );
  fgets ( Txt, 1024, F );
  fclose(F);
  P = Txt;
  for ( int p = 0; p < 12; p++ )
    {
    TEG
    }
  int Pr = atoi( P );//13
  TEG
  Pr += atoi( P );//14
  int64_t dU = Pr - U;
  if ( dSt >  0 )
  {
  ThisProc = (double)dU * 100. / (double)(dSt);
  U = Pr;
  }
  for ( int p = 0; p < 4; p++ )
  {
    TEG
  }
  int64_t Prty = atoi( P );//18
  TEG
  int64_t Nice = atoi( P );//19
  TEG TEG TEG TEG
    const double MB = 1024. * 1024.;
  Mem = atof( P ) / MB; // 23
  // KKK();
  };

void OutDI( double V, char Txt[256])
  {
  if (!finite(V))
  {
    strcpy(Txt, "#");
    return;
  }
  if (V > 1e10)
  {
    strcpy(Txt, ">");
    return;
  }
  if (V < -1e10)
  {
    strcpy(Txt, "<");
    return;
  }
  if (V == 0.)
    strcpy(Txt, "0");
  else
    if (fabs(V) < 1e-5)
      snprintf(Txt, 128, "%13.5le", V);
    else
      if (fabs(V) < 1e-4)
        snprintf(Txt, 128, "%11.9lf", V);
      else
        if (fabs(V) < 1e-3)
          snprintf(Txt, 128, "%10.8lf", V);
        else
          if (fabs(V) < 1e-2)
            snprintf(Txt, 128, "%9.7lf", V);
          else
            if (fabs(V) < 1e-1)
              snprintf(Txt, 128, "%8.6lf", V);
            else
              if (fabs(V) < 1.)
                snprintf(Txt, 128, "%7.5lf", V);
              else
                if (fabs(V) < 10.)
                  snprintf(Txt, 128, "%6.4lf", V);
                else
                  if (fabs(V) < 100.)
                    snprintf(Txt, 128, "%5.3lf", V);
                  else
                    if (fabs(V) < 1000.)
                      snprintf(Txt, 128, "%5.2lf", V);
                    else
                      if (fabs(V) < 10000.)
                        snprintf(Txt, 128, "%5.1lf", V);
                      else
                        if (fabs(V) > 10000000.)
                          snprintf(Txt, 128, "%e", V);
                        else
                          snprintf(Txt, 128, "%5.0lf", V);
  }

  int Skip( char * Txt )
  {
      int p = 0;
      while ( 1 )
      {
          char s = Txt[p];
          if ( s == 0 || s == 0x0d || s == 0x0a || s == '%' )
              return -1;   // До  конца строки ничего полезного
          if ( s == 0x07 || s == 0x08 || s == ' ' )
          {
              p++;
              continue;
          }
          break;
      }
      return p;
  }

  int Shablon ( char * Txt, char * Example )  // Поиск в строке
  {
      int p = 0;
      int q = 0;
      char s[2];
      char c[2];
      CLEAR(s)
      CLEAR(c)
      while ( 1 )
      {
          s[0] = Txt[p];
          if ( *s == 0 || *s == '%' ) // % - начало комментария
              return -1;
          if ( *s == 0x07 || *s == 0x08 || *s == ' ' )
          {
              p++;
              continue;
          }
          c[0] = Example[q];
          if ( *c == 0 )
              return p;
          if ( strcmpi ( s, c ) == 0 )
          {
              p++;
              q++;
              continue;
          }
          return -1;
      }
      return -1;
  }

  int Variable ( char * Txt, char Var[256] )  // Var=
  {
      int p = 0;
      int q = 0;
      char s;
      while ( 1 )
      {
          s = Txt[p];
          if ( s == 0 || s == '%' ) // % - начало комментария
              return -1;
          if ( s == 0x07 || s == 0x08 || s == ' ' )
          {
              p++;
              continue;
          }
          if ( s == '=' )
          {
              Var[q] = 0;
              return p + 1;
          }
          Var[q++] = s;
          p++;
      }
      return -1;
  }

  int Text ( char * Txt, char * Text )  // Поиск в строке
  {
      int p = Skip( Txt );
      if ( Txt[p] == '\"' )
      {
          int q = 0;
          p++;
          while ( Txt[p] && Txt[p] != '\"' )
          {
              Text[q] = Txt[p];
              q++;
              p++;
          }
          Text[q] = 0;
          return p + 1;
      }
      return -1;
  }

int Split(
      const char * Model,
      const char * Section,
      const char ** ParamsName,
      char Params[][256] )
  {
      Char<1024>Path;
      Path.Prt ( "%sDATA\\CHEMScript\\%s.txt", PROJECT_ROOT, Model  );
      FILE * F;
      F = fopen ( Path, "rt" );
      if ( F == NULL )
          return -1;
      Char<16>Start;
      Char<16>Finish;
      Char<64>Parm;
      bool Find[1024];
      CLEAR(Find)
      int kParams = 0;
      Start.Prt ( "<%s", Section );
      Finish.Prt ( "</%s>", Section );
      char Str[256];
      char * Txt = Str;
      bool Started = false;
      while ( fgets ( Str, 255, F ))
      {
          int p = Skip ( Txt );
          if ( p < 0 )
              continue;
          Txt += p;
          if ( Started )
          {
              p = Shablon ( Txt, Finish );  // Поиск в строке
              if ( p >= 0 )
                  break;
          }
          if ( !Started )
          {
              p = Shablon ( Txt, Start );  // Поиск в строке
              if ( p < 0 )
                  continue;
              Started = true;
              Txt += p;
          }
          char Var[256];
          while ( 1 )
          {
              CLEAR(Var)
              p = Variable ( Txt, Var );  // Var=
              if ( p < 0 )
                  break;
              int nParam = 0;
              Txt += p;
              while ( ParamsName[nParam] )
              {
                  if ( strcmpi ( ParamsName[nParam], Var ) == 0 )
                  {
                      p = Text ( Txt, Params[nParam] );
                      kParams++;
                      break;
                  }
                  nParam++;
              }
              Txt += p;
          }
      }
      fclose ( F );
      return kParams;
  };

#include <QClipboard>
#include <QGuiApplication>

void TxtToClp( char * Txt )
  {
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText( Txt );
  }
void TxtFromClp( int MaxLen, char * Txt )
  {
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString S = clipboard->text( );
    strcpy_s ( Txt, MaxLen, STR(S));
  }

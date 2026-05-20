#include "mainwindow.h"
#include "DB.h"
#include <malloc.h>
#include <mcheck.h>
//https://man7.org/linux/man-pages/man3/mallopt.3.html
#include <QApplication>

/*
static void TestMem ( enum mcheck_status mstatus )
{
  KKK();
}


//typeof(void *(void *p, const void *caller))
//  *volatile
  __free_hook = Free;
//  *volatile  __malloc_hook;
//__malloc_hook = Mall;

//typeof(void *(size_t size, const void *caller))
//  *volatile
    __malloc_hook = Mall ;
*/
/*
static void * Mall (size_t size, const void *caller)
{
  KKK();
}

static void Free (void *p, const void *caller)
{
  KKK();
}
void *(*__malloc_hook)(size_t size, const void *caller) = Mall;

void (*__free_hook)(void *ptr, const void *caller) = Free;
*/
/*
void * malloc(size_t __size) noexcept (true)
{
  return NewMem (__size );
}
void free(  void * __ptr) noexcept (true)
{
  KKK();
}
void * calloc(size_t __nmemb, size_t __size) noexcept (true)
{
  return NewMem (__size * __nmemb );
}
void * realloc(void *__ptr, size_t __size) noexcept (true)
{
  return NewMem (__size );

}
/**/
//https://www.linuxjournal.com/article/6390
#include "malloc.h"
#include "stdio.h"
#include <sys/resource.h>

int main(int argc, char *argv[])
{
  try {
      // struct rlimit rl;
      // int err;
      // rl.rlim_cur = 400 * 1024 * 1024; // задаём размер стека в байтах
      // err = setrlimit(RLIMIT_STACK, &rl);
      // KKK();
      // mallopt( M_ARENA_MAX, 16 );
 //     struct mallinfo2 Info2 = mallinfo2();
      // int Res = mallopt(M_ARENA_MAX, 200);
      // Res = mallopt(M_TRIM_THRESHOLD, -1);
      // Res = mallopt(M_CHECK_ACTION,7);
//  ptrace(PTRACE_TRACEME, getpid(), 0, 0);
//  DB::Test( );
//  uint customStackSize = 4 * 1024 * 1024;
//  setStackSize(customStackSize);
  //mcheck( TestMem );
//  mcheck_check_all();
//  mallopt(M_ARENA_MAX, 100);
//  mallopt(M_TRIM_THRESHOLD, -1);
  DB::Read( );
  QApplication a(argc, argv);
  MainWindow w;
  w.show();

  return a.exec();
  } catch (...)
  {
    KKK();
  }

}
// MCHECK_DISABLED: consistency checking turned off (mtrace() was not called).
//                   MCHECK_OK: the block is correct.
//                               MCHECK_FREE: the block has been freed twice.
//                                             MCHECK_HEAD: memory before the block is corrupted.
//                                                           MCHECK_TAIL: memory after the block is corrupted.

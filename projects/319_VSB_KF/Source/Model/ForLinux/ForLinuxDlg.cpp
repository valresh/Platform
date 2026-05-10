
// ForLinuxDlg.cpp : файл реализации
//

#include "stdafx.h"
#include "ForLinux.h"
#include "ForLinuxDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CAboutDlg используется для описания сведений о приложении
#include <sys/types.h>

typedef __wchar_t uchar_t; //u_int16_t
typedef unsigned int u_int32_t;
typedef unsigned char u_int8_t;



class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Данные диалогового окна
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// диалоговое окно CForLinuxDlg




CForLinuxDlg::CForLinuxDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CForLinuxDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CForLinuxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CForLinuxDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
  ON_BN_CLICKED(IDOK, &CForLinuxDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// обработчики сообщений CForLinuxDlg

BOOL CForLinuxDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Задает значок для этого диалогового окна. Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void CForLinuxDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок. Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CForLinuxDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CForLinuxDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

size_t UnicodeToUTF8(char ** pDest, const uchar_t* Src, size_t SrcLen);
bool Convert ( char * File );

void Find ( char * File )
  {
  char Path[1024];
  sprintf_s ( Path, 1024, "%s\\*.*", File );
  CFileFind finder;
  BOOL bWorking = finder.FindFile( Path );
  while ( bWorking )
    {
    bWorking = finder.FindNextFile();
    if ( finder.IsDots())
      continue;
    if ( strstr ( File, "ForLinux"))
      continue;
    if ( finder.IsDirectory())
      {
      char Path[1024];
      strcpy ( Path, finder.GetFilePath() );
      Find ( Path );
      continue;
      }
    char File[512];
    strcpy ( File, finder.GetFileName() );
    bool OK = false;
    if ( strstr ( File, ".cpp"))
      OK = true;
    if ( strstr ( File, ".h"))
      OK = true;
    if ( strstr ( File, ".hpp"))
      OK = true;
    if ( !OK )
      continue;
    char Path[1024];
    strcpy ( Path, finder.GetFilePath() );
    if ( Convert ( Path ))
      {
      TRACE(_T("%s\n"), (LPCTSTR)finder.GetFileName());
      }
    } 
  finder.Close();
  }

#define MAX 1000000
char txt[MAX];
__wchar_t TXT[MAX];


bool Convert ( char * File )
  {
  HANDLE hFile = CreateFile ( File, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, NULL );
  if ( hFile == INVALID_HANDLE_VALUE )
    {
    AfxMessageBox( "1" );
    return false;
    }
  int S = GetFileSize ( hFile, NULL );
  DWORD Lr;
  ReadFile ( hFile, txt, S, &Lr, NULL );
  CloseHandle( hFile );
//
  BYTE B0 = txt[0];
  BYTE B1 = txt[1];
  BYTE B2 = txt[2];
  if ( B0 == 0xFF && B1 == 0xFE )
    {
    // Little
    return false;
    }     
  bool Big = false;
  if ( B0 == 0xFE && B1 == 0xFF )
    {
    // Big
    Big = true;
    }
  int Size = 0;
  if ( Big )
    {
    int p = 0;
    for ( int n = 2; n < S; n += 2 )
      {
      int s0 = txt[n];
      int s1 = txt[n+1];
      int W = ( s0 << 8 ) | s1;
      TXT[p++] = W;
      }
    TXT[p] = 0;
    Size = p;
    }
  else
    {
    Size = MultiByteToWideChar( 1251, 0, txt, S, TXT, MAX );
    }
  char * UTF = NULL;
  size_t Res = UnicodeToUTF8( &UTF, TXT, Size );
  hFile = CreateFile ( File, GENERIC_WRITE, 0, 0, OPEN_ALWAYS, 0, NULL );
  if ( hFile == INVALID_HANDLE_VALUE )
    {
    AfxMessageBox( "2" );
    return false;
    }
  DWORD Lw;
  WriteFile( hFile, UTF, Res, &Lw, NULL );
  SetEndOfFile( hFile );
  CloseHandle( hFile );
  return true;
  }

void CForLinuxDlg::OnBnClickedOk()
  {
  //Convert ("D:\\Platform_3.0\\Projects\\319_VSB_KF\\Source\\Model\\Vol_b\\Points.h" );
  Find ( "D:\\Platform_3.0\\Projects\\319_VSB_KF\\Source" );
  }


size_t UnicodeToUTF8(char** pDest, const uchar_t* Src, size_t SrcLen) {
	size_t dlen = 3;				// Длина целевой строки
	char* offset = *pDest = NULL	// Итератор по целевой строке
		, header = (char)0x80		// "заголовок" первого байта
		, fcap = 0x3F;//0b00111111;		// Вместимость первого бийта
	const uchar_t* src = Src		// указатель на кодируемую точку
		, * end = Src + SrcLen;		// Указатель на конец исходной строки
	uchar_t code = 0;				// Кодируемая точка
	u_int32_t* ibuf = NULL;			// Указатель на целевую строку представленную как беззнаковое целое 32 бита
	// Выделяем память, сохраняем указатель в *pDest, заодно инициализируем итератор
	offset = *pDest = (char*)malloc(SrcLen * 4 + 4);
  *offset++ = 0xEF;
  *offset++ = 0xBB;
  *offset++ = 0xBF;
	// Проверяем выделена ли память
	if (NULL != offset) {
		// основной цикл
		while (src < end) {
			/* Проверяем размерность исходного символа, если величина
				меньше чем 0x80U, значит его кодировать не нужно */
			if (*src < 0x80UL) {
				// копируем значение
				*(offset) = *(char*)(src);
			}
			else {
				// Коируем кодовую точку
				code = *src;
				// сохраняем указатель на первый байт как на беззнаковое целое 32 бита
				ibuf = (u_int32_t*)offset;
				// Обнуляем 4 байта
				(*ibuf) = 0UL;
				// Инициализируем заголовок
				header = (char)0x80;
				// выставляем вместимость первого байта
				fcap = 0x3F;//0b00111111U;
				/* цикл непосредственно кодирования точки в последовательность байтов */
				while (code) {
					// 
					// сравниваем вместимость первого байта и остаток незакодированных битов
					if (fcap < code) {
						// вместимость не меньше остатка
						// копируем младшие 6 битов
						*ibuf |= code & 0x3F;//0b00111111UL;
						// копируем маркер последующего байта
						*ibuf |= 0x80UL;
						// сдвигаем все биты в более старший байт
						*ibuf <<= 8;
						// убираем закодированные 6 битов
						code >>= 6;
						// сдвигаем "заголовок" первого байта на 1 вправо
						header >>= 1;
						// уменьшаем вместимость первого байта
						fcap >>= 1;
						// инкрементируем длину целевой строки
						++dlen;
						// смещаем указатель на на итоговую строку вправо на 1 символ
						++offset;
					}
					else {
						// сохраняем заголовок и оставшиеся биты в первый байт последовательности
						*(u_int8_t*)ibuf |= (header | (u_int8_t)code);
						// обнуляем кодовую точку
						code = 0U;
					}
				}
			}
			// Смещаем указатель на целевую строку
			++offset;
			// Смещаем указатель на исходную строку
			++src;
			// Увеличиваем показатель длины итоговой целевой строки
			++dlen;
		}
	}
	// Проверяем на ошибки, если их нет - обозначаем конец целевой строки
	if (dlen)
		*offset = '\0';
	// возвращаем итоговую длину
	return dlen;
}
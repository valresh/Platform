#ifndef API_H
#define API_H
#define API __declspec(dllexport)

#include <stdlib.h>
#include <crosswindows.h>
#include <crossplatform.h>

#ifndef HDC
#define HDC uint32_t
#endif
#include "string"

typedef void* HVIEWMNEMO;
#ifndef __uint32_t_defined
typedef unsigned int		uint32_t;
# define __uint32_t_defined
#endif

    //typedef  uint32_t HENHMETAFILE;
    typedef struct tagENHMETAHEADER
    {
        DWORD   iType;              // Record type EMR_HEADER
        DWORD   nSize;              // Record size in bytes.  This may be greater
        // than the sizeof(ENHMETAHEADER).
        // RECTL   rclBounds;          // Inclusive-inclusive bounds in device units
        // RECTL   rclFrame;           // Inclusive-inclusive Picture Frame of metafile in .01 mm units
        DWORD   dSignature;         // Signature.  Must be ENHMETA_SIGNATURE.
        DWORD   nVersion;           // Version number
        DWORD   nBytes;             // Size of the metafile in bytes
        DWORD   nRecords;           // Number of records in the metafile
        WORD    nHandles;           // Number of handles in the handle table
        // Handle index zero is reserved.
        WORD    sReserved;          // Reserved.  Must be zero.
        DWORD   nDescription;       // Number of chars in the unicode description string
        // This is 0 if there is no description string
        DWORD   offDescription;     // Offset to the metafile description record.
        // This is 0 if there is no description string
        DWORD   nPalEntries;        // Number of entries in the metafile palette.
        // SIZEL   szlDevice;          // Size of the reference device in pels
        // SIZEL   szlMillimeters;     // Size of the reference device in millimeters
#if(WINVER >= 0x0400)
        DWORD   cbPixelFormat;      // Size of PIXELFORMATDESCRIPTOR information
        // This is 0 if no pixel format is set
        DWORD   offPixelFormat;     // Offset to PIXELFORMATDESCRIPTOR
        // This is 0 if no pixel format is set
        DWORD   bOpenGL;            // TRUE if OpenGL commands are present in
        // the metafile, otherwise FALSE
#endif /* WINVER >= 0x0400 */
#if(WINVER >= 0x0500)
        SIZEL   szlMicrometers;     // Size of the reference device in micrometers
#endif /* WINVER >= 0x0500 */

    } ENHMETAHEADER, * PENHMETAHEADER, * LPENHMETAHEADER;


#define E_FAIL                           _HRESULT_TYPEDEF_(0x80004005L)

 enum WrapperDrawTextFormat
{
    DT_TOP = 0x00000000,
    DT_LEFT = 0x00000000,
    DT_CENTER = 0x00000001,
    DT_RIGHT = 0x00000002,
    DT_VCENTER = 0x00000004,
    DT_BOTTOM = 0x00000008,
    DT_WORDBREAK = 0x00000010,
    DT_SINGLELINE = 0x00000020,
    DT_EXPANDTABS = 0x00000040,
    DT_TABSTOP = 0x00000080,
    DT_NOCLIP = 0x00000100,
    DT_EXTERNALLEADING = 0x00000200,
    DT_CALCRECT = 0x00000400,
    DT_NOPREFIX = 0x00000800,
    DT_INTERNAL = 0x00001000,
    DT_EDITCONTROL = 0x00002000,
    DT_PATH_ELLIPSIS = 0x00004000,
    DT_END_ELLIPSIS = 0x00008000,
    DT_MODIFYSTRING = 0x00010000,
    DT_RTLREADING = 0x00020000,
    DT_WORD_ELLIPSIS = 0x00040000
};

enum MessageBoxFlags
 {
     MB_OK = 0,
     MB_OKCANCEL = 1,
     MB_ABORTRETRYIGNORE = 2,
     MB_YESNOCANCEL = 3,
     MB_YESNO = 4,
     MB_RETRYCANCEL = 5,
     MB_CANCELTRYCONTINUE = 6
 };



struct WrapperColor
{
    std::byte A;
    std::byte R;
    std::byte G;
    std::byte B;
};

struct WrapperFont
{
    std::string fontFamily;
    std::byte fontSize;
    std::byte fontWeight;
};


struct WrapperPoint
{
    std::byte X;
    std::byte Y;
};


struct WrapperRectF
{
    double X;
    double Y;
    double width;
    double height;

};
struct WrapperBitmap
{
    std::string filePath;
    int fileType;
};

struct WrapperSolidBrush
{
    std::byte A;
    std::byte R;
    std::byte G;
    std::byte B;
};


struct WrapperBrush
{
    HDC hDC;
    bool selected;
    WrapperColor color;
};

struct WrapperPen
{
    HDC hDC;
    int color ;
    int nWidth;
    int nStyle;
};

struct WrapperTagRect
{
    int left;
    int top;
    int right;
    int bottom;
};




// C++ код (HoneywellDataLibd)
//typedef void (*DrawLineDelegate)(double x1, double y1, double x2, double y2, uint32_t color);

//typedef void (*CreateBrushDelegate)(double x1, double y1, double x2, double y2, uint32_t color);

typedef void (*DrawLineCallback)(int x1, int y1, int x2, int y2, uint32_t color) ;
typedef void (*FillRectangeCallback)(WrapperSolidBrush* brush, const WrapperRectF* rectF);
typedef void (*FillRectangeBCallback)(WrapperBrush* brush, const WrapperRectF* rectF);
typedef void (*DrawStringCallback)(std::string str, int  length, const WrapperFont* font, const WrapperRectF* rectF, const WrapperBrush* ssBrush);
typedef void (*DrawTextWCallback)(HDC hdc, std::string str, int  length, WrapperTagRect& rect, WrapperDrawTextFormat format);
typedef void (*OffsetRectCallback)(WrapperTagRect& tagRef, int dx, int dy);
typedef void (*RoundRectCallback)(UINT hdc, int left, int top, int right, int bottom, int width, int height);
typedef void (*SelectFontObjectCallback)(UINT hdc, WrapperFont font);
typedef void (*SelectPenObjectCallback)(UINT hdc, WrapperPen pen);
typedef void (*GetPenObjectCallback)(UINT hdc);
typedef void (*SelectBrushObjectCallback)(UINT hdc, WrapperBrush brush);
typedef void (*GetBrushObjectCallback)(UINT hdc);
typedef void (*DrawBitmapCallback)(WrapperBitmap bitmap, WrapperRectF rectF);
typedef void (*ShowMessageBoxCallback)(UINT hdc, std::string text, MessageBoxFlags flags);



//files graphis
//WINGDIAPI BOOL  WINAPI DeleteEnhMetaFile( _In_opt_ HENHMETAFILE hmf);
typedef bool (*DeleteEnhMetaFile)(HENHMETAFILE hmf);

//HENHMETAFILE -is HANDEL
//WINGDIAPI HENHMETAFILE  WINAPI GetEnhMetaFileA( __in LPCSTR lpName);
typedef HENHMETAFILE (*GetEnhMetaFileA)(char* lpName);
typedef bool(*Load)(char* lpName);


//BLEAT SIMPsLE
// WINGDIAPI UINT  WINAPI GetEnhMetaFileHeader(    _In_ HENHMETAFILE hemf,
//                                            _In_ UINT nSize,
//                                            _Out_writes_bytes_opt_(nSize) LPENHMETAHEADER lpEnhMetaHeader);


//DrawTextW(
//    __in HDC hdc,
//    __inout_ecount(cchText) LPCWSTR lpchText,
//    __in int cchText,
//    __inout LPRECT lprc,
//    __in UINT format);


//OffsetRect(
//    __inout LPRECT lprc,
//    __in int dx,
//    __in int dy);


//WINGDIAPI BOOL  WINAPI RoundRect(__in HDC hdc, 
// __in int left, __in int top, 
// __in int right, __in int bottom, __in int width, __in int height);


struct RenderCallbacks {

    DrawLineCallback _DrawLineCallback;
    FillRectangeCallback  _FillRectangeCallback;
    FillRectangeBCallback _FillRectangeBCallback;
    DrawStringCallback _DrawStringCallback;
    DrawTextWCallback _DrawTextWCallback;
    OffsetRectCallback _OffsetRectCallback;
    RoundRectCallback _RoundRectCallback;
    SelectFontObjectCallback _SelectFontObjectCallback;
    SelectPenObjectCallback _SelectPenObjectCallback;
    GetPenObjectCallback _GetPenObjectCallback;
    SelectBrushObjectCallback _SelectBrushObjectCallback;
    GetBrushObjectCallback _GetBrushObjectCallback;
    DrawBitmapCallback _DrawBitmapCallback;
    ShowMessageBoxCallback _ShowMessageBoxCallback;
};

struct GraphisFilesCallbacks
{
    DeleteEnhMetaFile _DeleteEnhMetaFile;
    Load _Load;
};



extern "C"
{

HVIEWMNEMO API CreateViewMnemo(const char* htmlFileContent);

//  void API Render(HVIEWMNEMO hViewMnemo, DrawLineDelegate drawLine);

//void API Render(HVIEWMNEMO hViewMnemo, IRenderContext renderContext);

void API OnMouseDown(HVIEWMNEMO hViewMnemo);




// Initialize renderer with a callback to draw lines
/*  typedef void (*DrawLineCallback)(int x1, int y1, int x2, int y2, uint32_t color);
    typedef void (*FillRectangeCallback)(WrapperSolidBrush* ssBrush, WrapperRectF* rectF);
    typedef void (*DrawStringCallback)(std::string , byte  length, WrapperFont* font, WrapperRectF* rectF, WrapperSolidBrush* ssBrush);*/

void API InitializeCallbacks(RenderCallbacks _renderCallbacks);
void API InitializeFilesCallbacks(GraphisFilesCallbacks _graphisFilesCallback);
void API RenderFrame(int width, int height);
void API TestRenderDIV();

}

static RenderCallbacks m_RenderCallbacks;
static GraphisFilesCallbacks m_GraphisFilesCallback;

//static FillRectangeCallback s_fillRectangleCallback = nullptr;

#endif

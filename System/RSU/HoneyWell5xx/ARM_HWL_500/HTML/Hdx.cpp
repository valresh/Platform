#include "../StdAfx.h"
#include "Tag.h"
#include "FormatScn.h"
#include "Hdx.h"
#include "HTML.h"
#include "../ReplaceString.h"

#include "Style.h"



CHdx::CHdx()
{
    int n = sizeof(*this);
    int m = sizeof(CStyle);
    memset( this, 0, sizeof(*this) );
    //
    // Параметры отличные от нуля
    colorTxt = 0xffffffff;
    colorBkg = 0xffffffff;
    m_eBorderStyle = eBorderUnk;
    m_nNumberOfChars = -1;
    m_nHDXBINDINGID  = -1;
    m_nPlaceDsd = -1;
    m_nGradientFillStyle =  1;
    m_nBreakpointType    = -1;
    m_dRangeHi = 100;
    m_ptPopup.x = -1;// Координаты
    m_ptPopup.y = -1;// окна
    mBorderImage = -1;
    m_clrFill = 0xffffffff;
    m_clrLine = 0x000000;
    m_bNoLineColor = true;    //нет параметра lineColor
    m_TimeFormat = -1;
    m_bReadOnly = true;
    m_nLineStyle=eLineStyleSolid;
    m_nLineWidth=1;
    m_nPopupCurrentPoint=0;
    m_nPopupGroupParametrs = 0;
    m_alarm = 0;
    m_bParameterFormat = false;
    m_Direction = -1;
    m_bClrFillCanChangedByScript = false;
    m_bChangedByData = false;
    m_PointerDirection = pdRight;
    m_nBorderWidth = 0;
    memset(m_sStyleClass, 0, sizeof(m_sStyleClass));
    memset(m_fontWeight, 0, sizeof(m_fontWeight));
}
//
POINT* CHdx::PT(CHTML& html)
{
    POINT* pt = html.Point(m_nPlacePT);
    return pt;
}
//
char* CHdx::Filtr(CHTML& html)
{
    return html.Buffer(m_nFiltrOfChars);
}
//
char* CHdx::NameID(CHTML& html)
{
    return html.Buffer(mId);
}
//
static char szDebugname[32] = "textbox003";
//
void CHdx::NameID(CHTML& html,char* value)
{
    if ( mId == 0 )
    {
#ifdef _DEBUG
        if(_stricmp(value, szDebugname)==0)
            AAA();
#endif
        mId = AddTexts( html, value );
    }
}
//
char* CHdx::SrcFile(CHTML& html)
{
    return html.Buffer(mSrc);
}
//
void  CHdx::SrcFile(CHTML& html,char* value)
{
    if ( mSrc == 0 )
        mSrc = AddTexts( html, value );
}
//
char* CHdx::PopupName(CHTML& html)
{
    return html.Buffer(m_nPopup);
}
//
char* CHdx::CurrentPointName(CHTML& html)
{
    return html.Buffer(m_nPopupCurrentPoint);
}
//
void  CHdx::PopupName(CHTML& html,char* value)
{
    if ( m_nPopup != 0 ) return;
    //
#ifdef _DEBUG
    if(strstr(value, "PARAM3=P-101/A"))
        AAA();
#endif
    std::string svalue(value);
    svalue = ReplaceString(svalue, "&amp;", "&");
    value = (char*)svalue.c_str();
    //
    CharMP szFile;
    char *ppname = strrchr(value, '\\');
    char *ppparams = strrchr(value, '?');
    if(ppname == NULL)
        if(ppparams == NULL)
            strcpy_s(szFile, value);
        else
        {
            *ppparams = 0;
            strcpy_s(szFile, value);
            *ppparams = '?';
        }
    else
        if(ppparams == NULL)
            strcpy_s(szFile, ppname+1);
        else
        {
            *ppparams = 0;
            strcpy_s(szFile, ppname+1);
            *ppparams = '?';
        }
    //SplitPath(value,0,0,szFile);
    m_nPopup = AddTexts( html, szFile );
    //"C:\ProgramData\Honeywell\Experion PKS\Client\Abstract\popups\popupPid.htm?currentpoint=TCA2065
    std::string strCurrentPointFull=value;
    std::string strFindCurrentPoint="?currentpoint=";
    std::size_t findCurrent = strCurrentPointFull.find(strFindCurrentPoint);
    if (findCurrent!=std::string::npos)
    {
        std::size_t substrBegin=findCurrent+strFindCurrentPoint.length();
        std::size_t substrEnd=strCurrentPointFull.length()-substrBegin;
        std::string strCurrentPointWithSpaces=strCurrentPointFull.substr(substrBegin,substrEnd);
        std::size_t findSpaces = strCurrentPointWithSpaces.find(" ");
        std::string strCurrentPoint=strCurrentPointWithSpaces;
        if (findSpaces!=std::string::npos)
            strCurrentPoint=strCurrentPointWithSpaces.substr(0,findSpaces);
        m_nPopupCurrentPoint= AddTexts( html, strCurrentPoint.c_str() );
    }
    else
    {//группа параметров
        if(char *pp = strchr(value + strlen(szFile), '?'))
            m_nPopupGroupParametrs = AddTexts( html, pp+1);
    }
}
//
void  CHdx::PopupPosX(CHTML& html,char* value)
{
    ASSERT( ::IsInt(value) );
    m_ptPopup.x = atoi(value);
}
//
void  CHdx::PopupPosY(CHTML& html,char* value)
{
    ASSERT( ::IsInt(value) );
    m_ptPopup.y = atoi(value);
}
//
void  CHdx::Positioni(CHTML& html,char* value)
{
    ASSERT( ::IsInt(value) );
    m_nPos = atoi(value);
    ASSERT( m_nPos == 0 );
}
//
COLORREF CHdx::DiscColor(CHTML& html,int n)
{
    COLORREF* pt = html.Color(m_nDiscreteColors);
    return pt[n];
}
//
COLORREF CHdx::ContColor(CHTML& html,int n)
{
    COLORREF* pt = html.Color(m_nContinuousColors);
    return pt[n];
}
//
COLORREF CHdx::Con_Color(CHTML& html,double val)
{
    double* D = html.Doubl(m_nContinuousValues);
    int n = 0;
    if ( D[0] < D[1] )
    {
        for ( n = 0; n < 8; n++ )
            if ( val <= D[n] )
                break;
            else
                if ( n > 0 )
                    if ( D[n-1] >= D[n] )
                        break;
    }
    else if(DOTF_PM==Version)
    {
        if ( val >= D[0] )
            n = 1;
    }
    //
    return ContColor(html,n);
}
//
void CHdx::SetAlias( CHTML& html, char* value )
{
    m_dwFlags |= HDX_ALIAS_TABLE;
    while ( *value )
    {
        char* a = strchr( value, '?' );
        char* b = strchr( value, ':' );
        char* c = strchr( value, ';' );
        ASSERT( a && b );
        if(!a || !b)
            return;
        *a++, *b++ = 0;
        if ( c != NULL ) *c++ = 0;
        int dl = lstrlen(a)+1;
        int ld = lstrlen(b)+1;
        if ( !html.AddChar(dl+ld) )
        {
            ASSERT(0);
            return;
        }
        if ( !html.AddParam(1) )
        {
            ASSERT(0);
            return;
        }
        UINT n = html.CurrParam(1);
        if ( m_nCountPRM == 0 )
            m_nPlacePRM = n;
        //
        SParamsData* list = html.Param(m_nPlacePRM+m_nCountPRM);
        m_nCountPRM++;
        list[0].nName  = AddTexts( html, a, dl );
        list[0].nValue = AddTexts( html, b, ld );
        //
        if ( c == NULL ) break;
        value = c;
    }
}
//
LPCTSTR CHdx::FindAlias( CHTML& html, LPCTSTR value )
{
    SParamsData* list = html.Param(m_nPlacePRM);
    for ( UINT n = 0; n < m_nCountPRM; n++ )
    {
        char* name = html.Buffer(list[n].nName);
        if ( lstrcmpi( value, name ) == 0 )
            return html.Buffer(list[n].nValue);
    }
    return value;
}

void CHdx::DatChange(CHTML& html,char* value)
{
#ifdef _DEBUG
    _static char sss[128][128];
    _static HFILE hFile = _lcreat( "c:/ForDebug/basic.call", 0 );
    if ( hFile != HFILE_ERROR )
    {
        for ( int n = 0; n < 128; n++ )
            if ( lstrcmpi(sss[n],value ) == 0 )
                return;
        _static int N = 0;
        ASSERT( N < 128 );
        lstrcpy( sss[N++], value );
        _hwrite( hFile, value, lstrlen(value) );
        _hwrite( hFile, "\r\n", 2 );
    }
#endif
}
//
#undef  STD_VAR
#define STD_VAR CHTML& html, CHdx& a, char* value, double*& dPoints, BYTE*& bt, SImage& rImg
typedef void (*LPFuncs)( STD_VAR );
//
void hdxSrc( STD_VAR )
{
    if ( !rImg.Yes() )
        LoadImages( value, rImg );
}
//
void hdxFillColorBlink( STD_VAR )
{
    a.mFillColorBlink = IsStrBOL(value);
}
//
void hdxLevelFillColorBlink( STD_VAR )
{
    a.mLevelFillColorBlink = IsStrBOL(value);
    ASSERT( !a.mLevelFillColorBlink );
}
//
void hdxLineColorBlink( STD_VAR )
{
    a.mLineColorBlink = IsStrBOL(value);
    ASSERT( !a.mLineColorBlink );
}
//
void hdxTextColorBlink( STD_VAR )
{
    a.mTextColorBlink = IsStrBOL(value);
    //SergejAMM ASSERT( !a.mTextColorBlink );
}
//
void hdxHeight( STD_VAR )
{
    ASSERT( ::IsInt(value) );
    //a.m_nHeight = atoi(value);
    //a.m_dwFlags |= HDX_HEIGHT;
}
//
void hdxWidth( STD_VAR )
{
    ASSERT( ::IsInt(value) );
    //a.m_nWidth = atoi(value);
    //a.m_dwFlags |= HDX_WIDTH;
}
//
void hdxRotation( STD_VAR )
{
    ASSERT( ::IsFlt(value) );
    a.m_dAngleNew = atof(value);
    if ( a.m_dAngleNew != 0 )
        a.m_dwFlags |= HDX_ROTATE;
}
//
void hdxTotalRotation( STD_VAR )
{
    ASSERT( ::IsFlt(value) );
    REAL m_fAngle = (REAL)atof(value);
    //ASSERT( m_fAngle == 0 );
}
//
void hdxPoints( STD_VAR )
{
    SkipSpace( value );
    int N = 0;
    char* b = value;
    while ( *b )
    {
        N++;
        while ( BYTE(*b) > ' ' ) b++;
        SkipSpace( b );
    }
    ASSERT( N > 0 && ((N % 2) == 0) );
    //
    dPoints = (double*)::BuffAlloc( sizeof(double)*N, 0 );
    if ( dPoints == NULL ) return;
    int _N = N;
    N = 0;
    b = value;
    while ( *b && N < _N )
    {
        char* s = b;
        while ( BYTE(*b) > ' ' ) b++;
        *b++ = 0;
        dPoints[N++] = atof(s);
        SkipSpace( b );
    }
    //
    a.m_nPointPT = N / 2;
    a.m_dwFlags |= HDX_POINTS;
}
//
void hdxPointTypes( STD_VAR )
{
    //Определяет количество полигонов и полилайнов
    //PointTypes:m l l lx m l l lx m l l lx ;
    //PointTypes:m l l l l l l l l l l l l l l l l l lx
    SkipSpace( value );
    cross::CString ss = value;
    ASSERT( a.m_nPointPT > 0 );
    //
    int N = 0, L = 0, M = 0;
    char* b = value;
    ::CharLower( b );
    ASSERT( *b == 'm' );
    while ( *b )
    {
        M++;
        switch ( *b )
        {
        case 'm': N++;
        case 'l':
        case 'c':
            break;
        default:
            ASSERT(0);
            break;
        }
        b++;
        if ( *b == 'x' ) b++;
        SkipSpace( b );
    }
    ASSERT( M == a.m_nPointPT );
    a.m_nPoint10 = N;
    //
    if ( !html.AddParam(a.m_nPoint10) ) return;
    a.m_nPlace10 = html.CurrParam(a.m_nPoint10);
    int* C = html.Int(a.m_nPlace10);
    int* T = C + a.m_nPoint10;
    //
    //
    bt = (BYTE*)::BuffAlloc( a.m_nPointPT, 0 );
    //
    _static int nS = 0;nS++;
    N =-1,L = 0,M = 0;
    b = value;
    while ( *b )
    {
        switch ( *b )
        {
        case 'm': L    = 0; N++; bt[M++] = PT_MOVETO;break;
        case 'l': if ( T[N] != 2 )
                T[N] = 0; L++; bt[M++] = PT_LINETO;break;// line
        case 'c': T[N] = 2; L++; bt[M++] = PT_BEZIERTO;break;// curve
        }
        b++;
        C[N]++;
        if ( *b == 'x' ) T[N] |= 0x10, b++;//Замкнута
        SkipSpace( b );
    }
}
//
void hdxLineWidth( STD_VAR )
{
    ASSERT( ::IsInt(value) );
    a.m_nLineWidth = atoi(value);
    a.m_dwFlags |= HDX_LINE_W;
}
//
void hdxObjectType( STD_VAR )
{
    ASSERT( ::IsInt(value) );
    a.m_nObjectType = atoi(value);
    ASSERT( a.m_nObjectType == 10 );
    DWORD dw = ~TYPE_PAINT;
    a.m_dwTypeObj &= ~TYPE_PAINT;
    a.m_dwTypeObj |=  TYPE_10;
}
//
void hdxRoundness( STD_VAR )
{
    ASSERT( ::IsInt(value) );
    a.m_nRoundness = atoi(value);
    a.m_dwFlags |= HDX_ROUND;
}
//
void hdxLineColor( STD_VAR )
{
    a.m_bNoLineColor = false;
    if ( lstrcmpi( value, "transparent" ) != 0 )
    {
        a.m_clrLine = ColorDiez(value);
        a.m_dwFlags |= HDX_CLRLINE;
        if (( a.m_dwFlags & HDX_CLRFILL) ==0)
        {
            if (a.m_clrFill==0xffffffff)
            {
                a.m_clrFill=0xffffff;
                a.m_dwFlags |= HDX_CLRFILL;
            }
        }
    }
    else
    {
        if ( a.m_dwFlags & HDX_CLRFILL )
            a.m_clrLine = a.m_clrFill, a.m_dwFlags |= HDX_CLRLINE;
        else
            a.m_clrLine = 0xff000000;
    }
}
//
void hdxFillColor( STD_VAR )
{
    if ( lstrcmpi( value, "transparent" ) != 0 )
    {
        a.m_clrFill = ColorDiez(value);
        a.m_dwFlags |= HDX_CLRFILL;
    }
    else
        a.m_clrFill = 0xff000000;
    a.m_clrFillEmpty = a.m_clrFill;
}
//
void hdxTextColor( STD_VAR )
{
    if ( lstrcmpi( value, "transparent" ) != 0 )
    {
        a.m_clrText = ColorDiez(value);
        a.m_dwFlags |= HDX_CLRTEXT;
    }
}
//
void hdxGradientFillColor( STD_VAR )
{
    a.m_clrFillGrd = ColorDiez(value);
    a.m_dwFlags |= HDX_CLRFILLG;
    if ( !(a.m_dwFlags & HDX_CLRFILL) )
        a.m_clrFill = 0xffffff;
}
//
void hdxLevelFillColor( STD_VAR )
{
    a.m_clrFillLvl = ColorDiez(value);
    a.m_dwFlags |= HDX_CLRFILLL;
}
//
void hdxGradientFillStyle( STD_VAR )
{
    ASSERT( ::IsInt(value) );
    ASSERT( lstrcmpi( value, "0" ) == 0 || lstrcmpi( value, "1" ) == 0 );
    a.m_nGradientFillStyle = atoi(value);
    ASSERT( a.m_nGradientFillStyle < 2 );
}
//
void hdxGradientFillFocus( STD_VAR )
{
    ASSERT( ::IsInt(value) );
    int i = atoi(value);
    ASSERT( i == 70 || i == 20 || i == 50 || i == 0 || i == 100 || i == 25 || i == 30 || i == 35 );
}
//
void hdxFillStyle( STD_VAR )
{
    ASSERT( IsInt(value) );
    a.m_nFillStyle = atoi(value);
    a.m_dwFlags |= HDX_FILL_S;
    ASSERT( 0 <= a.m_nFillStyle && a.m_nFillStyle < 3 || a.m_nFillStyle == 8 );
}
//
void hdxLineStyle( STD_VAR )
{
    ASSERT( ::IsInt(value) );
    a.m_nLineStyle = atoi(value);
    a.m_dwFlags |= HDX_LINE_S;
}
//
void hdxQuadrant( STD_VAR )
{
    ASSERT( ::IsInt(value) );
    a.m_nQuadrant = atoi(value);
    a.m_dwFlags |= HDX_QUADRANT;
    ASSERT( a.m_nQuadrant == 0 || a.m_nQuadrant == 1 || a.m_nQuadrant == 2 || a.m_nQuadrant == 3 );
}
//
void hdxBreakpointType( STD_VAR )
{
    ASSERT( ::IsInt(value) );
    a.m_nBreakpointType = atoi(value);
    ASSERT( 0 <= a.m_nBreakpointType && a.m_nBreakpointType <= 1 );
    // 0 - ContinuousColors, ContinuousValues
    // 1 - DiscreteColors;
    // Сссуки - это поле установили, а данные нет.
}
//
void hdxDiscreteColors( STD_VAR )
{
    if ( !html.AddParam(4) ) return;
    a.m_nDiscreteColors = html.CurrParam(4);
    COLORREF* C = html.Color(a.m_nDiscreteColors);
    //
    int n = 0;
    while ( *value )
    {
        char* ptr = strchr( value, '_' );
        if ( ptr ) *ptr = 0;
        //
        ASSERT( n < 8 );
        int N = atoi( value );
        C[n++] = N;
        //
        if ( ptr == 0 ) break;
        value = ptr+1;
    }
    ASSERT( n == 8 );
    a.m_dwFlags |= HDX_DISK_COLORS;
}
//
void hdxDiscreteStates( STD_VAR )
{
    // Инверсия цветов ???
    ASSERT( ::IsInt(value) );
    a.m_nDiscreteStates = atoi(value);
}
//
void hdxHDXBINDINGID( STD_VAR )
{
    ASSERT( ::IsInt(value) );
    a.m_dwFlags |= HDX_BINDING;
    a.m_nHDXBINDINGID = atoi(value);
}
//
void hdxIndicateAlarmState( STD_VAR )
{
    if ( lstrcmpi( value, "True" ) == 0 || lstrcmpi( value, "1" ) == 0 )
        a.m_dwFlags |= HDX_ALMSTATE;
    else
        if ( lstrcmpi( value, "False" ) == 0 || lstrcmpi( value, "0" ) == 0 )
        {}
        else
            if ( lstrcmpi( value, "-1" ) == 0 )
            {
                a.m_dwFlags |= HDX_ALMSTATEMINUS1;
            }
            else
            {
                ASSERT(0);
            }
}
//
void hdxIndicateScanState( STD_VAR )
{
    if ( lstrcmpi( value, "1" ) == 0 || lstrcmpi( value, "true" ) == 0 )
        a.m_dwFlags |= HDX_SCNSTATE;
    else
        if ( lstrcmpi( value, "0" ) == 0 || lstrcmpi( value, "false" ) == 0 )
        {}
        else
            if ( lstrcmpi( value, "-1" ) == 0 )
            {
            }
            else
            {
                ASSERT(0);
            }
}
//
void hdxIndicateBadState( STD_VAR )
{
    ASSERT( lstrcmpi( value, "-1" ) == 0 || lstrcmpi( value, "0" ) == 0 );
}
//
void hdxIndicateNaNState( STD_VAR )
{
    ASSERT( ::IsInt(value) );
    int i = atoi(value);
    ASSERT( i == -1 || i == 0 );
}
//
void hdxEndArrow( STD_VAR )
{
    ASSERT( ::IsInt(value) );
    int i = atoi(value);
    a.m_nEndArrow = i;
    ASSERT( 0 <= i && i < 4 );
}
//
void hdxStartArrow( STD_VAR )
{
    ASSERT( ::IsInt(value) );
    int i = atoi(value);
    a.m_nStartArrow = i;
    ASSERT( 0 <= i && i < 4 );
}
//
void hdxLevelFilled( STD_VAR )
{
    a.mLevelFilled = IsStrBOL(value);
}
//
void hdxFillDirection( STD_VAR )
{
    ASSERT( ::IsInt(value) );
    int i = atoi(value);
    ASSERT( i == 0 || i == 1 );
    a.fillDirection = i;
}
//
void hdxVisibleRangeHi( STD_VAR )
{
    ASSERT( ::IsFlt(value) );
    a.m_dRangeHi = atof(value);
    a.m_dwFlags |= HDX_RANGELOHI;
}
//
void hdxVisibleRangeLo( STD_VAR )
{
    ASSERT( ::IsFlt(value) );
    a.m_dRangeLo = atof(value);
    a.m_dwFlags |= HDX_RANGELOHI;
}
//
void hdxRangeOrigin( STD_VAR )
{
    ASSERT( ::IsFlt(value) );
    a.m_dRangeOrigin = atof(value);
    a.m_dwFlags |= HDX_RANGEORIGIN;
}
//
void hdxBorderStyle( STD_VAR )
{
    if ( lstrcmpi( value, "inset" ) == 0 )
        a.m_eBorderStyle = eBorderInset;
    else
        if ( lstrcmpi( value, "outset" ) == 0 )
            a.m_eBorderStyle = eBorderOutset;
        else
            if ( lstrcmpi( value, "solid" ) == 0 )
                a.m_eBorderStyle = eBorderSolid;
            else
                if ( lstrcmpi( value, "Dashed" ) == 0 )
                    a.m_eBorderStyle = eBorderDashed;
                else
                    if ( lstrcmpi( value, "None" ) == 0 )
                    {}//a.m_eBorderStyle = eBorderOutset;
                    else
                    {
                        ASSERT(0);
                    }
}
//
void hdxBorderWidth( STD_VAR )
{
    a.m_nBorderWidth = Number( value );
    if ( *value )
    {
        while ( *value == ' ' ) value++;
        ASSERT( lstrcmpi( value, "px" ) == 0 );
    }
}
//
void hdxNumberOfChars( STD_VAR )
{
    ASSERT( ::IsInt(value) );
    a.m_nNumberOfChars = atoi(value);
}
//
void hdxNumericDisplayFormat( STD_VAR )
{
    a.m_nFiltrOfChars = a.AddTexts( html, value );
}
//
void hdxCurrentLevelPageHeight( STD_VAR )
{
    ASSERT( ::IsInt(value) );
    int nHeight = atoi(value);
    //SergejAmm ASSERT( nHeight == 797 );
}
//
void hdxCurrentLevelPageWidth( STD_VAR )
{
    ASSERT( ::IsInt(value) );
    int nWidth = atoi(value);
    //SergejAmm ASSERT( nWidth == 1276 );
}
//
void hdxTopLevelNumberOfFrames( STD_VAR )
{
    ASSERT( lstrcmpi( value, "0" ) == 0 );
}
//
void hdxTopLevelPageHeight( STD_VAR )
{
    ASSERT( ::IsInt(value) );
    int nHeight = atoi(value);
    //SergejAmm ASSERT( nHeight == 797 );
}
//
void hdxTopLevelPageWidth( STD_VAR )
{
    ASSERT( ::IsInt(value) );
    int nWidth = atoi(value);
    //SergejAmm ASSERT( nWidth == 1276 );
}
//
void hdxContinuousColors( STD_VAR )
{
    if ( !html.AddParam(5) ) return;
    a.m_nContinuousColors = html.CurrParam(5);
    COLORREF* C = html.Color(a.m_nContinuousColors);
    //
    int n = 0;
    while ( *value )
    {
        char* ptr = strchr( value, '_' );
        if ( ptr ) *ptr = 0;
        //
        ASSERT( n < 9 );
        int N = atoi( value );
        C[n++] = N;
        //
        if ( ptr == 0 ) break;
        value = ptr+1;
    }
    ASSERT( n == 9 );
    a.m_dwFlags |= HDX_CONT_COLORS;
}
//
void hdxContinuousValues( STD_VAR )
{
    if ( !html.AddParam(8) ) return;
    a.m_nContinuousValues = html.CurrParam(8);
    double* D = html.Doubl(a.m_nContinuousValues);
    int n = 0;
    if (Version==AT_PRP || Version==UKR_PRP)
    {// у клапанов почему-то были неверные значения на схеме P-101
        if (lstrcmp(value,"2_0_0_0_0_0_0_0")==0)
            lstrcpy(value,"2_100_0_0_0_0_0_0");
    }
    while ( *value )
    {
        char* ptr = strchr( value, '_' );
        if ( ptr ) *ptr = 0;
        //
        ASSERT( n < 8 );
        double N = atof( value );
        D[n++] = N;
        //
        if ( ptr == 0 ) break;
        value = ptr+1;
    }
    ASSERT( n == 8 );
}
//
void hdxStyleClass( STD_VAR )
{
    if ( !*value ) return;

    EStyleClass sc =  GiveClass( value );
    if(sc != clsUnknown || Version < BERTH_ONPZ)
        a.mClass = sc;
    strcpy_s(a.m_sStyleClass, value);
    if (Version==Реформинг_КНПЗ)
    {
        if ((lstrcmp(value,"ConAlpSP")==0)||(lstrcmp(value,"ConAlpOP")==0)
            ||(lstrcmp(value,"RecFrameEquipment")==0))
            a.AddFlag(TYPE_FACEPLATE);
    }
}
void hdxInverseLogic( STD_VAR )
{
    if (value)
    {
        a.m_inverseLogic = atoi(value);
    }
}
//
void hdxValue( STD_VAR )
{
    if ( a.mClass == clsTagEuDesc )
        a.mClass = clsTagName;
    //
    if ( *value )
        AAA();
}
//
void hdxTimeFormat( STD_VAR )
{
    a.m_TimeFormat = atoi(value);
}
//
void hdxPointerDirection( STD_VAR )
{
    a.m_PointerDirection = (PoinDirect)atoi(value);
}
//
void hdxDirection( STD_VAR )
{
    a.m_Direction = atoi(value);
}

//
void CHdx::AnalizHDX( CHTML& html, char* ptr, SImage& rImg )
{
    char* id = NameID( html );
    if ( lstrcmp( id, "polygon002" ) == 0 )
        AAA();
    //
    int N;
    STegScn* teg = theApp.ParsEXT( ptr, N );
    ASSERT( N > 0 );
    //
    struct SLocal {  char* name; LPFuncs func; };
    _static SLocal list[] =
        {
         { "fillColorBlink"        , hdxFillColorBlink         },// False
         { "lineColorBlink"        , hdxLineColorBlink         },// False
         { "textColorBlink"        , hdxTextColorBlink         },// False
         { "Height"                , hdxHeight                 },// 5
         { "Width"                 , hdxWidth                  },// 35
         { "points"                , hdxPoints                 },// Массив
         { "Src"                   , hdxSrc                    },// Имя файла
         { "Rotation"              , hdxRotation               },// Вращение
         { "TotalRotation"         , hdxTotalRotation          },// Вращение
         { "LineWidth"             , hdxLineWidth              },//
         { "LineColor"             , hdxLineColor              },//
         { "FillColor"             , hdxFillColor              },//
         { "TextColor"             , hdxTextColor              },//
         { "FillStyle"             , hdxFillStyle              },//
         { "LineStyle"             , hdxLineStyle              },//
         { "Quadrant"              , hdxQuadrant               },//
         { "ObjectType"            , hdxObjectType             },//
         { "PointTypes"            , hdxPointTypes             },//
         { "Roundness"             , hdxRoundness              },//
         { "GradientFillColor"     , hdxGradientFillColor      },//
         { "GradientFillStyle"     , hdxGradientFillStyle      },//
         { "GradientFillFocus"     , hdxGradientFillFocus      },//
         { "levelFillColor"        , hdxLevelFillColor         },//
         { "borderStyle"           , hdxBorderStyle            },//
         { "borderWidth"           , hdxBorderWidth            },//
         { "EndArrow"              , hdxEndArrow               },//
         { "StartArrow"            , hdxStartArrow             },//
         { "CurrentLevelPageHeight", hdxCurrentLevelPageHeight },//
         { "CurrentLevelPageWidth" , hdxCurrentLevelPageWidth  },//
         { "TopLevelNumberOfFrames", hdxTopLevelNumberOfFrames },//
         { "TopLevelPageHeight"    , hdxTopLevelPageHeight     },//
         { "TopLevelPageWidth"     , hdxTopLevelPageWidth      },//
         { "BreakpointType"        , hdxBreakpointType         },//
         { "DiscreteColors"        , hdxDiscreteColors         },//
         { "DiscreteStates"        , hdxDiscreteStates         },//
         { "HDXBINDINGID"          , hdxHDXBINDINGID           },//
         { "indicateAlarmState"    , hdxIndicateAlarmState     },//
         { "indicateScanState"     , hdxIndicateScanState      },//
         { "indicateBadState"      , hdxIndicateBadState       },//
         { "indicateNaNState"      , hdxIndicateNaNState       },//
         { "levelFillColorBlink"   , hdxLevelFillColorBlink    },//
         { "numberOfChars"         , hdxNumberOfChars          },//
         { "numericDisplayFormat"  , hdxNumericDisplayFormat   },//
         { "ContinuousColors"      , hdxContinuousColors       },//
         { "ContinuousValues"      , hdxContinuousValues       },//
         { "styleClass"            , hdxStyleClass             },
         { "fillDirection"         , hdxFillDirection          },
         { "visiblerangeHi"        , hdxVisibleRangeHi         },
         { "visiblerangeLo"        , hdxVisibleRangeLo         },
         //  { "positiveDicross::RECTion"     , hdxPositiveDicross::RECTion      },
         { "rangeOrigin"           , hdxRangeOrigin            },
         { "value"                 , hdxValue                  },
         { "inverseLogic"          , hdxInverseLogic           },
         { "LevelFilled"           , hdxLevelFilled            },
         { "timeFormat"            , hdxTimeFormat             },
         { "direction"             , hdxDirection              },
         { "pointerDirection"      , hdxPointerDirection   },

         }InitLocal;
    //
    double* dPoints = NULL;
    BYTE  * bt      = NULL;
    for ( int n = 0; n < N; n++ )
    {
        SLocal* find = FindLocal( teg[n].name );
        //  ASSERT( find );
        if ( find != NULL )
            (*find->func)(html,*this,teg[n].value,dPoints,bt,rImg);
    }
    //
    if ( (m_dwFlags & HDX_POINTS) && dPoints != NULL )
    {
        m_dwFlags &= ~HDX_CLRTEXT; //у графики нет цвета текста, а HDX_CLRTEXT используется для отключения управления цветом в CTag::OutDbl, CTag::OutChr, CTag::OutInt

        if (Version == UKR_PRP &&
            (!html.m_strFile.CompareNoCase("MNEMO309") || !html.m_strFile.CompareNoCase("MNEMO318") || !html.m_strFile.CompareNoCase("SCREEN7")) &&
            (!lstrcmp( id, "polygon001" )||!lstrcmp( id, "polygon002" )) &&
            mRect.Width == 70)
        {
            mRect.Width = 80;
        }
        if (Version == UKR_PRP &&
            !html.m_strFile.CompareNoCase("MNEMO351") &&
            mRect.Width == 40)
        {
            mRect.Width = 60;
            mRect.Height = 45;
        }
        // +1 для завершающей точки, если полигон
        // придётся выводить как полилайн.
        int nPoint = (m_nPointPT+1)*2;
        if ( !html.AddParam(nPoint) ) return;
        m_nPlacePT = html.CurrParam(nPoint);
        POINT* pt = PT(html);
        for ( int n = 0; n < m_nPointPT; n++ )
        {
            pt[n].x = long(mRect.X + mRect.Width *dPoints[2*n  ]*0.01+0.5);
            pt[n].y = long(mRect.Y + mRect.Height*dPoints[2*n+1]*0.01+0.5);
        }
        ::BuffFree( dPoints );
        if ( bt )
        {
            BYTE*  tb = (BYTE*)(pt + m_nPointPT+1);
            memcpy( tb, bt, m_nPointPT );
            ::BuffFree( bt );
        }
    }
    //
    if ( m_eBorderStyle != eBorderUnk )
    {
        m_dwFlags |= HDX_CLRLINE;
        m_dwFlags -= HDX_CLRLINE;
    }
    // Предохраняемся
    if ( m_nBreakpointType == 0 && (m_dwFlags & HDX_CONT_COLORS) == 0 )
        m_nBreakpointType = -1;
    else
        if ( m_nBreakpointType == 1 && (m_dwFlags & HDX_DISK_COLORS) == 0 )
            m_nBreakpointType = -1;
    //
}
// Компенсация поворота
#define PI 3.14159265
void CHdx::Angles()
{
    if ( m_dwFlags & HDX_ROTATE )
    {
        Matrix mat;
        mat.Rotate( (REAL)m_dAngleNew );
        mat.GetElements((REAL*)&m_XF);
        double cx = mRect.X+mRect.Width /2;
        double cy = mRect.Y+mRect.Height/2;

        if(Version != LG35_8_KF)
        {
            if(mRect.Height ==0)  //похоже так везде
                cy+=8;
        }

        m_XF.eDx = REAL((1.-m_XF.eM11)*cx - m_XF.eM21*cy);
        m_XF.eDy = REAL((1.-m_XF.eM22)*cy - m_XF.eM12*cx);

        /*так по-науке
  XFORM XForm, XForm1, XForm2, XForm3;
  double sn = sin(PI*m_dAngleNew/180.);
  double cs = cos(PI*m_dAngleNew/180.);

  // normalize coordinates by rotation center
  // нормализация координат по точке вращения
  XForm1.eM11 = 1.0; XForm1.eM12 = 0.0;
  XForm1.eM21 = 0.0; XForm1.eM22 = 1.0;
  XForm1.eDx = -cx; XForm1.eDy = -cy;

  // rotation matrix
  // матрица поворота
  XForm2.eM11 =  cs; XForm2.eM12 = sn;
  XForm2.eM21 = -sn; XForm2.eM22 = cs;
  XForm2.eDx = 0.0;  XForm2.eDy = 0.0;

  // transfer to the rotation center
  // перенос в точку вращения
  XForm3.eM11 = 1.0; XForm3.eM12 = 0.0;
  XForm3.eM21 = 0.0; XForm3.eM22 = 1.0;
  XForm3.eDx = cx;  XForm3.eDy = cy;

  // multiply matrices in the required order
  // перемножение матриц в требуемом порядке
  CombineTransform(&XForm, &XForm1, &XForm2);
  CombineTransform(&XForm, &XForm, &XForm3);
  m_XF = XForm;
*/
        m_dAngleOld = m_dAngleNew;
    }
}
//

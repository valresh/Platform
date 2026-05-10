#ifndef GDI_H
#define GDI_H

#include "crossplatform.h"
#include <vector>
#include <map>
//#include <tchar.h>
#include <QBrush>
#include <QPen>
#include <QFont>

#define HBRUSH QBrush *
#define HPEN QPen *
#define HFONT QFont *
struct BRUSH_DATA
{
    BRUSH_DATA (BOOL bTextColor = FALSE) : m_nType (0), m_cr (0), m_nModelID (-1), m_bTextColor (bTextColor) {}
    BRUSH_DATA (int nModelID, int nType, COLORREF cr, BOOL bTextColor = FALSE) : m_nModelID (nModelID), m_nType (nType), m_cr (cr), m_bTextColor (bTextColor) {}
    BRUSH_DATA (const BRUSH_DATA& srcData)
    {
      Copy (srcData);
    }
    ~BRUSH_DATA () {}

    int m_nModelID;
    int m_nType;
    COLORREF m_cr;
    BOOL m_bTextColor;

    virtual void Copy (const BRUSH_DATA& srcData)
    {
      m_nModelID = srcData.m_nModelID;
      m_nType = srcData.m_nType;
      m_cr = srcData.m_cr;
      m_bTextColor = srcData.m_bTextColor;
    }
};

struct PEN_DATA : BRUSH_DATA
{
    PEN_DATA () : m_nSize (0) {}
    PEN_DATA (int nModelID, int nType, COLORREF cr, int nSize) : BRUSH_DATA (nModelID, nType, cr), m_nSize (nSize) {}
    PEN_DATA (const PEN_DATA& srcData) : BRUSH_DATA ((const BRUSH_DATA&)srcData)
    {
      Copy (srcData);
    }
    virtual ~PEN_DATA () {}

    int m_nSize;

    virtual void Copy (const PEN_DATA& srcData)
    {
      m_nSize = srcData.m_nSize;
    }
};
struct FONT_DATA : PEN_DATA
{
    FONT_DATA (int nModelID, int nType, COLORREF cr, int nSize, BOOL bBold, LPCTSTR szFaceName) :
      PEN_DATA (nModelID, nType, cr, nSize), m_bBold (bBold), m_hObject (NULL)
    {
      strcpy_s (m_szFaceName, LF_FACESIZE, szFaceName);
      m_nOrigSize = nSize;
    }
    FONT_DATA (const FONT_DATA& srcData) : PEN_DATA ((const PEN_DATA&)srcData)
    {
      Copy (srcData);
    }

    virtual ~FONT_DATA ();

    virtual void Copy (const FONT_DATA& srcData)
    {
      m_bBold = srcData.m_bBold;
      m_nOrigSize = srcData.m_nOrigSize;
      strcpy_s (m_szFaceName, LF_FACESIZE, srcData.m_szFaceName);
      m_nRef = -1;
      m_hObject = NULL;
    }

    void Destroy ()
    {
      if ( m_hObject )
      {
        //??        ::DeleteObject (m_hObject);
        m_hObject = NULL;
      }
    }

    int m_nOrigSize;
    int m_nRef;
    HFONT m_hObject;
    BOOL m_bBold;
    TCHAR m_szFaceName[LF_FACESIZE];

    BOOL IsEqual (int nSize, BOOL bBold, LPCTSTR szFaceName)
    {
      return (nSize == m_nSize && bBold == m_bBold && strcmp (szFaceName, m_szFaceName) == 0);
    }

    BOOL Create ();
};

struct comparer
{
  public:
    bool operator()(LPCTSTR x, LPCTSTR y) const
    {
      return (strcmp (x, y) < 0);
    }
};

enum GDI_RESOURCE_TYPE
{
  GDI_RESOURCE_BRUSH,
  GDI_RESOURCE_PEN,
  GDI_RESOURCE_FONT,
  GDI_RESOURCE_TEXTCOLOR
};



template <class T> class GDIData
{
  public:
    GDIData () {}

    static BOOL HasFonts (int nModelID, std::vector<T *>& v)
    {
      for (int i=0; i<(int)v.size(); i++)
      {
        T* pdata = v[i];
        if ( pdata->m_nModelID == nModelID )
          return TRUE;
      }
      return FALSE;
    }
    static int HasResources (int nModelID, std::vector<T>& v)
    {
      for (int i=0; i<(int)v.size(); i++)
      {
        T& data = v[i];
        if ( data.m_nModelID == nModelID )
          return TRUE;
      }
      return FALSE;
    }
    static int FindFontData (int nModelID, int nType, std::vector<T *>& v)
    {
      for (int i=0; i<(int)v.size(); i++)
      {
        T* pdata = v[i];
        if ( pdata->m_nModelID == nModelID && pdata->m_nType == nType )
          return i;
      }
      return -1;
    }
    static int FindData (int nModelID, int nType, std::vector<T>& v)
    {
      for (int i=0; i<(int)v.size(); i++)
      {
        T& data = v[i];
        if ( data.m_nModelID == nModelID && data.m_nType == nType )
          return i;
      }
      return -1;
    }
    static void SetColor (int nModelID, int nType, std::vector<T>& v, COLORREF cr)
    {
      int nIndex = FindData (nModelID, nType, v);
      if ( nIndex >= 0 )
      {
        v[nIndex].m_cr = cr;
      }
    }
};

typedef std::vector<BRUSH_DATA> brushArray;
typedef std::vector<PEN_DATA> penArray;
typedef std::vector<FONT_DATA> fontArray;
typedef std::map<LPCTSTR, int, comparer> modelMap;
typedef std::pair <LPCTSTR, int> mapPair;
typedef modelMap::iterator model_it_type;

class CGDIResourceMgr
{
  public:
    CGDIResourceMgr ();
    ~CGDIResourceMgr ();


    void AddBrush (int nType, COLORREF cr);
    void AddTextColor (int nType, COLORREF cr);
    void AddPen (int nType, COLORREF cr, int nSize);
    void SetBrushPenColor (LPCTSTR szModel, BOOL bPen, int nType, COLORREF cr);
    HBRUSH GetBrush (int nModelID, int nType);
    COLORREF GetTextColor (int nModelID, int nType);
    HPEN GetPen (int nModelID, int nType);
    int GetModelID (LPCTSTR szModel);
    BOOL AddModel (LPCTSTR szModel);
    void Clear ();
    void AddFont (int nType, COLORREF cr, int nSize, BOOL bBold, LPCTSTR szFaceName);
    void ScaleFonts (double dZoom);
    int FindModelFont (int nModelID, int nType);
    void SetFontColor (int nModelID, int nType, COLORREF cr);
    HFONT GetFont (int nModelID, int nType, COLORREF *pcr = NULL);
    BOOL HasResources (LPCTSTR szModel);
    modelMap& GetModels () { return m_map; }
    brushArray& GetBrushes () { return m_brushes; }
    penArray& GetPens () { return m_pens; }
    std::vector<FONT_DATA*>& GetFonts () { return m_fonts; }

  protected:
    brushArray m_brushes;
    penArray m_pens;
    std::vector<FONT_DATA*> m_fonts;
    modelMap m_map;
    int m_nCurModelID;

    int FindFont (int nSize, BOOL bBold, LPCTSTR szFaceName);
};

//class __declspec(dllexport) CGDIResourceMgr;


#endif // GDI_H

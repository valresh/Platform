#include "GDI.h"
//#include <locale.h>
#include <math.h>

#define MIN_FONT_SIZE	4

FONT_DATA::~FONT_DATA ()
{
  Destroy ();
}

BOOL FONT_DATA::Create ()
{
  Destroy ();
  if ( m_nSize >= MIN_FONT_SIZE )
  {
    // m_hObject = ::CreateFont (m_nSize, 0, 0, 0, m_bBold ? FW_BOLD : FW_REGULAR, 0, 0, 0,
    //                          ANSI_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS,
    //                          ANTIALIASED_QUALITY, VARIABLE_PITCH, m_szFaceName);
  }
  return (m_hObject != NULL);
}

CGDIResourceMgr::CGDIResourceMgr () : m_nCurModelID (-1)
{
  //setlocale(LC_ALL, "");
}

CGDIResourceMgr::~CGDIResourceMgr ()
{
  Clear ();
}

void CGDIResourceMgr::AddBrush (int nType, COLORREF cr)
{
  if ( m_nCurModelID < 0 )
    return;
  m_brushes.push_back (BRUSH_DATA (m_nCurModelID, nType, cr));
}

void CGDIResourceMgr::AddTextColor (int nType, COLORREF cr)
{
  if ( m_nCurModelID < 0 )
    return;
  m_brushes.push_back (BRUSH_DATA (m_nCurModelID, nType, cr, TRUE));
}

void CGDIResourceMgr::AddPen (int nType, COLORREF cr, int nSize)
{
  if ( m_nCurModelID < 0 )
    return;
  m_pens.push_back (PEN_DATA (m_nCurModelID, nType, cr, nSize));
}

void CGDIResourceMgr::SetBrushPenColor (LPCTSTR szModel, BOOL bPen, int nType, COLORREF cr)
{
  int nModelID = GetModelID (szModel);
  if ( nModelID < 0 )
    return;
  if ( bPen )
    GDIData<PEN_DATA>::SetColor (nModelID, nType, m_pens, cr);
  else
    GDIData<BRUSH_DATA>::SetColor (nModelID, nType, m_brushes, cr);
}

HBRUSH CGDIResourceMgr::GetBrush (int nModelID, int nType)
{
  int nIndex = (nModelID >= 0) ? GDIData<BRUSH_DATA>::FindData (nModelID, nType, m_brushes) : -1;
  if ( nIndex >= 0 )
  {
    BRUSH_DATA& data = m_brushes[nIndex];
    return NULL;
//    return ::CreateSolidBrush (data.m_cr);
  }
  return NULL;
//  return (HBRUSH)::GetStockObject (NULL_BRUSH);
}

COLORREF CGDIResourceMgr::GetTextColor (int nModelID, int nType)
{
  int nIndex = (nModelID >= 0) ? GDIData<BRUSH_DATA>::FindData (nModelID, nType, m_brushes) : -1;
  if ( nIndex >= 0 )
  {
    BRUSH_DATA& data = m_brushes[nIndex];
    return data.m_cr;
  }
  return RGB (0, 0, 0);
}

HPEN CGDIResourceMgr::GetPen (int nModelID, int nType)
{
  int nIndex = (nModelID >= 0) ? GDIData<PEN_DATA>::FindData (nModelID, nType, m_pens) : -1;
  if ( nIndex >= 0 )
  {
    PEN_DATA& data = m_pens[nIndex];
    return NULL;
//    return ::CreatePen (PS_SOLID, data.m_nSize, data.m_cr);
  }
  return NULL;
//  return (HPEN)::GetStockObject (NULL_PEN);
}

void CGDIResourceMgr::Clear ()
{
  int nCount = (int)m_fonts.size ();
  for (int i=0; i<nCount; i++)
  {
    FONT_DATA *pData = m_fonts[i];
    delete pData;
  }
  m_fonts.clear ();
}

void CGDIResourceMgr::ScaleFonts (double dZoom)
{
  int nCount = (int)m_fonts.size ();
  for (int i=0; i<nCount; i++)
  {
    FONT_DATA *pData = m_fonts[i];
    if ( pData->m_nRef >= 0 || pData->m_hObject == NULL )
      continue;
    int nNewSize = (int)ceil ((double)(pData->m_nOrigSize) * dZoom);
    if ( nNewSize == pData->m_nSize )
      continue;
    pData->m_nSize = nNewSize;
    pData->Create ();
  }
}

int CGDIResourceMgr::FindModelFont (int nModelID, int nType)
{
  return (nModelID >= 0) ? GDIData<FONT_DATA>::FindFontData (nModelID, nType, m_fonts) : -1;
}

int CGDIResourceMgr::FindFont (int nSize, BOOL bBold, LPCTSTR szFaceName)
{
  int nCount = (int)m_fonts.size ();
  for (int i=0; i<nCount; i++)
  {
    FONT_DATA *pData = m_fonts[i];
    if ( pData->IsEqual (nSize, bBold, szFaceName) )
      return i;
  }
  return -1;
}

void CGDIResourceMgr::AddFont (int nType, COLORREF cr, int nSize, BOOL bBold, LPCTSTR szFaceName)
{
  if ( m_nCurModelID < 0 )
    return;
  FONT_DATA *pData = new FONT_DATA (m_nCurModelID, nType, cr, nSize, bBold, szFaceName);
  int nIndex = FindFont (nSize, bBold, szFaceName);
  if ( nIndex >= 0 )
    pData->m_nRef = nIndex;
  else
    pData->Create ();
  m_fonts.push_back (pData);
}

void CGDIResourceMgr::SetFontColor (int nModelID, int nType, COLORREF cr)
{
  int nIndex = FindModelFont (nModelID, nType);
  if ( nIndex >= 0 )
  {
    FONT_DATA *pData = m_fonts[nIndex];
    pData->m_cr = cr;
  }
}

HFONT CGDIResourceMgr::GetFont (int nModelID, int nType, COLORREF *pcr)
{
  HFONT hFont = NULL;
  int nIndex = FindModelFont (nModelID, nType);
  FONT_DATA *pData = (nIndex >= 0) ? m_fonts[nIndex] : NULL;
  if ( pData )
  {
    if ( pcr )
      *pcr = pData->m_cr;
    if ( pData->m_nRef >= 0 && pData->m_nRef < (int)m_fonts.size () )
      pData = m_fonts[pData->m_nRef];
    if ( pData )
      hFont = pData->m_hObject;
  }
  return hFont;
}

int CGDIResourceMgr::GetModelID (LPCTSTR szModel)
{
//  setlocale (LC_COLLATE, "Russian");
  modelMap::iterator it = m_map.find (szModel);
  return (it != m_map.end()) ? (int)(*it).second : -1;
}

BOOL CGDIResourceMgr::AddModel (LPCTSTR szModel)
{
  if ( GetModelID (szModel) >= 0 )
    return FALSE;
  m_nCurModelID ++;
  m_map.insert (mapPair (szModel, m_nCurModelID));
  return TRUE;
}

BOOL CGDIResourceMgr::HasResources (LPCTSTR szModel)
{
  int nModelID = GetModelID (szModel);
  if ( nModelID < 0 )
    return FALSE;
  if ( GDIData<BRUSH_DATA>::HasResources (nModelID, m_brushes) )
    return TRUE;
  if ( GDIData<PEN_DATA>::HasResources (nModelID, m_pens) )
    return TRUE;
  return GDIData<FONT_DATA>::HasFonts (nModelID, m_fonts);
}

void AddBrush(CGDIResourceMgr *pResMgr, int nType, COLORREF cr)
{
  pResMgr->AddBrush (nType, cr);
}

void AddTextColor(CGDIResourceMgr *pResMgr, int nType, COLORREF cr)
{
  pResMgr->AddTextColor(nType, cr);
}

void AddPen(CGDIResourceMgr *pResMgr, int nType, COLORREF cr, int nSize)
{
  pResMgr->AddPen(nType, cr, nSize);
}

void AddFont(CGDIResourceMgr *pResMgr, int nType, COLORREF cr, int nSize, BOOL bBold, LPCTSTR szFaceName)
{
  pResMgr->AddFont(nType, cr, nSize, bBold, szFaceName);
}

HBRUSH GetBrush(CGDIResourceMgr *pResMgr, int nModelID, int nType)
{
  return pResMgr->GetBrush (nModelID, nType);
}

COLORREF GetTextColor(CGDIResourceMgr *pResMgr, int nModelID, int nType)
{
  return pResMgr->GetTextColor (nModelID, nType);
}

HPEN GetPen(CGDIResourceMgr *pResMgr, int nModelID, int nType)
{
  return pResMgr->GetPen (nModelID, nType);
}

HFONT GetFont(CGDIResourceMgr *pResMgr, int nModelID, int nType, COLORREF *pcr)
{
  return pResMgr->GetFont (nModelID, nType, pcr);
}

#include "CnfReader.h"
//#include <atlbase.h>
#include <macros/AutoCloser.h>
#include <vector>
#include <macros/StrHelps.h>
#include <algorithm>
#include <list>
//#include <macros/CsSync.h>
#include <macros/FileFind.h>
#include <fstream>
// #include <crossplatform.h>
// #include <crosspath.h>
// #include <crosslock.h>
// #include <crossstring.h>
#include <cassert>
//#include "crosslocale.h"
#include <filesystem>
#include <rsu_cfg_ldr.h>

using namespace std;

static std::recursive_mutex s_csOnContour;
static DWORD m_ActiveThreadsCount = 0;
#undef fopen


KCnfReader::KCnfReader(void)
: m_MaskRead( IBlock::emrMinimum )
{
}

KCnfReader::~KCnfReader(void)
{
}

size_t _FileToBuff( const char* pszFileName, std::string &buf )
{
  size_t szBuffer = 0;
  FILE* pFile = fopen(pszFileName, "rb");
  if ( pFile != NULL )
  {
    fseek(pFile, 0, SEEK_END);
    long lSize = ftell(pFile);
    rewind(pFile);
    buf.resize( lSize );
    szBuffer = lSize;
    if( szBuffer != 0 )
    {
      size_t lRead = fread((void*)buf.data(), 1, lSize, pFile);
      if( lRead != lSize )
      {
        buf.clear();
        szBuffer = 0;
      }
      else
      {
        //szBuffer[lSize] = '\0';
      }
    }
    fclose( pFile );
  }
  return szBuffer;
}

void KCnfReader::PrepareUTF8(LPCSTR pszXml)
{
#if 0 //???
    using namespace std;
    string sbuf;
    if (0 != _FileToBuff(pszXml, sbuf))
    {
        BYTE* buf = (BYTE*)sbuf.data();
        if (0xff == buf[0] && 0xfe == buf[1])
        {
            auto utf16 = cross::locale::get_codepage_id("utf-16");
            assert(utf16);
            std::string convbuf;
            convbuf.resize(sbuf.length());
            cross::string::convert_to_utf8(utf16, (char*) & buf[2], (sbuf.size() - 2), (char*)convbuf.data(), convbuf.size());
            string::size_type pos = convbuf.find('>');
            if (string::npos != pos)
            {
                string::size_type utftype = convbuf.find("16");
                if (string::npos != utftype && utftype < pos)
                    convbuf.replace(utftype - 4, 6, "utf-8");
            }

            FILE* m_f = NULL;
            int err = fopen_s(&m_f, pszXml, "wb");
            if (m_f)
            {
                fputs(convbuf.data(), m_f);
                fclose(m_f);
            }
        }
        else
            if (auto pos = sbuf.find("1251"); pos != string::npos && pos < sbuf.find('>'))
            {
                std::string convbuf;
                convbuf.resize(sbuf.length() * 4);
                cross::string::convert_to_utf8(1251, sbuf.data(), (char*)convbuf.data(), convbuf.size());
                convbuf.replace(pos - 8, 12, "utf-8");
                FILE* m_f = NULL;
                int err = fopen_s(&m_f, pszXml, "wb");
                if (m_f)
                {
                    fputs(convbuf.data(), m_f);
                    fclose(m_f);
                }
            }

    }
#endif
}

DWORD KCnfReader::ThreadSingleWin( LPVOID lpParam )
{
  SCnfParam *pParams = (SCnfParam*)lpParam;
  pParams->pOwner->ThreadReadCnf( pParams->szFilePath.c_str() );
  m_ActiveThreadsCount--;
  return 0;
}

void KCnfReader::ThreadReadCnf( LPCSTR pszCnfFile )
{
  char *pdecpoint = localeconv()->decimal_point;
  ASSD( pdecpoint && *pdecpoint=='.' );

  PrepareUTF8( pszCnfFile );

  tinyxml2::XMLDocument doc;
  int openTry = 0;
  for( openTry; openTry < 3; ++openTry )
  {
  tinyxml2::XMLError err = doc.LoadFile( pszCnfFile );
    if( tinyxml2::XML_SUCCESS==err )
      break;
    using namespace std;
    std::string xml;
    _FileToBuff( pszCnfFile, xml);

    if( !xml.empty() )
    {
      LPSTR pW = (LPSTR)&xml[0];
      LPSTR pE = strstr( pW, "</MultiBlock>");
      if( pE )
      {
        size_t l = pE - pW + 13 + 1;
        xml.resize( l );
        FILE *m_f = NULL;
        fopen_s( &m_f, pszCnfFile, "wb");
        if( m_f )
        {
          fwrite( &xml[0], xml.size(), 1, m_f );
          fclose( m_f );
        }
      }
    }
  }

  if( openTry>1 )
  {
    ASS(0);
    return;
  }

  tinyxml2::XMLElementPtr root = doc.RootElement();
  LPCSTR element_name = root->Name();
  ASS( !strcmp( element_name, "MultiBlock"));

  /*tHandlersCont_ cont;
  cont["ErdbVersion"] = &KCnfReader::OnSkip;
  cont["Block"] = &KCnfReader::OnBlock;
  __super::Read( root, cont );*/

  using namespace tinyxml2;
  for( XMLNode *node = root->FirstChild(); node != NULL; node = node->NextSibling())
  {
    XMLElementPtr item = node->ToElement();
    if( item == NULL )
      continue;
    LPCSTR element_name = item->Name();
    if( !strcmp(element_name, "Block") )
      OnBlock( item, pszCnfFile );
  }

  doc.SaveFile( pszCnfFile );
}

int KCnfReader::ReadCnf( LPCSTR pszMask, DWORD maskRead )
{
  m_MaskRead = maskRead;
  auto path = fs::path(pszMask);  
  
  KFileFind ff(pszMask);
  if (ff.files().size() == 0)
      return -1;

  typedef std::list<SCnfParam> tReadingParamsCont;

  tReadingParamsCont _readThreads;

  typedef std::list<std::string> tExportedCont_;
  tExportedCont_ exported;
  {
    fs::path exp = path.parent_path() / "Export.sl";
    ifstream file(exp.generic_string().c_str());
    string line;
    while( std::getline( file, line) )
    {
      //line.erase(std::remove(line.begin(), line.end(), '\0'), line.end());
      //cross::string::rtrim(line);
      exported.push_back( line );
    }
  }

  int count = ff.files().size();
  
  auto nNumberOfProcessors = std::thread::hardware_concurrency();
  m_ActiveThreadsCount = 0;

  int n = 0;
  char Title[128*4] = { 0 };
  for (const auto& file : ff.files())
  {
      fs::path path(file);
      std::string sFileName = path.filename().generic_string();
      if (count > 1)
      {
          sprintf_s(Title, 128*4, "%s ( %d / %d )", sFileName.c_str(), n, count);
      }
      qDebug(Title);
      if (sFileName.find('.') != string::npos)
      {
          std::string name = sFileName.substr(0, sFileName.find('.'));
          tExportedCont_::iterator itf = std::find(exported.begin(), exported.end(), name);
          if (exported.end() != itf)
              exported.erase(itf);
      }
      _readThreads.push_back(SCnfParam());
      SCnfParam& lastParm = _readThreads.back();
      lastParm.pOwner = this;
      lastParm.szFilePath = file;
      m_ActiveThreadsCount++;
      //ThreadSingleWin(&lastParm);
      lastParm.hThread = std::thread(ThreadSingleWin, &lastParm);
      while (m_ActiveThreadsCount >= nNumberOfProcessors)
      {
          std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }

      n++;
  }
  
  for (auto&& t : _readThreads)
  {
      if (t.hThread.joinable())
        t.hThread.join();
  }
  _readThreads.clear();
#ifdef _WIN32
  if (count > 1)
      CloseProgress();
#endif

  std::string sPath;
  char names[3][_MAX_FNAME] = { 0 };
  //??? if (SplitString(path.filename().string().c_str(), names, '.', false))
  //     sPath = rsu_cp::makepath_with_ext_cp(path.parent_path().generic_string(), names[0], "dep.xml").generic_string();
  // else
  //     sPath = rsu_cp::makepath_with_ext_cp(path.parent_path().generic_string(), "*", "dep.xml").generic_string();

  KFileFind ff1(sPath);
  if (ff1.files().size() == 0)
      return 0;
  n = 0;
  for (const auto& file : ff1.files())
  {
      AddDependencyListFrom(file.c_str());
      n++;
  }
  return 0;
}

int KCnfReader::AddDependencyListFrom( LPCSTR pszPath )
{
  PrepareUTF8( pszPath );

  tinyxml2::XMLDocument doc;
  tinyxml2::XMLError err = doc.LoadFile( pszPath );
  ASS( tinyxml2::XML_SUCCESS==err );
  
  tinyxml2::XMLElementPtr root = doc.RootElement();
  LPCSTR element_name = root->Name();
  ASS( !strcmp( element_name, "DependencyList"));
  
  tHandlersCont_ cont;
  cont["DependencyListObj"] = &KCnfReader::OnDependencyListObj;
  KTinyXmlReader<KCnfReader>::Read( root, cont );

  return 0;
}

void KCnfReader::OnDependencyListObj( tinyxml2::XMLElementPtr &root )
{
  string ObjectName;
  TINYXMLHELPER::GetXMLAttribute( root, "ObjectName", ObjectName );

  std::transform( ObjectName.begin(), ObjectName.end(), ObjectName.begin(), ::toupper );

  DependencyObjectName = GetBlockAnyThere( ObjectName );
  if( !DependencyObjectName )
    return;
 
  tHandlersCont_ cont;
  cont["Category"] = &KCnfReader::OnCategory;
  KTinyXmlReader<KCnfReader>::Read( root, cont );

  if( DependencyObjectName->DepConnections.empty() )
    return;

  bool bEraceDouble = true;
  while( bEraceDouble )
  {
    bEraceDouble = false;
    for( size_t i=0, C(DependencyObjectName->DepConnections.size()-1); i<C; ++i )
    {
      const tDepConnectionsCont_::value_type &first = DependencyObjectName->DepConnections.at(i);
      for( size_t n=i+1, N(DependencyObjectName->DepConnections.size()); n<N; ++n )
      {
        tDepConnectionsCont_::value_type &test = DependencyObjectName->DepConnections.at(n);
        if( first.SourceEnd==test.SourceEnd && first.TargetEnd==test.TargetEnd )
        {
          ASSD( first.ConnectionForm==test.ConnectionForm && first.ConnectionType==test.ConnectionType );
          DependencyObjectName->DepConnections.erase( DependencyObjectName->DepConnections.begin()+n );
          bEraceDouble = true;
          break;
        }
      }
      if( bEraceDouble )
        break;
    }
  }
  for( size_t i=0, N(DependencyObjectName->DepConnections.size()); i<N; ++i )
  {
    const tDepConnectionsCont_::value_type &con = DependencyObjectName->DepConnections.at(i);
    if( 256!=con.ConnectionType )
      continue;
    char names[2][32*4] = { 0 };
    const int C = SplitString( con.SourceEnd.c_str(), names, '.', false );
    ASSD( 2==C );
    char szName[64*4];
    sprintf_s( szName, 64*4, "%s.%s", names[0], names[1] );
    ObjectName = szName;
    tBlockPtr_ ptr = GetBlockAnyThere( ObjectName, &DependencyObjectName->m_EmbBlocks );
    if( !ptr )
      ptr = GetBlockAnyThere( ObjectName, NULL );
    ASSD( ptr );
    SConnection nc;
    nc.InputEnd = con.SourceEnd;
    nc.OutputEnd = con.TargetEnd;
    nc.bForwardConnection = false;
    ptr->Connections.push_back( nc );
  }
}

void KCnfReader::OnCategory( tinyxml2::XMLElementPtr &root )
{
  string Type;
  TINYXMLHELPER::GetXMLAttribute( root, "Type", Type );
  ASS( "Connection"==Type );

  tHandlersCont_ cont;
  cont["Dependency"] = &KCnfReader::OnDependency;
  KTinyXmlReader<KCnfReader>::Read( root, cont );
}

void KCnfReader::OnDependency( tinyxml2::XMLElementPtr &root )
{
  string TagName;
  string FullName = DependencyObjectName->BlockName;
  TINYXMLHELPER::GetXMLAttribute( root, "TagName", TagName );
  /*FullName.append( "." );
  FullName.append( TagName );*/
  DependencyTagName = GetBlock( TagName, false );

  if( !DependencyTagName )
    return;

  tHandlersCont_ cont;
  cont["DependencyInstances"] = &KCnfReader::OnDependencyInstances;
  KTinyXmlReader<KCnfReader>::Read( root, cont );
}

void KCnfReader::OnDependencyInstances( tinyxml2::XMLElementPtr &root )
{
  tHandlersCont_ cont;
  cont["DependencyInstance"] = &KCnfReader::OnDependencyInstance;
  KTinyXmlReader<KCnfReader>::Read( root, cont );
}

void KCnfReader::OnDependencyInstance( tinyxml2::XMLElementPtr &root )
{
  curDependencyInstance.Clear(); 
  
  tHandlersCont_ cont;
  cont["Attribute"] = &KCnfReader::OnAttribute;
  KTinyXmlReader<KCnfReader>::Read( root, cont );

  switch( curDependencyInstance.ConnectionType )
  {
  case 1:
    break;
  case 2:
    break;
  case 130:
    break;
  case 256:
    break;
  case 8210:
    break;
  default:
    ASSD(0);
  }

  DependencyTagName->DepConnections.push_back( curDependencyInstance );
}

void KCnfReader::OnAttribute( tinyxml2::XMLElementPtr &root )
{
  string szName, szValue;

  for( tinyxml2::XMLNode *node = root->FirstChild(); node != NULL; node = node->NextSibling())
  {
    tinyxml2::XMLElementPtr item = node->ToElement();
    if( item == NULL )
      continue;
    LPCSTR element_name = item->Name();
    if( !strcmp( "Name", element_name) )
      TINYXMLHELPER::GetXMLData( item, szName );
    else if( !strcmp( "Value", element_name) )
      TINYXMLHELPER::GetXMLData( item, szValue );
    else
    {
      ASS(0);
    }
  }
  if( "SourceEnd"==szName )
    curDependencyInstance.SourceEnd = szValue;
  else if( "TargetEnd"==szName )
    curDependencyInstance.TargetEnd = szValue;
  else if( "ConnectionForm"==szName )
    curDependencyInstance.ConnectionForm = atoi( szValue.c_str() );
  else if( "ConnectionType"==szName )
    curDependencyInstance.ConnectionType = atoi( szValue.c_str() );
  else
  {
    ASSD(0);
  }
}

void KCnfReader::OnBlock( tinyxml2::XMLElementPtr &root )
{
  ASS(!"не использовать");
  m_allBlocks.push_back( new KBlock( tBlockPtr_() ) );
  tBlockPtr_ &last = m_allBlocks.back();
  last->Parse( root, m_MaskRead );
  /*if( !last->IsCoordSame() )
    last->szXmlFile = m_szCurFile;*/
}

void KCnfReader::OnBlock( tinyxml2::XMLElementPtr &root, LPCSTR pszFileName )
{
  tBlockPtr_ block;
  {
    s_csOnContour.lock();
    m_allBlocks.push_back( new KBlock( tBlockPtr_() ) );
    tBlockPtr_ &last = m_allBlocks.back();
    block = last;
  }
  block->Parse( root, m_MaskRead );
  
  if( !block->IsCoordSame() )
  {
    fs::path path(pszFileName);
    block->szXmlFile = path.filename().string();
  }
}

static LPCSTR pszEntryNames[] =
{
  "C300",
};

tBlockPtr_ KCnfReader::SortFor( std::string &cntlName )
{
  if( NULL!=m_entryBlock )
    return m_entryBlock;
  if( !cntlName.empty() )
  {
    m_entryBlock = GetBlock( cntlName );
    ASS( m_entryBlock );
    if( m_entryBlock )
    {
      ASS( !m_entryBlock->BlockName.empty() );
      FindChildFor( m_entryBlock );
    }
  }
  else
  {
    tBlocksCont_ blks;
    EnumAssignedToEmpty( blks );
    for( tBlocksCont_::iterator it = blks.begin(); it!=blks.end(); )
    {
      tBlocksCont_::value_type &bl = *it;
      LPCSTR pszTnName = bl->tnClass.c_str();
      bool bRemove = true;
      for( int i=0; i<_countof(pszEntryNames); ++i )
      {
        if( !strcmp(pszEntryNames[i], pszTnName) )
        {
          bRemove = false;
          break;
        }
      }
      if( bRemove )
        it = blks.erase( it );
      else
        ++it;
    }
    ASSD( !blks.empty() );
    if( blks.empty() )
      return tBlockPtr_();
    tBlockPtr_ root =  new KBlock( tBlockPtr_() );
    root->tnClass = root->BlockName = root->EntityName = "Root";
    for( tBlocksCont_::iterator it = blks.begin(), end(blks.end()); it!=end; ++it )
    {
      tBlockPtr_ t = *it;
      FindChildFor( t );
      root->AddChildBlock( *it );
    }
    m_entryBlock = root;
  }
  return m_entryBlock;
}

void KCnfReader::FindChildFor( tBlockPtr_ &root )
{
  while( 1 )
  {
    tBlockPtr_ t = GetAssignedTo( root->BlockName );
    if( NULL==t )
      break;
    ASS( !t->BlockName.empty() );
    root->AddChildBlock( t );
  }
  for( tBlocksCont_::iterator it = root->m_EmbBlocks.begin(), end( root->m_EmbBlocks.end() ); it!=end; ++it )
  {
    tBlockPtr_ t = *it;
    FindChildFor( t );
  }
}

tBlockPtr_ KCnfReader::GetAssignedTo( std::string &parentName )
{
  tBlockPtr_ r;
  for( tBlocksCont_::iterator it=m_allBlocks.begin(), end(m_allBlocks.end()); it!=end; ++it )
  {
    if( (*it)->AssignedTo==parentName )
    {
      r = *it;
      m_allBlocks.erase( it );
      break;
    }
  }
  return r;
}

tBlockPtr_ KCnfReader::GetBlock( std::string &name, bool bRemove /*= true*/ )
{
  tBlockPtr_ r;
  for( tBlocksCont_::iterator it=m_allBlocks.begin(), end(m_allBlocks.end()); it!=end; ++it )
  {
    if( (*it)->BlockName==name )
    {
      r = *it;
      if( bRemove )
        m_allBlocks.erase( it );
      break;
    }
  }
  return r;
}

tBlockPtr_ KCnfReader::GetBlockAnyThere( std::string &name, tBlocksCont_ *pCont /*= NULL*/ )
{
  tBlockPtr_ r;
  if( !pCont )
    pCont = &m_allBlocks;
  for( tBlocksCont_::iterator it=pCont->begin(), end(pCont->end()); it!=end; ++it )
  {
    tBlocksCont_::value_type &blk = *it;
    if( blk->BlockName==name )
    {
      r = *it;
      break;
    }
    r = GetBlockAnyThere( name, &blk->m_EmbBlocks );
    if( r )
      break;
  }
  return r;
}

void KCnfReader::EnumAssignedToEmpty( tBlocksCont_ &arr )
{
  tBlockPtr_ r;
  for( tBlocksCont_::iterator it=m_allBlocks.begin(), end(m_allBlocks.end()); it!=end; ++it )
  {
    tBlocksCont_::value_type &blk = *it;
    if( blk->AssignedTo.empty() )
    {
      arr.push_back( blk );
    }
  }
}

#include "Block.h"
#include <macros/csvrow.h>
//#include <Shlwapi.h>
#include "SymbolAttr.h"
#include <algorithm>
#include <H_Data.h>

using namespace TINYXMLHELPER;
using namespace std;

KBlock::KBlock( tBlockPtr_ parent )
: BlockId( 0 )
, m_maskRead( emrMinimum )
{
  ZeroMemory( &m_InflateRect, sizeof(m_InflateRect) );
  m_ParentBlock = parent;
  ZeroMemory( &BlockGUID, sizeof(BlockGUID) );
}

KBlock::~KBlock(void)
{
}

void KBlock::AddChildBlock( tBlockPtr_ &bl )
{
  m_EmbBlocks.push_back( bl );
}

SRECT KBlock::GetCoord()
{
  return Coord;
}

bool KBlock::IsCoordSame()
{
  SRECT my( Coord );
  SRECT res( Coord );
  for( tBlocksCont_::iterator it=m_EmbBlocks.begin(), end(m_EmbBlocks.end()); it!=end; ++it )
  {
    SRECT n = (*it)->GetCoord();
    res.left = std::min<LONG>( res.left, n.left );
    res.right = std::max<LONG>( res.right, n.right );
    res.top = std::min<LONG>( res.top, n.top );
    res.bottom = std::max( res.bottom, n.bottom );
  }
  if( res.EqualsTo(my) )
    return true;
  return false;
}

void KBlock::Parse( tinyxml2::XMLElementPtr &root, DWORD mskRead )
{
  m_maskRead = mskRead;
  tHandlersCont_ cont;
  cont["BlockDef"] = &KBlock::OnBlockDef;
  cont["Parameters"] = ( m_maskRead & IBlock::emrParameters ) ? &KBlock::OnParameters : &KBlock::OnSkip;
  cont["EmbBlocks"] = &KBlock::OnEmbBlocks;
  cont["SymbolAttrs"] = ( m_maskRead&IBlock::emrSymbolAttrs ) ? &KBlock::OnSymbolAttrs : &KBlock::OnSkip;
  cont["Connections"] = &KBlock::OnConnections;
  cont["ProjParameters"] = &KBlock::OnSkip;
  cont["NonCeePoints"] = &KBlock::OnNonCeePoints;
  KTinyXmlReader<KBlock>::Read( root, cont );
}

void KBlock::OnBlockDef( tinyxml2::XMLElementPtr &root )
{
  tHandlersCont_ cont;
  cont["BlockName"] = &KBlock::OnBlockName;
  cont["EntityName"] = &KBlock::OnEntityName;
  cont["BlockId"] = &KBlock::OnBlockId;
  cont["BlockGUID"] = &KBlock::OnBlockGUID;
  cont["Coord"] = &KBlock::OnCoord;
  cont["AssignedTo"] = &KBlock::OnAssignedTo;
  cont["HandlerName"] = &KBlock::OnHandlerName;
  cont["TemplateName"] = &KBlock::OnTemplateName;
  cont["ClassName"] = &KBlock::OnClassName;
  cont["Container"] = &KBlock::OnContainer;
  KTinyXmlReader<KBlock>::Read( root, cont, false );
}

bool UDsmallerORDERINCM( tBlocksCont_::value_type &elem1, tBlocksCont_::value_type &elem2 )
{
  return elem1->ORDERINCM < elem2->ORDERINCM;
}

void KBlock::OnEmbBlocks( tinyxml2::XMLElementPtr &root )
{
  for( tinyxml2::XMLNode *node = root->FirstChild(); node != NULL; node = node->NextSibling())
  {
    tinyxml2::XMLElementPtr item = node->ToElement();
    LPCSTR element_name = item->Name();
    ASS( !strcmp( element_name, "Block") );
    tBlockPtr_ bl = new KBlock( tBlockPtr_(this) );
    m_EmbBlocks.push_back( bl );
    bl->Parse( item, m_maskRead );
  }
  for( tBlocksCont_::iterator it=m_EmbBlocks.begin(), end(m_EmbBlocks.end()); it!=end; ++it )
  {
    tBlockPtr_ bl = *it;
    string &szBlkName = bl->BlockName;
    size_t lb = szBlkName.length();
    bool bRepeate = false;
    do
    {
      bRepeate = false;
      for( tConnectionsCont_::iterator ic=bl->Connections.begin(), endC(bl->Connections.end()); ic!=endC; ++ic )
      {
        string &szIn = ic->InputEnd;
        string &szOut = ic->OutputEnd;
        if( szIn.length()>lb && '.'==szIn.at( lb ) && !strncmp( szIn.c_str(), szBlkName.c_str(), lb ) )
        {
        }
        else
        {
          for( tBlocksCont_::iterator it2=m_EmbBlocks.begin(), end2(m_EmbBlocks.end()); it2!=end2; ++it2 )
          {
            tBlockPtr_ bl2 = *it2;
            string &szBlkName2 = bl2->BlockName;
            size_t lb2 = szBlkName2.length();
            if( szIn.length()>lb2 && '.'==szIn.at( lb2 ) && !strncmp( szIn.c_str(), szBlkName2.c_str(), lb2 ) )
            {
              bRepeate = true;
              bl2->Connections.push_back( *ic );
              bl->Connections.erase( ic );
              break;
            }
          }
          //ASSD( bRepeate );//из-за связей с другиму модулями
          break;
        }
      }
    }while( bRepeate );
  }
  for( tBlocksCont_::iterator it=m_EmbBlocks.begin(), end(m_EmbBlocks.end()); it!=end; ++it )
  {
    if( (*it)->ORDERINCM >= 10 )
    {
      std::sort( m_EmbBlocks.begin(), m_EmbBlocks.end(), UDsmallerORDERINCM );
      break;
    }
  }
}

void KBlock::OnBlockName( tinyxml2::XMLElementPtr &root )
{
  GetXMLData( root, BlockName );
  std::transform( BlockName.begin(), BlockName.end(), BlockName.begin(), ::toupper );
}

void KBlock::OnEntityName( tinyxml2::XMLElementPtr &root )
{
  GetXMLData( root, EntityName );
  std::transform( EntityName.begin(), EntityName.end(), EntityName.begin(), ::toupper );
  if( EntityName.empty() )
    EntityName = BlockName;
}

void KBlock::OnBlockId( tinyxml2::XMLElementPtr &root )
{
  GetXMLData( root, BlockId );
}

void KBlock::OnBlockGUID( tinyxml2::XMLElementPtr &root )
{
  string guid;
  GetXMLData( root, guid );
  BlockGUID = StringToGuidWithoutBrackets(guid.c_str());
}

void KBlock::OnCoord( tinyxml2::XMLElementPtr &root )
{
  Coord.Parse( root );
}

void KBlock::OnAssignedTo( tinyxml2::XMLElementPtr &root )
{
  GetXMLData( root, AssignedTo );
  std::transform( AssignedTo.begin(), AssignedTo.end(), AssignedTo.begin(), ::toupper );
}

void KBlock::OnHandlerName( tinyxml2::XMLElementPtr &root )
{
  GetXMLData( root, HandlerName );
  std::transform( HandlerName.begin(), HandlerName.end(), HandlerName.begin(), ::toupper );
}

void KBlock::OnClassName( tinyxml2::XMLElementPtr &root )
{
  GetXMLData( root, ClassName );
}

void KBlock::OnTemplateName( tinyxml2::XMLElementPtr &root )
{
  GetXMLData( root, TemplateName );
  string::size_type pos = TemplateName.find(':');
  ASS( pos!=string::npos );
  if( pos==string::npos )
    return;
  tnNamespace = TemplateName.substr( 0, pos );
  tnClass = TemplateName.substr( pos+1 );
}

void KBlock::OnContainer( tinyxml2::XMLElementPtr &root )
{
  GetXMLData( root, Container );
  std::transform( Container.begin(), Container.end(), Container.begin(), ::toupper );
  if( Container.empty() )
    return;
  string::size_type p = BlockName.find('.');
  if( string::npos!=p )
  {
    if( strncmp(BlockName.c_str(), Container.c_str(), p) )
    {
      BlockName.insert( p, 1, '.' );
      BlockName.insert( p+1, Container );
    }
  }

  p = EntityName.find('.');
  if( string::npos==p )
  {
      char msg[128*4];
      sprintf_s(msg, 128*4, "!!!!!   blk:%s item:%s (%s)\n", BlockName.c_str(), EntityName.c_str(), Container.c_str());
      qDebug( msg );
    EntityName = BlockName;
  }
  else if( strncmp(EntityName.c_str(), Container.c_str(), p) )
  {
    EntityName.insert( p, 1, '.' );
    EntityName.insert( p+1, Container );
  }

  size_t cb = count( BlockName.begin(), BlockName.end(), '.');
  size_t ce = count( EntityName.begin(), EntityName.end(), '.');
  ASS( cb==ce );
}

void KBlock::OnParameters( tinyxml2::XMLElementPtr &root )
{
  tHandlersCont_ cont;
  cont["Parameter"] = &KBlock::OnParameter;
  KTinyXmlReader<KBlock>::Read( root, cont );

  tParam_ test;
  test.first = "ORDERINCM";
  tParamsCont_::iterator itO = std::find( Parameters.begin(), Parameters.end(), test );
  if( Parameters.end() != itO )
    ORDERINCM = atoi( itO->second.c_str() );
  else
    KKK();
}

void KBlock::OnParameter( tinyxml2::XMLElementPtr &root )
{
  string nv[3];
  bool oks[] = { false, false, false };
  bool *poks = &oks[0];
  int i=0;

  for( tinyxml2::XMLNode *node = root->FirstChild(); node != NULL; node = node->NextSibling(), ++poks, ++i )
  {
    ASS( i<_countof(nv) );
    tinyxml2::XMLElementPtr item = node->ToElement();
#ifdef _DEBUG
    LPCSTR element_name = item->Name();
#endif
    *poks = GetXMLData( item, nv[i] );
  }
  if( !oks[0] || !oks[1] )
    return;
  //if( nv[1].empty() ) return;
  Trim( nv[1], '"');
  Parameters.push_back( tParamsCont_::value_type(nv[0],nv[1],nv[2]) );
}

void KBlock::OnConnections( tinyxml2::XMLElementPtr &root )
{
  tHandlersCont_ cont;
  cont["Connection"] = &KBlock::OnConnection;
  KTinyXmlReader<KBlock>::Read( root, cont );
}

void KBlock::OnConnection( tinyxml2::XMLElementPtr &root )
{
  Connections.push_back( tConnectionsCont_::value_type() );

  tHandlersCont_ cont;
  cont["BlockId"] = &KBlock::OnSkip;
  cont["InputEnd"] = &KBlock::OnConInputEnd;
  cont["OutputEnd"] = &KBlock::OnConOutputEnd;
  cont["ConnectionForm"] = (m_maskRead&IBlock::emrConVertexs) ? &KBlock::OnConConnectionForm : &KBlock::OnSkip;
  cont["GraphicalForm"] = (m_maskRead&IBlock::emrConVertexs) ? &KBlock::OnConGraphicalForm : &KBlock::OnSkip;
  cont["Vertex"] = (m_maskRead&IBlock::emrConVertexs) ? &KBlock::OnConVertex : &KBlock::OnSkip;
  KTinyXmlReader<KBlock>::Read( root, cont );
}

void KBlock::OnConInputEnd( tinyxml2::XMLElementPtr &root )
{
  string &str = Connections.back().InputEnd;
  GetXMLData( root, str );
  std::transform( str.begin(), str.end(), str.begin(), ::toupper );
}

void KBlock::OnConOutputEnd( tinyxml2::XMLElementPtr &root )
{
  string &str = Connections.back().OutputEnd;
  GetXMLData( root, str );
  std::transform( str.begin(), str.end(), str.begin(), ::toupper );
  if( str=="P_1002.GOP.OUT.ENUM" )
    KKK();
}

void KBlock::OnConConnectionForm( tinyxml2::XMLElementPtr &root )
{
  string at;
  GetXMLData( root, at );
  Connections.back().ConnectionForm = ns_ConnectionForm::StringToeConnectionForm( at.c_str() );
  ASS( Connections.back().ConnectionForm < ns_ConnectionForm::MAX_NUMBER_OF_eConnectionForm );
}

void KBlock::OnConGraphicalForm( tinyxml2::XMLElementPtr &root )
{
  string at;
  GetXMLData( root, at );
  Connections.back().GraphicalForm = ns_GraphicalForm::StringToeGraphicalForm( at.c_str() );
  ASS( Connections.back().GraphicalForm < ns_GraphicalForm::MAX_NUMBER_OF_eGraphicalForm );
}

void KBlock::OnConVertex( tinyxml2::XMLElementPtr &root )
{
  tConnectionsCont_::value_type &con = Connections.back();
  con.path.push_back( KVertex() );
  con.path.back().Parse( root );
}

bool UDsmallerAttr( tSymbolAttrPtr_ &elem1, tSymbolAttrPtr_ &elem2 )
{
  return elem1->AttrOrder < elem2->AttrOrder;
}

void KBlock::OnSymbolAttrs( tinyxml2::XMLElementPtr &root )
{
  tHandlersCont_ cont;
  cont["SymbolAttr"] = &KBlock::OnSymbolAttr;
  KTinyXmlReader<KBlock>::Read( root, cont );

  for( tSortedAttrs_::iterator it=m_Attrs.begin(), end(m_Attrs.end()); it!=end; ++it )
  {
    tSortedAttrs_::mapped_type &vattrs = it->second;
    std::sort( vattrs.begin(), vattrs.end(), UDsmallerAttr );
    KKK();
  }
}

void KBlock::OnSymbolAttr( tinyxml2::XMLElementPtr &root )
{
  tSymbolAttrPtr_ symAttr = new KSymbolAttr;
  symAttr->Parse( root );

  ns_AttrType::eAttrType type = symAttr->AttrType;

  tSortedAttrs_::iterator it = m_Attrs.find( type );
  if( m_Attrs.end()==it )
  {
    it = m_Attrs.insert( tSortedAttrs_::value_type( type, tSortedAttrs_::mapped_type() ) ).first;
    switch( type )
    {
    case ns_AttrType::InputTop:
      m_InflateRect.top = Top;
      if( ClassName=="DEVCTL" )
        m_InflateRect.top = TopDEVCTL;
      break;
    case ns_AttrType::InputLeft:
      m_InflateRect.left = Left;
      break;
    case ns_AttrType::OutputBottom:
      m_InflateRect.bottom = Bottom;
      if( ClassName=="DEVCTL" )
        m_InflateRect.bottom = BottomDEVCTL;
      break;
    case ns_AttrType::OutputRight:
      m_InflateRect.right = Right;
      break;
    }
  }

  it->second.push_back( symAttr );
}

SRECT KBlock::GetBlkBodyCoord()
{
  SRECT blk = GetCoord();

  //blk.InflateRect( &m_InflateRect );
  blk.left -= m_InflateRect.left;
  blk.top -= m_InflateRect.top;
  blk.right += m_InflateRect.right;
  blk.bottom += m_InflateRect.bottom;

  return blk;
}

tSymbolAttrPtr_ KBlock::GetSymbAttr( ns_AttrType::eAttrType attr, size_t index )
{
  tSymbolAttrPtr_ ptr;
  tSortedAttrs_::iterator it = m_Attrs.find( attr );
  if( m_Attrs.end()==it )
    return ptr;
  if( index < it->second.size() )
    return it->second.at(index);
  return ptr;
}

void KBlock::OnNonCeePoints( tinyxml2::XMLElementPtr &root )
{
  tHandlersCont_ cont;
  cont["NonCeePointName"] = &KBlock::OnNonCeePointName;
  KTinyXmlReader<KBlock>::Read( root, cont );
}

void KBlock::OnNonCeePointName( tinyxml2::XMLElementPtr &root )
{
  string str;
  GetXMLData( root, str );
  m_NonCeePointNames.push_back( str );
}

FILE * l_fopen ( const char * filename,
              const char * modes )
{
   return fopen ( filename, modes );
}

#pragma once
#include <string>
#include <vector>
#include "Vertex.h"
#include <macros/en2str.h>

namespace ns_ConnectionForm
{
  DECLARE_ENUM( eConnectionForm,
                noForm,
                NamedConnVisible,
                NamedConnNotVisible,
                AssociationNotVisible,
                GraphicalConn );
}

namespace ns_GraphicalForm
{
  DECLARE_ENUM( eGraphicalForm,
                noForm,
                IG,
                BG,
                OG );
}

struct SConnection
{
  SConnection() 
    : ConnectionForm( ns_ConnectionForm::noForm )
    , GraphicalForm( ns_GraphicalForm::noForm )
    , bForwardConnection( true )
  {
  }
  std::string InputEnd;
  std::string OutputEnd;
  ns_ConnectionForm::eConnectionForm ConnectionForm;
  ns_GraphicalForm::eGraphicalForm GraphicalForm;
  typedef std::vector<KVertex> tVerCont_;
  tVerCont_ path;
  bool bForwardConnection;
  bool operator==( const SConnection &thr )
  {
    return thr.OutputEnd==InputEnd && thr.InputEnd==OutputEnd;
  }
};

struct SConnectionFromDependency
{
  std::string SourceEnd;
  std::string TargetEnd;
  int ConnectionForm;
  int ConnectionType;
  SConnectionFromDependency() : ConnectionForm( 0 ), ConnectionType( 0 )
  {}
  void Clear()
  {
    SourceEnd.clear();
    TargetEnd.clear();
    ConnectionForm = ConnectionType = 0;
  }
};
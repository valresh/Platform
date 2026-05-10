#pragma once
#include "Tag.h"

class CShape : public CTag
  {
  const char* myID;
  public:
    int m_nShapeNumber;
    CShape(void);	
    virtual ~CShape() {}	
	virtual TagType GetType(){ return TT_SHAPE; }
    //
  };


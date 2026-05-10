#pragma once


struct SCommDataWW
{
  enum eDefVal : BYTE
  {
    edf_NULL = ' ',
  };
  enum eDataType : BYTE
  {
    edt_HZ = edf_NULL,
    edt_16sI = 1,
    edt_32sI = 2,
    edt_16uI = 3, 
    edt_32uI = 4, 
    edt_32fI = 5, 
    edt_64fI = 6,
    edt_16sO = 7,
    edt_32sO = 8,
    edt_16uO = 9, 
    edt_32uO = 10, 
    edt_32fO = 11, 
    edt_64fO = 12,
    edt_discretInput = 13,
    edt_discretOutput = 14,
    edt_discretHZ = 15,
    edt_discretSelectIO = 16,
    edtCount,
  };
  eDataType DataType;
  bool IsDataTypeDiscrete()
  {
    if( DataType>=edt_discretInput && DataType < edtCount )
      return true;
    ASS( DataType!=edt_HZ );
    return false;
  }
  bool isDataTypeBit()
  {
    switch( DataType )
    {
    case edt_32fI:
    case edt_64fI:
    case edt_32fO:
    case edt_64fO:
      return false;
    }
    ASS( DataType!=edt_HZ );
    return true;
  }
};

struct SScsCommIO
{
  enum eDefVal : BYTE
  {
    edf_NULL = ' ',
  };
  enum eDataType : BYTE
  {
    edt_HZ = edf_NULL,
    edt_16sI = 1,
    edt_32sI = 2,
    edt_16uI = 3, 
    edt_32fI = 4, 
    edt_16sO = 5,
    edt_32sO = 6,
    edt_16uO = 7, 
    edt_32fO = 8, 
    edt_discretInput = 9,
    edt_discretOutput = 10,
    edtCount,
  };
  eDataType DataType;
};

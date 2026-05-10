#pragma once
#include <string>
#include <vector>
#include <sstream>
//#include <crossstring.h>

template<typename T>
void TrimLeft(std::basic_string<T>& str, const T chars2remove)
{
  if (!str.empty())
  {
    typename std::basic_string<T>::size_type pos = str.find_first_not_of(chars2remove);

    if (pos != std::basic_string<T>::npos)
      str.erase(0,pos);
    else
      str.erase( str.begin() , str.end() );
  }
}

template<typename T>
void TrimRight(std::basic_string<T>& str, const T chars2remove)
{
  if (!str.empty())
  {
    typename std::string::size_type pos = str.find_last_not_of(chars2remove);
    if (pos != std::string::npos)
      str.erase(pos+1);
    else
      str.erase( str.begin() , str.end() ); // make empty
  }
}

template<typename T>
void Trim(std::basic_string<T>& str, const T chars2remove)
{
  TrimLeft( str, chars2remove );
  TrimRight( str, chars2remove );
}

template<class _Elem>
std::basic_stringstream<_Elem> & Getline( std::basic_stringstream<_Elem> &stream, std::basic_string<_Elem> &str, const int _Delim )
{
  bool bClered = false;
  bool bIgnoreDelim = false;
  const int stringSel = stream.widen('\"');

  str.clear();

  bool _Changed = false;
  std::ios_base::iostate _State = std::ios_base::goodbit;

  for( int _Meta = stream.rdbuf()->sgetc(); ;_Meta = stream.rdbuf()->snextc() )
  {    
    if( _Meta==std::char_traits<_Elem>::eof() )
    {
      _State |= std::ios_base::eofbit;
      break;
    }
    if( _Meta==stringSel )
      bIgnoreDelim = !bIgnoreDelim;
    if( !bIgnoreDelim && _Meta==_Delim )
    {// got a delimiter, discard it and quit
      _Changed = true;
      stream.rdbuf()->sbumpc();
      break;
    }
    if( !bClered )
    {
      bClered = true;
      str.clear();
    }
    str += std::char_traits<_Elem>::to_char_type( _Meta );
    _Changed = true;
  }
  if( !_Changed )
    _State |= std::ios_base::failbit;
  stream.setstate(_State);
  return (stream);
}


template<typename _Elem>
class CSVRow
{
  _Elem m_splitter, m_comment;
  bool m_bEmptyCell;
public:
  CSVRow( _Elem split, _Elem comment = 0, bool bEmptyCell = true ) 
    : m_splitter( split )
    , m_comment( comment )
    , m_bEmptyCell( bEmptyCell )
  {}
  std::basic_string<_Elem> & operator[](std::size_t index) 
  {
    return m_data[index];
  }
  std::size_t size() const
  {
    return m_data.size();
  }

  void readNextRow(std::basic_istream<_Elem, std::char_traits<_Elem> >& str)
  {
    std::basic_string<_Elem> line;
    std::getline( str, line);
//???    std::string::rtrim(line);

    if( m_comment > 0 )
    {
      size_t pos = line.find_first_of( m_comment );
      if( std::basic_string<_Elem>::npos != pos )
        line = line.substr( 0, pos );
      if( line.empty() )
        return;
    }

    std::basic_stringstream<_Elem>   lineStream(line);
    std::basic_string<_Elem>         cell;

    m_data.clear();
    while( Getline( lineStream, cell, m_splitter) )
    {
      /*if( !cell.empty() && str.widen('\"')==cell.at(0) )
      {
        TrimLeft( cell, str.widen('\"') );
        TrimRight( cell, str.widen('\"') );
      }*/
      if( cell.empty() && !m_bEmptyCell )
        continue;
      m_data.push_back(cell);
    }
  }
public:
  typedef std::vector<std::basic_string<_Elem>> tDataCont_;
  tDataCont_ m_data;
};

template<typename _Elem>
inline std::basic_istream<_Elem, std::char_traits<_Elem> >& operator>>(std::basic_istream<_Elem, std::char_traits<_Elem> >& str,CSVRow<_Elem>& data)
{
  data.readNextRow(str);
  return str;
}

typedef CSVRow<char> aCSVRow_;
typedef CSVRow<wchar_t> wCSVRow_;
#ifdef _WIN32
	typedef CSVRow<TCHAR> tCSVRow_;
#endif


#pragma once

#include <QMutex>

class KCsSync
{
  QMutex * m_pCS;
public:
  KCsSync( QMutex &cs ) : m_pCS( &cs )
  {
    m_pCS->lock();
  }
  ~KCsSync()
  {
    m_pCS->unlock();
  }
};

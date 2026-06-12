#ifndef START_H
#define START_H
#include <QObject>
#include "CommProc.h"

class Start : public QObject
{
 Q_OBJECT
  public:
    ProcessData PD;
    class MainWindow* pMainWnd;
    explicit Start(QObject* parent = nullptr);
    virtual ~Start();
    bool Load();
		int Prepare();
    void InitWork();
  public slots:

    virtual void start();
    virtual void stop ();

  signals:
    void started();
    void stopped();
    void loaded();

};

class Models : public QObject
{
    Q_OBJECT
public:
    ProcessData PD;
		qint64 total_tm;
		qint64 total_step;
		explicit Models(QObject* parent = nullptr);
    virtual ~Models();
    void Go();
    void OutMsg( const char * Fmt, ... );

public slots:

    virtual void start();
    virtual void stop ();

signals:
    void started();
    void stopped();
    void loaded();
    void ShowSteps ( int kSteps );
    void ShowAccel ( double Accel );
    void ShowData ( double ProcUser, int Mem );
    void OutTxt( const char * Txt );
};

class Hydro : public QObject
{
    Q_OBJECT
  public:
    ProcessData PD;
		qint64 total_tm;
		qint64 total_step;
		explicit Hydro(QObject* parent = nullptr);
    virtual ~Hydro();
    void Go();
  public slots:
  virtual void start();
  signals:
    void ShowData ( double ProcUser, int Mem );
};

class DCU : public QObject
{
    Q_OBJECT
public:
    ProcessData PD;
		qint64 total_tm;
		qint64 total_step;
    explicit DCU(QObject* parent = nullptr);
    virtual ~DCU();
    void Go();
public slots:
    virtual void start();
signals:
    void ShowData ( double ProcUser, int Mem );
};

#endif // START_H

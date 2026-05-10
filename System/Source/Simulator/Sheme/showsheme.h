#ifndef SHOWSHEME_H
#define SHOWSHEME_H

#include <QWidget>
#include "EMF.h"
#include "crossplatform.h"
#include "BaseModel.h"
#include "Err.h"
#include "HydroBasicTypes.h"
#include <QListView>
#include "winlist.h"

#define MAX_OBJ 10000


struct ObjCrd
{
    double L,B,R,T;
    CStr ObjName;
    CStr GoTo;
    IBaseModel * pObj;
};

class ShowSheme : public EMF, public WinList
{
    Q_OBJECT
  public:
    class MainWindow * pMainWnd;
    bool Drug;
    QPointF PosDrug;
    QRectF Etalon;
    ObjCrd * pCrd;
    int kCrd;
    CDataPtr * pDataPtr;
    const char * Select;
    QToolBar * toolbar;
//    CStr ShemeName;
 ///
    explicit ShowSheme(QWidget *parent, const char * File, const char * Select );
//    bool event (QEvent * event );
    void SetCrd( const char * crd );
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent ( QKeyEvent * event );
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void moveEvent(QMoveEvent *event);
    void updateTime();
    void closeEvent(QCloseEvent * event);
//
    void Model_To_Emf ( double & L, double & T, double & R, double & B, QRectF & Model );
    void Model_To_Emf ( double & L, double & T, double & R, double & B, ObjCrd & Obj );
    void Model_To_Emf ( double & L, double & T, double & R, double & B, CObjectRef * pO );
    int Find_Object ( QPointF Pos, IBaseModel * pObj[16] );
    int Find_Pipe ( QPointF Pos );
    void SpecKey ( int key );
    void Draw( QPainter * pP );
    bool OnScheme (LPCTSTR ObjScheme);
  signals:
    void Closed( ShowSheme * I );
    void on_Start();
  public slots:
//    void Show ( const char * Sheme );
      virtual void on_New_Shema_triggered();
      virtual void on_Exit_triggered();
      void tbPause();
      void tbCont();
      void tbSaveParams();
      void tbSaveState();
      void tbSaveAll();
};
void KKK();
void P_to_P( SS * pSS, QPointF Pos, double & X, double & Y );
int ChatToUTF8( char * To, int MaxTo, char * From, bool Pre );

#endif // SHOWSHEME_H


#ifndef EMF_H
#define EMF_H

#include <QLabel>
#include <QMainWindow>
#include <QScrollArea>

QT_BEGIN_NAMESPACE
namespace Ui {
class EMF;
}
QT_END_NAMESPACE

#include <QtCore/QtGlobal>

#ifdef EMF_DLL
#define IN_DLL Q_DECL_EXPORT
#else
#define IN_DLL Q_DECL_IMPORT
#endif

typedef
  void (*tExtPaint) ( void * pSheme, QPainter * pP );

struct SS
{
  char * data; //// содержимое emf
  qint64 Size;
  double ScaleX, ScaleY;
  double ShiftX, ShiftY;
  tExtPaint pExtPaint;
  void * pSheme;
  class QPixmap * pixmap;
  const char * NewPath;
  SS()
  {
    ScaleX= 1.;
    ScaleY = 1.;
    ShiftX = 0.;
    ShiftY = 0.;
    data = NULL;
    Size = 0;
    pExtPaint = NULL;
    pSheme = NULL;
    NewPath = NULL;
  }
};

class IN_DLL EMF : public QMainWindow
{
    Q_OBJECT

public:
    EMF(QWidget *parent,  const char * File );
    ~EMF();

    bool loadEmfFile(const QString &filePath);

private slots:
    void openEmfFile();

//private:

    virtual void on_New_Shema_triggered() = 0;

    virtual void on_Exit_triggered() = 0;

public:
    Ui::EMF *ui;
    QPixmap * pMap;
    void setupViewer( char * Path );
    bool renderEmf( struct SS * pSS, const QString &filePath);
    QLabel *m_imageLabel;
    QScrollArea *m_scrollArea;
    QString m_currentFilePath;
////////////////////////////////////////////////
    SS ss; // содержимое картинки
};
#endif // EMF_H

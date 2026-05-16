#include "EMF.h"
#include "ui_EMF.h"

#include <QAction>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QPainter>
#include <QPixmap>
#include <QStatusBar>
#include <QToolBar>
#include <QVBoxLayout>
#include <EmfParser.h>
#include <QEmfRenderer.h>
#include "CommProc.h"

extern class QEmf::QEmfRenderer * pRender;
EMF::EMF(QWidget *parent, const  char * Sheme )
    : QMainWindow(parent)
    , ui(new Ui::EMF)
    , m_imageLabel(nullptr)
    , m_scrollArea(nullptr)
  {
  char Path[1024];
  sprintf ( Path, "%sDATA/SCHEME/%s.emf", PROJECT_ROOT, Sheme );
  pMap = NULL;
  ui->setupUi(this);
  setupViewer( Path );
  }

EMF::~EMF()
{
    delete ui;
}

void EMF::setupViewer( char * Path )
{
//setStyleSheet("background-color:black;");
    auto *openAction = new QAction(tr("Open EMF"), this);
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, &EMF::openEmfFile);

    auto *toolbar = addToolBar(tr("File"));
    toolbar->addAction(openAction);

    auto *central = new QWidget(this);
    auto *layout = new QVBoxLayout(central);
    layout->setContentsMargins(0, 0, 0, 0);

    m_imageLabel = new QLabel(central);
    m_imageLabel->setAlignment(Qt::AlignLeft|Qt::AlignTop);
    m_imageLabel->setBackgroundRole(QPalette::Base);
    m_imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    m_imageLabel->setText(tr("Open an EMF file to render it with libqemf."));

    m_scrollArea = new QScrollArea(central);
    m_scrollArea->setWidget(m_imageLabel);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setAlignment(Qt::AlignLeft|Qt::AlignTop);
    m_scrollArea->setBackgroundRole(QPalette::Dark);

    layout->addWidget(m_scrollArea);
    setCentralWidget(central);

    statusBar()->showMessage(tr("libqemf source integration example is ready."));
    setWindowTitle(tr("EMFReader - libqemf example"));
    resize(960, 720);
    QString filePath = Path;
    loadEmfFile(filePath);
}

void EMF::openEmfFile()
{

    const QString startDir = m_currentFilePath.isEmpty()
        ? QDir::homePath()
        : QFileInfo(m_currentFilePath).absolutePath();
    const QString filePath = QFileDialog::getOpenFileName(
        this,
        tr("Open EMF file"),
        startDir,
        tr("Enhanced Metafile (*.emf)")
    );

    if (!filePath.isEmpty()) {
        loadEmfFile(filePath);
    }
}

bool EMF::loadEmfFile(const QString &filePath)
{
  //##pRender = NULL;
    if (!renderEmf(&ss,filePath)) {
        QMessageBox::critical(
            this,
            tr("EMF load error"),
            tr("Failed to load or render EMF file:\n%1").arg(filePath)
        );
        return false;
    }

    m_currentFilePath = filePath;
    setWindowTitle(tr("%1").arg(QFileInfo(filePath).fileName()));
    statusBar()->showMessage(tr("%1").arg(filePath));
    //##pRender->m_painter->translate( 200., 200.);//##
    return true;
}

bool EMF::renderEmf( struct SS * pSS, const QString &filePath)
{
    QEmf::EmfParser parser;
    parser.setParseHeaderOnly();
    if (!parser.load( pSS, filePath)) {
        return false;
    }

    QSize targetSize = parser.bounds().size();
    if (!targetSize.isValid() || targetSize.isEmpty()) {
        targetSize = QSize(1024, 768);
    }

    QPixmap pixmap(targetSize);
    pMap = &pixmap;
    pSS->pixmap = pMap;
    pixmap.fill(Qt::white);

    static QPainter painter;//(&pixmap);
    painter.begin(&pixmap);
    QBrush br(QColor( 0,0,0 ));
    painter.setBrush(br);// setBackground( br );
    painter.drawRect( 0, 0, targetSize.width(), targetSize.height() );
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    QEmf::QEmfRenderer renderer(painter, targetSize, true);
    if (!renderer.load( pSS,filePath)) {
        return false;
    }
    painter.end();
//##
/*
    double L = 681 + 200;
    double R = 959 + 300;
    double T = 487+50;
    double B = 541+50;
    QPixmap PM = pixmap.copy( L, T, R-L, B-T );
    QImage I = PM.toImage();
    QRgb From = qRgb(1, 2, 3);
    QRgb To = qRgb(0, 255, 0);
    for (int y = 0; y < I.height(); ++y)
      {
      QRgb *line = reinterpret_cast<QRgb*>(I.scanLine(y));
      for (int x = 0; x < I.width(); ++x)
        {
        QRgb &rgb = line[x];
//        rgb = qRgba(qRed(rgb), qGreen(0), qBlue(rgb), qAlpha(rgb));
        if ( rgb == From )
          line[x] = To;
        }
      }
   QRectF target(L, T, R-L, B-T);
   QRectF source(0.0, 0.0, R-L, B-T);
   painter.drawImage(target, I, source);
    QPen pen(Qt::yellow, 2, Qt::SolidLine);
    painter.setPen( pen );
    painter.drawLine( L, T, R, T);
    painter.drawLine( R, T, R, B);
    painter.drawLine( R, B, L, B);
    painter.drawLine( L, B, L, T );
/**/
    m_imageLabel->setPixmap(pixmap);
    m_imageLabel->resize(pixmap.size());
    return true;
}

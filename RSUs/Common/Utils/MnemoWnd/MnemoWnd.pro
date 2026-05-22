QT += core gui
QT += widgets

CONFIG += c++17

TEMPLATE = lib

DEFINES += LINUX\
X64

INCLUDEPATH += $$PWD/../../../Common/Include\
$$PWD/../../Include


DESTDIR= $$PWD/../../../../../EXE
HEADERS += \
  QueueGDI\Tele.cpp\
  QueueGDI\Text.cpp\
  QueueGDI\TextColor.cpp\
  QueueGDI\TextExtra.cpp\
  QueueGDI\UVPScreen.cpp\
  QueueGDI\Volume.cpp\
  QueueGDI\Watch.cpp\
  QueueGDI\Word.cpp\
  QueueGDI\alarmrect.cpp\
  QueueGDI\checkbox.cpp\
  QueueGDI\colorcircle.cpp\
  QueueGDI\drum.cpp\
  QueueGDI\kcabtn.cpp\
  QueueGDI\kcacombobox.cpp\
  QueueGDI\kcaedit.cpp\
  QueueGDI\kcaoval.cpp\
  QueueGDI\kcarect.cpp\
  QueueGDI\kcaSpec.cpp\
  QueueGDI\kcaspecbtn.cpp\
  QueueGDI\level.cpp\
  QueueGDI\linearrow.cpp\
  QueueGDI\lodka.cpp\
  QueueGDI\ShowLLLHHH.cpp\
  QueueGDI\Timer.cpp\
  QueueGDI\transrect.cpp\
  QueueGDI\Trend.cpp\
  Железо\Val.cpp\
  Железо\Value.cpp\
  QueueGDI\CloseWindow.cpp\
  QueueGDI\waterbtn.cpp\
  QueueGDI\waterrect.cpp\
  QueueGDI\watervalue.cpp\
  FileNetWork.h\
  ..\..\Include\PipeClient.h\
  ..\..\Include\PipeTrends.h\
  ..\..\Include\Sound.h\
  ..\..\Include\Sterver.h\
  ..\..\Include\INetworkTransport.h\
  ..\..\Include\PipeTransport.h\
  socketclient.h\
  ..\..\Include\TcpTransport.h\
  QueueGDI\Basis21.h
SOURCES += \
  EditScn.cpp\
  Function.cpp\
  MnemoWnd.cpp\
  MouseL.cpp\
  MouseM.cpp\
  MouseSet.cpp\
  QueueAPM.cpp\
  QueueGDI.cpp\
  QueuePnt.cpp\
  ScrollManager.cpp\
  stdafx.cpp\
  Synchronizer.cpp\
  UniLogic.cpp\
  Lang.cpp\
  Parser.cpp\
  SetQueue.cpp\
  SetValue.cpp\
  QueueMnm.cpp\
  QueueTxn.cpp\
  QueueWnd.cpp\
  DrawBeko.cpp\
  FileNetWork.cpp\
  ..\..\..\..\SYSTEM\Source\Utils\Utils\LogFile.cpp\
  PipeClient.cpp\
  PipeTrends.cpp\
  Sound.cpp\
  Sterver.cpp\
  PipeTransport.cpp\
  RWFile.cpp\
  SocketClient.cpp\
  TcpTransport.cpp\
  Queue\Button.cpp\
  Железо\DataSH.cpp\
  Железо\EdgSH.cpp\
  Железо\ForsAvi.cpp\
  Железо\ForsSH.cpp\
  Железо\GasCurve.cpp\
  Железо\GasSH.cpp\
  Железо\HsSH.cpp\
  Железо\KlapSH.cpp\
  Железо\KvoSH.cpp\
  Железо\LabelD.cpp\
  Железо\PolyAvi.cpp\
  Железо\PolySH.cpp\
  Железо\PumpSH.cpp\
  Железо\SensSH.cpp\
  Железо\Show.cpp\
  Железо\ZadvSH.cpp\
  Железо\Close.cpp\
  Железо\Control.cpp\
  Железо\DataST.cpp\
  Железо\EdgST.cpp\
  Железо\ForsST.cpp\
  Железо\GasPR.cpp\
  Железо\HsST.cpp\
  Железо\KlapST.cpp\
  Железо\KvoST.cpp\
  Железо\PolyST.cpp\
  Железо\PresentFE.cpp\
  Железо\PumpST.cpp\
  Железо\SensST.cpp\
  Железо\ValueST.cpp\
  Железо\YokoST.cpp\
  Железо\ZadvST.cpp\
  QueueGDI\_XPEH.cpp\
  QueueGDI\Alarm.cpp\
  QueueGDI\Align.cpp\
  QueueGDI\AOP_trends.cpp\
  QueueGDI\Arc.cpp\
  QueueGDI\Arrow.cpp\
  QueueGDI\Basis12.cpp\
  QueueGDI\Basis21.cpp\
  QueueGDI\Basis21bargraphs.cpp\
  QueueGDI\Basis21combined.cpp\
  QueueGDI\Basis21trend.cpp\
  QueueGDI\Basis21utils.cpp\
  QueueGDI\Bitmap.cpp\
  QueueGDI\Brush.cpp\
  QueueGDI\Btn.cpp\
  QueueGDI\Call.cpp\
  QueueGDI\Circle.cpp\
  QueueGDI\DnoE.cpp\
  QueueGDI\DnoT.cpp\
  QueueGDI\Draw.cpp\
  QueueGDI\Edge.cpp\
  QueueGDI\Elemer.cpp\
  QueueGDI\Ell.cpp\
  QueueGDI\Ellipse.cpp\
  QueueGDI\FileEMF.cpp\
  QueueGDI\FillRect.cpp\
  QueueGDI\Font.cpp\
  QueueGDI\FontScript.cpp\
  QueueGDI\Frame.cpp\
  QueueGDI\Goto.cpp\
  QueueGDI\GradRC.cpp\
  QueueGDI\GradRM.cpp\
  QueueGDI\Grup.cpp\
  QueueGDI\Header.cpp\
  QueueGDI\Horn.cpp\
  QueueGDI\If-Else.cpp\
  QueueGDI\IF.cpp\
  QueueGDI\Image.cpp\
  QueueGDI\Include.cpp\
  QueueGDI\KlapanStateFromValue.cpp\
  QueueGDI\LedTest.cpp\
  QueueGDI\Line.cpp\
  QueueGDI\Lines.cpp\
  QueueGDI\LineTo.cpp\
  QueueGDI\MoveTo.cpp\
  QueueGDI\PAS_01.cpp\
  QueueGDI\PAS_05.cpp\
  QueueGDI\Pen.cpp\
  QueueGDI\Pipe.cpp\
  QueueGDI\PipeF.cpp\
  QueueGDI\PipeH.cpp\
  QueueGDI\PipeV.cpp\
  QueueGDI\Poly.cpp\
  QueueGDI\Rc.cpp\
  QueueGDI\Rect.cpp\
  QueueGDI\Romb.cpp\
  QueueGDI\Round.cpp\
  QueueGDI\Run.cpp\
  QueueGDI\SensKbd.cpp\
  QueueGDI\Sensorika.cpp\
  QueueGDI\Shift.cpp\
  QueueGDI\Slider.cpp\
  QueueGDI\TegscnData.cpp\
  QueueGDI\TegscnTrend.cpp\
  QueueGDI\Tele.cpp\
  QueueGDI\Text.cpp\
  QueueGDI\TextColor.cpp\
  QueueGDI\TextExtra.cpp\
  QueueGDI\UVPScreen.cpp\
  QueueGDI\Volume.cpp\
  QueueGDI\Watch.cpp\
  QueueGDI\Word.cpp\
  QueueGDI\alarmrect.cpp\
  QueueGDI\checkbox.cpp\
  QueueGDI\colorcircle.cpp\
  QueueGDI\drum.cpp\
  QueueGDI\kcabtn.cpp\
  QueueGDI\kcacombobox.cpp\
  QueueGDI\kcaedit.cpp\
  QueueGDI\kcaoval.cpp\
  QueueGDI\kcarect.cpp\
  QueueGDI\kcaSpec.cpp\
  QueueGDI\kcaspecbtn.cpp\
  QueueGDI\level.cpp\
  QueueGDI\linearrow.cpp\
  QueueGDI\lodka.cpp\
  QueueGDI\ShowLLLHHH.cpp\
  QueueGDI\Timer.cpp\
  QueueGDI\transrect.cpp\
  QueueGDI\Trend.cpp\
  Железо\Val.cpp\
  Железо\Value.cpp\
  QueueGDI\CloseWindow.cpp\
  QueueGDI\waterbtn.cpp\
  QueueGDI\waterrect.cpp\
  QueueGDI\watervalue.cpp

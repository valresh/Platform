QT       += core gui

CONFIG += c++17

DESTDIR= "$$PWD/../../../EXE"
TEMPLATE = lib

DEFINES += LINUX\
X64\
NO_CPP

INCLUDEPATH += $$PWD/../../Include

SOURCES += \
adler32.c\
compress.c\
  contrib/minizip/ioapi.c \
  contrib/minizip/iowin32.c \
  contrib/minizip/miniunz.c \
  contrib/minizip/minizip.c \
  contrib/minizip/mztools.c \
  contrib/minizip/unzip.c \
  contrib/minizip/zip.c \
crc32.c\
deflate.c\
gzio.c\
infback.c\
inffast.c\
inflate.c\
inftrees.c\
minigzip.c\
trees.c\
uncompr.c\
zutil.c

HEADERS  += \
  contrib/minizip/crypt.h \
  contrib/minizip/ioapi.h \
  contrib/minizip/iowin32.h \
  contrib/minizip/mztools.h \
  contrib/minizip/unzip.h \
  contrib/minizip/zip.h \
crc32.h\
deflate.h\
inffast.h\
inffixed.h\
inflate.h\
inftrees.h\
trees.h\
zconf.h\
zconf.in.h\
zlib.h\
zutil.h

DISTFILES += \
  contrib/minizip/ChangeLogUnzip \
  contrib/minizip/Makefile

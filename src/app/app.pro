include(../../roc-launcher.pri)

QT       += core gui widgets sql winextras

TARGET = roc_launcher
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += ../libs

DESTDIR = $$build_bin_dir

LIBS += -lshell32 -luser32


SOURCES += \
        main.cpp \
    launcher_widget.cpp \
    database_manager.cpp \
    lnk_tool.cpp \
    input_link_line_edit.cpp

HEADERS += \
    launcher_widget.h \
    database_manager.h \
    link.h \
    lnk_tool.h \
    input_link_line_edit.h

FORMS += \
    launcher_widget.ui

RESOURCES += \
    media.qrc

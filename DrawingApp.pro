QT       += core gui widgets printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# can make the code fail to compile if it uses deprecated APIs.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/canvas.cpp \
    src/layer.cpp \
    src/layermanager.cpp \
    src/tools/tool.cpp \
    src/tools/brushtool.cpp \
    src/tools/pentool.cpp \
    src/tools/penciltool.cpp \
    src/tools/erasertool.cpp \
    src/tools/linetool.cpp \
    src/tools/rectangletool.cpp \
    src/tools/ellipsetool.cpp \
    src/tools/polygontool.cpp \
    src/tools/selecttool.cpp \
    src/tools/texttool.cpp \
    src/tools/filltool.cpp \
    src/tools/spraytool.cpp \
    src/tools/calligraphytool.cpp \
    src/colorpicker.cpp \
    src/brushengine.cpp \
    src/commandhistory.cpp \
    src/commands/drawcommand.cpp \
    src/commands/layercommand.cpp \
    src/filters/filterbase.cpp \
    src/filters/blurfilter.cpp \
    src/filters/sharpenfilter.cpp \
    src/filters/coloradjustfilter.cpp \
    src/dialogs/newdocumentdialog.cpp \
    src/dialogs/preferencesdialog.cpp \
    src/dialogs/aboutdialog.cpp \
    src/widgets/layerwidget.cpp \
    src/widgets/colorwheel.cpp \
    src/widgets/simplecolorpicker.cpp \
    src/widgets/brushpreview.cpp \
    src/widgets/tooloptionswidget.cpp \
    src/widgets/colorpalette.cpp \
    src/filemanager.cpp

HEADERS += \
    src/mainwindow.h \
    src/canvas.h \
    src/layer.h \
    src/layermanager.h \
    src/tools/tool.h \
    src/tools/brushtool.h \
    src/tools/pentool.h \
    src/tools/penciltool.h \
    src/tools/erasertool.h \
    src/tools/linetool.h \
    src/tools/rectangletool.h \
    src/tools/ellipsetool.h \
    src/tools/polygontool.h \
    src/tools/selecttool.h \
    src/tools/texttool.h \
    src/tools/filltool.h \
    src/tools/spraytool.h \
    src/tools/calligraphytool.h \
    src/colorpicker.h \
    src/brushengine.h \
    src/commandhistory.h \
    src/commands/drawcommand.h \
    src/commands/layercommand.h \
    src/filters/filterbase.h \
    src/filters/blurfilter.h \
    src/filters/sharpenfilter.h \
    src/filters/coloradjustfilter.h \
    src/dialogs/newdocumentdialog.h \
    src/dialogs/preferencesdialog.h \
    src/dialogs/aboutdialog.h \
    src/widgets/layerwidget.h \
    src/widgets/colorwheel.h \
    src/widgets/simplecolorpicker.h \
    src/widgets/brushpreview.h \
    src/widgets/tooloptionswidget.h \
    src/widgets/colorpalette.h \
    src/filemanager.h

RESOURCES += \
    resources/resources.qrc

INCLUDEPATH += src

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

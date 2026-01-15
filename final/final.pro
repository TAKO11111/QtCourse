QT       += core gui widgets sql network

CONFIG += c++17

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    dbmanager.cpp \
    tcpclient.cpp \
    tcpmessage.cpp \
    messagemodel.cpp \
    contactmodel.cpp \
    logindialog.cpp  # 新增

HEADERS += \
    mainwindow.h \
    dbmanager.h \
    tcpclient.h \
    tcpmessage.h \
    messagemodel.h \
    contactmodel.h \
    logindialog.h    # 新增

# 添加资源文件（样式表）
RESOURCES += \
    resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    resources/style.qss

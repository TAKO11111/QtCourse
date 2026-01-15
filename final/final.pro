QT       += core gui widgets sql network
CONFIG += c++17
DEFINES += QT_DEPRECATED_WARNINGS

# 客户端程序
TARGET = IM_Client
TEMPLATE = app

# 源文件列表
SOURCES += \
    logindialog.cpp \
    main.cpp \
    mainwindow.cpp \
    dbmanager.cpp \
    tcpclient.cpp \
    tcpmessage.cpp \
    contactmodel.cpp \
    messagemodel.cpp \
    tcpthreadmanager.cpp \


# 头文件列表
HEADERS += \
    logindialog.h \
    mainwindow.h \
    dbmanager.h \
    tcpclient.h \
    tcpmessage.h \
    contactmodel.h \
    messagemodel.h \
    tcpthreadmanager.h

# 服务器编译配置（独立生成）





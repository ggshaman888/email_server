#-------------------------------------------------
#
# Project created by QtCreator 2012-08-08T22:34:43
#
#-------------------------------------------------

QT       += core network
QT       -= gui
QT       += sql

TARGET = ThreadedServer
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    qsimpleserver.cpp \
    providersmtp.cpp \
    serveremail.cpp \
    emailedit.cpp \
    taskthread.cpp \
    timergo.cpp \
    taskconfig.cpp \
    subscribeconfig.cpp \
    receiver.cpp \
    mailout.cpp \
    dbreload.cpp \
    log.cpp \
    transportissubscribe.cpp \
    transportmail.cpp \
    reportsubscribe.cpp \
    config/configprovider.cpp \
    config/errormutt.cpp \
    config/muttconfig.cpp \
    config/subscribeconf.cpp \
    config/mailfilterprovider.cpp \
    config/dbsqliteconf.cpp \
    config/connectdb.cpp \
    config/queuemail.cpp \
    tm.cpp

HEADERS += \
    qsimpleserver.h \
    providersmtp.h \
    serveremail.h \
    emailedit.h \
    taskthread.h \
    timergo.h \
    taskconfig.h \
    subscribeconfig.h \
    receiver.h \
    mailout.h \
    dbreload.h \
    log.h \
    sleep.h \
    transportissubscribe.h \
    transportmail.h \
    reportsubscribe.h \
    manifest.h \
    config/configprovider.h \
    config/errormutt.h \
    config/mutt.h \
    config/muttconfig.h \
    config/subscribeconf.h \
    config/mailfilterprovider.h \
    config/dbsqliteconf.h \
    config/connectdb.h \
    config/queuemail.h \
    tm.h





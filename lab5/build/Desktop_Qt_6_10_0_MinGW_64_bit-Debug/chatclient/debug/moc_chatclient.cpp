/****************************************************************************
** Meta object code from reading C++ file 'chatclient.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../chatclient/chatclient.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'chatclient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN10chatclientE_t {};
} // unnamed namespace

template <> constexpr inline auto chatclient::qt_create_metaobjectdata<qt_meta_tag_ZN10chatclientE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "chatclient",
        "connected",
        "",
        "messagereceived",
        "text",
        "jsonreceived",
        "QJsonObject",
        "docobj",
        "onreadyread",
        "sendmessage",
        "type",
        "connecttoserver",
        "QHostAddress",
        "address",
        "port",
        "disconnectfromhost"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'connected'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'messagereceived'
        QtMocHelpers::SignalData<void(const QString &)>(3, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 4 },
        }}),
        // Signal 'jsonreceived'
        QtMocHelpers::SignalData<void(const QJsonObject &)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 6, 7 },
        }}),
        // Slot 'onreadyread'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'sendmessage'
        QtMocHelpers::SlotData<void(const QString &, const QString &)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 4 }, { QMetaType::QString, 10 },
        }}),
        // Slot 'sendmessage'
        QtMocHelpers::SlotData<void(const QString &)>(9, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { QMetaType::QString, 4 },
        }}),
        // Slot 'connecttoserver'
        QtMocHelpers::SlotData<void(const QHostAddress &, quint16)>(11, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 12, 13 }, { QMetaType::UShort, 14 },
        }}),
        // Slot 'disconnectfromhost'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<chatclient, qt_meta_tag_ZN10chatclientE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject chatclient::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10chatclientE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10chatclientE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10chatclientE_t>.metaTypes,
    nullptr
} };

void chatclient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<chatclient *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->connected(); break;
        case 1: _t->messagereceived((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->jsonreceived((*reinterpret_cast<std::add_pointer_t<QJsonObject>>(_a[1]))); break;
        case 3: _t->onreadyread(); break;
        case 4: _t->sendmessage((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 5: _t->sendmessage((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 6: _t->connecttoserver((*reinterpret_cast<std::add_pointer_t<QHostAddress>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<quint16>>(_a[2]))); break;
        case 7: _t->disconnectfromhost(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (chatclient::*)()>(_a, &chatclient::connected, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (chatclient::*)(const QString & )>(_a, &chatclient::messagereceived, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (chatclient::*)(const QJsonObject & )>(_a, &chatclient::jsonreceived, 2))
            return;
    }
}

const QMetaObject *chatclient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *chatclient::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10chatclientE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int chatclient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void chatclient::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void chatclient::messagereceived(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void chatclient::jsonreceived(const QJsonObject & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}
QT_WARNING_POP

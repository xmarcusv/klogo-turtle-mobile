/****************************************************************************
** Meta object code from reading C++ file 'klogoturtlemobileapp.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/klogoturtlemobileapp.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'klogoturtlemobileapp.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_KlogoTurtleMobileApp_t {
    QByteArrayData data[15];
    char stringdata[229];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_KlogoTurtleMobileApp_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_KlogoTurtleMobileApp_t qt_meta_stringdata_KlogoTurtleMobileApp = {
    {
QT_MOC_LITERAL(0, 0, 20),
QT_MOC_LITERAL(1, 21, 23),
QT_MOC_LITERAL(2, 45, 0),
QT_MOC_LITERAL(3, 46, 19),
QT_MOC_LITERAL(4, 66, 14),
QT_MOC_LITERAL(5, 81, 18),
QT_MOC_LITERAL(6, 100, 21),
QT_MOC_LITERAL(7, 122, 7),
QT_MOC_LITERAL(8, 130, 16),
QT_MOC_LITERAL(9, 147, 14),
QT_MOC_LITERAL(10, 162, 5),
QT_MOC_LITERAL(11, 168, 7),
QT_MOC_LITERAL(12, 176, 17),
QT_MOC_LITERAL(13, 194, 18),
QT_MOC_LITERAL(14, 213, 14)
    },
    "KlogoTurtleMobileApp\0applicationWidthChanged\0"
    "\0portraitModeChanged\0hMarginChanged\0"
    "notifyPortraitMode\0Qt::ScreenOrientation\0"
    "slotRun\0applicationWidth\0isPortraitMode\0"
    "ratio\0hMargin\0sliderHandleWidth\0"
    "sliderHandleHeight\0sliderGapWidth\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_KlogoTurtleMobileApp[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       7,   46, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x05,
       3,    0,   40,    2, 0x05,
       4,    0,   41,    2, 0x05,

 // slots: name, argc, parameters, tag, flags
       5,    1,   42,    2, 0x09,
       7,    0,   45,    2, 0x0a,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 6,    2,
    QMetaType::Void,

 // properties: name, type, flags
       8, QMetaType::Int, 0x00495103,
       9, QMetaType::Bool, 0x00495103,
      10, QMetaType::QReal, 0x00095401,
      11, QMetaType::QReal, 0x00495001,
      12, QMetaType::QReal, 0x00095401,
      13, QMetaType::QReal, 0x00095401,
      14, QMetaType::QReal, 0x00095401,

 // properties: notify_signal_id
       0,
       1,
       0,
       2,
       0,
       0,
       0,

       0        // eod
};

void KlogoTurtleMobileApp::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        KlogoTurtleMobileApp *_t = static_cast<KlogoTurtleMobileApp *>(_o);
        switch (_id) {
        case 0: _t->applicationWidthChanged(); break;
        case 1: _t->portraitModeChanged(); break;
        case 2: _t->hMarginChanged(); break;
        case 3: _t->notifyPortraitMode((*reinterpret_cast< Qt::ScreenOrientation(*)>(_a[1]))); break;
        case 4: _t->slotRun(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (KlogoTurtleMobileApp::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&KlogoTurtleMobileApp::applicationWidthChanged)) {
                *result = 0;
            }
        }
        {
            typedef void (KlogoTurtleMobileApp::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&KlogoTurtleMobileApp::portraitModeChanged)) {
                *result = 1;
            }
        }
        {
            typedef void (KlogoTurtleMobileApp::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&KlogoTurtleMobileApp::hMarginChanged)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject KlogoTurtleMobileApp::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_KlogoTurtleMobileApp.data,
      qt_meta_data_KlogoTurtleMobileApp,  qt_static_metacall, 0, 0}
};


const QMetaObject *KlogoTurtleMobileApp::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *KlogoTurtleMobileApp::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_KlogoTurtleMobileApp.stringdata))
        return static_cast<void*>(const_cast< KlogoTurtleMobileApp*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int KlogoTurtleMobileApp::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = applicationWidth(); break;
        case 1: *reinterpret_cast< bool*>(_v) = isPortraitMode(); break;
        case 2: *reinterpret_cast< qreal*>(_v) = ratio(); break;
        case 3: *reinterpret_cast< qreal*>(_v) = hMargin(); break;
        case 4: *reinterpret_cast< qreal*>(_v) = sliderHandleWidth(); break;
        case 5: *reinterpret_cast< qreal*>(_v) = sliderHandleHeight(); break;
        case 6: *reinterpret_cast< qreal*>(_v) = sliderGapWidth(); break;
        }
        _id -= 7;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setApplicationWidth(*reinterpret_cast< int*>(_v)); break;
        case 1: setIsPortraitMode(*reinterpret_cast< bool*>(_v)); break;
        }
        _id -= 7;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 7;
    } else if (_c == QMetaObject::RegisterPropertyMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void KlogoTurtleMobileApp::applicationWidthChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void KlogoTurtleMobileApp::portraitModeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void KlogoTurtleMobileApp::hMarginChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'mesamobile.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../klogoturtle/mesamobile.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mesamobile.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MesaMobile_t {
    QByteArrayData data[13];
    char stringdata[180];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MesaMobile_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MesaMobile_t qt_meta_stringdata_MesaMobile = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MesaMobile"
QT_MOC_LITERAL(1, 11, 23), // "applicationWidthChanged"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 19), // "portraitModeChanged"
QT_MOC_LITERAL(4, 56, 14), // "hMarginChanged"
QT_MOC_LITERAL(5, 71, 10), // "slotVoltar"
QT_MOC_LITERAL(6, 82, 16), // "applicationWidth"
QT_MOC_LITERAL(7, 99, 14), // "isPortraitMode"
QT_MOC_LITERAL(8, 114, 5), // "ratio"
QT_MOC_LITERAL(9, 120, 7), // "hMargin"
QT_MOC_LITERAL(10, 128, 17), // "sliderHandleWidth"
QT_MOC_LITERAL(11, 146, 18), // "sliderHandleHeight"
QT_MOC_LITERAL(12, 165, 14) // "sliderGapWidth"

    },
    "MesaMobile\0applicationWidthChanged\0\0"
    "portraitModeChanged\0hMarginChanged\0"
    "slotVoltar\0applicationWidth\0isPortraitMode\0"
    "ratio\0hMargin\0sliderHandleWidth\0"
    "sliderHandleHeight\0sliderGapWidth"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MesaMobile[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       7,   38, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x06 /* Public */,
       3,    0,   35,    2, 0x06 /* Public */,
       4,    0,   36,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   37,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,

 // properties: name, type, flags
       6, QMetaType::Int, 0x00495103,
       7, QMetaType::Bool, 0x00495103,
       8, QMetaType::QReal, 0x00095401,
       9, QMetaType::QReal, 0x00495001,
      10, QMetaType::QReal, 0x00095401,
      11, QMetaType::QReal, 0x00095401,
      12, QMetaType::QReal, 0x00095401,

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

void MesaMobile::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MesaMobile *_t = static_cast<MesaMobile *>(_o);
        switch (_id) {
        case 0: _t->applicationWidthChanged(); break;
        case 1: _t->portraitModeChanged(); break;
        case 2: _t->hMarginChanged(); break;
        case 3: _t->slotVoltar(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MesaMobile::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MesaMobile::applicationWidthChanged)) {
                *result = 0;
            }
        }
        {
            typedef void (MesaMobile::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MesaMobile::portraitModeChanged)) {
                *result = 1;
            }
        }
        {
            typedef void (MesaMobile::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MesaMobile::hMarginChanged)) {
                *result = 2;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject MesaMobile::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MesaMobile.data,
      qt_meta_data_MesaMobile,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MesaMobile::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MesaMobile::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MesaMobile.stringdata))
        return static_cast<void*>(const_cast< MesaMobile*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MesaMobile::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
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
        default: break;
        }
        _id -= 7;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setApplicationWidth(*reinterpret_cast< int*>(_v)); break;
        case 1: setIsPortraitMode(*reinterpret_cast< bool*>(_v)); break;
        default: break;
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
void MesaMobile::applicationWidthChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void MesaMobile::portraitModeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void MesaMobile::hMarginChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
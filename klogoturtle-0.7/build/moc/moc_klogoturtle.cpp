/****************************************************************************
** Meta object code from reading C++ file 'klogoturtle.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/klogoturtle.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'klogoturtle.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_KlogoTurtleApp_t {
    QByteArrayData data[13];
    char stringdata[108];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_KlogoTurtleApp_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_KlogoTurtleApp_t qt_meta_stringdata_KlogoTurtleApp = {
    {
QT_MOC_LITERAL(0, 0, 14),
QT_MOC_LITERAL(1, 15, 19),
QT_MOC_LITERAL(2, 35, 0),
QT_MOC_LITERAL(3, 36, 8),
QT_MOC_LITERAL(4, 45, 6),
QT_MOC_LITERAL(5, 52, 7),
QT_MOC_LITERAL(6, 60, 7),
QT_MOC_LITERAL(7, 68, 4),
QT_MOC_LITERAL(8, 73, 4),
QT_MOC_LITERAL(9, 78, 6),
QT_MOC_LITERAL(10, 85, 5),
QT_MOC_LITERAL(11, 91, 5),
QT_MOC_LITERAL(12, 97, 9)
    },
    "KlogoTurtleApp\0slotLanguageChanged\0\0"
    "QAction*\0action\0slotRun\0newFile\0open\0"
    "save\0saveAs\0about\0print\0setScript\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_KlogoTurtleApp[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x09,
       5,    0,   62,    2, 0x0a,
       6,    0,   63,    2, 0x08,
       7,    0,   64,    2, 0x08,
       8,    0,   65,    2, 0x08,
       9,    0,   66,    2, 0x08,
      10,    0,   67,    2, 0x08,
      11,    0,   68,    2, 0x08,
      12,    0,   69,    2, 0x08,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool,
    QMetaType::Bool,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void KlogoTurtleApp::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        KlogoTurtleApp *_t = static_cast<KlogoTurtleApp *>(_o);
        switch (_id) {
        case 0: _t->slotLanguageChanged((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 1: _t->slotRun(); break;
        case 2: _t->newFile(); break;
        case 3: _t->open(); break;
        case 4: { bool _r = _t->save();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 5: { bool _r = _t->saveAs();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 6: _t->about(); break;
        case 7: _t->print(); break;
        case 8: _t->setScript(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAction* >(); break;
            }
            break;
        }
    }
}

const QMetaObject KlogoTurtleApp::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_KlogoTurtleApp.data,
      qt_meta_data_KlogoTurtleApp,  qt_static_metacall, 0, 0}
};


const QMetaObject *KlogoTurtleApp::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *KlogoTurtleApp::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_KlogoTurtleApp.stringdata))
        return static_cast<void*>(const_cast< KlogoTurtleApp*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int KlogoTurtleApp::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

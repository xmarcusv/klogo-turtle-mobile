/****************************************************************************
** Meta object code from reading C++ file 'klogoturtle.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../klogoturtle/klogoturtle.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'klogoturtle.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_KlogoTurtleApp_t {
    QByteArrayData data[12];
    char stringdata[97];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_KlogoTurtleApp_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_KlogoTurtleApp_t qt_meta_stringdata_KlogoTurtleApp = {
    {
QT_MOC_LITERAL(0, 0, 14), // "KlogoTurtleApp"
QT_MOC_LITERAL(1, 15, 19), // "slotLanguageChanged"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 8), // "QAction*"
QT_MOC_LITERAL(4, 45, 6), // "action"
QT_MOC_LITERAL(5, 52, 7), // "slotRun"
QT_MOC_LITERAL(6, 60, 7), // "newFile"
QT_MOC_LITERAL(7, 68, 4), // "open"
QT_MOC_LITERAL(8, 73, 4), // "save"
QT_MOC_LITERAL(9, 78, 6), // "saveAs"
QT_MOC_LITERAL(10, 85, 5), // "about"
QT_MOC_LITERAL(11, 91, 5) // "print"

    },
    "KlogoTurtleApp\0slotLanguageChanged\0\0"
    "QAction*\0action\0slotRun\0newFile\0open\0"
    "save\0saveAs\0about\0print"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_KlogoTurtleApp[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x09 /* Protected */,
       5,    0,   57,    2, 0x0a /* Public */,
       6,    0,   58,    2, 0x08 /* Private */,
       7,    0,   59,    2, 0x08 /* Private */,
       8,    0,   60,    2, 0x08 /* Private */,
       9,    0,   61,    2, 0x08 /* Private */,
      10,    0,   62,    2, 0x08 /* Private */,
      11,    0,   63,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool,
    QMetaType::Bool,
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
      qt_meta_data_KlogoTurtleApp,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *KlogoTurtleApp::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *KlogoTurtleApp::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
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
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
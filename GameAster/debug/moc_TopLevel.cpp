/****************************************************************************
** Meta object code from reading C++ file 'TopLevel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../TopLevel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TopLevel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_KAstTopLevel_t {
    QByteArrayData data[8];
    char stringdata0[96];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_KAstTopLevel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_KAstTopLevel_t qt_meta_stringdata_KAstTopLevel = {
    {
QT_MOC_LITERAL(0, 0, 12), // "KAstTopLevel"
QT_MOC_LITERAL(1, 13, 11), // "slotNewGame"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 14), // "slotShipKilled"
QT_MOC_LITERAL(4, 41, 13), // "slotEraserHit"
QT_MOC_LITERAL(5, 55, 4), // "size"
QT_MOC_LITERAL(6, 60, 18), // "slotErasersRemoved"
QT_MOC_LITERAL(7, 79, 16) // "slotUpdateVitals"

    },
    "KAstTopLevel\0slotNewGame\0\0slotShipKilled\0"
    "slotEraserHit\0size\0slotErasersRemoved\0"
    "slotUpdateVitals"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_KAstTopLevel[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x08 /* Private */,
       3,    0,   40,    2, 0x08 /* Private */,
       4,    1,   41,    2, 0x08 /* Private */,
       6,    0,   44,    2, 0x08 /* Private */,
       7,    0,   45,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void KAstTopLevel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<KAstTopLevel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slotNewGame(); break;
        case 1: _t->slotShipKilled(); break;
        case 2: _t->slotEraserHit((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->slotErasersRemoved(); break;
        case 4: _t->slotUpdateVitals(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject KAstTopLevel::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_KAstTopLevel.data,
    qt_meta_data_KAstTopLevel,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *KAstTopLevel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *KAstTopLevel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_KAstTopLevel.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int KAstTopLevel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

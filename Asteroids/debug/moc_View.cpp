/****************************************************************************
** Meta object code from reading C++ file 'View.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../View.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'View.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_KAsteroidsView_t {
    QByteArrayData data[9];
    char stringdata0[103];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_KAsteroidsView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_KAsteroidsView_t qt_meta_stringdata_KAsteroidsView = {
    {
QT_MOC_LITERAL(0, 0, 14), // "KAsteroidsView"
QT_MOC_LITERAL(1, 15, 10), // "shipKilled"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 9), // "eraserHit"
QT_MOC_LITERAL(4, 37, 4), // "size"
QT_MOC_LITERAL(5, 42, 14), // "erasersRemoved"
QT_MOC_LITERAL(6, 57, 12), // "updateVitals"
QT_MOC_LITERAL(7, 70, 10), // "hideShield"
QT_MOC_LITERAL(8, 81, 21) // "showCaraculaAnimation"

    },
    "KAsteroidsView\0shipKilled\0\0eraserHit\0"
    "size\0erasersRemoved\0updateVitals\0"
    "hideShield\0showCaraculaAnimation"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_KAsteroidsView[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x06 /* Public */,
       3,    1,   45,    2, 0x06 /* Public */,
       5,    0,   48,    2, 0x06 /* Public */,
       6,    0,   49,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   50,    2, 0x08 /* Private */,
       8,    0,   51,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void KAsteroidsView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<KAsteroidsView *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->shipKilled(); break;
        case 1: _t->eraserHit((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->erasersRemoved(); break;
        case 3: _t->updateVitals(); break;
        case 4: _t->hideShield(); break;
        case 5: _t->showCaraculaAnimation(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (KAsteroidsView::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KAsteroidsView::shipKilled)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (KAsteroidsView::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KAsteroidsView::eraserHit)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (KAsteroidsView::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KAsteroidsView::erasersRemoved)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (KAsteroidsView::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KAsteroidsView::updateVitals)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject KAsteroidsView::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_KAsteroidsView.data,
    qt_meta_data_KAsteroidsView,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *KAsteroidsView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *KAsteroidsView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_KAsteroidsView.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int KAsteroidsView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void KAsteroidsView::shipKilled()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void KAsteroidsView::eraserHit(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void KAsteroidsView::erasersRemoved()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void KAsteroidsView::updateVitals()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

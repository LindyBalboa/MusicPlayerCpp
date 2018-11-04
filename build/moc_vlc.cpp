/****************************************************************************
** Meta object code from reading C++ file 'vlc.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../MusicPlayerCpp/vlc.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'vlc.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_VLC_t {
    QByteArrayData data[13];
    char stringdata0[110];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VLC_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VLC_t qt_meta_stringdata_VLC = {
    {
QT_MOC_LITERAL(0, 0, 3), // "VLC"
QT_MOC_LITERAL(1, 4, 10), // "endReached"
QT_MOC_LITERAL(2, 15, 0), // ""
QT_MOC_LITERAL(3, 16, 13), // "lengthChanged"
QT_MOC_LITERAL(4, 30, 11), // "timeChanged"
QT_MOC_LITERAL(5, 42, 18), // "handleDeviceChange"
QT_MOC_LITERAL(6, 61, 6), // "device"
QT_MOC_LITERAL(7, 68, 5), // "pause"
QT_MOC_LITERAL(8, 74, 4), // "play"
QT_MOC_LITERAL(9, 79, 8), // "setPause"
QT_MOC_LITERAL(10, 88, 9), // "setVolume"
QT_MOC_LITERAL(11, 98, 6), // "volume"
QT_MOC_LITERAL(12, 105, 4) // "stop"

    },
    "VLC\0endReached\0\0lengthChanged\0timeChanged\0"
    "handleDeviceChange\0device\0pause\0play\0"
    "setPause\0setVolume\0volume\0stop"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VLC[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x06 /* Public */,
       3,    1,   60,    2, 0x06 /* Public */,
       4,    1,   63,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   66,    2, 0x0a /* Public */,
       7,    0,   69,    2, 0x0a /* Public */,
       8,    0,   70,    2, 0x0a /* Public */,
       9,    0,   71,    2, 0x0a /* Public */,
      10,    1,   72,    2, 0x0a /* Public */,
      12,    0,   75,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Bool,
    QMetaType::Void, QMetaType::LongLong,    2,
    QMetaType::Void, QMetaType::LongLong,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::LongLong,   11,
    QMetaType::Void,

       0        // eod
};

void VLC::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VLC *_t = static_cast<VLC *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: { bool _r = _t->endReached();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 1: _t->lengthChanged((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 2: _t->timeChanged((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 3: _t->handleDeviceChange((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->pause(); break;
        case 5: _t->play(); break;
        case 6: _t->setPause(); break;
        case 7: _t->setVolume((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 8: _t->stop(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = bool (VLC::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VLC::endReached)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (VLC::*)(qint64 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VLC::lengthChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (VLC::*)(qint64 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VLC::timeChanged)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject VLC::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_VLC.data,
      qt_meta_data_VLC,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *VLC::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VLC::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VLC.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int VLC::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
bool VLC::endReached()
{
    bool _t0{};
    void *_a[] = { const_cast<void*>(reinterpret_cast<const void*>(&_t0)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
    return _t0;
}

// SIGNAL 1
void VLC::lengthChanged(qint64 _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void VLC::timeChanged(qint64 _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

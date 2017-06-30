#ifndef GLOBALS_H
#define GLOBALS_H

#include <QSqlDatabase>

#ifdef DEFINE_GLOBALS
#define EXTERN //empty
#else
#define EXTERN extern
#endif //DEFINE_GLOBALS

EXTERN QSqlDatabase libraryDb;

#endif // GLOBALS_H

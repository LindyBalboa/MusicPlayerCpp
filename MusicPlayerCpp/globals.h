#ifndef GLOBALS_H
#define GLOBALS_H

#ifdef DEFINE_GLOBALS
#define EXTERN //empty
#else
#define EXTERN extern
#endif DEFINE_GLOBALS

#include <QSqlDatabase>

EXTERN QSqlDatabase libraryDb;

#endif // GLOBALS_H

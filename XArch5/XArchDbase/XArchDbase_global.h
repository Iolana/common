#ifndef XARCHDBASE_GLOBAL_H
#define XARCHDBASE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(XARCHDBASE_LIBRARY)
#  define XARCHDBASESHARED_EXPORT Q_DECL_EXPORT
#else
#  define XARCHDBASESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // XARCHDBASE_GLOBAL_H

#ifndef ANALYZE_GLOBAL_H
#define ANALYZE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ANALYZE_LIBRARY)
#  define ANALYZE_EXPORT Q_DECL_EXPORT
#else
#  define ANALYZE_EXPORT Q_DECL_IMPORT
#endif

#endif // ANALYZE_GLOBAL_H

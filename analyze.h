#ifndef ANALYZE_H
#define ANALYZE_H

#include "analyze_global.h"
#include <QMap>
#include <QJsonObject>
#include "structures.h"

using namespace Structures;

class ANALYZE_EXPORT Analyze
{
public:
    //static void analyze(ProjectData &Project);
    static QMap<QString, complex> analyze(QMap<QString,Structures::complex> list_elements,
                        QString project_path);
    static QMap<QString, QStringList> get_actors_list(QMap<QString,Structures::complex> list_elements,
                                                     QString project_path);
};





#endif // ANALYZE_H

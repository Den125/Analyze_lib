#include "structures.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>

QString Structures::type(DiagramType type_d)
{
    QMap<DiagramType,QString> map_type ={
        {use_case,"use_cases"},{robustness,"robustness"},{sequence,"sequence"},{classes,"classes"}
    };
    return map_type.value(type_d);
}

Structures::DiagramType Structures::type(QString type_d)
{
    QMap<QString,DiagramType> map_type ={
        {"use_cases",use_case},{"robustness",robustness},{"sequence",sequence},{"classes",classes}
    };
    return map_type.value(type_d);
}


void Structures::setFalseAllStatus(ProjectData &Project)
{
    foreach(QString key,Project.mapElements.keys())
    {
        if (Project.mapElements[key].type!=Structures::use_case && Project.mapElements[key].type!=Structures::classes)
        {
            Project.mapElements[key].status=false;
        }
    }
}


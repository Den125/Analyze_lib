#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <QString>
#include <QMap>
#include <QVector>

namespace Structures
{
    enum DiagramType
    {
        use_case,
        robustness,
        sequence,
        classes
    };

    struct complex
    {
        DiagramType type;
        bool status;
    };

    DiagramType type(QString type_d);
    QString type(DiagramType type_d);

    struct ProjectData
    {
        friend class FileProject;
        QMap<QString, Structures::complex> mapElements;
        QString path;
        ProjectData()
        {

        }
    private:

        ProjectData(QMap<QString,Structures::complex> m_mapElements,QString m_path)
        {
            path=m_path;
            mapElements=m_mapElements;
        }
    };
    void setFalseAllStatus(ProjectData &Project);
}

#endif // STRUCTURES_H

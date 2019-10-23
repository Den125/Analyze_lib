#include <QRegExp>
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include "analyzing.h"

QStringList analyze_robustness_file(QString str);
QStringList analyze_description_main(QMap<QString,Structures::complex>* list_elements, QString key, QJsonObject main, QStringList objects, ProjectData Project);
QStringList analyze_description_alt(QMap<QString,Structures::complex>* list_elements, QString key, QJsonObject alt, QStringList object, ProjectData Projects);


QStringList analyze_robustness_file(QString str)
{
    QStringList objects;
    QRegExp objects_all("\\b(actor |boundary |entity |control ).*(?=\\n)");
    objects_all.setMinimal(true);
    objects_all.setCaseSensitivity(Qt::CaseInsensitive);
    int index=0;
    while ((index= objects_all.indexIn(str,index))!=-1)
    {
        QStringList captured_object=objects_all.cap().split(" ");
        if (captured_object[0]!="actor")
        {
            if (captured_object.length()==2)
            {
                captured_object<<"as";
                captured_object<<captured_object[1];
            }
            captured_object[1]="\":"+captured_object[1]+"\"";
        }
        index+=objects_all.cap().length();
        objects<<captured_object.join(" ");
    }
    return objects;
}
QStringList analyze_description_main(QMap<QString,Structures::complex>* list_elements, QString key, QJsonObject main, QStringList objects, ProjectData Project)
{
    QString str;
    QStringList sequences;
    const QString format_text=".txt",format_picture=".png";
    str.append("Главная последовательность:\n");
    for (int i=0;i<main.keys().count();i++)
    {
        str.append(QString::number(i+1)+") "+main.value(QString::number(i+1)).toString()+"\n");
    }
    sequences<<str;
    list_elements->insert(key+"_main",{Structures::DiagramType::sequence,true});
    QFile newfile(Project.path+"/"+Structures::type(Structures::DiagramType::sequence)+"/"+key+"_main"+format_text);
    if (newfile.open(QIODevice::ReadWrite))
    {
        QTextStream text(&newfile);
        QString file_text;
        if (newfile.size()!=0)
        {
            file_text=text.readAll();
            QRegExp note("note as scenario.*end note");
            int index=0;
            //TODO Настроить добавление объектов при созданном файле
            if ((index= note.indexIn(file_text,index))!=-1)
            {
                file_text.replace(index,note.matchedLength(),"note as scenario\n"+str+"\nend note");
                newfile.resize(0);
                text<<file_text;
            }
            else
            {
                file_text.chop(7);
                file_text.append("note as scenario\n "+str+"\nend note");
                newfile.resize(0);
                text<<file_text<<"\n@enduml";
            }
        }
        else
        {
            text<<"@startuml\nnote as scenario\n"<<str<<"end note\n"<<objects.join('\n')<<"\n@enduml";
        }
        newfile.close();
    }
    return sequences;
}

QStringList analyze_description_alt(QMap<QString,Structures::complex>* list_elements, QString key, QJsonObject alt, QStringList objects, ProjectData Project)
{
    QStringList sequences;
    const QString format_text=".txt",format_picture=".png";
    if (!alt.keys().empty())
    {
        QString str;
        for (int i=0;i<alt.keys().count();i++)
        {
            QJsonObject alt_seq=alt.value(QString::number(i+1)).toObject();
            str.append("Альтернативная последовательность 1:\n");
            for (int j=0;j<alt_seq.keys().count()-1;j++)
            {
                str.append(QString::number(j+1)+") "+alt_seq.value(QString::number(j+1)).toString()+"\n");
            }
            list_elements->insert(key+"_alt"+QString::number(i),{Structures::sequence,true});
            sequences<<str;
            QFile newfile(Project.path+"/"+Structures::type(Structures::sequence)+"/"+key+"_alt"+QString::number(i)+format_text);
            if (newfile.open(QIODevice::WriteOnly))
            {
                QTextStream text(&newfile);
                QString file_text;
                if (newfile.size()!=0)
                {
                    file_text=text.readAll();
                    QRegExp note("note left.*end note");
                    int index=0;
                    if ((index= note.indexIn(file_text,index))!=-1)
                    {
                        file_text.replace(index,note.matchedLength()-17,'\n'+str+'\n');
                        newfile.resize(0);
                        text<<file_text;
                    }
                    else
                    {
                        file_text.chop(7);
                        file_text.append("note left\n "+str+"\n end note");
                        newfile.resize(0);
                        text<<file_text<<objects.join(' ')<<"\n@enduml";
                    }
                }
                else
                {
                    text<<"@startuml\nnote left\n"<<str<<"end note\n"<<objects.join('\n')<<"\n@enduml";
                }
                newfile.close();
            }
        }

    }
    return sequences;
}

QMap<QString,Structures::complex> analyze_robustness_diagram(QString filename, ProjectData Project)
{
    QStringList objects,sequences;
    const QString format_text=".txt",format_picture=".png";
    QString str,file_str;
    QMap<QString,Structures::complex> list_elements;
    QFile file(Project.path+"/"+Structures::type(Structures::robustness)+"/"+filename+format_text);
    if (file.open(QIODevice::ReadOnly))
    {
        file_str=file.readAll();
        objects=analyze_robustness_file(file_str);
    }
    QFile desc(Project.path+"/"+"description.json");
    if (desc.exists())
    {
        if (desc.open(QIODevice::ReadOnly))
        {
            QJsonDocument doc = QJsonDocument::fromJson(desc.readAll());
            if (doc.isNull())
            {
            // qDebug()<<"Ошибка чтения JSON";
                return list_elements;
            }
            QJsonObject root = doc.object().value(filename).toObject();
            QJsonObject main=root.value("main_seq").toObject();
            sequences<<analyze_description_main(&list_elements,filename,main,objects,Project);
            QJsonObject alt=root.value("alt_seq").toObject();
            sequences<<analyze_description_alt(&list_elements,filename,alt,objects,Project);
            desc.close();
        }
        QRegExp note("\nnote as scenario.*end note\n");
        note.setMinimal(true);
        int index=0;
        if ((index=note.indexIn(file_str,index))!=-1)
        {
            file_str.replace(index,note.matchedLength(),"\nnote as scenario\n"+sequences.join("\n")+"\nend note\n");
        }
        else
        {
            file_str.insert(9,"\nnote as scenario\n"+sequences.join("\n")+"\nend note\n");
        }
        file.close();
        if (file.open(QIODevice::WriteOnly))
        {
            file.resize(0);
            QTextStream seq_write(&file);
            seq_write<<file_str;
            file.close();
        }
    }
    return list_elements;
}


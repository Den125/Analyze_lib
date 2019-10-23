#include "analyze.h"
#include <QFile>
#include <QTextStream>

QMap<QString,QStringList> analyze_sequence_file(QString str);

QMap<QString,QStringList> analyze_sequence_file(QString str)
{
    QMap<QString,QStringList> class_list;
    QRegExp objects("\".*:.*\"\\sas\\s.*(?=\\n)");
    objects.setMinimal(true);
    int index=0;
    while ((index= objects.indexIn(str,index))!=-1)
    {
        QString class_name;
        class_name=objects.cap().split(" ")[0];
        class_name=class_name.right(class_name.length()-class_name.indexOf(":",0)-1);
        class_name.chop(1);
        QString class_psevdo=objects.cap().split(" ")[2];
        QRegExp class_method("("+class_psevdo+"\\s-\\W{1,}\\s\\w*|\\w*\\s<\\W{1,}\\s"+class_psevdo+")\\s:.*\\n");
        class_method.setMinimal(true);
        QStringList methods;
        int index2=index;
        while ((index2=class_method.indexIn(str,index2))!=-1)
        {
            QStringList method=class_method.cap().split(" : ");
            method[1].chop(1);
            methods<<method[1]+"()";
            index2+=class_method.matchedLength();
        }
        class_list.insert(class_name,methods);
        index=index+objects.matchedLength();
    }
    return class_list;
}

QMap<QString,QStringList> analyze_sequence_diagram(QString filename, ProjectData Project)
{
    const QString format_text=".txt",format_picture=".png";
    QFile diagramFile(Project.path+"/"+Structures::type(Structures::sequence)+"/"+filename+format_text);
    QMap<QString,QStringList> class_list;
    if (!diagramFile.open(QIODevice::ReadOnly))
        return class_list;
    QString file_str=diagramFile.fileName();
    class_list=analyze_sequence_file(file_str);
    QFile model(Project.path+"/"+Structures::type(Structures::DiagramType::classes)+"/"+"model.txt");
    if (model.open(QIODevice::ReadWrite))
    {
        QString model_file=model.readAll();
        foreach (QString name_class,class_list.keys())
        {
            //TODO сделай реализацию
            QRegExp find_class("class "+name_class+"\\{.*\\}");
            int index=0;
            if ((index=find_class.indexIn(model_file,index))!=-1)
            {

            }
            else
            {
                model_file.insert(9,"\nclass "+name_class+"{\n"+class_list.value(name_class).join('\n')+"\n}");
            }
        }
        model.resize(0);
        QTextStream in_model(&model);
        in_model<<model_file;
        model.close();
    }
    return class_list;
}

#include "mainwindow.h"
#include "customs.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QDebug>
#include <QString>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    QString q_lang;
    Customs g_customs("ECGDisplaySystem");
    g_customs.getCfgValue("View", "language", q_lang);
    if (0 == QString::compare(q_lang, "zh_CN"))
    {
        const QString baseName = "ECGDisplaySystem_zh_CN.qm";
        qDebug() << baseName;
        translator.load(":/i18n/" + baseName);
        a.installTranslator(&translator);
    }
    //    const QStringList uiLanguages = QLocale::system().uiLanguages();
    //    for (const QString &locale : uiLanguages)
    //    {
    //        const QString baseName = "ECGDisplaySystem_" + QLocale(locale).name() + ".qm";
    //        qDebug() << baseName;
    //        if (translator.load(":/i18n/" + baseName))
    //        {
    //            a.installTranslator(&translator);
    //            break;
    //        }
    //    }
    MainWindow w;
    w.show();

    QString q_font_size;
    g_customs.getCfgValue("view", "FontSize", q_font_size);
    QFont font = a.font();
    font.setPointSize((q_font_size.toInt() >= 8 && q_font_size.toInt() <= 18)? q_font_size.toInt():12);
    a.setFont(font);

    return a.exec();
}

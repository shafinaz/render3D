#include "mainwindow.h"
#include <QMessageBox>
#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    QString filename = "/Users/shafinazyasin/Desktop/GITHUB/Render3D/object.txt";

    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "simple2_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    MainWindow w(nullptr, filename);
    w.show();
    return a.exec();
}


#include <QApplication>
#include <QTime>
#include "TopLevel.h"

int main(int argc, char **argv)
{
    //Q_INIT_RESOURCE(portedasteroids);

    QApplication app(argc, argv);

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    KAstTopLevel topLevel;
    topLevel.setWindowTitle("Hello World");

    topLevel.show();

    app.setQuitOnLastWindowClosed(true);
    return app.exec();
}

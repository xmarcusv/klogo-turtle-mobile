#include "klogoturtle.h"
#include "klogoturtlemobileapp.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setApplicationName("Klogo-Turtle-Mobile");
    QApplication::setApplicationVersion("0.7");
    QApplication::setOrganizationName("Klogo");
    QApplication::setOrganizationDomain("https://github.com/xmarcusv/klogo-turtle-mobile");

#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS) || defined(Q_OS_BLACKBERRY)
    KlogoTurtleMobileApp w;
#else
    KlogoTurtleApp w;
#endif
    w.show();
    return a.exec();
}

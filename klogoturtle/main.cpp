#include "klogoturtle.h"
#include "klogoturtlemobileapp.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS) || defined(Q_OS_BLACKBERRY)
    KlogoTurtleMobileApp w;
#else
    KlogoTurtleApp w;
#endif
    w.show();
    return a.exec();
}

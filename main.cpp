#include "form.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Form w;
    w.setWindowTitle("Hand Gesture");
    w.show();

    return a.exec();
}

#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include "displayview.h"

namespace Ui {
    class Form;
}

class Form : public QWidget
{
    Q_OBJECT
public:
    explicit Form(QWidget *parent = 0);
    ~Form();

signals:

public slots:


private:
    DisplayView *ui;

};

#endif // FORM_H

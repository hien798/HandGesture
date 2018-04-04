#include "form.h"

Form::Form(QWidget *parent) : QWidget(parent)
{
    ui = new DisplayView(this);

}

Form::~Form()
{
    // dealloc form
//    delete this;
    ui->~DisplayView();
}

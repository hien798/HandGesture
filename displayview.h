#ifndef DISPLAYVIEW_H
#define DISPLAYVIEW_H

#include <QWidget>
#include "ui_form.h"
#include <iostream>
#include <QTimer>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
using namespace cv;
using namespace std;

class DisplayView : public QWidget, public Ui::Form
{
    Q_OBJECT
public:
    explicit DisplayView(QWidget *parent = 0);
    ~DisplayView();

signals:

public slots:
    void on_buttonStart_clicked();
    void on_buttonExit_clicked();

private:
    CvCapture *cam;
    IplImage *iplImg;
    Mat img;
    Mat img_threshold;
    Mat img_gray;
    Mat img_roi;
    int result;
    bool mode = false;

protected:
    void timerEvent(QTimerEvent*);

};

#endif // DISPLAYVIEW_H

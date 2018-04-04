#include "displayview.h"

DisplayView::DisplayView(QWidget *parent) : QWidget(parent)
{
    setupUi(parent);
    cam = cvCreateCameraCapture(1);
    if (!cam)
        cam = cvCaptureFromCAM(0);
    assert(cam);
    startTimer(100);

    connect(buttonStart,SIGNAL(clicked()),this,SLOT(on_buttonStart_clicked()));
    connect(buttonExit,SIGNAL(clicked()),this,SLOT(on_buttonExit_clicked()));
}

DisplayView::~DisplayView() {
    delete this;
}

void DisplayView::on_buttonStart_clicked() {
    cout << "Start" << endl;

    if (!mode) {
        namedWindow("grayView", CV_WINDOW_AUTOSIZE);
        namedWindow("roiView", CV_WINDOW_AUTOSIZE);
        namedWindow("thresholdView", CV_WINDOW_AUTOSIZE);
        buttonStart->setText("Stop");
        mode = true;
    } else {
        destroyAllWindows();
        buttonStart->setText("Start");
        mode = false;
    }
}

void DisplayView::on_buttonExit_clicked() {
    cout << "Exit" << endl;
//    this->~DisplayView();
    this->on_buttonStart_clicked();
    qApp->exit();
}

void DisplayView::timerEvent(QTimerEvent*) {

    iplImg = cvQueryFrame(cam);
    img = cvarrToMat(iplImg);
    cvtColor(img, img, CV_BGR2RGB);
    if (mode){
        Rect roi(300,100,300,300);
        img_roi = img(roi);
        cvtColor(img_roi, img_gray, CV_RGB2GRAY);
        GaussianBlur(img_gray, img_gray, Size(19,19), 0.0);
        threshold(img_gray, img_threshold, 0, 255, THRESH_BINARY_INV+THRESH_OTSU);


        vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;
        findContours(img_threshold, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point());
        if (contours.size() > 0) {
            size_t indexOfBiggestContour = -1;
            size_t sizeOfBiggestContour = 0;

            for (size_t i=0; i<contours.size(); i++) {
                if (contours[i].size() > sizeOfBiggestContour) {
                    sizeOfBiggestContour = contours[i].size();
                    indexOfBiggestContour = i;
                }
            }

            vector<vector<int>> hull(contours.size());
            vector<vector<Point>> hullpoint(contours.size());
            vector<vector<Vec4i>> defects(contours.size());
            vector<vector<Point>> defectPoint(contours.size());
            vector<vector<Point>> contours_poly(contours.size());
            Point2f rect_point[4];
            vector<RotatedRect> minRect(contours.size());
            vector<Rect> boundRect(contours.size());

            for (size_t i=0; i<contours.size(); i++) {
                if (contourArea(contours[i]) > 5000) {
                    convexHull(contours[i], hull[i], true);
                    convexityDefects(contours[i], hull[i], defects[i]);
                    if (indexOfBiggestContour == i) {
                        minRect[i] = minAreaRect(contours[i]);
                        for (size_t k=0; k<hull[i].size(); k++) {
                            int ind = hull[i][k];
                            hullpoint[i].push_back(contours[i][ind]);
                        }
                        result = 0;
                        for (size_t k=0; k<defects[i].size(); k++) {
                            if (defects[i][k][3] > 13*256) {
                                int p_end = defects[i][k][1];
                                int p_far = defects[i][k][2];
                                defectPoint[i].push_back(contours[i][p_far]);
                                circle(img_roi, contours[i][p_end], 3, Scalar(0,255,0), 2);
                                result++;
                            }
                        }

                        if (result == 1) {
                            labelNumber->setText("1");
                        } else if(result == 2) {
                            labelNumber->setText("2");
                        } else if(result == 3) {
                            labelNumber->setText("3");
                        } else if(result == 4) {
                            labelNumber->setText("4");
                        } else if(result == 5) {
                            labelNumber->setText("5");
                        } else {
                            labelNumber->setText("X");
                        }

                        drawContours(img_threshold, contours, i, Scalar(255,255,0), 2, 8, vector<Vec4i>(), 0, Point());
                        drawContours(img_threshold, hullpoint, i, Scalar(0,0,0), 1, 8, vector<Vec4i>(), 0, Point());
                        drawContours(img_roi, hullpoint, i, Scalar(0,0,255), 2, 8, vector<Vec4i>(), 0, Point());
                        approxPolyDP(contours[i], contours_poly[i], 3, false);
                        boundRect[i] = boundingRect(contours_poly[i]);
                        rectangle(img_roi, boundRect[i].tl(), boundRect[i].br(), Scalar(255,0,0), 2, 8, 0);
                        minRect[i].points((rect_point));
                        for (size_t k=0; k<4; k++) {
                            cv::line(img_roi, rect_point[k], rect_point[(k+1)%4], Scalar(0,255,0), 2, 8, 0);
                        }

                    }
                }
            }

            QImage imgIn= QImage((uchar*) img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
            labelCameraView->setPixmap(QPixmap::fromImage(imgIn));
            imshow("grayView", img_gray);
            cvtColor(img_roi, img_roi, CV_BGR2RGB);
            imshow("roiView", img_roi);
            imshow("thresholdView", img_threshold);

        }
    } else {
        QImage imgIn= QImage((uchar*) img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
        labelCameraView->setPixmap(QPixmap::fromImage(imgIn));
    }

}

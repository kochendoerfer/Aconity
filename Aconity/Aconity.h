#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Aconity.h"
#include <opencv2\core\core.hpp>
#include <opencv2\features2d\features2d.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\imgcodecs\imgcodecs.hpp>

class Aconity : public QMainWindow
{
	Q_OBJECT

public:
	Aconity(QWidget *parent = Q_NULLPTR);

public slots:
	void loadtb1();
	void threshtb1();
	void morphfirsttb1();
	void morphsecondtb1();
	void loadtb2();
	void threshtb2();
	void morphfirsttb2();
	void morphsecondtb2();
	void morphfirstmain();
	void morphsecondmain();
	void mainBitwise();
	void mainMask();
	void findSquares();

private:
	Ui::AconityClass ui;
	cv::Mat src1;
	cv::Mat thr1;
	cv::Mat firstMorph1;
	cv::Mat secondMorph1;
	cv::Mat resImg1;
	cv::Mat src2;
	cv::Mat thr2;
	cv::Mat firstMorph2;
	cv::Mat secondMorph2;
	cv::Mat resImg2;
	cv::Mat bitWise;
	cv::Mat firstMorph3;
	cv::Mat secondMorph3;
	cv::Mat resImg3;
	cv::Mat maskImg;
};

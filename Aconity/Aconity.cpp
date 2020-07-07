#include "Aconity.h"

double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0)
{
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;
	return (dx1*dx2 + dy1 * dy2) / sqrt((dx1*dx1 + dy1 * dy1)*(dx2*dx2 + dy2 * dy2) + 1e-10);
}

Aconity::Aconity(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	//set ui
	//tb1
	ui.tb1leLoad->setText("H:/WorkDir/AufnahmenAconity/S1_M0_1000us.bmp");
	ui.tb1cbStructType->addItem("Rechteck");
	ui.tb1cbStructType->addItem("Kreuz");
	ui.tb1cbStructType->addItem("Ellipse");
	ui.tb1cbOperation->addItem("Erosion");
	ui.tb1cbOperation->addItem("Dilatation");
	ui.tb1cbOperation->addItem("Öffnung");
	ui.tb1cbOperation->addItem("Schliessung");
	ui.tb1cbStructType_2->addItem("Rechteck");
	ui.tb1cbStructType_2->addItem("Kreuz");
	ui.tb1cbStructType_2->addItem("Ellipse");
	ui.tb1cbOperation_2->addItem("Erosion");
	ui.tb1cbOperation_2->addItem("Dilatation");
	ui.tb1cbOperation_2->addItem("Öffnung");
	ui.tb1cbOperation_2->addItem("Schliessung");
	//tb2
	ui.tb2leLoad->setText("H:/WorkDir/AufnahmenAconity/S1_M0_1000us.bmp");
	ui.tb2cbStructType->addItem("Rechteck");
	ui.tb2cbStructType->addItem("Kreuz");
	ui.tb2cbStructType->addItem("Ellipse");
	ui.tb2cbOperation->addItem("Erosion");
	ui.tb2cbOperation->addItem("Dilatation");
	ui.tb2cbOperation->addItem("Öffnung");
	ui.tb2cbOperation->addItem("Schliessung");
	ui.tb2cbStructType_2->addItem("Rechteck");
	ui.tb2cbStructType_2->addItem("Kreuz");
	ui.tb2cbStructType_2->addItem("Ellipse");
	ui.tb2cbOperation_2->addItem("Erosion");
	ui.tb2cbOperation_2->addItem("Dilatation");
	ui.tb2cbOperation_2->addItem("Öffnung");
	ui.tb2cbOperation_2->addItem("Schliessung");
	//main
	ui.maincbStructType->addItem("Rechteck");
	ui.maincbStructType->addItem("Kreuz");
	ui.maincbStructType->addItem("Ellipse");
	ui.maincbOperation->addItem("Erosion");
	ui.maincbOperation->addItem("Dilatation");
	ui.maincbOperation->addItem("Öffnung");
	ui.maincbOperation->addItem("Schliessung");
	ui.maincbStructType_2->addItem("Rechteck");
	ui.maincbStructType_2->addItem("Kreuz");
	ui.maincbStructType_2->addItem("Ellipse");
	ui.maincbOperation_2->addItem("Erosion");
	ui.maincbOperation_2->addItem("Dilatation");
	ui.maincbOperation_2->addItem("Öffnung");
	ui.maincbOperation_2->addItem("Schliessung");
	ui.maincbBitwise->addItem("OR");
	////connects
	connect(ui.tb1pbLoadImage, SIGNAL(clicked()), this, SLOT(loadtb1()));
	connect(ui.tb1pbShowFirst, SIGNAL(clicked()), this, SLOT(morphfirsttb1()));
	connect(ui.tb1pbShowSecond, SIGNAL(clicked()), this, SLOT(morphsecondtb1()));
	connect(ui.tb1pbShowThresh, SIGNAL(clicked()), this, SLOT(threshtb1()));
	connect(ui.mainpbContour, SIGNAL(clicked()), this, SLOT(findSquares()));
	connect(ui.tb2pbLoadImage, SIGNAL(clicked()), this, SLOT(loadtb2()));
	connect(ui.tb2pbShowFirst, SIGNAL(clicked()), this, SLOT(morphfirsttb2()));
	connect(ui.tb2pbShowSecond, SIGNAL(clicked()), this, SLOT(morphsecondtb2()));
	connect(ui.tb2pbShowThresh, SIGNAL(clicked()), this, SLOT(threshtb2()));

	connect(ui.mainpbShowFirst, SIGNAL(clicked()), this, SLOT(morphfirstmain()));
	connect(ui.mainpbShowSecond, SIGNAL(clicked()), this, SLOT(morphsecondmain()));
	connect(ui.mainpbBitwise, SIGNAL(clicked()), this, SLOT(mainBitwise()));
	connect(ui.mainpbMask, SIGNAL(clicked()), this, SLOT(mainMask()));
}

void Aconity::loadtb1()
{

	std::string path = ui.tb1leLoad->text().toStdString();
	src1 = cv::imread(path, cv::IMREAD_GRAYSCALE);
	src1.convertTo(src1, CV_8U);
	ui.tb1lblimage->setPixmap(QPixmap::fromImage(QImage(src1.data, src1.cols, src1.rows, src1.step, QImage::Format_Grayscale8)));
}

void Aconity::threshtb1()
{
	//convert to binary
	int threshFlag = 0;
	if (ui.tb1cbInverted->isChecked())
	{
		threshFlag = cv::THRESH_BINARY_INV;
	}
	else
	{
		threshFlag = cv::THRESH_BINARY;
	}
	thr1 = cv::Mat(src1.rows, src1.cols, src1.type());
	if (ui.tb1cbOTSU->isChecked())
	{
		cv::threshold(src1, thr1, 0, 255, threshFlag | cv::THRESH_OTSU);
	}
	else
	{
		cv::threshold(src1, thr1, ui.tb1sbThreshold->value(), 255, threshFlag);
	}
	ui.tb1lblimage->setPixmap(QPixmap::fromImage(QImage(thr1.data, thr1.cols, thr1.rows, thr1.step, QImage::Format_Grayscale8)));
}

void Aconity::morphfirsttb1()
{
	int structElement = 0;
	int structElemSizeX = ui.tb1sbStructSizeX->value();
	int structElemSizeY = ui.tb1sbStructSizeY->value();
	if (ui.tb1cbStructType->currentIndex() == 0)
	{
		structElement = cv::MORPH_RECT;
	}
	else if (ui.tb1cbStructType->currentIndex() == 1)
	{
		structElement = cv::MORPH_CROSS;
	}
	else if (ui.tb1cbStructType->currentIndex() == 2)
	{
		structElement = cv::MORPH_ELLIPSE;
	}
	
	//create structure element
	cv::Mat element = cv::getStructuringElement(structElement,
		cv::Size(2 * structElemSizeX + 1, 2 * structElemSizeY + 1),
		cv::Point(structElemSizeX, structElemSizeY));


	if (ui.tb1cbOperation->currentIndex() == 0)
	{
		cv::erode(thr1, firstMorph1, element);
	}
	else if (ui.tb1cbOperation->currentIndex() == 1)
	{
		cv::dilate(thr1, firstMorph1, element);
	}
	else if (ui.tb1cbOperation->currentIndex() == 2)
	{
		cv::morphologyEx(thr1, firstMorph1, cv::MORPH_OPEN, element);
	}
	else if (ui.tb1cbOperation->currentIndex() == 3)
	{
		cv::morphologyEx(thr1, firstMorph1, cv::MORPH_CLOSE, element);
	}
	ui.tb1lblimage->setPixmap(QPixmap::fromImage(QImage(firstMorph1.data, firstMorph1.cols, firstMorph1.rows, firstMorph1.step, QImage::Format_Grayscale8)));
	resImg1 = firstMorph1;
}

void Aconity::morphsecondtb1()
{
	int structElement = 0;
	int structElemSizeX = ui.tb1sbStructSizeX_2->value();
	int structElemSizeY = ui.tb1sbStructSizeY_2->value();
	if (ui.tb1cbStructType_2->currentIndex() == 0)
	{
		structElement = cv::MORPH_RECT;
	}
	else if (ui.tb1cbStructType_2->currentIndex() == 1)
	{
		structElement = cv::MORPH_CROSS;
	}
	else if (ui.tb1cbStructType_2->currentIndex() == 2)
	{
		structElement = cv::MORPH_ELLIPSE;
	}

	//create structure element
	cv::Mat element = cv::getStructuringElement(structElement,
		cv::Size(2 * structElemSizeX + 1, 2 * structElemSizeY + 1),
		cv::Point(structElemSizeX, structElemSizeY));


	if (ui.tb1cbOperation_2->currentIndex() == 0)
	{
		cv::erode(firstMorph1, secondMorph1, element);
	}
	else if (ui.tb1cbOperation_2->currentIndex() == 1)
	{
		cv::dilate(firstMorph1, secondMorph1, element);
	}
	else if (ui.tb1cbOperation_2->currentIndex() == 2)
	{
		cv::morphologyEx(firstMorph1, secondMorph1, cv::MORPH_OPEN, element);
	}
	else if (ui.tb1cbOperation_2->currentIndex() == 3)
	{
		cv::morphologyEx(firstMorph1, secondMorph1, cv::MORPH_CLOSE, element);
	}

	ui.tb1lblimage->setPixmap(QPixmap::fromImage(QImage(secondMorph1.data, secondMorph1.cols, secondMorph1.rows, secondMorph1.step, QImage::Format_Grayscale8)));
	resImg1 = secondMorph1;
}

void Aconity::mainBitwise()
{
	cv::bitwise_or(resImg1, resImg2 , bitWise);
	ui.mainlblimage->setPixmap(QPixmap::fromImage(QImage(bitWise.data, bitWise.cols, bitWise.rows, bitWise.step, QImage::Format_Grayscale8)));
}

void Aconity::loadtb2()
{
	std::string path = ui.tb2leLoad->text().toStdString();
	src2 = cv::imread(path, cv::IMREAD_GRAYSCALE);
	src2.convertTo(src2, CV_8U);
	ui.tb2lblimage->setPixmap(QPixmap::fromImage(QImage(src2.data, src2.cols, src2.rows, src2.step, QImage::Format_Grayscale8)));
}

void Aconity::threshtb2()
{
	//convert to binary
	int threshFlag = 0;
	if (ui.tb2cbInverted->isChecked())
	{
		threshFlag = cv::THRESH_BINARY_INV;
	}
	else
	{
		threshFlag = cv::THRESH_BINARY;
	}
	thr2 = cv::Mat(src2.rows, src2.cols, src2.type());
	if (ui.tb2cbOTSU->isChecked())
	{
		cv::threshold(src2, thr2, 0, 255, threshFlag | cv::THRESH_OTSU);
	}
	else
	{
		cv::threshold(src2, thr2, ui.tb2sbThreshold->value(), 255, threshFlag);
	}
	ui.tb2lblimage->setPixmap(QPixmap::fromImage(QImage(thr2.data, thr2.cols, thr2.rows, thr2.step, QImage::Format_Grayscale8)));
}

void Aconity::morphfirsttb2()
{
	int structElement = 0;
	int structElemSizeX = ui.tb2sbStructSizeX->value();
	int structElemSizeY = ui.tb2sbStructSizeY->value();
	if (ui.tb2cbStructType->currentIndex() == 0)
	{
		structElement = cv::MORPH_RECT;
	}
	else if (ui.tb2cbStructType->currentIndex() == 1)
	{
		structElement = cv::MORPH_CROSS;
	}
	else if (ui.tb2cbStructType->currentIndex() == 2)
	{
		structElement = cv::MORPH_ELLIPSE;
	}

	//create structure element
	cv::Mat element = cv::getStructuringElement(structElement,
		cv::Size(2 * structElemSizeX + 1, 2 * structElemSizeY + 1),
		cv::Point(structElemSizeX, structElemSizeY));


	if (ui.tb2cbOperation->currentIndex() == 0)
	{
		cv::erode(thr2, firstMorph2, element);
	}
	else if (ui.tb2cbOperation->currentIndex() == 1)
	{
		cv::dilate(thr2, firstMorph2, element);
	}
	else if (ui.tb2cbOperation->currentIndex() == 2)
	{
		cv::morphologyEx(thr2, firstMorph2, cv::MORPH_OPEN, element);
	}
	else if (ui.tb2cbOperation->currentIndex() == 3)
	{
		cv::morphologyEx(thr2, firstMorph2, cv::MORPH_CLOSE, element);
	}
	resImg2 = firstMorph2;
	ui.tb2lblimage->setPixmap(QPixmap::fromImage(QImage(firstMorph2.data, firstMorph2.cols, firstMorph2.rows, firstMorph2.step, QImage::Format_Grayscale8)));
}

void Aconity::morphsecondtb2()
{
	int structElement = 0;
	int structElemSizeX = ui.tb2sbStructSizeX_2->value();
	int structElemSizeY = ui.tb2sbStructSizeY_2->value();
	if (ui.tb2cbStructType_2->currentIndex() == 0)
	{
		structElement = cv::MORPH_RECT;
	}
	else if (ui.tb2cbStructType_2->currentIndex() == 1)
	{
		structElement = cv::MORPH_CROSS;
	}
	else if (ui.tb2cbStructType_2->currentIndex() == 2)
	{
		structElement = cv::MORPH_ELLIPSE;
	}

	//create structure element
	cv::Mat element = cv::getStructuringElement(structElement,
		cv::Size(2 * structElemSizeX + 1, 2 * structElemSizeY + 1),
		cv::Point(structElemSizeX, structElemSizeY));


	if (ui.tb2cbOperation_2->currentIndex() == 0)
	{
		cv::erode(firstMorph2, secondMorph2, element);
	}
	else if (ui.tb2cbOperation_2->currentIndex() == 1)
	{
		cv::dilate(firstMorph2, secondMorph2, element);
	}
	else if (ui.tb2cbOperation_2->currentIndex() == 2)
	{
		cv::morphologyEx(firstMorph2, secondMorph2, cv::MORPH_OPEN, element);
	}
	else if (ui.tb2cbOperation_2->currentIndex() == 3)
	{
		cv::morphologyEx(firstMorph2, secondMorph2, cv::MORPH_CLOSE, element);
	}
	resImg2 = secondMorph2;
	ui.tb2lblimage->setPixmap(QPixmap::fromImage(QImage(secondMorph2.data, secondMorph2.cols, secondMorph2.rows, secondMorph2.step, QImage::Format_Grayscale8)));
}

void Aconity::morphfirstmain()
{
	int structElement = 0;
	int structElemSizeX = ui.mainsbStructSizeX->value();
	int structElemSizeY = ui.mainsbStructSizeY->value();
	if (ui.maincbStructType->currentIndex() == 0)
	{
		structElement = cv::MORPH_RECT;
	}
	else if (ui.maincbStructType->currentIndex() == 1)
	{
		structElement = cv::MORPH_CROSS;
	}
	else if (ui.maincbStructType->currentIndex() == 2)
	{
		structElement = cv::MORPH_ELLIPSE;
	}

	//create structure element
	cv::Mat element = cv::getStructuringElement(structElement,
		cv::Size(2 * structElemSizeX + 1, 2 * structElemSizeY + 1),
		cv::Point(structElemSizeX, structElemSizeY));


	if (ui.maincbOperation->currentIndex() == 0)
	{
		cv::erode(bitWise, firstMorph3, element);
	}
	else if (ui.maincbOperation->currentIndex() == 1)
	{
		cv::dilate(bitWise, firstMorph3, element);
	}
	else if (ui.maincbOperation->currentIndex() == 2)
	{
		cv::morphologyEx(bitWise, firstMorph3, cv::MORPH_OPEN, element);
	}
	else if (ui.maincbOperation->currentIndex() == 3)
	{
		cv::morphologyEx(bitWise, firstMorph3, cv::MORPH_CLOSE, element);
	}
	resImg3 = firstMorph3;
	ui.mainlblimage->setPixmap(QPixmap::fromImage(QImage(firstMorph3.data, firstMorph3.cols, firstMorph3.rows, firstMorph3.step, QImage::Format_Grayscale8)));
}

void Aconity::morphsecondmain()
{
	int structElement = 0;
	int structElemSizeX = ui.mainsbStructSizeX_2->value();
	int structElemSizeY = ui.mainsbStructSizeY_2->value();
	if (ui.maincbStructType_2->currentIndex() == 0)
	{
		structElement = cv::MORPH_RECT;
	}
	else if (ui.maincbStructType_2->currentIndex() == 1)
	{
		structElement = cv::MORPH_CROSS;
	}
	else if (ui.maincbStructType_2->currentIndex() == 2)
	{
		structElement = cv::MORPH_ELLIPSE;
	}

	//create structure element
	cv::Mat element = cv::getStructuringElement(structElement,
		cv::Size(2 * structElemSizeX + 1, 2 * structElemSizeY + 1),
		cv::Point(structElemSizeX, structElemSizeY));


	if (ui.maincbOperation_2->currentIndex() == 0)
	{
		cv::erode(firstMorph3, secondMorph3, element);
	}
	else if (ui.maincbOperation_2->currentIndex() == 1)
	{
		cv::dilate(firstMorph3, secondMorph3, element);
	}
	else if (ui.maincbOperation_2->currentIndex() == 2)
	{
		cv::morphologyEx(firstMorph3, secondMorph3, cv::MORPH_OPEN, element);
	}
	else if (ui.maincbOperation_2->currentIndex() == 3)
	{
		cv::morphologyEx(firstMorph3, secondMorph3, cv::MORPH_CLOSE, element);
	}
	resImg3 = secondMorph3;
	ui.mainlblimage->setPixmap(QPixmap::fromImage(QImage(secondMorph3.data, secondMorph3.cols, secondMorph3.rows, secondMorph3.step, QImage::Format_Grayscale8)));
}

void Aconity::mainMask()
{
	cv::bitwise_and(src1, resImg3, maskImg);
	ui.mainlblimage->setPixmap(QPixmap::fromImage(QImage(maskImg.data, maskImg.cols, maskImg.rows, maskImg.step, QImage::Format_Grayscale8)));
}

void Aconity::findSquares()
{
	std::vector < std::vector<cv::Point> > contours;
	cv::findContours(resImg3, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
	std::vector<cv::Point> approx;
	std::vector<std::vector<cv::Point> > squares;
	for (size_t i = 0; i < contours.size(); i++)
	{
		cv::approxPolyDP(contours[i], approx, cv::arcLength(contours[i], true)*0.002, true);
		if (approx.size() == 4 &&
			//fabs(cv::contourArea(approx)) > 1000 &&
			cv::isContourConvex(approx))
		{
			double maxCosine = 0;
			for (size_t j = 2; j < 5; j++)
			{
				double cosine = fabs(angle(approx[j % 4], approx[j - 2], approx[j - 1]));
				maxCosine = MAX(maxCosine, cosine);
			}
			// if cosines of all angles are small
			// (all angles are ~90 degree) then write quandrange
			// vertices to resultant sequence
			if (maxCosine < 0.3)
				squares.push_back(approx);
		}
	}
	cv::Mat outImg;
	cv::cvtColor(src1, outImg, CV_GRAY2BGR);
	outImg.convertTo(outImg, CV_16U);
	for (size_t i = 0; i < squares.size(); i++)
	{
		const cv::Point* p = &squares[i][0];
		int n = (int)squares[i].size();
		polylines(outImg, &p, &n, 1, true, cv::Scalar(0, 0, 0), 3, cv::LINE_AA);
	}
	cv::Mat showImg;
	ui.mainlblimage->setPixmap(QPixmap::fromImage(QImage(outImg.data, outImg.cols, outImg.rows, outImg.step, QImage::Format_RGB16)));
}
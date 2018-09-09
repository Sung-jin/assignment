#include "opencv2/opencv.hpp"  
#include <iostream> 
#include <ctype.h>
#include <math.h>

using namespace cv;
using namespace std;

void reSizeImage(Mat *VGA, Mat *XGA, Mat *HD, Mat *origin);
//reSize the original Image Function
int getNumber();
//receive number Function
void selectMode(int select, Mat VGA, Mat XGA, Mat HD);
//select size Function
void selectBlur(Mat *VGA, Mat *XGA, Mat *HD);
//select Blur Function
void makePadding(Mat origin, Mat *tempR, Mat *tempG, Mat *tempB, Mat *Red, Mat *Green, Mat * Blue, int paddingNum);
//makePadding and input pixel

int main() {

	int select, paddingNum, Max, key;
	//select space

	Mat originalImage;

	while(1){
		cout << "1. lenna Image 2. My Image" << endl;

		select = getNumber();
		if (select == 3) {
			cout << "잘못된 입력!" << endl;
		}
		else if (select == 4) {
			cout << "잘못된 입력!" << endl;
		}
		else
			break;
	}

	if (select == 1) {
		originalImage = imread("lenna.jpg");
	}
	else {
		originalImage = imread("OSJ.jpg");
	}

	if (!originalImage.data) {
		cout << "이미지를 불러올 수 없습니다." << endl;

		return 0;
	}

	Mat lennaVGA, lennaXGA, lennaHD;
	//Image Size

	reSizeImage(&lennaVGA, &lennaXGA, &lennaHD, &originalImage);

	//Image.at<uchar>(x, y);

	vector<Mat> channelsTemp, channels;
	split(originalImage, channels); //Partition image into three channel planes
	split(originalImage, channelsTemp);

	Mat tempL3R(Size(originalImage.rows + 2, originalImage.cols + 2), CV_8UC1);
	Mat tempL3G(Size(originalImage.rows + 2, originalImage.cols + 2), CV_8UC1);
	Mat tempL3B(Size(originalImage.rows + 2, originalImage.cols + 2), CV_8UC1);
	Mat tempL5R(Size(originalImage.rows + 4, originalImage.cols + 4), CV_8UC1);
	Mat tempL5G(Size(originalImage.rows + 4, originalImage.cols + 4), CV_8UC1);
	Mat tempL5B(Size(originalImage.rows + 4, originalImage.cols + 4), CV_8UC1);
	Mat tempL7R(Size(originalImage.rows + 6, originalImage.cols + 6), CV_8UC1);
	Mat tempL7G(Size(originalImage.rows + 6, originalImage.cols + 6), CV_8UC1);
	Mat tempL7B(Size(originalImage.rows + 6, originalImage.cols + 6), CV_8UC1);
	Mat tempLMerge(Size(originalImage.rows, originalImage.cols), CV_32FC3);
	
	int differTempR[4], differTempG[4], differTempB[4];

	while (1) {

		cout << "1. Difference Operator 2. Edge Detector 3.quit" << endl;

		select = getNumber();

		if (select == 3) {
			cout << "Good Bye" << endl;
			break;
		}
		else if (select == 4)
			cout << "잘못된 입력 !" << endl;

		switch (select){
		case 1:
			cout << "1. 3X3 2. 5X5 3. 7X7 4. quit" << endl;
			select = getNumber();
			if (select == 4)
				break;
			else if (select == 1) {
				paddingNum = 1;

				makePadding(originalImage, &tempL3R, &tempL3G, &tempL3B, &channelsTemp[2], &channelsTemp[1], &channelsTemp[0], paddingNum);

				for (int i = paddingNum; i < originalImage.cols; i++) {
					for (int j = paddingNum; j < originalImage.rows; j++) {
						differTempR[0] = abs(tempL3R.at<uchar>(j - paddingNum, i - paddingNum) - tempL3R.at<uchar>(j + paddingNum, i + paddingNum));
						differTempR[1] = abs(tempL3R.at<uchar>(j, i - paddingNum) - tempL3R.at<uchar>(j, i + paddingNum));
						differTempR[2] = abs(tempL3R.at<uchar>(j + paddingNum, i - paddingNum) - tempL3R.at<uchar>(j - paddingNum, i + paddingNum));
						differTempR[3] = abs(tempL3R.at<uchar>(j + paddingNum, i) - tempL3R.at<uchar>(j - paddingNum, i));

						Max = differTempR[0];

						for (int k = 1; k < 4; k++) {
							if (Max < differTempR[k])
								Max = differTempR[k];
						}

						channels[2].at<uchar>(j, i) = Max;

						differTempG[0] = abs(tempL3G.at<uchar>(j - paddingNum, i - paddingNum) - tempL3G.at<uchar>(j + paddingNum, i + paddingNum));
						differTempG[1] = abs(tempL3G.at<uchar>(j, i - paddingNum) - tempL3G.at<uchar>(j, i + paddingNum));
						differTempG[2] = abs(tempL3G.at<uchar>(j + paddingNum, i - paddingNum) - tempL3G.at<uchar>(j - paddingNum, i + paddingNum));
						differTempG[3] = abs(tempL3G.at<uchar>(j + paddingNum, i) - tempL3G.at<uchar>(j - paddingNum, i));

						Max = differTempG[0];

						for (int k = 1; k < 4; k++) {
							if (Max < differTempG[k])
								Max = differTempG[k];
						}

						channels[1].at<uchar>(j, i) = Max;

						differTempB[0] = abs(tempL3B.at<uchar>(j - paddingNum, i - paddingNum) - tempL3B.at<uchar>(j + paddingNum, i + paddingNum));
						differTempB[1] = abs(tempL3B.at<uchar>(j, i - paddingNum) - tempL3B.at<uchar>(j, i + paddingNum));
						differTempB[2] = abs(tempL3B.at<uchar>(j + paddingNum, i - paddingNum) - tempL3B.at<uchar>(j - paddingNum, i + paddingNum));
						differTempB[3] = abs(tempL3B.at<uchar>(j + paddingNum, i) - tempL3B.at<uchar>(j - paddingNum, i));

						Max = differTempB[0];

						for (int k = 1; k < 4; k++) {
							if (Max < differTempB[k])
								Max = differTempB[k];
						}

						channels[0].at<uchar>(j, i) = Max;


					}
				}

				merge(channels, tempLMerge);

				imshow("7X7DO", tempLMerge);

				key = waitKey(0);
				if (key == 27) {
					destroyAllWindows();
					break;
				}

			}
			else if(select == 2) {
				paddingNum = 2;

				makePadding(originalImage, &tempL5R, &tempL5G, &tempL5B, &channelsTemp[2], &channelsTemp[1], &channelsTemp[0], paddingNum);

				for (int i = paddingNum; i < originalImage.cols; i++) {
					for (int j = paddingNum; j < originalImage.rows; j++) {
						differTempR[0] = abs(tempL5R.at<uchar>(j - paddingNum, i - paddingNum) - tempL5R.at<uchar>(j + paddingNum, i + paddingNum));
						differTempR[1] = abs(tempL5R.at<uchar>(j, i - paddingNum) - tempL5R.at<uchar>(j, i + paddingNum));
						differTempR[2] = abs(tempL5R.at<uchar>(j + paddingNum, i - paddingNum) - tempL5R.at<uchar>(j - paddingNum, i + paddingNum));
						differTempR[3] = abs(tempL5R.at<uchar>(j + paddingNum, i) - tempL5R.at<uchar>(j - paddingNum, i));

						Max = differTempR[0];

						for (int k = 1; k < 4; k++) {
							if (Max < differTempR[k])
								Max = differTempR[k];
						}

						channels[2].at<uchar>(j, i) = Max;

						differTempG[0] = abs(tempL5G.at<uchar>(j - paddingNum, i - paddingNum) - tempL5G.at<uchar>(j + paddingNum, i + paddingNum));
						differTempG[1] = abs(tempL5G.at<uchar>(j, i - paddingNum) - tempL5G.at<uchar>(j, i + paddingNum));
						differTempG[2] = abs(tempL5G.at<uchar>(j + paddingNum, i - paddingNum) - tempL5G.at<uchar>(j - paddingNum, i + paddingNum));
						differTempG[3] = abs(tempL5G.at<uchar>(j + paddingNum, i) - tempL5G.at<uchar>(j - paddingNum, i));

						Max = differTempG[0];

						for (int k = 1; k < 4; k++) {
							if (Max < differTempG[k])
								Max = differTempG[k];
						}

						channels[1].at<uchar>(j, i) = Max;

						differTempB[0] = abs(tempL5B.at<uchar>(j - paddingNum, i - paddingNum) - tempL5B.at<uchar>(j + paddingNum, i + paddingNum));
						differTempB[1] = abs(tempL5B.at<uchar>(j, i - paddingNum) - tempL5B.at<uchar>(j, i + paddingNum));
						differTempB[2] = abs(tempL5B.at<uchar>(j + paddingNum, i - paddingNum) - tempL5B.at<uchar>(j - paddingNum, i + paddingNum));
						differTempB[3] = abs(tempL5B.at<uchar>(j + paddingNum, i) - tempL5B.at<uchar>(j - paddingNum, i));

						Max = differTempB[0];

						for (int k = 1; k < 4; k++) {
							if (Max < differTempB[k])
								Max = differTempB[k];
						}

						channels[0].at<uchar>(j, i) = Max;


					}
				}

				merge(channels, tempLMerge);

				imshow("5X5DO", tempLMerge);

				key = waitKey(0);
				if (key == 27) {
					destroyAllWindows();
					break;
				}

			}
			else{
				paddingNum = 3;

				makePadding(originalImage, &tempL7R, &tempL7G, &tempL7B, &channelsTemp[2], &channelsTemp[1], &channelsTemp[0], paddingNum);

				for (int i = paddingNum; i < originalImage.cols; i++) {
					for (int j = paddingNum; j < originalImage.rows; j++) {
						differTempR[0] = abs(tempL7R.at<uchar>(j - paddingNum, i - paddingNum) - tempL7R.at<uchar>(j + paddingNum, i + paddingNum));
						differTempR[1] = abs(tempL7R.at<uchar>(j, i - paddingNum) - tempL7R.at<uchar>(j, i + paddingNum));
						differTempR[2] = abs(tempL7R.at<uchar>(j + paddingNum, i - paddingNum) - tempL7R.at<uchar>(j - paddingNum, i + paddingNum));
						differTempR[3] = abs(tempL7R.at<uchar>(j + paddingNum, i) - tempL7R.at<uchar>(j - paddingNum, i));

						Max = differTempR[0];

						for (int k = 1; k < 4; k++) {
							if (Max < differTempR[k])
								Max = differTempR[k];
						}

						channels[2].at<uchar>(j, i) = Max;

						differTempG[0] = abs(tempL7G.at<uchar>(j - paddingNum, i - paddingNum) - tempL7G.at<uchar>(j + paddingNum, i + paddingNum));
						differTempG[1] = abs(tempL7G.at<uchar>(j, i - paddingNum) - tempL7G.at<uchar>(j, i + paddingNum));
						differTempG[2] = abs(tempL7G.at<uchar>(j + paddingNum, i - paddingNum) - tempL7G.at<uchar>(j - paddingNum, i + paddingNum));
						differTempG[3] = abs(tempL7G.at<uchar>(j + paddingNum, i) - tempL7G.at<uchar>(j - paddingNum, i));

						Max = differTempG[0];

						for (int k = 1; k < 4; k++) {
							if (Max < differTempG[k])
								Max = differTempG[k];
						}

						channels[1].at<uchar>(j, i) = Max;

						differTempB[0] = abs(tempL7B.at<uchar>(j - paddingNum, i - paddingNum) - tempL7B.at<uchar>(j + paddingNum, i + paddingNum));
						differTempB[1] = abs(tempL7B.at<uchar>(j, i - paddingNum) - tempL7B.at<uchar>(j, i + paddingNum));
						differTempB[2] = abs(tempL7B.at<uchar>(j + paddingNum, i - paddingNum) - tempL7B.at<uchar>(j - paddingNum, i + paddingNum));
						differTempB[3] = abs(tempL7B.at<uchar>(j + paddingNum, i) - tempL7B.at<uchar>(j - paddingNum, i));

						Max = differTempB[0];

						for (int k = 1; k < 4; k++) {
							if (Max < differTempB[k])
								Max = differTempB[k];
						}

						channels[0].at<uchar>(j, i) = Max;


					}
				}

				merge(channels, tempLMerge);

				imshow("7X7DO", tempLMerge);

				key = waitKey(0);
				if (key == 27) {
					destroyAllWindows();
					break;
				}

			}
			break;

		case 2:
			cout << "1. VGA 2.XGA 3.HD 4.quit" << endl;

			select = getNumber();

			if (select == 4) {
				cout << "Good Bye" << endl;
				break;
			}

			selectMode(select, lennaVGA, lennaXGA, lennaHD);

			break;

		default:
			break;
		}	

	}

	return 0;
}

void reSizeImage(Mat *VGA, Mat *XGA, Mat *HD, Mat *origin) {
	//resize image by VGA, XGA, HD
	resize(*origin, *VGA, Size(640, 480), 0, 0, CV_INTER_LINEAR);
	cvtColor(*VGA, *VGA, COLOR_BGR2GRAY);
	resize(*origin, *XGA, Size(1024, 768), 0, 0, CV_INTER_LINEAR);
	cvtColor(*XGA, *XGA, COLOR_BGR2GRAY);
	resize(*origin, *HD, Size(1920, 1080), 0, 0, CV_INTER_LINEAR);
	cvtColor(*HD, *HD, COLOR_BGR2GRAY);
}

int getNumber() {
	int Num;

	cout << "insert : ";
	cin >> Num;

	if (cin.fail() || Num > 4 || Num < 0) {
		cout << "잘못된 입력 !" << endl;
		cin.clear();						//Initialize input error
		cin.ignore(256, '\n');				//clear input buffer
		return getNumber();					//recursive call
	}

	return Num;
}

void selectMode(int select, Mat VGA, Mat XGA, Mat HD) {
	
	int key, Max, Min;
	//Min, Max threshold value storage space
	//key = keyboard input value, 27 = 'ESC'

	bool initCheck = true;
	//Checking initialization

	Mat dstImage;
	//Edge Storage space
	

	selectBlur(&VGA, &XGA, &HD);
	//select blur as function


	switch (select) {
	case 1://VGA mode
		while (1) {

			namedWindow("VGA", WINDOW_AUTOSIZE);

			createTrackbar("Min Value", "VGA", &Min, 255);
			if (initCheck) {
				setTrackbarPos("Min Value", "VGA", 50);
			}

			createTrackbar("Max Value", "VGA", &Max, 255);
			if (initCheck) {
				setTrackbarPos("Max Value", "VGA", 150);
				initCheck = false;
			}

			Min = getTrackbarPos("Min Value", "VGA");
			Max = getTrackbarPos("Max Value", "VGA");

			Canny(VGA, dstImage, Min, Max, 5);

			imshow("VGA", dstImage);

			key = waitKey(0);
			if (key == 27) {
				destroyAllWindows();
				break;
			}
		}
		break;

	case 2://XGA mode
		while (1) {

			namedWindow("XGA", WINDOW_AUTOSIZE);

			createTrackbar("Min Value", "XGA", &Min, 255);
			if (initCheck) {
				setTrackbarPos("Min Value", "XGA", 50);
			}

			createTrackbar("Max Value", "XGA", &Max, 255);
			if (initCheck) {
				setTrackbarPos("Max Value", "XGA", 150);
				initCheck = false;
			}

			Min = getTrackbarPos("Min Value", "XGA");
			Max = getTrackbarPos("Max Value", "XGA");

			Canny(XGA, dstImage, Min, Max, 5);

			imshow("XGA", dstImage);

			key = waitKey(0);
			if (key == 27) {
				destroyAllWindows();
				break;
			}
		}
		break;

	case 3://HD mode
		while (1) {

			namedWindow("HD", WINDOW_AUTOSIZE);

			createTrackbar("Min Value", "HD", &Min, 255);
			if (initCheck) {
				setTrackbarPos("Min Value", "HD", 50);
			}

			createTrackbar("Max Value", "HD", &Max, 255);
			if (initCheck) {
				setTrackbarPos("Max Value", "HD", 150);
				initCheck = false;
			}

			Min = getTrackbarPos("Min Value", "HD");
			Max = getTrackbarPos("Max Value", "HD");

			Canny(HD, dstImage, Min, Max, 5);

			imshow("HD", dstImage);

			key = waitKey(0);
			if (key == 27) {
				destroyAllWindows();
				break;
			}
		}
		break;

	default:
		cout << "잘못된 입력!" << endl;
		break;
	}
}

void selectBlur(Mat *VGA, Mat *XGA, Mat *HD) {

	int blurSel;
	//blur select number

	cout << "Blurring" << endl;
	cout << "1. 3X3\t2. 5X5\t3.7X7\t4.original Image" << endl;;

	blurSel = getNumber();

	switch (blurSel) {
	case 1://apply 3X3 gaussian mask blur
		GaussianBlur(*VGA, *VGA, Size(3, 3), 0);
		GaussianBlur(*XGA, *XGA, Size(3, 3), 0);
		GaussianBlur(*HD, *HD, Size(3, 3), 0);
		break;

	case 2://apply 5X5 gaussian mask blur
		GaussianBlur(*VGA, *VGA, Size(5, 5), 0);
		GaussianBlur(*XGA, *XGA, Size(5, 5), 0);
		GaussianBlur(*HD, *HD, Size(5, 5), 0);
		break;

	case 3://apply 7X7 gaussian mask blur
		GaussianBlur(*VGA, *VGA, Size(7, 7), 0);
		GaussianBlur(*XGA, *XGA, Size(7, 7), 0);
		GaussianBlur(*HD, *HD, Size(7, 7), 0);
		break;

	default:
		break;
	}
}

void makePadding(Mat origin, Mat *tempR, Mat *tempG, Mat *tempB, Mat *Red, Mat *Green, Mat * Blue, int paddingNum) {

	for (int i = 0; i < paddingNum; i++) {//LeftTop
		for (int j = 0; j < paddingNum; j++) {
			tempR->at<uchar>(j,i) = Red->at<uchar>(0, 0);
			tempG->at<uchar>(j, i) = Green->at<uchar>(0, 0);
			tempB->at<uchar>(j, i) = Blue->at<uchar>(0, 0);
		}
	}

	for (int i = 0; i < paddingNum; i++) {//RightTop
		for (int j = 0; j < paddingNum; j++) {
			tempR->at<uchar>(origin.rows + j, i) = Red->at<uchar>(origin.rows - 1, 0);
			tempG->at<uchar>(origin.rows + j, i) = Green->at<uchar>(origin.rows - 1, 0);
			tempB->at<uchar>(origin.rows + j, i) = Blue->at<uchar>(origin.rows - 1, 0);
		}
	}

	for (int i = 0; i < paddingNum; i++) {//LeftBottom
		for (int j = 0; j < paddingNum; j++) {
			tempR->at<uchar>(j, origin.cols + paddingNum + i) = Red->at<uchar>(0, origin.cols - 1);
			tempG->at<uchar>(j, origin.cols + paddingNum + i) = Green->at<uchar>(0, origin.cols - 1);
			tempB->at<uchar>(j, origin.cols + paddingNum + i) = Blue->at<uchar>(0, origin.cols - 1);
		}
	}

	for (int i = 0; i < paddingNum; i++) {//RightBottom
		for (int j = 0; j < paddingNum; j++) {
			tempR->at<uchar>(origin.rows + paddingNum + j, origin.cols + paddingNum + i) = Red->at<uchar>(origin.rows - 1, origin.cols - 1);
			tempG->at<uchar>(origin.rows + paddingNum + j, origin.cols + paddingNum + i) = Green->at<uchar>(origin.rows - 1, origin.cols - 1);
			tempB->at<uchar>(origin.rows + paddingNum + j, origin.cols + paddingNum + i) = Blue->at<uchar>(origin.rows - 1, origin.cols - 1);
		}
	}

	for (int i = 0; i < paddingNum; i++) {
		for (int j = paddingNum; j < origin.rows + paddingNum - 1; j++) {//TopBottom
			tempR->at<uchar>(j, i) = Red->at<uchar>(j - paddingNum, 0);
			tempG->at<uchar>(j, i) = Green->at<uchar>(j - paddingNum, 0);
			tempB->at<uchar>(j, i) = Blue->at<uchar>(j - paddingNum, 0);

			tempR->at<uchar>(j, origin.cols + paddingNum + i) = Red->at<uchar>(j - paddingNum, origin.cols - 1);
			tempG->at<uchar>(j, origin.cols + paddingNum + i) = Green->at<uchar>(j - paddingNum, origin.cols - 1);
			tempB->at<uchar>(j, origin.cols + paddingNum + i) = Blue->at<uchar>(j - paddingNum, origin.cols - 1);
		}
	}

	for (int i = paddingNum; i < origin.cols + paddingNum - 1; i++) {//LeftRight
		for (int j = 0; j < paddingNum; j++) {
			tempR->at<uchar>(j, i) = Red->at<uchar>(0, i - paddingNum);
			tempG->at<uchar>(j, i) = Green->at<uchar>(0, i - paddingNum);
			tempB->at<uchar>(j, i) = Blue->at<uchar>(0, i - paddingNum);

			tempR->at<uchar>(origin.rows + paddingNum + j, i) = Red->at<uchar>(origin.rows - 1, i - paddingNum);
			tempG->at<uchar>(origin.rows + paddingNum + j, i) = Green->at<uchar>(origin.rows - 1, i - paddingNum);
			tempB->at<uchar>(origin.rows + paddingNum + j, i) = Blue->at<uchar>(origin.rows - 1, i - paddingNum);
		}
	}

	//Input pixel

	for (int i = 0; i < paddingNum; i++) {
		for (int j = 0; j < paddingNum; j++) {
			tempR->at<uchar>(j, i) = Red->at<uchar>(0, 0);
			tempG->at<uchar>(j, i) = Green->at<uchar>(0, 0);
			tempB->at<uchar>(j, i) = Blue->at<uchar>(0, 0);
		}
	}


	for (int i = 0; i < origin.cols; i++) {
		for (int j = 0; j < origin.rows; j++) {
			tempR->at<uchar>(j + 2, i + 2) = Red->at<uchar>(j, i);
			tempG->at<uchar>(j + 2, i + 2) = Green->at<uchar>(j, i);
			tempB->at<uchar>(j + 2, i + 2) = Blue->at<uchar>(j, i);
		}
	}

}
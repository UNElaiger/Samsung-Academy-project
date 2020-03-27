#include <iostream>
#include <wiringPi.h>
#include "libSonar.h"
#include <opencv2/core/core.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/ml/ml.hpp>
#include <ostream>
#include <stdio.h>
#include <opencv2/objdetect/objdetect.hpp>
#include <algorithm>
#include <iostream>
#include <math.h>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <functional>
#include <tesseract/baseapi.h>
#include <tesseract/strngs.h>
#include <leptonica/allheaders.h>
#include <string>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <fstream>
#include <sstream>
 
using namespace cv;
using namespace std;
 int sonar()
{
	    int trigger = 4;
    int echo = 5;
    Sonar sonar;
    sonar.init(trigger, echo);
    while(1){
        cout << "Distance is " << sonar.distance(30000) << " cm." << endl; // 30000 задержка в милисекундах
        if (sonar.distance(30000) < 10)
        break;
    }
    return 0;
}


 void timer_start(std::function<bool(void)> func, unsigned int interval)
{
    std::thread([func, interval]() {
        while (true)
        {
            if(func())
		return;
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
        }
    }).detach();
}
void obraboka()
{
const int GREEN_MIN = 21;
const int GREEN_MAX = 110;

Mat src = imread("output_img.jpg"); //Исходное изображение
	
	imshow("input", src);
	imwrite("src.jpg", src);
	//Переводим в формат HSV
	Mat hsv = Mat(src.cols, src.rows, 8, 3);
	vector<Mat> splitedHsv = vector<Mat>(); 
	cvtColor(src, hsv, CV_BGR2HSV);
	split(hsv, splitedHsv);

	//Удаление фона

	for (int y = 0; y < hsv.cols; y++) {
		for (int x = 0; x < hsv.rows; x++) {

			// polychaem HSV-komponenti pixlelja
			int H = static_cast<int>(splitedHsv[0].at<uchar>(x, y));        // Тон
			int S = static_cast<int>(splitedHsv[1].at<uchar>(x, y));        // Интенсивность
			int V = static_cast<int>(splitedHsv[2].at<uchar>(x, y));        // Яркость
			
			//Если яркость слишком низкая то закрашиваем в белый цвет
			if (V > 100){
				src.at<Vec3b>(x, y)[0] = 255;
				src.at<Vec3b>(x, y)[1] = 255;
				src.at<Vec3b>(x, y)[2] = 255;
			}
		}
	}
	Mat tmp;
	

	int an = 5;
	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(an * 2 + 1, an * 2 + 1), Point(an, an));
	dilate(src, tmp, element);
	erode(tmp, tmp, element);


	Mat grayscaleMat;
	cvtColor(tmp, grayscaleMat, CV_BGR2GRAY);


	Mat mask(grayscaleMat.size(), grayscaleMat.type());
	Mat out(src.size(), src.type());
	threshold(grayscaleMat, mask, 200, 255, THRESH_BINARY_INV);
	

	out = Scalar::all(255);

	src.copyTo(out, mask);

	imshow("Do zamikanija", src);
	imwrite("img777.jpg", src);
	imshow("rezultat zamikanija", tmp);
	imshow("rezultat", out);

}
int func_with_cam()
{
    int trigger = 4;
    int echo = 5;
    //sonar----------------------------------
    if (wiringPiSetup() == -1)
    return -1;
    
    Sonar sonar;
    sonar.init(trigger, echo);
	//sonar-----------------------------------
    CascadeClassifier numdber_cascade;	// подключаем каскад хаара
	numdber_cascade.load("/home/pi/openCV/haar/haarcascade_russian_plate_number.xml");
	bool number_cascade_load = numdber_cascade.load("/home/pi/openCV/haar/haarcascade_russian_plate_number.xml");
	if (!number_cascade_load)	// проверяем подключен каскад или нет
	{
		cout << "cascade not found" << endl;
		waitKey(1);
		return -1;
	}
	VideoCapture cap(0); // инициализация камеры
	if (!cap.isOpened()) //проверяет доступность камеры
	{
		cout << "camera is not opened" << endl;
		return -1;
	}
	int old_val = 0;
	int new_val = 0;
	int count = 0;
	bool flag = false;
	Mat src;
	Mat img;
	Point pt1;
	Point pt2;
	Mat sat;
	std::vector<Rect> rectangl;
	bool exit = false;
		timer_start([&](){
		numdber_cascade.detectMultiScale(src, rectangl);
		for (size_t i = 0; i < rectangl.size(); i++) // отрисовка квадрата вокруг автомобильного номера
		{
			pt1=Point(rectangl[i].x, rectangl[i].y);
			pt2=Point(rectangl[i].x + rectangl[i].width, rectangl[i].y + rectangl[i].height);
			rectangle(src, pt2, pt1, Scalar(255, 255, 255), 2, 8, 0);
			Rect r(rectangl[i].x, rectangl[i].y,rectangl[i].width,rectangl[i].height);
			src(r).copyTo(img); // вырезаем квадрат с номером из общего видеопотока 
			imshow("out", img); // отображение вырезанного номера
			if(flag) {
				imwrite("output_img.jpg", img);
				//cout << "================save" << endl;
				exit = true;
				return true;
			}
		}
		return false;
	} , 10);
	timer_start([&](){
		new_val=sonar.distance(400000);
		cout<<"sonar dist="<<new_val<<endl;
		if(new_val>=30 && new_val<=31)
		{
			count++;
			//cout << "count ="<< count << endl;
		}
		else {
			count=0;
			flag = false;
		}
			    
		if(count>=10)
		{
			flag = true;
		}
		if(exit)
			return true;
		else
			return false;
	}, 1);

	while (exit==false)
	{	
		
		cap >> src;
		imshow("Detected number plate", src);
		waitKey(1);
	}
		
		imshow("Detected number plate", src);
		usleep(100);
		Mat out=imread("output_img.jpg");
		out.convertTo(sat,1,1.9);
		imwrite("img10.jpg",sat);
		cout<<"open cv ended"<<endl;
		Mat out_img=imread("img10.jpg");
		imshow("img10.jpg", out_img);
	return 0;
}
int tesseractt(int argc, char* argv[]) {
    
	setlocale(LC_ALL,"Russian");
    tesseract::TessBaseAPI tessApi;
    tessApi.Init(argv[1], "rus");            // тут data каталог в котором лежат файлы *.traineddata,
    // а rus указывает какой именно из них использовать
    FILE* f; // объявляем файловую переменную
    if(argc >= 1) {

        PIX *pix = pixRead("/home/pi/tesseract_project/img10.jpg");// считываем картинку из файла с именем,
            // переданным первым аргументом, это функционал Leptonica
        tessApi.SetImage(pix);// говорим tesseract, что распознавать нужно эту картинку
        char *text = tessApi.GetUTF8Text();//распознаём

        //---генерируем имя файла в который будет записан распознанный текст
        char *fileName = NULL;
        long prefixLength;
        string STR = string(text);
        string s = "car number =";
        cout << text<<endl;
        cout << STR[0]<<endl;
        cout << STR[1]<<endl;
        f=fopen("output.txt","w");
        fprintf(f,tessApi.GetUTF8Text());
    }
    return 0;
}

int main(int argc, char* argv[])
{
    setlocale(LC_ALL,"Russian");
    tesseract::TessBaseAPI tessApi;
    tessApi.Init(argv[1], "rus");            // тут data каталог в котором лежат файлы *.traineddata,
    // а rus указывает какой именно из них использовать
    FILE* f; // объявляем файловую переменную
	func_with_cam();
	obraboka();
	cout<<"tesseract_started"<<endl;
    if(argc >= 1) {

        PIX *pix = pixRead("/home/pi/releas/build/img777.jpg");// считываем картинку из файла с именем,
            // переданным первым аргументом, это функционал Leptonica
        tessApi.SetImage(pix);// говорим tesseract, что распознавать нужно эту картинку
        char *text = tessApi.GetUTF8Text();//распознаём

        //---генерируем имя файла в который будет записан распознанный текст
        char *fileName = NULL;
        long prefixLength;
        string STR = string(text);
        
        f=fopen("output.txt","w");
        fprintf(f,tessApi.GetUTF8Text());
	fclose(f);
	string str;
	stringstream buffer;
	ifstream f;
	f.open("output.txt");
	getline(f, str);
	cout<<"cars number: "<<str<<endl;
	buffer << str;
	f.close();
	std::string filename = "/home/pi/releas/build/http.py";
	std::string command = "python ";
	command += filename + " " + buffer.str();
	system(command.c_str());
    }
    return 0;
}


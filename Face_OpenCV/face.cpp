/*
交互式对话框：请选择要执行的动作
	1.人脸采集：
		打开摄像头，采集照片，保存训练模型
	2.人脸识别：
		<1>打开摄像头，采集照片，预识别；
		<2>输入图像的路径，图像识别
		输出结果：
			找到匹配的对象，输出名字；
			未找到匹配的对象，提示：人脸采集并保存训练模型
*/
 
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <direct.h>
#include <io.h>
 
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/face.hpp>
 
using namespace std;
using namespace cv;
using namespace cv::face;
 
int  face_image_save(string dir_name, string cascade_path);
int face_model_train_save(string cvs_path);
int face_recg(Mat image, string cascade_path);
//void read_cvs(const string& filename, vector<Mat>& img, vector<int>& lable, char separator = ';');
//void write_cvs(string& filename, string line, int label_number, char separator = ';');
 
string cascade_xml_path = "F:\\project\\vs2017\\openCV_test\\openCV_test\\cascade\\haarcascades\\haarcascade_frontalface_default.xml";
string face_image_path = "F:\\project\\vs2017\\openCV_test\\openCV_test\\face_recg\\face\\";
string cvs_path = "F:\\project\\vs2017\\openCV_test\\openCV_test\\face_recg\\face\\at.txt";
 
int label_number = 100;
 
#define  MY_SIZE	Size(92, 112)
 
int main()
{
	while (true)
	{
		printf("\n<------------------ 欢迎使用人脸识别系统，请输入你的选择 ------------------>\n1 -- 人脸图像采集\n2 -- 人脸识别\n3 -- 退出系统\n");
		int action = 0;
		cin >> action;
 
		if (action == 1)
		{
			printf("请输入你的名字：");
 
			string name;
			cin >> name;
			string dir_name = face_image_path + name + "\\";
			int ret = face_image_save(dir_name, cascade_xml_path);
			if (ret == 0)
			{
				face_model_train_save(cvs_path);
			}
		}
		else if (action == 2)
		{
			printf("\n\n请选择图片来源：\n1 -- 从指定目录选择图片\n2 -- 打开摄像头读取图片\n");
			cin >> action;
 
			Mat test_img;
 
			switch (action)
			{
			case 1:
				{
					printf("请输入图片的完整路径\n");
 
					string img_path;
 
					while (true)
					{
						cin >> img_path;
 
						if (_access(img_path.c_str(), 0) == -1)
						{
							printf("没有找到图片，请重新选择图片！！！\n");
						}
						else
						{
							test_img = imread(img_path.c_str());
							break;
						}
					}
				}
				break;
 
			case 2: 
			default:
				{
					printf("请对准摄像头，我们将读取照片...\n");
 
					//打开摄像头
					VideoCapture capture;
					capture.open(0);
					if (!capture.isOpened())
					{
						printf("Can't open camera...\n");
						return -1;
					}
				
					capture.read(test_img);
					capture.release();
				}
 
				break;
			}
 
			//人脸识别
			face_recg(test_img, cascade_xml_path);
		}
		else
		{
			break;
		}
	}
 
	return 0;
}
 
//使用CVS文件读取图片和标签
void read_cvs(const string& filename, vector<Mat>& img, vector<int>& lable, char separator = ';')
{
	Mat tmp_img;
	ifstream cvs_file(filename.c_str());
	assert(cvs_file);
	string line, path, tag;
	while (getline(cvs_file, line))
	{
		stringstream lines(line);
		getline(lines, path, separator);
		getline(lines, tag);
		if (!path.empty() && !tag.empty())
		{
			tmp_img = imread(path, IMREAD_GRAYSCALE);   //读入图像时转成灰度图
			assert(!tmp_img.empty());
			img.push_back(tmp_img);
			lable.push_back(atoi(tag.c_str()));
		}
	}
	tmp_img.release();
}
 
void write_cvs(string& filename, string line, int label_number, char separator = ';')
{
	ofstream cvs_file;
	string str = line + separator + format("%d\n", label_number);
 
	//printf("%s\n", str.c_str());
	cvs_file.open(filename.c_str(), ios::app);
	if (cvs_file.is_open())
	{
		cvs_file << str;
		cvs_file.close();
	}
}
 
int  face_image_save(string dir_name, string cascade_path)
{
	//printf("我们将保存你的图片到以下路径：%s\n", dir_name.c_str());
	if (_access(dir_name.c_str(), 0) == -1)
	{
		//printf("不存在文件夹，我们将创建...\n");
		_mkdir(dir_name.c_str());
	}
	else
	{
		printf("人脸图片已经保存在数据库，不需要重复读取\n");
		return -1;
	}
 
 
	VideoCapture capture;
 
	//打开笔记本摄像头
	capture.open(0);
	if (!capture.isOpened())
	{
		printf("can't open camera...\n");
		return -1;
	}
 
	//加载分类器
	CascadeClassifier cascade;
	cascade.load(cascade_path);
	if (cascade.empty())
	{
		printf("can't load classifier\n");
		return -1;
	}
 
	Mat img, img_gray, faceROI;
	vector<Rect> faces;
	int count = 0;
	string my_path, tmp_path;
 
	while (true)
	{
		capture.read(img);
		cvtColor(img, img_gray, CV_RGB2GRAY);	//转化为灰度图
		equalizeHist(img_gray, img_gray);		//直方图均衡化
		cascade.detectMultiScale(img_gray, faces, 1.1, 6, 0, Size(30, 30));	//人脸识别，获取faces矢量数据集
 
		if (faces.size() > 0)
		{
			for (int i = 0; i < faces.size(); ++i)
			{				
				faceROI = img_gray(faces[i]);//ROI部分为将要保存的图片，用img_gray获取灰度图部分，保存成灰度图；如果将img_gray改为img，就变成彩色图片了
				resize(faceROI, faceROI, MY_SIZE);	//调整ROI图片大小到指定大小
				rectangle(img, faces[i], Scalar(0, 255, 0), 2, 8, 0);
				imshow("view", img);	//显示视频图像
				imshow("ROI", faceROI);	//显示ROI区域
 
				tmp_path = format("%d.jpg", count++);
				my_path = dir_name + tmp_path;
				imwrite(my_path, faceROI);		//保存采集到的图片到指定目录
 
				write_cvs(cvs_path, my_path, label_number);
			}
		}
 
		if (waitKey(30) == 27 || count == 20)
		{
			break;
		}
	}
	//保存的标签值
	label_number++;
 
	capture.release();
	destroyAllWindows();
 
	return 0;
}
 
int face_model_train_save(string cvs_path)
{
	printf("创建人脸识别的模型并进行训练，保存训练结果中（需要一些时间）...\n");
 
	vector<Mat> faces;
	vector<int> labels;
 
	try
	{
		read_cvs(cvs_path, faces, labels);
	}
	catch (Exception &e)
	{
		cerr << "Error opening file, reason :" << e.msg << endl;
		return -1;
	}
 
	if (faces.size() <= 1)
	{
		printf("Too few face images\n");
		return -1;
	}
 
	/* 创建人脸识别的模型，并进行训练，之后保存训练结果*/
	double start1, start2, start3;
	double end1, end2, end3;
 
		//PCA特征脸算法
	start1 = (double)getTickCount();
	Ptr<EigenFaceRecognizer> eigen_model = EigenFaceRecognizer::create();
	eigen_model->train(faces, labels);
	eigen_model->save("my_eigen_face_model.xml");
	end1 = (getTickCount() - start1) / getTickFrequency();
 
	//LDA线性判别分析
	start2 = (double)getTickCount();
	Ptr<FisherFaceRecognizer> fisher_model = FisherFaceRecognizer::create();
	fisher_model->train(faces, labels);
	fisher_model->save("my_fisher_face_model.xml");
	end2 = (getTickCount() - start2) / getTickFrequency();
 
	//LBP局部二值模式直方图
	start3 = (double)getTickCount();
	Ptr<LBPHFaceRecognizer> lbph_model = LBPHFaceRecognizer::create();
	lbph_model->train(faces, labels);
	lbph_model->save("my_lbph_face_model.xml");
	end3 = (getTickCount() - start3) / getTickFrequency();
 
	printf("PAC算法训练所用时间：	%f s\n", end1);
	printf("LDA算法训练所用时间：	%f s\n", end2);
	printf("LBP算法训练所用时间：	%f s\n", end3);
 
 
	printf("训练结束，保存成功！\n");
 
	return 0;
}
 
int face_recg(Mat image, string cascade_path)
{
	printf("人脸识别中...\n");
 
	//显示读入的图片
	imshow("origin", image);
	waitKey(30);
 
	//加载分类器
	CascadeClassifier cascade;
	cascade.load(cascade_path);
	if (cascade.empty())
	{
		printf("can't load classifier\n");
		return -1;
	}
 
	//读取人脸识别模型
	double start1, start2, start3;
	double end1, end2, end3;
 
	start1 = (double)getTickCount();
	Ptr<EigenFaceRecognizer> eigen_model = Algorithm::load<EigenFaceRecognizer>("my_eigen_face_model.xml"); //Eigen Faces算法
	end1 = (getTickCount() - start1) / getTickFrequency();
 
	start2 = (double)getTickCount();
	Ptr<FisherFaceRecognizer> fisher_model = Algorithm::load<FisherFaceRecognizer>("my_fisher_face_model.xml");//Fisher Faces算法
	end2 = (getTickCount() - start2) / getTickFrequency();
 
	start3 = (double)getTickCount();
	Ptr<LBPHFaceRecognizer> lbph_model = Algorithm::load<LBPHFaceRecognizer>("my_lbph_face_model.xml");		//LBP 算法
	end3 = (getTickCount() - start3) / getTickFrequency();
 
	printf("加载PAC算法训练模型所用时间：	%f s\n", end1);
	printf("加载LDA算法训练模型所用时间：	%f s\n", end2);
	printf("加载LBP算法训练模型所用时间：	%f s\n", end3);
 
	Mat faceROI, img_gray;
	int p_label;
	vector<Rect> faces;
	cvtColor(image, img_gray, CV_RGB2GRAY);	//转化为灰度图
	equalizeHist(img_gray, img_gray);		//直方图均衡化
	cascade.detectMultiScale(img_gray, faces, 1.1, 6, 0, Size(30, 30));	//人脸识别，获取faces矢量数据集
	
	//printf("size = %zd\n", faces.size());
	if (faces.size() > 0)
	{
		for (int i = 0; i < faces.size(); ++i)
		{
			faceROI = img_gray(faces[i]);
			resize(faceROI, faceROI, MY_SIZE);	//调整成训练集一样的大小
			rectangle(image, faces[i], Scalar(0, 255, 0), 2, 8, 0);
			imshow("Origin", image);
 
			p_label = eigen_model->predict(faceROI);
			cout << "Eigen算法识别到的结果：" << p_label << endl;
 
			p_label = fisher_model->predict(faceROI);
			cout << "Fisher算法识别到的结果：" << p_label << endl;
 
			p_label = lbph_model->predict(faceROI);
			cout << "LBPH算法识别到的结果：" << p_label << endl;
			
			imshow("result", faceROI);
		}
	}
 
	destroyAllWindows();
 
	return 0;
}
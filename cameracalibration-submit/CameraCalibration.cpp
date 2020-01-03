#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/imgproc/types_c.h>  
#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;

int main() 
{
	ifstream fin("back.txt"); /* 标定所用图像文件的路径 */
	ofstream fout("result_back.txt");  /* 保存标定结果的文件 */	
	//读取每一幅图像，从中提取出角点，然后对角点进行亚像素精确化	
	cout<<"开始提取角点………………\n";
	int image_count=0;  /* 图像数量 */
	Size image_size;  /* 图像的尺寸 */
	Size board_size = Size(11,7);
	vector<Point2f> image_points_buf;  /* 缓存每幅图像上检测到的角点坐标 */
	vector<vector<Point2f>> image_points_seq; /* 保存检测到的所有角点 */
	string filename;
	string image_gray;
	stringstream Str;
	int a=0;
	int count= 0 ;//用于存储角点个数。
	while (getline(fin,filename))
	{
		image_count++;		
		cout<<"image_count = "<<image_count<<endl;		
		/* 输出检验*/
		Mat imageInput=imread(filename);
		if (image_count == 1)  //读入第一张图片时获取图像宽高信息
		{
			image_size.width = imageInput.cols;
			image_size.height =imageInput.rows;
			cout<<"image_size.width = "<<image_size.width<<endl;
			cout<<"image_size.height = "<<image_size.height<<endl;
		}
		/* 提取角点 */
		if (0 == findChessboardCorners(imageInput,board_size,image_points_buf))   //存储找到的角点坐标
		{			
			cout<<"can not find chessboard corners!\n"; //找不到角点
			exit(1);
		} 
		else 
		{
			Mat view_gray;
			cvtColor(imageInput,view_gray,CV_RGB2GRAY);//将输入图片转换为灰度图
			/* 亚像素精确化 */
			find4QuadCornerSubpix(view_gray,image_points_buf,Size(5,5)); //对粗提取的角点进行精确化
			count += image_points_buf.size();
			image_points_seq.push_back(image_points_buf);  //保存亚像素角点
			/* 在图像上显示角点位置 */
			drawChessboardCorners(view_gray,board_size,image_points_buf,true); //用于在图片中标记角点并显示
			imshow("Camera Calibration",view_gray);//显示图片
			string outpath_gray="./out_gray/";
                
                 	Str.clear();//清除缓存
                 	Str << a + 1;
                 	Str >> image_gray;
                 	outpath_gray="./out_gray/";
                 	outpath_gray += image_gray;
                 	outpath_gray += "_gray.jpg";
		 	a++;
			imwrite(outpath_gray,view_gray);//存储角点灰度图
			waitKey(500);//暂停0.5S		
		}
		cout << "count = " << count<< endl;//显示角点累加后的角点数
	}
	int total = image_points_seq.size();//图片总数
	cout<<"total = "<<total<<endl;
	int CornerNum=board_size.width*board_size.height;  //每张图片上总的角点数
	for (int ii=0 ; ii<total ;ii++)
	{
		if (0 == ii%CornerNum)//在第一幅图时输出以下log
		{	
			cout<<endl;
			cout<<"每幅图片的第一个角点位置坐标:"<<endl;
			fout<<"每幅图片的第一个角点位置坐标:"<<endl;
		}
		if (0 == ii%3)	
		{
			cout<<endl;//每3个参数一行输出到控制台
			fout<<endl;
		}
		else
		{
			cout.width(10);//输出宽度为10
			fout.width(10);
		}
		//输出每一幅图的第一个角点的xy像素坐标
		cout<<" -->"<<image_points_seq[ii][0].x;
		fout<<" -->"<<image_points_seq[ii][0].x;
		cout<<" -->"<<image_points_seq[ii][0].y;
		fout<<" -->"<<image_points_seq[ii][0].y;
	}
	cout<<endl<<endl;
	fout<<endl<<endl;
	cout<<"角点提取完成!"<<endl;

	//以下是摄像机标定
	cout<<"开始标定………………"<<endl;
	/*棋盘三维信息*/
	Size square_size = Size(19,19);  /* 实际测量得到的标定板上每个棋盘格的大小,单位mm*/
	vector<vector<Point3f>> object_points; /* 保存标定板上角点的三维坐标 */
	/*内外参数*/
	Mat cameraMatrix=Mat(3,3,CV_32FC1,Scalar::all(0)); /* 摄像机内参数矩阵,创建一个3行3列,1通道32位浮点型的矩阵,赋值全部为0 */
	vector<int> point_counts;  // 每幅图像中角点的数量
	Mat distCoeffs=Mat(1,5,CV_32FC1,Scalar::all(0)); /* 摄像机的5个畸变系数：k1,k2,p1,p2,k3 */
	vector<Mat> tvecsMat;  /* 每幅图像的平移向量 */
	vector<Mat> rvecsMat; /* 每幅图像的旋转向量 */
	/* 初始化标定板上角点的三维坐标 */
	int i,j,t;
	for (t=0;t<image_count;t++) 
	{
		vector<Point3f> tempPointSet;
		for (i=0;i<board_size.height;i++) 
		{
			for (j=0;j<board_size.width;j++) 
			{
				Point3f realPoint;
				/* 假设标定板放在世界坐标系中z=0的平面上 */
				realPoint.x = i*square_size.width;
				realPoint.y = j*square_size.height;
				realPoint.z = 0;
				tempPointSet.push_back(realPoint);
			}
		}
		
		object_points.push_back(tempPointSet);
	}
	/* 初始化每幅图像中的角点数量，假定每幅图像中都可以看到完整的标定板 */
	for (i=0;i<image_count;i++)
	{
		point_counts.push_back(board_size.width*board_size.height);
	}	
	/* 开始标定 */
	calibrateCamera(object_points,image_points_seq,image_size,cameraMatrix,distCoeffs,rvecsMat,tvecsMat,0);
	cout<<"标定完成！\n";
	//对标定结果进行评价
	cout<<"开始评价标定结果………………\n";
	double total_err = 0.0; /* 所有图像的平均误差的总和 */
	double err = 0.0; /* 每幅图像的平均误差 */
	vector<Point2f> image_points2; /* 保存重新计算得到的投影点 */
	cout<<"每幅图像的标定误差:"<<endl;
	fout<<"每幅图像的标定误差:"<<endl;
	for (i=0;i<image_count;i++)
	{
		vector<Point3f> tempPointSet=object_points[i];
		/* 通过得到的摄像机内外参数，对空间的三维点进行重新投影计算，得到新的投影点 */
		projectPoints(tempPointSet,rvecsMat[i],tvecsMat[i],cameraMatrix,distCoeffs,image_points2);//image_points2为得到的新投影点
		if(i%image_count==0)
		{
			cout<<"new_image_points2.size="<<image_points2.size()<<endl;
		}
		/* 计算新的投影点和旧的投影点之间的误差*/
		vector<Point2f> tempImagePoint = image_points_seq[i];
		Mat tempImagePointMat = Mat(1,tempImagePoint.size(),CV_32FC2);//1行角点数 列2通道的32位浮点数矩阵,角点坐标(x,y),所以是两通道。
		Mat image_points2Mat = Mat(1,image_points2.size(), CV_32FC2);//1行角点数 列2通道的32位浮点数矩阵
		for (int j = 0 ; j < tempImagePoint.size(); j++)
		{
			image_points2Mat.at<Vec2f>(0,j) = Vec2f(image_points2[j].x, image_points2[j].y);
			tempImagePointMat.at<Vec2f>(0,j) = Vec2f(tempImagePoint[j].x, tempImagePoint[j].y);
		}
		err = norm(image_points2Mat, tempImagePointMat, NORM_L2);//求原坐标与投影坐标的L2范数，即欧氏距离
		err /= point_counts[i];//每个角点的误差，相当于是每幅图每个角点的平均误差
		total_err += err;    
		cout<<"第"<<i+1<<"幅图像的平均误差："<<err<<"像素"<<endl;   
		fout<<"第"<<i+1<<"幅图像的平均误差："<<err<<"像素"<<endl;   
	}   
	cout<<"总体平均误差："<<total_err/image_count<<"像素"<<endl;   
	fout<<"总体平均误差："<<total_err/image_count<<"像素"<<endl<<endl;   
	cout<<"评价完成！"<<endl;  
	//保存定标结果  	
	std::cout<<"开始保存定标结果………………"<<endl;       
	Mat rotation_matrix = Mat(3,3,CV_32FC1, Scalar::all(0)); /* 保存每幅图像的旋转矩阵 */
	fout<<"相机内参数矩阵："<<endl;   
	fout<<cameraMatrix<<endl<<endl;   
	fout<<"畸变系数"<<endl;   
	fout<<distCoeffs<<endl<<endl<<endl;   
	for (int i=0; i<image_count; i++) 
	{ 
		fout<<"第"<<i+1<<"幅图像的旋转向量："<<endl;   
		fout<<tvecsMat[i]<<endl;   
		/* 将旋转向量转换为相对应的旋转矩阵 */   
		Rodrigues(tvecsMat[i],rotation_matrix); //输入为旋转向量，输出旋转为矩阵  
		fout<<"第"<<i+1<<"幅图像的旋转矩阵："<<endl;   
		fout<<rotation_matrix<<endl;   
		fout<<"第"<<i+1<<"幅图像的平移向量："<<endl;   
		fout<<rvecsMat[i]<<endl<<endl;   
	}   
	cout<<"完成保存"<<endl; 
	fout<<endl;

	Mat mapx = Mat(image_size, CV_32FC1);//输出的X坐标重映射参数
    	Mat mapy = Mat(image_size, CV_32FC1);//输出的Y坐标重映射参数
    	Mat R = Mat::eye(3, 3, CV_32F);
   	cout << "保存矫正图像" << endl;
    	string imageFileName;
    	stringstream StrStm;
    	for (int i = 0; i < image_count; i++)
    	{
        	std::cout << "Frame #" << i + 1 << "..." << endl;
        	initUndistortRectifyMap(cameraMatrix, distCoeffs, R, cameraMatrix, image_size, CV_32FC1, mapx, mapy);//用来计算畸变映射
        	StrStm.clear();//清除缓存
        	imageFileName.clear();
		string filePath = "./back/";
        	StrStm << i + 1;
        	StrStm >> imageFileName;
        	filePath += imageFileName;
        	filePath += ".jpg";//获取图片路径
        	Mat imageSource = imread(filePath);
        	Mat newimage = imageSource.clone();
        	remap(imageSource, newimage, mapx, mapy, INTER_LINEAR);
		/*第5个参数为插值方式，有四中插值方式：INTER_NEAREST——最近邻插值，INTER_LINEAR——双线性插值，
		INTER_CUBIC——双三样条插值，INTER_LANCZOS4——lanczos插值*/
        	imshow("原始图像", imageSource);
        	imshow("矫正后图像", newimage);
        	waitKey(500);
        	StrStm.clear();
        	filePath.clear();
		string outpath="./out_back/";
		if(i==image_count-1)
			cout<<"outpath="<<outpath<<endl;
        	StrStm << i + 1;
        	StrStm >> imageFileName;
		outpath += imageFileName;
		outpath += "_d.jpg";
		imwrite(outpath,newimage);
    	}
    	std::cout << "保存结束" << endl;
	
	return 0;
}

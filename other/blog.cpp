//#pragma comment(lib,"opencv_highgui231d.lib")
#include"opencv2/core/core.hpp"
#include"opencv2/imgproc/imgproc.hpp"
#include"opencv2/calib3d/calib3d.hpp"
#include"opencv2/highgui/highgui.hpp"
#include <opencv2/imgproc/types_c.h>
#include <opencv2/highgui/highgui_c.h>
#include<iostream>
#include<fstream>

using namespace std;
using namespace cv;


int main()
{
    ifstream fin("train.txt");//�궨����ͼ���ļ���·��
    ofstream fout("calibration_result.txt");//����궨���

    //��ȡÿһ��ͼ�񣬴�����ȡ���ڽǵ㣬Ȼ��Խǵ���������ؾ�ȷ��
    cout<<"��ʼ��ȡ������..........";
    int image_count = 0;//ͼ�������
    Size image_size;//ͼ��ĳߴ�
    Size board_size = Size(13,9);//�궨����ÿ�С��е��ڽǵ�����һ��أ���������Ҫ��ͬ
    vector<Point2f> image_points_buf;//���ڴ洢��⵽���ڽǵ�ͼ������λ��
    vector<vector<Point2f>> image_points_seq;//�����⵽�����нǵ�
    string filename;//ͼ����
    int count = 0;//����ǵ���
   // while(getline(fin,filename))
   // {
        //filename="left02.jpg";
     //   cout<<"filename="<<filename<<endl;
       // Mat src=imread(filename);
       // cout<<"src="<<src<<endl;
   // }
    while(getline(fin,filename))//���ı��ĵ������ζ�ȡ���궨ͼƬ��
    {
        image_count++;
        cout<<"image_count = "<<image_count<<endl;
//	filename = "/home/dengjie/dengjie/project/CameraCalibration/Zhang-s-method-1/" + filename;
	cout<<"filename is:"<<filename<<endl;

        Mat imageInput;
	imageInput = imread(filename);//���ζ�ȡ��ǰĿ¼��ͼƬ
	//if(imageInput.empty())  
        //{  
        //        fprintf(stderr, "Can not load image %s\n", filename); 
		//imshow("image", imageInput);
		//waitKey(); 
                //return -1; 
	//	continue; 
        //}  
//	Mat imageInput = cvLoadImage(filename,0);
	cout<<"2222filename is:"<<filename<<endl;
//	cout<<"3333imageInput is:"<<imageInput<<endl;	
//        if(image_count == 1)//�����һ��ͼƬʱ��ȡͼ������Ϣ
//        {
            image_size.width = imageInput.cols;
            image_size.height = imageInput.rows;

            cout<<"image_size.width = "<<image_size.width<<endl;
            cout<<"image_size.height = "<<image_size.height<<endl;
//        }
	continue;
        //��ȡ�궨����ڽǵ㣬���������8λ�ĻҶȻ��߲�ɫͼ��
        if(0 == findChessboardCorners(imageInput,board_size,image_points_buf))

        {
            cout<<"Cannot find chessboard corners!\n";
            exit(1);
        }
        else
        {
            Mat view_gray;
            cvtColor(imageInput,view_gray,CV_RGB2GRAY);//ת�Ҷ�ͼ
            //find4QuadCornerSubpix(view_gray,image_points_buf,Size(5,5));
            //�����ؾ�ȷ������һ
            //image_points_buf��ʼ���Ľǵ�����������ͬʱ��Ϊ����������λ�õ��������������
           find4QuadCornerSubpix(view_gray,image_points_buf,Size(5,5)); 
            //cornerSubPix(view_gray,image_points_buf,Size(5,5),Size(-1,-1),5);
           //�����ؾ�ȷ��������
             //Size(5,5)���������ڵĴ�С,Size(-1,-1)��ʾû������
            //���ĸ�����������ǵ�ĵ������̵���ֹ����������Ϊ���������ͽǵ㾫�����ߵ����
            count += image_points_buf.size();
            image_points_seq.push_back(image_points_buf);//���������ؽǵ�
            drawChessboardCorners(view_gray,board_size,image_points_buf,false);
            //���ڻ��Ʊ��ɹ��궨�Ľǵ㣬����8λ�ҶȻ��߲�ɫͼ��
            //���ĸ������Ǳ�־λ������ָʾ����������ڽǵ��Ƿ�������̽�⵽
            //false��ʾ��δ��̽�⵽���ڽǵ㣬��ʱ��������ԲȦ��ǳ���⵽���ڽǵ�
            imshow("Camera Calibration",view_gray);//��ʾͼƬ
            waitKey(500);//��ͣ0.5S
        }
        cout << "count = " << count<< endl;//��ʾ�ǵ��ۼӺ��ֵ
    }

    int total = image_points_seq.size();//ͼƬ����
    cout << "total = " << total << endl;
    int CornerNum = board_size.width*board_size.height;//ÿ��ͼƬ���ܵ��ڽǵ���

    for(int ii = 0;ii<total;ii++)
    {
        cout << "��" << ii + 1 << "��ͼƬ�����ݣ�" << endl;
        for(int jj = 0;jj<CornerNum;jj++)
        {
            if (0 == jj % 3)
                cout << endl;//ÿ�����ǵ�����֮����
            else
                cout.width(10);//�����ʽ����

            cout << "��" << image_points_seq[ii][jj].x << "��" << image_points_seq[ii][jj].y << ")";
        }
        cout << endl;
    }

    cout<<"�ǵ���ȡ��ɣ�\n";


    cout<<"��ʼ�궨............";

    Size square_size = Size(10,10);//�������̸��ӵ�ʵ�ʱ߳�����λΪmm
    vector<vector<Point3f>> object_points;//����궨���Ͻǵ����ά����
    Mat cameraMatrix = Mat(3,3,CV_32FC1,Scalar::all(0));//����ڲ�������
    vector<int> point_counts;//ÿ��ͼ���нǵ������
    Mat distCoeffs = Mat(1,5,CV_32FC1,Scalar::all(0));//�������5������ϵ����k1,k2,k3,p1,p2
    vector<Mat> tvecsMat;//ÿ��ͼ���ƽ������
    vector<Mat> rvecsMat;//ÿ��ͼ�����ת����

    //��ʼ���궨���Ͻǵ����ά����
    int i,j,t;
    for(t = 0;t<image_count;t++)
    {
        vector<Point3f> temPointSet;
        for(i = 0;i<board_size.height;i++)
        {
            for(j = 0;j<board_size.width;j++)
            {
                Point3f realPoint;

                //����궨�������������ϵ�е�z = 0ƽ����
                //��Ҫ���������ϵ����ڰ׾���Ĵ�С�����������ʼ����ÿһ���ڽǵ����������
                realPoint.x = i*square_size.width;
                realPoint.y = j*square_size.height;
                realPoint.z = 0;
                temPointSet.push_back(realPoint);

            }
        }
        object_points.push_back(temPointSet);
    }

    //��ʼ��ÿ��ͼ���еĽǵ��������ٶ�ÿ��ͼ���ж����Կ��������ı궨��
    for(i = 0;i<image_count;i++)
    {
        point_counts.push_back(board_size.width*board_size.height);
    }

    calibrateCamera(object_points,image_points_seq,image_size,
        cameraMatrix,distCoeffs,rvecsMat,tvecsMat,0);
/*  object_points ��������ϵ�нǵ����ά���꣬image_points_seq ÿ���ڽǵ��Ӧ��ͼ�������
    image_size ͼ������سߴ��С��cameraMatrix ������ڲ�������distCoeffs ���������ϵ��
    rvecsMat �������ת������tvecsMat �����λ��������0�궨ʱ���õ��㷨
    ��ʹ�øú������б궨����֮ǰ����Ҫ��������ÿһ���ڽǵ�Ŀռ�����ϵ��λ��������г�ʼ����
    �궨�Ľ��������������ڲ�������cameraMatrix�������5������ϵ��distCoeffs������ÿ��ͼ��
    �������������Լ���ƽ����������ת����
*/
    cout<<"�궨��ɣ�"<<endl;

    cout<<"��ʼ����궨���....."<<endl;
    double total_err = 0.0; //����ͼ���ƽ�������ܺͣ���ʼ��Ϊ0.0
    double err = 0.0; //ÿ��ͼ���ƽ�����
    vector<Point2f> image_points2; //�������¼���õ���ͶӰ��

    cout<<"ÿ��ͼ��ı궨��\n";
    fout<<"ÿ��ͼ��ı궨��\n";

    for(i = 0;i<image_count;i++)
    {
        vector<Point3f> tempPointSet = object_points[i];//ȡ��ÿ��ͼ��ǵ����ά�ռ�����
        projectPoints(tempPointSet,rvecsMat[i],tvecsMat[i],
            cameraMatrix,distCoeffs,image_points2);
        //ͨ���õ������������������Կռ����ά���������ͶӰ���㣬�õ��µ�ͶӰ��

        vector<Point2f> tempImagePoint = image_points_seq[i];//ԭ��ÿ��ͼ���нǵ�ͼ������
        Mat tempImagePointMat = Mat(1,tempImagePoint.size(),CV_32FC2);
        //���ڽ�ԭͼ�������洢��һ�ж��е�Mat��������ͨ�����������ݹ���
        Mat image_points2Mat = Mat(1,image_points2.size(),CV_32FC2);
        //���ڽ���ͶӰ�����洢��һ�ж��е�Mat���Ա��ڼ�����ͶӰ���

        for(int j = 0;j <tempImagePoint.size(); j++)
        {
            image_points2Mat.at<Vec2f>(0,j) = Vec2f(image_points2[j].x,image_points2[j].y);
            tempImagePointMat.at<Vec2f>(0,j) = Vec2f(tempImagePoint[j].x,tempImagePoint[j].y);
        }//��ֵ
        //Vec2f��ʾ����2ͨ��float���͵�Vector,mat.at<Vec2f>(y, x)�Ƿ���ͼ���һ�ַ�ʽ

        err = norm(image_points2Mat,tempImagePointMat,NORM_L2);
        //����ÿ��ͼƬ��ͶӰ����������ؽǵ�����֮���ƫ��
        total_err += err /= point_counts[i];//�ۼ����
        cout<<"��"<<i+1<<"��ͼ���ƽ����"<<err<<"����"<<endl;
        fout<<"��"<<i+1<<"��ͼ���ƽ����"<<err<<"����"<<endl;

    }

    cout<<"����ƽ����"<<total_err/image_count<<"����"<<endl;
    fout<<"����ƽ����"<<total_err/image_count<<"����"<<endl<<endl;
    cout<<"������ɣ�"<<endl;

    cout<<"��ʼ����궨���............"<<endl;
    Mat rotation_matrix = Mat(3,3,CV_32FC1,Scalar::all(0));//����ÿ��ͼ�����ת����
    fout<<"����ڲ�������"<<endl;
    fout<<cameraMatrix<<endl<<endl;
    fout<<"����ϵ����\n";
    fout<<distCoeffs<<endl<<endl<<endl;

    for(i = 0; i<image_count;i++)
    {
        fout<<"��"<<i+1<<"��ͼ�����ת������"<< endl;
        fout<<rvecsMat[i]<<endl;
        Rodrigues(rvecsMat[i],rotation_matrix);//����ת����ת��Ϊ��Ӧ����ת����
        fout<<"��"<<i+1<<"��ͼ�����ת����"<< endl;
        fout<<rotation_matrix<< endl << endl;
        fout<<"��"<<i+1<<"��ͼ���ƽ��������"<<  endl;
        fout<<tvecsMat[i]<< endl <<endl;

    }
    cout<<"��ɱ���"<< endl;
    fout<< endl;


    //��ʾ�궨���
    Mat mapx = Mat(image_size,CV_32FC1);//�����X������ӳ�����
    Mat mapy = Mat(image_size,CV_32FC1);//�����Y������ӳ�����
    Mat R = Mat::eye(3,3,CV_32F);
    cout<<"�������ͼ��"<<endl;

    string imageFileName;
    std::stringstream StrStm;
    for(int i = 0;i < image_count;i++)
    {
        cout<<"Frame # "<<i+1<<"....."<<endl;
        initUndistortRectifyMap(cameraMatrix, distCoeffs, R, cameraMatrix,
            image_size, CV_32FC1, mapx, mapy);//�����������ӳ��

        StrStm.clear();//�������
        imageFileName.clear();
        string filePath = "chess";
        StrStm<<i+1;
        StrStm>>imageFileName;
        filePath += imageFileName;
        filePath += ".bmp";
        //��ȡͼƬ·��
        Mat imageSource = imread(filePath);//��ȡͼ��
        Mat newimage = imageSource.clone();//����ͼ��

       remap(imageSource,newimage,mapx,mapy,INTER_LINEAR);//����õ�ӳ��Ӧ�õ�ͼ����
       //��initUndistortRectifyMap���ʹ�ã�Ϊ��������֮һ

       //undistort(imageSource,newimage,cameraMatrix,distCoeffs);//����������
       //���������newCameraMatrix=noArray()��Ĭ�ϸ�cameraMatrix����һ��,�ʿ�ʡ

        imageFileName += "_d.jpg";//������ͼƬ����
        imwrite(imageFileName,newimage);//����������ͼƬ

    }
    cout<<"�������"<<endl;

    fin.close();
    fout.close();
    getchar();//�ȴ��������˳�

    return 0;
}

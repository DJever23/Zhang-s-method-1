# Zhang-s-method
张正友相机标定法学习。

实现平台Ubuntu18.04+OpenCV4.1。

主要包含：
1.CameraCalibration.cpp--标定文件。
2.back文件夹--存放标定的图像。
3.out_back文件夹--保存矫正后的图片
4.out_gray文件夹--保存绘制角点时的灰度图
5.result_back.txt--存放用于标定的图像路径。
6.CMakeLists.txt--编译文件。
7.cal1--可执行文件。
8.cali_log.txt--存储执行cal1时的log。
9.生成result_back.txt文件用于存储校正参数。
10.project2-lkf为另一份代码和相关文件。
11.编译方法：
	sudo cmake .
	sudo make
	./cal1

     

问题点：
1.修改main的类型为int，并在程序最后return 0；
2./home/dengjie/dengjie/project/CameraCalibration/Zhang-s-method-1/CameraCalibration.cpp:58:34: error: ‘CV_RGB2GRAY’ was not declared in this scope
    cvtColor(imageInput,view_gray,CV_RGB2GRAY);
  修改：
  #include <opencv2/imgproc/types_c.h>
3.image_count = 1
  filename is:/left/left01.jpg
  2222filename is:/left/left01.jpg
  image_size.width = 0
  image_size.height = 0
  terminate called after throwing an instance of 'cv::Exception'
    what():  OpenCV(4.1.0) /home/dengjie/opencv-4.1.0/modules/core/src/matrix.cpp:757: error: (-215:Assertion failed) dims <= 2 &&           step[0] > 0 in function 'locateROI'
  修改：
  绝对路径或者相对路径才能正常读取图像信息：/home/dengjie/dengjie/project/CameraCalibration/Zhang-s-method-1/left/left01.jpg
  或者：./left/left01.jpg  
4.存放图片路径的train.txt需要在ubuntu下新建填写或者使用以下命令生成：	
  find /dir -name "*.jpg" > train.txt
5.博客：https://blog.csdn.net/DJames23/article/details/103704163

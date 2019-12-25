# Zhang-s-method
张正友相机标定法学习。

实现平台Ubuntu18.04+OpenCV4.1。

主要包含：
1.CameraCalibration.cpp--标定文件。
2.left.zip用于标定的图像。
3.calibdata.txt存放用于标定的图像路径。
4.CMakeLists.txt--编译文件。
5.编译方法：
	sudo cmake .
	sudo make
	./cal1
6.生成result.txt文件用于存储校正参数。
     

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
    what():  OpenCV(4.1.0) /home/dengjie/opencv-4.1.0/modules/core/src/matrix.cpp:757: error: (-215:Assertion failed) dims <= 2 && step[0] > 0 in function 'locateROI'
  修改：
  绝对路径或者相对路径才能正常读取图像信息：/home/dengjie/dengjie/project/CameraCalibration/Zhang-s-method-1/left/left01.jpg
  或者：./left/left01.jpg

4.博客：https://blog.csdn.net/DJames23/article/details/103704163

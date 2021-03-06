#include "camera_node.h"

int main(int argc, char** argv){

ros::init(argc,argv,"my_camera");

ros::NodeHandle n("~");

std::string device, device1;

if(!n.hasParam("device_name")){
	n.setParam("device_name","/dev/video0");
}

n.getParam("device_name", device);

ros::Publisher camera = n.advertise<sensor_msgs::Image>("partha_topic",1000);

ros::Rate loop_rate(30);

cv::VideoCapture cap;
cap.open(device);
cv::Mat cv_img=cv::Mat(480,640,CV_8UC3);
int c=0;
while(ros::ok()){

n.getParam("device_name", device1);
ROS_INFO("%s\n",device.c_str());
	if(device.compare(device1) !=0){
		cap.release();
		cap.open(device1);
		device=device1;
}
sensor_msgs::Image img;
cap>>cv_img;

std_msgs::Header h;
h.seq=c;
h.stamp=ros::Time::now();

cv_bridge::CvImage br_img=cv_bridge::CvImage(h, sensor_msgs::image_encodings::BGR8,cv_img);
br_img.toImageMsg(img);
camera.publish(img);

ros::spinOnce();
loop_rate.sleep();

c++;
}

return 0;
}

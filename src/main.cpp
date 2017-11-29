/*Program for the senior design node
1. Captures an image from a USB camera,
2. Pipes it into apriltags.
3. Then finds the transform from the camera to the tag
   and extracts the angle from this transform
4. Tells the servo to move to this angle through
   an overdamped PI compensator (for now,
   will later need to be type 2 system.)
5. Sends transform from tag to camera X,Y,theta
   over CAN in a format

*/

#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include <TagDetector.h> //system library?

#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Geometry>

#include <can_sensors/position_can_sensor.h>

#define DEFAULT_TAG_FAMILY "Tag36h11"

Eigen::Matrix4f getRelativeTransform(double tag_size,const cv::Point2f tag_p[], double fx, double fy, double px, double py);

int main()
{
	TagDetectorParams p; //leave defaults for now
/*
    p.sigma = ;
    p.segSigma = ;
    p.thetaThresh = ;
    p.magThresh = ;
    p.adaptiveThresholdValue = ;
    p.adaptiveThresholdRadius = ;
*/
    TagFamily fam(DEFAULT_TAG_FAMILY);
	TagDetector detector(fam, p);
	TagDetectionArray detections;

	Eigen::Matrix4f myT;
	Eigen::Matrix4f cameraInTagT;
	Eigen::Vector3f eulerAngles;
	float Xpos;
	float Ypos;
	float rotationAngle;

	//these need loaded from a camera calibration
    float camfx = 730;
	float camfy = 730;
    float campx = 320;
	float campy = 240;

	float tagsize = .0505;

	//can stuff
    PositionCanSensor posSensor(20, (char*)"can0");



    //camera stuff
	cv::VideoCapture cap(0);
	if(!cap.isOpened()) return -1;

	cv::Mat img(480, 640, CV_8UC3, cv::Scalar(69,42,200));
	cv::Mat img2(480, 640, CV_8UC3, cv::Scalar(69,42,200));

    cv::Point2d opticalCenter(240, 320);

    std::cout <<"Warming up camera (2 seconds)" <<std::endl;

    cv::waitKey(2000); //let camera warm up

    std::cout <<"Camera hot!" <<std::endl;

    while(1)
    {
        cap >> img;
        cv::cvtColor(img, img2, cv::COLOR_BGR2GRAY);

        detector.process(img2, opticalCenter, detections);

        for(size_t index=0;index<detections.size(); ++index)
		{
			const TagDetection& currDetection = detections[index];
			//get transform from camera to tag (tag in camera coord)
			myT = getRelativeTransform(tagsize, currDetection.p, camfx, camfy, campx, campy);
			//need to get inverse (camera in tag coords)
			cameraInTagT = myT.inverse();
            Eigen::Matrix3f wRo = cameraInTagT.topLeftCorner(3,3);
			//Eigen::AngleAxisf angryAxe(wRo);
			//axisOfRotation = angryAxe.axis();
			//rotationAngle = angryAxe.angle();
			eulerAngles = wRo.eulerAngles(1,2,0);
			std::cout <<"Found tag with ID: " <<currDetection.id <<std::endl;
			std::cout << "X: " <<cameraInTagT(0, 3)<<std::endl;
			std::cout << "Y: " <<cameraInTagT(1, 3)<<std::endl;
            std::cout << "Z: " <<cameraInTagT(2, 3)<<std::endl;
            std::cout << "rotAngles: " <<std::endl
                                     <<eulerAngles(0) << std::endl
                                     <<eulerAngles(1) << std::endl
                                     <<eulerAngles(2) << std::endl;
            //std::cout << "angle: " <<rotationAngle<<std::endl;

			//actual X = Z measurement
            //actual Y = X measurement
			//actual theta = first rotAngle
            rotationAngle = eulerAngles(0) > M_PI/2 ? eulerAngles(0) - M_PI : eulerAngles(0);
			Xpos = cameraInTagT(2,3);
			Ypos = cameraInTagT(0,3);
			std::cout <<"Actual Data: " <<std::endl
					  <<"X    : " << Xpos <<std::endl
				      <<"Y    : " << Ypos <<std::endl
					  <<"Theta: " << rotationAngle<<std::endl;

            if (posSensor.sendData(Xpos, Ypos, rotationAngle, 0) == PositionCanSensor::SendStatus::WRITE_FAILED)
            {
                std::cout <<"CAN WRITE ERRRORORORORO!!!!"<<std::endl;
            }
            else
            {
                std::cout <<"can write success" <<std::endl;
            }


		}
	}
}

Eigen::Matrix4f getRelativeTransform(double tag_size,const cv::Point2f tag_p[], double fx, double fy, double px, double py)
{
   std::vector<cv::Point3f> objPts;
   std::vector<cv::Point2f> imgPts;
   double s = tag_size/2.;
   objPts.push_back(cv::Point3f(-s,-s, 0));
   objPts.push_back(cv::Point3f( s,-s, 0));
   objPts.push_back(cv::Point3f( s, s, 0));
   objPts.push_back(cv::Point3f(-s, s, 0));

   imgPts.push_back(tag_p[0]);
   imgPts.push_back(tag_p[1]);
   imgPts.push_back(tag_p[2]);
   imgPts.push_back(tag_p[3]);

   /*imgPts.push_back(cv::Point2f(tag_p[0][0], tag_p[0][1]));
   imgPts.push_back(cv::Point2f(tag_p[1][0], tag_p[1][1]));
   imgPts.push_back(cv::Point2f(tag_p[2][0], tag_p[2][1]));
   imgPts.push_back(cv::Point2f(tag_p[3][0], tag_p[3][1]));*/

   cv::Mat rvec, tvec;
   cv::Matx33f cameraMatrix(
                            fx, 0, px,
                            0, fy, py,
                            0,  0,  1);
   cv::Vec4f distParam(0,0,0,0); // all 0?
   cv::solvePnP(objPts, imgPts, cameraMatrix, distParam, rvec, tvec);
   cv::Matx33d r;
   cv::Rodrigues(rvec, r);
   Eigen::Matrix3f wRo;
   wRo << r(0,0), r(0,1), r(0,2), r(1,0), r(1,1), r(1,2), r(2,0), r(2,1), r(2,2);

   Eigen::Matrix4f T;
   T.topLeftCorner(3,3) = wRo;
   T.col(3).head(3) << tvec.at<double>(0), tvec.at<double>(1), tvec.at<double>(2);
   T.row(3) << 0,0,0,1;

   return T;
}

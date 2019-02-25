#include "filesave_module.h"
#include <pcl/io/pcd_io.h>
#include <ctime>
#include <sstream>
#include <sys/stat.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cv.h>

#include <iostream>





FilesaveModule::FilesaveModule(DataType dataType, const char* filename, TimestampType timestampType)
{
	//copy values to class
	_dataType = dataType;
	_timestampType = timestampType;
	_filename = new char[strlen(filename)+1];
	strcpy(_filename, filename);

	//save dirname and create dir
	if(timestampType != TimestampType::None){
		Dirname = new std::string(std::string("pointdata_") + GetSystemTimestamp() + std::string("/"));
		mkdir(Dirname->c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	}

	//open camera stream
	ipcam.open("http://root:root@192.168.1.246/axis-cgi/mjpg/video.cgi?resolution=1280x720&req_fps=10&.mjpg");
	
    	std::time(&start);


}

FilesaveModule::~FilesaveModule()
{
	//clean up
	if(_filename != nullptr){
		delete[] _filename;
		_filename = nullptr;
	}
	if(Dirname != nullptr){
		delete Dirname;
		Dirname = nullptr;
	}
}

void FilesaveModule::slot(const quanergy::PointCloudXYZIRConstPtr& cloud)
{
	//decide which method to use
	switch(_dataType){
		case DataType::Binary:
			saveAsBinary(cloud);
			saveRGB();
		break;
		case DataType::Text:
			saveAsText(cloud);
			saveRGB();
		break;
	}
}

std::string FilesaveModule::GetFilenameTimed(bool rgb) 
{
	std::string fn;
	//if dirname is set, append it
	if(Dirname == nullptr){
		fn = std::string(_filename);
	}else{
		fn = *Dirname + std::string(_filename);
	}
	//append timestamp if needed
	switch(_timestampType){
		case TimestampType::Unix:
			fn = fn + std::string("_")+GetUnixTimestamp();
			// GetUnixTimestamp and GetSystemTimestamp interchangeable
			//EDIT Markus: NOT interchangable!!!!!
			//Unix-Timestamp is seconds since 1970 (just a single number)
			//System-Timestamp is formated (e.g. 2018-07-04_14-29-13)
			break;
		case TimestampType::System:
			fn = fn + std::string("_")+GetSystemTimestamp();
			break;
		default:
			break;
	}
	if(fn == _LastFilename){
		//Filename did not change, increase filenumber
		Filenumber++;
	}else{
		//Filename changed, start with number 1
		_LastFilename = fn;
		Filenumber = 1;
	}
	if (rgb == true) {
		Filenumber--;
	}
	//Append filenumber and fileending
	return fn + std::string("_") + std::to_string(Filenumber) + std::string(".pcd");
}

std::string FilesaveModule::GetUnixTimestamp()
{
	std::time_t result = std::time(nullptr);
	std::stringstream strm;
   	strm << result;
   	return strm.str();
}

std::string FilesaveModule::GetSystemTimestamp()
{
	std::time_t now = std::time(nullptr);
	tm *ltm = localtime(&now);
	std::stringstream strm;
	strm << (1900+ltm->tm_year) << '-';
	strm << (1 + ltm->tm_mon) << '-';
	strm << ltm->tm_mday << '_';
	strm << (1 + ltm->tm_hour) << '-';
	strm << (1 + ltm->tm_min) << '-';
	strm << (1 + ltm->tm_sec);
	return strm.str();
}

void FilesaveModule::saveAsText(const quanergy::PointCloudXYZIRConstPtr& cloud)
{
	pcl::io::savePCDFile(GetFilenameTimed(false), *cloud, true ); // Text format
}

void FilesaveModule::saveAsBinary(const quanergy::PointCloudXYZIRConstPtr& cloud)
{
	pcl::io::savePCDFile(GetFilenameTimed(false), *cloud, false ); // Binary format
}

void FilesaveModule::saveRGB()
{
	
	if (first == true)
	{
		std::time(&end);
    		double seconds = std::difftime(end, start);
		for (int i = 0; i < (seconds*10); i++)
			ipcam.grab();
		first = false;		
	}

	ipcam >> frame;

	std::stringstream strm;
	strm << GetFilenameTimed(true) << ".png";
	
	cv::imwrite(strm.str(), frame);
	std::cout << "image written" << std::endl;
}


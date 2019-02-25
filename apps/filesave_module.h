#ifndef QUANERGY_CLIENT_FILESAVE_MODULE_H
#define QUANERGY_CLIENT_FILESAVE_MODULE_H

#include <quanergy/common/pointcloud_types.h>
#include <string.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <ctime>

//! \brief A module that saves the received point cloud and a corresponding image.
class FilesaveModule{
public:
	/*! \brief An enum that sets the data type of saved point cloud.
	 */
	enum DataType{
		Binary,
		Text
	};
	
	/*! \brief An enum that sets the timestamp type of saved point cloud and image.
	 */
	enum TimestampType{
		None,
		Unix,
		System
	};

	/*! \brief Constructor that needs the point cloud data type, a chosen name and the timestamp type.
	\param dataType The saved .pcd file will have this format.
	\param filename If used, a timestamp will be added to the filename for every point cloud. 
	\param timestampType Unix timestamp is unformatted, System timestamp is formatted. 
	*/	
	FilesaveModule(DataType dataType, const char* filename, TimestampType timestampType=TimestampType::None);
	/*! \brief Deconstructor */	
	~FilesaveModule();

	
	DataType _dataType;
	TimestampType _timestampType;
	char* _filename;

	std::string _LastFilename;
	int Filenumber;

	std::string* Dirname = nullptr;

	/*! \brief A function that is called in the boost::signals2::connection pipeline in the main application.
	\param cloud The received point cloud. 
	*/	
	void slot(const quanergy::PointCloudXYZIRConstPtr& cloud);
private:
	std::string GetFilenameTimed(bool rgb);
	static std::string GetUnixTimestamp();
	static std::string GetSystemTimestamp();

	cv::VideoCapture ipcam;
	cv::Mat frame;
	bool first = true;
	std::time_t start, end;

	void saveAsText(const quanergy::PointCloudXYZIRConstPtr& cloud);
	void saveAsBinary(const quanergy::PointCloudXYZIRConstPtr& cloud);
	
	void saveRGB();
};

#endif

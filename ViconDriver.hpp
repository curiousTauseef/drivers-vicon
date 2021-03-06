#ifndef __VICON_DRIVER_HPP__
#define __VICON_DRIVER_HPP__

/** base types **/
#include <base/Time.hpp>
#include <base/Eigen.hpp>

/** Boost **/
#include <boost/shared_ptr.hpp>

/** Eigen library **/
#include <Eigen/Core>
#include <Eigen/Geometry>

#include <vector>

namespace vicon
{

    class DriverImpl;

    /**
     * @file
     * @author Jakob Schwendner <jakob.schwendner@dfki.de>
     * 
     * @section DESCRIPTION
     *
     * This class is wrapping the driver for the Vicon DataStream SDK (currently
     * version 1.1) It is intended for easy acces to the device using Eigen2
     * types as interface types. 
     */
    class Driver
    {
    public:
        enum GetResult { UNKNOWN, SUCCESS, INVALID_SUBJECT_NAME, INVALID_SEGMENT_NAME};

	Driver();

	/** 
	 * Open a connection to the Vicon DataStream host
	 *
	 * @param hostname name of the network host
	 * @param port port numer
	 * @return true of the connection was successful, false otherwise
	 */
	bool connect( const std::string& hostname, const unsigned int port = 801 );

	/**
	 * @return true if there is a connection to the server
	 */
	bool isConnected();

	/**
	 * disconnect from the server
	 */
	void disconnect();

	/**
	 * Step to the next frame in the data stream. Always call this method
	 * before accessing any of the data methods.
	 *
	 * @param timeout time in milliseconds to wait for a new frame
	 * @result true if a frame has been received
	 */
	bool getFrame( const base::Time& timeout = base::Time::fromSeconds(1.0) );

	/** 
	 * @result the timestamp of the current frame
	 */
	base::Time getTimestamp();

	/** 
	 * Get the transformation which transforms the given segment from the
	 * local to the global coordinate frame.
	 *
	 * @param subjectName name of the subject
	 * @param segmentName name of the segment in the subject
	 * @param inFrame is true if segment is present in the current frame (not occluded).
	 * @result transformation from the segments local to the global reference frame
	 */
	Eigen::Affine3d getSegmentTransform( const std::string& subjectName, const std::string& segmentName, bool& inFrame);

    /** For backcompatibility. */	
    Eigen::Affine3d getSegmentTransform( const std::string& subjectName, const std::string& segmentName) { 
        bool lin_frame; 
        return getSegmentTransform ( subjectName, segmentName, lin_frame );
    }

        /** Get the result o the last getSegmentTransform operation. */
        GetResult getLastResult() { return mLastResult; }

	/** 
	 * Returns a list of unlabeled markers found by the system in global
	 * coordinates.
	 *
	 * @result a vector of unlabeled markers
	 */
	std::vector<base::Vector3d> getUnlabeledMarkers();

    /** @brief Get axes direction.*/
    void getAxesDir ( int& x_dir, int& y_dir, int& z_dir );

    /** @brief Set axes directions.
     *
     * Mapping is as follows: Forward=1, Backward=-1, Left=2, Right=-2, Up=3, Down=-3 */
    void setAxesDir ( int x_dir, int y_dir, int z_dir );

    private:
	boost::shared_ptr<DriverImpl> impl;
        GetResult mLastResult;
    };
}

#endif

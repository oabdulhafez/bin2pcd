 
#include <iostream>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <fstream>
#include <string>
#include <sstream>

namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

using namespace std;

int main(int argc, char const *argv[])
{

  pcl::PointCloud<pcl::PointXYZI> point_cloud;
  pcl::PointXYZI point;

  // allocate 4 MB buffer (only ~130*4*4 KB are needed)
  int32_t num = 1000000;
  int Nfiles= 4500;
  int32_t file_size;
  float *data;
  int initial_frame;
  int num_frames;
  int velo_file;
  istringstream (argv[1]) >> initial_frame;
  istringstream (argv[2]) >> num_frames;
  istringstream (argv[3]) >> velo_file;
  std::string velodyne_file;
  if (velo_file < 10)
  {
    velodyne_file = "0"+std::to_string(velo_file);
  }
  else
  {
    velodyne_file = std::to_string(velo_file);
  }
  for (int loop = initial_frame; loop < num_frames; ++loop)
  {

    data = (float*)malloc(num*sizeof(float));

    // pointers
    float *px = data+0;
    float *py = data+1;
    float *pz = data+2;
    float *pr = data+3;

    // from the filename
    string filename;
    std::stringstream ss;
    ss << loop;
    if (loop < 10)
    {
      filename= "00000" + ss.str() ;
    }
    else if (loop < 100)
    {
      filename= "0000" + ss.str() ;
    }
    else if (loop < 1000)
    {
      filename= "000" + ss.str() ;
    }
    else
    {
      filename= "00" + ss.str();
    }

    cout<< "filename: "<< filename<< endl;
    string filenameIn= "/media/osama/8822969D22969034/Ubuntu_Downloads/dataset/sequences/" + velodyne_file + "/velodyne/" + filename + ".bin";

    fstream a_file( filenameIn.c_str() , ios::binary | ios::in |ios::ate);
    file_size= a_file.tellg();
    cout<< "file size: "<< file_size<< endl;

     a_file.seekg( 0, ios::beg );

     cout<< "start reading..."<<endl;
    if ( ! a_file.read( reinterpret_cast<char*>( data ), file_size ) )
      {
        cout << "Error reading from file" << endl;
        return 1;
      }

    cout<< "...end reading"<< endl;

    // change the num to the actual number of points 
    // num= (file_size/sizeof(float))/4;

    point_cloud.width= (file_size/sizeof(float))/4;
    point_cloud.height= 1;
    point_cloud.is_dense= false;
    point_cloud.points.resize(point_cloud.width * point_cloud.height);

    cout<< "resized to "<< point_cloud.points.size() <<endl;

     // fill in the point cloud
    for (int i=0; i<point_cloud.points.size() ; ++i) {
      point_cloud.points[i].x= *px;
      point_cloud.points[i].y= *py;
      point_cloud.points[i].z= *pz;
      point_cloud.points[i].intensity= *pr;
      // cout<< "file size: "<< file_size << "  num: "<< num<< endl<< "point: " << i<< endl;
      // point_cloud.points.push_back(point);
      px+=4; py+=4; pz+=4; pr+=4;
    }

    cout << "variable loaded" << endl;

    pcl::io::savePCDFileASCII ("/media/osama/8822969D22969034/Ubuntu_Downloads/dataset/sequences/" + velodyne_file + "/velodyne_pcd/" + filename + ".pcd", point_cloud);
    std::cerr << "Saved " << point_cloud.points.size () << " data points to " << filename << ".pcd." << std::endl;

    cout<< " end of loop"<< endl;
  }

  delete data;
  data= NULL;

  return 0;
}

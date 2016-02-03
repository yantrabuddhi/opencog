// TimeSpaceAtom.h
// make cicular buffer of struct of time+octomap array holding atom values
// api to put time space atom
// api to query time space for atom
// api to search and delete all atom occurences

#ifndef TIME_SPACE_ATOM_H
#define TIME_SPACE_ATOM_H
#include <iostream>
#include <boost/circular_buffer.hpp>
#include <list>
#include <map>
#include <string>
#include <chrono>
#include <algorithm>
#include "AtomOcTree.h"
//#include "AtomOcTreeNode.h"
using namespace std;
using namespace octomap;

typedef std::chrono::system_clock::time_point time_pt;
typedef std::chrono::system_clock::duration duration_c;
typedef list<time_pt> TimeList;
//typedef list<aHandle> AtomList;

//data structures
struct time_unit{
	time_pt t; duration_c duration;
	map<int,AtomOcTree> map_tree;
	time_unit(time_pt tp,duration_c d):t(tP),duration(d){}
	bool operator==(time_pt tp){
		return (tp>=t && tp<=t+duration);
	}
	//>,< not needed as only == search happens although created buffer should always be sorted, just simplifies a bit over search speed cost
	bool has_map(int handle){
		auto it=map_tree.find(handle);
		return !(it==map_tree.end());
	}
};

class TimeSpaceAtom{
	public:
	//API
	int AddMap(const string& name,float resolution_meters);//returns map handle
	bool GetMapHandleFromName(const string& name,int& handle);
	bool GetMapResolution(const int handle,float& res);
	//bool GetMapNameFromHandle(const int handle,string& name);//seems unlikely to be required
	bool GetCurrentTimeRange(time_pt& time_p,duration_c& duration);
	bool IsTimePointInRange(time_pt time_to_check,time_pt& t,duration_c& duration){
		return (time_to_check>=t && time_to_check<=t+duration);
	}
	bool CreateNewTimeUnit(const time_pt time_p,const duration_c duration);
	int PutAtomAtCurrentTime(const int map_handle,const point3d location,const Handle& ato);
	bool GetAtomCurrentTime(const int map_handle,const point3d location,Handle& ato);
	bool GetAtomAtTime(const time_pt& time_p,const int map_handle,const point3d location,Handle& ato);
	TimeList GetTimesOfAtomOccurenceAtLocation(const int map_handle,const point3d location,const Handle& ato);
	TimeList GetTimesOfAtomOccurenceInMap(int map_handle,const Handle& ato);
	point3d_list GetLocationsOfAtomOccurenceNow(const int map_handle,const Handle& ato);
	point3d_list GetLocationsOfAtomOccurenceAtTime(const time_pt& time_p,const int map_handle,const Handle& ato);
	//AtomList& GetAtomsInLocationBBXatTime();//BBX = bounding box
	
	public:
	//constructor
	TimeSpaceAtom(unsigned int num_time_units):map_count(0),time_circle(num_time_units){}
	private:
	int map_count;
	map<string,int>map_name;//each map may have translation rotation (orientation) co-ordinates managed by user
	map<int,float>map_res;//resolution of maps
	boost::circular_buffer<time_unit> time_circle;
	time_pt curr_time;duration_c curr_duration;
};
#end

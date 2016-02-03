// time space atom
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
#include "AtomOcTree.h"
//#include "AtomOcTreeNode.h"
using namespace std;
using namespace octomap;

typedef std::chrono::system_clock::time_point time_pt;
typedef std::chrono::system_clock::duration duration_c;
typedef list<time_pt> TimeList;
//typedef list<aHandle> AtomList;
class TimeSpaceAtom{
	public:
	//API
	int AddMap(const string& name);
	bool GetMapHandle(const string& name,int& handle);
	bool SetCurrentTimeRange(const time_pt& time_p,const duration_c duration);
	void GetCurrentTimeRange(time_pt& time_p,duration_c& duration);
	bool CreateNewTimeUnit(const time_pt time_p,const duration_c duration);
	int PutAtomAtCurrentTime(const int map_handle,const point3d location,const Handle& ato);
	bool GetAtomCurrentTime(const int map_handle,const point3d location,Handle& ato);
	bool GetAtomAtTime(const time_pt& time_p,const int map_handle,const point3d location,Handle& ato);
	TimeList GetTimesOfAtomOccurenceAtLocation(const int map_handle,const point3d location,const Handle& ato);
	point3d_list GetLocationsOfAtomOccurenceNow(const int map_handle,const Handle& ato);
	point3d_list GetLocationsOfAtomOccurenceAtTime(const time_pt& time_p,const int map_handle,const Handle& ato);
	TimeList GetTimesOfAtomOccurenceInMap(int map_handle,const Handle& ato);
	//AtomList& GetAtomsInLocationBBXatTime();//BBX = bounding box
	
	private:
	map<string,int>map_name;
};
#end

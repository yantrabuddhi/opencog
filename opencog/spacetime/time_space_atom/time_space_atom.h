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
#include <chrono>
#include "TemplateOcTree.h"
#include "TemplateOcTreeNode.h"

typedef int Hanadle;
class TimeSpaceAtom{
	public:
	//API
	int AddMap(std::string name);
	int GetMapHandle(std::string name);
	bool SetCurrentTimeRange();
	getCurrentTimeRange();
	CreateNewTimeUnit();
	int PutAtomCurrentTime(point3d location,const Handle& ato);
	Handle GetAtomCurrentTime();
	Handle GetAtomAtTime();
	TimeList GetTimesOfAtomOccurenceAtLocation();
	TimeList GetTimesOfAtomOccurence();
	Timelist GetTimesOfAtomOccurenceInLocationBox();
	point3d_list GetLocationsOfAtomOccurence();
	point3d_list GetLocationsOfAtomOccurenceAtTime();
};
#end

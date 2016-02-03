//TimeSpaceAtom.cpp
#include "TimeSpaceAtom.h"
#include <assert.h>

TimeSpaceAtom::TimeSpaceAtom(unsigned int num_time_units,vector<float>map_res_meters):time_circle(num_time_units),created_once(false)
{
	map_count=map_res_meters.size();//if zero throw exception
	int i=0;
	for_each(map_res_meters.begin(),map_res_meters.end(),[](auto resolution){i++;map_res[i]=resolution;});
}

unsigned int TimeSpaceAtom::GetMapCount()
{
	return map_count;
}

bool TimeSpaceAtom::GetMapResolution(const int handle,float& res)
{
	auto it=map_res.find(handle);
	if (it==map_res.end()) return false;
	res=it->second();
	return true;	
}

bool TimeSpaceAtom::GetCurrentTimeRange(time_pt& time_p,duration_c& duration)
{
	if (time_circle.size()<1) return false;
	time_p=curr_time;
	duration=curr_duration;
	return true;
}

bool TimeSpaceAtom::CreateNewTimeUnit(const time_pt time_p,const duration_c duration)
{
	//ideally greater than check should be done
	if (IsTimePointInRange(time_p,curr_time,curr_duration) ||
	    IsTimePointInRange(time_p+duration,curr_time,curr_duration)){
			return false;
	}
	time_unit temp(time_p,duration);
	for_each( map_res.begin(),map_res.end(),[](auto handle){
		temp.map_tree[handle]=AtomOcTree(map_res[handle]);
		}
	);
	time_circle.push_back(temp);
	created_once=true;
	return true;
}

bool TimeSpaceAtom::PutAtomAtCurrentTime(const int map_handle,const point3d location,const Handle& ato)
{
	//if (!created_once)return false;
	assert(created_once);
	int i=time_circle.capacity()-1;
	if (time_circle.size()<time_circle.capacity()) i=time_circle.size()-1;
	//if (!time_circle[i].has_map(handle)) return false;//may assert too
	assert(time_circle[i].has_map(map_handle));
	time_circle[i].map_tree[map_handle].updateNode(location,true);
	time_circle[i].map_tree[map_handle].setNodeData(location,ato);
	return true;
}

bool TimeSpaceAtom::GetAtomCurrentTime(const int map_handle,const point3d location,Handle& ato)
{
	//
	assert(created_once);
	int i=time_circle.capacity()-1;
	if (time_circle.size()<time_circle.capacity()) i=time_circle.size()-1;
	assert(time_circle[i].has_map(map_handle));
	OcTreeNode* result = time_circle[i].map_tree[map_handle].search(location);
	if (result==NULL) return false;
	ato=(static_cast<AtomOcTreeNode*>(result))->getData();
	if (ato==UndefinedHandle) return false;
	return true;
}

//TimeSpaceAtom.cpp
#include "TimeSpaceAtom.h"

int TimeSpaceAtom::AddMap(const string& name,float resolution_meters)
{
	map_count++;
	map_name[name]=map_count;
	map_res[map_count]=resolution_meters;
	return map_count;
}

bool TimeSpaceAtom::GetMapHandleFromName(const string& name,int& handle)
{
	auto it=map_name.find(name);
	if (it==map_name.end()) return false;
	handle=it->second();
	return true;
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
	if (IsTimePointInRange(time_p,curr_time,curr_duration) ||
	    IsTimePointInRange(time_p+duration,curr_time,curr_duration)){
			return false;
	}
	time_unit temp(time_p,duration);
	for_each( map_name.begin(),map_name.end(),[](auto name){
		int handle;GetMapHandleFromName(name,handle);temp.map_tree[handle]=AtomOcTree(map_res[handle]);
		}
	);
	
		
}

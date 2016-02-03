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

bool TimeSpaceAtom::GetAtomAtTime(const time_pt& time_p,const int map_handle,const point3d location,Handle& ato)
{
	//
	assert(created_once);
	//find time in time circle time unit
	auto it=std::find(std::begin(time_circle), std::end(time_circle), time_p);//time_circle.begin(),time_circle.end()
	if (it==std::end(time_circle))return false;
	assert(it->has_map(map_handle));
	OcTreeNode* result = it->map_tree[map_handle].search(location);
	if (result==NULL) return false;
	ato=(static_cast<AtomOcTreeNode*>(result))->getData();
	if (ato==UndefinedHandle) return false;
	return true;
}

TimeList TimeSpaceAtom::GetTimesOfAtomOccurenceAtLocation(const int map_handle,const point3d location,const Handle& ato)
{
	//
	TimeList tl;
	for_each(time_circle.begin(),time_circle.end(),[](auto tu){
		if (!tu.has_map(map_handle)return;
			OcTreeNode* result = tu.map_tree[map_handle].search(location);
			if (result==NULL) return;
			ato=(static_cast<AtomOcTreeNode*>(result))->getData();
			if (ato==UndefinedHandle) return;
			tl.push_back(tu.t)
	});
	return tl;
}

TimeList TimeSpaceAtom::GetTimesOfAtomOccurenceInMap(int map_handle,const Handle& ato)
{
	//
	TimeList tl;
	for_each(time_circle.begin(),time_circle.end(),[](auto tu){
		if (!tu.has_map(map_handle)return;
		bool found=false;
		//go through all nodes and leafs of octomap to search atom
		for(AtomOcTree::tree_iterator it = tu.map_tree[map_handle].begin_tree(),
        end=tu.map_tree[map_handle].end_tree(); it!= end; ++it){
			//
			if (it->getData()==ato) found=true;
		}
		if (found) tl.push_back(tu.t);
	});
	return tl;
}

point3d_list TimeSpaceAtom::GetLocationsOfAtomOccurenceNow(const int map_handle,const Handle& ato)
{
	//
	assert(created_once);
	point3d_list pl;
	int i=time_circle.capacity()-1;
	if (time_circle.size()<time_circle.capacity()) i=time_circle.size()-1;
	assert(time_circle[i].has_map(map_handle));
	for(AtomOcTree::tree_iterator it = time_circle[i].map_tree[map_handle].begin_tree(),
       end=time_circle[i].map_tree[map_handle].end_tree(); it!= end; ++it){
		//
		if (it->getData()==ato) pl.push_back(it.getCoordinate());
	}
	return pl;
}

point3d_list TimeSpaceAtom::GetLocationsOfAtomOccurenceAtTime(const time_pt& time_p,const int map_handle,const Handle& ato)
{
	//
	assert(created_once);
	point3d_list pl;
	auto it=std::find(std::begin(time_circle), std::end(time_circle), time_p);//time_circle.begin(),time_circle.end()
	if (it==std::end(time_circle))return false;
	assert(it->has_map(map_handle));//map handles not added dynamically, just during construction. assert in case user put wrong one
	for(AtomOcTree::tree_iterator ita = it->map_tree[map_handle].begin_tree(),
       end=it->map_tree[map_handle].end_tree(); ita!= end; ++ita){
		//
		if (ita->getData()==ato) pl.push_back(ita.getCoordinate());
	}
	return pl;	
}

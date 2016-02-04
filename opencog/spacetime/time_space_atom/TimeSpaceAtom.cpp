//TimeSpaceAtom.cpp
#include "TimeSpaceAtom.h"
//#include "octomap/OcTreeKey.h"
#include <assert.h>

TimeSpaceAtom::TimeSpaceAtom(unsigned int num_time_units,vector<double>map_res_meters):time_circle(num_time_units),created_once(false)
{
	map_count=map_res_meters.size();//if zero throw exception
	int i=0;
	for_each(map_res_meters.begin(),map_res_meters.end(),[&](auto resolution){i++;map_res[i]=resolution;});
}

unsigned int TimeSpaceAtom::GetMapCount()
{
	return map_count;
}

bool TimeSpaceAtom::GetMapResolution(const int handle,double& res)
{
	auto it=map_res.find(handle);
	if (it==map_res.end()) return false;
	res=it->second;
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
	if (created_once){
	if (IsTimePointInRange(time_p,curr_time,curr_duration) ||
	    IsTimePointInRange(time_p+duration,curr_time,curr_duration)){
			return false;
	}
	}
	
	time_unit temp(time_p,duration);
	/*
	for_each( map_res.begin(),map_res.end(),[&](auto handle){
		temp.map_tree[handle.first]=AtomOcTree(map_res[handle.second]);
		}
	);
	*/
	time_circle.push_back(temp);
	int i=time_circle.capacity()-1;
	if (time_circle.size()<time_circle.capacity()) i=time_circle.size()-1;
	for_each( map_res.begin(),map_res.end(),[&](auto handle){
		//time_circle[i].map_tree[handle.first]=AtomOcTree(handle.second);
		time_circle[i].map_tree[handle.first].setResolution(handle.second);
		}
	);
	
	curr_time=time_p;
	curr_duration=duration;
	created_once=true;
	return true;
}

bool TimeSpaceAtom::PutAtomAtCurrentTime(const int map_handle,const point3d location,const aHandle& ato)
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

bool TimeSpaceAtom::RemoveAtomAtCurrentTime(const int map_handle,const point3d location)
{
	assert(created_once);
	int i=time_circle.capacity()-1;
	if (time_circle.size()<time_circle.capacity()) i=time_circle.size()-1;
	assert(time_circle[i].has_map(map_handle));
	time_circle[i].map_tree[map_handle].updateNode(location,false);
	//time_circle[i].map_tree[map_handle].setNodeData(location,ato);
	return true;
}

bool TimeSpaceAtom::RemoveAtomAtTime(time_pt tp,const int map_handle,const point3d location)
{
	assert(created_once);
	auto it=std::find(std::begin(time_circle), std::end(time_circle), tp);//time_circle.begin(),time_circle.end()
	if (it==std::end(time_circle))return false;
	assert(it->has_map(map_handle));
	it->map_tree[map_handle].updateNode(location,false);
	//time_circle[i].map_tree[map_handle].setNodeData(location,ato);
	return true;
}


bool TimeSpaceAtom::GetAtomCurrentTime(const int map_handle,const point3d location,aHandle& ato)
{
	//
	assert(created_once);
	int i=time_circle.capacity()-1;
	if (time_circle.size()<time_circle.capacity()) i=time_circle.size()-1;
	assert(time_circle[i].has_map(map_handle));
	OcTreeNode* result = time_circle[i].map_tree[map_handle].search(location);
	if (result==NULL) {ato=UndefinedHandle;return false;}
	ato=(static_cast<AtomOcTreeNode*>(result))->getData();
	if (ato==UndefinedHandle) return false;
	return true;
}

bool TimeSpaceAtom::GetAtomAtTime(const time_pt& time_p,const int map_handle,const point3d location,aHandle& ato)
{
	//
	assert(created_once);
	//find time in time circle time unit
	auto it=std::find(std::begin(time_circle), std::end(time_circle), time_p);//time_circle.begin(),time_circle.end()
	if (it==std::end(time_circle))return false;
	assert(it->has_map(map_handle));
	OcTreeNode* result = it->map_tree[map_handle].search(location);
	if (result==NULL) {ato=UndefinedHandle;return false;}
	ato=(static_cast<AtomOcTreeNode*>(result))->getData();
	if (ato==UndefinedHandle) return false;
	return true;
}//ok

TimeList TimeSpaceAtom::GetTimesOfAtomOccurenceAtLocation(const int map_handle,const point3d location,const aHandle& ato)
{
	//
	TimeList tl;
	//for_each(time_circle.begin(),time_circle.end(),[&](auto tu){//map not copied correctly
	for(auto tu=std::begin(time_circle), end=std::end(time_circle);(tu!=end);tu++){
		if (!tu->has_map(map_handle)){cout<<"map "<<map_handle<<" not found"<<endl;continue;}
			OcTreeNode* result = tu->map_tree[map_handle].search(location);
			if (result==NULL) {cout<<"null ret by search"<<endl;continue;}
			aHandle ato_t=(static_cast<AtomOcTreeNode*>(result))->getData();
			if (ato_t!=ato) {cout<<"incorrect atom="<<ato_t;continue;}
			tl.push_back(tu->t);
	};
	return tl;
}//ok time_circle.begin is causing problem
TimeList TimeSpaceAtom::GetTimesOfAtomOccurenceInMap(int map_handle,const aHandle& ato)
{
	//
	TimeList tl;
	//for_each(time_circle.begin(),time_circle.end(),[&](auto tu){
	for(auto tu=std::begin(time_circle), end=std::end(time_circle);(tu!=end);tu++){
		if (!tu->has_map(map_handle))continue;
		bool found=false;
		//go through all nodes and leafs of octomap to search atom
		for(AtomOcTree::tree_iterator it = tu->map_tree[map_handle].begin_tree(),
        end=tu->map_tree[map_handle].end_tree(); it!= end; ++it){
			//
			if (it->getData()==ato) {found=true;break;}
		}
		if (found) tl.push_back(tu->t);
	};
	return tl;
}//ok

/*
TimeList TimeSpaceAtom::GetTimesOfAtomOccurenceAtLocation(const int map_handle,const point3d location,const aHandle& ato)
{
	//
	TimeList tl;
	for_each(time_circle.begin(),time_circle.end(),[&](auto tu){//map not copied correctly
		if (!tu.has_map(map_handle)){cout<<"map "<<map_handle<<" not found"<<endl;return;}
			OcTreeNode* result = tu.map_tree[map_handle].search(location);
			if (result==NULL) {cout<<"null ret by search"<<endl;return;}
			aHandle ato_t=(static_cast<AtomOcTreeNode*>(result))->getData();
			if (ato_t!=ato) {cout<<"incorrect atom="<<ato_t;return;}
			tl.push_back(tu.t);
	});
	return tl;
}//not ok time_circle.begin is causing problem

TimeList TimeSpaceAtom::GetTimesOfAtomOccurenceInMap(int map_handle,const aHandle& ato)
{
	//
	TimeList tl;
	for_each(time_circle.begin(),time_circle.end(),[&](auto tu){
		if (!tu.has_map(map_handle))return;
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
*/
point3d_list TimeSpaceAtom::GetLocationsOfAtomOccurenceNow(const int map_handle,const aHandle& ato)
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
}//ok

point3d_list TimeSpaceAtom::GetLocationsOfAtomOccurenceAtTime(const time_pt& time_p,const int map_handle,const aHandle& ato)
{
	//
	assert(created_once);
	point3d_list pl;
	auto it=std::find(std::begin(time_circle), std::end(time_circle), time_p);//time_circle.begin(),time_circle.end()
	if (it==std::end(time_circle))return point3d_list();
	assert(it->has_map(map_handle));//map handles not added dynamically, just during construction. assert in case user put wrong one
	for(AtomOcTree::tree_iterator ita = it->map_tree[map_handle].begin_tree(),
       end=it->map_tree[map_handle].end_tree(); ita!= end; ++ita){
		//
		if (ita->getData()==ato) pl.push_back(ita.getCoordinate());
	}
	return pl;	
}//ok

void TimeSpaceAtom::RemoveAtom(const aHandle& ato)
{
	//remove all occurences of atom in all maps at all times
	//point3d_list pl;
	for(auto tu=std::begin(time_circle), end=std::end(time_circle);(tu!=end);tu++){
		for(auto it1=std::begin(tu->map_tree),endit1=std::end(tu->map_tree);it1!=endit1;it1++){
			//pl.clear();
			for(AtomOcTree::tree_iterator it2 = it1->second.begin_tree(),
			endit2=it1->second.end_tree(); it2!= endit2; ++it2){
				if (it2->getData()==ato) {
					//pl.push_back(it2.getCoordinate());
					it2->setData(UndefinedHandle);
				/*
					if (it2->hasChildren()){
						for (int i=0;i<8;i++){
							if(it2->childExists(i))it2->deleteChild(i);
						}
					}
					
					it2->setLogOdds(0);
					it2->pruneNode();
					*/
				//cout<<it1->first<<"::"<<it2->getData()<<"::"<<it2.getCoordinate()<<endl;
				}
			};
			/*
			cout<<"remove size="<<oc.size()<<endl;
			for(auto it3=std::begin(oc),endit3=std::end(oc);it3!=endit3;it3++){
				//cout<<*it3<<endl;
				point3d pp=it1->second.keyToCoord(*it3);
				cout<<pp<<endl;
				tu->map_tree[it1->first].updateNode(pp,false);
			};
			*/
		/*	
			cout<<"remove size="<<pl.size()<<endl;
			for(auto it3=std::begin(pl),endit3=std::end(pl);it3!=endit3;it3++){
				//RemoveAtomAtTime(tu->t,it1->first,*it3);
				//cout<<*it3<<endl;
				//it1->second.updateNode(*it3,false);
				//tu->map_tree[it1->first].updateNode(*it3,false);
			};
			*/
		};
	};
}


/*
void TimeSpaceAtom::RemoveAtom(const aHandle& ato)
{
	//remove all occurences of atom in all maps at all times
	point3d_list pl;
	for_each(time_circle.begin(),time_circle.end(),[&](auto tu){
		for_each(tu.map_tree.begin(),tu.map_tree.end(),[&](auto mapt){
			auto map_handle=mapt.first;
			pl.clear();
			for(AtomOcTree::leaf_iterator it = tu.map_tree[map_handle].begin_leafs(),
			end=tu.map_tree[map_handle].end_leafs(); it!= end; ++it){
				if (it->getData()==ato) {pl.push_back(it.getCoordinate());
				cout<<map_handle<<"::"<<it->getData()<<"::"<<it.getCoordinate()<<endl;
				}
			}
			for_each(pl.begin(),pl.end(),[&](auto pt){
				tu.map_tree[map_handle].updateNode(pt,false);
			});
		});
	});

}// not okay
*/

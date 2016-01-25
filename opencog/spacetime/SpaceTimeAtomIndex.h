#ifndef SPACE_TIME_ATOM_INDEX_H
#define SPACE_TIME_ATOM_INDEX_H

#include <vector>
#include <list>
#include <map>
using namespace std;

//template <class handle>
struct location{
  int x,y,z;
  //handle h;//atom ptr
  bool operator==(location& rhs){
    return (x==rhs.x && y==rhs.y && z==rhs.z);
  }
  bool operator < (location& rhs){
    if (x<rhs.x)return true;
    if (x==rhs.x && y<rhs.y)return true;
    if (x==rhs.x && y==rhs.y && z<rhs.z)return true;
    return false;
  }
};
//handle has to be pointer
template <class handle>
struct time_unit{
  time_point t;
  map<location,handle> *m;
};
//cicular buffer
template <class handle>
class time_circle{
  unsigned int num_time_unit;float time_res,space_res;//time resolution and space resolution
  float originX,originY,originZ;

  int curr_t,start,stop;//start,stop is vector index of start,stop curr_t is index current vector of time
  time_point start_t;//current start of time
  vector<time_unit<handle>>vt;

public:
  time_circle(unsigned int time_units,time_point start_time,float time_res_seconds,float space_resolution,float oX,float oY,float oZ):
  num_time_unit(time_units),curr_t(0),start(0),start_t(start_time),time_res(time_res_seconds){
    vt.reserve(time_units);
    for (int a=0;a<time_units;a++){
      vt[a].m=new map<location,handle>;
    }
  }
  ~time_circle(){
    for (int a=0;a<time_units;a++){
      delete vt[a].m;
    }
  }
  time_point get_start_time();
  time_point get_stop_time();
  bool put(handle atom,time_point t,location *l=NULL);
  handle get(time_point t,location *l=NULL);
  time_interval_list get(handle atom,location l);
  space_block_list get(handle atom,time_point t);
  void getResOrigin(float& res,float& oX,float& oY,float& oZ){
    res=space_res;oX=originX;oY=originY;oZ=originZ;
  }

};
//API
#endif

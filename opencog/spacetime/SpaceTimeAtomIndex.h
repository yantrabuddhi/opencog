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

template <class handle>
struct time_unit{
  time_point t;
  map<location,handle> *m;
};
//cicular buffer
template <class handle>
struct time_circle{
  unsigned int num_time_unit;
  int start,stop;
  time_point start_t,stop_t;
  vector<time_unit<handle>>vt;
  time_circle(unsigned int time_units)num_time_unit(time_units),start(0),stop(-1){
    vt.reserve(time_units);
  }
};
//API
template <class handle>
class SpaceTimeAtomIndex{
  unsigned int num_time_unit;float space_res;
  float originX,originY,originZ;
  time_circle<handle>t_circle;
public:
  SpaceTimeAtomIndex(unsigned int num_time_units,float space_resolution,float oX,float oY,float oZ);
  void getResOrigin(float& res,float& oX,float& oY,float& oZ){
    res=space_res;oX=originX;oY=originY;oZ=originZ;
  }
  bool put(handle atom,time_point t,location *l=NULL);
  handle get(time_point t,location *l=NULL);
  time_interval_list get(handle atom,location l);
  space_block_list get(handle atom,time_interval t);
}
#endif

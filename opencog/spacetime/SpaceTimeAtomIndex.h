#ifndef SPACE_TIME_ATOM_INDEX_H
#define SPACE_TIME_ATOM_INDEX_H

#include <vector>
#include <list>
#include <map>
//#include <ctime>
#include <chrono>
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
typedef chrono::system_clock::time_point time_pt;
typedef list<location> location_list;
typedef list<time_pt> tm_list;
//handle has to be pointer
template <class handle>
struct time_unit{
  time_pt t;
  map<location,handle> m;
};
//cicular buffer
template <class handle>
class time_circle{
private:
  unsigned int num_time_unit;float time_res,space_res;//time resolution and space resolution
  float originX,originY,originZ;

  int curr_t,start,stop;//start,stop is vector index of start,stop curr_t is index current vector of time
  time_pt start_t;//current start of time
  vector<time_unit<handle>>vt;

public:
  time_circle(unsigned int time_units,time_pt start_time,float time_res_seconds,float space_resolution,float oX,float oY,float oZ):
  num_time_unit(time_units),curr_t(0),start(0),start_t(start_time),time_res(time_res_seconds){
    //if time units is less than two then throw exception
    vt.reserve(time_units);
    /*
    for (int a=0;a<time_units;a++){
      vt[a].m=new map<location,handle>;
    }*/
  }
  ~time_circle(){
    /*
    for (int a=0;a<time_units;a++){
      delete vt[a].m;
    }
    */
  }
  time_pt get_start_time();
  time_pt get_stop_time();
  int get_time_unit(time_pt tp);//-neg if out of circle

  void get_res_origin(float& res,float& oX,float& oY,float& oZ){
    res=space_res;oX=originX;oY=originY;oZ=originZ;
  }
  inline unsigned int get_num_time_units(){return num_time_unit;}
  //API
  int get_current_time_handle();
  int next_new_time_unit();
  void set_start_time(time_t t);
  bool put(handle atom,int time_handle,location l)
  bool put(handle atom,time_pt t,location l);
  bool get_atom(handle &h,time_pt t,location l);
  void get_times(handle atom,time_pt_list& t,location l);
  void get_locations(handle atom,time_pt t,location_list& ll);
  //bool save(std::string file);//resolve atom uuid
  //bool load(std::string file);//
};
//API
#endif

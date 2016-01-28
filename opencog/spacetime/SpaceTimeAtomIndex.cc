#include "SpaceTimeAtomIndex.h"
template <class handle>
bool time_circle::put(handle atom,time_pt t,location l)
{
  int tt=get_time_unit(t);//negative distance from start if full rotation
  //struct time_pt tms;
  chrono::system_clock::duration dd;//seconds

  if (tt<0) {//fold all
    for (int a=0;a<vt.size();a++){
      vt[a].m.clear();
      //set start and current to same
    }
    //tms.tm_seconds=(-tt)*time_resolution
    dd=chrono::seconds((-tt)*time_resolution);
    vt[0].t=start_t+dd; //check if this is correct
    start_t=start_t+dd;
    start=0;
    stop=0;
    curr_t=0;
    vt[0].m[l]=atom;
    //int nt=get_next(-tt);
    //vt[get_next(-tt)].m[l]=atom;
  }else{
    int dt=time_unit_dist(start,curr_t);
    int dtt=time_unit_dist(start,tt);
    if (dtt>dt){
      for (int a=dt;a<dtt;a++){
        dd=chrono::seconds(a*time_resolution)
        vt[curr_t].t=start_t+dd; //fix this for time calc
        vt[curr_t].m.clear();
        move_to_next();//modify curr_t
      }
      vt[curr_t].t=start_t+chrono::seconds(dtt*time_resolution); // fix this
      vt[curr_t].m[l]=atom;
    }
    else{
      vt[tt].m[l]=atom;
    }
  }
  return true;
}

template <class handle>
bool time_circle::get_atom(handle &h,time_t t,location l)
{
  int tt=get_time_unit(t);
  if (tt<0)return false;
  h= vt[tt].m[l];
  return true;
}

template <class handle>
void time_circle::get_times(handle atom,time_t_list& t,location l)
{
  //threaded search - boost threads
}

template <class handle>
void time_circle::get_locations(handle atom,time_t t,location_list& ll){
  ll.clear();
  int tt=get_time_unit(t);
  if (tt<0)return;
  for (auto it=vt[tt].m.begin();it=vt[tt].m.end();it++){
    if (it->second()==atom) ll.push_front(it->first());
  }
}

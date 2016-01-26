#include "SpaceTimeAtomIndex.h"
template <class handle>
bool time_circle::put(handle atom,time_t t,location l)
{
  int tt=get_time_unit(t);
  struct tm tms;

  if (tt<0) {//fold all
    for (int a=0;a<vt.size();a++){
      vt[a].m.clear;
      //set start and current to same
    }
    tms.tm_seconds=(-tt)*time_resolution
    vt[0].t=start_t+mktime(tms); //check if this is correct
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
        vt[curr_t].t=start_t+a*time_resolution; //fix this for time calc
        vt[curr_t].m.clear();
        move_to_next();
      }
      vt[curr_t].t=start_t+dtt*time_resolution; // fix this
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
  //threaded search
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

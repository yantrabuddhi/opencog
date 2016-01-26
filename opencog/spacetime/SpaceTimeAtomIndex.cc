#include "SpaceTimeAtomIndex.h"
template <class handle>
bool time_circle::put(handle atom,time_t t,location l)
{
  int tt=get_time_unit(t);
  if (tt<0) {//fold
    vt[get_next(-tt)].m[l]=atom;
  }else{
    vt[tt].m[l]=atom;
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
void time_circle::get_times(handle atom,time_t_list& t,location l);

template <class handle>
void time_circle::get_locations(handle atom,time_t t,location_list& ll){
  ll.clear();
  int tt=get_time_unit(t);
  if (tt<0)return;
  for (auto it=vt[tt].m.begin();it=vt[tt].m.end();it++){
    if (it->second()==atom) ll.push_front(it->first());
  }
}

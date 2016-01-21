#ifndef SPACE_TIME_ATOM_INDEX_H
#define SPACE_TIME_ATOM_INDEX_H
#include <list>
enum unit{MM,CM,M,KM};
struct space_block{float x1,y1,z1;int delta;unit u;};//meter, cm,mm
struct sta_space;
struct time_interval{
  time;int delay;
};
struct sta_time{time_interval, sta_space* };
struct sta_space{space_block};
typedef list<time_interval> time_interval_list;
typedef list<space_block> space_block_list;
typedef list<sta_space>space_list;
typedef list<sta_time> time_list;
//API
template <class handle>
class SpaceTimeAtomIndex{
public:
  bool put(handle atom,time_interval t,location* l=NULL);
  handle get(time_interval t,location *l=NULL);
  time_interval_list get(handle atom,location l);
  space_block_list get(handle atom,time_interval t);
}
#endif

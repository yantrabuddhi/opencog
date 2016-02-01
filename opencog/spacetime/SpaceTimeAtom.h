#include <chrono>
#include <boost/circular_buffer.hpp>
#include <map>
#include <list>
using namespace std;
struct location{
  int x,y,z;
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
typedef chrono::system_clock::duration elapse;
template <class T>
struct time_unit{
  time_pt t;
  elapse e;
  map<location,T> m;
  bool operator==(time_unit& rhs){
    return ((t==rhs.t) && (e==rhs.e);
  }
  bool operator==(time_pt tpt){
    return ((tpt<=t+e) && (tpt>=t);
  }
  bool operator<((time_unit& rhs)){
  }
};

template <class T>
class SpaceTimeAtom{
private:
  boost::circular_buffer<time_unit<T>> time_buffer;

public:
  //API
  SpaceTimeAtom(unsigned int TimeBufferSize, double TimeUnitRes):
  time_buffer(TimeBufferSize)
  {
    //
  }
  //if put time requested is in unit time then put everything in unit time
  //if put time is outside unit time
  int make_new_time_unit(time_pt tpt, double time_duration);//returns t handle
  void putLocationAtom(int t_handle,location l,T atom);
  bool getAtom(time_pt tpt,location l, T& atom);
  list<time_pt> getTimes(location l, T atom);
  list<location> getLocations(time_pt tpt,T atom);
};

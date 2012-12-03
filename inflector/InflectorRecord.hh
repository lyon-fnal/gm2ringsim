#ifndef INFLECTORRECORD_HH
#define INFLECTORRECORD_HH

#include <vector>

namespace gm2ringsim {

  class InflectorRecord {
  public:
    InflectorRecord();
    InflectorRecord(int);
    ~InflectorRecord();

    int x;


  };

  typedef std::vector<InflectorRecord> InflectorRecordCollection;



}

#endif

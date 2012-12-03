#ifndef INFLECTORRECORD_HH
#define INFLECTORRECORD_HH


namespace gm2ringsim {

  class InflectorRecord {
  public:
    InflectorRecord();
    ~InflectorRecord();

    int x;


  };

  typedef std::vector<InflectorRecord> InflectorRecordCollection;



}

#endif

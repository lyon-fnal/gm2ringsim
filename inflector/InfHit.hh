#ifndef INFHIT_HH
#define INFHIT_HH


namespace gm2ringsim {

  class InfHit {
  public:
    InfHit();
    ~InfHit();

    int x;


  };

  typedef std::vector<InfHit> InfHitCollection;



}

#endif

#ifndef QUADFIELD_HH
#define QUADFIELD_HH

/** @file QuadField.hh 

    Provides a host of specifications to support the quadField class: 
    quadFieldImpl, innerFieldImpl, vanishingInnerImpl,
    simpleInnerImpl, mappedInnerImpl, outerFieldImpl,
    vanishingOuterImpl, simpleOuterImpl, quadFieldFactory,
    quadFieldMessenger, mappedInnerMessenger, simpleInnerMessenger,
    and simpleOuterMessenger.


    @author Zach Hartwig
    @author Kevin Lynch
    @date 2005-2009
*/

#include "Geant4/G4ElectroMagneticField.hh"

#include <fstream>
#include <vector>
#include <CLHEP/Matrix/Matrix.h>

namespace gm2ringsim {
  /** Abstract base class for concrete quadrupole field implementations. */
  class QuadFieldImpl {
  public:
    // FIXME: Tried to add because QuadFieldFactory destructor
    //        tries to destroy a bunch of InnerField Impl and
    //        gm2ringsim won't compile if I delete an object
    //        of an abstract class (gm2ringsim::InnerFieldImpl )
    //        that doesn't have a virtual destructor
    //virtual ~QuadFieldImpl();
    
    /** Returns the field value at @p Point in @p EMfield in the
	scraping limit.  Concrete implementations must modify ONLY the
	electric field components of EMfield (3-5). */
    virtual void GetScrapingFieldValue(const double *Point,
				       double *EMfield) const = 0;
    /** Returns the field value at @p Point in @p EMfield in the
	non-scraping limit.  Concrete implementation must modify ONLY
	the electric field components of EMfield (3-5). */
    virtual void GetStorageFieldValue(const double *Point,
				      double *EMfield) const = 0;
  };
  

  /** Abstract base class for the implementation of the quadrupole field
      inside the volume bounded by the quadrupole plates. */
  class InnerFieldImpl : public QuadFieldImpl {};
  
  
  /** Abstract base class for the implementation of the quadrupole field
      outside the volume bounded by the quadrupole plates. */
  class OuterFieldImpl : public QuadFieldImpl {};
  
  
  /** The concrete class that provides the interface to the inner and
      outer field implementation classes.  A quadField instantiation
      must be created by quadFieldFactory.  Hence, this class is a
      friend to quadFieldFactory, and the latter is the only class with
      access to call the constructor.  */
  class QuadField : public G4ElectroMagneticField {
  public:
    /** Returns the field value at @p Point in @p EMfield */  
    void GetFieldValue( const double *Point,
			double *EMfield ) const;
    
    /** Returns true; electric fields modify particle energy. */
    G4bool DoesFieldChangeEnergy() const { return true; }
    
    friend class QuadFieldFactory;
    
    /** Get the scraping turn off time. */
    double scraping_turnoff() const { return scrapingTurnOffTime; }
    /** Set the scraping turn off time; returns the previous setting. */
    double scraping_turnoff(double offtime);
    
    /** Get the quadrupole time constant. */
    double scraping_timeconst() const { return quadTimeConstant; }
    /** Set the quadrupole time constant; returns the previous setting. */
    double scraping_timeconst(double timeconst);
    
    /** Get the time offset. */
    double scraping_offset() const { return timeOffset; }
    /** Set the time offset; returns the previous setting. */
    double scraping_offset(double offset);
    
    /** Is this field set to do scraping? */
    bool do_scraping() const { return do_scraping_; }
    /** Set the scraping status; return the previous setting. */
    bool do_scraping(bool enable);
    
  private:
    QuadField(InnerFieldImpl *ifi, OuterFieldImpl *ofi, bool DoScraping);
    InnerFieldImpl const *ifi_;
    OuterFieldImpl const *ofi_;
    double scrapingTurnOffTime, quadTimeConstant, timeOffset;
    bool do_scraping_;
  };


/** A concrete implementation of the inner field, based on pure
    quadrupole field equations. */
class SimpleInnerImpl : public InnerFieldImpl {
public:
  /** Provides the only defined constructor:
      @p tb_voltage The voltage of the top/bottom plates in the
      storage configuration
      @p delta_volts[4] The storage-scraping voltage deltas for the
      plates in Inner, Outer, Top, Bottom order
   */
  SimpleInnerImpl(double tb_voltage, double delta_volts[4], bool DoScraping);
  virtual void GetScrapingFieldValue(const double *Point,
				     double *EMfield) const;
  virtual void GetStorageFieldValue(const double *Point,
				    double *EMfield) const;

  /** Get the current top/bottom plate voltage during storage. */
  double storage_tb_volts() const { return storage_tb_volts_; }
  /** Set the current top/bottom plate voltage during storage;
      returns the previous value. */
  double storage_tb_volts(double);

  /** Get the current plate voltage deltas between storage and scraping. */
  void scraping_dvolts(double[4]) const;
  /** Sets the current plate voltage deltas between storage and
      scraping; modifies the input array with the old values. */
  void scraping_dvolts(double[4]);
private:
  double storage_tb_volts_;
  bool DoScraping_;
  double scraping_dvolts_[4];
};


/** A concrete implementation of the inner field with value zero. */
class VanishingInnerImpl : public InnerFieldImpl {
public:
  virtual void GetScrapingFieldValue(const double *Point,
				     double *EMfield) const;
  virtual void GetStorageFieldValue(const double *Point,
				    double *EMfield) const;
};


/** A concrete implementation of the outer field, based on pure linear
    field equations. Internal implementation is copied from
    simpleInnerImpl. */
class SimpleOuterImpl : public OuterFieldImpl {
public:
  /** Provides the only defined constructor:
      @p tb_voltage The voltage of the top/bottom plates in the
      storage configuration
      @p delta_volts[4] The storage-scraping voltage deltas for the
      plates in Inner, Outer, Top, Bottom order
   */
  SimpleOuterImpl(double tb_voltage, double delta_volts[4], bool DoScraping);
  virtual void GetScrapingFieldValue(const double *Point,
				     double *EMfield) const;
  virtual void GetStorageFieldValue(const double *Point,
				    double *EMfield) const;

  /** Get the current top/bottom plate voltage during storage. */
  double storage_tb_volts() const { return storage_tb_volts_; }
  /** Set the current top/bottom plate voltage during storage;
      returns the previous value. */
  double storage_tb_volts(double);

  /** Get the current plate voltage deltas between storage and scraping. */
  void scraping_dvolts(double[4]) const;
  /** Sets the current plate voltage deltas between storage and
      scraping; modifies the input array with the old values. */
  void scraping_dvolts(double[4]);
private:
  double storage_tb_volts_;
  bool DoScraping_;
  double scraping_dvolts_[4];
  static double const rmax;
};


/** A concrete implementation of the outer field with value zero. */
class VanishingOuterImpl : public OuterFieldImpl {
public:
  virtual void GetScrapingFieldValue(const double *Point,
				     double *EMfield) const;
  virtual void GetStorageFieldValue(const double *Point,
				    double *EMfield) const;
};

enum inner_field_impl_type { VANISHING_INNER, SIMPLE_INNER, MAPPED_INNER };
enum outer_field_impl_type { VANISHING_OUTER, SIMPLE_OUTER };

/** A concrete factory that creates the concrete quadField. */
class QuadFieldFactory {
public:
  QuadFieldFactory(bool DoScraping);
  virtual ~QuadFieldFactory();
  QuadField* buildQuadField(int quadNumber, int quadSection);

  void setInnerImpl(int quadNumber, int quadSection, inner_field_impl_type);
  void setOuterImpl(int quadNumber, int quadSection, outer_field_impl_type);

private:
  InnerFieldImpl* innerFromType(int, int, inner_field_impl_type);
  OuterFieldImpl* outerFromType(int, int, outer_field_impl_type);

  InnerFieldImpl *ifi_[4][2];
  OuterFieldImpl *ofi_[4][2];

  bool DoScraping_;
};


struct unknown_inner_field_impl_type {};
struct unknown_outer_field_impl_type {};



/*
class quadFieldMessenger : public G4UImessenger {};

class mappedInnerMessenger : public G4UImessenger {};
class simpleInnerMessenger : public G4UImessenger {};

class simpleOuterMessenger : public G4UImessenger {};
*/




/*
//convert this original quadField implementation into mappedInnerImpl 
class mappedInnerImpl : public innerFieldImpl {}; // should slurp
inputs from a runtime text file, not statics
*/
#if 0
/** Implements the quadrupole field, field maps, and scraping. */
class QuadField : public G4ElectroMagneticField
{
  
public:
  /** @pre @p quadNumber and @p sectionType must be in range */
  QuadField(G4int quadNumber, G4int sectionType, bool DoScraping);
  ~QuadField();
  
  void GetFieldValue( const double *Point,
		      double *EMfield ) const;
  

  G4bool DoesFieldChangeEnergy() const { return true; }
  
private:
  void CalcWithFieldMaps(G4double, G4double, 
			 G4double, G4double *) const;
  void GenerateFieldMaps();
  void LoadFieldMaps(G4int); 

  static bool generate_maps;

  G4int const quadNumber, sectionType, n_max, r_rims;
  G4double const r0, r_max, r_delta;

  G4int const th_spokes;
  G4double const th_max, th_delta;

  G4double scrapingTurnOffTime;
  G4double quadTimeConst;

  std::vector<G4String> mapFNames;
  
  std::ofstream writeQuadFieldMap;
  std::ifstream readQuadFieldMap;
  
  std::vector<G4double> r_vec;
  std::vector<G4double> th_vec;

  std::vector<G4double> Er_storageVec;
  std::vector<G4double> Eth_storageVec;
  std::vector<CLHEP::HepMatrix> Er_storageMatrixHolder;
  std::vector<CLHEP::HepMatrix> Eth_storageMatrixHolder;
  
  std::vector<G4double> Er_scrapingVec;
  std::vector<G4double> Eth_scrapingVec;
  std::vector<CLHEP::HepMatrix> Er_scrapingMatrixHolder;
  std::vector<CLHEP::HepMatrix> Eth_scrapingMatrixHolder;
  
};



#endif // #if 0

}//namespace gm2ringsim
 
#endif // QUADFIELD_HH

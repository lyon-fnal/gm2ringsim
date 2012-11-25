#ifndef INFLECTORFIELD_HH
#define INFLECTORFIELD_HH

/** @file inflectorField.hh

    Provides declarations of multiple inflector field types:
    vanishingInflectorField, simpleInflectorField,
    mappedInflectorField. 

    @bug These should all have a common base class with a public field
    normalization member function.

    @author Zach Hartwig
    @author Kevin Lynch
    @date 2005-2009

    @author Brendan Kiburg
    @date Nov 2012 artified
*/

#include <fstream>

#include "Geant4/G4MagneticField.hh"


//////////// vanishingInflectorField
/** Provides the simplest possible inflector field, where the
    inflector fully cancels the yoke field. */
class vanishingInflectorField : public G4MagneticField {

public:
  vanishingInflectorField(){}
  ~vanishingInflectorField(){}

  void GetFieldValue( const double *Point, double *Bfield ) const;

};

//////////// simpleInflectorField
/** Provides a simple, purely vertical field.

    The field over the entire aperture is derived from the field along
    the aperture axis given by the mappedInflectorField.
*/
class simpleInflectorField : public G4MagneticField
{
  
public:
  simpleInflectorField(G4double);
  ~simpleInflectorField();

  void GetFieldValue( const double *Point,
		      double *Bfield ) const;
  
  G4double CalculateFieldValue(G4double, G4double, G4double) const;
  
private:
  G4String fieldMapFName;
  std::ifstream readInflectorFieldMap;
  
  std::vector<G4double> fieldPos;
  std::vector<G4double> fieldVal;  
  
  G4String inflectorMapFileName;
  G4double epsilon,delta,gamma,zeta;
  G4double apertureRadius,inflectorTotalLength;
  G4double fieldNormConst;
};


///////////////// mappedInflectorField

#include <string>

//FIXME: path not found
//#include "gm2ringsim/external_libs/libkdtree++/kdtree++/kdtree.hpp"
#include "field_point.hh"

#include "Geant4/G4ThreeVector.hh"

/** Provides a 3D field maps based approach to generating a field over
    the entire inflector aperture.

    This field was calculated by Wuzheng Meng at Brookhaven. */
class mappedInflectorField : public G4MagneticField {

public:
  mappedInflectorField();
  ~mappedInflectorField(){}

  void GetFieldValue( double const * Point, double *Bfield) const;

  void load_maps();

  std::string magnet_file() const { return magnet_file_; }
  std::string magnet_file(std::string name);

  std::string inflector_file() const { return inflector_file_; }
  std::string inflector_file(std::string name);


private:
  std::string magnet_file_;
  std::string inflector_file_;

  typedef field_point<G4ThreeVector,G4ThreeVector> data_point;
  typedef std::vector<data_point> data_vec;
  //FIXME
  //  typedef KDTree::KDTree<3, data_point> data_tree;

  // the coordinates of the storage field data in the magnet_tree_ are
  // the global coordinates of the simulation.
  //FIXME
  //data_tree magnet_tree_;
  void magnet_transform(data_point&);

  // the coordinates of the inflector field data in the
  // inflector_tree_ are a local coordinate system along the inflector
  // axis, because of the variable inflector positioning
  //FIXME
  //data_tree inflector_tree_;
  void inflector_transform(data_point &);

  void reflect_and_merge(data_vec&);

  data_vec parse_file(std::istream&, int exp_toks);

  G4ThreeVector shepard_interpolate(data_point const& dp, data_vec const& dv) const;

  G4double delta_, gamma_, zeta_;

};

/** An exception class.
    
    @bug Provide a real implementation.
*/
struct no_magnet_file {};

/** An exception class.
    
    @bug Provide a real implementation.
*/
struct no_inflector_file {};

#endif //INFLECTORFIELD_HH



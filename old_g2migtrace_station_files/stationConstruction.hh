#ifndef stationConstruction_hh
#define stationConstruction_hh

/** @file stationConstruction.hh

    Provides the interface to the detector station construction and
    setup.  A "detector station" is a realization of the 24
    hodoscope/calorimeter measurement stations.

    @author Kevin Lynch
    @date 2011
*/

#include <vector>
#include <tr1/array>
#include <tr1/memory>

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"

// forward declarations
class stationParameters;
class stationMessenger;
struct plate_t;
struct hodoscope_t;

/** Builds/rebuilds the detector stations.  Also esponsible for
    building the subcomponents of each station.  Individual modules
    can be controlled by the appropriate Messenger. */
class stationConstruction {
public:
  stationConstruction();
  ~stationConstruction();

  void ConstructStations(std::vector<G4VPhysicalVolume *> const &);

  void rebuildAll();
  void rebuildStation(G4int);

private:
  void buildStation(G4int);
  void destroyStation(G4int);
  void doRefresh();

  void build_plate(G4String, G4int, plate_t const&, G4double&);
  void build_hodo(G4String, G4int, hodoscope_t const&, G4double&);

  std::vector<G4VPhysicalVolume*> arcs_;

  std::tr1::array<G4VPhysicalVolume *, 24> station_P_;

  std::tr1::shared_ptr<stationParameters> params_;
  std::tr1::shared_ptr<stationMessenger> mess_;
};

#endif // stationConstruction_hh

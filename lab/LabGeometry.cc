// Geometry for the lab

#include "gm2ringsim/lab/LabGeometry.hh"
#include <iostream>

gm2ringsim::LabGeometry::LabGeometry(std::string const & detName) :
  GeometryBase(detName)
  lab_x(p.get<double>("lab_x")),
  lab_y(p.get<double>("lab_y")),
  lab_z(p.get<double>("lab_z"))
{}

void gm2ringsim::LabGeometry::print() const {
  // Do this later with message service
}


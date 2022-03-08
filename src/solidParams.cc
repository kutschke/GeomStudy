//
// Fill an std::vector of geometry parameters for certain
// simple G4 solids.  For other G4 solids, the parameter
// vector is left empty.
//
#include "GeomStudy/inc/solidParams.hh"
#include "GeomStudy/inc/SolidId.hh"

#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Polycone.hh"
#include "G4Torus.hh"
#include "G4Tubs.hh"

#include <iostream>

namespace {

  void doG4Box( G4VSolid const* vsolid,
                std::vector<double>& par ){
    G4Box const* box = static_cast<G4Box const *>(vsolid);
    par.reserve(3);
    par.emplace_back(box->GetXHalfLength());
    par.emplace_back(box->GetYHalfLength());
    par.emplace_back(box->GetZHalfLength());
  }

  void doG4Cons( G4VSolid const* vsolid,
                 std::vector<double>& par ){
    G4Cons const* cons = static_cast<G4Cons const *>(vsolid);
    par.reserve(7);
    par.emplace_back(cons->GetInnerRadiusMinusZ());
    par.emplace_back(cons->GetOuterRadiusMinusZ());
    par.emplace_back(cons->GetInnerRadiusPlusZ());
    par.emplace_back(cons->GetOuterRadiusPlusZ());
    par.emplace_back(cons->GetZHalfLength());
    par.emplace_back(cons->GetStartPhiAngle());
    par.emplace_back(cons->GetDeltaPhiAngle());
  }

  void doG4Torus( G4VSolid const* vsolid,
                std::vector<double>& par ){
    G4Torus const* torus = static_cast<G4Torus const *>(vsolid);
    par.reserve(5);
    par.emplace_back(torus->GetRmin());
    par.emplace_back(torus->GetRmax());
    par.emplace_back(torus->GetRtor());
    par.emplace_back(torus->GetSPhi());
    par.emplace_back(torus->GetDPhi());
  }

  void doG4Tubs( G4VSolid const* vsolid,
                std::vector<double>& par ){
    G4Tubs const* tubs = static_cast<G4Tubs const *>(vsolid);
    par.reserve(5);
    par.emplace_back(tubs->GetInnerRadius());
    par.emplace_back(tubs->GetOuterRadius());
    par.emplace_back(tubs->GetZHalfLength());
    par.emplace_back(tubs->GetStartPhiAngle());
    par.emplace_back(tubs->GetDeltaPhiAngle());
  }

  void doG4Polycone( G4VSolid const* vsolid,
                     std::vector<double>& par ){
    G4Polycone const* tmp = static_cast<G4Polycone const *>(vsolid);
    G4PolyconeHistorical const* pcon = tmp->GetOriginalParameters();
    par.reserve(3*pcon->Num_z_planes+2);
    par.emplace_back(tmp->GetStartPhi());
    par.emplace_back(tmp->GetEndPhi());
    double* z = pcon->Z_values;
    double* rmin = pcon->Rmin;
    double* rmax = pcon->Rmax;
    for ( int i=0; i<pcon->Num_z_planes; ++i ){
      par.emplace_back(z[i]);
      par.emplace_back(rmin[i]);
      par.emplace_back(rmax[i]);
    }
  }

}

std::vector<double> mu2e::solidParams( G4VSolid const* vsolid){

  // Intialize the return value.
  std::vector<double> pars;

  SolidId type(std::string(vsolid->GetEntityType()));

  switch (type){
  case SolidId::G4Box:
    doG4Box(vsolid,pars);
    break;
  case SolidId::G4Cons:
    doG4Cons(vsolid,pars);
    break;
  case SolidId::G4Torus:
    doG4Torus(vsolid,pars);
    break;
  case SolidId::G4Tubs:
    doG4Tubs(vsolid,pars);
    break;
  case SolidId::G4Polycone:
    doG4Polycone(vsolid,pars);
    break;
  default:
    //std::cout << "Skipped type: " << vsolid->GetEntityType() << " " << type.name() << " " << type.id() << std::endl;
    break;
  }

  return pars;
}

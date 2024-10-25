//
// Fill an std::vector of geometry parameters for certain
// simple G4 solids.  For other G4 solids, the parameter
// vector is left empty.
//
#include "GeomStudy/inc/solidParams.hh"
#include "GeomStudy/inc/SolidId.hh"

#include "G4BooleanSolid.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4ExtrudedSolid.hh"
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

  void doG4ExtrudedSolid( G4VSolid const* vsolid,
                          std::vector<double>& par ){
    G4ExtrudedSolid const* exts = static_cast<G4ExtrudedSolid const *>(vsolid);
    constexpr int sizeDouble  = sizeof(double);
    const int nVerts = exts->GetNofVertices();
    const int nZsecs = exts->GetNofZSections();

    const int nBytes = nVerts*sizeof(G4TwoVector) + nZsecs*sizeof(G4ExtrudedSolid::ZSection);
    const int check  = nBytes%sizeDouble;
    const int npars  = 2 + nBytes/sizeDouble;

    if ( check != 0 ){
      std::cout << "Fubar: " << nBytes << std::endl;
    }

    par.reserve(npars);
    par.emplace_back(nVerts);
    auto const verts = exts->GetPolygon();
    for ( auto const& v : verts ){
      par.emplace_back(v.x());
      par.emplace_back(v.y());
    }
    par.emplace_back(nZsecs);
    auto const zsecs = exts->GetZSections();
    for ( auto const& zs : zsecs ){
      par.emplace_back(zs.fZ);
      par.emplace_back(zs.fOffset.x());
      par.emplace_back(zs.fOffset.y());
      par.emplace_back(zs.fScale);
    }

  } // end doG4ExtrudedSolid

  void doG4BooleanSolid( G4VSolid const* vsolid,
                          std::vector<double>& par ){

    G4BooleanSolid const* bs = static_cast<G4BooleanSolid const *>(vsolid);
    //auto s0 = bs->GetConstituentSolid(0);
    auto s1  = bs->GetConstituentSolid(1);
    auto s1d = s1->GetDisplacedSolidPtr();

    // Fixme: clean up all commented out code in this function after verifiying that the code
    // for physical volumes is correct.
    /*

    std::cout << "doing boolean: "
              << bs->GetName() << " "
              << s0 << " "
              << s1 << "   | "
              << s1d
              << std::endl;
    std::cout << "              "
              << bs->GetConstituentSolid(0)->GetName() << " "
              << bs->GetConstituentSolid(1)->GetName()
              << std::endl;
    */
    if ( s1d != nullptr ){
      auto rot   = s1d->GetObjectRotation();
      auto trans = s1d->GetObjectTranslation();
      //std::cout <<  "              " << trans << std::endl;
      //std::cout <<  "              " << rot << std::endl;

    }
  } // end doG4BooleanSolid

} // end anonymous namespace

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
  case SolidId::G4ExtrudedSolid:
    doG4ExtrudedSolid(vsolid,pars);
    break;
  case SolidId::G4SubtractionSolid:
    doG4BooleanSolid(vsolid, pars);
    break;
  case SolidId::G4IntersectionSolid:
    doG4BooleanSolid(vsolid, pars);
    break;
  case SolidId::G4UnionSolid:
    doG4BooleanSolid(vsolid, pars);
    break;
  default:
    std::cout << "Skipped type: " << vsolid->GetEntityType() << " " << type.name() << " " << type.id() << std::endl;
    break;
  }

  return pars;
}

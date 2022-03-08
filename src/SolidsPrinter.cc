//
// Dump information about the G4SolidStore
//
// Original author Rob Kutschke
//

// Mu2e includes
#include "GeomStudy/inc/SolidId.hh"
#include "GeomStudy/inc/SolidsPrinter.hh"
#include "GeomStudy/inc/solidParams.hh"

// Framework includes
//#include "cetlib_except/exception.h"

// G4 includes
#include "G4SolidStore.hh"
#include "G4VSolid.hh"
//#include "G4Cons.hh"

using namespace std;

#include <ostream>
#include <iomanip>
#include <set>

namespace {

  struct pConePlane {
    double z;
    double rmin;
    double rmax;
    pConePlane( double az, double armin, double armax):
      z(az), rmin(armin), rmax(armax){
    }
    bool operator==(pConePlane const& a) const{
      return ( z==a.z && rmin==a.rmin && rmax==a.rmax);
    }
    bool operator<(pConePlane const& a) const{
      if ( z    < a.z    ) return true;
      if ( rmin < a.rmin ) return true;
      if ( rmax < a.rmax ) return true;
      return false;
    }
  };

  inline std::ostream& operator<<(std::ostream& ost,
                                  const pConePlane& p ){
    ost << p.z << " "
        << p.rmin << " "
        << p.rmax;
    return ost;
  }


  void pConeCheckz ( std::vector<double> const&  p, string const& name, ostream& out){
    int check = (p.size()-2)%3;
    out << "Check: " << check << endl;
    if ( check != 0 ) return;
    size_t nPlanes = (p.size()-2)/3;
    out << "nPlanes: " << nPlanes << endl;
    set<double> uniqueZ;
    set<pConePlane> uniquePlane;
    pConePlane last(0.,0.,0.);
    for ( size_t i=0; i<nPlanes; ++i ){
      //out << "   " << i << " " << p[i*3+2] << " " << p[i*3+3] << " " << p[i*3+4] << endl;
      uniqueZ.insert(p[i*3+2]);
      pConePlane pcp( p[i*3+2], p[i*3+3], p[i*3+4]);
      bool test=(pcp==last);
      out << "   " << i << " " << pcp << " | " << last << "  |  " << test << endl;
      uniquePlane.insert(pcp);
      last = pcp;
    }
    if ( uniquePlane.size() != nPlanes ){
      out << "Fail2: ";
    } else if ( uniqueZ.size() != nPlanes ){
      out << "Fail1: ";
    } else {
      out << "OK:    ";
    }
    out << "nPlanes: "        << setw(2) << nPlanes
        << "   UniqueZ: "     << setw(2) << uniqueZ.size()
        << "   UniquePlane: " << setw(2) << uniquePlane.size()
        << "    " << name
        << endl;
    for ( auto z : uniqueZ ){
      out << "  " << z << endl;
    }
}
}

mu2e::SolidsPrinter::SolidsPrinter( std::ostream& out ){

  G4SolidStore* sstore = G4SolidStore::GetInstance();

  out << "Physical volume store size: " << sstore->size() << endl;

  map<string,int> counts;

  int n=-1;
  for ( auto i=sstore->begin(); i!=sstore->end(); ++i){

    // Add volume to the map; it's an error if its already there.
    G4VSolid* solid = *i;

    SolidId type(std::string(solid->GetEntityType()));
    out << "Solid: "
        << setw(5) << ++n << " "
        << solid->GetName() << " "
        << solid->GetEntityType() << " "
        << type
        << endl;

    if ( type == SolidId::G4Polycone ){
      std::vector<double> pars = solidParams(solid);
      out << "  Polycone: ";
      for ( auto p : pars){
        out << " " << p;
      }
      out << endl;
      pConeCheckz( pars, string(solid->GetName()), out);
    }

    string name(solid->GetName());
    ++counts[name];

  }
  out << "Unique names: " << counts.size() << endl;

  int nDuplicate{0};
  int sumDuplicate{0};
  for ( auto i : counts ){
    if ( i.second > 1 ){
      out << "Duplicates: "
          << setw(4) << i.second << " "
          << i.first
          << endl;
      ++nDuplicate;
      sumDuplicate += i.second;
    }
  }
  out << "Check counts: " << sstore->size() - counts.size() - sumDuplicate + nDuplicate;

}

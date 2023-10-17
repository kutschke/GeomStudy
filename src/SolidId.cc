//
// Enum-matched-to-String class for the names of G4 solids.
//
// Contact person, Rob Kutschke
//

#include "GeomStudy/inc/SolidId.hh"

namespace mu2e {

  std::string const& SolidIdDetail::typeName() {
    static std::string type("SolidId");
    return type;
  }

  std::map<SolidIdDetail::enum_type,std::string> const& SolidIdDetail::names(){

    static std::map<enum_type,std::string> nam;

    if ( nam.empty() ){
      nam[unknown]             = "unknown";
      nam[G4Box]               = "G4Box";
      nam[G4Cons]              = "G4Cons";
      nam[G4ExtrudedSolid]     = "G4ExtrudedSolid";
      nam[G4IntersectionSolid] = "G4IntersectionSolid";
      nam[G4Polycone]          = "G4Polycone";
      nam[G4Polyhedra]         = "G4Polyhedra";
      nam[G4SubtractionSolid]  = "G4SubtractionSolid";
      nam[G4Torus]             = "G4Torus";
      nam[G4Tubs]              = "G4Tubs";
      nam[G4Trd]               = "G4Trd";
      nam[G4GenericTrap]       = "G4GenericTrap";
      nam[G4UnionSolid]        = "G4UnionSolid";
      nam[G4DisplacedSolid]    = "G4DisplacedSolid";
      nam[G4EllipticalTube]    = "G4EllipticalTube";
      nam[G4Trap]              = "G4Trap";
      nam[G4Sphere]            = "G4Sphere";
    }
    return nam;
  }

}

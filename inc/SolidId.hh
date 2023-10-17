#ifndef GeomStudy_SolidId_hh
#define GeomStudy_SolidId_hh
//
// Enum-matched-to-String class for the names of G4 solids.
//
// Contact person, Rob Kutschke
//

#include "Offline/GeneralUtilities/inc/EnumToStringSparse.hh"

namespace mu2e {

  class SolidIdDetail{
  public:

    // Retain the G4 style and use mixed case names.
    enum enum_type { unknown,         G4Box,               G4Cons,
                     G4ExtrudedSolid, G4IntersectionSolid, G4Polycone,
                     G4Polyhedra,     G4SubtractionSolid,  G4Torus,
                     G4Tubs,          G4Trd,               G4GenericTrap,
                     G4UnionSolid,    G4DisplacedSolid,    G4EllipticalTube,
                     G4Trap,          G4Sphere };

    static std::string const& typeName();

    static std::map<enum_type,std::string> const& names();

  };

  typedef EnumToStringSparse<SolidIdDetail> SolidId;
}

#endif

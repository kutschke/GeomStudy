#ifndef Mu2eG4_solidParams_hh
#define Mu2eG4_solidParams_hh

//
// Return an std::vector of geometry parameters for certain
// simple G4 solids.  For other G4 solids, the parameter
// vector is left empty.
//


#include "G4VSolid.hh"

#include <vector>

namespace mu2e {

  std::vector<double> solidParams( G4VSolid const* solid);

}

#endif /* Mu2eG4_solidParams_hh */

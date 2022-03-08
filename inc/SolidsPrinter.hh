#ifndef Mu2eG4_SolidsPrinter_hh
#define Mu2eG4_SolidsPrinter_hh
//
// Print info about G4 solids
//
// Original author Rob Kutschke
//

#include "GeomStudy/inc/SolidInfo.hh"

#include <iosfwd>
#include <vector>

namespace mu2e {

  class SolidsPrinter {

  public:

    SolidsPrinter( std::ostream& );
    // Accept compiler written d'tor, copy c'tor an assignment operator.

  private:

    std::vector<SolidInfo> info_;

  };

  //std::ostream& operator<<(std::ostream& ost,
  //                        const SolidsInfo& vol );

} // end namespace mu2e

#endif /* Mu2eG4_SolidsPrinter_hh */

#ifndef Mu2eG4_PhysicalVolumePrinter_hh
#define Mu2eG4_PhysicalVolumePrinter_hh
//
// Print info about physical volumes
//
// Original author Rob Kutschke
//

#include "GeomStudy/inc/PhysicalVolumeInfo2.hh"

#include <iosfwd>
#include <vector>

namespace mu2e {

  class PhysicalVolumePrinter {

  public:

    typedef PhysicalVolumeInfo2::key_type key_type;

    PhysicalVolumePrinter( std::ostream& vols,
                           std::ostream& heirarchy,
                           std::ostream& details );
    // Accept compiler written d'tor, copy c'tor an assignment operator.

    void beginRun( std::ostream& );

  private:

    std::vector<PhysicalVolumeInfo2> info_;
    std::vector<PhysicalVolumeInfo2> info2_;

  };

  std::ostream& operator<<(std::ostream& ost,
                           const PhysicalVolumeInfo2& vol );

} // end namespace mu2e

#endif /* Mu2eG4_PhysicalVolumePrinter_hh */


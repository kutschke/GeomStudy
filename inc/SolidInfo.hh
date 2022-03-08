#ifndef MCDataProducts_SolidInfo_hh
#define MCDataProducts_SolidInfo_hh

//
// Persistable information about a G4 Physical Volume.
//
// Original author Rob Kutschke
//
//

#include <iostream>
#include <string>
#include <vector>

namespace mu2e {

  struct SolidInfo {

    typedef std::vector<double> params_type;

    // This c'tor is required for ROOT.
    SolidInfo();

    SolidInfo( std::string const& name,
               std::string const& solidTypeName,
               params_type const& solidParams
               );

    // Accept compiler generated versions of the
    // destructor, copy constructor and the assignment
    // operator.

    // Accessors

    std::string const&           name()     const { return name_;     }
    std::string const&           solidTypeName()     const { return solidTypeName_;     }
    params_type const&           solidParams()       const { return solidParams_;       }

    // Modifiers

    static size_t lName;
    static size_t lSolidTypeName;

  private:
    std::string            name_;
    std::string            solidTypeName_;
    params_type            solidParams_;

  };

  std::ostream& operator<<(std::ostream& ost,
                           const SolidInfo& vol );
}

#endif /* MCDataProducts_SolidInfo_hh */

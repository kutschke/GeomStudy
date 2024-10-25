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
    // Accept compiler generated rule of 5 functions

    // Accessors
    std::string const&           name()              const { return name_;     }
    std::string const&           solidTypeName()     const { return solidTypeName_;     }
    params_type const&           solidParams()       const { return solidParams_;       }
    bool                         isBoolean()         const { return isBoolean_; }
    std::string const&           solidA() const;
    std::string const&           solidB() const;

    // Modifiers

    static size_t lName;
    static size_t lSolidTypeName;

  private:
    std::string            name_;
    std::string            solidTypeName_;
    std::string            solidA_;
    std::string            solidB_;
    params_type            solidParams_;
    bool                   isBoolean_ = false;

  };

  std::ostream& operator<<(std::ostream& ost,
                           const SolidInfo& vol );
}

#endif /* MCDataProducts_SolidInfo_hh */

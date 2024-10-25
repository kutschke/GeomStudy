#ifndef MCDataProducts_PhysicalVolumeInfo2_hh
#define MCDataProducts_PhysicalVolumeInfo2_hh

//
// Persistable information about a G4 Physical Volume.
//
// Original author Rob Kutschke
//
//

#include <iostream>
#include <string>
#include <vector>

#include "CLHEP/Vector/ThreeVector.h"
#include "CLHEP/Vector/Rotation.h"

namespace mu2e {

  struct PhysicalVolumeInfo2 {

    typedef int key_type;
    typedef std::vector<double> params_type;

    // This c'tor is required for ROOT.
    PhysicalVolumeInfo2();

    PhysicalVolumeInfo2( std::string const& name,
                         unsigned           copyNo,
                         unsigned           depth,
                         std::string const& materialName,
                         std::string const& motherName,
                         std::string const& solidTypeName,
                         params_type const& solidParams,
                         double             mass,
                         double             volume,
                         CLHEP::Hep3Vector const&  objectTranslation,
                         CLHEP::HepRotation const& objectRotation,
                         key_type                  parent = -1,
                         std::string const& solidAName="",
                         std::string const& solidBName=""
                         );

    // Accept compiler generated rule of 5 functions

    // Accessors

    std::string const&           name()     const { return name_;     }
    unsigned                     copyNo()   const { return copyNo_;   }
    unsigned                     count()    const { return count_;    }
    unsigned                     depth()    const { return depth_;    }
    key_type                     parent()   const { return parent_;   }
    std::vector<key_type> const& children() const { return children_; }
    std::string const&           materialName()      const { return materialName_;      }
    std::string const&           motherName()        const { return motherName_;        }
    std::string const&           solidTypeName()     const { return solidTypeName_;     }
    params_type const&           solidParams()       const { return solidParams_;       }
    double                       mass()              const { return mass_;              }
    double                       volume()            const { return volume_;            }
    CLHEP::Hep3Vector const&     objectTranslation() const { return objectTranslation_; }
    CLHEP::HepRotation const&    objectRotation()    const { return objectRotation_;    }
    std::string const&           solidAName()        const { return solidA_;            }
    std::string const&           solidBName()        const { return solidB_;            }
    bool                         isBoolean()         const { return isBoolean_;         }

    // Modifiers
    void addChild ( key_type childIndex );
    void incrementCount() { ++count_;}

    static size_t lName;
    static size_t lMaterialName;
    static size_t lMotherName;
    static size_t lSolidTypeName;
    static unsigned    maxDepth;
    static unsigned    maxCount;
    static unsigned    maxCopyNo;
    static key_type    maxParentIndex;

  private:
    std::string            name_;
    unsigned               depth_;
    unsigned               count_          = 0;
    unsigned               copyNo_         = 0;
    std::string            materialName_;
    std::string            motherName_;
    std::string            solidTypeName_;
    key_type               parent_         = 0;
    params_type            solidParams_;
    double                 mass_;
    double                 volume_;
    CLHEP::Hep3Vector      objectTranslation_;
    CLHEP::HepRotation     objectRotation_;
    std::vector<key_type>  children_;
    std::string            solidA_;     // only valid for Boolean solids
    std::string            solidB_;
    bool                   isBoolean_ = false;

  };

  std::ostream& operator<<(std::ostream& ost,
                           const PhysicalVolumeInfo2& vol );
}

#endif /* MCDataProducts_PhysicalVolumeInfo2_hh */

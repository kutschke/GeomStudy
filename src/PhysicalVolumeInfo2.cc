#include "GeomStudy/inc/PhysicalVolumeInfo2.hh"

#include <iomanip>

using namespace std;

mu2e::PhysicalVolumeInfo2::PhysicalVolumeInfo2()
  : name_("NoSuchVolume")
  , depth_(0)
  , count_(0)
  , copyNo_(0)
  , materialName_("Undefined")
  , motherName_("Undefined")
  , solidTypeName_("Undefined")
  , parent_(-1)
  , solidParams_()
  , mass_()
  , volume_()
  , objectTranslation_()
  , objectRotation_()
  , children_()
{}

mu2e::PhysicalVolumeInfo2::PhysicalVolumeInfo2( std::string const& name,
                                                unsigned           depth,
                                                unsigned           copyNo,
                                                std::string const& materialName,
                                                std::string const& motherName,
                                                std::string const& solidTypeName,
                                                params_type const& solidParams,
                                                double             mass,
                                                double             volume,
                                                CLHEP::Hep3Vector  const& objectTranslation,
                                                CLHEP::HepRotation const& objectRotation,
                                                key_type                  parent)
  : name_(name)
  , depth_(depth)
  , count_(1)
  , copyNo_(copyNo)
  , materialName_(materialName)
  , motherName_(motherName)
  , solidTypeName_(solidTypeName)
  , parent_(parent)
  , solidParams_(solidParams)
  , mass_(mass)
  , volume_(volume)
  , objectTranslation_(objectTranslation)
  , objectRotation_(objectRotation)
{
  lName          = std::max( lName,          name_.size()           );
  lMaterialName  = std::max( lMaterialName,  materialName_.size()   );
  lMotherName    = std::max( lMotherName,    motherName_.size()     );
  lSolidTypeName = std::max( lSolidTypeName, solidTypeName_.size() );
  maxDepth       = std::max( maxDepth,       depth_ );
  maxCount       = std::max( maxCount,       count_ );
  maxCopyNo      = std::max( maxCopyNo,      copyNo_ );
  maxParentIndex = std::max( maxParentIndex, parent_ );
}

void mu2e::PhysicalVolumeInfo2::addChild ( key_type childIndex ){
  children_.push_back(childIndex);
}

size_t mu2e::PhysicalVolumeInfo2::lName = 0;
size_t mu2e::PhysicalVolumeInfo2::lMaterialName = 0;
size_t mu2e::PhysicalVolumeInfo2::lMotherName = 0;
size_t mu2e::PhysicalVolumeInfo2::lSolidTypeName = 0;
unsigned mu2e::PhysicalVolumeInfo2::maxDepth = 0;
unsigned mu2e::PhysicalVolumeInfo2::maxCount = 0;
unsigned mu2e::PhysicalVolumeInfo2::maxCopyNo = 0;
mu2e::PhysicalVolumeInfo2::key_type mu2e::PhysicalVolumeInfo2::maxParentIndex = 0;

std::ostream& mu2e::operator<<(std::ostream& ost,
                               const mu2e::PhysicalVolumeInfo2& vol ){

  std::string rotated = ( vol.objectRotation().isIdentity() ) ? " nonRot" : "Rotate";
  int head  = (vol.depth()-1)*2+1;
  int tail  = (mu2e::PhysicalVolumeInfo2::maxDepth-vol.depth())*2+1;
  ost << setw(head) << " "
      << setw(2)    << vol.depth()
      << setw(tail) << " "
      << setw(mu2e::PhysicalVolumeInfo2::lName+2) << vol.name()              << " "
      << setw(6)                                 << vol.copyNo()            << " "
      << setw(6)                                 << vol.count()             << " "
      << setw(7)                                 << vol.parent()            << " "
      << setw(5)                                 << vol.children().size()   << " "
      << setw(mu2e::PhysicalVolumeInfo2::lMaterialName+2)  << vol.materialName()      << " "
      << setw(mu2e::PhysicalVolumeInfo2::lSolidTypeName+2) << vol.solidTypeName()     << " ";
  /*
      << setw(mu2e::PhysicalVolumeInfo2::lMotherName+2)    << vol.motherName()        << " "
      << vol.objectTranslation() << " "
      << rotated;
  if ( !vol.solidParams().empty() ){
    ost << " (";
  }
  bool first=true;
  for ( auto const& p : vol.solidParams() ){
    if ( ! first ) {
      ost << ",";
    }
    first = false;
    ost << p;
  }
  if ( !vol.solidParams().empty() ){
    ost << ")";
  }
    << " [";
    for ( auto child : vol.children() ){
    ost << " " << child;
    }
    ost << " ] )";
  */
  return ost;
}

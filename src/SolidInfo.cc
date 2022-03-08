#include "GeomStudy/inc/SolidInfo.hh"

#include <iomanip>

using namespace std;

mu2e::SolidInfo::SolidInfo()
  : name_("NoSuchVolume")
  , solidTypeName_("Undefined")
  , solidParams_()
{}

mu2e::SolidInfo::SolidInfo( std::string const& name,
                            std::string const& solidTypeName,
                            params_type const& solidParams )
  : name_(name)
  , solidTypeName_(solidTypeName)
  , solidParams_(solidParams)
{
  lName          = std::max( lName,          name_.size()           );
  lSolidTypeName = std::max( lSolidTypeName, solidTypeName_.size() );
}


size_t mu2e::SolidInfo::lName = 0;
size_t mu2e::SolidInfo::lSolidTypeName = 0;

std::ostream& mu2e::operator<<(std::ostream& ost,
                               const mu2e::SolidInfo& vol ){

  int head{1};
  ost << setw(head)                                                      << " "
      << setw(mu2e::SolidInfo::lName+2)          << vol.name()           << " "
      << setw(mu2e::SolidInfo::lSolidTypeName+2) << vol.solidTypeName()  << " ";
  return ost;
}

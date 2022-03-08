//
// A utility class to do indexolgy related to persistence of
// physical volume information.
//
//
// Original author Rob Kutschke
//
// Notes:
// 1) This should be instantiated after the G4 initialization phase is complete.
//    ( After  _runManager->BeamOnBeginRun(); ).
//    At this time the information in PhysicalVolumeStore is static until
//    the end of the run; in particular, the addresses of all physical
//    volumes are valid until the end of the run.
//

// Mu2e includes
#include "GeomStudy/inc/PhysicalVolumePrinter.hh"
#include "GeomStudy/inc/solidParams.hh"
#include "Offline/Mu2eG4/inc/DuplicateLogicalVolumeChecker.hh"

// Framework includes
#include "cetlib_except/exception.h"

#include "CLHEP/Vector/Rotation.h"

// G4 includes
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4VSolid.hh"
#include "G4Cons.hh"

using namespace std;

#include <ostream>
#include <iomanip>
#include <set>

namespace {

  // Find the physical volume identified by the name and copyNo in the collection.
  int find ( std::string const& name, unsigned copyNo, std::vector<mu2e::PhysicalVolumeInfo2>& pInfo ){
    for ( size_t i=0; i<pInfo.size(); ++i ){
      if ( copyNo == pInfo[i].copyNo() ) {
        if ( name  == pInfo[i].name() ){
          return i;
        }
      }
    }
    return -1;
  }

  class Descender{
  public:
    Descender( G4VPhysicalVolume const* v,
                int depth,
                ostream& out,
               std::vector<mu2e::PhysicalVolumeInfo2>& pInfo,
               int motherIndex );

  private:
    int depth_;
  };

  Descender::Descender( G4VPhysicalVolume const* pv,
                        int      depth,
                        ostream& out,
                        std::vector<mu2e::PhysicalVolumeInfo2>& pInfo,
                        int motherIndex ):
    depth_(depth+1){
    G4LogicalVolume const* lv = pv->GetLogicalVolume();

    string const& motherName = (pv->GetMotherLogical() ) ?
      pv->GetMotherLogical()->GetName() : "Top";

    pInfo.emplace_back( pv->GetName(),
                        depth_,
                        pv->GetCopyNo(),
                        pv->GetLogicalVolume()->GetMaterial()->GetName(),
                        motherName,
                        pv->GetLogicalVolume()->GetSolid()->GetEntityType(),
                        mu2e::solidParams(pv->GetLogicalVolume()->GetSolid()),
                        pv->GetLogicalVolume()->GetMass()/CLHEP::gram,
                        pv->GetLogicalVolume()->GetSolid()->GetCubicVolume()/CLHEP::cm3,
                        pv->GetObjectTranslation(),
                        *pv->GetObjectRotation(),
                        motherIndex
                        );
    int myIndex = pInfo.size()-1;
    if ( motherIndex > -1 ){
      pInfo.at(motherIndex).addChild(myIndex);
    }

    for ( size_t i=0; i<lv->GetNoDaughters(); ++i){
      auto physical = lv->GetDaughter(i);
      int index = find(physical->GetName(), physical->GetCopyNo(), pInfo);
      if ( index == -1 ){
        Descender next( physical, depth_, out, pInfo, myIndex);
      } else{
        pInfo[index].incrementCount();
      }
    }
  }

}


namespace {
  struct PInfo{
    PInfo( std::string aname, int acopyNo ):
      name(aname), copyNo(acopyNo){
    }

    bool operator < ( PInfo const& rhs) const {
      if ( name < rhs.name   ) {
        return true;
      } else if ( name == rhs.name ) {
        if ( copyNo < rhs.copyNo ) {
          return true;
        }
      }
      return false;
    }

    bool operator == ( PInfo const& rhs) const {
      if ( name  == rhs.name && copyNo == rhs.copyNo ) return true;
      return false;
    }

    std::string name;
    int copyNo;
  };
}

// Returns true if all physical volume names are unique.
// The verbose flag only controls the behaviour when all names are unique.
// If there are non-unique names there is unconditional printout.
bool checkDuplicatePhysicals( ostream& out, bool verbose ){

  G4PhysicalVolumeStore* pstore = G4PhysicalVolumeStore::GetInstance();

  std::map<PInfo,int> pinfos;

  for ( auto i : *pstore ){
    ++pinfos[PInfo(i->GetName(),i->GetCopyNo())];
  }

  if ( pstore->size() == pinfos.size()){
    if ( verbose ) {
      out << "All physical volumes are unique (name,copyNo)." << endl;
    }
    return true;
  }

  // There are duplicate physical volume (name,copyNo); print information.
  out << "Number of physical volumes: "        << pstore->size()
      << "  Number of unique (name,copyNo): "  << pinfos.size()
      << endl;
  for ( auto const& i : pinfos ){
    int n = i.second;
    if ( n > 1 ){
      out << "Duplicate Physical Volume id: "
          << setw(6) << n << " "
          << i.first.name << " "
          << i.first.copyNo
          << endl;
    }
  }

  return false;
}

// Returns true if all physical volume names are unique.
void listPhysicalsWithMultipleCopies( ostream& out ){

  G4PhysicalVolumeStore* pstore = G4PhysicalVolumeStore::GetInstance();

  std::map<std::string,int> pinfos;

  for ( auto i : *pstore ){
    ++pinfos[i->GetName()];
  }

  for ( auto const& i : pinfos ){
    std::string const& name{i.first};
    int n{i.second};
    if ( n > 1 ){
      out << "Physical Volume with multiple copies: : "
          << setw(6) << n << " "
          << name
          << endl;
    }
  }
}

mu2e::PhysicalVolumePrinter::PhysicalVolumePrinter( std::ostream& out,
                                                    std::ostream& hierarchy,
                                                    std::ostream& details ){

  G4PhysicalVolumeStore* pstore = G4PhysicalVolumeStore::GetInstance();
  G4LogicalVolumeStore*  vstore = G4LogicalVolumeStore::GetInstance();

  info2_.clear();
  info2_.reserve( pstore->size() );

  out << "Physical volume store size: " << pstore->size() << endl;
  out << "Logical volume store size:  " << vstore->size() << endl;

  std::map<G4LogicalVolume*,int> count;
  int n(0);
  for ( auto i : *vstore ){
    count[i] = 0;
  }
  out << "Count size: " << count.size()  << endl;

  DuplicateLogicalVolumeChecker dups;
  dups.print(out);

  checkDuplicatePhysicals( out, vstore );
  listPhysicalsWithMultipleCopies( out);

  std::map<G4LogicalVolume const*, std::vector<G4VPhysicalVolume const*>> l2p;

  int nPhys(0);
  for ( auto i=pstore->begin(); i!=pstore->end(); ++i){

    // Add volume to the map; it's an error if its already there.
    G4VPhysicalVolume* vpv = *i;
    info2_.emplace_back( vpv->GetName(),
                         0,
                         vpv->GetCopyNo(),
                         vpv->GetLogicalVolume()->GetMaterial()->GetName(),
                         vpv->GetLogicalVolume()->GetName(),
                         vpv->GetLogicalVolume()->GetSolid()->GetEntityType(),
                         mu2e::solidParams(vpv->GetLogicalVolume()->GetSolid()),
                         vpv->GetLogicalVolume()->GetMass()/CLHEP::gram,
                         vpv->GetLogicalVolume()->GetSolid()->GetCubicVolume()/CLHEP::cm3,
                         vpv->GetObjectTranslation(),
                         *vpv->GetObjectRotation()
                         );

    out << "Physical: "
        << ++nPhys << " "
        << vpv->GetName() << " "
        << vpv->GetCopyNo() << " "
        << vpv->GetLogicalVolume()->GetName() << " "
        << vpv->GetLogicalVolume()->GetNoDaughters() << " "
        << vpv->GetLogicalVolume()->GetMaterial()->GetName() << " "
        << vpv->GetLogicalVolume()->GetSolid()->GetEntityType()
        << endl;

    ++count[vpv->GetLogicalVolume()];

    l2p[vpv->GetLogicalVolume()].push_back(vpv);

  }
  out << "l2p size:   " << l2p.size() << endl;

  int sum(0);
  int sum0(0);
  int sum1(0);
  int sumN(0);
  int sumDau(0);
  for ( auto const& i : count ){
    int nPhysical = i.second;
    out << "Logical: " << n++ << " "
         << i.first->GetName() << " "
         << nPhysical << " "
         << i.first->GetNoDaughters()
         << endl;
    sum += nPhysical;
    sumDau += i.first->GetNoDaughters();
    if ( nPhysical == 0 ){
      ++sum0;
    } else if ( nPhysical == 1){
      ++sum1;
    } else{
      ++sumN;
    }
  }
  out << "Sizes: " << sum << " : " << sum0 << " " << sum1 << " "  << sumN << " | " << sumDau << endl;

  auto pworld = pstore->GetVolume("World");

  info_.clear();
  info_.reserve( pstore->size() );

  Descender pw(pworld,0,out, info_, -1);
  out << "info_.size() " << info_.size() << endl;

  int nn{0};
  for ( PhysicalVolumeInfo2 const& i : info_ ){
    hierarchy << setw(6) << nn++ << " " << i << endl;
  }

  int nprops(0);
  for ( auto const& vol : info_ ){
    if ( nprops > 0 ) details << endl;
    details << nprops++     << " "
          << vol.name()   << " "
          << vol.copyNo() << " "
          << vol.materialName()
          << "  mother: " << vol.motherName()
          << endl;
    details << "   Translation: " << vol.objectTranslation() << endl;
    if ( vol.objectRotation().isIdentity() ){
      details << "   No Rotation" << endl;
    } else {
      details << "   Rotation: " << vol.objectRotation();
    }
    if ( vol.solidParams().empty() ){
      details << "   Shape: " << vol.solidTypeName()
            << "   Shape Parameters are not available." << endl;
    } else{
      details << "   " << vol.solidTypeName() << ":  ";
      for ( auto const& p : vol.solidParams() ){
        details << " " << p;
      }
      details << endl;
    }
    details << "   Volume: " << vol.volume() << endl;
    details << "   Mass:   " << vol.mass()   << endl;
  }

  int sumps{0};
  int nZero{0};
  int nOne{0};
  int nMulti{0};
  for ( auto i : l2p ){
    auto l = i.first;
    auto ps = i.second;
    sumps += ps.size();
    if ( ps.size() == 0 ){
      ++nZero;
    } else if ( ps.size() == 1 ){
      ++nOne;
    } else if ( ps.size() > 1 ){
      ++nMulti;
      out << "Multi: "
          << setw(6) << ps.size()    << " "
          << l->GetName() << " "
          << endl;
    }
  }
  out << "Zero, One, Multi: "
      << nZero << " "
      << nOne << " "
      << nMulti << " | "
      << nZero+nOne+nMulti
      << endl;
  out << "Sum of p.size: " << sumps << endl;

  for ( auto const& i : info_ ){
    if ( i.count() > 1 ){
      out << "Multi2: "
          << setw(6) << i.count()   << " "
          << i.name() << " "
          << endl;
    }
  }

  int index{-1};
  CLHEP::Hep3Vector inWorld;
  for ( auto const& i : info_ ){
    if ( i.name() == "TrackerMother" ){
      out << "Ready: " << i << endl;
      index = i.parent();
      inWorld = i.objectTranslation();
      out << "Is identity: "
          << i.objectRotation().isIdentity()
          << endl;
      break;
    }
  }
  int safe(0);
  while (index > -1 && ++safe < 11 ){
    out << "Index: "
         << index << " "
         << info_[index]
         << endl;
    inWorld += info_[index].objectTranslation();
    index = info_[index].parent();
  }
  out << "In world: "<< inWorld << endl;
  out << "&IDENTIY:  " << &CLHEP::HepRotation::IDENTITY << endl;

}

// Build _persistentInfo and _volumeMap from G4PhysicalVolumeStore.
void mu2e::PhysicalVolumePrinter::beginRun( std::ostream& out){
}

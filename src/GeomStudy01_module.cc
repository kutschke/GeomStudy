//
// An analyzer module that prints information about the geometry.
//
// Original author Rob Kutschke
//
//

// Mu2e includes
#include "GeomStudy/inc/PhysicalVolumePrinter.hh"
#include "GeomStudy/inc/SolidsPrinter.hh"

// From art and its tool chain.
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "fhiclcpp/ParameterSet.h"
#include "canvas/Utilities/InputTag.h"

// C++ includes.
#include <iostream>
#include <fstream>

using namespace std;

namespace mu2e {

  class GeomStudy01 : public art::EDAnalyzer {

  public:
    GeomStudy01(fhicl::ParameterSet const& pSet);

    virtual void analyze(art::Event const& event ) override;
    virtual void beginRun(art::Run const & run   ) override;

  private:

    std::string volumesFilename_;
    std::string detailsFilename_;
    std::string hierarchyFilename_;
    std::string solidsFilename_;

  }; // end GeomStudy01 header

} // end namespace mu2e

mu2e::GeomStudy01::GeomStudy01( fhicl::ParameterSet const& pSet):
  EDAnalyzer(pSet),
  volumesFilename_(pSet.get<std::string>("volumesFilename")),
  detailsFilename_(pSet.get<std::string>("detailsFilename")),
  hierarchyFilename_(pSet.get<std::string>("hierarchyFilename")),
  solidsFilename_(pSet.get<std::string>("solidsFilename")){
}

void
mu2e::GeomStudy01::analyze(art::Event const& ){
}

void
mu2e::GeomStudy01::beginRun(art::Run const &){
  ofstream out(volumesFilename_.c_str());
  ofstream details(detailsFilename_.c_str());
  ofstream hierarchy(hierarchyFilename_.c_str());
  PhysicalVolumePrinter printer(out,hierarchy,details);
  printer.beginRun(out);

  ofstream solids(solidsFilename_.c_str());
  SolidsPrinter solidsPrinter( solids );
}

DEFINE_ART_MODULE(mu2e::GeomStudy01);

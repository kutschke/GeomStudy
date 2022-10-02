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
    struct Config {
      fhicl::Atom<std::string> summaryFilename {
        fhicl::Name("summaryFilename"),
          fhicl::Comment("Filename for the summary output file.")
          };

      fhicl::Atom<std::string> detailsFilename {
        fhicl::Name("detailsFilename"),
          fhicl::Comment("Filename for the details output file.")
          };

      fhicl::Atom<std::string> hierarchyFilename {
        fhicl::Name("hierarchyFilename"),
          fhicl::Comment("Filename for the hierarchy output file.")
          };

      fhicl::Atom<std::string> solidsFilename {
        fhicl::Name("solidsFilename"),
          fhicl::Comment("Filename for the solids output file.")
          };
    };
    using Parameters = art::EDAnalyzer::Table<Config>;

    GeomStudy01( Parameters const& conf );

    virtual void analyze(art::Event const& event ) override;
    virtual void beginRun(art::Run const & run   ) override;

  private:

    std::string summaryFilename_;
    std::string detailsFilename_;
    std::string hierarchyFilename_;
    std::string solidsFilename_;

  }; // end GeomStudy01 header

} // end namespace mu2e

mu2e::GeomStudy01::GeomStudy01( Parameters const& conf ):
  EDAnalyzer(conf),
  summaryFilename_(conf().summaryFilename()),
  detailsFilename_(conf().detailsFilename()),
  hierarchyFilename_(conf().hierarchyFilename()),
  solidsFilename_(conf().solidsFilename()){
}

void
mu2e::GeomStudy01::analyze(art::Event const& ){
}

void
mu2e::GeomStudy01::beginRun(art::Run const &){

  ofstream summary(summaryFilename_.c_str());
  ofstream details(detailsFilename_.c_str());
  ofstream hierarchy(hierarchyFilename_.c_str());
  PhysicalVolumePrinter printer(summary,hierarchy,details);
  printer.beginRun(summary);

  ofstream solids(solidsFilename_.c_str());
  SolidsPrinter solidsPrinter( solids );
}

DEFINE_ART_MODULE(mu2e::GeomStudy01);

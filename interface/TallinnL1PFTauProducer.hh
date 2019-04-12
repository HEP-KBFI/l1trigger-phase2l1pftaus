#ifndef TallinnL1PFTauProducer_H
#define TallinnL1PFTauProducer_H

#include <iostream>
#include <math.h>
#include <vector>
#include <list>
#include <TLorentzVector.h>

////////////////////
// FRAMEWORK HEADERS
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/Ref.h"

//L1 TPG Legacy
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "CondFormats/L1TObjects/interface/L1CaloHcalScale.h"
#include "CondFormats/DataRecord/interface/L1CaloHcalScaleRcd.h"
#include "DataFormats/HcalDigi/interface/HcalTriggerPrimitiveDigi.h"

#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/EcalAlgo/interface/EcalBarrelGeometry.h"
#include "Geometry/EcalAlgo/interface/EcalEndcapGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"

//Geometry
#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/HcalTowerAlgo/interface/HcalTrigTowerGeometry.h"
#include "Geometry/HcalTowerAlgo/interface/HcalGeometry.h"

#include "DataFormats/TallinnL1PFTaus/interface/TallinnL1PFTau.h"
#include "DataFormats/Phase2L1ParticleFlow/interface/PFCandidate.h"
#include "L1Trigger/TallinnL1PFTaus/interface/TallinnTauCluster.h"

using namespace l1t;

//#define tracker_eta 3.5
//#define tau_size_eta 0.7
//#define tau_size_phi 0.7

using std::vector;
using namespace l1t;

class TallinnL1PFTauProducer : public edm::EDProducer {
   public:
  explicit TallinnL1PFTauProducer(const edm::ParameterSet&);

  ~TallinnL1PFTauProducer();

   private:
  //virtual void produce(edm::Event&, const edm::EventSetup&) override;

  TallinnTauClusterCollection tauCandidates;
  
  void createTau(TallinnTauClusterCollection &inputCollection, l1t::PFCandidate tauSeed);
  void tau_cand_sort(TallinnTauClusterCollection tauCandidates, std::unique_ptr<TallinnL1PFTauCollection> &newTallinnL1PFTauCollection, unsigned int nCands);

  /// ///////////////// ///
  /// MANDATORY METHODS ///
  virtual void beginRun( const edm::Run& run, const edm::EventSetup& iSetup );
  virtual void endRun( const edm::Run& run, const edm::EventSetup& iSetup );
  virtual void produce( edm::Event& iEvent, const edm::EventSetup& iSetup );
  /// ///////////////// ///

  bool debug;
  double deltaR_;
  double min_tauSeedPt_;
  double max_tauSeedEta_;
  edm::EDGetTokenT< vector<l1t::PFCandidate> > L1PFToken_;

};


#endif

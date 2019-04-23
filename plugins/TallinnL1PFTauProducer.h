#ifndef L1Trigger_TallinnL1PFTaus_TallinnL1PFTauProducer_h
#define L1Trigger_TallinnL1PFTaus_TallinnL1PFTauProducer_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"

#include "L1Trigger/TallinnL1PFTaus/interface/TallinnL1PFTauQualityCut.h"
#include "L1Trigger/TallinnL1PFTaus/interface/TallinnL1PFTauBuilder.h"
#include "DataFormats/TallinnL1PFTaus/interface/TallinnL1PFTau.h"
#include "DataFormats/Phase2L1ParticleFlow/interface/PFCandidate.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include <vector>

class TallinnL1PFTauProducer : public edm::EDProducer 
{
 public:
  explicit TallinnL1PFTauProducer(const edm::ParameterSet& cfg);
  ~TallinnL1PFTauProducer();

 private:
  void produce(edm::Event& evt, const edm::EventSetup& es);

  TallinnL1PFTauBuilder tauBuilder_;

  edm::EDGetTokenT<std::vector<l1t::PFCandidate>> l1PFCandToken_;
  edm::EDGetTokenT<std::vector<reco::Vertex>> vtxTagToken_;

  std::vector<TallinnL1PFTauQualityCut> signalQualityCuts_;
  std::vector<TallinnL1PFTauQualityCut> isolationQualityCuts_;

  double min_tauSeed_pt_;
  double max_tauSeed_eta_;
  double max_tauSeed_dz_;

  double deltaR_cleaning_;
  double deltaR2_cleaning_;

  bool debug_;
};

#endif

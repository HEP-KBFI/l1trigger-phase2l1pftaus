#ifndef L1Trigger_TallinnL1PFTaus_TallinnL1PFTauProducer_h
#define L1Trigger_TallinnL1PFTaus_TallinnL1PFTauProducer_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include <FWCore/Framework/interface/Frameworkfwd.h>
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "DataFormats/Phase2L1ParticleFlow/interface/PFCandidate.h"
#include "L1Trigger/TallinnL1PFTaus/interface/TallinnL1PFTauBuilder.h"
#include "DataFormats/TallinnL1PFTaus/interface/TallinnL1PFTau.h"

class TallinnL1PFTauProducer : public edm::EDProducer 
{
 public:

  explicit TallinnL1PFTauProducer(const edm::ParameterSet& cfg);
  ~TallinnL1PFTauProducer();

 private:

  void produce(edm::Event& evt, const edm::EventSetup& es);

  bool debug_;
  double deltaR_;
  double deltaR2_;
  double dz_cut_;
  double min_tauSeedPt_;
  double max_tauSeedEta_;

  edm::EDGetTokenT<std::vector<l1t::PFCandidate>> l1PFCandToken_;
  edm::EDGetTokenT<std::vector<reco::Vertex>> vtxTagToken_;
};


#endif

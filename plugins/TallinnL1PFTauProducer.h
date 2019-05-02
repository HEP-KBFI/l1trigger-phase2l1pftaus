#ifndef L1Trigger_TallinnL1PFTaus_TallinnL1PFTauProducer_h
#define L1Trigger_TallinnL1PFTaus_TallinnL1PFTauProducer_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"

#include "L1Trigger/TallinnL1PFTaus/interface/TallinnL1PFTauQualityCut.h" // TallinnL1PFTauQualityCut
#include "L1Trigger/TallinnL1PFTaus/interface/TallinnL1PFTauBuilder.h"    // TallinnL1PFTauBuilder
#include "DataFormats/TallinnL1PFTaus/interface/TallinnL1PFTau.h"         // l1t::TallinnL1PFTau
#include "DataFormats/TallinnL1PFTaus/interface/TallinnL1PFTauFwd.h"      // l1t::TallinnL1PFTauCollection
#include "DataFormats/Phase2L1ParticleFlow/interface/PFCandidate.h"       // l1t::PFCandidate
#include "DataFormats/Phase2L1ParticleFlow/interface/PFCandidateFwd.h"    // l1t::PFCandidateCollection, l1t::PFCandidateRef
#include "DataFormats/JetReco/interface/PFJet.h"                          // reco::PFJet
#include "DataFormats/JetReco/interface/PFJetCollection.h"                // reco::PFJetCollection, reco::PFJetRef
#include "DataFormats/VertexReco/interface/Vertex.h"                      // reco::Vertex
#include "DataFormats/VertexReco/interface/VertexFwd.h"                   // reco::VertexCollection

#include <vector>

class TallinnL1PFTauProducer : public edm::EDProducer 
{
 public:
  explicit TallinnL1PFTauProducer(const edm::ParameterSet& cfg);
  ~TallinnL1PFTauProducer();

 private:
  void produce(edm::Event& evt, const edm::EventSetup& es);

  TallinnL1PFTauBuilder tauBuilder_;

  edm::EDGetTokenT<l1t::PFCandidateCollection> l1PFCandToken_;
  edm::EDGetTokenT<reco::PFJetCollection> l1PFJetToken_;
  edm::EDGetTokenT<reco::VertexCollection> vtxTagToken_;

  std::vector<TallinnL1PFTauQualityCut> signalQualityCuts_;
  std::vector<TallinnL1PFTauQualityCut> isolationQualityCuts_;

  bool useChargedPFCandSeeds_;
  double min_seedChargedPFCand_pt_;
  double max_seedChargedPFCand_eta_;
  double max_seedChargedPFCand_dz_;

  bool usePFJetSeeds_;
  double min_seedPFJet_pt_;
  double max_seedPFJet_eta_;

  double min_PFTau_pt_;
  double max_PFTau_eta_;
  double min_leadChargedPFCand_pt_;
  double max_leadChargedPFCand_eta_;
  double max_leadChargedPFCand_dz_;
  double max_chargedIso_;
  double max_chargedRelIso_;

  double deltaR_cleaning_;
  double deltaR2_cleaning_;

  bool debug_;
};

#endif

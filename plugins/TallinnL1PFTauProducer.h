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
#include "DataFormats/Phase2L1ParticleFlow/interface/PFJet.h"             // l1t::PFJet
#include "DataFormats/Phase2L1ParticleFlow/interface/PFJetFwd.h"          // l1t::PFJetCollection, l1t::PFJetRef
#include "DataFormats/L1TVertex/interface/Vertex.h"                       // l1t::Vertex
#include "DataFormats/L1TVertex/interface/VertexFwd.h"                    // l1t::VertexCollection

#include <string>
#include <vector>

class TallinnL1PFTauProducer : public edm::EDProducer 
{
 public:
  explicit TallinnL1PFTauProducer(const edm::ParameterSet& cfg);
  ~TallinnL1PFTauProducer();

 private:
  void produce(edm::Event& evt, const edm::EventSetup& es);

  std::string moduleLabel_;

  TallinnL1PFTauBuilder* tauBuilder_;
  
  edm::InputTag srcL1PFCands_;
  edm::EDGetTokenT<l1t::PFCandidateCollection> tokenL1PFCands_;
  edm::InputTag srcL1PFJets_;
  edm::EDGetTokenT<l1t::PFJetCollection> tokenL1PFJets_;
  edm::InputTag srcL1Vertices_;
  edm::EDGetTokenT<l1t::VertexCollection> tokenL1Vertices_;
  edm::InputTag srcRho_;
  edm::EDGetTokenT<double> tokenRho_;

  std::vector<TallinnL1PFTauQualityCut> signalQualityCuts_dzCut_disabled_;
  std::vector<TallinnL1PFTauQualityCut> isolationQualityCuts_dzCut_disabled_;

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

  bool applyPreselection_;

  bool debug_;
};

#endif

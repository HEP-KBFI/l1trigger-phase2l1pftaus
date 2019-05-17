#ifndef L1Trigger_TallinnL1PFTaus_TallinnL1PFTauBuilder_h
#define L1Trigger_TallinnL1PFTaus_TallinnL1PFTauBuilder_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"                   // edm::ParameterSet
#include "DataFormats/Provenance/interface/ProductID.h"                   // edm::ProductID

#include "L1Trigger/TallinnL1PFTaus/interface/TallinnL1PFTauQualityCut.h" // TallinnL1PFTauQualityCut
#include "DataFormats/TallinnL1PFTaus/interface/TallinnL1PFTau.h"         // l1t::TallinnL1PFTau
#include "DataFormats/Phase2L1ParticleFlow/interface/PFCandidate.h"       // l1t::PFCandidate
#include "DataFormats/Phase2L1ParticleFlow/interface/PFCandidateFwd.h"    // l1t::PFCandidateCollection, l1t::PFCandidateRef
#include "DataFormats/JetReco/interface/PFJet.h"                          // reco::PFJet
#include "DataFormats/JetReco/interface/PFJetCollection.h"                // reco::PFJetCollection, reco::PFJetRef
#include "DataFormats/L1TVertex/interface/Vertex.h"                       // l1t::VertexRef

#include <TFormula.h> // TFormula

#include <vector>

class TallinnL1PFTauBuilder
{
 public:
  TallinnL1PFTauBuilder(const edm::ParameterSet& cfg);
  ~TallinnL1PFTauBuilder();
  
  void reset();
  void setL1PFCandProductID(const edm::ProductID& l1PFCandProductID);
  void setVertex(const l1t::VertexRef& primaryVertex);
  void setL1PFTauSeed(const l1t::PFCandidateRef& l1PFCand_seed);
  void setL1PFTauSeed(const reco::PFJetRef& l1PFJet_seed);
  void addL1PFCandidates(const std::vector<l1t::PFCandidateRef>& l1PFCands);
  void buildL1PFTau();

  l1t::TallinnL1PFTau getL1PFTau() const { return l1PFTau_; }

 private:
  l1t::PFCandidateRefVector convertToRefVector(const std::vector<l1t::PFCandidateRef>& l1PFCands);

  bool isWithinSignalCone(const l1t::PFCandidate& l1PFCand);
  bool isWithinStrip(const l1t::PFCandidate& l1PFCand);
  bool isWithinIsolationCone(const l1t::PFCandidate& l1PFCand);

  TFormula* signalConeSizeFormula_;
  static int signalConeSizeFormula_instance_counter_;
  double signalConeSize_;
  double signalConeSize2_;
  double min_signalConeSize_;
  double max_signalConeSize_;

  bool useStrips_;
  double stripSize_eta_;
  double stripSize_phi_;

  double isolationConeSize_;
  double isolationConeSize2_;

  std::vector<TallinnL1PFTauQualityCut> signalQualityCuts_dzCut_disabled_;
  std::vector<TallinnL1PFTauQualityCut> signalQualityCuts_dzCut_enabled_primary_;
  std::vector<TallinnL1PFTauQualityCut> isolationQualityCuts_dzCut_disabled_;
  std::vector<TallinnL1PFTauQualityCut> isolationQualityCuts_dzCut_enabled_primary_;
  std::vector<TallinnL1PFTauQualityCut> isolationQualityCuts_dzCut_enabled_pileup_;

  edm::ProductID l1PFCandProductID_;
  bool isPFCandSeeded_;
  l1t::PFCandidateRef l1PFCand_seed_;
  bool isPFJetSeeded_;
  reco::PFJetRef l1PFJet_seed_;
  double l1PFTauSeed_eta_;
  double l1PFTauSeed_phi_;
  double l1PFTauSeed_zVtx_;
  l1t::VertexRef primaryVertex_;
  l1t::TallinnL1PFTau l1PFTau_;

  reco::Particle::LorentzVector strip_p4_;

  std::vector<l1t::PFCandidateRef> signalAllL1PFCandidates_;
  std::vector<l1t::PFCandidateRef> signalChargedHadrons_;
  std::vector<l1t::PFCandidateRef> signalElectrons_;
  std::vector<l1t::PFCandidateRef> signalNeutralHadrons_;
  std::vector<l1t::PFCandidateRef> signalPhotons_;
  std::vector<l1t::PFCandidateRef> signalMuons_;
  
  std::vector<l1t::PFCandidateRef> isoAllL1PFCandidates_;
  std::vector<l1t::PFCandidateRef> isoChargedHadrons_;
  std::vector<l1t::PFCandidateRef> isoElectrons_;
  std::vector<l1t::PFCandidateRef> isoNeutralHadrons_;
  std::vector<l1t::PFCandidateRef> isoPhotons_;
  std::vector<l1t::PFCandidateRef> isoMuons_;
  
  std::vector<l1t::PFCandidateRef> sumAllL1PFCandidates_;
  std::vector<l1t::PFCandidateRef> sumChargedHadrons_;
  std::vector<l1t::PFCandidateRef> sumElectrons_;
  std::vector<l1t::PFCandidateRef> sumNeutralHadrons_;
  std::vector<l1t::PFCandidateRef> sumPhotons_;
  std::vector<l1t::PFCandidateRef> sumMuons_;

  double sumChargedIsoPileup_;

  bool debug_;
};

#endif

#ifndef L1Trigger_TallinnL1PFTaus_TallinnL1PFTauBuilder_h
#define L1Trigger_TallinnL1PFTaus_TallinnL1PFTauBuilder_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "L1Trigger/TallinnL1PFTaus/interface/TallinnL1PFTauQualityCut.h"
#include "DataFormats/TallinnL1PFTaus/interface/TallinnL1PFTau.h"
#include "DataFormats/Phase2L1ParticleFlow/interface/PFCandidate.h"

#include <vector>

class TallinnL1PFTauBuilder
{
 public:
  TallinnL1PFTauBuilder(const edm::ParameterSet& cfg);
  ~TallinnL1PFTauBuilder();
  
  void reset();
  void setL1PFTauSeed_and_Vertex(const l1t::PFCandidate& l1PFCand_seed, const reco::Vertex* primaryVertex);
  void addL1PFCandidates(const l1t::PFCandidateCollection& l1PFCands);
  void buildL1PFTau();

  TallinnL1PFTau getL1PFTau() const { return l1PFTau_; }

 private:

  bool isWithinSignalCone(const l1t::PFCandidate& l1PFCand);
  bool isWithinStrip(const l1t::PFCandidate& l1PFCand);
  bool isWithinIsolationCone(const l1t::PFCandidate& l1PFCand);

  double signalConeSize_;
  double signalConeSize2_;
  double min_signalConeSize_;
  double max_signalConeSize_;

  double stripSize_eta_;
  double stripSize_phi_;

  double isolationConeSize_;
  double isolationConeSize2_;

  std::vector<TallinnL1PFTauQualityCut> signalQualityCuts_;
  std::vector<TallinnL1PFTauQualityCut> isolationQualityCuts_;

  l1t::PFCandidate l1PFTauSeed_;
  double l1PFTauSeed_eta_;
  double l1PFTauSeed_phi_;
  const reco::Vertex* primaryVertex_;
  TallinnL1PFTau l1PFTau_;

  reco::Particle::LorentzVector strip_p4_;

  l1t::PFCandidateCollection signalAllL1PFCandidates_;
  l1t::PFCandidateCollection signalChargedHadrons_;
  l1t::PFCandidateCollection signalElectrons_;
  l1t::PFCandidateCollection signalNeutralHadrons_;
  l1t::PFCandidateCollection signalPhotons_;
  l1t::PFCandidateCollection signalMuons_;
  
  l1t::PFCandidateCollection isoAllL1PFCandidates_;
  l1t::PFCandidateCollection isoChargedHadrons_;
  l1t::PFCandidateCollection isoElectrons_;
  l1t::PFCandidateCollection isoNeutralHadrons_;
  l1t::PFCandidateCollection isoPhotons_;
  l1t::PFCandidateCollection isoMuons_;
  
  l1t::PFCandidateCollection sumAllL1PFCandidates_;
  l1t::PFCandidateCollection sumChargedHadrons_;
  l1t::PFCandidateCollection sumElectrons_;
  l1t::PFCandidateCollection sumNeutralHadrons_;
  l1t::PFCandidateCollection sumPhotons_;
  l1t::PFCandidateCollection sumMuons_;
};

#endif

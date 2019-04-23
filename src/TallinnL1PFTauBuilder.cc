#include "L1Trigger/TallinnL1PFTaus/interface/TallinnL1PFTauBuilder.h"

#include "L1Trigger/TallinnL1PFTaus/interface/TallinnL1PFTauQualityCut.h"

#include <algorithm> // std::max(), std::sort()

TallinnL1PFTauBuilder::TallinnL1PFTauBuilder(const edm::ParameterSet& cfg)
  : min_signalConeSize_(cfg.getParameter<double>("min_signalConeSize"))
  , max_signalConeSize_(cfg.getParameter<double>("max_signalConeSize"))
  , stripSize_eta_(cfg.getParameter<double>("stripSize_eta"))
  , stripSize_phi_(cfg.getParameter<double>("stripSize_phi"))
  , isolationConeSize_(cfg.getParameter<double>("isolationConeSize"))
  , primaryVertex_(nullptr)
{
  assert(max_signalConeSize_ >= min_signalConeSize_);

  isolationConeSize2_ = isolationConeSize_*isolationConeSize_;

  edm::ParameterSet cfg_signalQualityCuts = cfg.getParameter<edm::ParameterSet>("signalQualityCuts");
  signalQualityCuts_ = readL1PFTauQualityCuts(cfg_signalQualityCuts);
  edm::ParameterSet cfg_isolationQualityCuts = cfg.getParameter<edm::ParameterSet>("isolationQualityCuts");
  isolationQualityCuts_ = readL1PFTauQualityCuts(cfg_isolationQualityCuts);
}

TallinnL1PFTauBuilder::~TallinnL1PFTauBuilder()
{}

void TallinnL1PFTauBuilder::reset()
{
  signalConeSize_ = 0.;
  signalConeSize2_ = 0.;

  l1PFTauSeed_eta_ = 0.;
  l1PFTauSeed_phi_ = 0.;
  primaryVertex_ = nullptr;
  l1PFTau_ = TallinnL1PFTau();

  strip_p4_ = reco::Particle::LorentzVector(0.,0.,0.,0.);

  signalAllL1PFCandidates_.clear();
  signalChargedHadrons_.clear();
  signalElectrons_.clear();
  signalNeutralHadrons_.clear();
  signalPhotons_.clear();
  signalMuons_.clear();
  
  isoAllL1PFCandidates_.clear();
  isoChargedHadrons_.clear();
  isoElectrons_.clear();
  isoNeutralHadrons_.clear();
  isoPhotons_.clear();
  isoMuons_.clear();
  
  sumAllL1PFCandidates_.clear();
  sumChargedHadrons_.clear();
  sumElectrons_.clear();
  sumNeutralHadrons_.clear();
  sumPhotons_.clear();
  sumMuons_.clear();
}

void TallinnL1PFTauBuilder::setL1PFTauSeed_and_Vertex(const l1t::PFCandidate& l1PFCand_seed, const reco::Vertex* primaryVertex)
{
  l1PFTauSeed_ = l1PFCand_seed;
  l1PFTauSeed_eta_ = l1PFTauSeed_.eta();
  l1PFTauSeed_phi_ = l1PFTauSeed_.phi();
  primaryVertex_ = primaryVertex;
}

void TallinnL1PFTauBuilder::addL1PFCandidates(const l1t::PFCandidateCollection& l1PFCands)
{
  for ( auto l1PFCand : l1PFCands ) 
  {
    if( !isWithinIsolationCone(l1PFCand) )
      continue;
    sumAllL1PFCandidates_.push_back(l1PFCand);
    if ( l1PFCand.id() == l1t::PFCandidate::ChargedHadron ) 
    {
      sumChargedHadrons_.push_back(l1PFCand);
    }
    else if ( l1PFCand.id() == l1t::PFCandidate::Electron ) 
    {
      sumElectrons_.push_back(l1PFCand);
    }
    else if ( l1PFCand.id() == l1t::PFCandidate::NeutralHadron ) 
    {
      sumNeutralHadrons_.push_back(l1PFCand);
    }
    else if ( l1PFCand.id() == l1t::PFCandidate::Photon ) 
    {
      sumPhotons_.push_back(l1PFCand);
    }
    else if ( l1PFCand.id() == l1t::PFCandidate::Muon ) 
    {
      sumMuons_.push_back(l1PFCand);
    }
  }

  double sumAllL1PFCandidates_pt = 0.;
  for ( auto l1PFCand : sumAllL1PFCandidates_ )
  {
    sumAllL1PFCandidates_pt += l1PFCand.pt();
  }
  signalConeSize_ = 3.0/std::max(1., sumAllL1PFCandidates_pt);
  if ( signalConeSize_ < min_signalConeSize_ ) signalConeSize_ = min_signalConeSize_;
  if ( signalConeSize_ > max_signalConeSize_ ) signalConeSize_ = max_signalConeSize_;
  signalConeSize2_ = signalConeSize_*signalConeSize_;

  for ( auto l1PFCand : sumAllL1PFCandidates_ ) 
  {
    bool isSignalPFCand = false;
    bool isElectron_or_Photon = l1PFCand.id() == l1t::PFCandidate::Electron || l1PFCand.id() == l1t::PFCandidate::Photon;
    bool isChargedHadron = l1PFCand.id() == l1t::PFCandidate::ChargedHadron;
    if ( (isWithinSignalCone(l1PFCand) || (isElectron_or_Photon && isWithinStrip(l1PFCand))) && !(isChargedHadron && signalChargedHadrons_.size() > 3) ) 
    { 
      isSignalPFCand = true;
    }

    if ( isSignalPFCand && isSelected(signalQualityCuts_, l1PFCand, primaryVertex_) )
    {
      signalAllL1PFCandidates_.push_back(l1PFCand);  
      if ( l1PFCand.id() == l1t::PFCandidate::ChargedHadron ) 
      {
	signalChargedHadrons_.push_back(l1PFCand);
      }
      else if ( l1PFCand.id() == l1t::PFCandidate::Electron ) 
      {
	signalElectrons_.push_back(l1PFCand);
	strip_p4_ += l1PFCand.p4();
      }
      else if ( l1PFCand.id() == l1t::PFCandidate::NeutralHadron ) 
      {
	signalNeutralHadrons_.push_back(l1PFCand);
      }
      else if ( l1PFCand.id() == l1t::PFCandidate::Photon ) 
      {
	signalPhotons_.push_back(l1PFCand);
	strip_p4_ += l1PFCand.p4();
      }
      else if ( l1PFCand.id() == l1t::PFCandidate::Muon ) 
      {
	signalMuons_.push_back(l1PFCand);
      }
    } 

    if ( isSelected(isolationQualityCuts_, l1PFCand, primaryVertex_) && !isSignalPFCand )
    {
      isoAllL1PFCandidates_.push_back(l1PFCand);
      if ( l1PFCand.id() == l1t::PFCandidate::ChargedHadron ) 
      {
	isoChargedHadrons_.push_back(l1PFCand);
      }
      else if ( l1PFCand.id() == l1t::PFCandidate::Electron )
      {
	isoElectrons_.push_back(l1PFCand);
      }
      else if ( l1PFCand.id() == l1t::PFCandidate::NeutralHadron ) 
      {
	isoNeutralHadrons_.push_back(l1PFCand);
      }
      else if ( l1PFCand.id() == l1t::PFCandidate::Photon )
      {
	isoPhotons_.push_back(l1PFCand);
      }
      else if ( l1PFCand.id() == l1t::PFCandidate::Muon )
      {
	isoMuons_.push_back(l1PFCand);
      }
    }
  }
}

bool TallinnL1PFTauBuilder::isWithinSignalCone(const l1t::PFCandidate& l1PFCand)
{
  double deltaEta = l1PFCand.eta() - l1PFTauSeed_eta_;
  double deltaPhi = l1PFCand.phi() - l1PFTauSeed_phi_;
  if ( (deltaEta*deltaEta + deltaPhi*deltaPhi) < signalConeSize2_ ) return true;
  else return false;
}

bool TallinnL1PFTauBuilder::isWithinStrip(const l1t::PFCandidate& l1PFCand)
{
  double deltaEta = l1PFCand.eta() - l1PFTauSeed_eta_;
  double deltaPhi = l1PFCand.phi() - l1PFTauSeed_phi_;
  if ( deltaEta < stripSize_eta_ && deltaPhi < stripSize_phi_ ) return true;
  else return false;
}

bool TallinnL1PFTauBuilder::isWithinIsolationCone(const l1t::PFCandidate& l1PFCand)
{
  double deltaEta = l1PFCand.eta() - l1PFTauSeed_eta_;
  double deltaPhi = l1PFCand.phi() - l1PFTauSeed_phi_;
  if ( (deltaEta*deltaEta + deltaPhi*deltaPhi) < isolationConeSize2_ ) return true;
  else return false;
}

void TallinnL1PFTauBuilder::buildL1PFTau()
{
  reco::Particle::LorentzVector l1PFTau_p4;
  for ( auto l1PFCand : signalAllL1PFCandidates_ )
  {
    l1PFTau_p4 += l1PFCand.p4();
  }
  l1PFTau_.setP4(l1PFTau_p4);

  l1PFTau_.seedChargedPFCand_ = l1PFTauSeed_;
  if ( signalChargedHadrons_.size() >= 1 )
  {
    l1PFTau_.leadChargedPFCand_ = signalChargedHadrons_[0];
  }

  l1PFTau_.strip_p4_ = strip_p4_;

  double sumChargedIso = 0.;
  double sumNeutralIso = 0.;
  for ( auto l1PFCand : isoAllL1PFCandidates_ )
  {
    if ( l1PFCand.charge() != 0 ) 
    { 
      sumChargedIso += l1PFCand.pt();
    }
    else if ( l1PFCand.id() == l1t::PFCandidate::Photon ) 
    { 
      sumNeutralIso += l1PFCand.pt();
    }
  }
  l1PFTau_.sumChargedIso_ = sumChargedIso;
  l1PFTau_.sumNeutralIso_ = sumNeutralIso;
  const double weightNeutralIso = 1.;
  const double offsetNeutralIso = 0.;
  l1PFTau_.sumCombinedIso_ = sumChargedIso + weightNeutralIso*(sumNeutralIso - offsetNeutralIso);

  if ( l1PFTau_.sumChargedIso() < 50. ) 
  {
    l1PFTau_.passVLooseIso_ = true;
  }
  if ( l1PFTau_.sumChargedIso() < 20. ) 
  {
    l1PFTau_.passLooseIso_ = true;
  }
  if ( l1PFTau_.sumChargedIso() < 10. ) 
  {
    l1PFTau_.passMediumIso_ = true;
  }
  if ( l1PFTau_.sumChargedIso() <  5. ) 
  {
    l1PFTau_.passTightIso_ = true;
  }
}



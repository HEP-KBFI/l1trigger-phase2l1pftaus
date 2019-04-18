#include "L1Trigger/TallinnL1PFTaus/interface/TallinnL1PFTauBuilder.h"

#include <algorithm> // std::max()

TallinnL1PFTauBuilder::TallinnL1PFTauBuilder()
  : min_signalConeSize_(0.05)
  , max_signalConeSize_(0.10)
  , stripSize_eta_(0.05)
  , stripSize_phi_(0.20)
  , isolationConeSize_(0.4)
{
  assert(max_signalConeSize_ >= min_signalConeSize_);
  isolationConeSize2_ = isolationConeSize_*isolationConeSize_;
}

TallinnL1PFTauBuilder::~TallinnL1PFTauBuilder()
{}

void TallinnL1PFTauBuilder::setL1PFTauSeed(const l1t::PFCandidate& l1PFCand_seed)
{
  l1PFTauSeed_ = l1PFCand_seed;
  l1PFTauSeed_eta_ = l1PFTauSeed_.eta();
  l1PFTauSeed_phi_ = l1PFTauSeed_.phi();
}

void TallinnL1PFTauBuilder::addL1PFCandidates(const l1t::PFCandidateCollection& l1PFCands)
{
  for ( auto l1PFCand : l1PFCands ) {
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
  for(auto l1PFCand : sumAllL1PFCandidates_){
    sumAllL1PFCandidates_pt += l1PFCand.pt();
  }
  signalConeSize_ = 3.0/std::max(1., sumAllL1PFCandidates_pt);
  if ( signalConeSize_ < min_signalConeSize_ ) signalConeSize_ = min_signalConeSize_;
  if ( signalConeSize_ > max_signalConeSize_ ) signalConeSize_ = max_signalConeSize_;
  signalConeSize2_ = signalConeSize_*signalConeSize_;

  for ( auto l1PFCand : sumAllL1PFCandidates_ ) 
  {
    bool isElectron_or_Photon = l1PFCand.id() == l1t::PFCandidate::Electron || l1PFCand.id() == l1t::PFCandidate::Photon;
    if ( isWithinSignalCone(l1PFCand) || (isElectron_or_Photon && isWithinStrip(l1PFCand)) ) 
    {
      signalAllL1PFCandidates_.push_back(l1PFCand);      
      if ( l1PFCand.id() == l1t::PFCandidate::ChargedHadron ) 
      {
	signalChargedHadrons_.push_back(l1PFCand);
      }
      else if ( l1PFCand.id() == l1t::PFCandidate::Electron ) 
      {
	signalElectrons_.push_back(l1PFCand);
      }
      else if ( l1PFCand.id() == l1t::PFCandidate::NeutralHadron ) 
      {
	signalNeutralHadrons_.push_back(l1PFCand);
      }
      else if ( l1PFCand.id() == l1t::PFCandidate::Photon ) 
      {
	signalPhotons_.push_back(l1PFCand);
      }
      else if ( l1PFCand.id() == l1t::PFCandidate::Muon ) 
      {
	signalMuons_.push_back(l1PFCand);
      }
    }
    else 
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
  math::XYZTLorentzVector l1PFTau_p4;
  for ( auto l1PFCand : signalAllL1PFCandidates_ )
  {
    l1PFTau_p4 += l1PFCand.p4();
  }
  l1PFTau_.setP4(l1PFTau_p4);

  double sumChargedIso = 0.;
  for ( auto l1PFCand : isoAllL1PFCandidates_ )
  {
    if ( l1PFCand.charge() != 0 ) 
    { 
      sumChargedIso += l1PFCand.pt();
    }
  }
  l1PFTau_.setChargedIso(sumChargedIso);

  if ( l1PFTau_.chargedIso() < 50. ) 
  {
    l1PFTau_.setPassVLooseIso(true);
  }
  if ( l1PFTau_.chargedIso() < 20. ) 
  {
    l1PFTau_.setPassLooseIso(true);
  }
  if ( l1PFTau_.chargedIso() < 10. ) 
  {
    l1PFTau_.setPassMediumIso(true);
  }
  if ( l1PFTau_.chargedIso() < 5. ) 
  {
    l1PFTau_.setPassTightIso(true);
  }
}



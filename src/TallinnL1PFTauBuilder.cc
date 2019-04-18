#include "L1Trigger/TallinnL1PFTaus/interface/TallinnL1PFTauBuilder.h"

void TallinnL1PFTauBuilder::addL1PFCandidates( l1t::PFCandidateCollection  l1PFCands ){
  for(auto l1PFCand : l1PFCands){
    if(l1PFCand==l1PFTauSeed_)
      continue;
    if(!isolationConeContains(l1PFCand))
      continue;
    sumAllL1PFCandidates_.push_back(l1PFCand);
    if(l1PFCand.id() == l1t::PFCandidate::ChargedHadron){
      sumChargedHadrons_.push_back(l1PFCand);
    }
    else if(l1PFCand.id() == l1t::PFCandidate::Electron){
      sumElectrons_.push_back(l1PFCand);
    }
    else if(l1PFCand.id() == l1t::PFCandidate::NeutralHadron){
      sumNeutralHadrons_.push_back(l1PFCand);
    }
    else if(l1PFCand.id() == l1t::PFCandidate::Photon){
      sumPhotons_.push_back(l1PFCand);
    }
    else if(l1PFCand.id() == l1t::PFCandidate::Muon){
      sumMuons_.push_back(l1PFCand);
    }
  }
  for(auto l1PFCand : l1PFCands){
    if(l1PFCand==l1PFTauSeed_)
      continue;
    if(!isolationConeContains(l1PFCand))
      continue;
    if(signalConeContains(l1PFCand)){
      signalAllL1PFCandidates_.push_back(l1PFCand);
      
      if(l1PFCand.id() == l1t::PFCandidate::ChargedHadron){
	signalChargedHadrons_.push_back(l1PFCand);
      }
      else if(l1PFCand.id() == l1t::PFCandidate::Electron){
	signalElectrons_.push_back(l1PFCand);
      }
      else if(l1PFCand.id() == l1t::PFCandidate::NeutralHadron){
	signalNeutralHadrons_.push_back(l1PFCand);
      }
      else if(l1PFCand.id() == l1t::PFCandidate::Photon){
	signalPhotons_.push_back(l1PFCand);
      }
      else if(l1PFCand.id() == l1t::PFCandidate::Muon){
	signalMuons_.push_back(l1PFCand);
      }
    }
    else{
      sumChargedIso_ += l1PFCand.pt();

      isoAllL1PFCandidates_.push_back(l1PFCand);
      if(l1PFCand.id() == l1t::PFCandidate::ChargedHadron){
        isoChargedHadrons_.push_back(l1PFCand);
      }
      else if(l1PFCand.id() == l1t::PFCandidate::Electron){
        isoElectrons_.push_back(l1PFCand);
      }
      else if(l1PFCand.id() == l1t::PFCandidate::NeutralHadron){
        isoNeutralHadrons_.push_back(l1PFCand);
      }
      else if(l1PFCand.id() == l1t::PFCandidate::Photon){
        isoPhotons_.push_back(l1PFCand);
      }
      else if(l1PFCand.id() == l1t::PFCandidate::Muon){
        isoMuons_.push_back(l1PFCand);
      }
    }
  }
}

bool TallinnL1PFTauBuilder::isolationConeContains( l1t::PFCandidate l1PFCand ){
  float l1PFCand_pt = l1PFCand.pt();
  float l1PFCand_eta = l1PFCand.eta();
  float l1PFCand_phi = l1PFCand.phi();

  float l1PFTauSeed_pt = l1PFTauSeed_.pt();
  float l1PFTauSeed_eta = l1PFTauSeed_.eta();
  float l1PFTauSeed_phi = l1PFTauSeed_.phi();

  float deltaR = 0.4;

  if(fabs(l1PFCand_eta-l1PFTauSeed_eta)+fabs(l1PFCand_phi-l1PFTauSeed_phi) < deltaR)
    return true;

  return false;
}

bool TallinnL1PFTauBuilder::signalConeContains( l1t::PFCandidate l1PFCand ){
  //float l1PFCand_pt = l1PFCand.pt();
  float l1PFCand_eta = l1PFCand.eta();
  float l1PFCand_phi = l1PFCand.phi();

  float l1PFTauSeed_pt = l1PFTauSeed_.pt();
  float l1PFTauSeed_eta = l1PFTauSeed_.eta();
  float l1PFTauSeed_phi = l1PFTauSeed_.phi();

  float l1PFCand_all_pt = 0;
  for(auto l1PFCand : sumAllL1PFCandidates_){
    l1PFCand_all_pt = l1PFCand_all_pt + l1PFCand.pt();
  }

  float l1PFCand_sum_pt = l1PFTauSeed_pt + l1PFCand_all_pt;

  float deltaR = 3/l1PFCand_sum_pt;
  if(l1PFCand_sum_pt>60){
    deltaR = 0.05;
  }
  if(l1PFCand_sum_pt<30){
    deltaR = 0.10;
  }
  if(fabs(l1PFCand_eta-l1PFTauSeed_eta)+fabs(l1PFCand_phi-l1PFTauSeed_phi) < deltaR)
    return true;
                                                                       
  return false;
}



void TallinnL1PFTauBuilder::buildL1PFTau(){

  float l1PFTauSeed_pt = l1PFTauSeed_.pt();
  float l1PFTauSeed_eta = l1PFTauSeed_.eta();
  float l1PFTauSeed_phi = l1PFTauSeed_.phi();
  math::PtEtaPhiMLorentzVector l1PFTauSeed_p4(l1PFTauSeed_pt, l1PFTauSeed_eta, l1PFTauSeed_phi, l1PFTauSeed_pt);

  float l1PFCand_pt = 0;
  float l1PFCand_eta = 0;
  float l1PFCand_phi = 0;
  math::PtEtaPhiMLorentzVector l1PFTau_p4(l1PFCand_pt, l1PFCand_eta, l1PFCand_phi, l1PFCand_pt);
  for(auto l1PFCand : signalAllL1PFCandidates_){
    float l1PFCand_pt = l1PFCand.pt();
    float l1PFCand_eta = l1PFCand.eta();
    float l1PFCand_phi = l1PFCand.phi();
    math::PtEtaPhiMLorentzVector tempP4(l1PFCand_pt, l1PFCand_eta, l1PFCand_phi, l1PFCand_pt);
    l1PFTau_p4 = l1PFTau_p4 + tempP4;
  }
  l1PFTau_p4 = l1PFTau_p4 + l1PFTauSeed_p4;

  l1PFTau_.setP4(l1PFTau_p4);

  l1PFTau_.setChargedIso(sumChargedIso_);


  if(l1PFTau_.chargedIso() < 50){
    l1PFTau_.setPassVLooseIso(true);
  }
  if(l1PFTau_.chargedIso() < 20){
    l1PFTau_.setPassLooseIso(true);
  }
  if(l1PFTau_.chargedIso() < 10){
    l1PFTau_.setPassMediumIso(true);
  }
  if(l1PFTau_.chargedIso() < 5){
    l1PFTau_.setPassTightIso(true);
  }
}



TallinnL1PFTauBuilder::TallinnL1PFTauBuilder() {};
TallinnL1PFTauBuilder::~TallinnL1PFTauBuilder() {};

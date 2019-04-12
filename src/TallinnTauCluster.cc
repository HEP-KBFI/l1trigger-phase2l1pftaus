#include "L1Trigger/TallinnL1PFTaus/interface/TallinnTauCluster.h"


void TallinnTauCluster::addPFCandidate( l1t::PFCandidate l1PFCand ){

  if(tauSeedConeContains(l1PFCand)){
    AllL1PFCandidates.push_back(l1PFCand);

    if(l1PFCand.id() == l1t::PFCandidate::ChargedHadron){
      ChargedHadrons.push_back(l1PFCand);
    }
    else if(l1PFCand.id() == l1t::PFCandidate::Electron){
    Electrons.push_back(l1PFCand);
    }
    else if(l1PFCand.id() == l1t::PFCandidate::NeutralHadron){
      NeutralHadrons.push_back(l1PFCand);
    }
    else if(l1PFCand.id() == l1t::PFCandidate::Photon){
      Photons.push_back(l1PFCand);
    }
    else if(l1PFCand.id() == l1t::PFCandidate::Muon){
    Muons.push_back(l1PFCand);
    }
  }
  else{
    sumChargedIso += l1PFCand.pt();
  }
}


bool TallinnTauCluster::tauSeedConeContains( l1t::PFCandidate l1PFCand ){

  float l1PFCand_pt = l1PFCand.pt();
  float l1PFCand_eta = l1PFCand.eta();
  float l1PFCand_phi = l1PFCand.phi();

  //float tauSeed_pt = tauSeed.pt();
  float tauSeed_eta = tauSeed.eta();
  float tauSeed_phi = tauSeed.phi();

  float deltaR = 0.2;
  if(l1PFCand_pt>3){
    deltaR = 0.18;
  }
  if(l1PFCand_pt>5){
    deltaR = 0.17;
  }
  if(l1PFCand_pt>10){
    deltaR = 0.1;
  }
  if(l1PFCand_pt>18){
    deltaR = 0.06;
  }
  if(l1PFCand_pt>25){
    deltaR = 0.03;
  }

  if(fabs(l1PFCand_eta-tauSeed_eta)+fabs(l1PFCand_phi-tauSeed_phi) < deltaR)
    return true;
                                                                       
  return false;

}



void TallinnTauCluster::process(){

  float pt = 0;
  float eta = 0;
  float phi = 0;
  int nl1PFCand = AllL1PFCandidates.size();
  for(int iCand=0; iCand<nl1PFCand; iCand++){
    pt = pt + AllL1PFCandidates.at(iCand).pt();
  }
  pt = pt + tauSeed.pt();
  eta = tauSeed.eta();
  phi = tauSeed.phi();
  
  math::PtEtaPhiMLorentzVector tempP4(pt,eta,phi,pt);
  l1PFTau.setP4(tempP4);

  l1PFTau.setChargedIso(sumChargedIso);

  
  if(l1PFTau.chargedIso() < 50){
    l1PFTau.setPassVLooseIso(true);
  }
  if(l1PFTau.chargedIso() < 20){
    l1PFTau.setPassLooseIso(true);
  }
  if(l1PFTau.chargedIso() < 10){
    l1PFTau.setPassMediumIso(true);
  }
  if(l1PFTau.chargedIso() < 5){
    l1PFTau.setPassTightIso(true);
  }
  

}

float TallinnTauCluster::delta_r_cluster(simple_object_t cluster_1, simple_object_t cluster_2){
  float delta_r = 20;
  delta_r = fabs(cluster_1.eta - cluster_2.eta) + fabs(cluster_1.phi - cluster_2.phi);

  return delta_r;
}

float TallinnTauCluster::weighted_avg_phi(simple_object_t cluster_1, simple_object_t cluster_2){
  float total_pt = (cluster_1.et+cluster_2.et);
  float avg_phi = (cluster_1.phi*cluster_1.et + cluster_2.phi*cluster_2.et)/total_pt;
  return avg_phi;
}

float TallinnTauCluster::weighted_avg_eta(simple_object_t cluster_1, simple_object_t cluster_2){
  float total_pt = (cluster_1.et+cluster_2.et);
  float avg_eta = (cluster_1.eta*cluster_1.et + cluster_2.eta*cluster_2.et)/total_pt;
  return avg_eta;
}


TallinnTauCluster::TallinnTauCluster() {};
TallinnTauCluster::~TallinnTauCluster() {};

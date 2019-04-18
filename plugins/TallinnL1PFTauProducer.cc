// -*- C++ -*-
//
// Package:    TallinnL1PFTauProducer
// Class:      TallinnL1PFTauProducer
// 
/**\class TallinnL1PFTauProducer TallinnL1PFTauProducer.cc L1Trigger/TallinnL1PFTaus/plugins/TallinnL1PFTauProducer.cc

 Description: Level 1 L1PFTaus Producer

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Sandeep Bhowmik
//         Created:  Tue, 12 Mar 2019 18:38:39 GMT
//
//

#include "L1Trigger/TallinnL1PFTaus/interface/TallinnL1PFTauProducer.hh"

TallinnL1PFTauProducer::TallinnL1PFTauProducer(const edm::ParameterSet& iConfig) :
  debug_          (iConfig.getUntrackedParameter<bool>("debug", false)),
  deltaR_         (iConfig.getParameter<double>("deltaR")),
  dz_cut_         (iConfig.getParameter<double>("dz_cut")),
  min_tauSeedPt_  (iConfig.getParameter<double>("min_tauSeedPt")),
  max_tauSeedEta_ (iConfig.getParameter<double>("max_tauSeedEta")),
  l1PFCandToken_  (consumes< vector<l1t::PFCandidate> >             (iConfig.getParameter<edm::InputTag>("l1PFCandToken"))),
  vtxTagToken_    (consumes<std::vector<reco::Vertex>>              (iConfig.getParameter<edm::InputTag>("vtxTagToken")))
{
  produces< TallinnL1PFTauCollection >( "L1PFTaus" ).setBranchAlias("L1PFTaus");
}

void TallinnL1PFTauProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  std::unique_ptr<TallinnL1PFTauCollection> newTallinnL1PFTauCollection(new TallinnL1PFTauCollection);
  std::unique_ptr<TallinnL1PFTauCollection> newTallinnL1PFTauCollectionUncleaned(new TallinnL1PFTauCollection);

  edm::Handle<  l1t::PFCandidateCollection > l1PFCandidates;
  iEvent.getByToken( l1PFCandToken_, l1PFCandidates);

  edm::Handle<std::vector<reco::Vertex> >  vertexes;
  iEvent.getByToken(vtxTagToken_, vertexes);

  l1t::PFCandidateCollection l1PFCandidates_sort;

  for(auto l1PFCand : *l1PFCandidates)
    l1PFCandidates_sort.push_back(l1PFCand);

  std::sort(l1PFCandidates_sort.begin(), l1PFCandidates_sort.end(), [](l1t::PFCandidate i,l1t::PFCandidate j){return(i.pt() > j.pt());});   

  l1t::PFCandidateCollection l1PFCandidates_cleaned;
  for(auto l1PFCand : l1PFCandidates_sort){
    //std::cout<< "PFCand vertex z  " << l1PFCand.vertex().z() << std::endl;  
    double zPV = 0;
    zPV = vertexes->at(0).z();
    double zTrackVertex = 0;
    if(l1PFCand.charge()!=0){
      l1t::PFTrackRef l1PFTrack = l1PFCand.pfTrack();
      zTrackVertex = l1PFTrack->vertex().z();
      //std::cout<< " l1PFTrack->vertex().z() " << l1PFTrack->vertex().z() << std::endl;
    }
    double dz = fabs(zPV - zTrackVertex);
    if (dz >= dz_cut_)
      continue;
    l1PFCandidates_cleaned.push_back(l1PFCand);
    //std::cout<<" dz "<<dz<<std::endl;
  }

  newTallinnL1PFTauCollectionUncleaned->clear();
  for(auto l1PFCand : l1PFCandidates_cleaned){
    if(l1PFCand.charge()!=0 && l1PFCand.pt()>min_tauSeedPt_ && fabs(l1PFCand.eta())<max_tauSeedEta_){
      std::cout<<"Seed Pt "<<l1PFCand.pt()<<" Seed Eta "<<l1PFCand.eta()<<" Seed Charge "<<l1PFCand.charge()<<std::endl;
      TallinnL1PFTauBuilder tempL1PFTau;
      tempL1PFTau.setL1PFTauSeed(l1PFCand);
      tempL1PFTau.addL1PFCandidates(l1PFCandidates_cleaned);
      tempL1PFTau.buildL1PFTau();
      newTallinnL1PFTauCollectionUncleaned->push_back(tempL1PFTau.l1PFTau_);
    }
  }

  newTallinnL1PFTauCollection->clear();
  cleanL1PFTaus(newTallinnL1PFTauCollectionUncleaned, newTallinnL1PFTauCollection);

  for(unsigned i = 0; i < newTallinnL1PFTauCollection->size(); i++)
  {
    std::cout<< i <<" Tau Pt "<<newTallinnL1PFTauCollection->at(i).pt()<<" Tau type "<< newTallinnL1PFTauCollection->at(i).tauType()<<std::endl;
  }

  iEvent.put( std::move(newTallinnL1PFTauCollection) , "L1PFTaus" );
}


void TallinnL1PFTauProducer::cleanL1PFTaus(std::unique_ptr<TallinnL1PFTauCollection> &newTallinnL1PFTauCollectionUncleaned, std::unique_ptr<TallinnL1PFTauCollection> &newTallinnL1PFTauCollection){
  std::sort(newTallinnL1PFTauCollectionUncleaned->begin(), newTallinnL1PFTauCollectionUncleaned->end(), [](l1t::TallinnL1PFTau i, l1t::TallinnL1PFTau j){return(i.pt() > j.pt());});
  int nL1PFTau = newTallinnL1PFTauCollectionUncleaned->size();
  float deltaR = 0.4;
  float L1PFtau1_eta = -50;
  float L1PFtau1_phi = -50;
  vector<int> tempL1PFTauTags;
  for(int iTau=0; iTau<nL1PFTau-1; iTau++){
    L1PFtau1_eta=newTallinnL1PFTauCollectionUncleaned->at(iTau).eta();
    L1PFtau1_phi=newTallinnL1PFTauCollectionUncleaned->at(iTau).phi();
    float L1PFTau2_eta = -100;
    float L1PFTau2_phi = -100;
    for(int kTau=iTau+1; kTau<nL1PFTau; kTau++){
      L1PFTau2_eta=newTallinnL1PFTauCollectionUncleaned->at(kTau).eta();
      L1PFTau2_phi=newTallinnL1PFTauCollectionUncleaned->at(kTau).phi();
      if(fabs(L1PFTau2_eta-L1PFtau1_eta)+fabs(L1PFTau2_phi-L1PFtau1_phi) < deltaR){
	tempL1PFTauTags.push_back(kTau);
      }
    }
  }
  int ntempL1PFTau = tempL1PFTauTags.size();
  for(int iTau=0; iTau<nL1PFTau; iTau++){
    bool ismatch = false;
    for(int jTau=0; jTau<ntempL1PFTau; jTau++){
      if(iTau==tempL1PFTauTags.at(jTau)){
	ismatch = true;
	break;
      }
    }
    if(!ismatch){
      newTallinnL1PFTauCollection->push_back(newTallinnL1PFTauCollectionUncleaned->at(iTau));
    }
  }
}



/////////////
// DESTRUCTOR
TallinnL1PFTauProducer::~TallinnL1PFTauProducer()
{
}  


//////////
// END JOB
void TallinnL1PFTauProducer::endRun(const edm::Run& run, const edm::EventSetup& iSetup)
{
}

////////////
// BEGIN JOB
void TallinnL1PFTauProducer::beginRun(const edm::Run& run, const edm::EventSetup& iSetup )
{
}


#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(TallinnL1PFTauProducer);

//  LocalWords:  PFChargedCandidates

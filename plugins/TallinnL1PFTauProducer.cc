// -*- C++ -*-
//
// Package:    TallinnL1PFTauProducer
// Class:      TallinnL1PFTauProducer
// 
/**\class TallinnL1PFTauProducer TallinnL1PFTauProducer.cc L1Trigger/TallinnL1PFTaus/plugins/TallinnL1PFTauProducer.cc

 Description: Level 1 L1PFTaus for the Demonstrator

 Implementation:
     [Notes on implementation]
*/


#include "L1Trigger/TallinnL1PFTaus/interface/TallinnL1PFTauProducer.hh"

TallinnL1PFTauProducer::TallinnL1PFTauProducer(const edm::ParameterSet& cfg) :
  debug(                cfg.getUntrackedParameter<bool>("debug", false)),
  deltaR_(           cfg.getParameter<double>("deltaR")),
  min_tauSeedPt_(       cfg.getParameter<double>("min_tauSeedPt")),
  max_tauSeedEta_(      cfg.getParameter<double>("max_tauSeedEta")),
  L1PFToken_(           consumes< vector<l1t::PFCandidate> >(cfg.getParameter<edm::InputTag>("L1PFObjects")))
{
  produces< TallinnL1PFTauCollection >( "L1PFTaus" ).setBranchAlias("L1PFTaus");
}

void TallinnL1PFTauProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  std::unique_ptr<TallinnL1PFTauCollection> newTallinnL1PFTauCollection(new TallinnL1PFTauCollection);

  edm::Handle<  l1t::PFCandidateCollection > l1PFCandidates;
  iEvent.getByToken( L1PFToken_, l1PFCandidates);

  l1t::PFCandidateCollection l1PFCandidates_sort;

  for(auto l1PFCand : *l1PFCandidates)
    l1PFCandidates_sort.push_back(l1PFCand);

  std::sort(l1PFCandidates_sort.begin(), l1PFCandidates_sort.end(), [](l1t::PFCandidate i,l1t::PFCandidate j){return(i.pt() > j.pt());});   


  // get all seed to make tau cluster
  tauCandidates.clear();
  for(auto l1PFCand : l1PFCandidates_sort){
    if(l1PFCand.charge()!=0 && l1PFCand.pt()>min_tauSeedPt_ && fabs(l1PFCand.eta())<max_tauSeedEta_){
      std::cout<<"Seed Pt "<<l1PFCand.pt()<<" Seed Eta "<<l1PFCand.eta()<<" Seed Charge "<<l1PFCand.charge()<<std::endl;
      createTau(tauCandidates, l1PFCand);
    }
  }
  // loop through PF Candidates and add PF Cands to each Tau 
  for(auto l1PFCand : l1PFCandidates_sort){
    for(auto &tauCandidate : tauCandidates){
      if(tauCandidate.tauSeed==l1PFCand)
	continue;
      if(fabs(l1PFCand.eta()-tauCandidate.tauSeed.eta())+fabs(l1PFCand.phi()-tauCandidate.tauSeed.phi()) < deltaR_){
	tauCandidate.addPFCandidate(l1PFCand);
	std::cout<<"in cluster PFCand Pt "<<l1PFCand.pt()<<" PFCand Eta "<<l1PFCand.eta()<<std::endl;
      }
    }
  }
  // Run Tau Algo over all Taus. 
  for(auto &tauCandidate : tauCandidates){
    tauCandidate.process();
  }
  
  // Sort by PT
  unsigned int nCands = tauCandidates.size();

  tau_cand_sort(tauCandidates, newTallinnL1PFTauCollection, nCands);

  for(unsigned i = 0; i < newTallinnL1PFTauCollection->size(); i++)
  {
    std::cout<< i <<" Tau Pt "<<newTallinnL1PFTauCollection->at(i).pt()<<" Tau type "<< newTallinnL1PFTauCollection->at(i).tauType()<<std::endl;
  }

  iEvent.put( std::move(newTallinnL1PFTauCollection) , "L1PFTaus" );
}
  

void TallinnL1PFTauProducer::createTau(TallinnTauClusterCollection &inputCollection, l1t::PFCandidate Seed){
  TallinnTauCluster tempTau;
  tempTau.tauSeed = Seed;
  inputCollection.push_back(tempTau);
}


void TallinnL1PFTauProducer::tau_cand_sort(TallinnTauClusterCollection tauCandidates, std::unique_ptr<TallinnL1PFTauCollection> &newTallinnL1PFTauCollection, unsigned int nCands){
  std::sort(tauCandidates.begin(), tauCandidates.end(), [](TallinnTauCluster i,TallinnTauCluster j){return(i.l1PFTau.pt() > j.l1PFTau.pt());});   
  
  for(unsigned int i = 0; i < nCands && i < tauCandidates.size(); i++){
    newTallinnL1PFTauCollection->push_back(tauCandidates.at(i).l1PFTau);
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

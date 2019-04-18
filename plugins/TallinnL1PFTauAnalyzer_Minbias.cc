// -*- C++ -*-
//
// Package:    TallinnL1PFTauAnalyzer
// Class:      TallinnL1PFTauAnalyzer
//
/**\class TallinnL1PFTauAnalyzer TallinnL1PFTauAnalyzer.cc L1Trigger/TallinnL1PFTaus/plugins/TallinnL1PFTauAnalyzer.cc

 Description: Level 1 L1PFTaus Analyzer

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Sandeep Bhowmik
//         Created:  Tue, 12 Mar 2019 18:38:39 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include <TTree.h>
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/L1Trigger/interface/L1PFTau.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include <DataFormats/PatCandidates/interface/Muon.h>
#include <DataFormats/PatCandidates/interface/Tau.h>
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/L1Trigger/interface/Tau.h"
#include <DataFormats/PatCandidates/interface/CompositeCandidate.h>


#include <cmath>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include <vector>
#include <utility>
#include <TNtuple.h>
#include <TString.h>
#include <bitset>


#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <FWCore/Framework/interface/Frameworkfwd.h>
#include <FWCore/Framework/interface/Event.h>
#include <FWCore/Framework/interface/ESHandle.h>
#include <FWCore/Utilities/interface/InputTag.h>
#include <DataFormats/PatCandidates/interface/Muon.h>
#include <DataFormats/PatCandidates/interface/Tau.h>
#include "DataFormats/PatCandidates/interface/Jet.h"

#include "DataFormats/L1Trigger/interface/Jet.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "DataFormats/L1Trigger/interface/Tau.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"

#include "DataFormats/Common/interface/TriggerResults.h"

#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include <DataFormats/Common/interface/View.h>
#include "DataFormats/TallinnL1PFTaus/interface/TallinnL1PFTau.h"

using reco::TrackCollection;

class TallinnL1PFTauAnalyzer_Minbias : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
public:
  explicit TallinnL1PFTauAnalyzer_Minbias(const edm::ParameterSet&);
  ~TallinnL1PFTauAnalyzer_Minbias();
  
private:
  virtual void beginJob() override;
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override;
  void Initialize();
  
  TTree *tree_;
  std::string treeName_;
  
  ULong64_t       indexevents_;
  Int_t           runNumber_;
  Int_t           lumi_;
  float MC_weight_;
  std::vector<int> l1PFTauType_;
  std::vector<float> l1PFTauPt_;
  std::vector<float> l1PFTauEta_;
  std::vector<float> l1PFTauPhi_;
  std::vector<int> l1PFTauIso_;
  std::vector<Bool_t> l1PFTauTightIso_;
  std::vector<Bool_t> l1PFTauMediumIso_;
  std::vector<Bool_t> l1PFTauLooseIso_;
  std::vector<Bool_t> l1PFTauVLooseIso_;
  int Nvtx_;

  // ----------member data ---------------------------

  edm::EDGetTokenT<l1t::TallinnL1PFTauCollection>   l1PFTauToken_;
  edm::EDGetTokenT<std::vector<reco::Vertex>> vtxTagToken_;

};


TallinnL1PFTauAnalyzer_Minbias::TallinnL1PFTauAnalyzer_Minbias(const edm::ParameterSet& iConfig)
 :
  l1PFTauToken_   (consumes<l1t::TallinnL1PFTauCollection>          (iConfig.getParameter<edm::InputTag>("l1PFTauToken"))),
  vtxTagToken_    (consumes<std::vector<reco::Vertex>>              (iConfig.getParameter<edm::InputTag>("vtxTagToken")))
{
   //now do what ever initialization is needed
  treeName_ = iConfig.getParameter<std::string>("treeName");
  return;
}


TallinnL1PFTauAnalyzer_Minbias::~TallinnL1PFTauAnalyzer_Minbias()
{

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
TallinnL1PFTauAnalyzer_Minbias::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   indexevents_ = iEvent.id().event();
   runNumber_ = iEvent.id().run();
   lumi_ = iEvent.luminosityBlock();

   edm::Handle<l1t::TallinnL1PFTauCollection>  L1PFTauHandle;
   iEvent.getByToken(l1PFTauToken_,  L1PFTauHandle);

   edm::Handle<std::vector<reco::Vertex> >  vertexes;
   iEvent.getByToken(vtxTagToken_,      vertexes);
   Nvtx_ = vertexes->size();

   if(L1PFTauHandle->size()>0){
     for(unsigned int i = 0; i < L1PFTauHandle->size(); i++){
       const l1t::TallinnL1PFTau& l1pftau = (*L1PFTauHandle)[i];
       l1PFTauPt_.push_back(l1pftau.pt());
       l1PFTauEta_.push_back(l1pftau.eta());
       l1PFTauPhi_.push_back(l1pftau.phi());
       l1PFTauType_.push_back(l1pftau.tauType());
       l1PFTauIso_.push_back(l1pftau.chargedIso());
       l1PFTauTightIso_.push_back(l1pftau.passTightIso());
       l1PFTauMediumIso_.push_back(l1pftau.passMediumIso());
       l1PFTauLooseIso_.push_back(l1pftau.passLooseIso());
       l1PFTauVLooseIso_.push_back(l1pftau.passVLooseIso());
     }
   }

   tree_ -> Fill();

}


void TallinnL1PFTauAnalyzer_Minbias::Initialize() {
  indexevents_ = 0;
  runNumber_ = 0;
  lumi_ = 0;
  MC_weight_ = 1;
  l1PFTauType_ .clear();
  l1PFTauPt_ .clear();
  l1PFTauEta_ .clear();
  l1PFTauPhi_ .clear();
  l1PFTauIso_ .clear();
  l1PFTauIso_ .clear();
  l1PFTauTightIso_ .clear();
  l1PFTauMediumIso_ .clear();
  l1PFTauLooseIso_ .clear();
  l1PFTauVLooseIso_ .clear();
  Nvtx_ = 0;
}


// ------------ method called once each job just before starting event loop  ------------
void
TallinnL1PFTauAnalyzer_Minbias::beginJob()
{
  edm::Service<TFileService> fs;
  tree_ = fs -> make<TTree>(treeName_.c_str(), treeName_.c_str());
  tree_ -> Branch("EventNumber",&indexevents_,"EventNumber/l");
  tree_ -> Branch("RunNumber",&runNumber_,"RunNumber/I");
  tree_ -> Branch("lumi",&lumi_,"lumi/I");
  tree_ -> Branch("MC_weight",&MC_weight_,"MC_weight/F");
  tree_ -> Branch("l1PFTauPt",  &l1PFTauPt_);
  tree_ -> Branch("l1PFTauEta", &l1PFTauEta_);
  tree_ -> Branch("l1PFTauPhi", &l1PFTauPhi_);
  tree_ -> Branch("l1PFTauType", &l1PFTauType_);
  tree_ -> Branch("l1PFTauIso", &l1PFTauIso_);
  tree_ -> Branch("l1PFTauTightIso", &l1PFTauTightIso_);
  tree_ -> Branch("l1PFTauMediumIso", &l1PFTauMediumIso_);
  tree_ -> Branch("l1PFTauLooseIso", &l1PFTauLooseIso_);
  tree_ -> Branch("l1PFTauVLooseIso", &l1PFTauVLooseIso_);
  tree_ -> Branch("Nvtx", &Nvtx_, "Nvtx/I");
  return;
}

// ------------ method called once each job just after ending the event loop  ------------
void
TallinnL1PFTauAnalyzer_Minbias::endJob()
{
  return;
}

//define this as a plug-in
DEFINE_FWK_MODULE(TallinnL1PFTauAnalyzer_Minbias);

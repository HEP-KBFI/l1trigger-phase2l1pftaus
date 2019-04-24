#include "L1Trigger/TallinnL1PFTaus/plugins/TallinnL1PFTauProducer.h"

#include "FWCore/Utilities/interface/InputTag.h"

TallinnL1PFTauProducer::TallinnL1PFTauProducer(const edm::ParameterSet& cfg) 
  : tauBuilder_(cfg)
  , l1PFCandToken_(consumes<l1t::PFCandidateCollection>(cfg.getParameter<edm::InputTag>("l1PFCandToken")))
  , vtxTagToken_(consumes<reco::VertexCollection>(cfg.getParameter<edm::InputTag>("vtxTagToken")))
  , min_tauSeed_pt_(cfg.getParameter<double>("min_tauSeed_pt"))
  , max_tauSeed_eta_(cfg.getParameter<double>("max_tauSeed_eta"))
  , max_tauSeed_dz_(cfg.getParameter<double>("max_tauSeed_dz"))
  , deltaR_cleaning_(cfg.getParameter<double>("deltaR_cleaning"))
  , debug_(cfg.getUntrackedParameter<bool>("debug", false))
{
  deltaR2_cleaning_ = deltaR_cleaning_*deltaR_cleaning_;

  edm::ParameterSet cfg_signalQualityCuts = cfg.getParameter<edm::ParameterSet>("signalQualityCuts");
  signalQualityCuts_ = readL1PFTauQualityCuts(cfg_signalQualityCuts);
  edm::ParameterSet cfg_isolationQualityCuts = cfg.getParameter<edm::ParameterSet>("isolationQualityCuts");
  isolationQualityCuts_ = readL1PFTauQualityCuts(cfg_isolationQualityCuts);

  produces<l1t::TallinnL1PFTauCollection>("L1PFTaus").setBranchAlias("L1PFTaus");
}

TallinnL1PFTauProducer::~TallinnL1PFTauProducer()
{}

namespace
{
  bool
  isHigherPt(const l1t::PFCandidateRef& l1PFCand1,
             const l1t::PFCandidateRef& l1PFCand2)
  {
    return l1PFCand1->pt() > l1PFCand2->pt();
  }
}

void TallinnL1PFTauProducer::produce(edm::Event& evt, const edm::EventSetup& es)
{
  std::unique_ptr<l1t::TallinnL1PFTauCollection> l1PFTauCollection_cleaned(new l1t::TallinnL1PFTauCollection());

  edm::Handle<l1t::PFCandidateCollection> l1PFCands;
  evt.getByToken(l1PFCandToken_, l1PFCands);

  edm::Handle<reco::VertexCollection> vertices;
  evt.getByToken(vtxTagToken_, vertices);
  const reco::Vertex* primaryVertex = ( vertices->size() > 0 ) ? &vertices->at(0) : nullptr;

  // build collection of selected PFCandidates
  std::vector<l1t::PFCandidateRef> l1PFCands_selected;
  size_t numL1PFCands = l1PFCands->size();
  for ( size_t idxL1PFCand = 0; idxL1PFCand < numL1PFCands; ++idxL1PFCand ) {
    l1t::PFCandidateRef l1PFCand(l1PFCands, idxL1PFCand);
    if ( isSelected(signalQualityCuts_,    *l1PFCand, primaryVertex) || 
	 isSelected(isolationQualityCuts_, *l1PFCand, primaryVertex) ) 
    {
      l1PFCands_selected.push_back(l1PFCand);
    }
  }

  // sort PFCandidate collection by decreasing pT
  std::sort(l1PFCands_selected.begin(), l1PFCands_selected.end(), isHigherPt);

  std::vector<l1t::PFCandidateRef> tauSeeds;
  for ( auto l1PFCand : l1PFCands_selected ) 
  {
    if ( l1PFCand->charge() != 0 && l1PFCand->pt() > min_tauSeed_pt_ && fabs(l1PFCand->eta()) < max_tauSeed_eta_ )
    {
      bool isFromPrimaryVertex = false;
      if ( primaryVertex ) 
      {
        l1t::PFTrackRef l1PFTrack = l1PFCand->pfTrack();
        double dz = std::fabs(l1PFTrack->vertex().z() - primaryVertex->z());
        if ( dz < max_tauSeed_dz_ ) 
        {
	  isFromPrimaryVertex = true;
        }
      } 
      else 
      {
        isFromPrimaryVertex = true;
      }
      if ( isFromPrimaryVertex ) 
      {
        tauSeeds.push_back(l1PFCand);
      }
    }
  }

  l1t::TallinnL1PFTauCollection l1PFTauCollection_uncleaned;
  for ( auto tauSeed : tauSeeds ) 
  {
    tauBuilder_.reset();
    tauBuilder_.setL1PFTauSeed_and_Vertex(tauSeed, primaryVertex);
    tauBuilder_.addL1PFCandidates(l1PFCands_selected);
    tauBuilder_.buildL1PFTau();
    l1PFTauCollection_uncleaned.push_back(tauBuilder_.getL1PFTau());
  }

  for ( auto l1PFTau : l1PFTauCollection_uncleaned )
  {
    bool isOverlap = false;
    for ( auto l1PFTau2 : *l1PFTauCollection_cleaned )
    {
      double deltaEta = l1PFTau.eta() - l1PFTau2.eta();
      double deltaPhi = l1PFTau.phi() - l1PFTau2.phi();
      if ( (deltaEta*deltaEta + deltaPhi*deltaPhi) < deltaR2_cleaning_ ) 
      {
	isOverlap = true;
      }      
    }
    if ( !isOverlap )
    {
      l1PFTauCollection_cleaned->push_back(l1PFTau);
    }
  }
  
  if ( debug_ ) 
  {
    for ( size_t idx = 0; idx < l1PFTauCollection_cleaned->size(); ++idx )
    {
      const l1t::TallinnL1PFTau& l1PFTau = l1PFTauCollection_cleaned->at(idx);
      std::cout << "tau #" << idx << ": pT = " << l1PFTau.pt()  << ", eta = " << l1PFTau.eta() << ", phi = " << l1PFTau.phi()
	        << " (type = " << l1PFTau.tauType() << ")" << std::endl;
    }
  }

  evt.put(std::move(l1PFTauCollection_cleaned), "L1PFTaus");
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(TallinnL1PFTauProducer);

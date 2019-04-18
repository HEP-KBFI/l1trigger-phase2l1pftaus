#include "L1Trigger/TallinnL1PFTaus/plugins/TallinnL1PFTauProducer.h"

#include "FWCore/Utilities/interface/InputTag.h"

using namespace l1t;

TallinnL1PFTauProducer::TallinnL1PFTauProducer(const edm::ParameterSet& cfg) 
  : debug_(cfg.getUntrackedParameter<bool>("debug", false))
  , deltaR_(cfg.getParameter<double>("deltaR"))
  , dz_cut_(cfg.getParameter<double>("dz_cut"))
  , min_tauSeedPt_(cfg.getParameter<double>("min_tauSeedPt"))
  , max_tauSeedEta_(cfg.getParameter<double>("max_tauSeedEta"))
  , l1PFCandToken_(consumes<vector<l1t::PFCandidate>>(cfg.getParameter<edm::InputTag>("l1PFCandToken")))
  , vtxTagToken_(consumes<std::vector<reco::Vertex>>(cfg.getParameter<edm::InputTag>("vtxTagToken")))
{
  deltaR2_ = deltaR_*deltaR_;
  produces<TallinnL1PFTauCollection>("L1PFTaus").setBranchAlias("L1PFTaus");
}

TallinnL1PFTauProducer::~TallinnL1PFTauProducer()
{}

void TallinnL1PFTauProducer::produce(edm::Event& evt, const edm::EventSetup& es)
{
  std::unique_ptr<TallinnL1PFTauCollection> l1PFTauCollection_cleaned(new TallinnL1PFTauCollection());

  edm::Handle<l1t::PFCandidateCollection> l1PFCands;
  evt.getByToken(l1PFCandToken_, l1PFCands);

  edm::Handle<std::vector<reco::Vertex>> vertices;
  evt.getByToken(vtxTagToken_, vertices);
  double primaryVertex_pz = ( vertices->size() > 0 ) ? vertices->at(0).z() : 0.;

  l1t::PFCandidateCollection l1PFCands_selected;
  for ( auto l1PFCand : *l1PFCands ) 
  {
    bool isFromPrimaryVertex = false;
    if ( l1PFCand.charge() != 0 && vertices->size() > 0 ) 
    {
      l1t::PFTrackRef l1PFTrack = l1PFCand.pfTrack();
      double dz = std::fabs(l1PFTrack->vertex().z() - primaryVertex_pz);
      if ( dz < dz_cut_ ) 
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
      l1PFCands_selected.push_back(l1PFCand);
    }
  }
  std::sort(l1PFCands_selected.begin(), l1PFCands_selected.end(), [](l1t::PFCandidate i, l1t::PFCandidate j){return(i.pt() > j.pt());});  

  TallinnL1PFTauCollection l1PFTauCollection_uncleaned;
  for ( auto l1PFCand : l1PFCands_selected ) 
  {
    if ( l1PFCand.charge() != 0 && l1PFCand.pt() > min_tauSeedPt_ && fabs(l1PFCand.eta()) < max_tauSeedEta_ )
    {
      TallinnL1PFTauBuilder l1PFTauBuilder;
      l1PFTauBuilder.setL1PFTauSeed(l1PFCand);
      l1PFTauBuilder.addL1PFCandidates(l1PFCands_selected);
      l1PFTauBuilder.buildL1PFTau();
      l1PFTauCollection_uncleaned.push_back(l1PFTauBuilder.getL1PFTau());
    }
  }

  for ( auto l1PFTau : l1PFTauCollection_uncleaned )
  {
    bool isOverlap = false;
    for ( auto l1PFTau2 : *l1PFTauCollection_cleaned )
    {
      double deltaEta = l1PFTau.eta() - l1PFTau2.eta();
      double deltaPhi = l1PFTau.phi() - l1PFTau2.phi();
      if ( (deltaEta*deltaEta + deltaPhi*deltaPhi) < deltaR2_ ) 
      {
	isOverlap = true;
      }      
    }
    if ( !isOverlap )
    {
      l1PFTauCollection_cleaned->push_back(l1PFTau);
    }
  }

  for ( size_t idx = 0; idx < l1PFTauCollection_cleaned->size(); ++idx )
  {
    const TallinnL1PFTau& l1PFTau = l1PFTauCollection_cleaned->at(idx);
    std::cout << "tau #" << idx << ": pT = " << l1PFTau.pt()  << ", eta = " << l1PFTau.eta() << ", phi = " << l1PFTau.phi()
	      << " (type = " << l1PFTau.tauType() << ")" << std::endl;
  }

  evt.put(std::move(l1PFTauCollection_cleaned), "L1PFTaus");
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(TallinnL1PFTauProducer);

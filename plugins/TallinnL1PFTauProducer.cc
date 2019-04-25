#include "L1Trigger/TallinnL1PFTaus/plugins/TallinnL1PFTauProducer.h"

#include "FWCore/Utilities/interface/InputTag.h"

TallinnL1PFTauProducer::TallinnL1PFTauProducer(const edm::ParameterSet& cfg) 
  : tauBuilder_(cfg)
  , l1PFCandToken_(consumes<l1t::PFCandidateCollection>(cfg.getParameter<edm::InputTag>("l1PFCandToken")))
  , l1PFJetToken_(consumes<reco::PFJetCollection>(cfg.getParameter<edm::InputTag>("l1PFJetToken")))
  , vtxTagToken_(consumes<reco::VertexCollection>(cfg.getParameter<edm::InputTag>("vtxTagToken")))
  , useChargedPFCandSeeds_(cfg.getParameter<bool>("useChargedPFCandSeeds"))
  , min_seedChargedPFCand_pt_(cfg.getParameter<double>("min_seedChargedPFCand_pt"))
  , max_seedChargedPFCand_eta_(cfg.getParameter<double>("max_seedChargedPFCand_eta"))
  , max_seedChargedPFCand_dz_(cfg.getParameter<double>("max_seedChargedPFCand_dz"))
  , usePFJetSeeds_(cfg.getParameter<bool>("usePFJetSeeds"))
  , min_seedPFJet_pt_(cfg.getParameter<double>("min_seedPFJet_pt"))
  , max_seedPFJet_eta_(cfg.getParameter<double>("max_seedPFJet_eta"))
  , min_PFTau_pt_(cfg.getParameter<double>("min_PFTau_pt"))
  , max_PFTau_eta_(cfg.getParameter<double>("max_PFTau_eta"))
  , min_leadChargedPFCand_pt_(cfg.getParameter<double>("min_leadChargedPFCand_pt"))
  , max_leadChargedPFCand_eta_(cfg.getParameter<double>("max_leadChargedPFCand_eta"))
  , max_leadChargedPFCand_dz_(cfg.getParameter<double>("max_leadChargedPFCand_dz"))
  , max_chargedIso_(cfg.getParameter<double>("max_chargedIso"))
  , max_chargedRelIso_(cfg.getParameter<double>("max_chargedRelIso"))
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
  isHigherPt_pfCandRef(const l1t::PFCandidateRef& l1PFCand1,
		       const l1t::PFCandidateRef& l1PFCand2)
  {
    return l1PFCand1->pt() > l1PFCand2->pt();
  }

  bool
  isHigherPt_pfTau(const l1t::TallinnL1PFTau& l1PFTau1,
		   const l1t::TallinnL1PFTau& l1PFTau2)
  {
    return l1PFTau1.pt() > l1PFTau2.pt();
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
  std::sort(l1PFCands_selected.begin(), l1PFCands_selected.end(), isHigherPt_pfCandRef);

  l1t::TallinnL1PFTauCollection l1PFTauCollection_uncleaned;

  if ( useChargedPFCandSeeds_ ) 
  {
    for ( auto l1PFCand : l1PFCands_selected ) 
    {
      if ( l1PFCand->charge() != 0 && l1PFCand->pt() > min_seedChargedPFCand_pt_ && fabs(l1PFCand->eta()) < max_seedChargedPFCand_eta_ )
      {
        bool isFromPrimaryVertex = false;
        if ( primaryVertex ) 
        {
          l1t::PFTrackRef l1PFTrack = l1PFCand->pfTrack();
          double dz = std::fabs(l1PFTrack->vertex().z() - primaryVertex->z());
          if ( dz < max_seedChargedPFCand_dz_ ) 
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
	  tauBuilder_.reset();
          tauBuilder_.setL1PFCandProductID(l1PFCands.id());
	  tauBuilder_.setVertex(primaryVertex);
	  tauBuilder_.setL1PFTauSeed(l1PFCand);
	  tauBuilder_.addL1PFCandidates(l1PFCands_selected);
	  tauBuilder_.buildL1PFTau();
	  l1PFTauCollection_uncleaned.push_back(tauBuilder_.getL1PFTau());
        }
      }
    }
  }

  if ( usePFJetSeeds_ )
  {
    edm::Handle<reco::PFJetCollection> l1PFJets;
    evt.getByToken(l1PFJetToken_, l1PFJets);

    size_t numL1PFJets = l1PFJets->size();
    for ( size_t idxL1PFJet = 0; idxL1PFJet < numL1PFJets; ++idxL1PFJet ) {
      reco::PFJetRef l1PFJet(l1PFJets, idxL1PFJet);
      if ( l1PFJet->pt() > min_seedPFJet_pt_ && std::fabs(l1PFJet->eta()) < max_seedPFJet_eta_ )
      {
	tauBuilder_.reset();
	tauBuilder_.setL1PFCandProductID(l1PFCands.id());
	tauBuilder_.setVertex(primaryVertex);
	tauBuilder_.setL1PFTauSeed(l1PFJet);
	tauBuilder_.addL1PFCandidates(l1PFCands_selected);
	tauBuilder_.buildL1PFTau();
	l1PFTauCollection_uncleaned.push_back(tauBuilder_.getL1PFTau());
      }
    }
  }

  // sort PFTau candidate collection by decreasing pT
  std::sort(l1PFTauCollection_uncleaned.begin(), l1PFTauCollection_uncleaned.end(), isHigherPt_pfTau);

  for ( auto l1PFTau : l1PFTauCollection_uncleaned )
  {
    if ( l1PFTau.pt() > min_PFTau_pt_ && std::fabs(l1PFTau.eta()) < max_PFTau_eta_ &&
	 l1PFTau.leadChargedPFCand().isNonnull() && 
	 l1PFTau.leadChargedPFCand()->pt() > min_leadChargedPFCand_pt_ && std::fabs(l1PFTau.leadChargedPFCand()->eta()) < max_leadChargedPFCand_eta_ && 
	 std::fabs(l1PFTau.leadChargedPFCand()->pfTrack()->vertex().z() - primaryVertex->z()) < max_leadChargedPFCand_dz_ &&
	 l1PFTau.sumChargedIso() < max_chargedIso_ && l1PFTau.sumChargedIso() < max_chargedRelIso_*l1PFTau.pt() )
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
  }
  
  if ( debug_ ) 
  {
    for ( size_t idx = 0; idx < l1PFTauCollection_cleaned->size(); ++idx )
    {
      const l1t::TallinnL1PFTau& l1PFTau = l1PFTauCollection_cleaned->at(idx);
      std::cout << "tau #" << idx << ": pT = " << l1PFTau.pt()  << ", eta = " << l1PFTau.eta() << ", phi = " << l1PFTau.phi() 
		<< " (type = " << l1PFTau.tauType() << ")" << std::endl;
      std::cout << " leadChargedPFCand:";
      if ( l1PFTau.leadChargedPFCand().isNonnull() ) 
      {
	const l1t::PFCandidate& leadChargedPFCand = *l1PFTau.leadChargedPFCand();
	std::cout << " pT = " << leadChargedPFCand.pt()  << ", eta = " << leadChargedPFCand.eta() << ", phi = " << leadChargedPFCand.phi(); 
      }
      else
      {
	std::cout << " N/A";
      }
      std::cout << std::endl;
      std::cout << " seed:";
      if ( l1PFTau.isChargedPFCandSeeded() ) 
      {
	std::cout << " chargedPFCand";
      }
      else if ( l1PFTau.isPFJetSeeded() ) 
      {
	std::cout << " PFJet";
      }
      else assert(0);
      std::cout << std::endl;
      std::cout << " isolation: charged = " << l1PFTau.sumChargedIso() << ", neutral = " << l1PFTau.sumNeutralIso() << std::endl;
    }
  }

  evt.put(std::move(l1PFTauCollection_cleaned), "L1PFTaus");
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(TallinnL1PFTauProducer);

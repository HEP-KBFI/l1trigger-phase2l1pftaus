#include "L1Trigger/TallinnL1PFTaus/interface/TallinnL1PFTauQualityCut.h"

#include "FWCore/Utilities/interface/Exception.h" // cms::Exception

TallinnL1PFTauQualityCut::TallinnL1PFTauQualityCut(const edm::ParameterSet& cfg)
  : debug_(cfg.getUntrackedParameter<bool>("debug", false))
{
  std::string pfCandType_string = cfg.getParameter<std::string>("pfCandType");
  if      ( pfCandType_string == "chargedHadron" ) pfCandType_ = l1t::PFCandidate::ChargedHadron;
  else if ( pfCandType_string == "electron"      ) pfCandType_ = l1t::PFCandidate::Electron;
  else if ( pfCandType_string == "muon"          ) pfCandType_ = l1t::PFCandidate::Muon;
  else if ( pfCandType_string == "neutralHadron" ) pfCandType_ = l1t::PFCandidate::NeutralHadron;
  else if ( pfCandType_string == "photon"        ) pfCandType_ = l1t::PFCandidate::Photon;
  else throw cms::Exception("TallinnL1PFTauQualityCut")
    << "Invalid Configuration parameter 'pfCandType' = '" << pfCandType_string << "' !!\n";

  std::string primary_or_pileup_string = cfg.getParameter<std::string>("primary_or_pileup");
  if      ( primary_or_pileup_string == "primary" ) primary_or_pileup_ = kPrimary;
  else if ( primary_or_pileup_string == "pileup"  ) primary_or_pileup_ = kPileup;
  else throw cms::Exception("TallinnL1PFTauQualityCut")
    << "Invalid Configuration parameter 'primary_or_pileup' = '" << primary_or_pileup_string << "' !!\n";

  min_pt_ = cfg.getParameter<double>("min_pt");
  max_dz_ = ( cfg.exists("max_dz") ) ? cfg.getParameter<double>("max_dz") : 1.e+3;

  if ( debug_ && primary_or_pileup_ == kPrimary )
  {
    std::cout << " applying pT > " << min_pt_ << " GeV && dz < " << max_dz_ << " cm to PFCands of type = '" << pfCandType_string << "'" << std::endl; 
  } 
}

TallinnL1PFTauQualityCut::~TallinnL1PFTauQualityCut()
{}

bool TallinnL1PFTauQualityCut::operator()(const l1t::PFCandidate& pfCand, const l1t::Vertex* primaryVertex) const
{
  if ( pfCand.id() == pfCandType_ )
  {  
    if ( pfCand.pt() < min_pt_ ) 
    {
      return false;
    }

    if ( pfCand.charge() != 0 )
    {
      if ( primaryVertex )
      {
	l1t::PFTrackRef pfCand_track = pfCand.pfTrack();
        double dz = std::fabs(pfCand_track->vertex().z() - primaryVertex->z0());  
	if ( primary_or_pileup_ == kPrimary && dz >  max_dz_ ) return false;
	if ( primary_or_pileup_ == kPileup  && dz <= max_dz_ ) return false;
      }
      else
      {
	return false; // CV: reject all charged PFCands in case event contains no primary vertex and dz cannot be computed
      }
    } 
    else if ( primary_or_pileup_ == kPileup )
    {
      return false;   // CV: only consider charged PFCands as originating from pileup
    }
  }
  return true;
}

TallinnL1PFTauQualityCut readL1PFTauQualityCut(const edm::ParameterSet& cfg, const std::string& pfCandType, const std::string& primary_or_pileup, bool debug)
{
  edm::ParameterSet cfg_pfCandType = cfg.getParameter<edm::ParameterSet>(pfCandType);
  cfg_pfCandType.addParameter<std::string>("pfCandType", pfCandType);
  cfg_pfCandType.addParameter<std::string>("primary_or_pileup", primary_or_pileup);
  cfg_pfCandType.addUntrackedParameter<bool>("debug", debug);
  TallinnL1PFTauQualityCut qualityCut(cfg_pfCandType);
  return qualityCut;
}

std::vector<TallinnL1PFTauQualityCut> readL1PFTauQualityCuts(const edm::ParameterSet& cfg, const std::string& primary_or_pileup, bool debug)
{
  std::vector<TallinnL1PFTauQualityCut> qualityCuts;
  qualityCuts.push_back(readL1PFTauQualityCut(cfg, "chargedHadron", primary_or_pileup, debug));
  qualityCuts.push_back(readL1PFTauQualityCut(cfg, "electron",      primary_or_pileup, debug));
  qualityCuts.push_back(readL1PFTauQualityCut(cfg, "muon",          primary_or_pileup, debug));
  qualityCuts.push_back(readL1PFTauQualityCut(cfg, "photon",        primary_or_pileup, debug));
  qualityCuts.push_back(readL1PFTauQualityCut(cfg, "neutralHadron", primary_or_pileup, debug));
  return qualityCuts;
}

bool isSelected(const std::vector<TallinnL1PFTauQualityCut>& qualityCuts, const l1t::PFCandidate& pfCand, const l1t::Vertex* primaryVertex)
{
  for ( auto qualityCut : qualityCuts ) 
  {
    if ( !qualityCut(pfCand, primaryVertex) ) return false;
  }
  return true;
}

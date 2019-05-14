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
  min_pt_ = cfg.getParameter<double>("min_pt");
  max_dz_ = ( cfg.exists("max_dz") ) ? cfg.getParameter<double>("max_dz") : 1.e+3;
  if ( debug_ )
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
    if ( !(pfCand.pt() > min_pt_) ) return false;

    if ( pfCand.charge() != 0 && primaryVertex )
    {
      l1t::PFTrackRef pfCand_track = pfCand.pfTrack();
      double dz = std::fabs(pfCand_track->vertex().z() - primaryVertex->z0());      
      if ( !(dz < max_dz_) ) return false;
    }
  }
  return true;
}

TallinnL1PFTauQualityCut readL1PFTauQualityCut(const edm::ParameterSet& cfg, const std::string& pfCandType, bool debug)
{
  edm::ParameterSet cfg_pfCandType = cfg.getParameter<edm::ParameterSet>(pfCandType);
  cfg_pfCandType.addParameter<std::string>("pfCandType", pfCandType);
  cfg_pfCandType.addUntrackedParameter<bool>("debug", debug);
  TallinnL1PFTauQualityCut qualityCut(cfg_pfCandType);
  return qualityCut;
}

std::vector<TallinnL1PFTauQualityCut> readL1PFTauQualityCuts(const edm::ParameterSet& cfg, bool debug)
{
  std::vector<TallinnL1PFTauQualityCut> qualityCuts;
  qualityCuts.push_back(readL1PFTauQualityCut(cfg, "chargedHadron", debug));
  qualityCuts.push_back(readL1PFTauQualityCut(cfg, "electron",      debug));
  qualityCuts.push_back(readL1PFTauQualityCut(cfg, "muon",          debug));
  qualityCuts.push_back(readL1PFTauQualityCut(cfg, "photon",        debug));
  qualityCuts.push_back(readL1PFTauQualityCut(cfg, "neutralHadron", debug));
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

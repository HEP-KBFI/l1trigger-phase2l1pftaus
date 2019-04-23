import FWCore.ParameterSet.Config as cms

TallinnL1PFTauProducer = cms.EDProducer("TallinnL1PFTauProducer",
  l1PFCandToken      = cms.InputTag("l1pfCandidates", "PF"),
  vtxTagToken        = cms.InputTag("offlineSlimmedPrimaryVertices"),
  min_tauSeed_pt     = cms.double(5.),
  max_tauSeed_eta    = cms.double(2.1),
  max_tauSeed_dz     = cms.double(0.2),                                      
  min_signalConeSize = cms.double(0.05),
  max_signalConeSize = cms.double(0.10),
  stripSize_eta      = cms.double(0.05),
  stripSize_phi      = cms.double(0.20),
  deltaR_cleaning    = cms.double(0.4),
  signalQualityCuts = cms.PSet(
    chargedHadron = cms.PSet(
      min_pt = cms.double(0.),
      max_dz = cms.double(0.2),                                          
    ),
    neutralHadron = cms.PSet(
      min_pt = cms.double(0.)
    ),                                        
    muon = cms.PSet(
      min_pt = cms.double(0.),
      max_dz = cms.double(0.2),                                          
    ),
    electron = cms.PSet(
      min_pt = cms.double(0.),
      max_dz = cms.double(0.2),                                          
    ),                                            
    photon = cms.PSet(
      min_pt = cms.double(0.)
    )                                      
  ),
  isolationQualityCuts = cms.PSet(
    chargedHadron = cms.PSet(
      min_pt = cms.double(0.),
      max_dz = cms.double(0.2),                                          
    ),
    neutralHadron = cms.PSet(
      min_pt = cms.double(0.)
    ),                                        
    muon = cms.PSet(
      min_pt = cms.double(0.),
      max_dz = cms.double(0.2),                                          
    ),
    electron = cms.PSet(
      min_pt = cms.double(0.),
      max_dz = cms.double(0.2),                                          
    ),                                            
    photon = cms.PSet(
      min_pt = cms.double(0.)
    )              
  ),                                        
  debug = cms.untracked.bool(False),                                    
)

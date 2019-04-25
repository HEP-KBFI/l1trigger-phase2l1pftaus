import FWCore.ParameterSet.Config as cms

TallinnL1PFTauProducer = cms.EDProducer("TallinnL1PFTauProducer",
  #l1PFCandToken             = cms.InputTag("l1pfCandidates:PF"),
  l1PFCandToken             = cms.InputTag("l1pfCandidates:Puppi"),                                      
  l1PFJetToken              = cms.InputTag("ak4PFL1Puppi"),                      
  vtxTagToken               = cms.InputTag("offlineSlimmedPrimaryVertices"),
  useChargedPFCandSeeds     = cms.bool(True),                          
  min_seedChargedPFCand_pt  = cms.double(5.),
  max_seedChargedPFCand_eta = cms.double(2.1),
  max_seedChargedPFCand_dz  = cms.double(0.2),
  usePFJetSeeds             = cms.bool(True),                          
  min_seedPFJet_pt          = cms.double(30.),
  max_seedPFJet_eta         = cms.double(2.1),
  min_signalConeSize        = cms.double(0.05),
  max_signalConeSize        = cms.double(0.10),
  stripSize_eta             = cms.double(0.05),
  stripSize_phi             = cms.double(0.20),
  isolationConeSize         = cms.double(0.4),                      
  deltaR_cleaning           = cms.double(0.4),
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
  debug = cms.untracked.bool(False)                                  
)

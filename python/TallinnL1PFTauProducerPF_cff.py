import FWCore.ParameterSet.Config as cms

TallinnL1PFTauProducerPF = cms.EDProducer("TallinnL1PFTauProducer",
  srcL1PFCands              = cms.InputTag("l1pfCandidates:PF"),
  srcL1PFJets               = cms.InputTag("ak4PFL1PF"),                      
  srcL1Vertices             = cms.InputTag("VertexProducer:l1vertextdr"),
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
  min_PFTau_pt              = cms.double(20.),
  max_PFTau_eta             = cms.double(2.1),                                       
  min_leadChargedPFCand_pt  = cms.double(1.),
  max_leadChargedPFCand_eta = cms.double(2.1),
  max_leadChargedPFCand_dz  = cms.double(0.2),
  max_chargedIso            = cms.double(1.e+3),
  max_chargedRelIso         = cms.double(1.0),                                       
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
  applyPreselection = cms.bool(False),                      
  debug = cms.untracked.bool(False)                                  
)
import FWCore.ParameterSet.Config as cms

TallinnL1PFTauProducer = cms.EDProducer("TallinnL1PFTauProducer",
                                 debug           = cms.untracked.bool(False),
                                 deltaR          = cms.double(0.4),
                                 min_tauSeedPt   = cms.double(5),
                                 max_tauSeedEta  = cms.double(2.1),
                                 L1PFObjects     = cms.InputTag("L1PFProducer","L1PFObjects"),
                                 )

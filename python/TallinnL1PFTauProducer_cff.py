import FWCore.ParameterSet.Config as cms

TallinnL1PFTauProducer = cms.EDProducer("TallinnL1PFTauProducer",
                                 debug           = cms.untracked.bool(False),
                                 min_pi0pt       = cms.double(0),
                                 L1PFObjects     = cms.InputTag("L1PFProducer","L1PFObjects"),
                                 L1Neutrals      = cms.InputTag("L1PFProducer", "L1PFObjects")
                                 )

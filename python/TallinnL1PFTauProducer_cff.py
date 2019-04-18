import FWCore.ParameterSet.Config as cms

TallinnL1PFTauProducer = cms.EDProducer("TallinnL1PFTauProducer",
                                 debug           = cms.untracked.bool(False),
                                 deltaR          = cms.double(0.4),
                                 dz_cut          = cms.double(0.2),
                                 min_tauSeedPt   = cms.double(5),
                                 max_tauSeedEta  = cms.double(2.1),
                                 l1PFCandToken   = cms.InputTag("l1pfCandidates","PF"),
                                 vtxTagToken     = cms.InputTag("offlineSlimmedPrimaryVertices"),

                                 )

import FWCore.ParameterSet.Config as cms

TallinnL1PFTauAnalyzer_Minbias = cms.EDAnalyzer("TallinnL1PFTauAnalyzer_Minbias",
                                 treeName        = cms.string("L1PFTauAnalyzer"),
                                 l1PFTauToken    = cms.InputTag("TallinnL1PFTauProducer","L1PFTaus"),
                                 vtxTagToken     = cms.InputTag("offlineSlimmedPrimaryVertices"),
                                 )


AnalyzerSeq = cms.Sequence(
    TallinnL1PFTauAnalyzer_Minbias
)

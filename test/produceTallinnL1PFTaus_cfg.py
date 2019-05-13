# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step2 --python_filename=rerun_step2_L1_onMCL1_FEVTHLTDEBUG.py --no_exec -s L1 --datatier GEN-SIM-DIGI-RAW -n 1 --era Phase2_timing --eventcontent FEVTDEBUGHLT --filein file:/afs/cern.ch/user/r/rekovic/release/CMSSW_9_3_2/src/step2_DIGI_PU200_10ev.root --conditions 93X_upgrade2023_realistic_v2 --beamspot HLLHC14TeV --geometry Extended2023D17 --fileout file:step2_ZEE_PU200_1ev_rerun-L1-L1Ntuple.root --customise=L1Trigger/L1TNtuples/customiseL1Ntuple.L1NtupleEMU
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

from Configuration.ProcessModifiers.convertHGCalDigisSim_cff import convertHGCalDigisSim
process = cms.Process('Produce',eras.Phase2_trigger,convertHGCalDigisSim)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.Geometry.GeometryExtended2023D17Reco_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        #'file:/hdfs/local/sbhowmik/Sample/VBFHToTauTau_M125_14TeV_powheg_pythia8_correctedGridpack/PhaseIIMTDTDRAutumn18MiniAOD-PU200_103X_upgrade2023_realistic_v2-v1/MINIAODSIM/556D067D-7601-BD4D-A168-88D0A67428DB.root',
        'file:/home/veelken/CMSSW_10_5_0_pre1/src/L1Trigger/TallinnL1PFTaus/samples/556D067D-7601-BD4D-A168-88D0A67428DB.root', 
    ),
    secondaryFileNames = cms.untracked.vstring(
        #'file:/hdfs/local/sbhowmik/Sample/VBFHToTauTau_M125_14TeV_powheg_pythia8_correctedGridpack/PhaseIIMTDTDRAutumn18MiniAOD-PU200_103X_upgrade2023_realistic_v2-v1/FEVT/B12B9398-9DBA-D442-855D-BFEBCE64D269.root',
        #'file:/hdfs/local/sbhowmik/Sample/VBFHToTauTau_M125_14TeV_powheg_pythia8_correctedGridpack/PhaseIIMTDTDRAutumn18MiniAOD-PU200_103X_upgrade2023_realistic_v2-v1/FEVT/5AB0C82F-3846-4D45-A27F-A356DEB6C5DE.root',
        'file:/home/veelken/CMSSW_10_5_0_pre1/src/L1Trigger/TallinnL1PFTaus/samples/B12B9398-9DBA-D442-855D-BFEBCE64D269.root',
        'file:/home/veelken/CMSSW_10_5_0_pre1/src/L1Trigger/TallinnL1PFTaus/samples/5AB0C82F-3846-4D45-A27F-A356DEB6C5DE.root',
    ),
    inputCommands = cms.untracked.vstring("keep *", 
        "drop l1tHGCalTowerMapBXVector_hgcalTriggerPrimitiveDigiProducer_towerMap_HLT",
        "drop l1tEMTFHit2016Extras_simEmtfDigis_CSC_HLT",
        "drop l1tEMTFHit2016Extras_simEmtfDigis_RPC_HLT",
        "drop l1tEMTFHit2016s_simEmtfDigis__HLT",
        "drop l1tEMTFTrack2016Extras_simEmtfDigis__HLT",
        "drop l1tEMTFTrack2016s_simEmtfDigis__HLT"
    ),
    #skipEvents = cms.untracked.uint32(80),
    #eventsToProcess = cms.untracked.VEventRange(
    #    '1:364:36355'
    #)         
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('step2 nevts:1'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '100X_upgrade2023_realistic_v1', '')

# Sequence, Path and EndPath definitions
process.productionSequence = cms.Sequence()

process.load('SimCalorimetry.HcalTrigPrimProducers.hcaltpdigi_cff')
process.load('CalibCalorimetry.CaloTPG.CaloTPGTranscoder_cfi')

process.load('L1Trigger.L1THGCal.hgcalTriggerPrimitives_cff')
process.productionSequence += process.hgcalTriggerPrimitives

process.load('SimCalorimetry.EcalEBTrigPrimProducers.ecalEBTriggerPrimitiveDigis_cff')
process.productionSequence += process.simEcalEBTriggerPrimitiveDigis

process.load('L1Trigger.TrackFindingTracklet.L1TrackletTracks_cff')
process.productionSequence += process.L1TrackletTracksWithAssociators

process.load('L1Trigger.VertexFinder.VertexProducer_cff')
process.VertexProducer.l1TracksInputTag = cms.InputTag("TTTracksFromTracklet", "Level1TTTracks")

process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.productionSequence += process.SimL1Emulator

process.load("L1Trigger.Phase2L1ParticleFlow.pfTracksFromL1Tracks_cfi")
process.productionSequence += process.pfTracksFromL1Tracks

process.load("L1Trigger.Phase2L1ParticleFlow.l1ParticleFlow_cff")
process.productionSequence += process.l1ParticleFlow

process.load("L1Trigger.Phase2L1ParticleFlow.l1pfJetMet_cff")
process.productionSequence += process.l1PFJets

############################################################
# Generator-level (visible) hadronic taus
############################################################

process.load("PhysicsTools.JetMCAlgos.TauGenJets_cfi")
process.tauGenJets.GenParticles = cms.InputTag("prunedGenParticles")

process.load("PhysicsTools.JetMCAlgos.TauGenJetsDecayModeSelectorAllHadrons_cfi")

process.genTaus = cms.Sequence(process.tauGenJets + process.tauGenJetsSelectorAllHadrons)
process.productionSequence += process.genTaus

############################################################
# Tallinn L1 Tau object
############################################################

from L1Trigger.TallinnL1PFTaus.TallinnL1PFTauProducerPF_cff import TallinnL1PFTauProducerPF
process.TallinnL1PFTauProducerWithStripsPF = TallinnL1PFTauProducerPF.clone(
    useStrips = cms.bool(True),
    debug = cms.untracked.bool(False)
    #debug = cms.untracked.bool(True)
)
process.productionSequence += process.TallinnL1PFTauProducerWithStripsPF

process.TallinnL1PFTauProducerWithoutStripsPF = TallinnL1PFTauProducerPF.clone(
    useStrips = cms.bool(False),
    debug = cms.untracked.bool(False)
)
process.productionSequence += process.TallinnL1PFTauProducerWithoutStripsPF

from L1Trigger.TallinnL1PFTaus.TallinnL1PFTauProducerPuppi_cff import TallinnL1PFTauProducerPuppi
process.TallinnL1PFTauProducerWithStripsPuppi = TallinnL1PFTauProducerPuppi.clone(
    useStrips = cms.bool(True),
    debug = cms.untracked.bool(False)
)
process.productionSequence += process.TallinnL1PFTauProducerWithStripsPuppi

process.TallinnL1PFTauProducerWithoutStripsPuppi = TallinnL1PFTauProducerPuppi.clone(
    useStrips = cms.bool(False),
    debug = cms.untracked.bool(False)
)
process.productionSequence += process.TallinnL1PFTauProducerWithoutStripsPuppi

############################################################ 
# L1 Tau object 
############################################################ 

process.load("L1Trigger.Phase2L1Taus.L1PFTauProducer_cff")
process.L1PFTauProducer.debug = cms.untracked.bool(False)
process.L1PFTauProducer.L1PFObjects = cms.InputTag("l1pfCandidates:PF")
process.L1PFTauProducer.L1Neutrals = cms.InputTag("l1pfCandidates")
process.productionSequence += process.L1PFTauProducer

process.production_step = cms.Path(process.productionSequence)

process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string("NTuple_TallinnL1PFTauProducer.root"),                           
    #fileName = cms.untracked.string("NTuple_TallinnL1PFTauProducer_2019May13.root"),
    #fileName = cms.untracked.string("NTuple_TallinnL1PFTauProducer_DEBUG.root"),                           
    outputCommands = cms.untracked.vstring(
        'drop *_*_*_*',                                 
        'keep *_l1pfCandidates_PF_*',
        'keep *_l1pfCandidates_Puppi_*',
        'keep *_l1pfProducer*_z0_*',
        'keep *_pfTracksFromL1Tracks*_*_*',
        'keep *_TTTracksFromTracklet_*_*',
        'keep *_VertexProducer_*_*',                                
        'keep *_slimmedTaus_*_*',
        'keep *_packedPFCandidates_*_*',
        'keep *_generator_*_*',
        'keep *_caloStage2Digis_*_*',
        'keep *_TallinnL1PFTauProducerWithStripsPF_*_*',                           
        'keep *_TallinnL1PFTauProducerWithoutStripsPF_*_*',
        'keep *_TallinnL1PFTauProducerWithStripsPuppi_*_*',                            
        'keep *_TallinnL1PFTauProducerWithoutStripsPuppi_*_*',                           
        'keep *_prunedGenParticles_*_*',
        'keep *_tauGenJetsSelectorAllHadrons_*_*',
        'keep *_particleFlow_*_*',
        'keep *_generalTracks_*_*',
        'keep *_electronGsfTracks_*_*',
        'keep *_offlineSlimmedPrimaryVertices_*_*',                           
        'keep *_L1PFTauProducer_*_*',
    )
)
process.outpath = cms.EndPath(process.out)

process.endjob_step = cms.EndPath(process.endOfProcess)

# Schedule definition
process.schedule = cms.Schedule(process.production_step, process.outpath, process.endjob_step)

from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion

# Enable module run-time report
#process.options = cms.untracked.PSet(
#    wantSummary = cms.untracked.bool(True)
#)

dump_file = open('dump.py','w')
dump_file.write(process.dumpPython())



# l1trigger-phase2l1pftaus
L1Trigger/TallinnL1PFTaus package for HL-HLC L1Tau trigger studies

# To checkout the code:

cmsrel CMSSW_10_5_0_pre1 
cd CMSSW_10_5_0_pre1/src 
cmsenv 

git cms-init 

git cms-checkout-topic -u p2l1pfp:L1PF_10_5_X_v3

#git clone git@github.com:p2l1pfp/FastPUPPI.git -b 105X_v3

git clone https://github.com/p2l1pfp/FastPUPPI -b 105X_v3

cp /home/veelken/public/DataFormats/Phase2L1ParticleFlow/PFCandidate.h     $CMSSW_BASE/src/DataFormats/Phase2L1ParticleFlow/interface

cp /home/veelken/public/DataFormats/Phase2L1ParticleFlow/PFCandidateFwd.h  $CMSSW_BASE/src/DataFormats/Phase2L1ParticleFlow/interface

cp /home/veelken/public/DataFormats/Phase2L1ParticleFlow/PFJet.h           $CMSSW_BASE/src/DataFormats/Phase2L1ParticleFlow/interface

cp /home/veelken/public/DataFormats/Phase2L1ParticleFlow/PFJetFwd.h        $CMSSW_BASE/src/DataFormats/Phase2L1ParticleFlow/interface

cp /home/veelken/public/DataFormats/Phase2L1ParticleFlow/classes.h         $CMSSW_BASE/src/DataFormats/Phase2L1ParticleFlow/src

cp /home/veelken/public/DataFormats/Phase2L1ParticleFlow/classes_def.xml   $CMSSW_BASE/src/DataFormats/Phase2L1ParticleFlow/src

cp /home/veelken/public/DataFormats/L1TVertex/Vertex.h                     $CMSSW_BASE/src/DataFormats/L1TVertex/interface

cp /home/veelken/public/DataFormats/L1TVertex/VertexFwd.h                  $CMSSW_BASE/src/DataFormats/L1TVertex/interface

cp /home/veelken/public/DataFormats/L1TVertex/classes.h                    $CMSSW_BASE/src/DataFormats/L1TVertex/src

cp /home/veelken/public/DataFormats/L1TVertex/classes_def.xml              $CMSSW_BASE/src/DataFormats/L1TVertex/src

git clone https://github.com/HEP-KBFI/l1trigger-phase2l1pftaus             $CMSSW_BASE/src/L1Trigger/TallinnL1PFTaus 
git clone https://github.com/HEP-KBFI/dataformats-phase2l1pftaus           $CMSSW_BASE/src/DataFormats/TallinnL1PFTaus 

scram b -j 8

# To run the L1Tau Producer

cd L1Trigger/TallinnL1PFTaus/test/ 

cmsRun test_TallinnL1PFTauProducer.py 

# To modify the DataFormats 

cd DataFormats/TallinnL1PFTaus

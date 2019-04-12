#ifndef L1Trigger_Phase2L1PFTau_TallinnTauCluster_h
#define L1Trigger_Phase2L1PFTau_TallinnTauCluster_h

#include "DataFormats/TallinnL1PFTaus/interface/TallinnL1PFTau.h"
#include "DataFormats/Phase2L1ParticleFlow/interface/PFCandidate.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <unordered_map>

#define tracker_eta 3.5
#define tau_size_eta 6*0.08750 //6 towers
#define tau_size_phi 6*0.08726 //6 towers
#define tower_size 0.087

//namespace l1ttau_impl { 
class TallinnTauCluster;

typedef std::vector<TallinnTauCluster> TallinnTauClusterCollection;

class TallinnTauCluster{
      public:
  //TallinnTauCluster( const edm::ParameterSet& ) ;

	TallinnTauCluster();

	~TallinnTauCluster();

	l1t::TallinnL1PFTau l1PFTau;
	l1t::PFCandidate tauSeed;

	typedef struct
	{
	  float et = 0;
	  float eta = 0;
	  float phi = 0;
	} simple_object_t;
	

	void addPFCandidate(l1t::PFCandidate in);

	void process();

	float delta_r_cluster(simple_object_t cluster_1, simple_object_t cluster_2);

	float weighted_avg_phi(simple_object_t cluster_1, simple_object_t cluster_2);

	float weighted_avg_eta(simple_object_t cluster_1, simple_object_t cluster_2);

	bool tauSeedConeContains( l1t::PFCandidate in);

 private:

	l1t::PFCandidateCollection AllL1PFCandidates;
	l1t::PFCandidateCollection ChargedHadrons;
	l1t::PFCandidateCollection Electrons;
	l1t::PFCandidateCollection NeutralHadrons;
	l1t::PFCandidateCollection Photons;
	l1t::PFCandidateCollection Muons;

	float sumChargedIso;
};

#endif

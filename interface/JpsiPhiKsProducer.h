#ifndef __JpsiPhiKsProducer_h_
#define __JpsiPhiKsProducer_h_

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/PatCandidates/interface/CompositeCandidate.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "RecoVertex/KinematicFitPrimitives/interface/RefCountedKinematicParticle.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "DataFormats/Candidate/interface/VertexCompositePtrCandidate.h"

#include "CommonTools/Utils/interface/PtComparator.h"
#include "TMatrixDSym.h"
#include "TVectorD.h"

#include <vector>

class JpsiPhiKsProducer : public edm::EDProducer {
	
 public: 
  explicit JpsiPhiKsProducer(const edm::ParameterSet& ps);
 
 private:

  virtual void produce(edm::Event& event, const edm::EventSetup& esetup) override;
  virtual void endJob();

  const edm::EDGetTokenT<pat::CompositeCandidateCollection> OniaCollection_;
  const edm::EDGetTokenT<pat::PackedCandidateCollection> TrakCollection_;
  const edm::EDGetTokenT<reco::VertexCompositePtrCandidateCollection> kshortCollection_;
  std::vector<double> OniaMassCuts_;
  std::vector<double> CandidateMassCuts_;
  const double kaonTMass_;
  const double piTMass_;
  const double ConstraintMass_;

  const bool IsTheSame(const reco::Track& tk, const pat::Muon& mu);
//  const pat::CompositeCandidate makeCandidate(const pat::CompositeCandidate& onia, const reco::Track& track1, const reco::Track& track2);
//  const pat::CompositeCandidate FitVtx(const reco::Track &mu1, const reco::Track &mu2, RefCountedKinematicParticle thePhoton, const reco::Track &tk1, const reco::Track &tk2, const reco::Vertex &thePrimaryV, edm::ESHandle<TransientTrackBuilder> &theB);
  const RefCountedKinematicParticle FitKshort(const reco::Track *tk0, const reco::Track *tk1, edm::ESHandle<TransientTrackBuilder> &theB);
  //reco::Track fix_track(const reco::Track& tk);
  reco::Track fix_track(const reco::Track *tk);

  int candidates;
  int nevents;

  GreaterByPt<reco::Track> PtComparator;

  template<typename T>
  struct GreaterByVProb {
         typedef T first_argument_type;
         typedef T second_argument_type;
         bool operator()( const T & t1, const T & t2 ) const { return t1.userFloat("vProb") > t2.userFloat("vProb"); }
  };
};

#endif

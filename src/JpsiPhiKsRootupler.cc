#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/PatCandidates/interface/CompositeCandidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/PatCandidates/interface/UserData.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/TrackReco/interface/TrackBase.h"
#include "FWCore/Common/interface/TriggerNames.h"

#include "TMatrixDSym.h"
#include "TVectorD.h"
#include "TLorentzVector.h"
#include "TTree.h"
#include <vector>
#include <sstream>

class JpsiPhiKsRootupler : public edm::EDAnalyzer {
   public:
      explicit JpsiPhiKsRootupler(const edm::ParameterSet&);
      ~JpsiPhiKsRootupler() override {};
      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      void analyze(const edm::Event&, const edm::EventSetup&) override;

  std::string file_name;
  const edm::EDGetTokenT<pat::CompositeCandidateCollection> TheCandidateLabel;
  const edm::EDGetTokenT<pat::CompositeCandidateCollection> TheUpsLabel;
  const edm::EDGetTokenT<reco::VertexCollection>            ThePrimaryVertexLabel;
  const edm::EDGetTokenT<edm::TriggerResults>               TheTriggerResultLabel;
  const edm::EDGetTokenT<reco::GenParticleCollection>       TheGenParticleLabel;
  const double kaonTMass_;
  const double pionTMass_;
  const double DimuonMass_;
  const int  candidate_pdgid_, onia_pdgid_, dikaon_pdgid_, kaon1_pdgid_, kaon2_pdgid_, dipion_pdgid_, pion1_pdgid_, pion2_pdgid_;
  const bool isMC_,OnlyBest_;

  UInt_t run, event, nCandPerEvent, numPrimaryVertices, trigger;

  TLorentzVector candidate_p4;
  TLorentzVector dimuon_p4;
  TLorentzVector kaon1_p4;
  TLorentzVector kaon2_p4;
  TLorentzVector pion1_p4;
  TLorentzVector pion2_p4;
  TLorentzVector muonp_p4;
  TLorentzVector muonn_p4;
  TLorentzVector dikaon_p4;
  TLorentzVector dipion_p4;

  Int_t    candidate_charge, kaon1_nvsh, kaon1_nvph, kaon2_nvsh, kaon2_nvph, gen_candidate_charge;
  Double_t candidate_vMass, candidate_vProb,  candidate_vChi2, candidate_cosAlpha, candidate_ctauPV, candidate_ctauErrPV;
  Double_t candidate_cosAlpha3D, candidate_lxy, candidate_lxyErr, candidate_lxyz, candidate_lxyzErr;

  Double_t thePrimaryV_X, thePrimaryV_Y, thePrimaryV_Z, TheDecayVertex_X, TheDecayVertex_Y, TheDecayVertex_Z, thePrimaryV_2D_position, thePrimaryV_3D_position, TheDecayVertex_2D_position, TheDecayVertex_3D_position, TheVertexDistance_2D, TheVertexDistance_3D;

  Double_t kaon1_d0, kaon1_d0Err, kaon1_dz, kaon1_dxy, kaon1_dzErr, kaon1_dxyErr, kaon1_dzAssocPV;
  Double_t kaon2_d0, kaon2_d0Err, kaon2_dz, kaon2_dxy, kaon2_dzErr, kaon2_dxyErr, kaon2_dzAssocPV;
  Double_t dimuon_vProb, dimuon_vChi2, dimuon_DCA, dimuon_ctauPV, dimuon_ctauErrPV, dimuon_cosAlpha, dimuon_nSigma;
  Double_t dimuon_lxyPV, dimuon_lxyErrPV, dimuon_ctauBS, dimuon_ctauErrBS, dimuon_lxyBS, dimuon_lxyErrBS;
  Double_t dimuon_vMass;

  Double_t kaon2_dRdimuon, kaon1_dRdimuon, dikaon_dRdimuon;
  Int_t kaon1_charge, kaon2_charge;

  //Covariance stuff
  Double_t kaon1_SQopQop, kaon1_SQopLam, kaon1_SQopPhi, kaon1_SQopDxy, kaon1_SQopDsz, kaon1_SLamLam;
  Double_t kaon1_SLamPhi, kaon1_SLamDxy, kaon1_SLamDsz, kaon1_SPhiPhi, kaon1_SPhiDxy, kaon1_SPhiDsz;
  Double_t kaon1_SDxyDxy, kaon1_SDxyDsz, kaon1_SDszDsz, kaon1_minEigen;;

  Double_t kaon2_SQopQop, kaon2_SQopLam, kaon2_SQopPhi, kaon2_SQopDxy, kaon2_SQopDsz, kaon2_SLamLam;
  Double_t kaon2_SLamPhi, kaon2_SLamDxy, kaon2_SLamDsz, kaon2_SPhiPhi, kaon2_SPhiDxy, kaon2_SPhiDsz;
  Double_t kaon2_SDxyDxy, kaon2_SDxyDsz, kaon2_SDszDsz, kaon2_minEigen;

  Double_t vChi2KalmanKK,vNDFKalmanKK,vProbKalmanKK,vChi2KinematicKK,vNDFKinematicKK,vProbKinematicKK;

  Int_t kaon1_fromPV, kaon2_fromPV, kaon1_refVtx, kaon2_refVtx, kaon1_pvAssocQ, kaon2_pvAssocQ;

  Int_t dimuon_vertexWeight, iPVwithmuons, dimuon_diMuIndx;

   Double_t pion1_d0, pion1_d0Err, pion1_dz, pion1_dzErr, pion1_dxy, pion1_dxyErr, pion1_dzAssocPV;
   Double_t pion2_d0, pion2_d0Err, pion2_dz, pion2_dzErr, pion2_dxy, pion2_dxyErr, pion2_dzAssocPV;
   Int_t pion1_nvsh, pion1_nvph, pion1_charge, pion1_fromPV, pion1_refVtx, pion1_pvAssocQ, pion1_highPurity, pion1_hasTrackDetails;
   Int_t pion2_nvsh, pion2_nvph, pion2_charge, pion2_fromPV, pion2_refVtx, pion2_pvAssocQ, pion2_highPurity, pion2_hasTrackDetails;

  //Covariance stuff
  Double_t pion1_SQopQop, pion1_SQopLam, pion1_SQopPhi, pion1_SQopDxy, pion1_SQopDsz, pion1_SLamLam;
  Double_t pion1_SLamPhi, pion1_SLamDxy, pion1_SLamDsz, pion1_SPhiPhi, pion1_SPhiDxy, pion1_SPhiDsz;
  Double_t pion1_SDxyDxy, pion1_SDxyDsz, pion1_SDszDsz, pion1_minEigen;;

  Double_t pion2_SQopQop, pion2_SQopLam, pion2_SQopPhi, pion2_SQopDxy, pion2_SQopDsz, pion2_SLamLam;
  Double_t pion2_SLamPhi, pion2_SLamDxy, pion2_SLamDsz, pion2_SPhiPhi, pion2_SPhiDxy, pion2_SPhiDsz;
  Double_t pion2_SDxyDxy, pion2_SDxyDsz, pion2_SDszDsz, pion2_minEigen;


  TLorentzVector gen_candidate_p4;
  Int_t          gen_candidate_pdgId;
  TLorentzVector gen_dimuon_p4;
  Int_t          gen_onia_pdgId;
  TLorentzVector gen_muonp_p4;
  TLorentzVector gen_muonn_p4;
  TLorentzVector gen_dikaon_p4;
  TLorentzVector gen_kaon1_p4;
  Int_t          gen_kaon1_pdgid;
  TLorentzVector gen_kaon2_p4;
  Int_t          gen_kaon2_pdgid;
  TLorentzVector gen_dipion_p4;
  TLorentzVector gen_pion1_p4;
  Int_t          gen_pion1_pdgid;
  TLorentzVector gen_pion2_p4;
  Int_t          gen_pion2_pdgid;

  //TLorentzVector kaon1;
  //TLorentzVector kaon2;

  TLorentzVector ups_p4, muonP_p4, muonN_p4;
  Double_t ups_vMass, ups_vertexWeight, ups_vProb, ups_vChi2, ups_DCA, ups_ctauPV, ups_ctauErrPV, ups_cosAlpha;
  Double_t ups_lxyPV, ups_lxyErrPV, ups_ctauBS, ups_ctauErrBS, ups_lxyBS, ups_lxyErrBS;
  Double_t mu1_pt, mu1_ptErr, mu1_d0, mu1_d0Err, mu1_dz, mu1_dzErr, mu1_dxy, mu1_dxyErr, mu2_pt, mu2_ptErr, mu2_d0, mu2_d0Err, mu2_dz, mu2_dzErr, mu2_dxy, mu2_dxyErr;
  Int_t mu1_nvsh, mu1_nvph, mu2_nvsh, mu2_nvph, iPVwithmuons_ups, mu1_charge, mu2_charge;

  TTree* TheTree;
  TTree* JpsiTree;
  bool is_dimuon_;
};

JpsiPhiKsRootupler::JpsiPhiKsRootupler(const edm::ParameterSet& iConfig):
TheCandidateLabel(consumes<pat::CompositeCandidateCollection>(iConfig.getParameter < edm::InputTag > ("TheCandidates"))),
TheUpsLabel(consumes<pat::CompositeCandidateCollection>(iConfig.getParameter < edm::InputTag > ("TheUps"))),
ThePrimaryVertexLabel(consumes<reco::VertexCollection>(iConfig.getParameter < edm::InputTag > ("PrimaryVertices"))),
TheTriggerResultLabel(consumes<edm::TriggerResults>(iConfig.getParameter < edm::InputTag > ("TriggerResults"))),
TheGenParticleLabel(consumes<reco::GenParticleCollection>(iConfig.getParameter < edm::InputTag > ("GenParticles"))),
kaonTMass_(iConfig.getParameter<double>("kaonTMass")),
pionTMass_(iConfig.getParameter<double>("pionTMass")),
DimuonMass_(iConfig.getParameter<double>("DimuonMass")),
candidate_pdgid_(iConfig.getParameter<uint32_t>("candidate_pdgid")),
onia_pdgid_(iConfig.getParameter<uint32_t>("onia_pdgid")),
dikaon_pdgid_(iConfig.getParameter<uint32_t>("dikaon_pdgid")),
kaon1_pdgid_(iConfig.getParameter<int32_t>("kaon1_pdgid")),
kaon2_pdgid_(iConfig.getParameter<int32_t>("kaon2_pdgid")),
dipion_pdgid_(iConfig.getParameter<uint32_t>("dipion_pdgid")),
pion1_pdgid_(iConfig.getParameter<int32_t>("pion1_pdgid")),
pion2_pdgid_(iConfig.getParameter<int32_t>("pion2_pdgid")),
isMC_(iConfig.getParameter<bool>("isMC")),
OnlyBest_(iConfig.getParameter<bool>("OnlyBest"))
{
	     edm::Service<TFileService> fs;
        TheTree = fs->make<TTree>("CandidateTree","CandidateTree");

        TheTree->Branch("run",                &run,                "run/I");
        TheTree->Branch("event",              &event,              "event/I");
        TheTree->Branch("nCandPerEvent", &nCandPerEvent, "nCandPerEvent/I");
        TheTree->Branch("numPrimaryVertices", &numPrimaryVertices, "numPrimaryVertices/I");
        TheTree->Branch("trigger",            &trigger,            "trigger/I");

        TheTree->Branch("candidate_p4","TLorentzVector", &candidate_p4);
        TheTree->Branch("dimuon_p4",   "TLorentzVector", &dimuon_p4);
        TheTree->Branch("muonp_p4",    "TLorentzVector", &muonp_p4);
        TheTree->Branch("muonn_p4",    "TLorentzVector", &muonn_p4);
        TheTree->Branch("dikaon_p4",   "TLorentzVector", &dikaon_p4);
        TheTree->Branch("kaon1_p4",   "TLorentzVector", &kaon1_p4);
        TheTree->Branch("kaon2_p4",   "TLorentzVector", &kaon2_p4);
        TheTree->Branch("dipion_p4",   "TLorentzVector", &dipion_p4);
        TheTree->Branch("pion1_p4",   "TLorentzVector", &pion1_p4);
        TheTree->Branch("pion2_p4",   "TLorentzVector", &pion2_p4);

        TheTree->Branch("iPVwithmuons",        &iPVwithmuons,        "iPVwithmuons/I");

        TheTree->Branch("dimuon_diMuIndx",       &dimuon_diMuIndx,        "dimuon_diMuIndx/I");
        TheTree->Branch("dimuon_vertexWeight",        &dimuon_vertexWeight,        "dimuon_vertexWeight/D");
        TheTree->Branch("dimuon_vProb",        &dimuon_vProb,        "dimuon_vProb/D");
        TheTree->Branch("dimuon_vMass",        &dimuon_vMass,        "dimuon_vMass/D");
        TheTree->Branch("dimuon_vNChi2",       &dimuon_vChi2,        "dimuon_vNChi2/D");
        TheTree->Branch("dimuon_DCA",          &dimuon_DCA,          "dimuon_DCA/D");
        TheTree->Branch("dimuon_ctauPV",       &dimuon_ctauPV,       "dimuon_ctauPV/D");
        TheTree->Branch("dimuon_ctauErrPV",    &dimuon_ctauErrPV,    "dimuon_ctauErrPV/D");
        TheTree->Branch("dimuon_lxyPV",        &dimuon_lxyPV,          "dimuon_lxyPV/D");
        TheTree->Branch("dimuon_lxyErrPV",     &dimuon_lxyErrPV,       "dimuon_lxyErrPV/D");
        TheTree->Branch("dimuon_cosAlpha",     &dimuon_cosAlpha,     "dimuon_cosAlpha/D");
        TheTree->Branch("dimuon_ctauBS",       &dimuon_ctauBS,       "dimuon_ctauBS/D");
        TheTree->Branch("dimuon_ctauErrBS",    &dimuon_ctauErrBS,    "dimuon_ctauErrBS/D");
        TheTree->Branch("dimuon_lxyBS",        &dimuon_lxyBS,          "dimuon_lxyBS/D");
        TheTree->Branch("dimuon_lxyErrBS",     &dimuon_lxyErrBS,       "dimuon_lxyErrBS/D");

        TheTree->Branch("candidate_vMass",      &candidate_vMass,        "candidate_vMass/D");
        TheTree->Branch("candidate_vProb",      &candidate_vProb,        "candidate_vProb/D");
        TheTree->Branch("candidate_vChi2",      &candidate_vChi2,        "candidate_vChi2/D");
        TheTree->Branch("candidate_cosAlpha",   &candidate_cosAlpha,     "candidate_cosAlpha/D");
        TheTree->Branch("candidate_ctauPV",     &candidate_ctauPV,       "candidate_ctauPV/D");
        TheTree->Branch("candidate_ctauErrPV",  &candidate_ctauErrPV,    "candidate_ctauErrPV/D");
        TheTree->Branch("candidate_charge",     &candidate_charge,       "candidate_charge/I");
        TheTree->Branch("candidate_lxy",        &candidate_lxy,          "candidate_lxy/D");
        TheTree->Branch("candidate_lxyErr",     &candidate_lxyErr,       "candidate_lxyErr/D");
        TheTree->Branch("candidate_lxyz",       &candidate_lxyz,         "candidate_lxyz/D");
        TheTree->Branch("candidate_lxyzErr",    &candidate_lxyzErr,      "candidate_lxyzErr/D");

        TheTree->Branch("thePrimaryV_X",      &thePrimaryV_X,        "thePrimaryV_X/D");
        TheTree->Branch("thePrimaryV_Y",      &thePrimaryV_Y,        "thePrimaryV_Y/D");
        TheTree->Branch("thePrimaryV_Z",      &thePrimaryV_Z,        "thePrimaryV_Z/D");
        TheTree->Branch("TheDecayVertex_X",      &TheDecayVertex_X,        "TheDecayVertex_X/D");
        TheTree->Branch("TheDecayVertex_Y",      &TheDecayVertex_Y,        "TheDecayVertex_Y/D");
        TheTree->Branch("TheDecayVertex_Z",      &TheDecayVertex_Z,        "TheDecayVertex_Z/D");
        TheTree->Branch("thePrimaryV_2D_position",      &thePrimaryV_2D_position,        "thePrimaryV_2D_position/D");
        TheTree->Branch("thePrimaryV_3D_position",      &thePrimaryV_3D_position,        "thePrimaryV_3D_position/D");
        TheTree->Branch("TheDecayVertex_2D_position",      &TheDecayVertex_2D_position,        "TheDecayVertex_2D_position/D");
        TheTree->Branch("TheDecayVertex_3D_position",      &TheDecayVertex_3D_position,        "TheDecayVertex_3D_position/D");
        TheTree->Branch("TheVertexDistance_2D",      &TheVertexDistance_2D,        "TheVertexDistance_2D/D");
        TheTree->Branch("TheVertexDistance_3D",      &TheVertexDistance_3D,        "TheVertexDistance_3D/D");

        TheTree->Branch("kaon1_d0",    &kaon1_d0,    "kaon1_d0/D");
        TheTree->Branch("kaon1_d0Err", &kaon1_d0Err, "kaon1_d0Err/D");
        TheTree->Branch("kaon1_dz",    &kaon1_dz,    "kaon1_dz/D");
        TheTree->Branch("kaon1_dzErr",    &kaon1_dzErr,    "kaon1_dzErr/D");
        TheTree->Branch("kaon1_dxy",   &kaon1_dxy,   "kaon1_dxy/D");
        TheTree->Branch("kaon1_dxyErr",   &kaon1_dxyErr,   "kaon1_dxyErr/D");
        TheTree->Branch("kaon1_nvsh",  &kaon1_nvsh,  "kaon1_nvsh/I");
        TheTree->Branch("kaon1_nvph",  &kaon1_nvph,  "kaon1_nvph/I");
        TheTree->Branch("kaon1_dRdimuon",  &kaon1_dRdimuon,  "kaon1_dRdimuon/D");
        TheTree->Branch("kaon1_charge",  &kaon1_charge,  "kaon1_charge/I");
        TheTree->Branch("kaon1_fromPV",  &kaon1_fromPV,  "kaon1_fromPV/I");
        TheTree->Branch("kaon1_refVtx",  &kaon1_refVtx,  "kaon1_refVtx/I");
        TheTree->Branch("kaon1_pvAssocQ",  &kaon1_pvAssocQ,  "kaon1_pvAssocQ/I");
        TheTree->Branch("kaon1_dzAssocPV",    &kaon1_dzAssocPV,    "kaon1_dzAssocPV/D");

        TheTree->Branch("kaon2_d0",    &kaon2_d0,    "kaon2_d0/D");
        TheTree->Branch("kaon2_d0Err", &kaon2_d0Err, "kaon2_d0Err/D");
        TheTree->Branch("kaon2_dz",    &kaon2_dz,    "kaon2_dz/D");
        TheTree->Branch("kaon2_dzErr",    &kaon2_dzErr,    "kaon2_dzErr/D");
        TheTree->Branch("kaon2_dxy",   &kaon2_dxy,   "kaon2_dxy/D");
        TheTree->Branch("kaon2_dxyErr",   &kaon2_dxyErr,   "kaon2_dxyErr/D");
        TheTree->Branch("kaon2_nvsh",  &kaon2_nvsh,  "kaon2_nvsh/I");
        TheTree->Branch("kaon2_nvph",  &kaon2_nvph,  "kaon2_nvph/I");
        TheTree->Branch("kaon2_dRdimuon",  &kaon2_dRdimuon,  "kaon2_dRdimuon/D");
        TheTree->Branch("kaon2_charge",  &kaon2_charge,  "kaon2_charge/I");
        TheTree->Branch("kaon2_fromPV",  &kaon2_fromPV,  "kaon2_fromPV/I");
        TheTree->Branch("kaon2_refVtx",  &kaon2_refVtx,  "kaon2_refVtx/I");
        TheTree->Branch("kaon2_pvAssocQ",  &kaon2_pvAssocQ,  "kaon2_pvAssocQ/I");
        TheTree->Branch("kaon2_dzAssocPV",    &kaon2_dzAssocPV,    "kaon2_dzAssocPV/D");

        TheTree->Branch("dikaon_dRdimuon",  &dikaon_dRdimuon,  "dikaon_dRdimuon/D");

        TheTree->Branch("kaon1_SQopQop",        &kaon1_SQopQop,        "kaon1_SQopQop/D");
        TheTree->Branch("kaon1_SQopLam",        &kaon1_SQopLam,        "kaon1_SQopLam/D");
        TheTree->Branch("kaon1_SQopPhi",        &kaon1_SQopPhi,        "kaon1_SQopPhi/D");
        TheTree->Branch("kaon1_SQopDxy",        &kaon1_SQopDxy,        "kaon1_SQopDxy/D");
        TheTree->Branch("kaon1_SQopDsz",        &kaon1_SQopDsz,        "kaon1_SQopDsz/D");

        TheTree->Branch("kaon1_SLamLam",        &kaon1_SLamLam,        "kaon1_SLamLam/D");
        TheTree->Branch("kaon1_SLamPhi",        &kaon1_SLamPhi,        "kaon1_SLamPhi/D");
        TheTree->Branch("kaon1_SLamDxy",        &kaon1_SLamDxy,        "kaon1_SLamDxy/D");
        TheTree->Branch("kaon1_SLamDsz",        &kaon1_SLamDsz,        "kaon1_SLamDsz/D");
        TheTree->Branch("kaon1_SPhiPhi",        &kaon1_SPhiPhi,        "kaon1_SPhiPhi/D");

        TheTree->Branch("kaon1_SPhiDxy",        &kaon1_SPhiDxy,        "kaon1_SPhiDxy/D");
        TheTree->Branch("kaon1_SPhiDsz",        &kaon1_SPhiDsz,        "kaon1_SPhiDsz/D");
        TheTree->Branch("kaon1_SDxyDxy",        &kaon1_SDxyDxy,        "kaon1_SDxyDxy/D");
        TheTree->Branch("kaon1_SDxyDsz",        &kaon1_SDxyDsz,        "kaon1_SDxyDsz/D");
        TheTree->Branch("kaon1_SDszDsz",        &kaon1_SDszDsz,        "kaon1_SDszDsz/D");

        TheTree->Branch("kaon2_SQopQop",        &kaon2_SQopQop,        "kaon2_SQopQop/D");
        TheTree->Branch("kaon2_SQopLam",        &kaon2_SQopLam,        "kaon2_SQopLam/D");
        TheTree->Branch("kaon2_SQopPhi",        &kaon2_SQopPhi,        "kaon2_SQopPhi/D");
        TheTree->Branch("kaon2_SQopDxy",        &kaon2_SQopDxy,        "kaon2_SQopDxy/D");
        TheTree->Branch("kaon2_SQopDsz",        &kaon2_SQopDsz,        "kaon2_SQopDsz/D");

        TheTree->Branch("kaon2_SLamLam",        &kaon2_SLamLam,        "kaon2_SLamLam/D");
        TheTree->Branch("kaon2_SLamPhi",        &kaon2_SLamPhi,        "kaon2_SLamPhi/D");
        TheTree->Branch("kaon2_SLamDxy",        &kaon2_SLamDxy,        "kaon2_SLamDxy/D");
        TheTree->Branch("kaon2_SLamDsz",        &kaon2_SLamDsz,        "kaon2_SLamDsz/D");
        TheTree->Branch("kaon2_SPhiPhi",        &kaon2_SPhiPhi,        "kaon2_SPhiPhi/D");

        TheTree->Branch("kaon2_SPhiDxy",        &kaon2_SPhiDxy,        "kaon2_SPhiDxy/D");
        TheTree->Branch("kaon2_SPhiDsz",        &kaon2_SPhiDsz,        "kaon2_SPhiDsz/D");
        TheTree->Branch("kaon2_SDxyDxy",        &kaon2_SDxyDxy,        "kaon2_SDxyDxy/D");
        TheTree->Branch("kaon2_SDxyDsz",        &kaon2_SDxyDsz,        "kaon2_SDxyDsz/D");
        TheTree->Branch("kaon2_SDszDsz",        &kaon2_SDszDsz,        "kaon2_SDszDsz/D");

        TheTree->Branch("vChi2KalmanKK",        &vChi2KalmanKK,        "vChi2KalmanKK/D");
        TheTree->Branch("vNDFKalmanKK",        &vNDFKalmanKK,        "vNDFKalmanKK/D");
        TheTree->Branch("vProbKalmanKK",        &vProbKalmanKK,        "vProbKalmanKK/D");
        TheTree->Branch("vChi2KinematicKK",        &vChi2KinematicKK,        "vChi2KinematicKK/D");
        TheTree->Branch("vNDFKinematicKK",        &vNDFKinematicKK,        "vNDFKinematicKK/D");
        TheTree->Branch("vProbKinematicKK",        &vProbKinematicKK,        "vProbKinematicKK/D");

        TheTree->Branch("pion1_d0",    &pion1_d0,    "pion1_d0/D");
        TheTree->Branch("pion1_d0Err", &pion1_d0Err, "pion1_d0Err/D");
        TheTree->Branch("pion1_dz",    &pion1_dz,    "pion1_dz/D");
        TheTree->Branch("pion1_dzErr",    &pion1_dzErr,    "pion1_dzErr/D");
        TheTree->Branch("pion1_dxy",   &pion1_dxy,   "pion1_dxy/D");
        TheTree->Branch("pion1_dxyErr",   &pion1_dxyErr,   "pion1_dxyErr/D");
        TheTree->Branch("pion1_nvsh",  &pion1_nvsh,  "pion1_nvsh/I");
        TheTree->Branch("pion1_nvph",  &pion1_nvph,  "pion1_nvph/I");
        TheTree->Branch("pion1_charge",  &pion1_charge,  "pion1_charge/I");
        TheTree->Branch("pion1_fromPV",  &pion1_fromPV,  "pion1_fromPV/I");
        TheTree->Branch("pion1_refVtx",  &pion1_refVtx,  "pion1_refVtx/I");
        TheTree->Branch("pion1_pvAssocQ",  &pion1_pvAssocQ,  "pion1_pvAssocQ/I");
        TheTree->Branch("pion1_dzAssocPV",    &pion1_dzAssocPV,    "pion1_dzAssocPV/D");
        TheTree->Branch("pion1_highPurity",  &pion1_highPurity,  "pion1_highPurity/I");
        TheTree->Branch("pion1_hasTrackDetails",  &pion1_hasTrackDetails,  "pion1_hasTrackDetails/I");

        TheTree->Branch("pion2_d0",    &pion2_d0,    "pion2_d0/D");
        TheTree->Branch("pion2_d0Err", &pion2_d0Err, "pion2_d0Err/D");
        TheTree->Branch("pion2_dz",    &pion2_dz,    "pion2_dz/D");
        TheTree->Branch("pion2_dzErr",    &pion2_dzErr,    "pion2_dzErr/D");
        TheTree->Branch("pion2_dxy",   &pion2_dxy,   "pion2_dxy/D");
        TheTree->Branch("pion2_dxyErr",   &pion2_dxyErr,   "pion2_dxyErr/D");
        TheTree->Branch("pion2_nvsh",  &pion2_nvsh,  "pion2_nvsh/I");
        TheTree->Branch("pion2_nvph",  &pion2_nvph,  "pion2_nvph/I");
        TheTree->Branch("pion2_charge",  &pion2_charge,  "pion2_charge/I");
        TheTree->Branch("pion2_fromPV",  &pion2_fromPV,  "pion2_fromPV/I");
        TheTree->Branch("pion2_refVtx",  &pion2_refVtx,  "pion2_refVtx/I");
        TheTree->Branch("pion2_pvAssocQ",  &pion2_pvAssocQ,  "pion2_pvAssocQ/I");
        TheTree->Branch("pion2_dzAssocPV",    &pion2_dzAssocPV,    "pion2_dzAssocPV/D");
        TheTree->Branch("pion2_highPurity",  &pion2_highPurity,  "pion2_highPurity/I");
        TheTree->Branch("pion2_hasTrackDetails",  &pion2_hasTrackDetails,  "pion2_hasTrackDetails/I");

        TheTree->Branch("pion1_SQopQop",        &pion1_SQopQop,        "pion1_SQopQop/D");
        TheTree->Branch("pion1_SQopLam",        &pion1_SQopLam,        "pion1_SQopLam/D");
        TheTree->Branch("pion1_SQopPhi",        &pion1_SQopPhi,        "pion1_SQopPhi/D");
        TheTree->Branch("pion1_SQopDxy",        &pion1_SQopDxy,        "pion1_SQopDxy/D");
        TheTree->Branch("pion1_SQopDsz",        &pion1_SQopDsz,        "pion1_SQopDsz/D");

        TheTree->Branch("pion1_SLamLam",        &pion1_SLamLam,        "pion1_SLamLam/D");
        TheTree->Branch("pion1_SLamPhi",        &pion1_SLamPhi,        "pion1_SLamPhi/D");
        TheTree->Branch("pion1_SLamDxy",        &pion1_SLamDxy,        "pion1_SLamDxy/D");
        TheTree->Branch("pion1_SLamDsz",        &pion1_SLamDsz,        "pion1_SLamDsz/D");
        TheTree->Branch("pion1_SPhiPhi",        &pion1_SPhiPhi,        "pion1_SPhiPhi/D");

        TheTree->Branch("pion1_SPhiDxy",        &pion1_SPhiDxy,        "pion1_SPhiDxy/D");
        TheTree->Branch("pion1_SPhiDsz",        &pion1_SPhiDsz,        "pion1_SPhiDsz/D");
        TheTree->Branch("pion1_SDxyDxy",        &pion1_SDxyDxy,        "pion1_SDxyDxy/D");
        TheTree->Branch("pion1_SDxyDsz",        &pion1_SDxyDsz,        "pion1_SDxyDsz/D");
        TheTree->Branch("pion1_SDszDsz",        &pion1_SDszDsz,        "pion1_SDszDsz/D");

        TheTree->Branch("pion2_SQopQop",        &pion2_SQopQop,        "pion2_SQopQop/D");
        TheTree->Branch("pion2_SQopLam",        &pion2_SQopLam,        "pion2_SQopLam/D");
        TheTree->Branch("pion2_SQopPhi",        &pion2_SQopPhi,        "pion2_SQopPhi/D");
        TheTree->Branch("pion2_SQopDxy",        &pion2_SQopDxy,        "pion2_SQopDxy/D");
        TheTree->Branch("pion2_SQopDsz",        &pion2_SQopDsz,        "pion2_SQopDsz/D");

        TheTree->Branch("pion2_SLamLam",        &pion2_SLamLam,        "pion2_SLamLam/D");
        TheTree->Branch("pion2_SLamPhi",        &pion2_SLamPhi,        "pion2_SLamPhi/D");
        TheTree->Branch("pion2_SLamDxy",        &pion2_SLamDxy,        "pion2_SLamDxy/D");
        TheTree->Branch("pion2_SLamDsz",        &pion2_SLamDsz,        "pion2_SLamDsz/D");
        TheTree->Branch("pion2_SPhiPhi",        &pion2_SPhiPhi,        "pion2_SPhiPhi/D");

        TheTree->Branch("pion2_SPhiDxy",        &pion2_SPhiDxy,        "pion2_SPhiDxy/D");
        TheTree->Branch("pion2_SPhiDsz",        &pion2_SPhiDsz,        "pion2_SPhiDsz/D");
        TheTree->Branch("pion2_SDxyDxy",        &pion2_SDxyDxy,        "pion2_SDxyDxy/D");
        TheTree->Branch("pion2_SDxyDsz",        &pion2_SDxyDsz,        "pion2_SDxyDsz/D");
        TheTree->Branch("pion2_SDszDsz",        &pion2_SDszDsz,        "pion2_SDszDsz/D");

	if(isMC_)
	  {
      TheTree->Branch("gen_candidate_pdgId", &gen_candidate_pdgId, "gen_candidate_pdgId/I");
      TheTree->Branch("gen_candidate_p4","TLorentzVector", &gen_candidate_p4);
      TheTree->Branch("gen_candidate_charge",     &gen_candidate_charge,       "gen_candidate_charge/I");
      TheTree->Branch("gen_dimuon_p4",   "TLorentzVector", &gen_dimuon_p4);
      TheTree->Branch("gen_onia_pdgId",      &gen_onia_pdgId,      "gen_onia_pdgId/I");
      if (dikaon_pdgid_) TheTree->Branch("gen_dikaon_p4",   "TLorentzVector", &gen_dikaon_p4);
	    TheTree->Branch("gen_kaon1_p4",   "TLorentzVector", &gen_kaon1_p4);
      TheTree->Branch("gen_kaon1_pdgid", &gen_kaon1_pdgid, "gen_kaon1_pdgid/I");
      TheTree->Branch("gen_kaon2_p4",   "TLorentzVector", &gen_kaon2_p4);
      TheTree->Branch("gen_kaon2_pdgid", &gen_kaon2_pdgid, "gen_kaon2_pdgid/I");
      if (dipion_pdgid_) TheTree->Branch("gen_dipion_p4",   "TLorentzVector", &gen_dipion_p4);
      TheTree->Branch("gen_pion1_p4",   "TLorentzVector", &gen_pion1_p4);
      TheTree->Branch("gen_pion1_pdgid", &gen_pion1_pdgid, "gen_pion1_pdgid/I");
      TheTree->Branch("gen_pion2_p4",   "TLorentzVector", &gen_pion2_p4);
      TheTree->Branch("gen_pion2_pdgid", &gen_pion2_pdgid, "gen_pion2_pdgid/I");
      TheTree->Branch("gen_muonp_p4",    "TLorentzVector", &gen_muonp_p4);
      TheTree->Branch("gen_muonn_p4",    "TLorentzVector", &gen_muonn_p4);
	  }

        JpsiTree = fs->make<TTree>("JpsiTree","JpsiTree");

        JpsiTree->Branch("run",                &run,                "run/I");
        JpsiTree->Branch("event",              &event,              "event/I");
        JpsiTree->Branch("numPrimaryVertices", &numPrimaryVertices, "numPrimaryVertices/I");
        JpsiTree->Branch("trigger",            &trigger,            "trigger/I");
        JpsiTree->Branch("ups_p4",   "TLorentzVector", &ups_p4);
        JpsiTree->Branch("muonP_p4",    "TLorentzVector", &muonP_p4);
        JpsiTree->Branch("muonN_p4",    "TLorentzVector", &muonN_p4);

        JpsiTree->Branch("iPVwithmuons_ups",        &iPVwithmuons_ups,        "iPVwithmuons_ups/I");

        JpsiTree->Branch("ups_vertexWeight",        &ups_vertexWeight,        "ups_vertexWeight/D");
        JpsiTree->Branch("ups_vProb",        &ups_vProb,        "ups_vProb/D");
        JpsiTree->Branch("ups_vMass",        &ups_vMass,        "ups_vMass/D");
        JpsiTree->Branch("ups_vNChi2",       &ups_vChi2,        "ups_vNChi2/D");
        JpsiTree->Branch("ups_DCA",          &ups_DCA,          "ups_DCA/D");
        JpsiTree->Branch("ups_ctauPV",       &ups_ctauPV,       "ups_ctauPV/D");
        JpsiTree->Branch("ups_ctauErrPV",    &ups_ctauErrPV,    "ups_ctauErrPV/D");
        JpsiTree->Branch("ups_lxyPV",        &ups_lxyPV,          "ups_lxyPV/D");
        JpsiTree->Branch("ups_lxyErrPV",     &ups_lxyErrPV,       "ups_lxyErrPV/D");
        JpsiTree->Branch("ups_cosAlpha",     &ups_cosAlpha,     "ups_cosAlpha/D");
        JpsiTree->Branch("ups_ctauBS",       &ups_ctauBS,       "ups_ctauBS/D");
        JpsiTree->Branch("ups_ctauErrBS",    &ups_ctauErrBS,    "ups_ctauErrBS/D");
        JpsiTree->Branch("ups_lxyBS",        &ups_lxyBS,          "ups_lxyBS/D");
        JpsiTree->Branch("ups_lxyErrBS",     &ups_lxyErrBS,       "ups_lxyErrBS/D");

        JpsiTree->Branch("mu1_pt",    &mu1_pt,    "mu1_pt/D");
        JpsiTree->Branch("mu1_ptErr",    &mu1_ptErr,    "mu1_ptErr/D");
        JpsiTree->Branch("mu1_d0",    &mu1_d0,    "mu1_d0/D");
        JpsiTree->Branch("mu1_d0Err", &mu1_d0Err, "mu1_d0Err/D");
        JpsiTree->Branch("mu1_dz",    &mu1_dz,    "mu1_dz/D");
        JpsiTree->Branch("mu1_dzErr",    &mu1_dzErr,    "mu1_dzErr/D");
        JpsiTree->Branch("mu1_dxy",   &mu1_dxy,   "mu1_dxy/D");
        JpsiTree->Branch("mu1_dxyErr",   &mu1_dxyErr,   "mu1_dxyErr/D");
        JpsiTree->Branch("mu1_nvsh",  &mu1_nvsh,  "mu1_nvsh/I");
        JpsiTree->Branch("mu1_nvph",  &mu1_nvph,  "mu1_nvph/I");
        JpsiTree->Branch("mu1_charge",  &mu1_charge,  "mu1_charge/I");

        JpsiTree->Branch("mu2_pt",    &mu2_pt,    "mu2_pt/D");
        JpsiTree->Branch("mu2_ptErr",    &mu2_ptErr,    "mu2_ptErr/D");
        JpsiTree->Branch("mu2_d0",    &mu2_d0,    "mu2_d0/D");
        JpsiTree->Branch("mu2_d0Err", &mu2_d0Err, "mu2_d0Err/D");
        JpsiTree->Branch("mu2_dz",    &mu2_dz,    "mu2_dz/D");
        JpsiTree->Branch("mu2_dzErr",    &mu2_dzErr,    "mu2_dzErr/D");
        JpsiTree->Branch("mu2_dxy",   &mu2_dxy,   "mu2_dxy/D");
        JpsiTree->Branch("mu2_dxyErr",   &mu2_dxyErr,   "mu2_dxyErr/D");
        JpsiTree->Branch("mu2_nvsh",  &mu2_nvsh,  "mu2_nvsh/I");
        JpsiTree->Branch("mu2_nvph",  &mu2_nvph,  "mu2_nvph/I");
        JpsiTree->Branch("mu2_charge",  &mu2_charge,  "mu2_charge/I");

}

void JpsiPhiKsRootupler::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  using namespace std;

  edm::Handle < pat::CompositeCandidateCollection >TheCandidates;
  iEvent.getByToken(TheCandidateLabel, TheCandidates);

  edm::Handle < pat::CompositeCandidateCollection >TheUps;
  iEvent.getByToken(TheUpsLabel, TheUps);

  edm::Handle < reco::VertexCollection  >ThePrimaryVertices;
  iEvent.getByToken(ThePrimaryVertexLabel, ThePrimaryVertices);

  edm::Handle < edm::TriggerResults > triggerResults_handle;
  iEvent.getByToken(TheTriggerResultLabel, triggerResults_handle);

  numPrimaryVertices = ThePrimaryVertices->size();
  run = iEvent.id().run();
  event = iEvent.id().event();

  gen_dikaon_p4.SetPtEtaPhiM(0,0,0,0);

  if ( isMC_ ) {
    edm::Handle<reco::GenParticleCollection> GenParticles;
    iEvent.getByToken(TheGenParticleLabel, GenParticles);
    int foundit = 0;
    //int gen_kaon1_pdgid_ = 0;
    //int gen_kaon2_pdgid_ = 0;
    gen_candidate_pdgId = 0;
    if (GenParticles.isValid() ) {
      for ( reco::GenParticleCollection::const_iterator itParticle = GenParticles->begin(); itParticle != GenParticles->end(); ++itParticle ) {
        int pdgId = itParticle->pdgId();
        if ( abs(pdgId) ==  candidate_pdgid_ ) {
          //const reco::Candidate* gen_y2s = itParticle
          gen_candidate_charge = itParticle->charge();
          gen_candidate_p4.SetPtEtaPhiM(itParticle->pt(),itParticle->eta(),itParticle->phi(),itParticle->mass());
          gen_candidate_pdgId = pdgId;
          foundit++;
          for (uint i = 0; i < itParticle->numberOfDaughters(); ++i) {
            const reco::Candidate* b = itParticle->daughter(i);
            int bpdgid = b->pdgId();
            if ( abs(bpdgid) == onia_pdgid_ && b->status() == 2 ) {
               gen_onia_pdgId = bpdgid;
               gen_dimuon_p4.SetPtEtaPhiM(b->pt(),b->eta(),b->phi(),b->mass());
               foundit++;
               for (uint j = 0; j < b->numberOfDaughters(); ++j) {
                  const reco::Candidate* p = b->daughter(j);
                  if ( p->pdgId() == -13 && p->status() == 1 ) {
                     gen_muonp_p4.SetPtEtaPhiM(p->pt(),p->eta(),p->phi(),p->mass());
                     foundit++;
                  }
                  if ( p->pdgId() == 13 && p->status() == 1 ) {
                     gen_muonn_p4.SetPtEtaPhiM(p->pt(),p->eta(),p->phi(),p->mass());
                     foundit++;
                  }
               }
            }
            if (dikaon_pdgid_ && abs(bpdgid) == dikaon_pdgid_) {
              gen_dikaon_p4.SetPtEtaPhiM(b->pt(),b->eta(),b->phi(),b->mass());
              for (uint k = 0; k < b->numberOfDaughters(); ++k) {
                const reco::Candidate* p = b->daughter(k);
                if ( p->pdgId() == kaon1_pdgid_ && p->status() == 1 ) {
                   //std::cout<<" in dipion - pion 1 "<<std::endl;
                   //kaon1.SetPtEtaPhiM(p->pt(),p->eta(),p->phi(),p->mass());
                   //gen_kaon1_pdgid_ = kaon1_pdgid_;
                   gen_kaon1_p4.SetPtEtaPhiM(p->pt(),p->eta(),p->phi(),p->mass());
                   gen_kaon1_pdgid = kaon1_pdgid_;
                   foundit++;
                }
                if ( p->pdgId() == kaon2_pdgid_ && p->status() == 1 ) {
                   //std::cout<<" in dipion - pion 2 "<<std::endl;
                   //kaon2.SetPtEtaPhiM(p->pt(),p->eta(),p->phi(),p->mass());
                   //gen_kaon2_pdgid_ = kaon2_pdgid_;
                   gen_kaon2_p4.SetPtEtaPhiM(p->pt(),p->eta(),p->phi(),p->mass());
                   gen_kaon2_pdgid = kaon2_pdgid_;
                   foundit++;
                }
              }
            } else {
              if ( bpdgid == kaon1_pdgid_ && b->status() == 1 ) {
                //std::cout<<" pion 1 "<<std::endl;
                //kaon1.SetPtEtaPhiM(b->pt(),b->eta(),b->phi(),b->mass());
                //gen_kaon1_pdgid_ = kaon1_pdgid_;
                gen_kaon1_p4.SetPtEtaPhiM(b->pt(),b->eta(),b->phi(),b->mass());
                gen_kaon1_pdgid = kaon1_pdgid_;
                foundit++;
              }
              if ( bpdgid == kaon2_pdgid_ && b->status() == 1 ) {
                //std::cout<<" pion 2 "<<std::endl;
                //kaon2.SetPtEtaPhiM(b->pt(),b->eta(),b->phi(),b->mass());
                //gen_kaon2_pdgid_ = kaon2_pdgid_;
                gen_kaon2_p4.SetPtEtaPhiM(b->pt(),b->eta(),b->phi(),b->mass());
                gen_kaon2_pdgid = kaon2_pdgid_;
                foundit++;
              }
            }
            if (dipion_pdgid_ && abs(bpdgid) == dipion_pdgid_) {
              gen_dipion_p4.SetPtEtaPhiM(b->pt(),b->eta(),b->phi(),b->mass());
              for (uint k = 0; k < b->numberOfDaughters(); ++k) {
                const reco::Candidate* p = b->daughter(k);
                if ( p->pdgId() == pion1_pdgid_ && p->status() == 1 ) {
                   //std::cout<<" in dipion - pion 1 "<<std::endl;
                   //pion1.SetPtEtaPhiM(p->pt(),p->eta(),p->phi(),p->mass());
                   //gen_pion1_pdgid_ = pion1_pdgid_;
                   gen_pion1_p4.SetPtEtaPhiM(p->pt(),p->eta(),p->phi(),p->mass());
                   gen_pion1_pdgid = pion1_pdgid_;
                   foundit++;
                }
                if ( p->pdgId() == pion2_pdgid_ && p->status() == 1 ) {
                   //std::cout<<" in dipion - pion 2 "<<std::endl;
                   //pion2.SetPtEtaPhiM(p->pt(),p->eta(),p->phi(),p->mass());
                   //gen_pion2_pdgid_ = pion2_pdgid_;
                   gen_pion2_p4.SetPtEtaPhiM(p->pt(),p->eta(),p->phi(),p->mass());
                   gen_pion2_pdgid = pion2_pdgid_;
                   foundit++;
                }
              }
            } else {
              if ( bpdgid == pion1_pdgid_ && b->status() == 1 ) {
                //std::cout<<" pion 1 "<<std::endl;
                //pion1.SetPtEtaPhiM(b->pt(),b->eta(),b->phi(),b->mass());
                //gen_pion1_pdgid_ = pion1_pdgid_;
                gen_pion1_p4.SetPtEtaPhiM(b->pt(),b->eta(),b->phi(),b->mass());
                gen_pion1_pdgid = pion1_pdgid_;
                foundit++;
              }
              if ( bpdgid == pion2_pdgid_ && b->status() == 1 ) {
                //std::cout<<" pion 2 "<<std::endl;
                //pion2.SetPtEtaPhiM(b->pt(),b->eta(),b->phi(),b->mass());
                //gen_pion2_pdgid_ = pion2_pdgid_;
                gen_pion2_p4.SetPtEtaPhiM(b->pt(),b->eta(),b->phi(),b->mass());
                gen_pion2_pdgid = pion2_pdgid_;
                foundit++;
              }
            }
          }
          if ( foundit == 8 ) break;
          else {
            foundit = 0;
            gen_candidate_pdgId = 0;
          }
        } // if ( abs(
      }   // for ( reco
    }
    if (!gen_candidate_pdgId) {std::cout << "OniaRecokaonkaonRootupler: didn't find the given decay " << run << "," << event << std::endl;} /*else {std::cout << "I found it yupppiiiii " << run << "," << event << std::endl; }*/
  } // end if isMC

   trigger = 0;
   if (triggerResults_handle.isValid()) {
      const edm::TriggerNames & TheTriggerNames = iEvent.triggerNames(*triggerResults_handle);
      unsigned int NTRIGGERS = 13;
      std::string TriggersToTest[NTRIGGERS] = {
        "HLT_Dimuon20_Jpsi_Barrel_Seagulls","HLT_Dimuon25_Jpsi",
        "HLT_Dimuon10_PsiPrime_Barrel_Seagulls","HLT_Dimuon18_PsiPrime",
        "HLT_Dimuon10_Upsilon_Barrel_Seagulls","HLT_Dimuon12_Upsilon_eta1p5",
        "HLT_Dimuon14_Phi_Barrel_Seagulls","HLT_Dimuon12_Upsilon_y1p4",
        "HLT_Dimuon8_Upsilon_Barrel","HLT_Dimuon13_Upsilon",
        "HLT_DoubleMu2_Jpsi_DoubleTrk1_Phi1p05","HLT_DoubleMu4_JpsiTrkTrk_Displaced",
        "HLT_DoubleMu4_JpsiTrk_Displaced"
      };

      for (unsigned int i = 0; i < NTRIGGERS; i++) {
         for (int version = 1; version < 19; version++) {
            std::stringstream ss;
            ss << TriggersToTest[i] << "_v" << version;
            unsigned int bit = TheTriggerNames.triggerIndex(edm::InputTag(ss.str()).label());
            if (bit < triggerResults_handle->size() && triggerResults_handle->accept(bit) && !triggerResults_handle->error(bit)) {
               trigger += (1<<i);
               break;
            }
         }
      }
    } else std::cout << "*** NO triggerResults found " << iEvent.id().run() << "," << iEvent.id().event() << std::endl;

// grabbing candidate information. Notice we are just keeping combinations with succesfull vertex fit
  if (TheCandidates.isValid() && !TheCandidates->empty()) {
    pat::CompositeCandidate TheCandidate_;
    nCandPerEvent = TheCandidates->size();
    for (unsigned int i=0; i< TheCandidates->size(); i++){
      TheCandidate_       = TheCandidates->at(i);
      candidate_vMass     = TheCandidate_.userFloat("vMass");
      candidate_vProb     = TheCandidate_.userFloat("vProb");
      candidate_vChi2     = TheCandidate_.userFloat("vChi2");
      candidate_cosAlpha  = TheCandidate_.userFloat("cosAlpha");
      candidate_ctauPV    = TheCandidate_.userFloat("ctauPV");
      candidate_ctauErrPV = TheCandidate_.userFloat("ctauErrPV");
      candidate_charge    = TheCandidate_.charge();
      candidate_lxy       = TheCandidate_.userFloat("lxy");
      candidate_lxyErr    = TheCandidate_.userFloat("lxyErr");
      candidate_lxyz      = TheCandidate_.userFloat("lxyz");
      candidate_lxyzErr   = TheCandidate_.userFloat("lxyzErr");

      thePrimaryV_X = TheCandidate_.userFloat("thePrimaryV_X");
      thePrimaryV_Y = TheCandidate_.userFloat("thePrimaryV_Y");
      thePrimaryV_Z = TheCandidate_.userFloat("thePrimaryV_Z");
      TheDecayVertex_X = TheCandidate_.userFloat("TheDecayVertex_X");
      TheDecayVertex_Y = TheCandidate_.userFloat("TheDecayVertex_Y");
      TheDecayVertex_Z = TheCandidate_.userFloat("TheDecayVertex_Z");
      thePrimaryV_2D_position = TheCandidate_.userFloat("thePrimaryV_2D_position");
      thePrimaryV_3D_position = TheCandidate_.userFloat("thePrimaryV_3D_position");
      TheDecayVertex_2D_position = TheCandidate_.userFloat("TheDecayVertex_2D_position");
      TheDecayVertex_3D_position = TheCandidate_.userFloat("TheDecayVertex_3D_position");
      TheVertexDistance_2D = TheCandidate_.userFloat("TheVertexDistance_2D");
      TheVertexDistance_3D = TheCandidate_.userFloat("TheVertexDistance_3D");

      dikaon_dRdimuon    = TheCandidate_.userFloat("dikaon_dRdimuon");
      kaon1_dRdimuon    = TheCandidate_.userFloat("kaon1_dRdimuon");
      kaon2_dRdimuon    = TheCandidate_.userFloat("kaon2_dRdimuon");
      kaon1_fromPV  = TheCandidate_.userInt("kaon1_fromPV");
      kaon2_fromPV  = TheCandidate_.userInt("kaon2_fromPV");
      kaon1_refVtx  = TheCandidate_.userInt("kaon1_refVtx");
      kaon2_refVtx  = TheCandidate_.userInt("kaon2_refVtx");
      kaon1_pvAssocQ  = TheCandidate_.userInt("kaon1_pvAssocQ");
      kaon2_pvAssocQ  = TheCandidate_.userInt("kaon2_pvAssocQ");

      kaon1_dzAssocPV = TheCandidate_.userFloat("kaon1_dzAssocPV");
      kaon2_dzAssocPV = TheCandidate_.userFloat("kaon2_dzAssocPV");

      vChi2KalmanKK    = TheCandidate_.userFloat("vChi2KalmanKK");
      vNDFKalmanKK    = TheCandidate_.userFloat("vNDFKalmanKK");
      vProbKalmanKK    = TheCandidate_.userFloat("vProbKalmanKK");

      vChi2KinematicKK    = TheCandidate_.userFloat("vChi2KinematicKK");
      vNDFKinematicKK    = TheCandidate_.userFloat("vNDFKinematicKK");
      vProbKinematicKK    = TheCandidate_.userFloat("vProbKinematicKK");

      pion1_fromPV  = TheCandidate_.userInt("pion1_fromPV");
      pion2_fromPV  = TheCandidate_.userInt("pion2_fromPV");
      pion1_refVtx  = TheCandidate_.userInt("pion1_refVtx");
      pion2_refVtx  = TheCandidate_.userInt("pion2_refVtx");
      pion1_pvAssocQ  = TheCandidate_.userInt("pion1_pvAssocQ");
      pion2_pvAssocQ  = TheCandidate_.userInt("pion2_pvAssocQ");
      pion1_dzAssocPV = TheCandidate_.userFloat("pion1_dzAssocPV");
      pion2_dzAssocPV = TheCandidate_.userFloat("pion2_dzAssocPV");

      pion1_highPurity = TheCandidate_.userInt("pion1_highPurity");
      pion1_hasTrackDetails = TheCandidate_.userInt("pion1_hasTrackDetails");

      pion2_highPurity = TheCandidate_.userInt("pion2_highPurity");
      pion2_hasTrackDetails = TheCandidate_.userInt("pion2_hasTrackDetails");

      const pat::CompositeCandidate *TheDimuon_ = nullptr;
      const reco::Vertex *ThePrimaryV_ = nullptr;

      TheDimuon_ = dynamic_cast <pat::CompositeCandidate *>(TheCandidate_.daughter("onia"));
      ThePrimaryV_ = TheDimuon_->userData<reco::Vertex>("PVwithmuons");

      const reco::Track *Thekaon1_ =  dynamic_cast <const reco::Track *>(TheCandidate_.userData<reco::Track>("kaon1"));
      const reco::Track *Thekaon2_ =  dynamic_cast <const reco::Track *>(TheCandidate_.userData<reco::Track>("kaon2"));

      const reco::Track *Thepion1_ =  dynamic_cast <const reco::Track *>(TheCandidate_.userData<reco::Track>("pion1"));
      const reco::Track *Thepion2_ =  dynamic_cast <const reco::Track *>(TheCandidate_.userData<reco::Track>("pion2"));

      candidate_p4.SetPtEtaPhiM(TheCandidate_.pt(),TheCandidate_.eta(),TheCandidate_.phi(),TheCandidate_.mass());
      dimuon_p4.SetPtEtaPhiM(TheDimuon_->pt(),TheDimuon_->eta(),TheDimuon_->phi(),TheDimuon_->mass());
      kaon1_p4.SetPtEtaPhiM(Thekaon1_->pt(),Thekaon1_->eta(),Thekaon1_->phi(),kaonTMass_);
      kaon2_p4.SetPtEtaPhiM(Thekaon2_->pt(),Thekaon2_->eta(),Thekaon2_->phi(),kaonTMass_);
      dikaon_p4 = kaon1_p4 + kaon2_p4;
      pion1_p4.SetPtEtaPhiM(Thepion1_->pt(),Thepion1_->eta(),Thepion1_->phi(),pionTMass_);
      pion2_p4.SetPtEtaPhiM(Thepion2_->pt(),Thepion2_->eta(),Thepion2_->phi(),pionTMass_);
      dipion_p4 = pion1_p4 + pion2_p4;

      typedef math::XYZPoint Point;
      Point pv_(ThePrimaryV_->x(),ThePrimaryV_->y(),ThePrimaryV_->z());

      kaon1_d0      = Thekaon1_->d0();
      kaon1_d0Err   = Thekaon1_->d0Error();
      kaon1_dz      = Thekaon1_->dz(pv_);
      kaon1_dzErr   = Thekaon1_->dzError();
      kaon1_dxy     = Thekaon1_->dxy(pv_);
      kaon1_dxyErr  = Thekaon1_->dxyError();
      kaon1_nvsh    = Thekaon1_->hitPattern().numberOfValidStripHits();
      kaon1_nvph    = Thekaon1_->hitPattern().numberOfValidPixelHits();
      kaon1_charge  = Thekaon1_->charge();

      float min_eig = 9999.9f;

      reco::TrackBase::CovarianceMatrix cm1 = Thekaon1_->covariance();
      TMatrixDSym new_cov1(cm1.kRows);
      for (int j = 0; j < cm1.kRows; j++)
          for (int k = 0; k < cm1.kRows; k++)
              new_cov1(j,k) = cm1(j,k);

      TVectorD eig1(cm1.kRows);
      new_cov1.EigenVectors(eig1);
      for (int j = 0; j < cm1.kRows; j++)
          if (eig1(j) < min_eig)
              min_eig = eig1(j);

      kaon1_minEigen = min_eig;
      kaon1_SQopQop = cm1( reco::TrackBase::i_qoverp, reco::TrackBase::i_qoverp ); //0
      kaon1_SQopLam = cm1( reco::TrackBase::i_qoverp, reco::TrackBase::i_lambda );
      kaon1_SQopPhi = cm1( reco::TrackBase::i_qoverp, reco::TrackBase::i_phi    );
      kaon1_SQopDxy = cm1( reco::TrackBase::i_qoverp, reco::TrackBase::i_dxy    );
      kaon1_SQopDsz = cm1( reco::TrackBase::i_qoverp, reco::TrackBase::i_dsz    );
      kaon1_SLamLam = cm1( reco::TrackBase::i_lambda, reco::TrackBase::i_lambda );
      kaon1_SLamPhi = cm1( reco::TrackBase::i_lambda, reco::TrackBase::i_phi    );
      kaon1_SLamDxy = cm1( reco::TrackBase::i_lambda, reco::TrackBase::i_dxy    );
      kaon1_SLamDsz = cm1( reco::TrackBase::i_lambda, reco::TrackBase::i_dsz    );
      kaon1_SPhiPhi = cm1( reco::TrackBase::i_phi   , reco::TrackBase::i_phi    );
      kaon1_SPhiDxy = cm1( reco::TrackBase::i_phi   , reco::TrackBase::i_dxy    );
      kaon1_SPhiDsz = cm1( reco::TrackBase::i_phi   , reco::TrackBase::i_dsz    );
      kaon1_SDxyDxy = cm1( reco::TrackBase::i_dxy   , reco::TrackBase::i_dxy    );
      kaon1_SDxyDsz = cm1( reco::TrackBase::i_dxy   , reco::TrackBase::i_dsz    );
      kaon1_SDszDsz = cm1( reco::TrackBase::i_dsz   , reco::TrackBase::i_dsz    );

      kaon2_d0      = Thekaon2_->d0();
      kaon2_d0Err   = Thekaon2_->d0Error();
      kaon2_dz      = Thekaon2_->dz(pv_);
      kaon2_dzErr   = Thekaon2_->dzError();
      kaon2_dxy     = Thekaon2_->dxy(pv_);
      kaon2_dxyErr  = Thekaon2_->dxyError();
      kaon2_nvsh    = Thekaon2_->hitPattern().numberOfValidStripHits();
      kaon2_nvph    = Thekaon2_->hitPattern().numberOfValidPixelHits();
      kaon2_charge  = Thekaon2_->charge();

      reco::TrackBase::CovarianceMatrix cm2 = Thekaon2_->covariance();
      min_eig = 9999.9f;

      TMatrixDSym new_cov2(cm2.kRows);
      for (int j = 0; j < cm2.kRows; j++)
          for (int k = 0; k < cm2.kRows; k++)
              new_cov2(j,k) = cm2(j,k);

      TVectorD eig2(cm2.kRows);
      new_cov2.EigenVectors(eig2);
      for (int j = 0; j < cm2.kRows; j++)
          if (eig2(j) < min_eig)
              min_eig = eig2(j);

      kaon2_minEigen = min_eig;

      kaon2_SQopQop = cm2( reco::TrackBase::i_qoverp, reco::TrackBase::i_qoverp ); //0
      kaon2_SQopLam = cm2( reco::TrackBase::i_qoverp, reco::TrackBase::i_lambda );
      kaon2_SQopPhi = cm2( reco::TrackBase::i_qoverp, reco::TrackBase::i_phi    );
      kaon2_SQopDxy = cm2( reco::TrackBase::i_qoverp, reco::TrackBase::i_dxy    );
      kaon2_SQopDsz = cm2( reco::TrackBase::i_qoverp, reco::TrackBase::i_dsz    );
      kaon2_SLamLam = cm2( reco::TrackBase::i_lambda, reco::TrackBase::i_lambda );
      kaon2_SLamPhi = cm2( reco::TrackBase::i_lambda, reco::TrackBase::i_phi    );
      kaon2_SLamDxy = cm2( reco::TrackBase::i_lambda, reco::TrackBase::i_dxy    );
      kaon2_SLamDsz = cm2( reco::TrackBase::i_lambda, reco::TrackBase::i_dsz    );
      kaon2_SPhiPhi = cm2( reco::TrackBase::i_phi   , reco::TrackBase::i_phi    );
      kaon2_SPhiDxy = cm2( reco::TrackBase::i_phi   , reco::TrackBase::i_dxy    );
      kaon2_SPhiDsz = cm2( reco::TrackBase::i_phi   , reco::TrackBase::i_dsz    );
      kaon2_SDxyDxy = cm2( reco::TrackBase::i_dxy   , reco::TrackBase::i_dxy    );
      kaon2_SDxyDsz = cm2( reco::TrackBase::i_dxy   , reco::TrackBase::i_dsz    );
      kaon2_SDszDsz = cm2( reco::TrackBase::i_dsz   , reco::TrackBase::i_dsz    );

      pion1_d0      = Thepion1_->d0();
      pion1_d0Err   = Thepion1_->d0Error();
      pion1_dz      = Thepion1_->dz(pv_);
      pion1_dzErr   = Thepion1_->dzError();
      pion1_dxy     = Thepion1_->dxy(pv_);
      pion1_dxyErr  = Thepion1_->dxyError();
      pion1_nvsh    = Thepion1_->hitPattern().numberOfValidStripHits();
      pion1_nvph    = Thepion1_->hitPattern().numberOfValidPixelHits();
      pion1_charge  = Thepion1_->charge();

      min_eig = 9999.9f;

      reco::TrackBase::CovarianceMatrix cm3 = Thepion1_->covariance();
      TMatrixDSym new_cov3(cm3.kRows);
      for (int j = 0; j < cm3.kRows; j++)
          for (int k = 0; k < cm3.kRows; k++)
              new_cov3(j,k) = cm3(j,k);

      TVectorD eig3(cm3.kRows);
      new_cov3.EigenVectors(eig3);
      for (int j = 0; j < cm3.kRows; j++)
          if (eig3(j) < min_eig)
              min_eig = eig3(j);

      pion1_minEigen = min_eig;
      pion1_SQopQop = cm3( reco::TrackBase::i_qoverp, reco::TrackBase::i_qoverp ); //0
      pion1_SQopLam = cm3( reco::TrackBase::i_qoverp, reco::TrackBase::i_lambda );
      pion1_SQopPhi = cm3( reco::TrackBase::i_qoverp, reco::TrackBase::i_phi    );
      pion1_SQopDxy = cm3( reco::TrackBase::i_qoverp, reco::TrackBase::i_dxy    );
      pion1_SQopDsz = cm3( reco::TrackBase::i_qoverp, reco::TrackBase::i_dsz    );
      pion1_SLamLam = cm3( reco::TrackBase::i_lambda, reco::TrackBase::i_lambda );
      pion1_SLamPhi = cm3( reco::TrackBase::i_lambda, reco::TrackBase::i_phi    );
      pion1_SLamDxy = cm3( reco::TrackBase::i_lambda, reco::TrackBase::i_dxy    );
      pion1_SLamDsz = cm3( reco::TrackBase::i_lambda, reco::TrackBase::i_dsz    );
      pion1_SPhiPhi = cm3( reco::TrackBase::i_phi   , reco::TrackBase::i_phi    );
      pion1_SPhiDxy = cm3( reco::TrackBase::i_phi   , reco::TrackBase::i_dxy    );
      pion1_SPhiDsz = cm3( reco::TrackBase::i_phi   , reco::TrackBase::i_dsz    );
      pion1_SDxyDxy = cm3( reco::TrackBase::i_dxy   , reco::TrackBase::i_dxy    );
      pion1_SDxyDsz = cm3( reco::TrackBase::i_dxy   , reco::TrackBase::i_dsz    );
      pion1_SDszDsz = cm3( reco::TrackBase::i_dsz   , reco::TrackBase::i_dsz    );

      pion2_d0      = Thepion2_->d0();
      pion2_d0Err   = Thepion2_->d0Error();
      pion2_dz      = Thepion2_->dz(pv_);
      pion2_dzErr   = Thepion2_->dzError();
      pion2_dxy     = Thepion2_->dxy(pv_);
      pion2_dxyErr  = Thepion2_->dxyError();
      pion2_nvsh    = Thepion2_->hitPattern().numberOfValidStripHits();
      pion2_nvph    = Thepion2_->hitPattern().numberOfValidPixelHits();
      pion2_charge  = Thepion2_->charge();

      reco::TrackBase::CovarianceMatrix cm4 = Thepion2_->covariance();
      min_eig = 9999.9f;

      TMatrixDSym new_cov4(cm4.kRows);
      for (int j = 0; j < cm4.kRows; j++)
          for (int k = 0; k < cm4.kRows; k++)
              new_cov4(j,k) = cm4(j,k);

      TVectorD eig4(cm4.kRows);
      new_cov4.EigenVectors(eig4);
      for (int j = 0; j < cm4.kRows; j++)
          if (eig4(j) < min_eig)
              min_eig = eig4(j);

      pion2_minEigen = min_eig;

      pion2_SQopQop = cm4( reco::TrackBase::i_qoverp, reco::TrackBase::i_qoverp ); //0
      pion2_SQopLam = cm4( reco::TrackBase::i_qoverp, reco::TrackBase::i_lambda );
      pion2_SQopPhi = cm4( reco::TrackBase::i_qoverp, reco::TrackBase::i_phi    );
      pion2_SQopDxy = cm4( reco::TrackBase::i_qoverp, reco::TrackBase::i_dxy    );
      pion2_SQopDsz = cm4( reco::TrackBase::i_qoverp, reco::TrackBase::i_dsz    );
      pion2_SLamLam = cm4( reco::TrackBase::i_lambda, reco::TrackBase::i_lambda );
      pion2_SLamPhi = cm4( reco::TrackBase::i_lambda, reco::TrackBase::i_phi    );
      pion2_SLamDxy = cm4( reco::TrackBase::i_lambda, reco::TrackBase::i_dxy    );
      pion2_SLamDsz = cm4( reco::TrackBase::i_lambda, reco::TrackBase::i_dsz    );
      pion2_SPhiPhi = cm4( reco::TrackBase::i_phi   , reco::TrackBase::i_phi    );
      pion2_SPhiDxy = cm4( reco::TrackBase::i_phi   , reco::TrackBase::i_dxy    );
      pion2_SPhiDsz = cm4( reco::TrackBase::i_phi   , reco::TrackBase::i_dsz    );
      pion2_SDxyDxy = cm4( reco::TrackBase::i_dxy   , reco::TrackBase::i_dxy    );
      pion2_SDxyDsz = cm4( reco::TrackBase::i_dxy   , reco::TrackBase::i_dsz    );
      pion2_SDszDsz = cm4( reco::TrackBase::i_dsz   , reco::TrackBase::i_dsz    );

      iPVwithmuons = TheDimuon_->userInt("iPV");

      dimuon_diMuIndx     = TheDimuon_->userInt("diMuIndex");
      dimuon_vertexWeight = TheDimuon_->userFloat("vertexWeight");
      dimuon_vProb        = TheDimuon_->userFloat("vProb");
      dimuon_vMass        = TheDimuon_->userFloat("vMass");
      dimuon_vChi2        = TheDimuon_->userFloat("vNChi2");
      dimuon_DCA          = TheDimuon_->userFloat("DCA");
      dimuon_ctauPV       = TheDimuon_->userFloat("ppdlPV");
      dimuon_ctauErrPV    = TheDimuon_->userFloat("ppdlErrPV");
      dimuon_cosAlpha     = TheDimuon_->userFloat("cosAlpha");

      dimuon_lxyPV        = TheDimuon_->userFloat("lxyPV");
      dimuon_lxyErrPV     = TheDimuon_->userFloat("lxyErrPV");
      dimuon_ctauBS       = TheDimuon_->userFloat("ppdlBS");
      dimuon_ctauErrBS    = TheDimuon_->userFloat("ppdlErrBS");
      dimuon_lxyBS        = TheDimuon_->userFloat("lxyBS");
      dimuon_lxyErrBS     = TheDimuon_->userFloat("lxyErrBS");

      const reco::Candidate::LorentzVector vP = TheDimuon_->daughter("muon1")->p4();
      const reco::Candidate::LorentzVector vM = TheDimuon_->daughter("muon2")->p4();
      if (TheDimuon_->daughter("muon1")->charge() > 0) {
      	 muonp_p4.SetPtEtaPhiM(vP.pt(), vP.eta(), vP.phi(), vP.mass());
      	 muonn_p4.SetPtEtaPhiM(vM.pt(), vM.eta(), vM.phi(), vM.mass());
      } else {
         muonn_p4.SetPtEtaPhiM(vP.pt(), vP.eta(), vP.phi(), vP.mass());
         muonp_p4.SetPtEtaPhiM(vM.pt(), vM.eta(), vM.phi(), vM.mass());
      }

      TheTree->Fill();
      if (OnlyBest_) break;
    }
  } else std::cout<< "No candidate information " << run << "," << event <<std::endl;

  if (TheUps.isValid() && !TheUps->empty()) {
    pat::CompositeCandidate TheUps_;
    for (unsigned int i=0; i< TheUps->size(); i++){
      TheUps_       = TheUps->at(i);

      ups_p4.SetPtEtaPhiM(TheUps_.pt(),TheUps_.eta(),TheUps_.phi(),TheUps_.mass());

      ups_vertexWeight = TheUps_.userFloat("vertexWeight");
      ups_vProb        = TheUps_.userFloat("vProb");
      ups_vMass        = TheUps_.userFloat("vMass");
      ups_vChi2        = TheUps_.userFloat("vNChi2");
      ups_DCA          = TheUps_.userFloat("DCA");
      ups_ctauPV       = TheUps_.userFloat("ppdlPV");
      ups_ctauErrPV    = TheUps_.userFloat("ppdlErrPV");
      ups_cosAlpha     = TheUps_.userFloat("cosAlpha");

      ups_lxyPV        = TheUps_.userFloat("lxyPV");
      ups_lxyErrPV     = TheUps_.userFloat("lxyErrPV");
      ups_ctauBS       = TheUps_.userFloat("ppdlBS");
      ups_ctauErrBS    = TheUps_.userFloat("ppdlErrBS");
      ups_lxyBS        = TheUps_.userFloat("lxyBS");
      ups_lxyErrBS     = TheUps_.userFloat("lxyErrBS");

      iPVwithmuons_ups = TheUps_.userInt("iPV");

      const reco::Candidate::LorentzVector muP = TheUps_.daughter("muon1")->p4();
      const reco::Candidate::LorentzVector muM = TheUps_.daughter("muon2")->p4();
      if (TheUps_.daughter("muon1")->charge() > 0) {
         muonP_p4.SetPtEtaPhiM(muP.pt(), muP.eta(), muP.phi(), muP.mass());
         muonN_p4.SetPtEtaPhiM(muM.pt(), muM.eta(), muM.phi(), muM.mass());
      } else {
         muonN_p4.SetPtEtaPhiM(muP.pt(), muP.eta(), muP.phi(), muP.mass());
         muonP_p4.SetPtEtaPhiM(muM.pt(), muM.eta(), muM.phi(), muM.mass());
      }

      //double testPt1 = 0.;
      if (TheUps_.userInt("mu1_charge") > 0) {
        //testPt1 = TheUps_.userFloat("mu1_pt");
        mu1_pt      = TheUps_.userFloat("mu1_pt");
        mu1_ptErr   = TheUps_.userFloat("mu1_ptErr");
        mu1_d0      = TheUps_.userFloat("mu1_d0");
        mu1_d0Err   = TheUps_.userFloat("mu1_d0Err");
        mu1_dz      = TheUps_.userFloat("mu1_dz");
        mu1_dzErr   = TheUps_.userFloat("mu1_dzErr");
        mu1_dxy     = TheUps_.userFloat("mu1_dxy");
        mu1_dxyErr  = TheUps_.userFloat("mu1_dxyErr");
        mu1_nvsh    = TheUps_.userInt("mu1_nvsh");
        mu1_nvph    = TheUps_.userInt("mu1_nvph");
        mu1_charge  = TheUps_.userInt("mu1_charge");

        mu2_pt      = TheUps_.userFloat("mu2_pt");
        mu2_ptErr   = TheUps_.userFloat("mu2_ptErr");
        mu2_d0      = TheUps_.userFloat("mu2_d0");
        mu2_d0Err   = TheUps_.userFloat("mu2_d0Err");
        mu2_dz      = TheUps_.userFloat("mu2_dz");
        mu2_dzErr   = TheUps_.userFloat("mu2_dzErr");
        mu2_dxy     = TheUps_.userFloat("mu2_dxy");
        mu2_dxyErr  = TheUps_.userFloat("mu2_dxyErr");
        mu2_nvsh    = TheUps_.userInt("mu2_nvsh");
        mu2_nvph    = TheUps_.userInt("mu2_nvph");
        mu2_charge  = TheUps_.userInt("mu2_charge");
      } else {
        //testPt1 = TheUps_.userFloat("mu2_pt");
        mu1_pt      = TheUps_.userFloat("mu2_pt");
        mu1_ptErr   = TheUps_.userFloat("mu2_ptErr");
        mu1_d0      = TheUps_.userFloat("mu2_d0");
        mu1_d0Err   = TheUps_.userFloat("mu2_d0Err");
        mu1_dz      = TheUps_.userFloat("mu2_dz");
        mu1_dzErr   = TheUps_.userFloat("mu2_dzErr");
        mu1_dxy     = TheUps_.userFloat("mu2_dxy");
        mu1_dxyErr  = TheUps_.userFloat("mu2_dxyErr");
        mu1_nvsh    = TheUps_.userInt("mu2_nvsh");
        mu1_nvph    = TheUps_.userInt("mu2_nvph");
        mu1_charge  = TheUps_.userInt("mu2_charge");

        mu2_pt      = TheUps_.userFloat("mu1_pt");
        mu2_ptErr   = TheUps_.userFloat("mu1_ptErr");
        mu2_d0      = TheUps_.userFloat("mu1_d0");
        mu2_d0Err   = TheUps_.userFloat("mu1_d0Err");
        mu2_dz      = TheUps_.userFloat("mu1_dz");
        mu2_dzErr   = TheUps_.userFloat("mu1_dzErr");
        mu2_dxy     = TheUps_.userFloat("mu1_dxy");
        mu2_dxyErr  = TheUps_.userFloat("mu1_dxyErr");
        mu2_nvsh    = TheUps_.userInt("mu1_nvsh");
        mu2_nvph    = TheUps_.userInt("mu1_nvph");
        mu2_charge  = TheUps_.userInt("mu1_charge");
      }

      //std::cout<<" ===> pt = "<<muonP_p4.Pt()<<" single one = "<<testPt1<<std::endl;

      JpsiTree->Fill();
    }
  } else std::cout<< "No Upsilon candidate information " << run << "," << event <<std::endl;



}

void JpsiPhiKsRootupler::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
DEFINE_FWK_MODULE(JpsiPhiKsRootupler);

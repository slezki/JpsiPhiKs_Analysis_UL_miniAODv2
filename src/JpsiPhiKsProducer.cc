#include "JpsiKKK/JpsiPhiKs/interface/JpsiPhiKsProducer.h"
#include "DataFormats/PatCandidates/interface/UserData.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "CommonTools/Statistics/interface/ChiSquaredProbability.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"
#include "RecoVertex/KinematicFit/interface/KinematicParticleVertexFitter.h"
#include "RecoVertex/KinematicFitPrimitives/interface/KinematicParticleFactoryFromTransientTrack.h"
#include "RecoVertex/KinematicFit/interface/MassKinematicConstraint.h"
#include "RecoVertex/KinematicFit/interface/KinematicParticleFitter.h"
#include "RecoVertex/KinematicFitPrimitives/interface/MultiTrackKinematicConstraint.h"
#include "RecoVertex/KinematicFit/interface/KinematicConstrainedVertexFitter.h"
#include "RecoVertex/KinematicFit/interface/TwoTrackMassKinematicConstraint.h"
#include "RecoVertex/KinematicFitPrimitives/interface/KinematicParticle.h"
#include "RecoVertex/KinematicFitPrimitives/interface/RefCountedKinematicParticle.h"
#include "RecoVertex/KinematicFitPrimitives/interface/TransientTrackKinematicParticle.h"
#include "RecoVertex/VertexTools/interface/VertexDistanceXY.h"
#include "RecoVertex/VertexTools/interface/VertexDistance3D.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "TVector3.h"

JpsiPhiKsProducer::JpsiPhiKsProducer(const edm::ParameterSet& ps):
  OniaCollection_(consumes<pat::CompositeCandidateCollection>(ps.getParameter<edm::InputTag>("Onia"))),
  TrakCollection_(consumes<pat::PackedCandidateCollection>(ps.getParameter<edm::InputTag>("Track"))),
  kshortCollection_(consumes<reco::VertexCompositePtrCandidateCollection>(ps.getParameter<edm::InputTag>("Kshort"))),
  OniaMassCuts_(ps.getParameter<std::vector<double>>("OniaMassCuts")),
  CandidateMassCuts_(ps.getParameter<std::vector<double>>("CandidateMassCuts")),
  kaonTMass_(ps.getParameter<double>("kaonTMass")),
  piTMass_(ps.getParameter<double>("piTMass")),
  ConstraintMass_(ps.getParameter<double>("ConstraintMass"))
{
  produces<pat::CompositeCandidateCollection>();
  candidates = 0;
  nevents = 0;
}

void JpsiPhiKsProducer::produce(edm::Event& event, const edm::EventSetup& esetup){

  std::unique_ptr<pat::CompositeCandidateCollection> TheCandidateColl(new pat::CompositeCandidateCollection);

  edm::Handle<pat::CompositeCandidateCollection> Onias;
  event.getByToken(OniaCollection_,Onias);
  edm::Handle<pat::PackedCandidateCollection> Tracks;
  event.getByToken(TrakCollection_,Tracks);

  edm::Handle<reco::VertexCompositePtrCandidateCollection> kshort;
  event.getByToken(kshortCollection_,kshort);

  edm::ESHandle<TransientTrackBuilder> theB;
  esetup.get<TransientTrackRecord>().get("TransientTrackBuilder",theB);

  for (pat::CompositeCandidateCollection::const_iterator TheOnia = Onias->begin(); TheOnia != Onias->end(); ++TheOnia) {
     float oniaM = TheOnia->mass();
     if ( oniaM < OniaMassCuts_[1]  && oniaM > OniaMassCuts_[0] ) {
       const pat::Muon *pmu1, *pmu2;
       const reco::Vertex *thePrimaryV;
       pmu1 = dynamic_cast<const pat::Muon*>(TheOnia->daughter("muon1"));
       pmu2 = dynamic_cast<const pat::Muon*>(TheOnia->daughter("muon2"));
       thePrimaryV = TheOnia->userData<reco::Vertex>("PVwithmuons");
       kaons.clear();
       kaons_fromPV.clear();
       refVtx.clear();
       pvAssocQ.clear();
       dzAssocPV.clear();
       //std::cout<<"track size = "<<Tracks->size()<<std::endl;

       //Get Kaons from PackedCandidate
       if (Tracks.isValid() && !Tracks->empty()) {
         for (std::vector<pat::PackedCandidate>::const_iterator pp = Tracks->begin(); pp!= Tracks->end(); ++pp) {
           //if (!pp->trackHighPurity() || !pp->fromPV() || std::abs(pp->pdgId())!=321 || !pp->hasTrackDetails()) continue;
           if (!pp->trackHighPurity() || !pp->fromPV() || !pp->hasTrackDetails()) continue;
           //std::cout<<" ######### I have kaons for you ######### "<<std::endl;
           //if ( pp->vertexRef().key()!=0) continue;
           const reco::Track* TheTrack = &pp->pseudoTrack();
           //if (TheTrack->pt()<0.5 || std::abs(TheTrack->eta())>2.5 || TheTrack->charge()==0 ) continue;
           if (TheTrack->charge()==0) continue;
           //std::cout<<" ######### K charge is not ZERO ######### "<<std::endl;
           if ( IsTheSame(*TheTrack,*pmu1) || IsTheSame(*TheTrack,*pmu2) ) continue;
           //std::cout<<" ######### Kaons does not match with Muons ######### "<<std::endl;
           kaons.push_back(*TheTrack);
           kaons_fromPV.push_back(pp->fromPV());
           refVtx.push_back(pp->vertexRef().key());
           pvAssocQ.push_back(pp->pvAssociationQuality());
           dzAssocPV.push_back(pp->dzAssociatedPV());
           //std::cout<<" ######### PV Association Quality = "<< pp->pvAssociationQuality() <<" ######### "<<" PV id = " << pp->fromPV() <<" ######### "<<" Ref. Vertex i = " << pp->vertexRef().key() <<" ######### "<<std::endl;
           //std::cout<< " dzAssociatedPV = " << pp->dzAssociatedPV() << std::endl;
         }
       }

       //order by pT
       //std::sort(kaons.begin(),kaons.end(),PtComparator);

       for (unsigned int ii=0; ii<kaons.size();++ii) {
         reco::Track* Kaon1 = &kaons.at(ii);
         for (unsigned int jj=ii+1; jj<kaons.size();++jj) {
            reco::Track* Kaon2 = &kaons.at(jj);
            if (Kaon1 == Kaon2) continue;
            //if(Kaon1->charge() * Kaon2->charge() >= 0) continue; // Right sign
            //if(Kaon1->charge() * Kaon2->charge() <= 0) continue; // Wrong sign
            //std::cout<<" charge = "<<Kaon1->charge() * Kaon2->charge()<<std::endl;
            float deltaR_kaon1 = std::sqrt(reco::deltaR2(*Kaon1,*TheOnia));
            float deltaR_kaon2 = std::sqrt(reco::deltaR2(*Kaon2,*TheOnia));
            //if (deltaR_Kaon1>0.7 || deltaR_Kaon2>0.7) continue;
            double trk1_k = sqrt(kaonTMass_*kaonTMass_ + Kaon1->p()*Kaon1->p());
            double trk2_k = sqrt(kaonTMass_*kaonTMass_ + Kaon2->p()*Kaon2->p());
            reco::Candidate::LorentzVector Trk1_p4(Kaon1->px(),Kaon1->py(),Kaon1->pz(),trk1_k);
            reco::Candidate::LorentzVector Trk2_p4(Kaon2->px(),Kaon2->py(),Kaon2->pz(),trk2_k);
            reco::Candidate::LorentzVector dikaon_p4 = Trk1_p4 + Trk2_p4;
            if (dikaon_p4.M()<0.98 || dikaon_p4.M()>1.06) continue;

            //KK vertex fitter test
            KinematicParticleFactoryFromTransientTrack kFactory;
            const ParticleMass kMass(kaonTMass_);
            float kSigma = kMass*1E-6;

            std::vector<reco::TransientTrack> TkTk;
            TkTk.push_back(theB->build(*Kaon1));
            TkTk.push_back(theB->build(*Kaon2));
            std::vector<RefCountedKinematicParticle> kkDaug;
            kkDaug.push_back(kFactory.particle (TkTk[0], kMass, float(0), float(0), kSigma));
            kkDaug.push_back(kFactory.particle (TkTk[1], kMass, float(0), float(0), kSigma));

            //Kinematic
            KinematicParticleVertexFitter kinVertexFitter;
            RefCountedKinematicTree TheKKTree = kinVertexFitter.fit(kkDaug);

            float vChi2KinematicKK = -1.0;
            float vNDFKinematicKK  = -1.0;
            float vProbKinematicKK = -1.0;

            if (!TheKKTree->isEmpty())
            {
              TheKKTree->movePointerToTheTop();
              RefCountedKinematicVertex TheKKVertex = TheKKTree->currentDecayVertex();
              vChi2KinematicKK = TheKKVertex->chiSquared();
              vNDFKinematicKK  = TheKKVertex->degreesOfFreedom();
              vProbKinematicKK = (float)ChiSquaredProbability((double)(vChi2KinematicKK),(double)(vNDFKinematicKK));
            }

            //Kalman
            KalmanVertexFitter vtxFitter(true);
            TransientVertex kalmanVertex = vtxFitter.vertex(TkTk);

            float vChi2KalmanKK = kalmanVertex.totalChiSquared();
            float vNDFKalmanKK  = kalmanVertex.degreesOfFreedom();
            float vProbKalmanKK(TMath::Prob(vChi2KalmanKK,(int)vNDFKalmanKK));

            //std::cout<<" ######### You have KK in phi mass range ######### "<<std::endl;
            float deltaR_dikaon = std::sqrt(reco::deltaR2(dikaon_p4,*TheOnia));

            //get pions from Ks
            if (!kshort.isValid() || kshort->empty()) continue;
            for (reco::VertexCompositePtrCandidateCollection::const_iterator vpp = kshort->begin(); vpp!= kshort->end(); ++vpp) {

              const pat::PackedCandidate *vp1, *vp2;
              vp1 = dynamic_cast<const pat::PackedCandidate*>(vpp->daughter(0));
              vp2 = dynamic_cast<const pat::PackedCandidate*>(vpp->daughter(1));

              //if (!vp1->trackHighPurity() || !vp1->fromPV() || !vp1->hasTrackDetails()) continue; //It should check offline to don't lose Ks candidates.
              //if (!vp2->trackHighPurity() || !vp2->fromPV() || !vp2->hasTrackDetails()) continue; //It should check offline to don't lose Ks candidates.

              const reco::Track *vpi_track1, *vpi_track2;
              vpi_track1 = &vp1->pseudoTrack();
              vpi_track2 = &vp2->pseudoTrack();

              if (vpi_track1->charge()==0 || vpi_track2->charge()==0) continue;

              //Check Ks->pipi fit
              RefCountedKinematicParticle fittedKshort = FitKshort(*vpi_track1, *vpi_track2, theB);
              if (!fittedKshort) continue;

              double trk1_pi = sqrt(piTMass_*piTMass_ + vpi_track1->p()*vpi_track1->p());
              double trk2_pi = sqrt(piTMass_*piTMass_ + vpi_track2->p()*vpi_track2->p());
              reco::Candidate::LorentzVector vpi1_p4(vpi_track1->px(),vpi_track1->py(),vpi_track1->pz(),trk1_pi);
              reco::Candidate::LorentzVector vpi2_p4(vpi_track2->px(),vpi_track2->py(),vpi_track2->pz(),trk2_pi);
              reco::Candidate::LorentzVector diVpi_p4 = vpi1_p4 + vpi2_p4;

              pat::CompositeCandidate TheCandidate;
              TheCandidate.addDaughter(*TheOnia,"onia");
              TheCandidate.addUserData<reco::Track>( "kaon1", *Kaon1 );
              TheCandidate.addUserData<reco::Track>( "kaon2", *Kaon2 );
              TheCandidate.addUserFloat("dikaon_dRdimuon",deltaR_dikaon);
              TheCandidate.addUserFloat("kaon1_dRdimuon",deltaR_kaon1);
              TheCandidate.addUserFloat("kaon2_dRdimuon",deltaR_kaon2);
              TheCandidate.addUserInt("kaon1_fromPV",kaons_fromPV[ii]);
              TheCandidate.addUserInt("kaon2_fromPV",kaons_fromPV[jj]);
              TheCandidate.addUserInt("kaon1_refVtx",refVtx[ii]);
              TheCandidate.addUserInt("kaon2_refVtx",refVtx[jj]);
              TheCandidate.addUserInt("kaon1_pvAssocQ",pvAssocQ[ii]);
              TheCandidate.addUserInt("kaon2_pvAssocQ",pvAssocQ[jj]);
              TheCandidate.addUserFloat("kaon1_dzAssocPV",dzAssocPV[ii]);
              TheCandidate.addUserFloat("kaon2_dzAssocPV",dzAssocPV[jj]);

              TheCandidate.addUserFloat("vChi2KalmanKK",vChi2KalmanKK);
              TheCandidate.addUserFloat("vNDFKalmanKK",vNDFKalmanKK);
              TheCandidate.addUserFloat("vProbKalmanKK",vProbKalmanKK);

              TheCandidate.addUserFloat("vChi2KinematicKK",vChi2KinematicKK);
              TheCandidate.addUserFloat("vNDFKinematicKK",vNDFKinematicKK);
              TheCandidate.addUserFloat("vProbKinematicKK",vProbKinematicKK);

              //Collect tracks' information from Ks
              TheCandidate.addUserData<reco::Track>( "pion1", *vpi_track1 );
              TheCandidate.addUserData<reco::Track>( "pion2", *vpi_track2 );
              TheCandidate.addUserInt("pion1_fromPV",vp1->fromPV());
              TheCandidate.addUserInt("pion1_refVtx",vp1->vertexRef().key());
              TheCandidate.addUserInt("pion1_pvAssocQ",vp1->pvAssociationQuality());
              TheCandidate.addUserFloat("pion1_dzAssocPV",vp1->dzAssociatedPV());
              if (vp1->trackHighPurity()){
                TheCandidate.addUserInt("pion1_highPurity",1);
              } else {
                TheCandidate.addUserInt("pion1_highPurity",0);
              }
              if (vp1->hasTrackDetails()){
                TheCandidate.addUserInt("pion1_hasTrackDetails",1);
              } else {
                TheCandidate.addUserInt("pion1_hasTrackDetails",0);
              }
              TheCandidate.addUserInt("pion2_fromPV",vp2->fromPV());
              TheCandidate.addUserInt("pion2_refVtx",vp2->vertexRef().key());
              TheCandidate.addUserInt("pion2_pvAssocQ",vp2->pvAssociationQuality());
              TheCandidate.addUserFloat("pion2_dzAssocPV",vp2->dzAssociatedPV());
              if (vp2->trackHighPurity()){
                TheCandidate.addUserInt("pion2_highPurity",1);
              } else {
                TheCandidate.addUserInt("pion2_highPurity",0);
              }
              if (vp2->hasTrackDetails()){
                TheCandidate.addUserInt("pion2_hasTrackDetails",1);
              } else {
                TheCandidate.addUserInt("pion2_hasTrackDetails",0);
              }

              TheCandidate.setCharge(Kaon1->charge()+Kaon2->charge()+vpi_track1->charge()+vpi_track2->charge());
              reco::Candidate::LorentzVector vCandidate = TheOnia->p4() + dikaon_p4 + diVpi_p4;
              TheCandidate.setP4(vCandidate);

              if ( TheCandidate.mass() > CandidateMassCuts_[1] || TheCandidate.mass() < CandidateMassCuts_[0]) continue;
              //std::cout<<" ######### You have some possible candidates ######### "<<std::endl;

              std::vector<reco::TransientTrack> MuMuTk;
              MuMuTk.push_back(theB->build(*pmu1->innerTrack()));
              MuMuTk.push_back(theB->build(*pmu2->innerTrack()));
              MuMuTk.push_back(theB->build(*Kaon1));
              MuMuTk.push_back(theB->build(*Kaon2));

              KinematicParticleFactoryFromTransientTrack pFactory;
              const ParticleMass muMass(0.1056583);
              float muSigma = muMass*1E-6;
              const ParticleMass tkMass(kaonTMass_);
              float tkSigma = tkMass*1E-6;
              ParticleMass mass_(ConstraintMass_);
              std::vector<RefCountedKinematicParticle> allDaughters_;
              allDaughters_.push_back(pFactory.particle (MuMuTk[0], muMass, float(0), float(0), muSigma));
              allDaughters_.push_back(pFactory.particle (MuMuTk[1], muMass, float(0), float(0), muSigma));
              allDaughters_.push_back(pFactory.particle (MuMuTk[2], tkMass, float(0), float(0), tkSigma));
              allDaughters_.push_back(pFactory.particle (MuMuTk[3], tkMass, float(0), float(0), tkSigma));
              allDaughters_.push_back(fittedKshort);

              KinematicConstrainedVertexFitter constVertexFitter;
              MultiTrackKinematicConstraint *onia_mtc = new  TwoTrackMassKinematicConstraint(mass_);
              RefCountedKinematicTree TheParticleTree = constVertexFitter.fit(allDaughters_,onia_mtc);

              TVector3 pvtx(thePrimaryV->position().x(),thePrimaryV->position().y(),0);
              TVector3 pvtx3D(thePrimaryV->position().x(),thePrimaryV->position().y(),thePrimaryV->position().z());

              if (TheParticleTree->isEmpty()) continue;
              TheParticleTree->movePointerToTheTop();
              RefCountedKinematicParticle TheParticle = TheParticleTree->currentParticle();
              RefCountedKinematicVertex TheDecayVertex = TheParticleTree->currentDecayVertex();
              if (!TheParticle->currentState().isValid()) continue;
              //std::cout<<" ######### TheDecayVertex = "<<TheDecayVertex->position()<<" ######### "<<std::endl;
              if (TheParticle->currentState().mass() > CandidateMassCuts_[1] || TheParticle->currentState().mass() < CandidateMassCuts_[0]) continue;
              double Prob = ChiSquaredProbability((double)(TheDecayVertex->chiSquared()),(double)(TheDecayVertex->degreesOfFreedom()));
              if(Prob<0.005) continue;

              //std::cout<<" ######### You have CANDIDATES ######### "<<std::endl;

              TheCandidate.addUserFloat("vMass",TheParticle->currentState().mass());
              TheCandidate.addUserFloat("vChi2",TheDecayVertex->chiSquared());

              TVector3 vtx(TheDecayVertex->position().x(),TheDecayVertex->position().y(),0);
              TVector3 pperp(TheParticle->currentState().kinematicParameters().momentum().x(), TheParticle->currentState().kinematicParameters().momentum().y(), 0);
              AlgebraicVector3 vpperp(pperp.x(),pperp.y(),0);
              TVector3 vdiff = vtx - pvtx;
              double cosAlpha = vdiff.Dot(pperp) / (vdiff.Perp() * pperp.Perp());
              GlobalError v1e = (reco::Vertex(*TheDecayVertex)).error();
              GlobalError v2e = thePrimaryV->error();
              AlgebraicSymMatrix33 vXYe = v1e.matrix() + v2e.matrix();

              float lxy = vdiff.Perp();
              ROOT::Math::SVector<double, 3> vDiff; // needed by Similarity method
              vDiff[0] = vdiff.x(); vDiff[1] = vdiff.y(); vDiff[2] = 0; // needed by Similarity method
              float lxyErr = sqrt(ROOT::Math::Similarity(vDiff,vXYe)) / vdiff.Perp();

              VertexDistanceXY vdistXY;
              Measurement1D distXY = vdistXY.distance(reco::Vertex(*TheDecayVertex),* thePrimaryV);
              double ctauPV = distXY.value() * cosAlpha * TheParticle->currentState().mass() / pperp.Perp();
              double ctauErrPV = sqrt(ROOT::Math::Similarity(vpperp,vXYe)) * TheParticle->currentState().mass() / pperp.Perp2();

              TVector3 vtx3D(TheDecayVertex->position().x(),TheDecayVertex->position().y(),TheDecayVertex->position().z());

              VertexDistance3D vdistXYZ;
              Measurement1D distXYZ = vdistXYZ.distance(reco::Vertex(*TheDecayVertex),* thePrimaryV);

              TVector3 pperp3D(TheParticle->currentState().kinematicParameters().momentum().x(), TheParticle->currentState().kinematicParameters().momentum().y(), TheParticle->currentState().kinematicParameters().momentum().z());
              TVector3 vdiff3D = vtx3D - pvtx3D;
              //double dis = vtx3D.Mag()
              //std::cout<<" ######### Vertex distance 3V = "<<vdiff3D.Mag()<<" ######### Vertex distance 3D = "<<distXYZ.value()<<" ######### Vertex distance 3D manuel = "<<pV3D<<" ######### "<<std::endl;
              double cosAlpha3D = vdiff3D.Dot(pperp3D) / (vdiff3D.Mag() * pperp3D.Mag());

              float lxyz = vdiff3D.Mag();
              ROOT::Math::SVector<double, 3> vDiff3D; // needed by Similarity method
              vDiff3D[0] = vdiff3D.x(); vDiff3D[1] = vdiff3D.y(); vDiff3D[2] = vdiff3D.z(); // needed by Similarity method
              float lxyzErr = sqrt(ROOT::Math::Similarity(vDiff3D,vXYe)) / vdiff3D.Mag();

              TheCandidate.addUserInt("vStatus",1);
              TheCandidate.addUserFloat("vProb",Prob);
              TheCandidate.addUserFloat("cosAlpha",cosAlpha);
              TheCandidate.addUserFloat("cosAlpha3D",cosAlpha3D);
              TheCandidate.addUserFloat("ctauPV",ctauPV);
              TheCandidate.addUserFloat("ctauErrPV",ctauErrPV);
              TheCandidate.addUserFloat("lxy",lxy);
              TheCandidate.addUserFloat("lxyErr",lxyErr);
              TheCandidate.addUserFloat("lxyz",lxyz);
              TheCandidate.addUserFloat("lxyzErr",lxyzErr);
              TheCandidate.addUserFloat("thePrimaryV_X",thePrimaryV->position().x());
              TheCandidate.addUserFloat("thePrimaryV_Y",thePrimaryV->position().y());
              TheCandidate.addUserFloat("thePrimaryV_Z",thePrimaryV->position().z());
              TheCandidate.addUserFloat("TheDecayVertex_X",TheDecayVertex->position().x());
              TheCandidate.addUserFloat("TheDecayVertex_Y",TheDecayVertex->position().y());
              TheCandidate.addUserFloat("TheDecayVertex_Z",TheDecayVertex->position().z());
              TheCandidate.addUserFloat("thePrimaryV_2D_position",pvtx.Mag());
              TheCandidate.addUserFloat("thePrimaryV_3D_position",pvtx3D.Mag());
              TheCandidate.addUserFloat("TheDecayVertex_2D_position",vtx.Mag());
              TheCandidate.addUserFloat("TheDecayVertex_3D_position",vtx3D.Mag());
              TheCandidate.addUserFloat("TheVertexDistance_2D",distXY.value());
              TheCandidate.addUserFloat("TheVertexDistance_3D",distXYZ.value());

              TheCandidate.setVertex(reco::Vertex(*TheDecayVertex).position());
              TheCandidateColl->push_back(TheCandidate);
              candidates++;

            }

         } //Second track

       } //First

     }
  }
  JpsiPhiKsProducer::GreaterByVProb<pat::CompositeCandidate> vPComparator;
  std::sort(TheCandidateColl->begin(),TheCandidateColl->end(),vPComparator);
  event.put(std::move(TheCandidateColl));
  nevents++;
}

void JpsiPhiKsProducer::endJob(){
  std::cout << "###########################" << std::endl;
  std::cout << "JpsiPhiKsProducer report:" << std::endl;
  std::cout << "###########################" << std::endl;
  std::cout << "Found " << nevents << " Events" << std::endl;
  std::cout << "###########################" << std::endl;
  std::cout << "Found " << candidates << " X_b candidates." << std::endl;
  std::cout << "###########################" << std::endl;
 /* std::cout << "dz max  " << dz << std::endl;
  std::cout << "###########################" << std::endl;
  std::cout << "dxy max  " << dxy << std::endl;
  std::cout << "###########################" << std::endl;
  */
  //std::cout<< " PV ---- n0 = " << fpv0 << ", n1 = " << fpv1 << ", n2 = " << fpv2 << ", n3 = " << fpv3 << ", n23 = " << fpv23 << std::endl;
  //std::cout << "###########################" << std::endl;
}

const bool JpsiPhiKsProducer::IsTheSame(const reco::Track& tk, const pat::Muon& mu){
  return std::abs(mu.eta()-tk.eta()) < 0.02 && std::abs(mu.p()-tk.p()) < 0.02;
}

const RefCountedKinematicParticle JpsiPhiKsProducer::FitKshort(const reco::Track &tk0, const reco::Track &tk1, edm::ESHandle<TransientTrackBuilder> &theB) {
  RefCountedKinematicParticle theKshort = nullptr;
  std::vector<reco::TransientTrack> EETT;
  EETT.push_back(theB->build(tk0));
  EETT.push_back(theB->build(tk1));

  const ParticleMass ks_mass(kaonTMass_);
  float ks_sigma = ks_mass*1E-6;

  const ParticleMass piMass(piTMass_);
  float piSigma = piMass*1E-6;

  KinematicParticleFactoryFromTransientTrack pFactory;

  std::vector<RefCountedKinematicParticle> kshortParticles;
  kshortParticles.push_back(pFactory.particle(EETT[0],piMass,float(0),float(0),piSigma));
  kshortParticles.push_back(pFactory.particle(EETT[1],piMass,float(0),float(0),piSigma));

  KinematicParticleVertexFitter fitter;

  RefCountedKinematicTree kshortVertexFitTree;
  kshortVertexFitTree = fitter.fit(kshortParticles);

  /*if (!kshortVertexFitTree->isValid()) {
     edm::ParameterSet pSet;
     pSet.addParameter<double>("maxDistance", 3);
     pSet.addParameter<int>("maxNbrOfIterations", 10000); //10
     KinematicParticleVertexFitter fitter2(pSet);
     kshortVertexFitTree = fitter2.fit(kshortParticles);
  }*/

  if (kshortVertexFitTree->isValid()) {
     KinematicParticleFitter csFitterkshort;
     KinematicConstraint * ks_c = new MassKinematicConstraint(ks_mass,ks_sigma);
     kshortVertexFitTree->movePointerToTheTop();
     kshortVertexFitTree = csFitterkshort.fit(ks_c,kshortVertexFitTree);

     if (kshortVertexFitTree->isValid()) {
        kshortVertexFitTree->movePointerToTheTop();
        theKshort = kshortVertexFitTree->currentParticle();
     }
  }
  return theKshort;
}



DEFINE_FWK_MODULE(JpsiPhiKsProducer);

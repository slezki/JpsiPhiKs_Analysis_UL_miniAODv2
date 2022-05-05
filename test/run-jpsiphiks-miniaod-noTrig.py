ouput_filename = 'rootuple_UL2018_JpsiPhiKs.root'

import FWCore.ParameterSet.Config as cms
process = cms.Process('JPsiKK')

import FWCore.ParameterSet.VarParsing as VarParsing

vP = VarParsing.VarParsing('analysis')
vP.register('nThr',4, VarParsing.VarParsing.multiplicity.singleton,VarParsing.VarParsing.varType.int,"threads")
vP.register('nEvt',100,VarParsing.VarParsing.multiplicity.singleton,VarParsing.VarParsing.varType.int,"events")
vP.parseArguments()

maxEvnt = vP.nEvt

process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '106X_dataRun2_v35', '')

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(maxEvnt))

from JpsiKKK.JpsiPhiKs.run2_UL_miniAODv2_files import UL2018D_Charmonium
#"""
process.source = cms.Source("PoolSource",fileNames = cms.untracked.vstring(UL2018D_Charmonium))
#process.source.duplicateCheckMode = cms.untracked.string('noDuplicateCheck')

process.TFileService = cms.Service("TFileService",fileName = cms.string(ouput_filename))
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False))

process.options.numberOfThreads=cms.untracked.uint32(vP.nThr)
process.options.numberOfStreams=cms.untracked.uint32(vP.nThr)

process.oniaSelectedMuons = cms.EDFilter('PATMuonSelector',
   src = cms.InputTag('slimmedMuons'),
   cut = cms.string('muonID(\"TMOneStationTight\")'
                    ' && abs(innerTrack.dxy) < 0.3'
                    ' && abs(innerTrack.dz)  < 20.'
                    ' && innerTrack.hitPattern.trackerLayersWithMeasurement > 5'
                    ' && innerTrack.hitPattern.pixelLayersWithMeasurement > 0'
                    ' && innerTrack.quality(\"highPurity\")'
                    ' && (pt > 2.)'
   ),
   filter = cms.bool(True)
)

process.load("JpsiKKK.JpsiPhiKs.onia2MuMuPAT_cfi")
process.onia2MuMuPAT.muons=cms.InputTag('oniaSelectedMuons')
process.onia2MuMuPAT.primaryVertexTag=cms.InputTag('offlineSlimmedPrimaryVertices')
process.onia2MuMuPAT.beamSpotTag=cms.InputTag('offlineBeamSpot')
process.onia2MuMuPAT.higherPuritySelection=cms.string("")
process.onia2MuMuPAT.lowerPuritySelection=cms.string("")
process.onia2MuMuPAT.dimuonSelection=cms.string("2.9 < mass && mass < 3.3")
process.onia2MuMuPAT.addMCTruth = cms.bool(False)
'''
triggers = [
'HLT_DoubleMu2_Jpsi_DoubleTrk1_Phi1p05',
'HLT_DoubleMu4_JpsiTrkTrk_Displaced',
'HLT_DoubleMu4_JpsiTrk_Displaced'
]

hltpathsV = cms.vstring([h + '_v*' for h in triggers ])

process.triggerSelection = cms.EDFilter("TriggerResultsFilter",
                                        triggerConditions = cms.vstring(hltpathsV),
                                        hltResults = cms.InputTag( "TriggerResults", "", "HLT" ),
                                        l1tResults = cms.InputTag( "" ),
                                        throw = cms.bool(False)
                                        )
'''
process.Onia2MuMuFiltered = cms.EDProducer('DiMuonFilter',
      OniaTag             = cms.InputTag("onia2MuMuPAT"),
      singlemuonSelection = cms.string(""),
      #dimuonSelection     = cms.string("8.6 < mass && mass < 11.4 && pt > 10. && abs(y) < 1.2 && charge==0 && userFloat('vProb') > 0.01"),
      #dimuonSelection     = cms.string("8.8 < mass && mass < 11.4 && pt > 12. && charge==0"),
      dimuonSelection     = cms.string("2.9 < mass && mass < 3.3 && charge==0 && userFloat('vProb') > 0.01"),
      do_trigger_match    = cms.bool(False),
      #HLTFilters          = cms.vstring(hltpathsV),
      HLTFilters          = cms.vstring(),
)

process.DiMuonCounter = cms.EDFilter('CandViewCountFilter',
    src       = cms.InputTag("Onia2MuMuFiltered"),
    minNumber = cms.uint32(1),
)

process.OniaPseudoTrackTrackCandidateProducer = cms.EDProducer('OniaPseudoTrackTrackProducer',
    Onia = cms.InputTag("Onia2MuMuFiltered"),
    Track = cms.InputTag("packedPFCandidates"),
    Kshort = cms.InputTag("slimmedKshortVertices"),
    OniaMassCuts = cms.vdouble(2.9,3.3), #J
    CandidateMassCuts = cms.vdouble(4.0,6.0),
    kaonTMass = cms.double(0.493677),#kaons
    piTMass = cms.double(0.13957061),#kaons
    ConstraintMass = cms.double(3.096916),#J/Psi
)

process.JpsiPhiCounter = cms.EDFilter('CandViewCountFilter',
    src       = cms.InputTag("OniaPseudoTrackTrackCandidateProducer"),
    minNumber = cms.uint32(1),
)

process.jpsikkSequence = cms.Sequence(
                                   #process.triggerSelection *
                                   process.oniaSelectedMuons *
                                   process.onia2MuMuPAT*
                                   process.Onia2MuMuFiltered *
                                   process.DiMuonCounter *
                                   process.OniaPseudoTrackTrackCandidateProducer *
                                   process.JpsiPhiCounter 
                                   )

process.rootuple = cms.EDAnalyzer('OniaRecoTrackTrackRootupler',
                          TheCandidates = cms.InputTag("OniaPseudoTrackTrackCandidateProducer"),
                          TheUps = cms.InputTag("Onia2MuMuFiltered"),
                          PrimaryVertices = cms.InputTag("offlineSlimmedPrimaryVertices"),
                          TriggerResults = cms.InputTag("TriggerResults", "", "HLT"),
                          GenParticles = cms.InputTag("prunedGenParticles"),
                          kaonTMass = cms.double(0.493677),#kaons
                          pionTMass = cms.double(0.13957061),#kaons
                          DimuonMass = cms.double(3.096916),#J/Psi
                          candidate_pdgid = cms.uint32(20443), #10443 for spin 0
                          onia_pdgid = cms.uint32(443),#j/psi
                          dikaon_pdgid = cms.uint32(333),
                          kaon1_pdgid = cms.int32(321),
                          kaon2_pdgid = cms.int32(-321),
                          isMC = cms.bool(False),
                          OnlyBest = cms.bool(False)
)

process.p = cms.Path(process.jpsikkSequence*process.rootuple)

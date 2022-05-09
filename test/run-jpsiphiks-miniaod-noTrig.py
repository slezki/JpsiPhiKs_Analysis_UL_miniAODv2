ouput_filename = 'rootuple_UL2018_JpsiPhiKs.root'

import FWCore.ParameterSet.Config as cms
process = cms.Process('JPsiKK')

import FWCore.ParameterSet.VarParsing as VarParsing

vP = VarParsing.VarParsing('analysis')
vP.register('nThr',4, VarParsing.VarParsing.multiplicity.singleton,VarParsing.VarParsing.varType.int,"Num. of Threads")
vP.register('nEvt',1000,VarParsing.VarParsing.multiplicity.singleton,VarParsing.VarParsing.varType.int,"Num. of Events")
vP.register('yData','2018',VarParsing.VarParsing.multiplicity.singleton,VarParsing.VarParsing.varType.string,"data year")
vP.parseArguments()

maxEvnt = vP.nEvt
inputs = []

if vP.yData == '2018':
    from JpsiKKK.JpsiPhiKs.run2_UL_miniAODv2_files import UL2018D_Charmonium
    inputs = UL2018D_Charmonium
if vP.yData == '2017':
    from JpsiKKK.JpsiPhiKs.run2_UL_miniAODv2_files import UL2017E_Charmonium
    inputs = UL2017E_Charmonium
if vP.yData == '2016':
    from JpsiKKK.JpsiPhiKs.run2_UL_miniAODv2_files import UL2016G_Charmonium
    inputs = UL2016G_Charmonium

process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '106X_dataRun2_v35', '')

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(maxEvnt))

#"""
process.source = cms.Source("PoolSource",fileNames = cms.untracked.vstring(inputs))
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
process.onia2MuMuPAT.dimuonSelection=cms.string("2.8 < mass && mass < 3.4")
process.onia2MuMuPAT.addMCTruth = cms.bool(False)

process.Onia2MuMuFiltered = cms.EDProducer('DiMuonFilter',
      OniaTag             = cms.InputTag("onia2MuMuPAT"),
      singlemuonSelection = cms.string(""),
      dimuonSelection     = cms.string("2.9 < mass && mass < 3.3 && charge==0 && userFloat('vProb') > 0.01"),
      do_trigger_match    = cms.bool(False),
      HLTFilters          = cms.vstring(),
)

process.DiMuonCounter = cms.EDFilter('CandViewCountFilter',
    src       = cms.InputTag("Onia2MuMuFiltered"),
    minNumber = cms.uint32(1),
)

process.JpsiPhiKsCandidateProducer = cms.EDProducer('JpsiPhiKsProducer',
    Onia = cms.InputTag("Onia2MuMuFiltered"),
    Track = cms.InputTag("packedPFCandidates"),
    Kshort = cms.InputTag("slimmedKshortVertices"),
    OniaMassCuts = cms.vdouble(2.9,3.3), #J
    CandidateMassCuts = cms.vdouble(4.0,6.0),
    kaonTMass = cms.double(0.493677),#kaons
    piTMass = cms.double(0.13957061),#kaons
    ConstraintMass = cms.double(3.096916),#J/Psi
)

process.JpsiPhiKsCounter = cms.EDFilter('CandViewCountFilter',
    src       = cms.InputTag("JpsiPhiKsCandidateProducer"),
    minNumber = cms.uint32(1),
)

process.jpsikkSequence = cms.Sequence(
                                   process.oniaSelectedMuons *
                                   process.onia2MuMuPAT*
                                   process.Onia2MuMuFiltered *
                                   process.DiMuonCounter *
                                   process.JpsiPhiKsCandidateProducer *
                                   process.JpsiPhiKsCounter 
                                   )

process.rootuple = cms.EDAnalyzer('JpsiPhiKsRootupler',
                          TheCandidates = cms.InputTag("JpsiPhiKsCandidateProducer"),
                          TheUps = cms.InputTag("Onia2MuMuFiltered"),
                          PrimaryVertices = cms.InputTag("offlineSlimmedPrimaryVertices"),
                          TriggerResults = cms.InputTag("TriggerResults", "", "HLT"),
                          GenParticles = cms.InputTag("prunedGenParticles"),
                          kaonTMass = cms.double(0.493677),#kaons
                          pionTMass = cms.double(0.13957061),#pions
                          DimuonMass = cms.double(3.096916),#J/Psi
                          candidate_pdgid = cms.uint32(521), #10443 for spin 0
                          onia_pdgid = cms.uint32(443),#j/psi
                          dikaon_pdgid = cms.uint32(333),
                          kaon1_pdgid = cms.int32(321),
                          kaon2_pdgid = cms.int32(-321),
                          dipion_pdgid = cms.uint32(310),
                          pion1_pdgid = cms.int32(211),
                          pion2_pdgid = cms.int32(-211),
                          isMC = cms.bool(False),
                          OnlyBest = cms.bool(False)
)

process.p = cms.Path(process.jpsikkSequence*process.rootuple)

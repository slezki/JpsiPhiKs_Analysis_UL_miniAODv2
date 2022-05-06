# JpsiPhiKs-Analysis

This package is mean to be run using Ultra Legacy MINIAODv2

* Setup: (it has being tested on 10_6_27)

```
export SCRAM_ARCH=slc7_amd64_gcc700
scram p -n CMSSW_10627_JpsiPhiKs CMSSW_10_6_27
cd CMSSW_10627_JpsiPhiKs/src/
cmsenv
git clone git@github.com:slezki/JpsiPhiKs_Analysis_UL_miniAODv2.git JpsiKKK/JpsiPhiKs/
scram b

```

* Run: (use your favorite input sample)

```
cd JpsiKKK/JpsiPhiKs/test/
voms-proxy-init -rfc -voms cms -valid 192:00
cmsRun run-jpsiphiks-miniaod.py yData=2018 (for B0 -> JpsiPhiKs reconstruction using 2018 UL samples)
```

* To send multiple tasks in CRAB for Run II:

```
chmod +x crabCfgCreator.sh (just once)
./crabCfgCreator.sh 
```

* Check status (or resubmit, report etc..) for CRAB tasks:

```
chmod +x crabTools.sh (just once)
./crabTools.sh status workDir (workDir: see crabCfgCreator.sh file; status can be change with all CRAB commands which can be used after tasks have been sent.) 
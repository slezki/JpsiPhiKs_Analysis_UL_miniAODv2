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
voms-proxy-init -rfc -voms cms -valid 192:00
cmsRun JpsiKKK/JpsiPhiKs/test/run-jpsiphiks-miniaod.py (for B0 -> JpsiPhiKs reconstruction using UL MC samples)
```

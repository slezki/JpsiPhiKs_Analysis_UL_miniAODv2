#!/bin/bash

##
## just use : ./crabCfgCreator.sh
##

echo " "
echo "SL*******************************************************SL"
echo "SL*******************************************************SL"
echo "SL************* MISSION HAS BEEN STARTED ****************SL"
echo "SL*******************************************************SL"
echo "SL*******************************************************SL"
echo "SL************** You can drink a coffee *****************SL"
echo "SL*******************************************************SL"
echo "SL*******************************************************SL"
echo " "

PyFile=crabConfig_Data.py
#rm ${PyFile}

#s1=$1

counter=0

k=1

for (( j=0; j<18; j++ ))
do

counter=$((counter+1))

if [ $j -ge 9 -a $j -lt 14 ]; then
	k=2
elif [ $j -ge 14 ]; then
	k=3
fi

rm ${PyFile} 
echo "Old ${PyFile} was deleted"
echo " "

#echo "j = ${j}"
#echo " "

#echo "k = ${k}"
#echo " "

cat>> ${PyFile} <<pyFile
import CRABClient
from dbs.apis.dbsClient import DbsApi
from CRABClient.UserUtilities import Configuration#, getUsernameFromSiteDB
config = Configuration()

psetS = [
'run-jpsiphiks-miniaod.py'
]

decays = [
'JpsiPhiKs'
]

datasetnames = [
'/Charmonium/Run2016B-ver1_HIPM_UL2016_MiniAODv2-v1/MINIAOD',
'/Charmonium/Run2016B-ver2_HIPM_UL2016_MiniAODv2-v1/MINIAOD',
'/Charmonium/Run2016C-HIPM_UL2016_MiniAODv2-v1/MINIAOD',
'/Charmonium/Run2016D-HIPM_UL2016_MiniAODv2-v1/MINIAOD',
'/Charmonium/Run2016E-HIPM_UL2016_MiniAODv2-v1/MINIAOD',
'/Charmonium/Run2016F-HIPM_UL2016_MiniAODv2-v1/MINIAOD',
'/Charmonium/Run2016F-UL2016_MiniAODv2-v1/MINIAOD',
'/Charmonium/Run2016G-UL2016_MiniAODv2-v1/MINIAOD',
'/Charmonium/Run2016H-UL2016_MiniAODv2-v2/MINIAOD',

'/Charmonium/Run2017B-UL2017_MiniAODv2-v1/MINIAOD',
'/Charmonium/Run2017C-UL2017_MiniAODv2-v1/MINIAOD',
'/Charmonium/Run2017D-UL2017_MiniAODv2-v1/MINIAOD',
'/Charmonium/Run2017E-UL2017_MiniAODv2-v1/MINIAOD',
'/Charmonium/Run2017F-UL2017_MiniAODv2-v1/MINIAOD',

'/Charmonium/Run2018A-UL2018_MiniAODv2-v1/MINIAOD',
'/Charmonium/Run2018B-UL2018_MiniAODv2-v1/MINIAOD',
'/Charmonium/Run2018C-UL2018_MiniAODv2-v1/MINIAOD',
'/Charmonium/Run2018D-UL2018_MiniAODv2-v1/MINIAOD'
]

runNumber = [
'',
'297620,297656',
'299420'
]

jsonfile = [
'',
'/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/Legacy_2016/Cert_271036-284044_13TeV_Legacy2016_Collisions16_JSON_MuonPhys.txt',
'/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/Legacy_2017/Cert_294927-306462_13TeV_UL2017_Collisions17_MuonJSON.txt',
'/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions18/13TeV/Legacy_2018/Cert_314472-325175_13TeV_Legacy2018_Collisions18_JSON_MuonPhys.txt'
]

workDir = 'JpsiPhiKs_DataRunII_UltraLegacy_miniAODv2_v2'
decay = decays[0]
pset = psetS[0]
runNum = runNumber[0]
lumi = jsonfile[$k] #no json: 0, 2016json: 1, 2017json: 2, 2018json: 3

datasetName = datasetnames[$j]

print "*****************"
print decay
print pset
print datasetName
print "*****************"

import datetime
timestamp = datetime.datetime.now().strftime("_%Y%m%d_%H%M%S")

dataset = filter(None, datasetName.split('/'))

config.section_('General')
config.General.transferOutputs  = True
config.General.workArea         = '%s' % (workDir)
config.General.requestName      = dataset[0]+'_'+dataset[1]+'_'+dataset[2]+'_'+runNum+'_'+decay+timestamp
config.General.transferLogs     = False

config.section_('JobType')
config.JobType.psetName         = pset
config.JobType.pluginName       = 'Analysis'
config.JobType.numCores			= 4
config.JobType.maxMemoryMB      = 10000
config.JobType.allowUndistributedCMSSW = True

config.section_('Data')
config.Data.inputDataset        = datasetName
config.Data.inputDBS            = 'global'
config.Data.splitting           = 'Automatic'
config.Data.runRange            = runNum
config.Data.lumiMask            = lumi
config.Data.outLFNDirBase       = '/store/user/slezki/%s' % (workDir)
config.Data.publication         = False
#config.Data.ignoreLocality      = True

config.section_('Site')
config.Site.storageSite         = 'T2_CN_Beijing'

pyFile

echo " Mission $counter:"
echo " "
echo "${PyFile} was created"
echo " "

crab submit ${PyFile} 

echo " "
echo "Crab task was submitted"
echo " "

done

#echo "$counter"

echo " "
echo "SL***************************************************SL"
echo "SL***************************************************SL"
echo "SL*************** MISSION COMPLETED *****************SL"
echo "SL***************************************************SL"
echo "SL***************************************************SL"
echo "SL**************** Are you here??? ******************SL"
echo "SL***************************************************SL"
echo "SL*********** Please check tasks' status ************SL"
echo "SL***************************************************SL"
echo "SL***************************************************SL"
echo " "

#!/bin/bash

# be sure this file is executable (chmod a+x crabStatus.sh)
# run:  ./crabStatus.sh $st/resubmit/kill $workArea (name of workArea in CRAB configuration)

arg1=$1
arg2=$2

find ${arg2}/*/ -name "crab_*" >& taskName.txt

echo "CRAB Task List have been created"

crabRS=crabStatusCheck
rm ${crabRS}.sh

cat>> ${crabRS}.sh <<rSfile
#!/bin/bash

counter=0

while IFS='' read -r line || [[ -n "\$line" ]]; do

counter=\$((counter+1))
#echo "\$counter"
#echo " \$line "
#echo " "
echo " Mission \$counter:"
echo "  crab ${arg1} \$line"
echo " "
crab-prod ${arg1} \$line
echo " "

done < "\$1"
rSfile

echo " "
echo "${crabRS}.sh have been created"
chmod a+x ${crabRS}.sh
echo " "
echo "${crabRS}.sh is executable now"
echo " "

./${crabRS}.sh taskName.txt

#mv ${crabRS}.sh ${crabRS}_Old.sh
mv taskName.txt taskName_Old.txt
rm ${crabRS}.sh

echo " "
echo "SL************************************************SL"
echo "SL************************************************SL"
echo "SL************* MISSION COMPLETED ****************SL"
echo "SL************************************************SL"
echo "SL************************************************SL"
echo "SL********** You can drink a coffee **************SL"
echo "SL************************************************SL"
echo "SL************************************************SL"
echo " "


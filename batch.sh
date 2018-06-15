# Ask for input directory
printf "Which folder do you want to analyse? "
read Folder
if [ "${Folder: -1}" != "/" ]
then
	Folder="$Folder/"
fi
echo ${Folder:0:-1}

# Read folder contents and ask to continue
printf "Folder \""
printf $Folder
echo "\" contains:"
echo ""
ls $Folder
echo ""
read -p "Press ENTER if you want to run ./execute on this folder?"

# 
for f in $Folder*
do
	ifname=${f/$Folder/}
	echo ""
	echo "--> RUNNING OVER FILE \""$ifname"\""
	echo ""
	ofname=${ifname/.txt/.root}
	ofname="output/"$ofname
	./execute -c $f -o $ofname
done
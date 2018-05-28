# Author: Remco de Boer
# Data: 19/05/2018

# Instructions to install the latest CERN ROOT Production ("Pro") distribution on a Linux system, including Minuit. The Pro version was 6.12/06 (2018-02-09) at the time of writing, but the procedure should work as well when later distributions come out.

# You can use these instructions as well as a bash script. Run using:
# ./"Install CERN ROOT.sh"

# To see which options are used by default, see: https://root.cern.ch/building-root

# Decide which version to install
read -p "This script will install CERN ROOT 6, version 6.12/06.
Check out the latest versions at https://root.cern.ch/downloading-root. If you want this version, press enter, otherwise, enter the version you want here: v6-" ROOTVERSION
if [ -n $ROOTVERSION ]
then
	ROOTVERSION="12-06"
	echo "--> Installing default version: v6-$ROOTVERSION
	"
else
	echo "--> Installing version: v6-$ROOTVERSION
	"
fi

# Clone entire ROOT source from the public Git repository
cd /usr/local/
sudo git clone http://github.com/root-project/root.git
sudo chown -R $(whoami):$(id -g -n $(whoami)) root
cd root

# Set the correct release tag
sudo git checkout -b v6-$ROOTVERSION v6-$ROOTVERSION

# Make a directory in the source folder that will be used for compilation files
mkdir compile
cd compile

# Build source material and make install
cmake -Dminuit=ON -Dminuit2=ON -DCMAKE_INSTALL_PREFIX:PATH=/usr/local/root ..
make -j$(nproc)
sudo make install

# Install ROOT: tell Ubuntu how to run ROOT
cd /usr/local/root
. bin/thisroot.sh
source bin/thisroot.sh

# Information about adding ROOT to your bash
read -p "
Installation of ROOT completed!
Now, please copy the following lines (Ctrl+Shift+C)

# CERN ROOT
export ROOTSYS=/usr/local/root
export PATH=\$ROOTSYS/bin:\$PATH
export PYTHONDIR=\$ROOTSYS
export LD_LIBRARY_PATH=\$ROOTSYS/lib:\$PYTHONDIR/lib:\$ROOTSYS/bindings/pyroot:\$LD_LIBRARY_PATH
export PYTHONPATH=\$ROOTSYS/lib:\$PYTHONPATH:\$ROOTSYS/bindings/pyroot

and paste them to the end of your bash file (~/bashrc). It will be opened when you press enter."
nano ~/.bashrc
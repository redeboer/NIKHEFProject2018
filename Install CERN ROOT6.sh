# Author: Remco de Boer
# Data: 19/05/2018

# Instructions to install the latest CERN ROOT Production ("Pro") distribution, including Minuit, on Ubuntu. The Pro version was 6.12/06 (2018-02-09) at the time of writing, but the procedure should work as well when later distributions come out.

# You can use these instructions as well as a bash script. Run using:
# ./"Install CERN ROOT.sh"

# To see which options are used by default, see: https://root.cern.ch/building-root

# Decide which version to install
read -p "This script will install CERN ROOT 6, version 6.12/06. First, verify that you meet the build requirements: https://root.cern.ch/build-prerequisites. Particularly, make sure that you have installed the right packages!

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

# Install prerequisites
sudo apt-get install git dpkg-dev cmake g++ gcc binutils libx11-dev libxpm-dev libxft-dev libxext-dev
# Optional packages
sudo apt-get install gfortran libssl-dev libpcre3-dev xlibmesa-glu-dev libglew1.5-dev libftgl-dev libmysqlclient-dev libfftw3-dev libcfitsio-dev graphviz-dev libavahi-compat-libdnssd-dev libldap2-dev python-dev libxml2-dev libkrb5-dev libgsl0-dev libqt4-dev

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
# Also possible: ccmake, which gives an overview of the options
make -j$(nproc)
read -p "

Source material built. Press ENTER to \"make install\" ROOT.
"

sudo make install

# Remove build files
cd ..
rm -rf compile

# Install ROOT: tell Ubuntu how to run ROOT
cd /usr/local/root
. bin/thisroot.sh
source bin/thisroot.sh

# Information about adding ROOT to your bash. BE CAREFUL IF YOU COPY FROM THIS FILE: REPLACE \$ BY $

read -p "
Installation of ROOT completed! Upon pressing ENTER, the path variables for ROOT will be set in your .bashrc file. Gedit will be opened so you can check the result."
echo "
# CERN ROOT
export ROOTSYS=/usr/local/root
export PATH=\$ROOTSYS/bin:\$PATH
export PYTHONDIR=\$ROOTSYS
export LD_LIBRARY_PATH=\$ROOTSYS/lib:\$PYTHONDIR/lib:\$ROOTSYS/bindings/pyroot:\$LD_LIBRARY_PATH
export PYTHONPATH=\$ROOTSYS/lib:\$PYTHONPATH:\$ROOTSYS/bindings/pyroot" >> ~/.bashrc
gedit ~/.bashrc
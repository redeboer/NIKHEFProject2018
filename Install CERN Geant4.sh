# Author: Remco de Boer
# Data: 09/06/2018

# Instructions to install CERN's Geant 4.10.04 on a Linux system, with OpenGL visualization driver.

# Set version
GEANTv=4.10.04.p02
CLHEPv=2.4.0.4

# Ask for sudo rights
sudo echo

# Install prerequisites
sudo apt update
sudo apt upgrade -y
sudo apt install build-essential libexpat1-dev libxmu-dev cmake cmake-curses-gui qt5-default -y

# Download CLHEP distribution. See this page for the latest releases: http://proj-clhep.web.cern.ch/proj-clhep/
cd ~
mkdir clhep
cd clhep
wget "http://proj-clhep.web.cern.ch/proj-clhep/DISTRIBUTION/tarFiles/clhep-2.4.0.4.tgz"
tar -zxf clhep-$CLHEPv.tgz

# Set installation congifuration. Default installation path is /usr/local
read -p "
Upon pressing ENTER, cmake will be opened. Press 'c' to change parameters. Nothing needs to be changed here, so you can immediately press 'g'."
mkdir build
cd build
ccmake ../$CLHEPv/CLHEP/

# Build CLHEP
make -j$(nproc)
sudo make install

# Remove files
cd ../..
rm -rf clhep

# Download installation files for GEANT. The latest version of Geant4 can be found at https://geant4.web.cern.ch/support/download.
mkdir geant
cd geant
wget "http://geant4-data.web.cern.ch/geant4-data/releases/geant$GEANTv.tar.gz"
tar -zxf geant$GEANTv.tar.gz

# Configure
read -p "
Geant installation first needs to be configured. For an overview of the options, see http://geant4-userdoc.web.cern.ch/geant4-userdoc/UsersGuides/InstallationGuide/html/installguide.html#advanced-options. Particularly, pay attention to the visualization drivers. Some nice example screenshots are given here: https://ruyetao.com/ubuntu_geant4/.

Important options
  CMAKE_INSTALL_PREFIX      /usr/local/Geant4
  (this directory is important for the path environment!)
  GEANT4_USE_SYSTEM_CLHEP   ON
  GEANT4_USE_OPENGL_X11     ON
  GEANT4_USE_QT             ON

Press ENTER to continue, then 'e', and then 'c' to configure..."
mkdir build
cd build
ccmake ../geant$GEANTv

# Build GEANT
make -j$(nproc)
sudo make install

# Remove files
cd ../..
rm -rf geant

# Set environment variables. BE CAREFUL IF YOU COPY FROM THIS FILE: REMOVE \'s !!
sudo cp /usr/local/Geant4/bin/geant4.sh /etc/profile.d/
read -p "
Upon pressing ENTER, gedit will open \"/etc/profile.d/geant4.sh\". Please REPLACE the following lines:

  if [ -z “\$BASH_VERSION” ]; then
    # Not bash, so rely on sourcing from correct location
  if [ ! -f geant4.sh ]; then
    echo ‘ERROR: geant4.sh could NOT self-locate Geant4 installation’
    echo ‘This is most likely because you are using ksh, zsh or similar’
    echo ‘To fix this issue, cd to the directory containing this script’
    echo ‘and source it in that directory.’
    return 1
  fi
    geant4_envbindir=$(pwd)
    else
    g4sls_sourced_dir=$(dirname \${BASH_ARGV[0]})
    geant4_envbindir=$(cd \$g4sls_sourced_dir \> /dev/null \; pwd)
  fi

by the following:

  geant4_envbindir=\"/usr/local/Geant4/bin\""
gedit /etc/profile.d/geant4.sh
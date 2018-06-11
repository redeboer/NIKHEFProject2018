# Author: Remco de Boer
# Data: 09/06/2018

# Instructions to install CERN's Geant 4.10.04 on a Linux system, with OpenGL visualization driver.

# Check if in sudo
if [ "$EUID" -ne 0 ]
  then echo "Please run as root. To do so, use sudo."
  exit
fi

# Instructions
read -p "
This script will install the latest version of Geant4. If you do not know what you are doing, please abort and visit http://geant4-userdoc.web.cern.ch/geant4-userdoc/UsersGuides/InstallationGuide/html/. There are also nice instructions on https://ruyetao.com/ubuntu_geant4/.

Press ENTER to continue."

# Set version
GEANT=4.10.04
GEANTv=$GEANT.p02
CLHEPv=2.4.0.4

# Install prerequisites
apt update
apt upgrade -y
apt install build-essential libexpat1-dev libxmu-dev cmake cmake-curses-gui qt5-default -y

# Download CLHEP distribution. See this page for the latest releases: http://proj-clhep.web.cern.ch/proj-clhep/
cd ~
mkdir clhep
cd clhep
wget "http://proj-clhep.web.cern.ch/proj-clhep/DISTRIBUTION/tarFiles/clhep-$CLHEPv.tgz"
tar -zxf clhep-$CLHEPv.tgz

# Set installation congifuration. Default installation path is /usr/local
# read -p "
# Upon pressing ENTER, cmake will be opened. Press 'c' to change parameters. Nothing needs to be changed here, so you can immediately press 'g'."
mkdir build
cd build
cmake ../$CLHEPv/CLHEP/

# Build CLHEP
make -j$(nproc)
make install

# Remove files
cd ../..
rm -rf clhep

# Download installation files for GEANT. The latest version of Geant4 can be found at https://geant4.web.cern.ch/support/download.
mkdir geant
cd geant
wget "http://geant4-data.web.cern.ch/geant4-data/releases/geant$GEANTv.tar.gz"
tar -zxf geant$GEANTv.tar.gz

# Configure build
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=/usr/local -DGEANT4_BUILD_MULTITHREADED=ON -DGEANT4_USE_SYSTEM_CLHEP=ON -DCLHEP_DIR=/usr/local/lib/CLHEP-$CLHEPv -DGEANT4_USE_OPENGL_X11=ON -DGEANT4_USE_QT=ON -DGEANT4_INSTALL_DATA=ON ../geant$GEANTv

# Build GEANT
make -j$(nproc)
make install

# Remove files
cd ../..
rm -rf geant

# Get ownership
chown -R $(whoami):$(id -g -n $(whoami)) /usr/local/share/

# # Set environment variables
cp /usr/local/bin/geant4.sh /etc/profile.d/
echo "
# Geant4
source /usr/local/bin/geant4.sh" >> ~/.bashrc
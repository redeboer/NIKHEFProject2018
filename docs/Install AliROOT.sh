# Author: Remco de Boer
# Data: 09/06/2018

# Instructions to install CERN's AliRoot on Ubuntu 18.04, with ROOT5 (this is done to be able to work with PCG software packages). See https://alice-doc.github.io/alice-analysis-tutorial/building/ for further instructions.

# Check if in sudo
if [ "$EUID" -ne 0 ]
  then echo "Please run as root. To do so, use sudo."
  exit
fi

read -p "
This script will attempt to install AliRoot on your system. This is, however, a delicate process, so it is recommended that you do not use this script, but instead follow the online instructions carefully: https://alice-doc.github.io/alice-analysis-tutorial/building/."

read -p "
Sure you want to continue? (Press ENTER to continue.)"

# Install prerequisites. See https://alice-doc.github.io/alice-analysis-tutorial/building/prereq-ubuntu.html.
apt update -y
apt install -y curl libcurl4-gnutls-dev build-essential gfortran cmake libmysqlclient-dev xorg-dev libglu1-mesa-dev libfftw3-dev libssl1.0 libssl1.0-dev libxml2-dev git unzip python-pip autoconf automake autopoint texinfo gettext libtool libtool-bin pkg-config bison flex libperl-dev libbz2-dev libboost-all-dev swig liblzma-dev libnanomsg-dev libyaml-cpp-dev rsync lsb-release environment-modules
pip install matplotlib numpy certifi ipython==5.1.0 ipywidgets ipykernel notebook metakernel pyyaml

# Install or update aliBuild
pip install alibuild --upgrade

# Download AliRoot
cd /usr/local/
mkdir alice
chown -R $(whoami):$(id -g -n $(whoami)) alice # change ownership
cd alice

# Get AliRoot and AliPhysics from Git through aliBuild
aliBuild init AliRoot,AliPhysics -z aliroot5 # Run 2 software
cd aliroot5
# aliBuild init O2 --defaults o2 # Run 3 software

# Check if prerequisites have been met (DOCTOR)
aliDoctor AliPhysics --defaults user
# aliDoctor O2 --defaults o2

# Build AliRoot: use option -d if you want to see debug output
aliBuild -z -w ../sw build AliPhysics --defaults user # if ROOT6: user-root6
# aliBuild build O2 --defaults o2 # Run 3 software

# Add lines to bash
echo "

# AliRoot
export ALIBUILD_WORK_DIR=\"/usr/local/alice/sw\"
eval \"\`alienv shell-helper\`\"
alias ali='alienv enter AliPhysics/latest-aliR00T5-user'" >> ~/.bashrc

# Test installation
read -p "
Finished installing AliRoot!
To test AliRoot installation, run \"alienv q\" or \"ali\"."
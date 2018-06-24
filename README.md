# NIKHEF Project 2018

Thank you for your interest in this C++/ROOT framework analyse beam data. If you read this, you are probably a student at NIKHEF doing research on some particle physics project. If so, and you are in the preliminary stages of your research, this documentation could be useful, even if you eventually decide the framework cannot be implemented in your research.

We (master's students from the GRAPPA master's of the University of Amsterdam) developed this framework specifically for the NIKHEF Student Project from April to June 2018, but with the intention to develop it in such a way that it can be adepted for any other beam analysis setup.

## Acknowledgements

The NIKHEF Project 2018 was supervised by Daniel Hynds, who codeveloped [Corryvreckan](https://gitlab.cern.ch/dhynds/corryvreckan) and recommended us to follow its structure.

## Introduction

Data analysis in student projects (be it individually or as a group) usually result in code that is rather specific for that project. Other students who later continue with a similar project will typically start their own work from scratch or at most borrow snippets of code from the previous group. For students, this is not, of course, necessarily a problem, as you learn best by doing. Still, it would be great if problems that previous groups encountered can be optimally communicated to whoever later picks up on their work.

This framework not only tries to address this 'continuity problem', but also offers a model to efficiently develop code for beam data analysis as a group. The idea is that a procedure where data is analyised event-by-event can be split up into several algorithms, like reading data files, recognising tracks, andfitting them. Similarly, information in a beam event can be split up into different data containers, like calorimeter data or measurements from a TPC. The form of these algorithms and containers are specific to each project, but the general procedure is still the same: data is processed in several steps of a procedure.

This invites the idea of using a virtual base class for data objects and algorithms. Steps in any data analysis procedure are written in the form of a class that inherits from this base algorithm class, while data containers are written in the form of classes that inherit from the data object base class. These algorithms can then be loaded by an analysis class that reads, processes, and writes data objects from a clipboard. A crude scheme of the main classes (with some example derived classes) is shown below.

![Basic of class structure](https://github.com/redeboer/NIKHEFProject2018/blob/master/docs/structure_basic.png "Basic scheme of class structure")

## Description of class structure

As can be seen above, there are four main components:

1. **Core**: This consists of the class `TClipboard`, which is basically a map of pointers to data objects, and `TAnalysis`, which holds and runs the algorithms and manages reading and writing to the clipboard.
2. **Objects**: When a bit of information of an event in your beam setup is described in the form of a class that inherits from TBeamObject, it can be written to the clipboard. Other algorithms can in turn access these objects through `TAnalysis` and use them in whatever way necessary.
3. **Algorithms**: An algorithm is a step in your event analysis. As an algorithm inherits from `TAlgorithm`, it requires three methods that can be called by `TAnalysis`: Initialise (called once), Run (called for each event), and Finalise (called once at the end).
4. **Global parameters**: This is no class, but a namespace called `NIKHEFProject`, as to make it more easily accessible to any component of the framework. It also contains a subnamespace `NIKHEFProject::Detector` that can be used to store for instance global positions of your detectors.

These four components are contained withing **Steering**. This is just the main C++ function that defines the executable.

Now, the cool thing is that, as long as an algorithm passes on passes data through the clipboard, it doesn't matter what other algorithms do with its output. This allows your group members (or students who later pick up your work) to add their own algorithms or switch off certain algorithms (steps) in the overall analysis procedure.

## Description of setup for our project

In 2018, we worked on a setup to analyse a proton beam (the proton beam of the KVI in Groningen). Initially, we planned to work with two TPCs and one calorimeter as to compute scattering angle. The first TPC would be placed in front of the sample (phantom) and second TPC and calorimeter after. The whole setup was to be triggered with a scintillator. Linear tracks in both TPCs would be intersected to reconstruct the point where the scattering took place. (Of course, for charged particles, the scattering does not happen at one point in the phantom, but we wanted to know more about the final direction in which the particle is scattered after leaving the phantom.)

In preparation to the data taking, we also performed simulations of the entire setup in Geant. Output of these simulations was also analysed using this framework. The algorithms we developed were therefore loaded in the following way:

![Algorithm structure](https://github.com/redeboer/NIKHEFProject2018/blob/master/docs/structure_algorithms.png "Algorithm structure")

The order of processing these algorithms is, of course, sequentially, but the arrows here indicate which altorithms rely on the output of previous ones.

In our project, one of our TPCs unfortunately broke down, so in the end, we were not able to intersect tracks. The TPC after the sample was therefore removed, so the intersecting algorithms became redundant. Also, it turned out to be difficult to properly trigger the remaining TPC, so a the z-component could also not be computed. The output was therefore a 1-dimensional profile of energy loss in the sample.

To make things worse, event the one remaining TPC started to fail when taking data at the KVI. We have therefore not continued developing scripts like the mask generator and loader and have not developed any alignment procedure. We encourage the reader to pick up where we left.

## Usage

Compiling everything is done easily using `make`, which simply follows the `Makefile` script. You can then run the executable with or without options, like:

- `./execute`
- `./execute -i "<folder or txt file you want to analyse>"`
- `./execute -i "<folder you want to analyse>" -c "<location of calo file>"`
- `./execute -i "<folder you want to analyse>" -c "<location of calo file>" -o <output ROOT file>`

In the first case, the default input file as defined in `GlobalParameters.cxx` is used. The calorimeter data file name is then just set to 'input file name' + `_calo.txt` and the output file name `output/` + 'input file name' + `.root`. This behaviour can be changed in the paramters. Otherwise, you can specify the calo file name and/or the output file name using option `-c` and option `-o` respectively.

If you are on Linux and have no CERN ROOT installed, you will have to install it prior to `make`. For this, you run the shell script `Install CERN ROOT6.sh` under `docs` (or just read that script as a guide).

Note that `TTimepixLoader` uses `aunpack` to unzip `zip` or `tar` files. If you intent to work with zipped files, install `atool` using `sudo apt-get install atool`.
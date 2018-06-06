# NIKHEFProject2018

Thank you for your interest in this C++/ROOT framework analyse beam data. If you read this, you are probably a student at NIKHEF doing research on some particle physics project. If so, and you are in the preliminary stages of your research, this documentation could be useful, even if you eventually decide the framework cannot be implemented in your research.

We (master's students from the GRAPPA master's of the University of Amsterdam) developed this framework specifically for the NIKHEF Student Project from April to June 2018, but with the intention to develop it in such a way that it can be adepted for any other beam analysis setup.

## Acknowledgements
The NIKHEF Project 2018 was supervised by Daniel Hynds, who codeveloped [Corryvreckan](https://gitlab.cern.ch/dhynds/corryvreckan) and recommended us to follow its structure.

## Introduction
Data analysis in student projects (be it individually or as a group) usually result in code that is rather specific for that project. Other students who later continue with a similar project will typically start their own work from scratch and at most borrow snippets of code from the previous group. For students, this is not, of course, necessarily a problem, as you learn best by doing. Still, it would be great if problems that previous groups encountered can be optimally communicated to whoever later picks up on their work.

This framework not only tries to address this 'continuity problem', but also offers a model to efficiently develop code for beam data analysis as a group. The idea is that a procedure where data is analyised event-by-event can be split up into several algorithms, like reading data files, recognising tracks, and aligning your detectors. Similarly, information in a beam event can be split up into different data containers, like calorimeter data or measurements from a TPC. The form of these algorithms and containers are specific to each project, but the general procedure is still the same: data is processed in several steps of a procedure.

This invites the idea of using a virtual base class for data objects and algorithms. Steps in any data analysis procedure are written in the form of a class that inherits from this base algorithm class, while data containers are written in the form of classes that inherit from the data object base class. These algorithms can then be loaded by an analysis class that reads, processes, and writes data objects from a clipboard. A crude scheme of the main classes (with some example derived classes) is shown below.

![Basic of class structure](https://github.com/redeboer/NIKHEFProject2018/blob/master/docs/structure_basic.png "Basic scheme of class structure")

## Description of class structure
As can be seen above, there are four main components:
1. **Core**: This consists of the class *TClipboard*, which is basically a map of pointers to data objects, and *TAnalysis*, which holds and runs the algorithms and manages reading and writing to the clipboard.
2. **Objects**: When a bit of information of an event in your beam setup is described in the form of a class that inherits from TBeamObject, it can be written to the clipboard. Other algorithms can in turn access these objects through *TAnalysis* and use them in whatever way necessary.
3. **Algorithms**: An algorithm is a step in your event analysis. As an algorithm inherits from *TAlgorithm*, it requires three methods that can be called by *TAnalysis*: Initialise (called once), Run (called for each event), and Finalise (called once at the end).
4. **Global parameters**: This is no class, but a namespace called *NIKHEFProject*, as to make it more easily accessible to any component of the framework. It also contains a subnamespace *NIKHEFProject::Detector* that can be used to store for instance global positions of your detectors.

These four components are contained withing **Steering**. This is just the main C++ function that defines the executable.

Now, the cool thing is that, as long as an algorithm passes on passes data through the clipboard, it doesn't matter what other algorithms do with the output. This allows your group members (or students who later pick up your work) to add their own algorithms or switch off certain algorithms (steps) in the overall analysis procedure.

## Usage
Compiling everything is done easily using `make`, which simply follows the `Makefile` script. You can then run the executable with or without options, like:
- `./execute`
- `./execute -i "<folder or txt file you want to analyse>"`
- `./execute -i "<folder you want to analyse>" -c <location of calo file>`

If you are on Linux and have no CERN ROOT installed, you will have to install it prior to `make`. For this, you run the shell script `Install CERN ROOT6.sh` (or just read that script as a guide).
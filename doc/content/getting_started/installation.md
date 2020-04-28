## Installation

!alert note title=CRANE comes prepackaged with Zapdos  
If the user is interested in modeling plasma transport, [Zapdos](https://github.com/shannon-lab/zapdos) is the appropriate software. The latest stable version of CRANE comes prepackaged as a submodule of Zapdos, so CRANE does not need to be separately installed. These steps are only necessary if the user intends to run CRANE by itself or contribute to the software.

The CRANE source code is available on [GitHub](https://github.com/lcpp-org/crane). It must be downloaded and compiled before using. Instructions for downloading and installing CRANE are shown below.

To install CRANE, first the MOOSE framework must be included. [Follow the instructions on the MOOSE website](https://mooseframework.org/getting_started/index.html) for installing and compiling MOOSE and libmesh on your operating system. Once MOOSE is installed, run the following commands in a terminal:

```bash
mkdir ~/projects
cd ~/projects
git clone https://github.com/lcpp-org/crane.git
cd crane
git checkout master
```

CRANE must then be compiled and tested to ensure the installation was successful: 

```bash
cd ~/projects/crane
make -j n
./run_tests -j n
```

where 'n' is the number of processes your CPU can run. 

## Contributing to CRANE

CRANE is an open source software, and as such contributions are welcome! If you wish to contribute to the software, it is recommended that you fork the [CRANE repository](https://github.com/lcpp-org/crane) to your own GitHub account. Then follow the same instructions listed above, but clone from your forked repository: 

```bash
git clone https://github.com/[your-repo-here]/crane.git
```

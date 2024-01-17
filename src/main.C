//* This file is part of Crane, an open-source
//* application for plasma chemistry and thermochemistry
//* https://github.com/lcpp-org/crane
//*
//* Crane is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "CraneApp.h"
#include "MooseMain.h"

// Create a performance log
PerfLog Moose::perf_log("Crane");

// Begin the main program.
int
main(int argc, char * argv[])
{
  Moose::main<CraneApp>(argc, argv);

  return 0;
}

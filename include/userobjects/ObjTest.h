#pragma once

#include "GeneralUserObject.h"

class ObjTest : public GeneralUserObject
{
public:
  ObjTest(const InputParameters & parameters);

  static InputParameters validParams();

  Real reaction_coefficient() const;

  virtual void initialize();

  virtual void execute();

  virtual void finalize();

protected:
  Real _reaction_coefficient;
};

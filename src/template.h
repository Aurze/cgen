#ifndef TEMPLATE_H
#define TEMPLATE_H

#include "args.hxx"
#include <string>

class CodeTemplate {
public:
  std::string name;

  virtual void init(args::Subparser &) = 0;
  virtual void generate(args::Subparser &) = 0;
  virtual void build(args::Subparser &) = 0;
  virtual void test(args::Subparser &) = 0;
  virtual void run(args::Subparser &) = 0;
  virtual void install(args::Subparser &) = 0;
};

#endif

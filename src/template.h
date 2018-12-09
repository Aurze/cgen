#ifndef TEMPLATE_H
#define TEMPLATE_H

#include "args.hxx"
#include <string>

class CodeTemplate {
public:
  std::string name;

  virtual void init(args::Subparser &) = 0;
  virtual void generate() = 0;
  virtual void build() = 0;
  virtual void test() = 0;
  virtual void run() = 0;
  virtual void install() = 0;
};

#endif

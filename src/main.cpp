#include "args.hxx"
#include <filesystem>
#include <fstream>
#include <iostream>

#include <array>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "cppclitemplate.h"
#include "template.h"

std::vector<CodeTemplate *> templates;

void CommitCommand(args::Subparser &parser) { templates[0]->init(parser); }

std::string exec(const char *cmd) {
  std::array<char, 128> buffer;
  std::string result;
  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
  if (!pipe)
    throw std::runtime_error("popen() failed!");
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
    result += buffer.data();
  return result;
}

void BuildCommand(args::Subparser &parser) {
  std::cout << exec("cd build") << std::endl;
  std::cout << exec("conan install ..") << std::endl;
  std::cout << exec("cmake .. -G \"Ninja\" ") << std::endl;
  std::cout << exec("cmake --build .") << std::endl;
  std::cout << exec("ctest") << std::endl;
}

int main(int argc, const char **argv) {
  CppCli c;
  templates.push_back(&c);

  args::ArgumentParser p("Code Generator");
  args::Group commands(p, "commands");
  args::Command cnew(commands, "new", "Create a project from a template",
                     &CommitCommand);
  args::Command generate(commands, "generate",
                         "Generate something in a project", &CommitCommand);
  args::Command buid(commands, "build", "Build the project", &BuildCommand);
  args::Command test(commands, "test", "Execute test suits", &CommitCommand);
  args::Command run(commands, "run", "Run the project", &CommitCommand);
  args::Command install(commands, "install", "Install the project",
                        &CommitCommand);
  args::Group arguments(p, "arguments");
  args::Flag foo(arguments, "version", "Get the current version",
                 {'v', "version"});
  args::HelpFlag help(p, "help", "Display this help menu", {'h', "help"});

  try {
    p.ParseCLI(argc, argv);
  } catch (args::Help) {
    std::cout << p;
  } catch (args::Completion &e) {
    std::cout << e.what();
  } catch (args::Error &e) {
    std::cerr << e.what() << std::endl << p;
    return 1;
  }
  return 0;
}

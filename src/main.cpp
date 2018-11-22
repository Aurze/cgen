#include "args.hxx"
//#include <experimental/filesystem>
#include <filesystem>
#include <fstream>
#include <iostream>

void find_and_replace(std::string origin, std::string dest) {
  std::cout << "origin:" << origin << std::endl;
  std::cout << "dest:" << dest << std::endl;
  std::ifstream in(origin);
  std::ofstream out(dest);
  std::string param = "{{project_name}}";
  std::string value = "Test";

  if (!in) {
    std::cerr << "Could not open " << origin << "\n";
    return;
  }

  if (!out) {
    std::cerr << "Could not open " << dest << "\n";
    return;
  }

  std::string line;
  std::size_t len = param.length();
  while (getline(in, line)) {
    while (true) {
      size_t pos = line.find(param);
      if (pos != std::string::npos)
        line.replace(pos, len, value);
      else
        break;
    }

    out << line << '\n';
  }
  std::filesystem::remove(std::filesystem::path(origin));
}

static void remove_str(std::string &original, const std::string &kickout) {
  std::string::size_type pos = 0u;
  while ((pos = original.find(kickout, pos)) != std::string::npos) {
    original.replace(pos, kickout.length(), "");
  }
}

template <class func>
void directory_iterator_rec(std::filesystem::path path, func f) {
  for (auto &p : std::filesystem::directory_iterator(path)) {
    std::cout << p << std::endl;
    if (p.is_directory()) {
      std::cout << "is_directory" << std::endl;
      directory_iterator_rec(p, f);
    } else if (p.path().filename().string().find("{{") == 0) {
      std::cout << "to replace" << std::endl;
      std::string p_str(p.path());
      remove_str(p_str, "}}");
      remove_str(p_str, "{{");
      f(p.path(), p_str);
    }
  }
}

void CommitCommand(args::Subparser &parser)
{
  args::ValueFlag<std::string> message(parser, "MESSAGE", "commit message",
                                       {'m'});
  parser.Parse();

  if (message) {
    std::cout << "Commit";
  } else {
    std::cout << parser;
  }

  std::cout << std::endl;

  if (message) {
    std::cout << "message: " << args::get(message) << std::endl;
  }

  std::cout << std::filesystem::current_path() << std::endl;
  std::filesystem::path p("/home/nicolas/devel/cgen/template");

  std::filesystem::copy(p, std::filesystem::current_path(),
                        std::filesystem::copy_options::skip_existing |
                            std::filesystem::copy_options::recursive);

  directory_iterator_rec(std::filesystem::current_path(), &find_and_replace);
}

int main(int argc, const char **argv)
{
  args::ArgumentParser p("Code Generator");
  args::Group commands(p, "commands");
  args::Command cnew(commands, "new", "Create a project from a template",
                     &CommitCommand);
  args::Command generate(commands, "generate",
                         "Generate something in a project", &CommitCommand);
  args::Command buid(commands, "build", "Build the project", &CommitCommand);
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

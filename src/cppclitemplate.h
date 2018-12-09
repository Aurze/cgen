#ifndef CPPCLI_TEMPLATE_H
#define CPPCLI_TEMPLATE_H

#include "args.hxx"
#include "template.h"
#include <string>
#include <utility>

void find_and_replace(std::string origin, std::string dest,
                      std::vector<std::pair<std::string, std::string>> params) {
  std::cout << "origin:" << origin << std::endl;
  std::cout << "dest:" << dest << std::endl;
  std::ifstream in(origin);
  std::ofstream out(dest);

  if (!in) {
    std::cerr << "Could not open " << origin << "\n";
    return;
  }

  if (!out) {
    std::cerr << "Could not open " << dest << "\n";
    return;
  }

  std::string line;
  for (const auto &param : params) {
    std::string name = param.first;
    std::string value = param.second;
    std::size_t len = name.length();
    while (getline(in, line)) {
      while (true) {
        size_t pos = line.find(name);
        if (pos != std::string::npos)
          line.replace(pos, len, value);
        else
          break;
      }

      out << line << '\n';
    }
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
  std::vector<std::pair<std::string, std::string>> params;

  params.emplace_back("{{project_name}}", "test");

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
      f(p.path(), p_str, params);
    }
  }
}

class CppCli : public CodeTemplate {
public:
  std::string name;

  void init(args::Subparser &parser) override {
    args::ValueFlag<std::string> name(parser, "NAME", "Project name",
                                      {'n', "name"});
    parser.Parse();

    if (name) {
      std::cout << "Name";
    } else {
      std::cout << parser;
    }

    std::cout << std::endl;

    if (name) {
      this->name = args::get(name);
    }

    std::cout << std::filesystem::current_path() << std::endl;
    std::filesystem::path p("/home/nicolas/devel/cgen/template/c++/cli");

    std::filesystem::copy(p, std::filesystem::current_path(),
                          std::filesystem::copy_options::skip_existing |
                              std::filesystem::copy_options::recursive);

    directory_iterator_rec(std::filesystem::current_path(), &find_and_replace);
  };

  void generate() override{};

  void build() override{
    std::string cmd =
        "docker run --rm -it -v $(pwd):/project -v ~/.conan:/.conan --user "
        "$(id -u):$(id -g) tm_build_gcc \"mkdir -p build && cd build && "
        "conan install --build missing .. && cmake -G Ninja .. && ninja\"";
  };

  void test() override{};
  void run() override{};
  void install() override{};
};

#endif

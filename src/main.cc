#include "utility_cli/utility_cli.cc"

using s21::UtilityCLI;
using std::cerr;
using std::cout;
using std::invalid_argument;
using std::string;

int main(int argc, const char* argv[]) {
  int exit_code = 1;
  try {
    UtilityCLI utility(argc, argv);
    utility.Exec();
    exit_code = 0;
  } catch (invalid_argument ex) {
    cerr << "Input command error: " << ex.what() << "\n";
  } catch (string ex) {
    cerr << "Algorithm execution error: " << ex << "\n";
  } catch (...) {
    cerr << "Unknown execution error occurred.\n";
  }
  return exit_code;
}

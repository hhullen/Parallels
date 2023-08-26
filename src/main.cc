#include "utility_cli/utility_cli.cc"

using s21::UtilityCLI;
using std::cerr;
using std::cout;
using std::exception;
using std::invalid_argument;
using std::string;

int main(int argc, const char* argv[]) {
  int exit_code = 1;
  UtilityCLI utility(argc, argv);
  try {
    utility.Exec();
    exit_code = 0;
  } catch (const invalid_argument& ex) {
    cerr << "Input command error: " << ex.what() << "\n";
  } catch (const string& ex) {
    cerr << "Algorithm execution error: " << ex << "\n";
  } catch (const exception& ex) {
    cerr << "Exception occurred: " << ex.what() << "\n";
  }
  return exit_code;
}

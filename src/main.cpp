
#include "CSV_reader.h"
using namespace std;

int main(int argc,char** argv) {
    CSV_reader reader (argv[1]);
    reader.Read();
    reader.Parse();
    reader.Write();
}


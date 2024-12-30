#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
//#include <vector>
//#include <future>
//#include <mutex>

using namespace std;

#include "helper.h"
#include "stringthings.h"
#include "system.h"

// ------------------------------------------------------------------------- //

int main_loop(string FileName);
int main(int argc, char* argv[]);

#endif //MAIN_H
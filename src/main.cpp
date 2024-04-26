#include <iostream>
#include "MainApp.h"
int main(int argc, char *argv[])
{
    theApp::Instance()->init(argc, argv);
    return theApp::Instance()->run();
}

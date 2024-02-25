#include "FormMainWindow.h"
#include "EarBug.h"

using namespace dd::forms;

//TODO This needs a lot of cleaning
int main(int argc, char *argv[]) {
    dd::earbug::EarBug eb;
    return eb.start(argc, argv);
}

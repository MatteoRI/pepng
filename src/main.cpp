#include <pepng.h>

int main() {
    if (!pepng::init("PEPNG", 1920, 1080)) return -1;

    return pepng::update();
}
#include"select.h"

int main(int argc, char *argv[])
{
    Select ser(8888);
    ser.loop();
    return 1;
}

#include <iostream>

#include <gflags/gflags.h>

DEFINE_int32(answer, 0, "what is the answer?");

int main(int argc, char **argv) {
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    std::cout << "the answer is " << FLAGS_answer << std::endl;
    return 0;
}
#include <gflags/gflags.h>
#include <glog/logging.h>

DEFINE_int32(answer, 0, "what is the answer?");

int main(int argc, char **argv) {
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    LOG(INFO) <<"the answer is " << FLAGS_answer;
    return 0;
}
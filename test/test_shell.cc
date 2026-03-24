#include "cpputils/shell.h"

#include <string>
#include <gtest/gtest.h>

#ifdef _WIN32

TEST(ShellTest, SimpleCommand) {
    EXPECT_EQ(0, Cpputils::Shell("dir"));
}

TEST(ShellTest, CaptureOutput) {
    std::string output;
    EXPECT_EQ(0, Cpputils::Shell("dir .", output));
    EXPECT_TRUE(output.find("Directory") != std::string::npos);
}

#else

TEST(ShellTest, SimpleCommand) {
    EXPECT_EQ(0, Cpputils::Shell("ls"));
}

TEST(ShellTest, CaptureOutput) {
    std::string output;
    EXPECT_EQ(0, Cpputils::Shell("ls -alh .", output));
    EXPECT_NE(std::string::npos, output.find("total"));
}

TEST(ShellTest, CaptureStderr) {
    std::string output;
    EXPECT_NE(0, Cpputils::Shell("tdd -alh .", output, true));
    EXPECT_NE(std::string::npos, output.find("not found"));
}

TEST(ShellTest, StderrNotCapturedByDefault) {
    std::string output;
    Cpputils::Shell("ls /nonexistent_dir_abc123", output, false);
    EXPECT_EQ(std::string::npos, output.find("No such file"));
}

TEST(ShellTest, NonZeroExitCode) {
    EXPECT_NE(0, Cpputils::Shell("false"));
}

TEST(ShellTest, NonZeroExitCodeWithOutput) {
    std::string output;
    int rc = Cpputils::Shell("false", output);
    EXPECT_NE(0, rc);
}

TEST(ShellTest, EchoOutput) {
    std::string output;
    EXPECT_EQ(0, Cpputils::Shell("echo hello", output));
    EXPECT_NE(std::string::npos, output.find("hello"));
}

TEST(ShellTest, MultilineOutput) {
    std::string output;
    EXPECT_EQ(0, Cpputils::Shell("printf 'line1\\nline2\\nline3'", output));
    EXPECT_NE(std::string::npos, output.find("line1"));
    EXPECT_NE(std::string::npos, output.find("line2"));
    EXPECT_NE(std::string::npos, output.find("line3"));
}

TEST(ShellTest, ExitCodeFromCommand) {
    std::string output;
    EXPECT_EQ(42, Cpputils::Shell("exit 42", output));
}

TEST(ShellTest, EmptyOutput) {
    std::string output;
    EXPECT_EQ(0, Cpputils::Shell("true", output));
    EXPECT_TRUE(output.empty());
}

#endif

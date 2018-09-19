#ifndef GAME_CONFIG_h
#define GAME_CONFIG_h
using namespace std;
#define CLASS_NAME "com/rwhz/jni/"
#define PACKAGE_NAME com_rwhz_jni
#define define_native_function_exec(a, b) Java_##a##_##b
#define define_native_function(a, b) define_native_function_exec(a, b)
#endif
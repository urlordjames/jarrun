#include "execjar.h"

#ifndef _WIN32
char* appendCString(const char *begin, const char *end) {
	char *newstr = strdup(begin);
	// reallocate with additional memory for terminating null byte
	newstr = (char*) realloc(newstr, strlen(begin) + strlen(end) + 1);
	strcat(newstr, end);
	return newstr;
}
#endif

void runJar(std::filesystem::path path, std::string classpath) {
#ifndef _WIN32
// if not on Windows invoke Java Native Interface
	JavaVMOption *options = new JavaVMOption[1];

	char *option = appendCString("-Djava.class.path=", path.c_str());

	options[0].optionString = option;

	JavaVMInitArgs vm_args;
	vm_args.options = options;
	vm_args.nOptions = 1;
	vm_args.version = JNI_VERSION_1_8;
	vm_args.ignoreUnrecognized = false;

	JavaVM *jvm;
	JNIEnv *env;

	int ret = JNI_CreateJavaVM(&jvm, (void**) &env, &vm_args);
	delete[] options;

	jclass clazz = env->FindClass(classpath.c_str());
	jmethodID meth = env->GetStaticMethodID(clazz, "main", "([Ljava/lang/String;)V");
	jarray args = env->NewObjectArray(0, env->FindClass("java/lang/String"), 0);

	env->CallStaticVoidMethod(clazz, meth, args);

	jvm->DestroyJavaVM();

#else
// otherwise do the hacky Windows way
	if (system(NULL)) {
		// this may have been less troublesome to do with std::string but it's too late now...
		const char *prepend = "java -cp ";
		// size for three strings, an extra space, and a terminating null byte
		size_t len = strlen(prepend) + path.string().length() + 1 + classpath.length() + 1;
		char *cmd = (char *) calloc(sizeof(char), len);

		strcpy(cmd, prepend);
		strcat(cmd, path.string().c_str());
		strcat(cmd, " ");
		strcat(cmd, classpath.c_str());

		int result = system(cmd);
		free(cmd);
		if (result != 0) exit(1);
	} else exit(1);
#endif
}

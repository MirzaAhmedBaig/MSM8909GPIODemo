#include <jni.h>
#include <string>

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define IN  0
#define OUT 1

#define LOW  0
#define HIGH 1

#define PIN  944 /* 33 + offset (911) */
#define POUT 942  /* 31 + offset (911) */

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_root_nativeappdemo_MainActivity_mymethod(JNIEnv *env, jobject object){


    jstring jstr = env->NewStringUTF("Hello from JNI LIB, This is your first Call to C code");
    jclass clazz = env->FindClass("com/example/root/nativeappdemo/MainActivity");
    jmethodID messageMe = env->GetMethodID(clazz, "callMeFromJNI", "(Ljava/lang/String;)V");
    /*jobject result = */env->CallVoidMethod(object, messageMe, jstr);

    /*const char* str = env->GetStringUTFChars((jstring) result, NULL);
    printf("%s\n", str);
    // Clean up
    env->ReleaseStringUTFChars(jstr, str);*/

    return env->NewStringUTF("Hello from JNI LIB, This is your first Call to C code");
}






static int
GPIOExport(int pin)
{
#define BUFFER_MAX 3
    char buffer[BUFFER_MAX];
    ssize_t bytes_written;
    int fd;

    fd = open("/sys/class/gpio/export", O_WRONLY);
    if (-1 == fd) {
        fprintf(stderr, "Failed to open export for writing!\n");
        return(-1);
    }

    bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
    write(fd, buffer, bytes_written);
    close(fd);
    return(0);
}

static int
GPIOUnexport(int pin)
{
    char buffer[BUFFER_MAX];
    ssize_t bytes_written;
    int fd;

    fd = open("/sys/class/gpio/unexport", O_WRONLY);
    if (-1 == fd) {
        fprintf(stderr, "Failed to open unexport for writing!\n");
        return(-1);
    }

    bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
    write(fd, buffer, bytes_written);
    close(fd);
    return(0);
}

static int
GPIODirection(int pin, int dir)
{
    static const char s_directions_str[]  = "in\0out";

#define DIRECTION_MAX 35
    char path[DIRECTION_MAX];
    int fd;

    snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction", pin);
    fd = open(path, O_WRONLY);
    if (-1 == fd) {
        fprintf(stderr, "Failed to open gpio direction for writing!\n");
        return(-1);
    }

    if (-1 == write(fd, &s_directions_str[IN == dir ? 0 : 3], IN == dir ? 2 : 3)) {
        fprintf(stderr, "Failed to set direction!\n");
        return(-1);
    }

    close(fd);
    return(0);
}

static int
GPIORead(int pin)
{
#define VALUE_MAX 30
    char path[VALUE_MAX];
    char value_str[3];
    int fd;

    snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
    fd = open(path, O_RDONLY);
    if (-1 == fd) {
        fprintf(stderr, "Failed to open gpio value for reading!\n");
        return(-1);
    }

    if (-1 == read(fd, value_str, 3)) {
        fprintf(stderr, "Failed to read value!\n");
        return(-1);
    }

    close(fd);

    return(atoi(value_str));
}

static int
GPIOWrite(int pin, int value)
{
    static const char s_values_str[] = "01";

    char path[VALUE_MAX];
    int fd;

    snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
    fd = open(path, O_WRONLY);
    if (-1 == fd) {
        fprintf(stderr, "Failed to open gpio value for writing!\n");
        return(-1);
    }

    if (1 != write(fd, &s_values_str[LOW == value ? 0 : 1], 1)) {
        fprintf(stderr, "Failed to write value!\n");
        return(-1);
    }

    close(fd);
    return(0);
}



extern "C" JNIEXPORT jboolean JNICALL
Java_com_example_root_nativeappdemo_MainActivity_initLED(JNIEnv *env, jobject object){

    /*
     * Enable GPIO pins
     */
    if (-1 == GPIOExport(POUT) || -1 == GPIOExport(PIN))
        return static_cast<jboolean>(false);

    /*
     * Set GPIO directions
     */
    if (-1 == GPIODirection(POUT, OUT) || -1 == GPIODirection(PIN, IN))
        return static_cast<jboolean>(false);

    GPIOWrite(POUT,1);
    jboolean statusOut = static_cast<jboolean>(false);
    int status = GPIORead(POUT);
    printf("@Status: %d",status);
    statusOut = static_cast<jboolean>((status != 1));
    return statusOut;
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_example_root_nativeappdemo_MainActivity_toggleLED(JNIEnv *env, jobject object){

    //get status
    //toggle status
    //return status
    jboolean statusOut = static_cast<jboolean>(false);
    int status = GPIORead(POUT);
    printf("@Status: %d",status);
    if(status==1){
        status=0;
        statusOut = static_cast<jboolean>(false);

    }
    else{
        status=1;
        statusOut = static_cast<jboolean>(true);
    }
    GPIOWrite(POUT,status);
    return statusOut;
}


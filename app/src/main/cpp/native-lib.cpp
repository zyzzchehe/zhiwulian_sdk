#include <jni.h>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/i2c-dev.h>
#include <linux/spi/spidev.h>

#define SLAVE_ADDRESS 0x50
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

int fd = -1;

//extern "C" JNIEXPORT jstring JNICALL
//Java_com_mixlinker_sdk_NativeInterface_stringFromJNI(
//        JNIEnv* env,
//        jobject /* this */) {
//    std::string hello = "Hello from C++";
//    return env->NewStringUTF(hello.c_str());
//}
extern "C"
JNIEXPORT void JNICALL
Java_com_mixlinker_sdk_NativeInterface_i2c_1open(JNIEnv *env, jobject thiz, jstring device) {
    // open device file
    fd = open(reinterpret_cast<const char *>(device), O_RDWR);
    if(fd < 0){
        printf("Open file error\n");
        goto Exit;

        Exit:
        close(fd);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_mixlinker_sdk_NativeInterface_i2c_1write(JNIEnv *env, jobject thiz, jchar addr,
                                                 jcharArray data, jshort len) {
    unsigned char* write_data = static_cast<unsigned char *>(malloc(len + 1));

    write_data[0] = addr;
    memcpy(&write_data[1], data, len);

    ioctl(fd, I2C_SLAVE, SLAVE_ADDRESS);
    ioctl(fd, I2C_TIMEOUT, 1);
    ioctl(fd, I2C_RETRIES, 1);

    write(fd, write_data, len + 1);

    printf("write data success\n");

    if(write_data != NULL)
    {
        free(write_data);
        write_data = NULL;
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_mixlinker_sdk_NativeInterface_i2c_1read(JNIEnv *env, jobject thiz, jchar addr, jcharArray data,
                                                jshort len) {

    ioctl(fd, I2C_SLAVE, SLAVE_ADDRESS);
    ioctl(fd, I2C_TIMEOUT, 1);
    ioctl(fd, I2C_RETRIES, 1);

    write(fd, &addr, 1);

    read(fd, data, len);

    printf("buf[0] = 0x%x\n", data[0]);

    printf("Read data success\n");
}

// spi begin
static const char *device = "/dev/spidev1.1";
static uint8_t mode;
static uint8_t bits = 8;
static uint32_t speed = 500000;
static uint16_t delay;

extern "C"
JNIEXPORT void JNICALL
Java_com_mixlinker_sdk_NativeInterface_spi_1open(JNIEnv *env, jobject thiz) {
    fd = open(device, O_RDWR);
    if (fd < 0){
        printf("can't open device");
        close(fd);
    }
}

extern "C"
JNIEXPORT jbyteArray JNICALL
Java_com_mixlinker_sdk_NativeInterface_spi_1transfer(JNIEnv *env, jobject thiz, jbyteArray tx) {
    int ret;
//    uint8_t tx[] = {
//            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//            0x40, 0x00, 0x00, 0x00, 0x00, 0x95,
//            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//            0xDE, 0xAD, 0xBE, 0xEF, 0xBA, 0xAD,
//            0xF0, 0x0D,
//    };
    uint8_t rx[ARRAY_SIZE(tx)] = {0, };
    struct spi_ioc_transfer tr = {
            .tx_buf = (unsigned long)tx,
            .rx_buf = (unsigned long)rx,
            .len = ARRAY_SIZE(tx),
            .delay_usecs = delay,
            .speed_hz = speed,
            .bits_per_word = bits,
    };

    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
    if (ret == 1)
        printf("can't send spi message");

    for (ret = 0; ret < ARRAY_SIZE(tx); ret++) {
        if (!(ret % 6))
            printf("%.2X ", rx[ret]);
    }
    close(fd);
    return reinterpret_cast<jbyteArray>(rx);
}
// spi end

// gpio begin
extern "C"
JNIEXPORT jint JNICALL
Java_com_mixlinker_sdk_NativeInterface_gpio_1set(JNIEnv *env, jobject thiz, jint index, jint value) {
    switch (index){
        case 0:
            fd = open("/sys/class/igb300_gpio/gpio0/value",O_RDWR);
            if(fd < 0){
                printf("open failed");
                return -1;
            }
            if(value == 0){
                write(fd, "10", strlen("10"));
            }else if(value == 1){
                write(fd, "11", strlen("11"));
            }else{
                printf("invalid params");
            }
            close(fd);
            return 0;
        case 1:
            fd = open("/sys/class/igb300_gpio/gpio1/value",O_RDWR);
            if(fd < 0){
                printf("open failed");
                return -1;
            }
            if(value == 0){
                write(fd, "10", strlen("10"));
            }else if(value == 1){
                write(fd, "11", strlen("11"));
            }else{
                printf("invalid params");
            }
            close(fd);
            return 0;
        case 2:
            fd = open("/sys/class/igb300_gpio/gpio2/value",O_RDWR);
            if(fd < 0){
                printf("open failed");
                return -1;
            }
            if(value == 0){
                write(fd, "10", strlen("10"));
            }else if(value == 1){
                write(fd, "11", strlen("11"));
            }else{
                printf("invalid params");
            }
            close(fd);
            return 0;
        case 3:
            fd = open("/sys/class/igb300_gpio/gpio3/value",O_RDWR);
            if(fd < 0){
                printf("open failed");
                return -1;
            }
            if(value == 0){
                write(fd, "10", strlen("10"));
            }else if(value == 1){
                write(fd, "11", strlen("11"));
            }else{
                printf("invalid params");
            }
            close(fd);
            return 0;
        case 4:
            fd = open("/sys/class/igb300_gpio/gpio4/value",O_RDWR);
            if(fd < 0){
                printf("open failed");
                return -1;
            }
            if(value == 0){
                write(fd, "10", strlen("10"));
            }else if(value == 1){
                write(fd, "11", strlen("11"));
            }else{
                printf("invalid params");
            }
            close(fd);
            return 0;
        case 5:
            fd = open("/sys/class/igb300_gpio/gpio5/value",O_RDWR);
            if(fd < 0){
                printf("open failed");
                return -1;
            }
            if(value == 0){
                write(fd, "10", strlen("10"));
            }else if(value == 1){
                write(fd, "11", strlen("11"));
            }else{
                printf("invalid params");
            }
            close(fd);
            return 0;
        case 6:
            fd = open("/sys/class/igb300_gpio/gpio6/value",O_RDWR);
            if(fd < 0){
                printf("open failed");
                return -1;
            }
            if(value == 0){
                write(fd, "10", strlen("10"));
            }else if(value == 1){
                write(fd, "11", strlen("11"));
            }else{
                printf("invalid params");
            }
            close(fd);
            return 0;
        case 7:
            fd = open("/sys/class/igb300_gpio/gpio7/value",O_RDWR);
            if(fd < 0){
                printf("open failed");
                return -1;
            }
            if(value == 0){
                write(fd, "10", strlen("10"));
            }else if(value == 1){
                write(fd, "11", strlen("11"));
            }else{
                printf("invalid params");
            }
            close(fd);
            return 0;

        case 8:
            fd = open("/sys/class/igb300_gpio/gpio8/value",O_RDWR);
            if(fd < 0){
                printf("open failed");
                return -1;
            }
            if(value == 0){
                write(fd, "10", strlen("10"));
            }else if(value == 1){
                write(fd, "11", strlen("11"));
            }else{
                printf("invalid params");
            }
            close(fd);
            return 0;

        default:
            return -1;
    }
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_mixlinker_sdk_NativeInterface_gpio_1get(JNIEnv *env, jobject thiz, jint index) {
    int fd = 0;
    char buf[10]={'0'};
    switch(index) {

        case 0:
            fd = open("/sys/class/igb300_gpio/gpio0/value",O_RDONLY);
            if(fd == -1) {
                printf("error is %s\n", strerror(errno));
                return NULL;
            }
            read(fd, buf, sizeof(buf)-1);
            close(fd);
            return env->NewStringUTF(buf);

        case 1:
            fd = open("/sys/class/igb300_gpio/gpio1/value",O_RDONLY);
            if(fd == -1) {
                printf("error is %s\n", strerror(errno));
                return NULL;
            }
            read(fd, buf, sizeof(buf)-1);
            close(fd);
            return env->NewStringUTF(buf);

        case 2:
            fd = open("/sys/class/igb300_gpio/gpio2/value",O_RDONLY);
            if(fd == -1) {
                printf("error is %s\n", strerror(errno));
                return NULL;
            }
            read(fd, buf, sizeof(buf)-1);
            close(fd);
            return env->NewStringUTF(buf);

        case 3:
            fd = open("/sys/class/igb300_gpio/gpio3/value",O_RDONLY);
            if(fd == -1) {
                printf("error is %s\n", strerror(errno));
                return NULL;
            }
            read(fd, buf, sizeof(buf)-1);
            close(fd);
            return env->NewStringUTF(buf);

        case 4:
            fd = open("/sys/class/igb300_gpio/gpio4/value",O_RDONLY);
            if(fd == -1) {
                printf("error is %s\n", strerror(errno));
                return NULL;
            }
            read(fd, buf, sizeof(buf)-1);
            close(fd);
            return env->NewStringUTF(buf);

        case 5:
            fd = open("/sys/class/igb300_gpio/gpio5/value",O_RDONLY);
            if(fd == -1) {
                printf("error is %s\n", strerror(errno));
                return NULL;
            }
            read(fd, buf, sizeof(buf)-1);
            close(fd);
            return env->NewStringUTF(buf);

        case 6:
            fd = open("/sys/class/igb300_gpio/gpio6/value",O_RDONLY);
            if(fd == -1) {
                printf("error is %s\n", strerror(errno));
                return NULL;
            }
            read(fd, buf, sizeof(buf)-1);
            close(fd);
            return env->NewStringUTF(buf);

        case 7:
            fd = open("/sys/class/igb300_gpio/gpio7/value",O_RDONLY);
            if(fd == -1) {
                printf("error is %s\n", strerror(errno));
                return NULL;
            }
            read(fd, buf, sizeof(buf)-1);
            close(fd);
            return env->NewStringUTF(buf);

        case 8:
            fd = open("/sys/class/igb300_gpio/gpio8/value",O_RDONLY);
            if(fd == -1) {
                printf("error is %s\n", strerror(errno));
                return NULL;
            }
            read(fd, buf, sizeof(buf)-1);
            close(fd);
            return env->NewStringUTF(buf);

        default:
            return NULL;
    }
}
// gpio end


// can begin



// can end
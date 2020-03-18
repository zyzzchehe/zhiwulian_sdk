package com.mixlinker.sdk;

public class NativeInterface {

    static {
        System.loadLibrary("libnative-lib");
    }

    public native void i2c_open(String device);

    public native void i2c_write(char addr,char[] data,short len);

    public native void i2c_read(char addr,char[] data,short len);

    public native void spi_open();

    public native byte[] spi_transfer(byte[] data);

    public native int gpio_set(int index,int value);

    public native String gpio_get(int index);
}

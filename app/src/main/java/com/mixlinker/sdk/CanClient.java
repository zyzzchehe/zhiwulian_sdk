package com.mixlinker.sdk;

import android.os.IFlexcanService;
import android.os.RemoteException;

public class CanClient {

    private IFlexcanService mIFlexcanService = null;
    private int mId;
    private OnDataReceivedListener mListener = null;
    private CanThread canThread = null;

    public interface OnDataReceivedListener {
        void onDataReceived(String data);
    }

    public CanClient(IFlexcanService service, int id, OnDataReceivedListener listener) {
        mIFlexcanService = service;
        mId = id;
        mListener = listener;
    }

    /**
     *
     * @param count 数据长度
     * @param data 数据
     * @return
     */
    public boolean sendData(int count, int[] data) {
        try {
            for (int i = 0; i < count; i++) {
                mIFlexcanService.set_data(mId, i, data[i]);
            }
            int ret = mIFlexcanService.Flexcan_send(mId, 25, count, 0, 0, 0, 1);
            if (ret == 0) {
                return true;
            } else {
                return false;
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return false;
    }

    //开始接收处理数据
    public void start() {
        stop();
        canThread = new CanThread();
        canThread.start();
    }

    public void stop() {
        if (canThread != null) {
            canThread.cancel();
            canThread.interrupt();
            canThread = null;
        }
    }

    class CanThread extends Thread {
        private boolean mRunning;

        public CanThread() {
            mRunning = true;
        }
        public void cancel() {
            mRunning = false;
        }
        
        public void run() {
            while (mRunning) {
                try {
                    Thread.sleep(500);
                    String str = "";
                    try {
                        int i;

                        int ret = mIFlexcanService.Flexcan_dump(mId, 25, 0);
                        int dlc = mIFlexcanService.get_dlc(mId);

                        if (ret == 0 && dlc > 0) {

                            for (i = 0; i < dlc; i++) {
                                if (i == 0) {
                                    str = "" + mIFlexcanService.get_data(mId, i);
                                } else {
                                    str = str + "-" + mIFlexcanService.get_data(mId, i);
                                }
                            }

                            if (mListener != null) {
                                mListener.onDataReceived(str);
                            }
                        }
                    } catch (RemoteException e) {
                        e.printStackTrace();
                    }
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }
}

package com.mixlinker.sdk;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.IFlexcanService;
import android.os.ServiceManager;
import android.util.Log;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {
    static final String TAG = "yazhou";
    private CanClient mCanClient;
    private IFlexcanService mIFlexcanService = null;
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mIFlexcanService = IFlexcanService.Stub.asInterface(ServiceManager.getService("flexcan"));
//        TextView tv = findViewById(R.id.sample_text);
//        tv.setText(stringFromJNI());
        startCanClient();
//        mCanClient.sendData(int count, int[] data);

    }

    // 处理接收的数据
    private void startCanClient() {
        stopCanClient();
        mCanClient = new CanClient(mIFlexcanService, 0, new CanClient.OnDataReceivedListener() {
            @Override
            public void onDataReceived(String data) {
                Log.i(TAG, "onDataReceived: "+data);
            }
        });
        mCanClient.start();
    }

    private void stopCanClient() {
        if (mCanClient != null) {
            mCanClient.stop();
            mCanClient = null;
        }
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
}

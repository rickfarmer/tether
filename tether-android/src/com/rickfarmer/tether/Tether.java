package com.rickfarmer.tether;

import android.app.Activity;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.ToggleButton;
import android.os.Bundle;

public class Tether extends Activity {

	private ToggleButton proxyButton;
	private TextView tv;
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);

		// Dynamically load the library at runtime
		// before calling the native method.
		System.loadLibrary("rickfarmer-tether");
		Log.d("tether:", "System.loadLibrary called");

		addTetherListener();

	}

	public native String proxy(int start);

	public void addTetherListener() {

		proxyButton = (ToggleButton) findViewById(R.id.proxyButton);
		tv = (TextView) findViewById(R.id.proxyIp);

		proxyButton.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				boolean isProxyStart = (proxyButton.getText().equals("Proxy On")?true:false);				
				Log.d("tether:", "onClick, isProxyStart=" + isProxyStart);

				StringBuffer result = new StringBuffer();
				result.append(proxyButton.getText());
				result.append(":\n");
				
				String text = "";
				
				if(isProxyStart){
					text = startProxy();
				}else{
					text = stopProxy();
				}
				result.append(text);
				tv.setText(text);

				Toast.makeText(Tether.this, result.toString(),
						Toast.LENGTH_SHORT).show();

			}

		});

	}

	public String startProxy() {
		Log.d("tether:", "startProxy() called");
		String result = proxy(1);
		return result;
	};

	public String stopProxy() {
		Log.d("tether:", "stopProxy() called");
		String result = proxy(0);
		return result;
	};

}

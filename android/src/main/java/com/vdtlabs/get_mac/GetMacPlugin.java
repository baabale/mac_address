package com.vdtlabs.get_mac;

import android.annotation.TargetApi;
import android.os.Build;

import java.net.NetworkInterface;
import java.util.Collections;
import java.util.List;

import io.flutter.embedding.engine.plugins.FlutterPlugin;
import io.flutter.plugin.common.MethodCall;
import io.flutter.plugin.common.MethodChannel;
import io.flutter.plugin.common.MethodChannel.MethodCallHandler;
import io.flutter.plugin.common.MethodChannel.Result;
import io.flutter.plugin.common.PluginRegistry.Registrar;

/** GetMacPlugin */
public class GetMacPlugin implements FlutterPlugin, MethodCallHandler {
  @Override
  public void onAttachedToEngine(FlutterPluginBinding flutterPluginBinding) {
    final MethodChannel channel = new MethodChannel(flutterPluginBinding.getFlutterEngine().getDartExecutor(), "get_mac");
    channel.setMethodCallHandler(new GetMacPlugin());
  }

  public static void registerWith(Registrar registrar) {
    final MethodChannel channel = new MethodChannel(registrar.messenger(), "get_mac");
    channel.setMethodCallHandler(new GetMacPlugin());
  }

  @Override
  public void onMethodCall(MethodCall call,Result result) {
    if (call.method.equals("getMacAddress")) {
      result.success(getMacAddress());
    } else {
      result.notImplemented();
    }
  }

  @Override
  public void onDetachedFromEngine(FlutterPluginBinding binding) {
  }

  @TargetApi(Build.VERSION_CODES.GINGERBREAD)
  private String getMacAddress(){
    try {
      List<NetworkInterface> all = Collections.list(NetworkInterface.getNetworkInterfaces());
      for (NetworkInterface nif : all) {
        if (!nif.getName().equalsIgnoreCase("wlan0")) continue;
        byte[] macBytes = nif.getHardwareAddress();
        if (macBytes == null) {
          return "";
        }
        StringBuilder res1 = new StringBuilder();
        for (byte b : macBytes) {
          res1.append(String.format("%02X:",b));
        }
        if (res1.length() > 0) {
          res1.deleteCharAt(res1.length() - 1);
        }
        return res1.toString();
      }
    } catch (Exception ex) {
    }
    return "02:00:00:00:00:00";
  }


}

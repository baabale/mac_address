import 'dart:async';

import 'package:flutter/services.dart';

class GetMac {
  static const MethodChannel _channel = const MethodChannel('get_mac');

  static Future<String> get macAddress async {
    final String macID = await _channel.invokeMethod('getMacAddress');
    return macID;
  }
}

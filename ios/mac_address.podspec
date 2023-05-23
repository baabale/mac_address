#
# To learn more about a Podspec see http://guides.cocoapods.org/syntax/podspec.html.
# Run `pod lib lint get_mac.podspec' to validate before publishing.
#
Pod::Spec.new do |s|
  s.name             = 'mac_address'
  s.version          = '1.0.0'
  s.summary          = 'A new Flutter plugin to get MAC Address of Android and iOS Device'
  s.description      = <<-DESC
A new Flutter plugin to get MAC Address of Android and iOS Device
                       DESC
  s.homepage         = 'https://github.com/baabale/mac_address'
  s.license          = { :file => '../LICENSE' }
  s.author           = { 'BTech Solutions' => 'me@baabale.com' }
  s.source           = { :path => '.' }
  s.source_files = 'Classes/**/*'
  s.dependency 'Flutter'
  s.platform = :ios, '8.0'

  # Flutter.framework does not contain a i386 slice. Only x86_64 simulators are supported.
  s.pod_target_xcconfig = { 'DEFINES_MODULE' => 'YES', 'VALID_ARCHS[sdk=iphonesimulator*]' => 'x86_64' }
  s.swift_version = '5.0'
end

#define _CRT_SECURE_NO_WARNINGS

#include "include/mac_address/mac_address_plugin.h"

#include <winsock2.h>
#include <stdio.h>
#include <iphlpapi.h>

#include <Assert.h>

#pragma comment(lib, "iphlpapi.lib") // For iphlpapi
#pragma comment(lib, "ws2_32.lib")   // For winsock

// This must be included before many other Windows headers.
#include <windows.h>

// For getPlatformVersion; remove unless needed for your plugin implementation.
#include <VersionHelpers.h>

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <map>
#include <memory>
#include <sstream>

namespace
{

  class MacAddressPlugin : public flutter::Plugin
  {
  public:
    static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

    MacAddressPlugin();

    virtual ~MacAddressPlugin();

  private:
    // Called when a method is called on this plugin's channel from Dart.
    void HandleMethodCall(
        const flutter::MethodCall<flutter::EncodableValue> &method_call,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    std::string getMAC();
  };

  // static
  void MacAddressPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarWindows *registrar)
  {
    auto channel =
        std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
            registrar->messenger(), "get_mac",
            &flutter::StandardMethodCodec::GetInstance());

    auto plugin = std::make_unique<MacAddressPlugin>();

    channel->SetMethodCallHandler(
        [plugin_pointer = plugin.get()](const auto &call, auto result)
        {
          plugin_pointer->HandleMethodCall(call, std::move(result));
        });

    registrar->AddPlugin(std::move(plugin));
  }

  MacAddressPlugin::MacAddressPlugin() {}

  MacAddressPlugin::~MacAddressPlugin() {}

  void MacAddressPlugin::HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
  {
    if (method_call.method_name().compare("getPlatformVersion") == 0)
    {
      std::ostringstream version_stream;
      version_stream << "Windows ";
      if (IsWindows10OrGreater())
      {
        version_stream << "10+";
      }
      else if (IsWindows8OrGreater())
      {
        version_stream << "8";
      }
      else if (IsWindows7OrGreater())
      {
        version_stream << "7";
      }
      result->Success(flutter::EncodableValue(version_stream.str()));
    }
    else if (method_call.method_name().compare("getMacAddress") == 0)
    {
      result->Success(flutter::EncodableValue(MacAddressPlugin::getMAC()));
    }
    else
    {
      result->NotImplemented();
    }
  }

  std::string MacAddressPlugin::getMAC()
  {
    PIP_ADAPTER_INFO AdapterInfo;
    DWORD dwBufLen = sizeof(IP_ADAPTER_INFO);
    char *mac_addr = (char *)malloc(18);

    AdapterInfo = (IP_ADAPTER_INFO *)malloc(sizeof(IP_ADAPTER_INFO));
    if (AdapterInfo == NULL)
    {
      printf("Error allocating memory needed to call GetAdaptersinfo\n");
      free(mac_addr);
      return NULL; // it is safe to call free(NULL)
    }

    // Make an initial call to GetAdaptersInfo to get the necessary size into the dwBufLen variable
    if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == ERROR_BUFFER_OVERFLOW)
    {
      free(AdapterInfo);
      AdapterInfo = (IP_ADAPTER_INFO *)malloc(dwBufLen);
      if (AdapterInfo == NULL)
      {
        printf("Error allocating memory needed to call GetAdaptersinfo\n");
        free(mac_addr);
        return NULL;
      }
    }

    std::stringstream result;
    std::string activeMacAddress;
    if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == NO_ERROR)
    {
      // Contains pointer to current adapter info

      PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;

      do
      {
        // technically should look at pAdapterInfo->AddressLength
        //   and not assume it is 6.
        sprintf(mac_addr, "%02X:%02X:%02X:%02X:%02X:%02X",
                pAdapterInfo->Address[0], pAdapterInfo->Address[1],
                pAdapterInfo->Address[2], pAdapterInfo->Address[3],
                pAdapterInfo->Address[4], pAdapterInfo->Address[5]);
        printf("Address: %s, mac: %s\n", pAdapterInfo->IpAddressList.IpAddress.String, mac_addr);
        // print them all, return the last one.
        // return mac_addr;

        std::string ipAddress(pAdapterInfo->IpAddressList.IpAddress.String);
        if (ipAddress != "0.0.0.0")
        {
          std::string str(mac_addr);
          result << str;
         pAdapterInfo = nullptr;
        }else{

        pAdapterInfo = pAdapterInfo->Next;
        }
        printf("\n");
       
      } while (pAdapterInfo);
    }
    free(AdapterInfo);
    std::string str(result.str()); // caller must free.

    return str;
  }
} // namespace

void MacAddressPluginRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar)
{
  MacAddressPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}

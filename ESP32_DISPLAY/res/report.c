📡 Connection Management
WiFi.begin(ssid, password) – Connect to a network.

WiFi.disconnect() – Disconnect from the network.

WiFi.reconnect() – Attempt to reconnect.

WiFi.mode(WIFI_STA / WIFI_AP / WIFI_AP_STA) – Set WiFi mode (station, access point, or both).

🌐 IP & Network Info
WiFi.localIP() – Get the local IP assigned.

WiFi.gatewayIP() – Get the network gateway IP.

WiFi.subnetMask() – Get subnet mask.

WiFi.dnsIP() – Get DNS IP address.

WiFi.macAddress() – Get MAC address as a string.

WiFi.BSSIDstr() – Get MAC address of the connected access point.

WiFi.RSSI() – Get signal strength (in dBm).

🔄 Configuration
WiFi.setHostname("custom-name") – Set device hostname.

WiFi.config(ip, gateway, subnet) – Use static IP instead of DHCP.

WiFi.persistent(true/false) – Control whether connection settings are saved to flash.

🔍 Scanning & Discovery
WiFi.scanNetworks() – Scan for nearby WiFi networks.

WiFi.SSID(i) – Get SSID of scanned network.

WiFi.RSSI(i) – Get RSSI of scanned network.

WiFi.encryptionType(i) – Get encryption type (e.g., WPA2).

WiFi.BSSIDstr(i) – Get MAC of scanned network.

🔒 Security & Diagnostics
WiFi.status() – Check if connected, and to what.

WiFi.getAutoReconnect() / WiFi.setAutoReconnect(true) – Enable auto reconnect.

WiFi.hostname() – Get the current hostname.

WiFi.isConnected() – Simple check for connection state.

📶 Access Point Mode (AP)
WiFi.softAP(ssid, password) – Start your own WiFi access point.

WiFi.softAPIP() – Get the IP of the ESP32 in AP mode.

WiFi.softAPmacAddress() – Get the MAC of the AP interface.

WiFi.softAPdisconnect() – Shut down the AP.

🧠 Advanced
WiFi.getSleep() / WiFi.setSleep(false) – Control WiFi sleep mode.

WiFi.getPhyMode() – Get PHY mode (11b/g/n).

WiFi.setTxPower(WIFI_POWER_*) – Set TX power levels.

WiFi.printDiag(Serial) – Print diagnostic info to serial.

📌 Example: Scan Networks
cpp
Copy
Edit
int n = WiFi.scanNetworks();
for (int i = 0; i < n; ++i) {
  Serial.printf("%d: %s (%d dBm)\n", i + 1, WiFi.SSID(i).c_str(), WiFi.RSSI(i));
}
Let me know what kind of app you're building—like IoT, mesh, access point, or captive portal—and I can recommend more specific functions!








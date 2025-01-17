#include <WiFi.h>
#include <ota.h>

#include "SPIFFS.h"
#define FILESYS SPIFFS

// This string should correspond to github tag used for Releasing (via. Github Actions)
#define VERSION "0.0.1"

// Replace your_username/your_repo with your values (ex. axcap/Esp-GitHub-OTA)
// This is a link to repo where your firmware updates will be pulled from
// #define RELEASE_URL "https://api.github.com/repos/your_username/your_repo/releases/latest"

// Use this version of the URL together with 
//Line 22: GitHubOTA GitHubOTA(VERSION, RELEASE_URL, "firmware.bin", "filesystem.bin", "/.fs_update_pending", false);
// under debugging to spare yourself from getting timeout from GitHub API
#define RELEASE_URL "https://github.com/your_username/your_repo/releases/latest"

#define DELAY_MS 1000

#define SSID ""
#define PASSWORD ""

// Uncomment the line below when using Github Api to get the latest release (Line 9)
//GitHubOTA GitHubOTA(VERSION, RELEASE_URL);

GitHubOTA GHOTA(VERSION, RELEASE_URL, "firmware.bin", "filesystem.bin", "/.fs_update_pending", false);

void listRoot(){
  Serial.printf("Listing root directory\r\n");

  File root = SPIFFS.open("/", "r");
  File file = root.openNextFile();

  while(file){
    Serial.printf("  FILE: %s\r\n", file.name());
    file = root.openNextFile();
  }
}

#define LED_PIN 2

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  while(!Serial);

  Serial.printf("Current firmware version: %s\n", VERSION); 

  SPIFFS.begin();
  listRoot();

  setup_wifi();

  GHOTA.handle();
}

void loop()
{
  // Uncomment this line to check for updates on every loop iteration
  // GitHubOTA.handle();

  // Your code goes here
  digitalWrite(LED_PIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(DELAY_MS);                  // wait for a second
  digitalWrite(LED_PIN, LOW);   // turn the LED off by making the voltage LOW
  delay(DELAY_MS);                  // wait for a second
}

void setup_wifi(){
  Serial.println("Initialize WiFi");
  WiFi.begin(SSID, PASSWORD);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

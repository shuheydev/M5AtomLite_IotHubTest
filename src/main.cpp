#include <M5Atom.h>
#include <WiFi.h>
#include <HTTPClient.h>

const char *ssid = "nautiluswireless24";
const char *password = "PrnuAQuD8YmB";

bool initWifi()
{
  WiFi.disconnect();
  Serial.printf("Attempting to connect to SSID: %s\n", ssid);
  Serial.println("Connecting Wifi");

  // 10回試行する
  int tryCount = 0;
  while (true)
  {
    tryCount++;

    if (WiFi.status() != WL_CONNECTED)
    {
      WiFi.begin(ssid, password);
    }
    else
    {
      Serial.println("Connected!");
      Serial.println(WiFi.localIP());
      return true;
    }

    if (tryCount >= 10)
    {
      Serial.println("Try 10 times to connect wifi, but could't");
      return false;
    }
    Serial.print(".");
    delay(1000);
  }
}

void sendHttpRequest(String url)
{
  Serial.println("Start Http request");

  M5.dis.drawpix(0, 0x220022);

  Serial.println("Create HttpClient");
  HTTPClient http;
  http.begin(url);
  http.setReuse(false);

  Serial.println("Sending request");
  int status_code = http.GET();

  if (status_code != 200)
  {
    Serial.println("Request failed");
    M5.dis.drawpix(0, 0x220000);
  }
  else
  {
    Serial.println(http.getString());
    M5.dis.drawpix(0, 0x000022);
  }

  delay(2000);
}


void setup()
{
  // put your setup code here, to run once:
  M5.begin(true, false, true);

  M5.dis.drawpix(0, 0x202020);

  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println("Serial started");

  bool wifiResult = initWifi();

  if (wifiResult == false)
  {
    M5.dis.drawpix(0, 0x200000);
    return;
  }
  else
  {
    M5.dis.drawpix(0, 0x002000);
    delay(1000);
  }
}

void loop()
{
  M5.update();
  
  // put your main code here, to run repeatedly:
  if (M5.Btn.isPressed())
  {
    M5.dis.drawpix(0, 0x000020);

    sendHttpRequest("https://www.google.co.jp");
  }
  else
  {
    M5.dis.drawpix(0, 0x202020);
  }
}

#include "FS.h"

#include <PJON.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "main.h"

void callback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("[AWS] Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
    }
    Serial.println();
}

WiFiClientSecure espClient;
PubSubClient client(aws_mqtt_server, 8883, callback, espClient); //set  MQTT port number to 8883 as per //standard
int tick = 0;

TinyGPSPlus gps; // The TinyGPS++ object
float latitude, longitude;
String date_str, time_str, lat_str, lng_str, ssid_str;

SoftwareSerial ss(4, 5); // The serial connection to the GPS device

#define SWBB_MODE 1
#define SWBB_RESPONSE_TIMEOUT 1500
#define SLAVE_BUS_ID 44
#define PJON_PIN D3
#define BAUD_RATE 115200

PJON<SoftwareBitBang> bus(SLAVE_BUS_ID);

void working_led()
{
    digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)
    delay(50);                     // wait for a second
    digitalWrite(LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
    delay(50);
}

void receiver_function(uint8_t *payload, uint16_t length, const PJON_Packet_Info &packet_info)
{
    DynamicJsonBuffer jsonBuffer(254);
    String tmp_ssid_str = "";
    Serial.print("[PJON DATA] ");
    for (int ii = 0; ii < length; ii++)
    {
        Serial.print(char(payload[ii]));
        tmp_ssid_str += char(payload[ii]);
    }
    JsonObject &tmp_ssid = jsonBuffer.parseObject(tmp_ssid_str);
    if (tmp_ssid["ssid"] != "" && tmp_ssid["ssid"] != ssid)
    {
        ssid_str = tmp_ssid_str;
    }
    Serial.println();
}

void error_handler(uint8_t code, uint16_t data, void *custom_pointer)
{
    if (code == PJON_CONNECTION_LOST)
    {
        Serial.print("Connection with device ID ");
        Serial.print(bus.packets[data].content[0], DEC);
        Serial.println(" is lost.");
    }
    if (code == PJON_PACKETS_BUFFER_FULL)
    {
        Serial.print("Packet buffer is full, has now a length of ");
        Serial.println(data);
        Serial.println("Possible wrong bus configuration!");
        Serial.println("higher PJON_MAX_PACKETS if necessary.");
    }
    if (code == PJON_CONTENT_TOO_LONG)
    {
        Serial.print("Content is too long, length: ");
        Serial.println(data);
    }
}

void setup_pjon()
{
    bus.strategy.set_pin(PJON_PIN);
    bus.set_error(error_handler);
    bus.set_receiver(receiver_function);
    bus.begin();
    working_led();
}

void setup_wifi()
{

    delay(100);
    working_led();
    // We start by connecting to a WiFi network
    Serial.println();
    Serial.print("[WIFI] Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
        working_led();
    }

    Serial.println("");
    Serial.print("[WIFI] WiFi connected on ");
    Serial.println(WiFi.localIP());
}

void setup_certs()
{
    if (!SPIFFS.begin())
    {
        Serial.println("[CERTS] Failed to mount file system");
        return;
    }

    Serial.print("[CERTS] Heap: ");
    Serial.println(ESP.getFreeHeap());

    // Load certificate file
    File cert = SPIFFS.open("/cert.der", "r"); //replace cert.crt eith your uploaded file name
    if (!cert)
    {
        Serial.println("[CERTS] Failed to open cert file");
    }
    else
        Serial.println("[CERTS] Success to open cert file");

    delay(200);

    if (espClient.loadCertificate(cert))
        Serial.println("[CERTS] cert loaded");
    else
        Serial.println("[CERTS] cert not loaded");

    // Load private key file
    File private_key = SPIFFS.open("/private.der", "r"); //replace private eith your uploaded file name
    if (!private_key)
    {
        Serial.println("[CERTS] Failed to open private cert file");
    }
    else
        Serial.println("[CERTS] Success to open private cert file");

    delay(200);

    if (espClient.loadPrivateKey(private_key))
        Serial.println("[CERTS] private key loaded");
    else
        Serial.println("[CERTS] private key not loaded");

    // Load CA file
    File ca = SPIFFS.open("/ca.der", "r"); //replace ca eith your uploaded file name
    if (!ca)
    {
        Serial.println("[CERTS] Failed to open ca ");
    }
    else
        Serial.println("[CERTS] Success to open ca");
    delay(200);

    if (espClient.loadCACert(ca))
        Serial.println("[CERTS] ca loaded");
    else
        Serial.println("[CERTS] ca failed");
    Serial.print("[CERTS] Heap: ");
    Serial.println(ESP.getFreeHeap());
    working_led();
}

void aws_reconnect()
{
    // Loop until we're reconnected
    while (!client.connected())
    {
        Serial.print("[AWS] Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect(aws_mqtt_client_id))
        {
            Serial.println("[AWS] connected");
            // ... and resubscribe
            client.subscribe(aws_mqtt_thing_topic_sub);
        }
        else
        {
            Serial.print("[AWS] failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            working_led();
            delay(5000);
        }
    }
}

void setup()
{
    // Debug LED
    pinMode(LED_BUILTIN, OUTPUT);

    setup_pjon();
    delay(200);
    Serial.begin(BAUD_RATE);
    delay(200);
    ss.begin(9600);

    delay(200);
    setup_wifi();
    delay(200);
    setup_certs();
    delay(200);
    aws_reconnect();
}

void loop()
{
    if (!client.connected())
    {
        aws_reconnect();
    }
    client.loop();

    while (ss.available() > 0)
    {
        if (gps.encode(ss.read()))
        {
            if (gps.location.isValid())
            {
                latitude = gps.location.lat();
                lat_str = String(latitude, 6);
                longitude = gps.location.lng();
                lng_str = String(longitude, 6);
            }
            else
            {
                lat_str = "";
                lng_str = "";
            }

            if (gps.date.isValid())
            {
                date_str = "";
                date_str += String(gps.date.day());
                date_str += "/";
                date_str += String(gps.date.month());
                date_str += "/";
                date_str += String(gps.date.year());
            }
            else
            {
                date_str = "";
            }

            if (gps.time.isValid())
            {
                time_str = "";
                time_str += String(gps.time.hour());
                time_str += ":";
                time_str += String(gps.time.minute());
                time_str += ":";
                time_str += String(gps.time.second());
            }
            else
            {
                time_str = "";
            }
        }
    }

    bus.receive(10000);
    bus.update();

    if (tick >= 10) // publish to topic every 1seconds
    {
        tick = 0;
        working_led();

        const size_t bufferSize = JSON_OBJECT_SIZE(12) + 20;
        DynamicJsonBuffer jsonBuffer(bufferSize);

        JsonObject &root = jsonBuffer.createObject();
        //root["date"] = date_str;
        //root["time"] = time_str;
        JsonArray& data = root.createNestedArray("location");
        data.add(latitude);
        data.add(longitude);
        //root["lat"] = lat_str;
        //root["lng"] = lng_str;
        JsonObject &json_ssid = jsonBuffer.parseObject(ssid_str);
        root["ssid"] = json_ssid;

        String json_output;

        root.printTo(json_output);
        char payload[bufferSize];

        json_output.toCharArray(payload, bufferSize);
        sprintf(payload, json_output.c_str());

        //ssid_str = "";
        Serial.print("[AWS MQTT] Publish Message:");
        Serial.println(payload);
        client.publish(aws_mqtt_thing_topic_pub, payload);
    }
    delay(100);
    tick++;
}

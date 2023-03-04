#include <WiFi.h>
#include <PubSubClient.h>
#include <esp_camera.h>

// Replace with your network credentials
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// Replace with your MQTT broker IP address
const char* mqtt_server = "MQTT_BROKER_IP_ADDRESS";

// Replace with your MQTT broker username and password
const char* mqtt_username = "your_MQTT_username";
const char* mqtt_password = "your_MQTT_password";

// Initialize the MQTT client
WiFiClient espClient;
PubSubClient client(espClient);

// Define the GPIO pin for the button
const int buttonPin = 12;
const int LED_GPIO_PIN = 4;

void setup() {
  // Initialize the serial communication
  Serial.begin(115200);

  // Initialize the camera
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = 5;
  config.pin_d1 = 18;
  config.pin_d2 = 19;
  config.pin_d3 = 21;
  config.pin_d4 = 36;
  config.pin_d5 = 39;
  config.pin_d6 = 34;
  config.pin_d7 = 35;
  config.pin_xclk = 0;
  config.pin_pclk = 22;
  config.pin_vsync = 25;
  config.pin_href = 23;
  config.pin_sscb_sda = 26;
  config.pin_sscb_scl = 27;
  config.pin_pwdn = 32;
  config.pin_reset = -1;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_VGA;
  config.jpeg_quality = 10;
  config.fb_count = 2;

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  // Connect to Wi-Fi network
  // WiFi.begin(ssid, password);
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(1000);
  //   Serial.println("Connecting to WiFi...");
  // }
  // Serial.println("Connected to WiFi");

  // // Connect to MQTT broker
  // client.setServer(mqtt_server, 1883);
  // while (!client.connected()) {
  //   Serial.println("Connecting to MQTT broker...");
  //   if (client.connect("ESP32camClient", mqtt_username, mqtt_password)) {
  //     Serial.println("Connected to MQTT broker");
  //   } else {
  //     Serial.print("Failed with state ");
  //     Serial.print(client.state());
  //     delay(2000);
  //   }
  // }

  // Initialize the button
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(LED_GPIO_PIN, OUTPUT);
  Serial.println("GPIO Initiatied");
}

void loop() {
  // Check if the button is pressed
  if (digitalRead(buttonPin) == LOW) {
    Serial.println("Pushbutton pressed");
    digitalWrite(LED_GPIO_PIN, HIGH);
    // Capture an image
    camera_fb_t* fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Failed to capture image");
      return;
    }
    Serial.println("Captured Image");
    delay(1500);
    digitalWrite(LED_GPIO_PIN, LOW);
    // Publish the image to MQTT broker
    char topic[50];
    sprintf(topic, "ESP32cam/image/%d", millis());
    Serial.println(sprintf(topic, "ESP32cam/image/%d", millis()));
    // client.publish(topic, fb->buf, fb->len);
    Serial.println("connecting to broker");
    delay(1000);
    // Release the image buffer
    esp_camera_fb_return(fb);
    Serial.println("Image captured and published to MQTT broker");
    delay(500);
  }

  // Reconnect to MQTT broker if
    // disconnected
  // if (!client.connected()) {
  //   if (client.connect("ESP32camClient", mqtt_username, mqtt_password)) {
  //   Serial.println("Reconnected to MQTT broker");
  //   } else {
  //   Serial.print("Failed to reconnect to MQTT broker with state ");
  //   Serial.print(client.state());
  //   }
  // }

// Maintain the MQTT connection
// client.loop();
}

// ==========================================
// IoT BASED WATER QUALITY MONITORING SYSTEM
// ==========================================

// --- BLYNK CREDENTIALS (MUST BE AT THE VERY TOP) ---
#define BLYNK_TEMPLATE_ID "TMPL32BfoaTqi"
#define BLYNK_TEMPLATE_NAME "IoT Based Water Monitoring System"
#define BLYNK_AUTH_TOKEN "1dGZCzQ_GWeIZhhYJAHZ4d8zzaOM_gWI"

// --- LIBRARIES ---
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// --- WIFI CREDENTIALS ---
char ssid[] = "Shrawani";      // <--- Type your Wi-Fi Name here
char pass[] = "123456789";  // <--- Type your Wi-Fi Password here

// --- LCD Configuration ---
LiquidCrystal_I2C lcd(0x27, 16, 2); 

// --- Sensor Pin Definitions ---
#define ONE_WIRE_BUS 33  // Temperature Sensor Pin
#define TDS_PIN 32       // TDS Sensor Pin
#define TURB_PIN 34      // Turbidity Sensor Pin
#define PH_PIN 35        // pH Sensor Pin

// --- Sensor Objects ---
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature tempSensor(&oneWire);

// ==========================================
// CUSTOM pH CALIBRATION VALUES
// ==========================================
// 1. Voltage reading in Neutral (Tap Water ~ pH 7)
float voltagePH7 = 1.63;  

// 2. Voltage reading in Acid (Lemon Juice ~ pH 2.5)
float voltagePH4 = 1.43;  

void setup() {
  Serial.begin(115200);
  
  // 1. Initialize LCD
  lcd.begin(); 
  lcd.backlight();

  // ==========================================
  // BOOT SCREEN
  // ==========================================
  lcd.setCursor(0, 0);
  lcd.print("IoT WATER QUALITY");   
  lcd.setCursor(0, 1);
  lcd.print("MONITOR SYSTEM");   
  
  // 2. Connect to Wi-Fi & Blynk
  Serial.println("Connecting to Wi-Fi...");
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  
  delay(2500);                  
  
  // 3. Initialize Sensors
  tempSensor.begin();
  
  // 4. Calibration Status
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Calibrating");
  lcd.setCursor(0, 1);
  lcd.print("Sensors...");
  delay(1000);
  lcd.clear();
}

void loop() {
  // Run the Blynk engine
  Blynk.run();

  // ----------------------------------------
  // 1. Read Digital Temperature
  // ----------------------------------------
  tempSensor.requestTemperatures(); 
  float tempC = tempSensor.getTempCByIndex(0);

  // ----------------------------------------
  // 2. Read & Calculate TDS
  // ----------------------------------------
  int rawTDS = analogRead(TDS_PIN);
  float tdsVoltage = rawTDS * (3.3 / 4095.0);
  float compCoeff = 1.0 + 0.02 * (tempC - 25.0);
  float compVolts = tdsVoltage / compCoeff;
  
  float tdsValue = (133.42 * pow(compVolts, 3) - 255.86 * pow(compVolts, 2) + 857.39 * compVolts) * 0.5;
  if (tdsValue < 0) { tdsValue = 0; }

  // ----------------------------------------
  // 3. Read Turbidity
  // ----------------------------------------
  int rawTurb = analogRead(TURB_PIN);
  float turbVoltage = rawTurb * (3.3 / 4095.0);

  // ----------------------------------------
  // 4. Read & Calculate pH
  // ----------------------------------------
  int rawPH = analogRead(PH_PIN);
  float phVoltage = rawPH * (3.3 / 4095.0); 
  
  // Calculate the step difference (Locked in at 5.5 for specific acid profile)
  float PH_step = (voltagePH4 - voltagePH7) / 5.5; 
  
  // Calculate the final pH
  float phValue = 7.0 + ((voltagePH7 - phVoltage) / PH_step);

  // Prevent impossible pH readings 
  if (phValue < 0.0) { phValue = 0.0; }
  if (phValue > 14.0) { phValue = 14.0; }

  // ----------------------------------------
  // 5. Output to Serial Monitor
  // ----------------------------------------
  Serial.print("T: "); Serial.print(tempC); Serial.print("C | ");
  Serial.print("TDS: "); Serial.print(tdsValue); Serial.print(" PPM | ");
  Serial.print("Turbidity: "); Serial.print(turbVoltage); Serial.print("V | ");
  Serial.print("pH Volts: "); Serial.print(phVoltage); Serial.print("V | ");
  Serial.print("pH: "); Serial.println(phValue);

  // ----------------------------------------
  // 6. Output to LCD Display
  // ----------------------------------------
  
  // Row 0: TDS and Temperature
  lcd.setCursor(0, 0);
  lcd.print("TDS:"); 
  lcd.print(tdsValue, 0); 
  lcd.print("ppm  "); // Extra spaces to clear old ghost digits
  
  lcd.setCursor(10, 0); // Move to column 10 for Temperature
  lcd.print("T:"); 
  lcd.print(tempC, 0); // 0 decimals to save space for the 'C'
  lcd.print("C ");

  // Row 1: Turbidity Volts and pH
  lcd.setCursor(0, 1);
  lcd.print("Tb:"); 
  lcd.print(turbVoltage, 2); 
  lcd.print("V ");
  
  lcd.setCursor(9, 1); // Move to column 9 for pH
  lcd.print("pH:"); 
  lcd.print(phValue, 1); 
  lcd.print("  "); // Clear trailing characters

  // ----------------------------------------
  // 7. SEND DATA TO BLYNK CLOUD
  // ----------------------------------------
  Blynk.virtualWrite(V0, tempC);        // Temperature
  Blynk.virtualWrite(V1, tdsValue);     // TDS
  Blynk.virtualWrite(V2, turbVoltage);  // Turbidity
  Blynk.virtualWrite(V3, phValue);      // pH

  // Fast Update Rate
  delay(500);
}
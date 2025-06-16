#include "main.h"

void setup()
{

#if F405_DEBUG_MODE
  Serial.begin(115200);
  delay(1000);
#endif

  // Initialize RGB LED
  neopixel.begin();
  neopixel.clear();
  neopixel.show();
  neopixel.setBrightness(30);  // Set brightness (0–255)

  initialize_mpu();

  // Trigger LOW
  // Start logging data in circular buffer
  pinMode(LOG_TRIGGER_GPIO, OUTPUT);
  digitalWrite(LOG_TRIGGER_GPIO, LOW);

#if F405_DEBUG_MODE
  Serial.println("Logging Disabled.");
#endif

  // Set initial color
  neopixel.setPixelColor(0, color_amber);
  neopixel.show();
}

void loop()
{

  switch (current_state)
  {
  case FLIGHT_READY_DETECTION:
  {
    // Check flight ready
    bool flight_ready = detect_flight_ready();

    // Change state on detect ready
    if (flight_ready)
    {
      // Change state
      current_state = FLIGHT_MONITORING;

      // Reset LED timing for purple heartbeat
      led_blink_time = millis();

#if F405_DEBUG_MODE
      Serial.println("Logging Enabled!");
#endif
    }
    break;
  }

  case FLIGHT_MONITORING:
  {

    // Get continuous data
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    // Is the rocket upright?
    rocket_curr_upright = (abs(a.acceleration.y) > UPRIGHT_ACCEL_THRESH);

    // Handle LED heartbeat for logging enabled state (purple heartbeat)
    if (!launch_detected)
    {
      unsigned long current_time = millis();
      unsigned long elapsed = current_time - led_blink_time;
      
      // Purple heartbeat pattern: 150ms on, 150ms off, 150ms on, 850ms off (total 1300ms cycle)
      if (elapsed < 150) {
        // First pulse
        neopixel.setPixelColor(0, color_purple);
        neopixel.show();
      } else if (elapsed < 300) {
        // Gap between pulses
        neopixel.clear();
        neopixel.show();
      } else if (elapsed < 450) {
        // Second pulse
        neopixel.setPixelColor(0, color_purple);
        neopixel.show();
      } else if (elapsed < 1300) {
        // Long gap
        neopixel.clear();
        neopixel.show();
      } else {
        // Reset cycle
        led_blink_time = current_time;
      }
    }

    // Upon motion detection
    if (mpu.getMotionInterruptStatus())
    {
      // Get data again
      // mpu.getEvent(&a, &g, &temp);

      // Compute total accel via squaring
      float totalAccel = sqrt(a.acceleration.x * a.acceleration.x +
                              a.acceleration.y * a.acceleration.y +
                              a.acceleration.z * a.acceleration.z);

// If no previous launch and significant accel
// Set launch
#if F405_DEBUG_MODE
      if (!launch_detected && totalAccel > DEBUG_LAUNCH_DETECT_ACCEL_THRESH)
#else
      if (!launch_detected && totalAccel > LAUNCH_DETECT_ACCEL_THRESH)
#endif
      {
        launch_detected = true;

        // Trigger GPIO low, write to main file
        digitalWrite(LOG_TRIGGER_GPIO, HIGH);
        
        // Change to solid green when launch detected
        neopixel.setPixelColor(0, color_green);
        neopixel.show();
        
#if F405_DEBUG_MODE
        Serial.println("Launch Detected! Wrote low to GPIO");
        Serial.print("Total Accel: ");
        Serial.println(totalAccel);
#endif
      }

      // If motion detected and rocket already launched
      // Restart quiet timer
      // This will keep setting during flight
      if (launch_detected)
      {
        quiet_start_time = millis();
#if F405_DEBUG_MODE
        Serial.println("~~Flying~~");
        Serial.print("Total Accel: ");
        Serial.println(totalAccel);
#endif
      }
    }

    // If launch detected and its been quiet
    if (launch_detected && (millis() - quiet_start_time > QUIET_TIME_THRESH_SEC))
    {

      // If rocket is still upright
      // Reset timer and reset launch detected because we are on pad
      if (rocket_curr_upright)
      {
        launch_detected = false;
        quiet_start_time = millis();
        
        // Return to purple heartbeat since we're back to monitoring
        led_blink_time = millis();
        
#if F405_DEBUG_MODE
        Serial.println("Still Upright - Resetting Timer");
#endif
      }

      // Rocket is NOT upright
      else
      {
        // Change state
        current_state = DATA_COLLECTION;

        // Indicate
        neopixel.setPixelColor(0, color_blue);
        neopixel.show();
        delay(100);
#if F405_DEBUG_MODE
        Serial.println("Flight Ended. Logging Disabled.");
#endif
      }
    }
    break;
  }

  case DATA_COLLECTION:
  {
    // Switch to new state
#if F405_DEBUG_MODE
    Serial.println();
    Serial.println("Standing by.");
#endif
    neopixel.setPixelColor(0, color_red);
    neopixel.show();
    current_state = STANDBY;
    break;
  }

  case STANDBY:
  {
    // NOTE: This delay is okay! Arbitrary number
    // The board will remain in this state unless restarted
    delay(10000);
    break;
  }
  }
}

void initialize_mpu()
{
  if (!mpu.begin(MPU6050_I2CADDR_DEFAULT, &MPU_I2C))
  {
    Serial.println("Failed to find MPU6050 chip");
    while (1)
    {
      delay(10);
    }
  }

  // Setup motion detection
  mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
  mpu.setMotionDetectionThreshold(1);
  mpu.setMotionDetectionDuration(MPU_MOT_DET_DUR);
  mpu.setInterruptPinLatch(true);
  mpu.setInterruptPinPolarity(true);
  mpu.setMotionInterrupt(true);
}

bool detect_flight_ready()
{
  // Only check if motion interrupt triggered
  if (mpu.getMotionInterruptStatus())
  {
    // Check if rocket is currently upright
    mpu.getEvent(&a, &g, &temp);
    rocket_curr_upright = (abs(a.acceleration.y) > UPRIGHT_ACCEL_THRESH);

    // If not upright anymore on new motion, reset upright tracker
    if (!rocket_curr_upright && rocket_prev_upright)
    {
#if F405_DEBUG_MODE
      Serial.println("Rocket no longer upright, resetting timer.");
#endif
      rocket_prev_upright = false;
    }

    // If just became upright, start timer and set upright tracker
    if (rocket_curr_upright && !rocket_prev_upright)
    {
      upright_start_time = millis();
      rocket_prev_upright = true;
#if F405_DEBUG_MODE
      Serial.println("Rocket became upright, starting timer.");
#endif
    }
  }

  // Check timer
  // Switch state past threshold
  if (rocket_prev_upright && (millis() - upright_start_time > UPRIGHT_TIME_THRESH_SEC))
  {
#if F405_DEBUG_MODE
    Serial.println("Rocket upright threshold met, switching to flight monitoring.");
#endif
    return true;
  }

  // Blink LED when timer is running
  if (rocket_prev_upright)
  {
    if (millis() - led_blink_time > 500)
    {
      led_blink_state = !led_blink_state;
      led_blink_time = millis();

      if (led_blink_state)
      {
        neopixel.setPixelColor(0, color_amber);
      }
      else
      {
        neopixel.clear();
      }
      neopixel.show();
    }
  }
  else
  {
    neopixel.setPixelColor(0, neopixel.Color(255, 191, 0));
    neopixel.show();
  }

  return false;
}
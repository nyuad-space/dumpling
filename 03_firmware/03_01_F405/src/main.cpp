#include "main.h"

void setup()
{

#if DEBUG
  Serial.begin(115200);
  delay(1000);
#endif

  // Initialize RGB LED
  neopixel.begin();
  neopixel.clear();
  neopixel.show();

  // Chip selects high
  // Must select chips manually!
  initialize_interboard_spi();
  initialize_mpu();


  // Initialize trigger
  pinMode(LOG_TRIGGER_GPIO, OUTPUT);
  digitalWrite(LOG_TRIGGER_GPIO, HIGH);

#if DEBUG
  Serial.println("Logging disabled.");
#endif

  // Build TX buffers
  dummy_packet_size = buildDummyRequest(dummy_buffer);
  command_packet_size = buildDataRequest(command_buffer);

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
      // Start logging
      digitalWrite(LOG_TRIGGER_GPIO, LOW);

      // Change state
      current_state = FLIGHT_MONITORING;

      // Indicate
      neopixel.setPixelColor(0, color_green);
      neopixel.show();

#if DEBUG
        Serial.println("Logging enabled!");
#endif
    }
    break;
  }

  case FLIGHT_MONITORING:
  {
    if (first_entry)
    {
      launch_detected = false;
      first_entry = false;
    }

    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    rocket_curr_upright = (abs(a.acceleration.y) > UPRIGHT_ACCEL_THRESH);

    if (mpu.getMotionInterruptStatus())
    {

      mpu.getEvent(&a, &g, &temp);

      float totalAccel = sqrt(a.acceleration.x * a.acceleration.x +
                              a.acceleration.y * a.acceleration.y +
                              a.acceleration.z * a.acceleration.z);

      // Detect launch event (significant acceleration spike)
      if (!launch_detected && totalAccel > LAUNCH_DETECT_ACCEL_THRESH) // Adjust threshold
      {
        launch_detected = true;
        // quiet_start_time = millis();
#if DEBUG
        Serial.println("Launch Detected!");
#endif
      }

      // Reset quiet timer if we detected launch and there's still motion
      if (launch_detected)
      {
        quiet_start_time = millis();
#if DEBUG
        Serial.println("~~Flying~~");
#endif
      }
    }

    // Only enter data collection if: launch detected AND quiet period elapsed AND we are upright
    if (launch_detected && (millis() - quiet_start_time > QUIET_TIME_THRESH_SEC))
    {

      // If rocket is still upright, still on pad
      // Reset timer
      if (rocket_curr_upright)
      {
        launch_detected = false;
        quiet_start_time = millis();
#if DEBUG
        Serial.println("Still upright - resetting timer");
#endif
      }

      else
      {

        // Stop logging
        digitalWrite(LOG_TRIGGER_GPIO, HIGH);

        // Change state
        current_state = DATA_COLLECTION;
        first_entry = true;

        // Indicate
        neopixel.setPixelColor(0, color_blue);
        neopixel.show();
#if DEBUG
        Serial.println("Flight ended. Logging disabled. Collecting logs now.");
#endif
      }
    }
    break;
  }

  case DATA_COLLECTION:
  {
    // Collect data from all coprocessors
    make_data_request(INTERBOARD_SPI_CO2_CS);
    // store_to_sd(data);

    // Switch to new state
#if DEBUG
    Serial.println();
    Serial.println("Data collection complete. Standing by.");
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

void initialize_interboard_spi()
{
  // Initialize all CS pins as outputs (HIGH = inactive, safe default)
  for (size_t i = 0; i < InterboardSPI_CS::COUNT; i++)
  {
    pinMode(InterboardSPI_CS::PINS_LIST[i], OUTPUT);
    digitalWrite(InterboardSPI_CS::PINS_LIST[i], HIGH);
    Serial.print(InterboardSPI_CS::PINS_LIST[i]);
  }

  // Initialize SPI bus
  INTERBOARD_SPI.begin();
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
#if DEBUG
      Serial.println("Rocket no longer upright, resetting timer.");
#endif
      rocket_prev_upright = false;
    }

    // If just became upright, start timer and set upright tracker
    if (rocket_curr_upright && !rocket_prev_upright)
    {
      upright_start_time = millis();
      rocket_prev_upright = true;
#if DEBUG
      Serial.println("Rocket became upright, starting timer.");
#endif
    }
  }

  // Check timer
  // Switch state past threshold
  if (rocket_prev_upright && (millis() - upright_start_time > UPRIGHT_TIME_THRESH_SEC))
  {
#if DEBUG
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

bool make_data_request(uint8_t CS_PIN)
{
  // Send initial command packet
  digitalWrite(CS_PIN, LOW);
  INTERBOARD_SPI.transfer(command_buffer, rx_buffer, command_packet_size, SPI_LAST);
  digitalWrite(CS_PIN, HIGH);

#if DEBUG
  _print_buffer("TX", command_buffer, command_packet_size);
  _print_buffer("RX", rx_buffer, command_packet_size);
#endif

  // Delay to allow coprocessor to prepare data.
  delay(3000);

  // Poll for response data with a timeout
  unsigned long start_time = millis();
  while (millis() - start_time < POLLING_TIMEOUT_MS)
  {
    digitalWrite(CS_PIN, LOW);
    delay(10);
    INTERBOARD_SPI.transfer(dummy_buffer, rx_buffer, dummy_packet_size, SPI_LAST);
    digitalWrite(CS_PIN, HIGH);

#if DEBUG
    _print_buffer("TX", dummy_buffer, 5);
    _print_buffer("RX", rx_buffer, MAX_PACKET_SIZE);
#endif

    if (check_end_sequence(rx_buffer))
    {
      Serial.println("Got end sequence!");
      return true;
    }

    // Delay before next poll
    delay(100);
  }

  return false; // Timeout
}

bool check_end_sequence(uint8_t *buffer)
{
  return (buffer[0] == 0xBE && buffer[1] == 0xEF);
}

void _print_buffer(const char *label, uint8_t *buffer, uint8_t size)
{
  Serial.print("Packet size: ");
  Serial.print(size);
  Serial.print(" | ");
  Serial.print(label);
  Serial.print(" Buffer: ");
  for (int i = 0; i < size; i++)
  {
    Serial.print("0x");
    if (buffer[i] < 16)
      Serial.print("0");
    Serial.print(buffer[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}
/*
 * Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>
 * Updated 2020 TMRh20
 * Updated 2021 Richard Campen <richard@campen.co>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 */

/**
 * Channel scanner
 *
 * Example to detect interference on the various channels available.
 * This is a good diagnostic tool to check whether you're picking a
 * good channel for your application.
 *
 * Inspired by cpixip.
 * See http://arduino.cc/forum/index.php/topic,54795.0.html
 */


#include "RF24.h"
#include "printf.h"

RF24 radio(9, 10);  // CE, CSN

const uint8_t CHANNEL_COUNT = 126;
uint8_t SCAN_VALUES[CHANNEL_COUNT];
int LISTEN_DELAY = 128;  // microseconds

/** 
 * Setup
 */


void setup(void)
{
  printPreamble();
  initRadio();
  radio.printDetails();
  Serial.println();
}

void printPreamble() {
  Serial.begin(115200);
  printf_begin();
  Serial.println("RF24 scanner");
  Serial.println();
}

RF24 initRadio() {  
  radio.begin();

  // don't set this to false if you actually want
  // to transmit/recieve legit data.
  radio.setAutoAck(false);
  
  // Get into standby mode
  radio.startListening();
  radio.stopListening();

  return radio;
}

/** 
 * Loop
 */


void loop(void) {

  int repititions = 100;

  clearPreviousValues();
  scanChannels(repititions);
  printValues();
}


void clearPreviousValues() {
  memset(SCAN_VALUES, 0, sizeof(SCAN_VALUES));
}

void scanChannels(int repeat) {

  while (repeat--) {
    int i = CHANNEL_COUNT;
    while (i--) {
      listenToChannel(i);
    }
  }
}

void listenToChannel(int channel) {

  radio.setChannel(channel);

  radio.startListening();
  delayMicroseconds(LISTEN_DELAY);
  radio.stopListening();

  // only increment the values if we actually got a signal.
  if ( radio.testCarrier() ) {
    ++SCAN_VALUES[channel];
  }
}

void printValues() {
  
  int i = 0;
  while ( i < CHANNEL_COUNT ) {
    // clamp values to a single hex digit
    Serial.print(min(0xf, SCAN_VALUES[i]), HEX);
    ++i;
  }
  Serial.println();
}

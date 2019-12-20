/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : main
 * @created     : Saturday Oct 12, 2019 17:32:14 MDT
 */

#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>

extern "C" {
#include "messages.h"
}

byte mac[] = {
  0x00, 0x08, 0xDC, 0x51, 0x39, 0xB4
};

IPAddress ip(192, 168, 0, 2);

EthernetServer server(80);

rmt_message rmt;
char buffer[10];

void setup() {
  Ethernet.init(10);
  Ethernet.begin(mac, ip);

  rmt = (rmt_message){0};
  memset(buffer, 0, 10);
  pinMode(33, OUTPUT);
  digitalWrite(33, HIGH);
}



void loop() {
  EthernetClient client = server.available();
  while(client.connected()) {
    if(client.available()) {
      client.read(buffer, 4);
      client.write("Erp", 4);
      if(rmt_from_wire(&rmt, buffer)) {
        rmt.lin++;
        rmt.ang++;
        if(rmt_to_wire(&rmt)) {
          client.write(rmt.buffer, 5);
        } else {
          client.write("Erp", 4);
        }
      } else {
        client.write("Erm", 4);
      }
    }
  }

  delay(10);
  client.stop();
}

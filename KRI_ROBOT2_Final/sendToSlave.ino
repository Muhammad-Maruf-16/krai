void sendToSlave() {
  char buf[64];
  sprintf(buf, "A%dB%dC%dD%dE%dF%dG%dH%dI\n", deRPM, beRPM, kaRPM, kiRPM, Up, Down, loss, bost);
  Serial1.print(buf);
  // Serial.print(buf);
}


/*
4
2
1
3
maju +--+
maju kanan
400
100
300
170

kanan kanan

170
300
400
100

kanan kiri
400
100
170
300

kiri kanan
300
170
100
400


kiri kiri
100
-400
300
-170






*/
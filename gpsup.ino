int startgps(){
  gsm.SimpleWriteln("AT+CGNSPWR=1");
  gsm.WaitResp(2000, 100, "OK");
  gsm.SimpleWriteln("AT+CGNSINF");
  //gsm.WaitResp(2000, 100, "OK");
  char tempGPSData[500]; int count; String gpsMessage="";
  count = gsm.read(tempGPSData, 500);
    //gsm.WaitResp(2000, 100, "OK");
    Serial.println("data received");
    for(int i=0; i<count; i++){
      gpsMessage += tempGPSData[i];
    }
    int satu = gpsMessage.indexOf(',');
    int dua = gpsMessage.indexOf(',', satu+1);
    int tiga = gpsMessage.indexOf(',', dua+1);
    int empat = gpsMessage.indexOf(',', tiga+1);
    int lima = gpsMessage.indexOf(',', empat+1);
    int enam = gpsMessage.indexOf(',', lima+1);
    int tujuh = gpsMessage.indexOf(',', enam+1);

    char aa[gpsMessage.length()];
    gpsMessage.toCharArray(aa, gpsMessage.length());

    //date
    dateGPS="";
    for(int i=dua+1; i<tiga; i++)
      dateGPS += aa[i];

    Serial.println("date yymmdd hhmmss: "+dateGPS);

    //latitude
    latitudeGPS="";
    for(int i=tiga+1; i<empat; i++)
      latitudeGPS += aa[i];

    Serial.println("latitude: "+latitudeGPS);

    //longitude
    longitudeGPS="";
    for(int i=empat+1; i<lima; i++)
      longitudeGPS += aa[i];

    Serial.println("longitude: "+longitudeGPS);

    //altitude
    altitudeGPS="";
    for(int i=lima+1; i<enam; i++)
      altitudeGPS += aa[i];

    Serial.println("altitude: "+altitudeGPS);

    //speed
    speedGPS="";
    for(int i=enam+1; i<tujuh; i++)
      speedGPS += aa[i];

    Serial.println("speed: "+speedGPS);

    return 1;
}


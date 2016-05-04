void startDevice(){
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  Serial.println("Serial started");
  
  //powerup gsm module
  pinMode(2, OUTPUT);
  pinMode(3, INPUT);
  if(digitalRead(3) == LOW){
  	  Serial.println("powerup gsm module");
	  digitalWrite(2, HIGH);
	  delay(2000);
	  digitalWrite(2, LOW);  	
  }
  else
  	Serial.println("gsm module already powered");

  Serial.println("Start GSM module");
  if (gsm.begin(9600)) {
    Serial.println("status=READY");
    started = true;
  }
  else
    Serial.println("status=IDLE");
}

int startSAPBR(){
      gsm.SimpleWriteln("AT+SAPBR=3,1,Contype,GPRS");
      gsm.WaitResp(2000, 100, "OK");
      if(!gsm.IsStringReceived("OK"))
          return 0;

      gsm.SimpleWriteln("AT+SAPBR=3,1,APN,internet");
      gsm.WaitResp(2000, 100, "OK");
      if(!gsm.IsStringReceived("OK"))
          return 0;

      gsm.SimpleWriteln("AT+SAPBR=1,1");
      gsm.WaitResp(2000, 100, "OK");

      gsm.SimpleWriteln("AT+SAPBR=2,1");
      gsm.WaitResp(2000, 100, "OK");

      Serial.println("sapbr started");
      return 1;      
}
int count;
int getdata(char* result, int resultlength, String id){
	count = 0; message = "";

	gsm.SimpleWriteln("AT+HTTPINIT");
    gsm.WaitResp(2000, 100, "OK");

	gsm.SimpleWriteln("AT+HTTPPARA=CID,1");
    gsm.WaitResp(2000, 100, "OK"); 

    gsm.SimpleWriteln("AT+HTTPPARA=URL,busway.merah-putih.co.id/retrieve_busway?id="+id);
    gsm.WaitResp(2000, 100, "OK");   

	gsm.SimpleWriteln("AT+HTTPACTION=0");
    gsm.WaitResp(2000, 100, "OK");    
    gsm.WaitResp(2000, 100, "+HTTPACTION:");    

    gsm.SimpleWriteln("AT+HTTPREAD");
    if(!gsm.IsStringReceived("OK"))
      count = gsm.read(result, resultlength);
//    Serial.println("data received");
//    Serial.println(result);
//    Serial.println(count);
    for(int i=0; i<count; i++){
    	message += result[i];
    }
    // delay(5000);
    // gsm.SimpleWriteln("AT+HTTPTERM");
    // gsm.WaitResp(2000, 100, "OK");
}
int postdata(String id, String lokasi, int tinggi, int temperatur, String engine, String door){
	Serial.println("try to post data");
  //StaticJsonBuffer<200> jsonBuffer;
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["id_bis"] = id;
  root["data_lokasi"] = lokasi;
  root["ketinggian"] = tinggi;
  root["temperatur"] = temperatur;
  root["engine_status"] = engine;
  root["door_status"] = door;
  String mee; root.printTo(mee);
  
	  gsm.SimpleWriteln("AT+HTTPPARA=URL,busway.merah-putih.co.id/insert_busway");
    gsm.WaitResp(2000, 100, "OK");   
//    if(!gsm.IsStringReceived("OK"))
//          return 0;
          
    gsm.SimpleWriteln("AT+HTTPDATA=900,2000");
    gsm.WaitResp(2000, 100, "Download");
//    if(!gsm.IsStringReceived("OK"))
//          return 0;
          
    gsm.SimpleWriteln(mee);
    gsm.WaitResp(10000, 100, "OK"); 
//    if(!gsm.IsStringReceived("OK"))
//          return 0;
    
	  gsm.SimpleWriteln("AT+HTTPACTION=1");
    gsm.WaitResp(2000, 100, "OK");
    switch(gsm.WaitResp(10000, 10, "+HTTPACTION")) {
     case RX_TMOUT_ERR:
          Serial.println("timeout");
          return 0;
          break;
     case RX_FINISHED_STR_NOT_RECV:
          Serial.println("not receive");
          return 0;
          break;
     case RX_FINISHED_STR_RECV:
          Serial.println("send data to database success");
          break;
     }
//          return 0;
  return 1;
}

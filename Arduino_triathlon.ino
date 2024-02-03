void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  String sports[4] = {"swim","bike","run","total"};
  String times[3] = {"hours","minutes","seconds"};
  long totals[4] = {0,0,0,0};
  long input = 0;
  
  const int secPerHour = 3600;
  const int secPerMin = 60;
  const int num_of_events = 3;
  

  for (int i = 0; i < num_of_events; i ++){
    for (int j = 0; j < 3; j ++){
      input = getInput("Eter number of " + sports[i] + " " + times[j]);
//      Serial.println(input);
//      Serial.println(pow(60,(2-j)));
//      Serial.println(input*pow(60,(2-j)));
      input *= round(pow(60,(2-j)));
//      Serial.println(input);
      totals[i] += input;
      totals[3] += input;
    }
  }

  
  for (int i = 0; i < num_of_events+1; i ++){
    long totalSec = totals[i];    

    Serial.print("Time taken for ");
    Serial.print(sports[i]);
    Serial.print(" ");
    Serial.print(totalSec / secPerHour);
    Serial.print("hrs ");
    totalSec %= secPerHour;
    Serial.print(totalSec / secPerMin);
    Serial.print("mins ");
    totalSec %= secPerMin;
    Serial.print(totalSec);
    Serial.println("secs ");
  }
}

long getInput(String x){
  Serial.println(x);
  while (!Serial.available()) {};
  return Serial.parseInt();
}



void loop() {
  // put your main code here, to run repeatedly:

}

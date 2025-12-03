#include<Keypad.h>
const char nb_row=4;
const char nb_col=4;
const char rows[nb_row]={5,4,3,2};
const char cols[nb_col]={6,7,8,9};
char values[nb_row][nb_col] = {
    {'a','b','c','d'},
    {'a','b','c','d'},
    {'a','b','c','d'},
    {'a','b','c','d'}
};

Keypad key=Keypad(makeKeymap(values),rows,cols,nb_row,nb_col);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  char key_pressed=key.getKey();
  if(key_pressed){
    Serial.println(key_pressed);
  }

}

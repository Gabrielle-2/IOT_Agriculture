#include <LiquidCrystal_I2C.h>
#include<Keypad.h>
const char nb_row=4;
const char nb_col=4;
const char rows[nb_row]={5,4,3,2};
const char cols[nb_col]={6,7,8,9};
char values[nb_row][nb_col] = {
    {'1','2','3','m'},
    {'4','5','6','d'},
    {'7','8','9','#'},
    {'o','0','n','#'}
};
LiquidCrystal_I2C lcd(0x27, 16, 2); // Format -> (Address,Width,Height )
Keypad key=Keypad(makeKeymap(values),rows,cols,nb_row,nb_col);
void setup()
{
    // initialize the lcd
    lcd.init();
    // Turn on the Backlight
    lcd.backlight();
}

void loop()
{
    lcd.setCursor(0,0);
  lcd.println("Bienvenue Gaby  ");
  delay(2000);

  lcd.clear();
  
  int pos=0;

  char value='0';
  String number="";
  do{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Enter land Nb     ");
  
    value=key.getKey();
   
  if(value){
    int intvalue=String(value).toInt();
    if((int)value-'0'<=9){
      number+=value;
    
    }

    if(value=='d'){
      number.remove(number.length()-1);
    }
    if(value=='o'){
      lcd.clear();
      break;
    }
    
  }
   lcd.setCursor(0,1);
      lcd.print(number); 
      delay(100);
  
     
  }while(true);

  lcd.clear();

    delay(100);
}

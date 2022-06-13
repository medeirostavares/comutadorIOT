#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <rotary.h>                 // rotary handler

LiquidCrystal_I2C lcd(0x27,20,4);
 
#define PINA A0
#define PINB A1
#define PUSHB A2
 
// Initialize the Rotary object
// Rotary(Encoder Pin 1, Encoder Pin 2, Button Pin) Attach center to ground
Rotary r = Rotary(PINA, PINB, PUSHB);        // there is no must for using interrupt pins !!
 
int CursorLine = 1;
int DisplayFirstLine = 1;
int menu = 0;

//MENU 0:
char* MenuEstadoLine[] = {" Tocantins", " Goias", " Bahia", " Para", " Maranhao", " Piaui", " Minas Gerais"};
int MenuEstadoItems = 7;

//MENU 1:
char* MenuLigaLine[] = {" 07:30", " 08:00", " 08:30", " 09:00", " 09:30"};
int MenuLigaItems = 5;

//MENU 2:
char* MenuDesligaLine[] = {" 21:30", " 22:00", " 22:30", " 23:00"};
int MenuDesligaItems = 4;

void setup ()
{
  digitalWrite (PINA, HIGH);     // enable pull-ups
  digitalWrite (PINB, HIGH);
  digitalWrite (PUSHB, HIGH);
 
  lcd.init(); 
  lcd.backlight(); 
  lcd.clear (); // go home
  lcd.setCursor(1, 0);
  lcd.print ("Comutador IOT 1.0");
  lcd.setCursor(2, 2);
  lcd.print("IFTO - Colinas");
  delay(6000);
  print_menu(menu);
 
}  // end of setup
 
void loop ()
{
  volatile unsigned char result = r.process();
 
  if (result == DIR_CCW) {
    move_up(menu);
    print_menu(menu);
  } else if (result == DIR_CW) {
    move_down(menu);
    print_menu(menu);
  }
 
  if (r.buttonPressedReleased(25)) {
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Selecionado:");
    lcd.setCursor(0, 2);
    selection(menu);
    print_menu(menu);
  } //endif buttonPressedReleased
} //End loop()
 
void print_menu(int menuSelected)
{
  int n = 4;                      //4 rows
  lcd.clear();
  lcd.setCursor(0, 0);
  if(menuSelected == 0)
  {
      lcd.print("  Selecione o Estado  ");
      if (MenuEstadoItems == 1) {           //if only 1 item
        n = 2;
      } else if (MenuEstadoItems == 2) {    //if only 2 item
        n = 3;
      }
      for (int i = 1; i < n; i++)     // row 0 is used for title Main Menu
      {
        lcd.setCursor(1, i);
        lcd.print(MenuEstadoLine[DisplayFirstLine + i - 2]);
      }
      lcd.setCursor(0, (CursorLine - DisplayFirstLine) + 1);
      lcd.print(">");
  }

  if(menuSelected == 1)
  {
      lcd.print("  Hora Ativar");
      if (MenuLigaItems == 1) {           //if only 1 item
        n = 2;
      } else if (MenuLigaItems == 2) {    //if only 2 item
        n = 3;
      }
      for (int i = 1; i < n; i++)     // row 0 is used for title Main Menu
      {
        lcd.setCursor(1, i);
        lcd.print(MenuLigaLine[DisplayFirstLine + i - 2]);
      }
      lcd.setCursor(0, (CursorLine - DisplayFirstLine) + 1);
      lcd.print(">");
  }

  if(menuSelected == 2)
  {
      lcd.print("  Hora Desativar");
      if (MenuDesligaItems == 1) {           //if only 1 item
        n = 2;
      } else if (MenuDesligaItems == 2) {    //if only 2 item
        n = 3;
      }
      for (int i = 1; i < n; i++)     // row 0 is used for title Main Menu
      {
        lcd.setCursor(1, i);
        lcd.print(MenuDesligaLine[DisplayFirstLine + i - 2]);
      }
      lcd.setCursor(0, (CursorLine - DisplayFirstLine) + 1);
      lcd.print(">");
  }
  
} //end print_menu
 
void move_down(int menuSelected)
{
  int MenuItems = NULL;
  if(menuSelected == 0)
    MenuItems = MenuEstadoItems;
  if(menuSelected == 1)
    MenuItems = MenuLigaItems;
  if(menuSelected == 2)
    MenuItems = MenuDesligaItems;
  
  if (CursorLine == (DisplayFirstLine + 3 - 1)) {
    DisplayFirstLine++;
  }
  //If reached last item...roll over to first item
  if (CursorLine == MenuItems) {
    CursorLine = 1;
    DisplayFirstLine = 1;
  } else {
    CursorLine = CursorLine + 1;
  }
} //end move_down
 
void move_up(int menuSelected)
{
  int MenuItems = NULL;
  if(menuSelected == 0)
    MenuItems = 7;
  if(menuSelected == 1)
    MenuItems = 4;
  if(menuSelected == 2)
    MenuItems = 4;    
  
  if ((DisplayFirstLine == 1) & (CursorLine == 1)) {
    if (MenuItems < 3) {
      //Do nothing
    } else {
      DisplayFirstLine = MenuItems - 2;
    }
  } else if (DisplayFirstLine == CursorLine) {
    DisplayFirstLine--;
  }
 
  if (CursorLine == 1) {
    if (MenuItems < 3) {
      //Do nothing
    } else {
      CursorLine = MenuItems; //roll over to last item
    }
  } else {
    CursorLine = CursorLine - 1;
  }
} //end move_up
 
void selection(int menuSelected)
{
    if(menuSelected == 0)
    {
          switch (CursorLine - 1) 
          {
                case 0:
                  lcd.print("Tocantins    ");
                  //set a flag or do something....
                  break;
                case 1:
                  lcd.print("Goias    ");
                  //set a flag or do something....
                  break;
                case 2:
                  lcd.print("Bahia    ");
                  //set a flag or do something....
                  break;
                case 3:
                  lcd.print("Para    ");
                  //set a flag or do something....
                  break;
                case 4:
                  lcd.print("Maranhao    ");
                  //set a flag or do something....
                  break;
                case 5:
                  lcd.print("Piaui    ");
                  //set a flag or do something....
                  break;
                case 6:
                  lcd.print("Minas Gerais    ");
                  //set a flag or do something....
                  break;
                default:
                  break;
                  
          } //end switch
          menu = 1;
    } //end if

    if(menuSelected == 1)
    {
          //{" 07:30", " 08:00", " 08:30", " 09:00", " 09:30"};
          switch (CursorLine - 1) 
          {
                case 0:
                  lcd.print("07:30    ");
                  //set a flag or do something....
                  break;
                case 1:
                  lcd.print("08:00    ");
                  //set a flag or do something....
                  break;
                case 2:
                  lcd.print("08:30    ");
                  //set a flag or do something....
                  break;
                case 3:
                  lcd.print("09:00    ");
                  //set a flag or do something....
                  break;
                case 4:
                  lcd.print("09:30    ");
                  //set a flag or do something....
                  break;
                default:
                  break;
                  
          } //end switch
          menu = 2;
    } //end if

    if(menuSelected == 2)
    {
          //{" 21:30", " 22:00", " 22:30", " 23:00"};
          switch (CursorLine - 1) 
          {
                case 0:
                  lcd.print("21:30    ");
                  //set a flag or do something....
                  break;
                case 1:
                  lcd.print("22:00    ");
                  //set a flag or do something....
                  break;
                case 2:
                  lcd.print("22:30    ");
                  //set a flag or do something....
                  break;
                case 3:
                  lcd.print("23:00    ");
                  //set a flag or do something....
                  break;
                default:
                  break;
                  
          } //end switch
           menu = 3; 
    } //end if

    if(menuSelected == 3)
    {
          //??
          switch (CursorLine - 1) 
          {
                case 0:
                  lcd.print("??    ");
                  //set a flag or do something....
                  break;
                case 1:
                  lcd.print("??    ");
                  //set a flag or do something....
                  break;
                case 2:
                  lcd.print("??    ");
                  //set a flag or do something....
                  break;
                case 3:
                  lcd.print("??    ");
                  //set a flag or do something....
                  break;
                default:
                  break;
                  
          } //end switch
          menu = 4;
    } //end if
 
  delay(2000);
  DisplayFirstLine = 1;
  CursorLine = 1;
} //End selection

// Definicija tastera na daljinskom s e-bay-a
#ifndef IR_CARMP3

#define IR_START_STOP 0xFFA25D // Code for "START/STOP time" button [CH-]
#define IR_PAUSE_1M 0xFF629D // Code for "PAUSE 1 minute" button [CH]
#define IR_REARM 0xFFE21D //Code for "Rearm all" button [CH+]
#define IR_L_TOUCH_PLUS 0xFF22DD //Code for "LEFT opponent touch +" button [|<<]
#define IR_L_TOUCH_MINUS 0xFFE01F //Code for "LEFT opponent touch -" button[-]
#define IR_PERIOD_PLUS 0xFF02FD //Code for "Mach PERIOD +" button [>>|]
#define IR_PERIOD_MINUS 0xFFA857 //Code for "Mach PERIOD -" button [+]
#define IR_R_TOUCH_PLUS 0xFFC23D //Code for "RIGHT opponent touch +" button [>||]
#define IR_R_TOUCH_MINUS 0xFF906F //Code for "RIGHT opponent touch -" button [EQ]
#define IR_L_YEL_CARD 0xFF6897 //Code for "LEFT opponent YELLOW card" button [0]
#define IR_L_RED_CARD 0xFF30CF //Code for "LEFT opponent RED card" button [1]
#define IR_R_YEL_CARD 0xFFB04F //Code for "RIGHT opponent YELLOW card" button [200+]
#define IR_R_RED_CARD 0xFF7A85 //Code for "RIGHT opponent RED card" button [3]
#define IR_RESET_CARDS 0xFF18E7 //Code for "Reset all cards" button [2]
#define IR_ADVANTAGE 0xFF9867 //Code for "ADVANTAGE"button [100+]
#define IR_SETTINGS 0xFF10EF // Code for "Enter SETTINGS" button [4]
#define IR_SAVE_AUTO_MAN 0xFF42BD //Code for "Save SETTINGS or AUTO/MANUAL mode" button [7]
#define IR_MINUTES_PLUS 0xFF38C7 //Code for "MINUTES +" button --> Duration of match in settings [5]
#define IR_MINUTES_MINUS 0xFF4AB5 //Code for "MINUTES -" button --> Duration of match in settings [8]
#define IR_POINTS_PLUS 0xFF5AA5 //Code for "POINTS +" button --> Maximum number of points in match [6]
#define IR_POINTS_MINUS 0xFF52AD //Code for "POINTS -" button --> Maximum number of points in match [9]

#endif

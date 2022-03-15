#ifndef _SerialInputParsing_h
#define _SerialInputParsin_h


int getNegativeSpeed_Port(int num);
int getPositiveSpeed_Port(int num, int recievedVal);
int getSpeed_Port(int num, int recievedVal);

int getNegativeRot_Port(int num);
int getPositiveRot_Port(int num, int recievedVal);
int getRot_Port(int num, int recievedVal);

int getNegativeSpeed_BT(int num);
int getPositiveSpeed_BT(int num, int recievedVal);
int getSpeed_BT(int num1);

int getNegativeRot_BT(int num);
int getPositiveRot_BT(int num, int recievedVal);
int getRot_BT(int num);

#endif

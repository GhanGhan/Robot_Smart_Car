#ifndef _MotorDriver_h
#define _MotorDriver_h
class MotorDriver
{
    public:
        MotorDriver();
        MotorDriver(int in1, int in2, int en);
        ~MotorDriver();
        void mdInit();
        void setMotorForward();
        void setMotorReverse();
        void applyControlInput(float controlInput);


    private:
        int input1;
        int input2;
        int enableMotor;
};

#endif
#ifndef _Encoder_h
#define _Encoder_h

const int MAX_WHEELS = 2;
class Encoder
{
    public:
        Encoder();
        ~Encoder();
        Encoder(int encoderA, int encoderB, int totalPulses, int delayTimeMills);
        void EncoderInit(int wheelNumber, int type);
        float getRPM();
        void setPulsesToZeros();


    private:
        int encoderPinA_Int;
        int encoderPinB;
        int previous_A;
        volatile bool direction;
        volatile int pulses;
        int pulses_per_rotation;
        int mode[3];
        int wheelID;
        int delayTime;
        static Encoder *myInstance[];
        static int slot[];
        static void globalISR0();
        static void globalISR1();
        void wheelSpeed();
};

#endif



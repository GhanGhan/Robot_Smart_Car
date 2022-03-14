#ifndef _PID_hpp
#define _PID_hpp

class PID
{
    public:
        PID();
        ~PID();
        PID(float proportionalK, float integralK, float derivativeK, int min, int max, int delay);
        void setDelayTime(int delayMilliseconds);
        void saturateControl();
        float getControl();
        void calcControl(float desired, float currentVal);
        float getError();
        float getOldError();
        float getErrorIntegral();
        float getErrorDerivative();
        void setErrorInt_DerToZero();
        void setMinMax(int minVal, int maxVal);

        


    private:
        float Kp;
        float Ki;
        float Kd;
        float error;
        float oldError;
        float errorIntegral;
        float errorDerivative;
        int delayTime;
        float control;
        bool zeroInt_DerFlag;
        int min;
        int max;
        
};

#endif
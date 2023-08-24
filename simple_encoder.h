/**
 * @file simple_encoder.h
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-06-13
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Button/Button.h"


namespace SIMPLE_ENCODER {


    class ENCODER {
        private:
            int _pinA;
            int _pinB;
            int _pinBTN;
            TaskHandle_t _ecTaskHandle;
            unsigned int _ecTaskPriority;
            int _oldPosition;
        public:
            /* Button */
            Button btn;
            /* Public methods */
            ENCODER();
            ~ENCODER();
            void init(int pinA = -1, int pinB = -1, int pinBTN = -1);
            void unInit();
            int getDirection();
            int getPosition();
            void resetPosition();
            bool wasMoved();
    };


}

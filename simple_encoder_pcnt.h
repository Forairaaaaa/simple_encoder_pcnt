/**
 * @file simple_encoder_pcnt.h
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-07-14
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include "simple_encoder_pcnt.h"
#include <driver/pulse_cnt.h>
#include <esp_log.h>
#include <driver/gpio.h>


namespace SIMPLE_ENCODER_PCNT
{
    struct EncoderConfig_t
    {
        /* Pin configs */
        int encoderA_gpio_num = GPIO_NUM_NC;
        int encoderB_gpio_num = GPIO_NUM_NC;

        /* Pcnt unit configs */
        int pcnt_high_limit = 1000;
        int pcnt_low_limit = -1000;
    }; 

    class Encoder
    {
        private:
            EncoderConfig_t _cfg;
            pcnt_unit_handle_t _pcnt_unit_handler;

            int _position_buffer;

        public:
            Encoder();
            ~Encoder();

            /**
             * @brief Configs
             * 
             * @return EncoderConfig_t 
             */
            inline EncoderConfig_t config(void) { return _cfg; }
            inline void config(const EncoderConfig_t& cfg) { _cfg = cfg; }

            void setPins(const int& encoderA_gpio_num, const int& encoderB_gpio_num);
            
            /**
             * @brief Init with gpio set
             * 
             * @param encoderA_gpio_num 
             * @param encoderB_gpio_num 
             */
            inline void init(const int& encoderA_gpio_num, const int& encoderB_gpio_num)
            {
                setPins(encoderA_gpio_num, encoderB_gpio_num);
                init();
            }
            void init(void);
            
            /* Wrap */
            inline esp_err_t enable() { return pcnt_unit_enable(_pcnt_unit_handler); }
            inline esp_err_t disable() { return pcnt_unit_disable(_pcnt_unit_handler); }
            inline esp_err_t start() { return pcnt_unit_start(_pcnt_unit_handler); }
            inline esp_err_t stop() { return pcnt_unit_stop(_pcnt_unit_handler); }
            inline esp_err_t clear() { return pcnt_unit_clear_count(_pcnt_unit_handler); }

            /**
             * @brief Read encoder current count
             * 
             * @return int 
             */
            int readCount();

            /**
             * @brief Read encoder current count then clear
             * 
             * @return int 
             */
            int readCountClear();


            /* Same API to Simple encoder */
            int getDirection();
            inline int getPosition() { return readCount(); }
            void resetPosition();
            bool wasMoved();
    };

}

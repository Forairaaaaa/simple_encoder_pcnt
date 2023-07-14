/**
 * @file simple_encoder_pcnt.cpp
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-07-14
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "simple_encoder_pcnt.h"
#include <esp_log.h>


namespace SIMPLE_ENCODER_PCNT
{
    const  char* TAG = "Encoder";


    Encoder::Encoder()
    { 
        _pcnt_unit_handler = NULL;
    }


    Encoder::~Encoder()
    {
        pcnt_del_unit(_pcnt_unit_handler);
    }


    void Encoder::setPins(const int& encoderA_gpio_num, const int& encoderB_gpio_num)
    {
        _cfg.encoderA_gpio_num = encoderA_gpio_num;
        _cfg.encoderB_gpio_num = encoderB_gpio_num;
    }


    void Encoder::init(void)
    {
        if (_cfg.encoderA_gpio_num <= GPIO_NUM_NC || _cfg.encoderB_gpio_num <= GPIO_NUM_NC)
        {
            ESP_LOGE(TAG, "Invalid pin config");
            return;
        }

        /* Pcnt setups */
        pcnt_unit_config_t pcnt_unit_config;
        pcnt_unit_config.high_limit = _cfg.pcnt_high_limit;
        pcnt_unit_config.low_limit = _cfg.pcnt_low_limit;
        pcnt_unit_config.flags.accum_count = true;          // enable counter accumulation
        ESP_ERROR_CHECK(pcnt_new_unit(&pcnt_unit_config, &_pcnt_unit_handler));

        /* Filter setups */
        pcnt_glitch_filter_config_t pcnt_filter_config;
        pcnt_filter_config.max_glitch_ns = 1000;
        ESP_ERROR_CHECK(pcnt_unit_set_glitch_filter(_pcnt_unit_handler, &pcnt_filter_config));

        /* Pin setups */
        pcnt_chan_config_t pcnt_chan_a_config;
        pcnt_chan_a_config.edge_gpio_num = _cfg.encoderA_gpio_num;
        pcnt_chan_a_config.level_gpio_num = _cfg.encoderB_gpio_num;
        pcnt_channel_handle_t pcnt_chan_a_handler = NULL;
        ESP_ERROR_CHECK(pcnt_new_channel(_pcnt_unit_handler, &pcnt_chan_a_config, &pcnt_chan_a_handler));
        pcnt_chan_config_t pcnt_chan_b_config;
        pcnt_chan_b_config.edge_gpio_num = _cfg.encoderB_gpio_num;
        pcnt_chan_b_config.level_gpio_num = _cfg.encoderA_gpio_num;
        pcnt_channel_handle_t pcnt_chan_b_handler = NULL;
        ESP_ERROR_CHECK(pcnt_new_channel(_pcnt_unit_handler, &pcnt_chan_b_config, &pcnt_chan_b_handler));

        ESP_ERROR_CHECK(pcnt_channel_set_edge_action(pcnt_chan_a_handler, PCNT_CHANNEL_EDGE_ACTION_DECREASE, PCNT_CHANNEL_EDGE_ACTION_INCREASE));
        ESP_ERROR_CHECK(pcnt_channel_set_level_action(pcnt_chan_a_handler, PCNT_CHANNEL_LEVEL_ACTION_KEEP, PCNT_CHANNEL_LEVEL_ACTION_INVERSE));
        ESP_ERROR_CHECK(pcnt_channel_set_edge_action(pcnt_chan_b_handler, PCNT_CHANNEL_EDGE_ACTION_INCREASE, PCNT_CHANNEL_EDGE_ACTION_DECREASE));
        ESP_ERROR_CHECK(pcnt_channel_set_level_action(pcnt_chan_b_handler, PCNT_CHANNEL_LEVEL_ACTION_KEEP, PCNT_CHANNEL_LEVEL_ACTION_INVERSE));
        ESP_ERROR_CHECK(pcnt_unit_add_watch_point(_pcnt_unit_handler, _cfg.pcnt_high_limit));
        ESP_ERROR_CHECK(pcnt_unit_add_watch_point(_pcnt_unit_handler, _cfg.pcnt_low_limit));
        ESP_ERROR_CHECK(pcnt_unit_enable(_pcnt_unit_handler));
        ESP_ERROR_CHECK(pcnt_unit_clear_count(_pcnt_unit_handler));
        ESP_ERROR_CHECK(pcnt_unit_start(_pcnt_unit_handler));

        _position_buffer = 0;
    }


    int Encoder::readCount()
    {
        int ret = 0;
        pcnt_unit_get_count(_pcnt_unit_handler, &ret);
        return ret; 
    }


    int Encoder::readCountClear()
    {
        int ret = 0;
        pcnt_unit_get_count(_pcnt_unit_handler, &ret);
        clear();
        return ret;
    }


    int Encoder::getDirection()
    {
        if (getPosition() > _position_buffer)
        {
            return 1;
        }
        else if (getPosition() < _position_buffer)
        {
            return -1;
        }
        else 
        {
            return 0;
        }
    }


    void Encoder::resetPosition()
    {
        clear();
        _position_buffer = 0;
    }


    bool Encoder::wasMoved()
    {
        // if (getPosition() != _position_buffer)
        // {
        //     _position_buffer = getPosition();
        //     return true;
        // }
        // return false;
        

        // printf("test: %d, %d\n", getPosition(), _position_buffer);

        if (std::abs(getPosition() - _position_buffer) >= 3)
        {
            _position_buffer = getPosition();
            return true;
        }
        return false;
    }

}



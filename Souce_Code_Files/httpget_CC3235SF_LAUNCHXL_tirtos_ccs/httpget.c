/*
 * Copyright (c) 2015-2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== httpget.c ========
 *  HTTP Client GET example application
 */

/* BSD support */
#include <string.h>
#include <ti/display/Display.h>
#include <ti/net/http/httpclient.h>
#include "semaphore.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <time.h>
#include <unistd.h>

#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>
#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC32XX.h>
#include <ti/drivers/apps/LED.h>
#include <ti/drivers/apps/Button.h>

#include <ti/drivers/ADC.h>
#include "ti_drivers_config.h"

#define HOSTNAME              "http://api.thingspeak.com"
#define USER_AGENT            "HTTPClient (ARM; TI-RTOS)"
#define HTTP_MIN_RECV         (256)

extern Display_Handle display;
extern sem_t ipEventSyncObj;
extern void printError(char *errString,
                       int code);

// function to read output of mq135 sensor using adc on pin P60
int mq135_value()
{
    uint16_t adcValue;
    ADC_Handle   adc;
    ADC_Params   params;
    int_fast16_t res;

    ADC_init();

    ADC_Params_init(&params);
    adc = ADC_open(CONFIG_ADC_135, &params);

    if (adc == NULL) {
        while (1);
    }

    /* Blocking mode conversion */
    res = ADC_convert(adc, &adcValue);

    ADC_close(adc);

    return (int)adcValue;
}

// function to read output of mq2 sensor using adc on pin P59
int mq2_value()
{
    uint16_t adcValue;
    ADC_Handle   adc;
    ADC_Params   params;
    int_fast16_t res;

    ADC_init();

    ADC_Params_init(&params);
    adc = ADC_open(CONFIG_ADC_2, &params);

    if (adc == NULL) {
        while (1);
    }

    /* Blocking mode conversion */
    res = ADC_convert(adc, &adcValue);

    ADC_close(adc);

    return (int)adcValue;
}

// function to read output of mq7 sensor using adc on pin P58
int mq7_value()
{
    uint16_t adcValue;
    ADC_Handle   adc;
    ADC_Params   params;
    int_fast16_t res;

    ADC_init();

    ADC_Params_init(&params);
    adc = ADC_open(CONFIG_ADC_7, &params);

    if (adc == NULL) {
        while (1);
    }

    /* Blocking mode conversion */
    res = ADC_convert(adc, &adcValue);

    ADC_close(adc);

    return (int)adcValue;
}

// function to read output of mq4 sensor using adc on pin P57
int mq4_value()
{
    uint16_t adcValue;
    ADC_Handle   adc;
    ADC_Params   params;
    int_fast16_t res;

    ADC_init();

    ADC_Params_init(&params);
    adc = ADC_open(CONFIG_ADC_4, &params);

    if (adc == NULL) {
        while (1);
    }

    /* Blocking mode conversion */
    res = ADC_convert(adc, &adcValue);

    ADC_close(adc);

    return (int)adcValue;
}

// function to compute the aqi
int aqi_calc(int val1, int val2, int val3, int val4)
{
    return (val1 + val2 + val3 + val4) / 4;
}


/*
 *  ======== httpTask ========
 *  Makes a HTTP GET request
 */
void* httpTask(void* pvParameters)
{
    while(1)
    {
        bool moreDataFlag = false;
        char data[HTTP_MIN_RECV];
        int16_t ret = 0;
        int16_t len = 0;

        Display_printf(display, 0, 0, "Sending a HTTP GET request to '%s'\n",
                       HOSTNAME);

        HTTPClient_Handle httpClientHandle;
        int16_t statusCode;
        httpClientHandle = HTTPClient_create(&statusCode,0);
        if(statusCode < 0)
        {
            printError("httpTask: creation of http client handle failed",
                       statusCode);
        }

        ret =
            HTTPClient_setHeader(httpClientHandle,
                                 HTTPClient_HFIELD_REQ_USER_AGENT,
                                 USER_AGENT,strlen(USER_AGENT)+1,
                                 HTTPClient_HFIELD_PERSISTENT);
        if(ret < 0)
        {
            printError("httpTask: setting request header failed", ret);
        }

        ret = HTTPClient_connect(httpClientHandle,HOSTNAME,0,0);
        if(ret < 0)
        {
            printError("httpTask: connect failed", ret);
        }


//        storing the values we want to send to ThingSpeak server
        int val1 = mq135_value();
        int val2 = mq2_value();
        int val3 = mq7_value();
        int val4 = mq4_value();
        int val5 = aqi_calc();

//      creating a URL string for the HTTP request
        char buf[100];
        int j = snprintf(buf, sizeof(buf), "/update?api_key=DCCB0O4SRYS4XU6V&field1=%d&field2=%d&field3=%d&field4=%d&field5=%d", val1, val2, val3, val4, val5);

        #define REQUEST_URI buf


        ret =
            HTTPClient_sendRequest(httpClientHandle,HTTP_METHOD_GET,REQUEST_URI,
                                   NULL,0,
                                   0);
        if(ret < 0)
        {
            printError("httpTask: send failed", ret);
        }

        if(ret != HTTP_SC_OK)
        {
            printError("httpTask: cannot get status", ret);
        }

        Display_printf(display, 0, 0, "HTTP Response Status Code: %d\n", ret);

        len = 0;
        do
        {
            ret = HTTPClient_readResponseBody(httpClientHandle, data, sizeof(data),
                                              &moreDataFlag);
            if(ret < 0)
            {
                printError("httpTask: response body processing failed", ret);
            }
            Display_printf(display, 0, 0, "%.*s \r\n",ret,data);
            len += ret;
        }
        while(moreDataFlag);

        Display_printf(display, 0, 0, "Received %d bytes of payload\n", len);

        ret = HTTPClient_disconnect(httpClientHandle);
        if(ret < 0)
        {
            printError("httpTask: disconnect failed", ret);
        }

        HTTPClient_destroy(httpClientHandle);

        sleep(15);
    }
    return(0);
}

/*
 * Copyright (C) 2017 Inria
 *               2017 Inria Chile
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     tests
 *
 * @file
 * @brief       Semtech LoRaMAC test application
 *
 * @author      Alexandre Abadie <alexandre.abadie@inria.fr>
 * @author      Jose Alamos <jose.alamos@inria.cl>
 */

#include <stdio.h>

#ifdef MODULE_SEMTECH_LORAMAC_RX
#include "thread.h"
#include "msg.h"
#include <string.h>
#endif

#include "shell.h"
#include "semtech_loramac.h"

extern semtech_loramac_t loramac;


/* Your OTAA keys (replace with your real ones) */
static const uint8_t deveui[LORAMAC_DEVEUI_LEN] = {
    0xCA, 0xFE, 0xBA, 0xBE, 0x0B, 0x3D, 0xAC, 0x81
};
//CAFEBABE0B3DAC81
static const uint8_t appeui[LORAMAC_APPEUI_LEN] = {
    0xCA, 0xFE, 0xBA, 0xBE, 0x00, 0x00, 0x00, 0x00
};
//CAFEBABE00000000

//4C05733A112513C881255445A97047B1

static const uint8_t appkey[LORAMAC_APPKEY_LEN] = {
 0x4C, 0x05, 0x73, 0x3A, 0x11, 0x25, 0x13, 0xC8,
    0x81, 0x25, 0x54, 0x45, 0xA9, 0x70, 0x47, 0xB1
};



#ifdef MODULE_SEMTECH_LORAMAC_RX

#define FPORT_REBOOT            (30U)
#define FPORT_SONG              (33U)

static void _reboot(const uint32_t delay) {
    printf("Rebooting in %ld sec\n", delay);
    // TODO
}

static void _play_song(const char* song) {
    printf("Playing song: %s\n", song);
    // TODO

}

#define LORAMAC_RECV_MSG_QUEUE                   (4U)
static msg_t _loramac_recv_queue[LORAMAC_RECV_MSG_QUEUE];
static char _recv_stack[THREAD_STACKSIZE_DEFAULT];


static void *_wait_recv(void *arg)
{
    msg_init_queue(_loramac_recv_queue, LORAMAC_RECV_MSG_QUEUE);

    (void)arg;
    while (1) {
        /* blocks until something is received */
        switch (semtech_loramac_recv(&loramac)) {
            case SEMTECH_LORAMAC_RX_DATA:
                loramac.rx_data.payload[loramac.rx_data.payload_len] = 0;
                const uint8_t fport = loramac.rx_data.port;
                printf("Data received: %s, port: %d\n",
                (char *)loramac.rx_data.payload, fport);

                switch(fport) {
                 case FPORT_REBOOT: {
                    uint32_t delay = (uint8_t)(loramac.rx_data.payload[0] & 0xFF);
                    _reboot(delay);
                    break;
                }
               case FPORT_SONG: {
                    _play_song((char *)loramac.rx_data.payload);
                    break;
                }
                default: {
                    break;
                }
                }
                break;
            case SEMTECH_LORAMAC_RX_LINK_CHECK:
                printf("Link check information:\n"
                   "  - Demodulation margin: %d\n"
                   "  - Number of gateways: %d\n",
                   loramac.link_chk.demod_margin,
                   loramac.link_chk.nb_gateways);
                break;

            case SEMTECH_LORAMAC_RX_CONFIRMED:
                puts("Received ACK from network");
                break;

            case SEMTECH_LORAMAC_TX_SCHEDULE:
                puts("The Network Server has pending data");
                break;

            default:
                break;
        }
    }
    return NULL;
}
#endif

/* loramac shell command handler is implemented in
   sys/shell/commands/sc_loramac.c */

uint8_t fpayload[255];

static size_t convert_hex(uint8_t *dest, const char *src) {
    size_t i;
    int value;
    size_t count = strlen(src);
    for (i = 0; i < count && sscanf(src + i * 2, "%2x", &value) == 1; i++) {
        dest[i] = value;
    }
    return i;
}

static void _loramac_txhex_usage(void)
{
    puts("Usage: loramac_txhex <hexpayload> [<cnf|uncnf>] [port]");
}

static int _loramac_txhex_handler(int argc, char **argv)
{
    if (argc < 2) {
        _loramac_txhex_usage();
        return 1;
    }
    uint8_t cnf = CONFIG_LORAMAC_DEFAULT_TX_MODE;  /* Default: confirmable */
    uint8_t port = CONFIG_LORAMAC_DEFAULT_TX_PORT; /* Default: 2 */
    /* handle optional parameters */
    if (argc > 2) {
        if (strcmp(argv[2], "cnf") == 0) {
            cnf = LORAMAC_TX_CNF;
        }
        else if (strcmp(argv[2], "uncnf") == 0) {
            cnf = LORAMAC_TX_UNCNF;
        }
        else {
            _loramac_txhex_usage();
            return 1;
        }

        if (argc > 3) {
            port = atoi(argv[3]);
            if (port == 0 || port >= 224) {
                printf("error: invalid port given '%d', "
                        "port can only be between 1 and 223\n", port);
                return 1;
            }
        }
    }

    semtech_loramac_set_tx_mode(&loramac, cnf);
    semtech_loramac_set_tx_port(&loramac, port);


    // convert hexstring to uint8_t*
    const size_t len = convert_hex(fpayload, argv[1]);
    switch (semtech_loramac_send(&loramac,  fpayload, len)) {

        case SEMTECH_LORAMAC_NOT_JOINED:
            puts("Cannot send: not joined");
            return 1;

        case SEMTECH_LORAMAC_DUTYCYCLE_RESTRICTED:
            puts("Cannot send: dutycycle restriction");
            return 1;

        case SEMTECH_LORAMAC_BUSY:
            puts("Cannot send: MAC is busy");
            return 1;

        case SEMTECH_LORAMAC_TX_ERROR:
            puts("Cannot send: error");
            return 1;

        case SEMTECH_LORAMAC_TX_CNF_FAILED:
            puts("Fail to send: no ACK received");
            return 1;
    }

    puts("Message sent with success");
    return 0;
}


SHELL_COMMAND(loramac_txhex, "Send hextring message", _loramac_txhex_handler);


int joined=0;
int main(void)
{
#ifdef MODULE_SEMTECH_LORAMAC_RX
    thread_create(_recv_stack, sizeof(_recv_stack),
                  THREAD_PRIORITY_MAIN - 1, 0, _wait_recv, NULL, "recv thread");
#endif

    puts("--- LoRaWAN Test ---");
   // char line_buf[SHELL_DEFAULT_BUFSIZE];
   // shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

  while(1){
     puts("hello world53\n");

     semtech_loramac_set_deveui(&loramac,deveui);
     semtech_loramac_set_appeui(&loramac,appeui);

     semtech_loramac_set_appkey(&loramac,appkey);
     puts("intialized\n");
    if(joined==0){
        if (semtech_loramac_join(&loramac, LORAMAC_JOIN_OTAA) != SEMTECH_LORAMAC_JOIN_SUCCEEDED) {
          puts("Join procedure failed");
          return 1;
      }
     puts("Join procedure succeeded");
     joined=1;
     char *message = "This is RIOT";
    if (semtech_loramac_send(&loramac,
                              (uint8_t *)message, strlen(message)) != SEMTECH_LORAMAC_TX_DONE) {
           printf("Cannot send message '%s'\n", message);
         return 1;
     }
    }
    else{
        printf("already joined\n");
        
    }
    

  }

}

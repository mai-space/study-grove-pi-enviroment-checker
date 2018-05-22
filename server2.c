//
// Created by Joel Mai on 20.05.2018.
//
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

#include "grovepi.h"
#include <string.h>

#define INPUT 0
#define OUTPUT 1

    int main(){
        char server_message[256] = "Du hast den Server erreicht";
        strcat(server_message, "\n");

        //Erstelle Server Socket
        int server_socket;
        server_socket = socket(AF_INET, SOCK_STREAM, 0);

        //Definiere Server Adress-Familie
        struct sockaddr_in server_address;
        server_address.sin_family = AF_INET;           //Adress-Familie
        server_address.sin_port = htons(5678);         //Portnummer
        server_address.sin_addr.s_addr = INADDR_ANY;   //Vlt noch ändern in IP Adresse

        //bind den socket zu unser IP und port
        bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address));

        //listen to conections
        listen(server_socket, 5);

        int client_socket;

        //accept Funktion      Server Socket    IP des Clients      Size of IP
        client_socket = accept(server_socket, NULL, NULL);
        //Send Funktion     Clientsocket, definierte Nachricht und Size of der Nachricht
        send(client_socket, server_message, sizeof(server_message), 0);
        while(1) {
            char client_command[256];
            recv(client_socket, &client_command, sizeof(client_command), 0);
            printf("Client: %s\n", client_command);
            if (strcmp(client_command, "GET TEMP") == 0) {
                float f = getTemperature(8);
                printf("[temp = %2.02f C]\n", f);
                char *value = (char *) &f;
                send(client_socket, value, sizeof(value), 0);
            } else if (strcmp(client_command, "GET HUMIDITY") == 0) {
                float f = getHumidity(8);
                printf("[humidity = %.02f%%]\n", f);
                char *value = (char *) &f;
                send(client_socket, value, sizeof(value), 0);
            } else if (strcmp(client_command, "GET LIGHT") == 0) {
                int f = getLicht(4);
                printf("Light value: %d  Resistance: %0.2f \n", f);
                char value = (char *) &f;
                send(client_socket, value, sizeof(value), 0);
            } else if (strcmp(client_command, "GET SOUND") == 0) {
                int f = getsound(3);
                printf("SOUND: %d\n", f);
                char *value = (char *) &f;
                send(client_socket, value, sizeof(value), 0);
            } else {
                send(client_socket, client_command, sizeof(client_command), 0);
            }
        }
        //close Funktion
        close(server_socket);
        return 0;
    }


float getTemperature(int port);

int getsound(int port);

float getHumidity(int port);

int getLicht(int port);

float getTemperature(int port)
{
    float temp = 0;
    pinMode(port, INPUT);
    pi_sleep(1000);
    getTemperatureerature(&temp, port);
    return temp;
}

float getHumidity(int port)
{
    float humidity = 0;
    getHumidity(&humidity, port);
    pi_sleep(1000); //wait 1s
    return humidity;

}

int getLicht(int port)
{
    int value;
    float resistance;
    value = analogRead(port);
    resistance = (float)(1023 - value) * 10 / value;
    return value;
}

int getsound(int port)
{
    int sound = 0;
    pinMode(port, INPUT);
    pi_sleep(1000);
    int i = 0;
    int summe = 0;
    while(i<5)
    {
        sound = analogRead(port);
        pi_sleep(100);
        summe = summe + sound;
        i++;
    }
    return summe / i;
}

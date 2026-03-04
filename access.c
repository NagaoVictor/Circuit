#include <gpiod.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    struct gpiod_chip *chip;
    struct gpiod_line_settings *settings;
    struct gpiod_line_config *line_cfg;
    struct gpiod_request_config *req_cfg;
    struct gpiod_line_request *request;
    
    unsigned int offsets[] = {17, 27, 22};


    chip = gpiod_chip_open("/dev/gpiochip4");
    if (!chip) {
        perror("Erro ao abrir chip");
        return 1;
    }


    settings = gpiod_line_settings_new();
    gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT);


    line_cfg = gpiod_line_config_new();
    gpiod_line_config_add_line_settings(line_cfg, offsets, 3, settings);


    req_cfg = gpiod_request_config_new();
    gpiod_request_config_set_consumer(req_cfg, "Fluxo_C1815");


    request = gpiod_chip_request_lines(chip, req_cfg, line_cfg);
    if (!request) {
        perror("Erro ao solicitar linhas");
        gpiod_chip_close(chip);
        return 1;
    }

    printf("Sistema Pronto (libgpiod v2.0)\n");

    int choose;
    while(1) {
        printf("\n--- SELEÇÃO DE CAMINHO ---");
        printf("\n0: DESLIGAR TUDO");
        printf("\n1: CAMINHO A (LED 2)");
        printf("\n2: CAMINHO B (LED 3)");
        printf("\nEscolha: ");
        if(scanf("%d", &choose) != 1) {
            while (getchar() != '\n'); 
            continue;
        }

        if (choose == 0){
            gpiod_line_request_set_value(request, 17, GPIOD_LINE_VALUE_ACTIVE); 
            gpiod_line_request_set_value(request, 27, GPIOD_LINE_VALUE_INACTIVE);
            gpiod_line_request_set_value(request, 22, GPIOD_LINE_VALUE_INACTIVE);

        } else if (choose == 1){
            gpiod_line_request_set_value(request, 17, GPIOD_LINE_VALUE_INACTIVE);
            gpiod_line_request_set_value(request, 27, GPIOD_LINE_VALUE_ACTIVE); 
            gpiod_line_request_set_value(request, 22, GPIOD_LINE_VALUE_INACTIVE);

        } else if (choose == 2){
            gpiod_line_request_set_value(request, 17, GPIOD_LINE_VALUE_INACTIVE);
            gpiod_line_request_set_value(request, 27, GPIOD_LINE_VALUE_INACTIVE); 
            gpiod_line_request_set_value(request, 22, GPIOD_LINE_VALUE_ACTIVE); 

        } else {
            gpiod_line_request_set_value(request, 17, GPIOD_LINE_VALUE_INACTIVE); 
            gpiod_line_request_set_value(request, 27, GPIOD_LINE_VALUE_INACTIVE); 
            gpiod_line_request_set_value(request, 22, GPIOD_LINE_VALUE_INACTIVE);
        }
        
        
    }
    gpiod_line_request_release(request);
    gpiod_chip_close(chip);
    return 0;
}

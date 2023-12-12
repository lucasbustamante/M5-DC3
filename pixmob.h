#define IRLED 9   

// Variáveis do programa
const uint16_t pin_IR_LED = 9; // número do pino para ligar o transmissor IR
int tamanho;  // variável para guardar o número de bytes referente à cor enviada para a pulseira, necessário para a função IRSender

// Código de Cores que será enviado para a pulseira

uint16_t red1[21]          = {1400, 1400, 700, 700, 700, 1400, 700, 2800, 700, 2100, 1400, 700, 700, 700, 700, 1400, 1400, 2800, 1400, 2800, 700};
uint16_t red2[21]          = {1400, 1400, 700, 700, 700, 2100, 700, 2100, 700, 2800, 700, 700, 700, 700, 700, 1400, 1400, 2800, 1400, 2800, 700};
uint16_t green1[21]             = {1400, 1400, 700, 700, 700, 700, 1400, 2800, 700, 1400, 700, 1400, 700, 1400, 700, 1400, 1400, 2800, 1400, 2800, 700};
uint16_t green2[23]             = {700, 700, 700, 700, 1400, 1400, 1400, 2800, 700, 1400, 700, 1400, 700, 1400, 700, 1400, 700, 700, 700, 2100, 1400, 2800, 700};
uint16_t blue1[27]              = {700, 700, 700, 2100, 1400, 1400, 700, 2100, 700, 1400, 700, 700, 700, 1400, 1400, 700, 700, 1400, 700, 700, 700, 700, 700, 700, 700, 2100, 700};
uint16_t blue2[27]              = {700, 700, 700, 2100, 1400, 700, 700, 2800, 700, 1400, 700, 700, 700, 1400, 700, 700, 700, 700, 700, 700, 700, 700, 1400, 1400, 700, 2100, 700};
uint16_t magenta1[23]           = {700, 700, 700, 700, 1400, 1400, 1400, 2800, 700, 2100, 1400, 2100, 700, 700, 700, 700, 1400, 2100, 700, 700, 700, 2100, 700};
uint16_t magenta2[25]           = {700, 700, 700, 2100, 1400, 1400, 1400, 700, 1400, 1400, 700, 700, 700, 1400, 1400, 700, 700, 1400, 700, 2100, 700, 1400, 1400, 700, 700};
uint16_t magenta3[25]           = {700, 700, 700, 2100, 1400, 1400, 1400, 700, 1400, 1400, 700, 700, 700, 1400, 1400, 700, 700, 1400, 700, 2100, 1400, 700, 1400, 700, 700};
uint16_t yellow1[21]            = {1400, 1400, 700, 700, 700, 700, 1400, 2800, 700, 2100, 700, 700, 700, 1400, 700, 1400, 1400, 2800, 1400, 2800, 700};
uint16_t yellow2[21]            = {700, 700, 700, 2100, 1400, 1400, 700, 2100, 700, 1400, 700, 700, 700, 2800, 1400, 1400, 700, 2100, 1400, 2800, 700};
uint16_t pink1[23]              = {700, 700, 700, 2100, 1400, 700, 700, 2800, 700, 1400, 700, 700, 700, 2800, 1400, 1400, 700, 2100, 700, 700, 700, 2100, 700};
uint16_t pink2[21]              = {700, 700, 700, 2100, 1400, 700, 700, 2800, 700, 2100, 1400, 2800, 1400, 1400, 700, 2100, 700, 700, 700, 2100, 700};
uint16_t orange1[23]           = {700, 700, 700, 700, 1400, 1400, 1400, 2800, 700, 2100, 700, 700, 700, 1400, 700, 700, 700, 1400, 700, 2100, 1400, 2800, 700};
uint16_t orange2[21]           = {700, 700, 700, 700, 1400, 1400, 1400, 2800, 700, 2100, 1400, 2100, 700, 700, 700, 700, 1400, 2100, 1400, 2800, 700};
uint16_t white1[23]             = {700, 700, 700, 2100, 1400, 700, 700, 2800, 700, 1400, 700, 700, 700, 2800, 1400, 1400, 700, 2100, 700, 1400, 700, 1400, 700};
uint16_t white2[25]             = {700, 700, 700, 2100, 1400, 1400, 1400, 700, 1400, 1400, 700, 700, 700, 2800, 1400, 1400, 700, 700, 1400, 700, 700, 700, 1400, 700, 700};
uint16_t turquoise1[23]          = {700, 700, 700, 700, 1400, 1400, 1400, 2800, 700, 1400, 700, 700, 1400, 1400, 700, 1400, 1400, 2800, 700, 700, 700, 2100, 700};
uint16_t turquoise2[27]          = {700, 700, 700, 2100, 1400, 1400, 1400, 700, 1400, 1400, 700, 700, 700, 2100, 700, 700, 700, 1400, 700, 700, 700, 700, 700, 1400, 1400, 700, 700};
uint16_t turquoise3[29]          = {700, 700, 700, 2100, 1400, 700, 700, 700, 700, 700, 1400, 1400, 700, 700, 700, 2100, 700, 700, 700, 1400, 700, 700, 700, 700, 1400, 700, 1400, 700, 700};
uint16_t off[29]                = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint16_t* dataa;
int deviceTypea = 0; 


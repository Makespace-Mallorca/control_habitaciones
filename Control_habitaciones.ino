unsigned long TiempoActual = 0;         // Estas dos variables sirven para contar
unsigned long TiempoReferencia = 0;     // el tiempo sin usar delay

const int PinIR = 2;
const int PinPuerta = 3;
const int PinLuz = 4;
const int PinAACC = 5;
const int PinVIP = 6;
const int PinRecepcion = 7;   //Previsión para la siguiente versión, de momento no se envía nada

int IR = HIGH;
int Puerta = HIGH;
int VIP = HIGH;
int EoS = 1;               // ¿Entras o sales? si esta a 1 es que no sabes si entran o salen si está a 0 es que ya lo sabes

void setup()
{
  pinMode (PinIR, INPUT);
  pinMode (PinPuerta, INPUT);
  pinMode (PinVIP, INPUT);
  pinMode (PinLuz, OUTPUT);
  pinMode (PinAACC, OUTPUT);
  pinMode (PinRecepcion, OUTPUT);

  digitalWrite (PinLuz, LOW);   //los relés están activos con 0 y apagados con 1
  digitalWrite (PinAACC, HIGH);
}

void loop()
{
  VIP = digitalRead (PinVIP);   //Si el modo VIP está activado
  if (VIP == LOW)
    {
      digitalWrite (PinLuz, LOW);    //Los reles se activan con LOW y se desactivan con HIGH
      digitalWrite (PinAACC, LOW);
      IR = digitalRead (PinIR);
      Puerta = digitalRead (PinPuerta);
      if (Puerta == LOW)  //LOW = puerta abierta
        {
          EoS = 1;
          digitalWrite (PinRecepcion, HIGH);    //Avisa que la habitación está ocupada
          TiempoReferencia = millis();          //Guarda en la variable cuanto tiempo hace que el arduino está encendido
        }
      if (IR == LOW)          //LOW = deteción por infrarojos
        {
          EoS = 0;
        }
      if (EoS = 1)
        {
          TiempoActual = millis();                //Guarda en la variable cuanto tiempo hace que el arduino está encendido
          if (TiempoActual < TiempoReferencia)    // Comprueba si se producido overflow
            {
              TiempoReferencia = millis();        // Si se a producido oveflow refresca el tiempo de referencia
              delay (1000);
            }
           if ((TiempoActual - TiempoReferencia) > 120000)  // Si han pasado 2 minutos
            {
              digitalWrite (PinRecepcion, LOW);     //Avisa que la habitación estám vacía
            }
        }
    }
  else
    {
      IR = digitalRead (PinIR);
      Puerta = digitalRead (PinPuerta);
      if (Puerta == LOW)      // El imán de la puerta está HIGH con la puerta cerrada y LOW con la puerta abierta
        {
         digitalWrite (PinLuz, LOW);  
         digitalWrite (PinAACC, LOW);
         digitalWrite (PinRecepcion, HIGH);
         TiempoReferencia = millis();
         EoS = 1;
        }
      if (IR == LOW)  // Los infrarrojos estan en HIGH mientras no hay movimiento y el LOW cuando detectan movimiento 
        {
          digitalWrite (PinLuz, LOW);
          digitalWrite (PinAACC, LOW);
          EoS = 0;
        }
      if (EoS == 1)
        {
          TiempoActual = millis();
          if (TiempoActual < TiempoReferencia)                // Comprueba si se producido overflow
            {
              TiempoReferencia = millis();
              delay (1000); 
            }
          if ((TiempoActual - TiempoReferencia) > 120000)     // Si la habitación lleva vacía 2 minutos
            {
              digitalWrite (PinLuz, HIGH);
              digitalWrite (PinRecepcion, LOW);
            }
          if ((TiempoActual - TiempoReferencia) > 300000)     // Si la habitación llava vacía 5 minutos
            {
              digitalWrite (PinAACC, HIGH);
            }
        }
    }
 
}

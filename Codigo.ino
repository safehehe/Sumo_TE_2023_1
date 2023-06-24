//Configuracion pines
  #define M_D_F 3 //Motor Derecha Forward
  #define M_I_F 5//Motor Izquierda Forward
  #define M_D_B 4//Motor Derecha Backward
  #define M_I_B 6//Motor Izquierda Backward

  #define E_D 10
  #define E_I 11
  
  #define IR_1 A1//InfraRed Sensor 1 (Analog) Frente
  #define IR_2 A2//InfraRed Sensor 2 (Analog) Atras Izquierda
  //#define IR_3 A3 InfraRed Sensor 3 (Analog) Atras Derecha
  
  #define US_TRG 7//UltraSonic Sensor Trigger
  #define US_ECH 8//UltraSonic Sensor Echo
  //Enable PWM, control de velocidad
  
  #define S_D 25//Smaller Distance Limit
  #define M_D 30//Medium Distance Limit
  #define L_D 35//Largest Distance Limit
  char US_DIST_C = 0; //Codigo de Distancia
  char IR_VAL = 0;
  int C_G = 100; //Contador de giro;
void setup() {
  pinMode(M_D_F, OUTPUT);
  pinMode(M_I_F, OUTPUT);
  pinMode(M_D_B, OUTPUT);
  pinMode(M_I_B, OUTPUT);
  pinMode(US_TRG, OUTPUT);
  pinMode(US_ECH, INPUT);
  pinMode(E_D,OUTPUT);
  pinMode(E_I,OUTPUT);
  analogWrite(E_D,125);
  analogWrite(E_I,165);
  Serial.begin(9600);
  avanzar();
  delay(400);
  paro();
}
  
void loop() {
  Serial.println("Funciona");
  analogWrite(E_D,125);
  analogWrite(E_I,165);
  US_DIST_C = US_lecture();
  IR_VAL = IR_lecture();
  if(IR_VAL == 1){ 
  if(US_DIST_C==1){
    avanzar();
    while(US_DIST_C==1 && IR_VAL == 1){
      Serial.println("Ataque");
      US_DIST_C = US_lecture();
      IR_VAL = IR_lecture();
      }
    paro();
   }
  else{
    giro_izquierda();
    US_DIST_C = US_lecture();
    IR_VAL = IR_lecture();
    while(US_DIST_C!=1 && C_G!=0 && IR_VAL == 1){
      Serial.println("Giro Derecha");
      US_DIST_C = US_lecture();
      IR_VAL = IR_lecture();
      C_G--;
      }
    paro();
    C_G = 100;
    giro_derecha();
    IR_VAL = IR_lecture();
    while(US_DIST_C!=1 && C_G!=0 && IR_VAL == 1){
      Serial.println("Giro Izquierda");
      US_DIST_C = US_lecture();
      IR_VAL = IR_lecture();
      C_G--;
      }
    paro();
    C_G = 100;
    /*
    Empieza a girar, mientras NO encuentre algo 
    y un contador no supere algo continua girando, en caso contrario, para el giro y ataca

    Lo mismo pero al revezs
    */
    }
  }
  if(IR_VAL==2){
    reversa();
    Serial.println("Frente fuera");
    delay(1000);
    paro();
    }
  if(IR_VAL==3){
    avanzar();
    Serial.println("Izquierda fuera");
    delay(1000);
    paro();
    }  
}

void on_motor_pin(char pin){
  //Enciende un motor y apaga su direccion contraria a la dada
    switch(pin){
      case M_D_F:
        digitalWrite(M_D_B,LOW);
        digitalWrite(pin,HIGH);
        break;
      case M_D_B:
        digitalWrite(M_D_F,LOW);
        digitalWrite(pin,HIGH);
        break;
      case M_I_F:
        digitalWrite(M_I_B,LOW);
        digitalWrite(pin,HIGH);
        break;
      case M_I_B:
        digitalWrite(M_I_F,LOW);
        digitalWrite(pin,HIGH);
        break;
      }
  }

void off_motor_pin(char pin){
    if(pin == M_D_F || pin == M_D_B){
      digitalWrite(M_D_F,LOW);
      digitalWrite(M_D_B,LOW);
      }
    if(pin == M_I_F || pin == M_I_B){
      digitalWrite(M_I_F,LOW);
      digitalWrite(M_I_B,LOW);
      }
  }

void avanzar (){
  analogWrite(E_D,200);
  analogWrite(E_I,225);
  //El robot avanza por cierto tiempo
  on_motor_pin(M_D_F);
  on_motor_pin(M_I_F);
  }

void reversa (){
  //El robot retrocede por cierto tiempo
  on_motor_pin(M_D_B);
  on_motor_pin(M_I_B);
  }

void giro_derecha (){
  //El robot gira hacia la derecha por cierto tiempo
  on_motor_pin(M_D_B);
  on_motor_pin(M_I_F);
  }

void giro_izquierda (){
  //El robot gira hacia la izquierda por cierto tiempo
  on_motor_pin(M_D_F);
  on_motor_pin(M_I_B);
  }

void paro(){
  off_motor_pin(M_D_F);
  off_motor_pin(M_I_F);
  }
char IR_lecture(){
  int lecture1 = analogRead(IR_1);
  int lecture2 = analogRead(IR_2);
  Serial.print("IR1 ") ;
  Serial.println(lecture1);
  if(lecture1>400 && lecture2>400){
    return 4;
    }
  if(lecture1<400 && lecture2<400){
    return 1; //En la pista
    }
  if(lecture1>400){
    return 2; //Frente fuera de la pista
    }
  if(lecture2>400){
    return 3; //Izquierda fuera
    }
  }

char US_lecture(){
  int t = 0;
  int d = 0;
  
  digitalWrite(US_TRG, HIGH);
  delayMicroseconds(10);//Enviamos un pulso de 10us
  digitalWrite(US_TRG, LOW);
  
  t = pulseIn(US_ECH, HIGH);//Obtenemos el ancho del pulso
  d = t/59;//Escalamos el tiempo a una distancia en cm
  
  Serial.print("Distancia: ");
  Serial.print(d);//Enviamos serialmente el valor de la distancia
  Serial.print("cm");
  Serial.println();
  delay(75);
  if(d<=S_D){
    return 1;
    }
  if(d<=M_D){
    return 2;
    }
  if(d<=L_D){
    return 3;
    }
  }

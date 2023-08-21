boolean subida1,subida2;
int entrada1=2,entrada2=3;  // Pin de la entrada 1 y 2
const int salida = 12;   // Pin de la salida
int contador = 0;        // Contador para contar los pulsos

void setup() {
  Serial.begin(9600);
  Serial.print("contador");
  pinMode(entrada1, INPUT);
  pinMode(entrada2, INPUT);
  pinMode(salida, OUTPUT);
  digitalWrite(salida, LOW);  // Inicialmente la salida está en estado bajo
}

void loop() {
  // Comprobar si se ha recibido un pulso en la entrada 1
  if (digitalRead(entrada1)==1&& subida1==0) {
    delay(1000);
    contador++;  // Incrementar el contador
    Serial.println(contador);
  }
  subida1 = digitalRead(entrada1);

    // Comprobar si el contador ha alcanzado el valor de 5
    if (contador == 5) {
      digitalWrite(salida, HIGH);  // Enviar señal por la salida
      contador = 0;
      digitalWrite(salida, LOW);
    }
    

  // Comprobar si se ha recibido un pulso en la entrada 2
  if (digitalRead(entrada2)==1&& subida2==0) {
    delay(1000);
    contador = 0;               // Reiniciar el contador a 0
    Serial.println("Contador 0");
    digitalWrite(salida, LOW);  // Apagar la salida
  }
  subida2 = digitalRead(entrada2);

}

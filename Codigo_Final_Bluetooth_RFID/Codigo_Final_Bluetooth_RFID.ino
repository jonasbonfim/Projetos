
#include <SPI.h>                        
#include <RFID.h>                          
#include <Servo.h> 
#include <Ethernet.h>
boolean iniciaValidacao = true;
const int sensorPress = 2;                 
const int sensorLed = 3;                   
const int indAberta = 4;                  
const int indFechada = 5;  
String tag = "";
int buttonState = 0; 
Servo meuservo;                            
int pos = 0; 
/*-------------------------------------------------------*/
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char server[] = "192.168.1.178"; // IP Estático do Notebook
IPAddress ip(192,168,1,177);    //  IP Estático do Shield Ethernet
EthernetClient client;
/*-------------------------------------------------------*/
#define SS_PIN 8 
#define RST_PIN 9                         
RFID rfid(SS_PIN, RST_PIN);                
int serNum0;
int serNum1;
int serNum2;
int serNum3;
int serNum4;
void setup()
{
 
 pinMode(sensorPress, INPUT);              
 pinMode(sensorLed, OUTPUT);               
 pinMode(indAberta, OUTPUT);               
 pinMode(indFechada, OUTPUT);              
Serial.begin(9600);                        
SPI.begin();                               
rfid.init();                       
meuservo.attach(7);               
meuservo.write(0);               
 delay(15);                         
}
void loop(){
 buttonState = digitalRead(sensorPress);
 char caracter = Serial.read();
    if (rfid.isCard()) {
        if (rfid.readCardSerial()) {
                 Serial.println(" ");
                Serial.println("Cartao Identificado");
                serNum0 = rfid.serNum[0];
                serNum1 = rfid.serNum[1];
                serNum2 = rfid.serNum[2];
                serNum3 = rfid.serNum[3];
                serNum4 = rfid.serNum[4];
                
                tag +=serNum0;
                tag +=serNum1;
                tag +=serNum2;
                tag +=serNum3;
                tag +=serNum4;
                
                Serial.println(tag);
 if(iniciaValidacao) 
    {
      Ethernet.begin(mac, ip);
      delay(2000);
      Serial.println("Inicia avaliacao..");       
      Serial.print("My IP address: ");
      Serial.println(Ethernet.localIP());   
      Serial.println("Processando..");   

     } 
      if (client.connect(server, 80))
      {
        Serial.println("Conectado");
        
      
        client.println("GET /rfid/consulta.php?tag="+tag);
        client.println("Host: 192.168.1.178");
        client.println("Connection: close");
        client.println();         
        Serial.println("enviou os dados para consulta");
        delay(2000);
        
             if (client.available()) 
        {      
          char c = client.read();
          Serial.print("Resultado da Consulta = ");
          Serial.print(c);
          
           if(c=='1')
          {
               if (pos==0){  
                pos=180;     
                Serial.println("Porta Aberta");
                digitalWrite (indAberta, HIGH);  
                delay(500);                       
                digitalWrite (indAberta, LOW);   
                delay(500);  
                meuservo.write(pos);                 
                } // abertura da porta  
              
          }// verificando existencia no banco de dados 
        } // consultando exxistencia no banco de dados
          }// conexão com o servidor 
          else 
      {        
        Serial.println("Falha de Conexão");
       
        Serial.print("Conection Failed");
        delay(2000);
      }  
        }// validação
                     
  } //leitura geral do cartão
             else      
                if ((pos==180) &&(buttonState == HIGH)) {     
                  pos=0; 
                
                 digitalWrite(sensorLed, HIGH);                 
                 digitalWrite(indFechada, HIGH);                 
                 delay(1000);                                   
                 digitalWrite(sensorLed, LOW);                  
                 digitalWrite(indFechada, LOW);                 
                 meuservo.write(pos);
                 Serial.println("Porta Fechada");
  } // leitura cartão magnetico.

  else   
           if ((pos==0) && (caracter == 'a')){  
           pos=180;      
           Serial.println("Porta Aberta");
           digitalWrite(sensorLed, HIGH);                 
           digitalWrite(indFechada, HIGH);                 
           delay(1000);                                   
           digitalWrite(sensorLed, LOW);                  
           digitalWrite(indFechada, LOW);                 
           meuservo.write(pos); 
    } // abrindo porta via bluetooth 
else    
 if((pos==180) &&(caracter == 'b')) {     
pos=0; 
digitalWrite(sensorLed, HIGH);                 
digitalWrite(indFechada, HIGH);                 
 delay(1000);                                   
digitalWrite(sensorLed, LOW);                  
digitalWrite(indFechada, LOW);                 
meuservo.write(pos);
} // fechando porta via bluetooth      

 } // fechamento do loop.


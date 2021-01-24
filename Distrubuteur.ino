#include <FastIO.h>
#include <I2CIO.h>
#include <LiquidCrystal_I2C.h>
//bibliothèque de l'ecran LCD
#include <Wire.h>

//bibliothèque de la carte RFID
#include <SPI.h>
#include <MFRC522.h>
#include <Keypad.h>

//on défini nos pin
#define SS_PIN 53
#define RST_PIN 5

// definition des pin du RC522
MFRC522 mfrc522(SS_PIN, RST_PIN);
char st[20];
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); //
const byte rows = 2;
const byte cols = 3;

char keys[rows][cols] = {
							{'1','2','3'},
							{'4','5','6'}
						};

byte rowPins[rows] = {36, 37};
byte colPins[cols] = {38, 40, 41};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);

int motorPin1 = 24; // the first motor's pin
int motorPin2 = 25; // the second motor's pin
int motorPin3 = 23; // the third motor's pin
int motorPin4 = 22;

int buttonPressed;
int SensorStatus;

int sensor= 44;
int sensor2=42;

int sensorstatus=0;
int sensor2status=0;

void setup() 
{
	lcd.begin(16, 2); // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display
	pinMode(motorPin1, OUTPUT);
	pinMode(motorPin2, OUTPUT);
	pinMode(motorPin3, OUTPUT);
	pinMode(motorPin4, OUTPUT);
	pinMode(sensor,INPUT);
	pinMode(sensor2,INPUT);
	Serial.begin(9600);
	SPI.begin();
	mfrc522.PCD_Init();
	// Message initial (le message afiché au démarrage dans le moniteur serie de notre programme arduino
	//Serial.println("Test acces via TAG RFID");
	//Serial.println();
}

void loop() 
{
	lcd.clear();
	lcd.setCursor(0,0);

	lcd.print("passez votre ");
	lcd.setCursor(5, 1);
	lcd.print("carte svp!");
	
	while (true) 
	{
		if ( ! mfrc522.PICC_IsNewCardPresent())
		{
		return;
		}
		if ( ! mfrc522.PICC_ReadCardSerial())
		{
		return;
		}
		//Serial.print("UID de tag :");
		String tag= "";
		byte caractere;
		for (byte i = 0; i < mfrc522.uid.size; i++)
		{
		Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
		Serial.print(mfrc522.uid.uidByte[i], HEX);
		tag.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
		tag.concat(String(mfrc522.uid.uidByte[i], HEX));
		}
		//Serial.println();
		//Serial.print("Message : ");
		tag.toUpperCase();

		// ici on va vérifier l'autorisation


		if (tag.substring(1) == "C0 C4 63 A7") // le numero de tag est visible lorsqu on presente la carte ou le badge via le moniteur serie
		//il suffit d'insérer ci-dessus le tag que l'on souhaite authoriser ici
		{ // si la bonne carte est passé, on sort de la boucle while
		break;
		}
	}
	
	delay(10);
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("choisissez votre");
	lcd.setCursor(4,1);
	lcd.print("produit!");
	while (true) 
	{
		char key = keypad.getKey();
		
		if (key == '1') 
		{
			buttonPressed = 1;
			break;
		}
		if (key =='2') 
		{
			buttonPressed = 2;
			break;
		}
		if (key == '3')
		{
			buttonPressed = 3;
			break;
		}
		if (key == '4') 
		{
			buttonPressed = 4;
			break;
		}
	}
	
	// Print "Delivering..."
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("attendez svp");
	switch (buttonPressed) 
	{
		case 1: digitalWrite(motorPin1,HIGH);
		break;
		case 2: digitalWrite(motorPin2,HIGH);
		break;
		case 3: digitalWrite(motorPin3,HIGH);
		break;
		case 4: digitalWrite(motorPin4,HIGH);
		break;
	}
	while (true) 
	{
		sensorstatus=digitalRead(sensor);
		sensor2status = digitalRead(sensor2);
		if(sensorstatus == 1 || sensor2status == 1)

		{
		SensorStatus = 1;
		break;
		}

	}
	
	switch(SensorStatus)
	{
		case 1: digitalWrite(motorPin1,LOW);
		digitalWrite(motorPin2,LOW);
		digitalWrite(motorPin3,LOW);
		digitalWrite(motorPin4,LOW);
		break;
	}
	
	delay(50);
	lcd.clear(); // Clears the display
	lcd.setCursor(0, 0);
	lcd.print("Produit delivrer!"); // Prints on the LCD
	delay(2000); 
}

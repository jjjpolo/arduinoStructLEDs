struct led
{
  led* prev =  NULL;
  int pin = 0;
  int state = 0;
  led* next = NULL;
};

struct led *mainLed;

void toggleState(struct led * miLed)
{
  if (validPin(miLed->pin))
  {
    if (miLed->state)
    {
      Serial.print("[INFO] - [toogleState] Set state to HIGH of pin: ");
      Serial.println (miLed->pin);
      miLed->state = 0;
      digitalWrite(miLed->pin, miLed->state);
    }
    else
    {
      Serial.print("[INFO] - [toogleState] Set state to LOW of pin: ");
      Serial.println (miLed->pin);
      miLed->state = 1;
      digitalWrite(miLed->pin, miLed->state);
    }
  }
  else
  {
    Serial.print("[ERROR] - [toogleState] It is not possible to change the state of pin: ");
    Serial.println (miLed->pin);
  }
}

bool validPin(int pinNumber)
{
  if (pinNumber > 0 && pinNumber <= 13)
    return true;
  return false;
}

void addLed(struct led * &myLed, int newPin, int initialState) //we need to use struct led *& cause evetually we are modifying the entire pointer (creating a new structure of led out of a pointer)
{
  if (validPin(newPin))
  {
    Serial.print("[INFO] - [addLed] Creating an struct for pin: ");
    Serial.println (newPin);
    if (myLed == NULL)
    {
      Serial.println("[INFO] - [addLed] This is the 1ST element added ");
      myLed = (struct led *) malloc(sizeof(led));
      myLed->pin = newPin;
      pinMode(newPin, OUTPUT);
      myLed->state = initialState;
      digitalWrite(newPin, initialState);
      myLed->next = myLed;
    }
    else
    {
      Serial.println("[INFO] - [addLed] Adding node to the existen structure ");
      struct led * newLed = (struct led *) malloc(sizeof(led));
      newLed->pin = newPin;
      pinMode(newPin, OUTPUT);
      newLed->state = initialState;
      digitalWrite(newPin, initialState);
      newLed->next = myLed->next;
      myLed->next = newLed;
    }
  }
  else
  {
    Serial.print("[ERROR] - [addLed] Error creating an struct for pin: ");
    Serial.println (newPin);
  }
}

void deleteLed(struct led * &myLed, int pin) //we need to use struct led *& cause eventually we are modifying the entire pointer (setting it as NULL)
{
  int firstElement = myLed->pin; //it is a circular struct so we need to know when we walked all over the structure to avoid an infinite loop
  bool elementFound = false;
  do
  {
    if (myLed->next->pin == pin)
    {
      if (myLed->next == myLed)//it means that we found the pin that we are looking for AND that the structure has only one element
      {
        Serial.println("[INFO] - [deleteLed] - This is the last element of the structure ");
        myLed = NULL;
        elementFound = true;
        break;
      }
      else
      {
        myLed->next = myLed->next->next; //we are only deleteing the connection with the node that we want to delete,it is not actually a deleting action
        elementFound = true;
        break;
      }
    }
    else
      myLed = myLed->next;
  } while (myLed->pin != firstElement);

  if (!elementFound)
  {
    Serial.print("[ERROR] - [deleteLed] - It was not possible to find pin: ");
    Serial.println(pin);
  }
  else
  {
    digitalWrite(pin, LOW);
    Serial.print("[INFO] - [deleteLed] - Deleting node for pin: ");
    Serial.println(pin);
  }

}

void setup()
{
  Serial.begin(9600);
  Serial.println("[INFO] - [setup] - Serial.begin(9600)");
  /*
    mainLed->pin = 0;
    mainLed->state = 0;
    mainLed->next = NULL;
  */
}

void serialEvent()
{
  // ej. add011 will add led struc pointing to pin 1 stating it on HIGH || set010 set pin 1 to low || del01X will delete struct that points to pin 01
  String command = "";
  int pinNumber = 0;
  int stateParam = 0;

  Serial.print("[INFO] - [serialInterrupt] - Echo: ");
  int counter = 0;
  char c = Serial.read();
  Serial.print(c);
  do
  {
    if (counter < 3)
      command = command + c;
    else if (counter < 5)
    {
      if (counter == 3)
        pinNumber = (c - 0x30) * 10;
      else
        pinNumber = pinNumber + (c - 0x30);
    }
    else
    {
      stateParam = c - 0x30;
    }
    counter++;
    delay(1);
    while (Serial.available() < 0) {} //wait for the next incoming char input
    c = Serial.read();
    Serial.print(c);
  } while (c != '.');
  Serial.println(" ");
  Serial.print("[INFO] - [serialInterrupt] - Command: ");
  Serial.println(command);
  Serial.print("[INFO] - [serialInterrupt] - pinNumber: ");
  Serial.println(pinNumber);
  Serial.print("[INFO] - [serialInterrupt] - Set to state: ");
  Serial.println(stateParam);

  if (command == "add")
    addLed(mainLed, pinNumber, stateParam);
  if (command == "del")
    deleteLed(mainLed, pinNumber);
}


void loop()
{
  //= (struct led *) malloc(sizeof(led));
  if (mainLed != NULL)
  {
    if (validPin(mainLed->pin))
      mainLed = mainLed->next;
    Serial.print("[INFO] - [LOOP] - Current pin: ");
    Serial.println(mainLed -> pin);
    Serial.print("[INFO] - [LOOP] - Pin state: ");
    Serial.println(mainLed -> state);
    toggleState(mainLed);
    delay(1);
  }
  else
  {
    Serial.println("[ERROR] - [LOOP] - The structure is void! ");
    delay(1);
  }
}

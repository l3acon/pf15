/*
   GOAL: 
     attempt to figure out switch matrix

   assume 8 channels x 12 notes (channels are sort-of like octaves)
   thus, we likely have 8 channels selector pins
   and 12 pins to read "notes" from.
   and in theory there are 10 pins that are un-used

   30-pin connector: (to be indexed from arduino digital pin 22)

                     ______
   .----------------|------|-----------------.
   | 28 26 24 22 20 18 16 14 12 10 8 6 4 2 0 |
   | 29 27 25 23 21 19 17 15 13 11 9 7 5 3 1 |
   '-----------------------------------------'

   channel select: 16, 14, 12, 10, 8, 6, 4, 2 
         and also: 15, 13, 11, 9,  7, 5, 3, 1
   note pins: 28, 29, 26, 27, 24, 25, 22, 23, 20, 21, 18, 19

*/


#define DEBUG 1

#define NOTE_PRE_READ_DELAY 10
#define NOTE_POST_READ_DELAY 50

#define N_DIGITAL_PINS  30

static size_t n_channels = 8;
static size_t channel_select[]     = {38, 36, 34, 32, 30, 28, 26, 24};
static size_t channel_select_alt[] = {37, 35, 33, 31, 29, 27, 25, 23};

static size_t n_notes = 12;
static size_t note_select[] = {50, 51, 48, 49, 46, 47, 44, 45, 42, 43, 40, 41};

char ks[]       = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
char ks_prev[]  = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

void print_pin(size_t i) {
    if (ks[i] == 0) {
      Serial.print("p");
      Serial.print(ks[i]);
      Serial.print(" L\t");
    } else {
      Serial.print("p");
      Serial.print(ks[i]);
      Serial.print(" H\t");
    }
}

/* 
   setup
   according to MIT-MIT, set note pinMode -> OUTPUT, HIGH
   according to MIT-MIT, set channels pinMode -> OUPUT, LOW
*/
void setup_pins() {
  for(size_t i = 0; i < n_notes; i++) {
    pinMode(note_select[i], OUTPUT);
    digitalWrite(note_select[i], HIGH);
  }
  for(size_t i = 0; i < n_channels; i++) {
    pinMode(channel_select[i], OUTPUT);
    digitalWrite(channel_select[i], LOW);
    digitalWrite(channel_select_alt[i], LOW);
  }
}

void setup() {
  #ifdef DEBUG
  Serial.begin(115200);
  #endif
  setup_pins();
}

/* 
   for each channel
     write to selected column -> LOW
     for each note in channel
       set pinMode for note INPUT_PULLUP
       delay for 10 ms 
       read note
       set pinMode for note OUTPUT
       delay 500 ms
*/

void update_keys() {
  // key index
  size_t k = 0;
  // channel select
  for( size_t i = 0; i < n_channels; i++) {
    digitalWrite(channel_select[i], LOW);
    for( size_t j=0; j < n_notes; j++) {
      pinMode(note_select[j], INPUT_PULLUP);
      delayMicroseconds(NOTE_PRE_READ_DELAY);
      
      ks[k] = digitalRead(note_select[j]);

      pinMode(note_select[j], OUTPUT);
      digitalWrite(note_select[j], LOW);
      k++;
      delayMicroseconds(NOTE_POST_READ_DELAY);
    }
  }
}

void loop() {
  update_keys();
}

// ==== EchoReverb Minimal (Teensy 4.1 + SGTL5000 Audio Shield) ====
// In/Out: stereo line-in/out on the Audio Shield
// Controls (when wired later): A0=Reverb Mix, A2=Echo Mix, PIN2=Tap, PIN4=Division

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>

// ---- Audio graph ----
AudioInputI2S            i2sIn;
AudioEffectDelay         dL, dR;
AudioEffectFreeverb      rvb;
AudioMixer4              wetL, wetR;
AudioMixer4              outL, outR;
AudioOutputI2S           i2sOut;
AudioControlSGTL5000     codec;

AudioConnection cords[] = {
  AudioConnection(i2sIn, 0, rvb, 0),
  AudioConnection(i2sIn, 1, rvb, 1),
  AudioConnection(i2sIn, 0, dL, 0),
  AudioConnection(i2sIn, 1, dR, 0),
  AudioConnection(rvb, 0, wetL, 0),
  AudioConnection(rvb, 1, wetR, 0),
  AudioConnection(dL,  0, wetL, 1),
  AudioConnection(dR,  0, wetR, 1),
  AudioConnection(i2sIn, 0, outL, 0), // dry L
  AudioConnection(i2sIn, 1, outR, 0), // dry R
  AudioConnection(wetL, 0, outL, 1),  // wet L
  AudioConnection(wetR, 0, outR, 1),  // wet R
  AudioConnection(outL, 0, i2sOut, 0),
  AudioConnection(outR, 0, i2sOut, 1),
};

// ---- Pins (for later) ----
const int PIN_TAP   = 2;
const int PIN_DIV   = 4;
const int A_REV_MIX = A0;
const int A_ECHO_MIX= A2;

// ---- State ----
elapsedMillis sinceTap;
uint32_t lastTapMs = 500, prevTap=0;   // default ~120 BPM
int divIx=0; const float divs[4] = {1.0, 0.75, 0.5, 0.25};

float revMix=0, echoMix=0;
static inline float smooth(float x, float target, float a=0.1f){ return x + a*(target-x); }
static inline bool fell(int pin){ static uint8_t s[40]; uint8_t o=s[pin]; s[pin]=digitalRead(pin); return o==HIGH && s[pin]==LOW; }

void setup() {
  // Tools → Board: Teensy 4.1, Tools → USB Type: Serial + Audio
  Serial.begin(115200);
  pinMode(PIN_TAP, INPUT_PULLUP);
  pinMode(PIN_DIV, INPUT_PULLUP);

  AudioMemory(120);
  codec.enable();
  codec.inputSelect(AUDIO_INPUT_LINEIN);
  codec.lineInLevel(8);         // tweak later if clipping
  codec.volume(0.8);

  // default delay 500 ms (updated by tap/div)
  dL.delay(0, 500);
  dR.delay(0, 500);

  // default gains
  wetL.gain(0, 0.5); wetL.gain(1, 0.5);
  wetR.gain(0, 0.5); wetR.gain(1, 0.5);
  outL.gain(0, 0.7); outL.gain(1, 0.3);
  outR.gain(0, 0.7); outR.gain(1, 0.3);

  // reverb character
  rvb.roomsize(0.35f);
  rvb.damping(0.4f);

  Serial.println("EchoReverb Minimal: compiled & ready.");
}

void loop() {
  // Simulate knobs if hardware not present (comment out when wiring pots)
  float simRev  = 0.40f; // 0..1
  float simEcho = 0.30f; // 0..1
  revMix  = smooth(revMix,  simRev);
  echoMix = smooth(echoMix, simEcho);

  // Map to parameters
  rvb.roomsize(revMix);         // 0..1
  float totalWet = min(0.9f, revMix + echoMix);
  outL.gain(0, 1.0f - totalWet); outR.gain(0, 1.0f - totalWet);

  // Button handling (works when wired)
  if (fell(PIN_DIV)) divIx = (divIx+1) & 3;
  if (fell(PIN_TAP)) {
    uint32_t now = millis(); uint32_t d = now - prevTap;
    if (d > 120 && d < 2000) lastTapMs = d; // 30–500 BPM
    prevTap = now;
  }

  // Update delay time from tap/division
  float dMs = lastTapMs * divs[divIx];
  dL.delay(0, dMs);
  dR.delay(0, dMs);

  // Debug (visible over USB Serial)
  static uint32_t t=0; if (millis()-t>500){ t=millis();
    Serial.printf("revMix=%.2f echoMix=%.2f  delay=%.0f ms (div %.2f)\n",
      revMix, echoMix, dMs, divs[divIx]);
  }
}

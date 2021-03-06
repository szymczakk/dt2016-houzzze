#include <iostream>
#include <stdexcept>
#include <wiringPi.h>

#include "community-sdk/include/Iedk.h"

void connect(bool emoComposer, char* emoComposerIp);
void handleFacialExpressions(EmoStateHandle eState);

void turnFanOn();
void turnFanOff();
void turnLedOn();
void turnLedOff();
void turnLedRedOn();
void turnLedRedOff();
bool anyMove();

void setupPins();

const int FAN_PIN = 5;
int FAN_STATE = 0;
const int LED_PIN = 4;
int LED_STATE = 0;
const int MOTIONDETECTOR_PIN = 1;
const int LED_RED_PIN = 6;
int LED_RED_STATE = 0;

int main(int argc, char **argv)
{
	wiringPiSetup();
	setupPins();
	EmoEngineEventHandle eEvent = IEE_EmoEngineEventCreate();
	EmoStateHandle eState = IEE_EmoStateCreate();

	char* composerIp = "127.0.0.1";
	bool useComposer = false;
	

	if(argc > 1){
		composerIp = argv[1];
		useComposer = true;
	}

	try {
		connect(useComposer, composerIp);
		while (true)
		{
			if(anyMove()){
				turnLedRedOn();
			}
			else{
				turnLedRedOff();
			}
			int state = IEE_EngineGetNextEvent(eEvent);
			if (state == EDK_OK) {
				IEE_Event_t eventType = IEE_EmoEngineEventGetType(eEvent);

				switch (eventType) {
				case IEE_EmoStateUpdated:
				{
					IEE_EmoEngineEventGetEmoState(eEvent, eState);
					handleFacialExpressions(eState);
				}
				break;
				}
			}
		}
	}
	catch (std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
		std::cout << "Press 'Enter' to exit..." << std::endl;
	}

	IEE_EngineDisconnect();
	IEE_EmoStateFree(eState);
	IEE_EmoEngineEventFree(eEvent);
	return 0;
}

void connect(bool emoComposer, char* emoComposerIp) {
	if (!emoComposer) {
		if (IEE_EngineConnect() != EDK_OK) {
			throw std::runtime_error("Emotiv Driver start up field");
		}
		else {
			std::cout << "Emotiv Driver started" << std::endl;
		}
	}
	else {
		if (IEE_EngineRemoteConnect(emoComposerIp, 1726) != EDK_OK) {
			throw std::runtime_error("Emotiv Driver start up field");
		}
		else {
			std::cout << "Emotiv Driver started" << std::endl;
		}
	}
}

void handleFacialExpressions(EmoStateHandle eState) {

	IEE_FacialExpressionAlgo_t upperFaceType = IS_FacialExpressionGetUpperFaceAction(eState);
	IEE_FacialExpressionAlgo_t lowerFaceType = IS_FacialExpressionGetLowerFaceAction(eState);

	float upperFaceAmp = IS_FacialExpressionGetUpperFaceActionPower(eState);
	float lowerFaceAmp = IS_FacialExpressionGetLowerFaceActionPower(eState);

	if (IS_FacialExpressionIsBlink(eState)) {
		std::cout << "Blink" << std::endl;
		if(LED_STATE == 0){
			turnLedOn();
		}
		else{
			turnLedOff();
		}
	}
	if (IS_FacialExpressionIsLeftWink(eState)) {
		std::cout << "Left wink" << std::endl;
		if(FAN_STATE == 0){
			turnFanOn();
		}
		else{
			turnFanOff();
		}
	}
	if (IS_FacialExpressionIsRightWink(eState)) {
		std::cout << "Right wink" << std::endl;
	}
	if(IS_FacialExpressionIsLookingLeft(eState)){
		std::cout << "Look left" << std::endl;
	}

	if (upperFaceAmp > 0.0) {
		switch(upperFaceType) {
			case FE_SURPRISE:
			{
				std::cout << "Suprise " << upperFaceAmp << std::endl;
				break;
			}
			case FE_FROWN:
			{
				std::cout << "Frown up " << upperFaceAmp << std::endl;
				break;
			}
		}
	}

	if (lowerFaceAmp > 0.0) {
		switch (lowerFaceType) {
			case FE_CLENCH: 
			{
				std::cout << "Clench " << lowerFaceAmp << std::endl;
				break;
			}
			case FE_SMILE:
			{
				std::cout << "Smile " << lowerFaceAmp << std::endl;
				break;
			}
		}
	}
}

void setupPins(){
	pinMode(LED_PIN, OUTPUT);
	pinMode(LED_RED_PIN, OUTPUT);
	pinMode(MOTIONDETECTOR_PIN, INPUT);
	pinMode(FAN_PIN, OUTPUT);
}

void turnFanOn(){
	digitalWrite(FAN_PIN, LOW);
	FAN_STATE = 1;
}
void turnFanOff(){
	digitalWrite(FAN_PIN, HIGH);
	FAN_STATE = 0;
}
void turnLedOn(){
	digitalWrite(LED_PIN, HIGH);
	LED_STATE = 1;
}
void turnLedOff(){
	digitalWrite(LED_PIN, LOW);
	LED_STATE = 0;
}

void turnLedRedOn(){
	digitalWrite(LED_RED_PIN, HIGH);
	LED_RED_STATE = 1;
}

void turnLedRedOff(){
	digitalWrite(LED_RED_PIN, LOW);
	LED_RED_STATE = 0;
}

bool anyMove(){
	return digitalRead(MOTIONDETECTOR_PIN);
}

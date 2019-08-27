#include "Game.h"

Game::Game() {

  isRequestingExit = false;
  resetGameTime();

  // time
  hours = 0;
  minutes = 0;
  seconds = 0;
  secondsChanged = false;
  timeSyncLastFrameTime = lastFrameTime;
  isSyncInternetTime = false;

  // frame rate
  frameRate = 0;
  frameCount = 0l;
  memset(&fpsDigit, 0, sizeof(fpsDigit));

  // button
  isGamepadEnabled = false;
  buttonDelay = 0.0f;
  isButtonPressed[numberOfButtons] = {false};
  //pressedButton = BUTTON_NONE;
  isButtonAllowed = true;
 


  // sound
  soundDuration = 0.0f;
  nextSound = 0;
  nextSoundDelay = 0.0f;
  volume = 1.0f; // 0.0 to 1.0
  isVolumeChanged = 0.0f;
  isSoundInterruptEnabled = true;
  isSoundEnabled = true;
}
void Game::writeHighScore() {
	//TODO:
}
long unsigned int Game::readHighScore() {
	return 100;
}
void Game::decreaseVolume() {
  volume = constrain(volume - (elapsedTime / 1000.0f), 0.0f, 1.0f);
  isVolumeChanged = 1000.0f;
}

void Game::drawBitmap(int x, int y, int width, int height, const uint8_t* bitmap, const uint8_t* mask, int color) {
  if (mask) {
    espert->oled.setColor(1 - color);
    espert->oled.drawBitmap(x, y, width, height, mask, false);
  }

  espert->oled.setColor(color);
  espert->oled.drawBitmap(x, y, width, height, bitmap, false);
}

String Game::floatToString(float value, int length, int decimalPalces) {
  String stringValue = String(value, decimalPalces);
  String prefix = "";

  for (int i = 0; i < length - stringValue.length(); i++) {
    prefix += " ";
  }

  return prefix + stringValue;
}

void Game::increaseVolume() {
  volume = constrain(volume + (elapsedTime / 1000.0f), 0.0f, 1.0f);
  isVolumeChanged = 1000.0f;
}


void Game::initGame() {
}



void Game::init(ESPert* e, bool menu) {
  isMenuEnabled = menu;
  espert = e;
  randomSeed(analogRead(0));
  initGame();
  resetGameTime();
  timeSyncLastFrameTime = lastFrameTime;
}

bool Game::isBackToMenuEnabled() {
  return false;
}

bool Game::isBlink(float factor) {
  int speed = maxBlinkTime * factor;
  return ((int)blinkTime % speed < speed * 0.5f) ? true : false;
}

bool Game::isExit() {
  return isRequestingExit;
}

float Game::lerp(float t, float v0, float v1) {
  v0 = (1.0f - t) * v0 + t * v1;

  if (fabs(v1 - v0) < 1.0f) {
    v0 = v1;
  }

  return v0;
}

String Game::longToString(unsigned long value, int length, String prefixChar) {
  String stringValue = String(value);
  String prefix = "";

  for (int i = 0; i < length - stringValue.length(); i++) {
    prefix += prefixChar;
  }

  return prefix + stringValue;
}

int Game::median(int arr[], int maxValues) {
  quickSort(arr, 0, maxValues - 1);
  return arr[maxValues / 2];
}

int Game::partition(int* arr, const int left, const int right) {
  const int mid = left + (right - left) / 2;
  const int pivot = arr[mid];

  // move the mid point value to the front.
  swap(arr[mid], arr[left]);
  int i = left + 1;
  int j = right;

  while (i <= j) {
    while (i <= j && arr[i] <= pivot) {
      i++;
    }

    while (i <= j && arr[j] > pivot) {
      j--;
    }

    if (i < j) {
      swap(arr[i], arr[j]);
    }
  }

  swap(arr[i - 1], arr[left]);
  return i - 1;
}

void Game::playSound(int index) {
}

void Game::pressButton() {
}

void Game::quickSort(int* arr, const int left, const int right) {
  if (left >= right) {
    return;
  }

  int part = partition(arr, left, right);

  quickSort(arr, left, part - 1);
  quickSort(arr, part + 1, right);
}


void Game::render() {
}


void Game::renderFPS(int x, int y, int bitmapWidth, int bitmapHeight, int gap, const uint8_t* numberBitmap, const uint8_t* numberMaskBitmap, int color, int fpsBitmapX, int fpsBitmapY) {
  if (isFPSVisibled && numberBitmap) {
    if (fpsBitmapX > -1 && fpsBitmapY > -1) {
      drawBitmap(fpsBitmapX, fpsBitmapY, 16, 8, fpsBitmap, fpsMaskBitmap, color);
    }

    for (int i = 0; i < 2 && (fpsDigit[i] >= 0 && fpsDigit[i] <= 9); i++) {
      drawBitmap(x + (i * (bitmapWidth + gap)), y, bitmapWidth, bitmapHeight, numberBitmap + (fpsDigit[i] * bitmapWidth), numberMaskBitmap ? numberMaskBitmap + (fpsDigit[i] * bitmapWidth) : NULL, color);
    }
  }
}

void Game::renderVolume(int x, int y, int color) {
  if (isSoundEnabled) {
    int i = (int)round(volume * (numberOfVolumeFrames - 1));

    if (i == 0 && volume > 0.0f) {
      i = 1;
    }

    drawBitmap(x, y, 16, 8, volumeBitmap[i], NULL, color);
  }
}

void Game::renderMakerAsia(int x, int y, int color) {
  drawBitmap(x, y, 64, 16, makerAsiaBitmap, makerAsiaMaskBitmap, color);
}

void Game::update() {
}

void Game::resetGameTime() {
  lastFrameTime = millis();
  elapsedTime = 0.0f;
  blinkTime = 0.0f;
  fpsLastFrameTime = lastFrameTime;
}

void Game::swap(int &a, int &b) {
  int t = a;
  a = b;
  b = t;
}

void Game::toggleVolume() {
  if (volume == 0.0f) {
    volume = 1.0f;
  } else {
    volume = 0.0f;
  }

  isVolumeChanged = 1000.0f;
}

void Game::updateGameTime(bool updateButton) {
	// game time
	unsigned long frameTime = millis();
	elapsedTime = frameTime - lastFrameTime;
	lastFrameTime = frameTime;


	// frame rate
	frameCount++;
	if (frameTime - fpsLastFrameTime >= 1000l) {
		if (frameRate != frameCount) {
			frameRate = frameCount;
			String fpsString = longToString(constrain(frameRate, 0, 99), 2, "0");

			for (int i = 0; i < 2; i++) {
				fpsDigit[i] = fpsString.charAt(i) - '0';
			}
		}

		frameCount = 0l;
		fpsLastFrameTime = frameTime;
	}

	// blink
	blinkTime += elapsedTime;
	if (blinkTime >= maxBlinkTime) {
		blinkTime = 0.0f;
	}

	// button
	if (updateButton) {
		buttonDelay += elapsedTime;
		if (buttonDelay >= maxButtonDelay) {
			buttonDelay = 0.0f;
			pressButton();
		}
	}




}
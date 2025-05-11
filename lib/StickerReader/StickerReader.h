#ifndef STICKER_READER_H
#define STICKER_READER_H
#include <FastLED.h>
#include <avr/pgmspace.h>

struct StickerSignature {
    const char* label;
    int values[16]; // [Red, Green, Blue, IR]
};

struct FenMapping {
    const char* label;
    const char* piece; // [Red, Green, Blue, IR]
};

enum PieceType {
    PIECE_NONE,
    PIECE_WHITE,
    PIECE_BLACK
};

class StickerReader {
public:
    StickerReader();
    
    void begin();
    void configure(int controlA0, int controlA1, int controlA2, int controlA3, int sensorNumber, CRGB &led);
    void readSignature(int* out);
    const char* identifySticker();
    PieceType getPieceType() const;
    const char* getFENFromLabel(const char* label);
    
    // New methods for ambient light detection
    int readAmbient();
    int checkAmbientChange(int threshold = 6); // Returns 1 (placed), -1 (removed), 0 (no change)

    // Getters for control bits
    int getControlA0() const { return _controlA0; }
    int getControlA1() const { return _controlA1; }
    int getControlA2() const { return _controlA2; }
    int getControlA3() const { return _controlA3; }

private:
    int _sensorNumber;
    int _controlA0, _controlA1, _controlA2, _controlA3;
    CRGB* _led;  // Changed to a pointer
    PieceType _pieceType;
    int _lastAmbient; // To track previous ambient reading
    unsigned long _lastAmbientTime; // To track when we last measured ambient
    int _currentSignature[4];  // Store the current signature

    int distance(int* a, const int* b);
};

#endif

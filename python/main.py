import board
import audiomp3
import audiopwmio
import time
import analogio
import audiocore
import audiomixer
audio = audiopwmio.PWMAudioOut(board.GP0)

mp3files = ["FOOD.mp3", "MEDICINE.mp3","WATER.mp3","WASHROOM.mp3"]
FOOD = analogio.AnalogIn(board.A1)
WATER = analogio.AnalogIn(board.A2)
WASHROOM = analogio.AnalogIn(board.A3)

mp3 = open(mp3files[0], "rb")
decoder = audiomp3.MP3Decoder(mp3)

while True:
    
    print(FOOD.value)
    print(WATER.value)
    print(WASHROOM.value)
for filename in mp3files:
    decoder.file = open(filename, "rb")
    audio.play(decoder)
    print("Playing:", filename)
    while audio.playing:
        pass

print("Done playing!")

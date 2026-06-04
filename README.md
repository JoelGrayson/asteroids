# Asteroids

A recreation of the 1979 Atari asteroids from bare-metal programming. You can purchase a unit at
[joelgrayson.com/asteroids](https://joelgrayson.com/asteroids)



## Developer Notes
* DON'T TOUCH PB4, PB5, PB6! They are being used for audio!
* init is called once for every boot of the mango pi
* setup is called once for every game. When the user starts a new game, it is called again
* loop is called every frame

### Tested Features
* bullets correctly deallocates


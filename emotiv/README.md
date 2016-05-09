# 1 Instalation

1. Clone here emotiv community sdk from `https://github.com/Emotiv/community-sdk`
2. Instal `wiringPi` from `http://wiringpi.com/`
3. Run `ldconfig -n community-sdk/bin/armhf` to make it linkable for linker
4. `make`


# 2 Run

Emotiv executable require 0 or 1 argument.

* If 0 then it will try to connect to emotiv headset through bluetooth.
* If 1 then it is string with IP for EmoComposer (ex.: "127.0.0.1")


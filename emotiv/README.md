Instalation

1) Clone here emotiv community sdk from github.com/emotiv/community-sdk
2) Run ldconfig -n on community-sdk/bin/armhf to make it linkable for linker
3) make


Run

emotiv executable require 0 or 2 arguments.

If 0 then it will try to connect to emotiv headset through bluetooth.
If 2 then:
	First one is a 0 or 1 for "Use EmoComposer" (0 no, 1 yes)
	Second one is string with IP for this composer (ex.: "127.0.0.1")

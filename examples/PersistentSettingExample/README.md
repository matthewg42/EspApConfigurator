This directory contains a bunch of tests and development code for the settings classes.

They're a bit ugly syntax - template classes with function pointers and the like, but it seems worth it.

Ultimately this may migrate into the Mutila library, although it will have to also be peppered with #ifdefs because the ESP EEPROM library works differently than the stock Arduino one (what call it the same damn thing?!)


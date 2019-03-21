# Compressors
## LocalMap
### Run
~~~~bash
# generate tokens
./tonkenizer ipfile tokens

# encode
./encoding tokens encodedopfile dictionary

# decode
./decoding encodedopfile opfile dictionary

# dictionary is list of words in language separated by \n

# Verify
diff ipfile opfile
~~~~

## LZW
### Run
~~~~bash
# encode
./encoding ipfile encodedopfile codebook

# decode
./decoding encodedopfile opfile codebook

# Verify
diff ipfile opfile
~~~~


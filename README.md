# Spray Paint

Spraypaint is a compression/decompression tool that uses huffman codes as a base
for encoding and decoding data.

```
Usage: ./spray_paint <flag> <filename> <output>

spray_paint is a file compression and decompression tool.

Arguments:
  <flag>       d or c for [d]ecompress or [c]ompress.
  <filename>   The name of the file to compress or decompress.
  <output>     The name of the output file.

Examples:
  ./spray_paint c example.txt example.spz
  ./spray_paint d example.spz example.txt
```

To build the project from source (using ninja, I've only tested with ninja):

```
git clone git@github.com:devanbenz/spray-paint.git && cd spray-paint

mkdir -p cmake-build-debug && cd cmake-build-debug

cmake .. -G "Ninja" -DCMAKE_BUILD_TYPE=Debug

ninja
```

## File Structure

Compressed SprayPaint file's contain the following structure as binary data:

```
   56 bytes          57...n bytes          1 byte   
┌──────────────┬───────────────────────┬───────────┐
│              │                       │           │
│   Decoder    │        Data           │  Padding  │
│    Tree      │                       │           │
└──────────────┴───────────────────────┴───────────┘
```

`Decode Tree` is a huffman code tree where each leaf node is a value from the source data. 

`Data` is the data in raw bits encoded by the decoder tree.

`Padding` since data can only be stored in byte format on disk (8 bits) sometimes we will have a number of bits
encoded that are not divisible by 8. This final byte stored the amount of padded 0's for our final byte of data.
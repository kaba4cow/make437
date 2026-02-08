# Make437

A utility to generate PNG glyph atlases for the [IBM437](https://en.wikipedia.org/wiki/Code_page_437) codepage from a given font.

Developed for educational purposes and personal use.

## Usage

Build from source:

```shell
bash ./build.sh
```

Run with args:

```shell
./build/make437 ./my-font.ttf ./output.png 8
```

- `./my-font.ttf` - your font's filename
- `./output.png` - export destination filename
- `8` - glyphs pixel height

## License

This project is licensed under the **MIT License** - see the [LICENSE](LICENSE) file for details.
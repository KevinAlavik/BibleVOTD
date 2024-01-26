# BibleVOTD - Verse of the Day Fetcher

BibleVOTD is a simple C program that fetches the Verse of the Day from the Bible using the "https://labs.bible.org/api/" API. The retrieved verse is then displayed in a stylized format on the console.

## Dependencies

- [libcurl](https://curl.se/libcurl/): Library for making HTTP requests

## Usage

Clone the repository:

```bash
git clone https://github.com/KevinAlavik/BibleVOTD.git
cd BibleVOTD
```

Compile and run the program:

```bash
make
./bin/bv
```

This will fetch the Verse of the Day and display it in a stylized format.

## Installation (Optional)

To install the program system-wide (requires sudo):

```bash
sudo make install
```

After installation, you can run the program using:

```bash
bv
```

## Cleanup

To remove the compiled files:

```bash
make clean
```

## Preview
![BibleVOTD Preview](https://raw.githubusercontent.com/KevinAlavik/BibleVOTD/main/preview.png)

Feel free to contribute or customize the program as needed!